# Out-of-Tree XIAO RP2350 Potentiometer LED Bar Graph (xiao-rp2350-bargraph)

*   **Author:** Brett Morgan
*   **Status:** Draft
*   **Date:** 2026-11-13

--------------------------------------------------------------------------------

## Overview

`xiao-rp2350-bargraph` is an out-of-tree Apache NuttX firmware project targeting a Seeed Studio XIAO RP2350 mounted on a solderless breadboard with one analog potentiometer (outer terminals connected to `3.3 V` and `GND`, wiper on `D0`) and six discrete active-high LEDs (`D3–D5`, `D8–D10`). The firmware samples the potentiometer at 20 Hz (every 50 ms) and drives 0 to 6 LEDs proportionally as a bar graph, launching automatically at power-on without an attached host computer while exposing an interactive NuttShell (`nsh>`) console over USB-C (CDC/ACM) when plugged into a host.

All custom board files, kernel configuration (`defconfig`), ROMFS startup scripts (`rcS`), user-space daemon source files, and GNU Make build automation live inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`, leaving tracked files in `nuttx/` and `apps/` unmodified.

## The Problem

Building a hardware-backed application on the XIAO RP2350 in Apache NuttX without modifying upstream submodules exposes four conflicts between the board's pinout, the RP2350 ADC driver (`rp23xx_adc.c`), the NuttShell USB console startup sequence, and the out-of-tree build system:

1.  **ADC Pin Clobbering and Analog Pad Input-Buffer Leakage (`GPIO 26–29`)**: During driver registration (`adc_register()`), `my_reset()` in `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` loops over all four entries in `g_gpio_map` (`GPIO 26, 27, 28, 29`) regardless of which `CONFIG_RP23XX_ADC_CHANNEL*` options are enabled. It reconfigures all four pins as floating inputs (`RP23XX_GPIO_FUNC_NULL`), overwriting the pin configuration of any LEDs wired to `D1` (`GPIO 27`) or `D2` (`GPIO 28`), and re-enables the digital input buffer (`RP23XX_PADS_BANK0_GPIO_IE`) that `rp23xx_common_earlyinitialize()` cleared for analog operation. Leaving `IE=1` on `GPIO 26–29` introduces digital input-buffer leakage on the potentiometer wiper (`GPIO 26`) and leaves unconnected pins `GPIO 27–29` floating with `IE=1`.
2.  **Unthrottled Free-Running ADC FIFO Interrupts and Stale Ring-Buffer Samples**: `rp23xx_adc.c` rejects `ANIOC_TRIGGER` (`-ENOTTY`), sets no ADC clock divider, and re-arms a new conversion inside its FIFO interrupt handler as long as `/dev/adc0` remains open. Meanwhile, the upper-half ring buffer (`nuttx/drivers/analog/adc.c`, `CONFIG_ADC_FIFOSIZE=8`) drops new samples once full rather than overwriting old ones. A task that keeps `/dev/adc0` open and sleeps for 50 ms leaves the ADC FIFO interrupt firing continuously and reads stale samples from the previous wake cycle.
3.  **Headless Boot vs. Blocking USB-C Console Initialization**: Upstream `xiao-rp2350/configs/usbnsh/defconfig` omits `CONFIG_BOARD_LATE_INITIALIZE=y`, which prevents `rp23xx_bringup()` from registering `/dev/adc0` and `/dev/userleds` at boot. In `apps/system/nsh/nsh_main.c`, `nsh_initialize()` executes `/etc/init.d/rcS` before `nsh_consolemain()` connects USB CDC/ACM and blocks waiting for three Enter keystrokes on `/dev/ttyACM0`. Any foreground task in `rcS` blocks USB console enumeration indefinitely, and because `# CONFIG_DEV_CONSOLE is not set` with `CONFIG_DEV_NULL=y`, `group_setupidlefiles()` binds boot-time FDs `0, 1, 2` to `/dev/null`, discarding `rcS` stdout/stderr.
4.  **Single-LED Upstream Board Support and Out-of-Tree Build Wiring**: Upstream `xiao-rp2350/src/rp23xx_userleds.c` hardcodes a single onboard LED (`GPIO_LED1` / `BOARD_LED1_BIT`, `GPIO 25`) in `board_userled_all()` rather than iterating over an array of LEDs, and `CONFIG_ARCH_LEDS` defaults to `y` when `ARCH_HAVE_LEDS` is selected, compiling `rp23xx_autoleds.c` instead of `rp23xx_userleds.c`. In addition, `xiao-rp2350/src/CMakeLists.txt` leaves `CONFIG_ETC_ROMFS` unimplemented (`# TODO`).

## Goals

*   Keep all project source files, custom board files, startup scripts, and build wrappers isolated in `xiao-rp2350-bargraph/` with zero modifications to tracked files in `nuttx/` or `apps/`.
*   Register `/dev/adc0` on `D0` (`GPIO 26` / `ADC0`) and clear digital pad input-enable (`IE=0`) across `GPIO 26–29` after `rp23xx_common_bringup()` completes.
*   Expose 6 active-high breadboard LEDs on `D3–D5` (`GPIO 5–7`) and `D8–D10` (`GPIO 2, 4, 3`) as a single bitmask device through `/dev/userleds`.
*   Sample `/dev/adc0` at 20 Hz using per-tick `open` -> `read` -> `close` cycles so the RP2350 ADC FIFO interrupt stops between ticks and each tick reads a fresh conversion.
*   Filter ADC FIFO conversion errors, map valid readings across 7 discrete levels (`0..6`) with hysteresis, and update `/dev/userleds` only when the displayed level changes.
*   Launch a singleton `bargraph` background daemon at boot from ROMFS `/etc/init.d/rcS` without blocking USB CDC/ACM console enumeration (`nsh>`), supporting clean `SIGTERM` shutdown and restart from NSH.
*   Log boot and runtime errors to RAMLOG (`dmesg`), and blink an alternating fault pattern (`0x15` / `0x2A`) on `/dev/userleds` after repeated `/dev/adc0` failures when running headless.

## Non-Goals

*   Supporting boards other than the Seeed Studio XIAO RP2350 in this project.
*   Editing tracked files inside `nuttx/` or `apps/` (only the gitignored `apps/external` symlink and standard build outputs are created during compilation).
*   Supporting CMake builds (`xiao-rp2350/src/CMakeLists.txt` does not implement `CONFIG_ETC_ROMFS`).
*   Adding PWM brightness dimming, hardware DMA pacing for the ADC, persistent flash calibration, or networking.

## Open Questions

*   None.

## Critical User Journeys (CUJs)

### CUJ 1: Standalone Power-Up and Potentiometer Tracking

*   **Given**: The XIAO RP2350 is flashed with `nuttx.uf2`, wired on a breadboard with a potentiometer across `3.3 V` and `GND` (wiper on `D0` / `GPIO 26`) and 6 active-high LEDs with series resistors on `D3`, `D4`, `D5`, `D8`, `D9`, and `D10`, and powered from a 5 V USB power supply with no serial terminal open.
*   **When**: The board powers on and the user rotates the potentiometer wiper from `0 V` (`GND`) to `3.3 V` and back.
*   **Then**: `board_late_initialize()` registers `/dev/adc0` and `/dev/userleds`, `/etc/init.d/rcS` spawns the `bargraph` daemon in the background, and the 6 LEDs step monotonically from 0 lit (`0x00`) to 6 lit (`0x3f`) at 20 Hz with no boundary flicker while input noise stays within the `±24`-count hysteresis band, and with ADC conversions halted between 50 ms ticks once the trailing shutdown ISR detaches the ADC IRQ.

### CUJ 2: Interactive Inspection and Control via USB-C NuttShell

*   **Given**: The board is already running the bar-graph daemon from power-up.
*   **When**: The user connects the XIAO RP2350 USB-C port to a Linux host, opens `/dev/ttyACM0`, presses Enter three times, and runs `dmesg`, `ps`, `bargraph`, `kill -15 <pid>`, and `bargraph`.
*   **Then**: `nsh>` appears without interrupting the running bar graph; `dmesg` displays the RAMLOG boot and daemon startup messages; running `bargraph` while it is already active prints an `"already running"` notice and exits without spawning a duplicate task; sending `SIGTERM` stops the daemon loop, turns off all 6 LEDs, closes `/dev/userleds`, and exits; running `bargraph` again restarts the daemon without holding `/dev/ttyACM0` open.

### CUJ 3: Headless Hardware or Driver Fault Indication

*   **Given**: The board boots without a USB host attached, and `/dev/adc0` fails to open or returns `>= 5` consecutive read errors (excluding `EINTR` and transient `am_data < 0` samples).
*   **When**: The `bargraph` daemon detects `>= 5` consecutive hard failures on `/dev/adc0` (while `/dev/userleds` is open).
*   **Then**: The daemon logs the error code to `syslog(LOG_ERR, ...)` (persisted in RAMLOG for later `dmesg` inspection) and enters a fault loop that alternates `/dev/userleds` between `0x15` (`0b010101`) and `0x2A` (`0b101010`) at 250 ms intervals until terminated via `SIGTERM`.

## Technical Plan (Architecture)

### Out-of-Tree Project Layout

All custom board files, kernel configuration, ROMFS startup scripts, external application code, and build automation reside inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`:

```text
/home/brett/Documents/GitHub/nuttxspace/
├── nuttx/                             (unchanged upstream submodule)
├── apps/
│   └── external -> ../xiao-rp2350-bargraph/external   (gitignored symlink)
└── xiao-rp2350-bargraph/
    ├── Makefile                       (top-level symlink, configure, and build wrapper)
    ├── board/                         (CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board")
    │   ├── Kconfig
    │   ├── configs/bargraph/defconfig
    │   ├── include/board.h
    │   ├── scripts/Make.defs
    │   └── src/
    │       ├── Make.defs
    │       ├── rp23xx_pico.h
    │       ├── rp23xx_boardinitialize.c
    │       ├── rp23xx_bringup.c
    │       ├── rp23xx_userleds.c
    │       └── etc/init.d/
    │           ├── rc.sysinit
    │           └── rcS
    └── external/                      (mounted into apps build via apps/external; Kconfig auto-generated by Directory.mk)
        ├── Makefile
        ├── Make.defs
        └── bargraph/
            ├── Makefile
            ├── Make.defs
            ├── Kconfig
            └── bargraph_main.c
```

Three NuttX build-system rules govern this layout:

1.  **Shared RP23XX Common Directory (`BOARD_COMMON_DIR`)**: In `nuttx/tools/Config.mk`, if `$(BOARD_DIR)/../common` exists, the build system binds `BOARD_COMMON_DIR` to that sibling directory instead of `nuttx/boards/arm/rp23xx/common`. Therefore, `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph/common/` MUST NOT exist. Keeping `common/` absent lets `CONFIG_ARCH_BOARD_COMMON=y` resolve directly to `nuttx/boards/arm/rp23xx/common` for `rp23xx_common_earlyinitialize()`, `rp23xx_common_initialize()`, `rp23xx_common_bringup()`, `rp23xx_reset.c`, and `rp23xx_boardctl.c`.
2.  **Relative Board Path Resolution**: `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"` resolves relative to `nuttx/` (the working directory from which `Makefile` invokes `./tools/configure.sh` and `$(MAKE) -C ../nuttx`).
3.  **Linker Script Reuse**: `board/scripts/Make.defs` sets `ARCHSCRIPT` to `$(TOPDIR)$(DELIM)boards$(DELIM)arm$(DELIM)rp23xx$(DELIM)xiao-rp2350$(DELIM)scripts$(DELIM)$(LDSCRIPT)` (`memmap_default.ld`) rather than copying the upstream RP2350 linker scripts out of tree.

### Hardware Pin Mapping

| XIAO Pin | RP2350 GPIO | Function / Polarity | NuttX Device | Notes |
| :--- | :--- | :--- | :--- | :--- |
| `D0` | `GPIO 26` | Analog Input (`ADC0`) | `/dev/adc0` | Potentiometer wiper (`0–3.3 V`; outer pins on `3.3 V` & `GND`); `IE` cleared after `rp23xx_common_bringup()` |
| `D1` | `GPIO 27` | Unassigned | — | Avoided (`rp23xx_adc.c` `my_reset()` resets to `FUNC_NULL`); `IE` cleared after `rp23xx_common_bringup()` |
| `D2` | `GPIO 28` | Unassigned | — | Avoided (`rp23xx_adc.c` `my_reset()` resets to `FUNC_NULL`); `IE` cleared after `rp23xx_common_bringup()` |
| `D3` | `GPIO 5` | Output, Active-High (`LED 1`) | `/dev/userleds` bit `0` (`0x01`) | `>= 330 Ω` series resistor to GND |
| `D4` | `GPIO 6` | Output, Active-High (`LED 2`) | `/dev/userleds` bit `1` (`0x02`) | `>= 330 Ω` series resistor to GND |
| `D5` | `GPIO 7` | Output, Active-High (`LED 3`) | `/dev/userleds` bit `2` (`0x04`) | `>= 330 Ω` series resistor to GND |
| `D6` | `GPIO 0` | Unassigned (`UART0 TX`) | — | Left free (`# CONFIG_RP23XX_UART0 is not set`) |
| `D7` | `GPIO 1` | Unassigned (`UART0 RX`) | — | Left free (`# CONFIG_RP23XX_UART0 is not set`) |
| `D8` | `GPIO 2` | Output, Active-High (`LED 4`) | `/dev/userleds` bit `3` (`0x08`) | `>= 330 Ω` series resistor to GND |
| `D9` | `GPIO 4` | Output, Active-High (`LED 5`) | `/dev/userleds` bit `4` (`0x10`) | `>= 330 Ω` series resistor to GND |
| `D10` | `GPIO 3` | Output, Active-High (`LED 6`) | `/dev/userleds` bit `5` (`0x20`) | `>= 330 Ω` series resistor to GND |
| Onboard | `GPIO 25` | Boot-initialized output | — | Driven high (`true`, active-low LED off) by `rp23xx_boardearlyinitialize()`, matching upstream `xiao-rp2350`; not part of `/dev/userleds` |

### System Data Flow and Boot Sequence

```text
+-----------------------------------------------------------------------------+
| 1. Kernel Boot & Board Bringup                                              |
|    rp23xx_boardearlyinitialize() -> rp23xx_common_earlyinitialize()         |
|    board_late_initialize()       -> rp23xx_bringup()                        |
|      1) rp23xx_common_bringup()  -> rp23xx_adc_setup("/dev/adc0", ADC0)     |
|                                     (adc_register -> my_reset sets IE=1)    |
|      2) clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, RP23XX_PADS_BANK0_GPIO(26..29))
|      3) userled_lower_initialize("/dev/userleds")          [6 outputs low]  |
+-----------------------------------------------------------------------------+
                                      |
                                      v
+-----------------------------------------------------------------------------+
| 2. NSH Initialization & Autonomous Background Launch                        |
|    nsh_main() -> nsh_initialize()                                           |
|      -> mounts ROMFS (/etc) -> executes /etc/init.d/rcS                     |
|      -> runs "bargraph" -> task_create("bargraph_daemon", ...) returns 0    |
|    nsh_consolemain()                                                        |
|      -> BOARDIOC_USBDEV_CONNECT -> blocks waiting for 3x Enter on /dev/ttyACM0
+-----------------------------------------------------------------------------+
                                      |
                                      v  (runs concurrently at 20 Hz)
+-----------------------------------------------------------------------------+
| 3. bargraph_daemon 20 Hz Control Loop (stdio redirected to /dev/null)       |
|    open("/dev/adc0", O_RDONLY) -> read(5-byte struct adc_msg_s) -> close()  |
|      |                                                                      |
|      +--> [am_data < 0 (FIFO error)] --> discard sample, sleep 50 ms        |
|      +--> [valid] --> raw = am_data >> 19 (0..4095)                         |
|                   --> quantize(raw, current_level, HYSTERESIS=24) -> 0..6   |
|                   --> if level changed: ioctl(led_fd, ULEDIOC_SETALL, mask) |
|    usleep(50000)  [trailing ADC completion ISR detaches IRQ once count==0]  |
+-----------------------------------------------------------------------------+
```

### Detailed Subsystem Mechanisms

#### Board Bringup and Analog Pad Input-Buffer Fix

*   **Pad `IE` Leakage Mitigation**: `rp23xx_common_earlyinitialize()` clears `RP23XX_PADS_BANK0_GPIO_IE` on `GPIO 26–29`, but `rp23xx_common_bringup()` immediately calls `rp23xx_adc_setup("/dev/adc0", true, false, false, false, false)` -> `adc_register()`. During registration, `my_reset()` in `rp23xx_adc.c` calls `rp23xx_gpio_set_function(pin, RP23XX_GPIO_FUNC_NULL)` for all four pins (`26–29`), re-enabling `IE=1`.
*   **Bringup Cleanup**: Because `my_reset()` runs only at registration time, `rp23xx_bringup.c` loops over `GPIO 26..29` immediately after `rp23xx_common_bringup()` and calls `clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, RP23XX_PADS_BANK0_GPIO(gpio))` (where `clrbits_reg32(clearbits, addr)` in `arm_internal.h` clears bitmask `clearbits` at 32-bit register address `addr`). `CONFIG_DEV_GPIO` is disabled and `GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1` are omitted from `board.h` so no GPIO character driver reconfigures `GPIO 26–28`.

#### 6-LED `/dev/userleds` Lower-Half

*   **Driver Selection**: With `# CONFIG_ARCH_LEDS is not set`, `CONFIG_BOARD_CUSTOM_LEDS=y` (`ARCH_HAVE_LEDS`), `CONFIG_USERLED=y`, and `CONFIG_USERLED_LOWER=y`, the board builds `rp23xx_userleds.c` instead of `rp23xx_autoleds.c`.
*   **Pin Mapping and Control**: `board_userled_initialize()` initializes all six pins in `g_ledcfg[6]` (`GPIO 5, 6, 7, 2, 4, 3`) as outputs driven low (`false`) and returns `BOARD_NLEDS` (`6`). `board_userled(int led, bool ledon)` updates index `0..5`, and `board_userled_all(uint32_t ledset)` loops across `0 <= i < BOARD_NLEDS`, writing `(ledset & (1U << i)) != 0` to `g_ledcfg[i]`.

#### ADC Sampling, Normalization, and Hysteresis

*   **Per-Tick `open` / `read` / `close` Cycle**: Opening `/dev/adc0` triggers `my_rxint(dev, true)` -> `add_device()`, which enables the RP2350 ADC FIFO, attaches `RP23XX_ADC_IRQ_FIFO`, and starts conversions. Reading `sizeof(struct adc_msg_s)` (5 bytes: 1-byte `am_channel` + 4-byte `int32_t am_data`) matches the `buflen % 5 == 0` record branch in `nuttx/drivers/analog/adc.c` and collects one fresh conversion. Closing `/dev/adc0` triggers `my_shutdown()` -> `remove_device()`, decrementing `g_active_count` to `0` so the next trailing FIFO completion interrupt (`interrupt_handler()` in `rp23xx_adc.c`) sees `g_active_count == 0`, disables `RP23XX_ADC_CS` and `RP23XX_ADC_INTE`, and detaches `RP23XX_ADC_IRQ_FIFO` for the remainder of the 50 ms sleep window.
*   **Error Filtering and Scaling**: `rp23xx_adc.c` shifts the 12-bit hardware result left by 19 bits (`value <<= 19`, range `0..0x7ff80000`) and negates `value` when `RP23XX_ADC_FIFO_ERR` is set. Any reading with `sample.am_data < 0` is discarded without counting as a hard device fault. Valid readings normalize to `uint16_t raw = (uint16_t)(sample.am_data >> 19)` (`0..4095`).
*   **7-Level Hysteresis Quantizer**: Full scale (`0..4095`) is divided into 7 buckets (`level` `0..6`) with nominal thresholds at multiples of `585` (`585, 1170, 1755, 2340, 2925, 3510`) and a hysteresis band `HYSTERESIS = 24` counts (~0.6% of full scale). Stepping upward from `level` `k` to `k + 1` requires `raw >= threshold[k] + HYSTERESIS`, and stepping downward from `k` to `k - 1` requires `raw <= threshold[k - 1] - HYSTERESIS`. The resulting LED bitmask is `mask = (level == 0) ? 0 : ((1U << level) - 1U)` (`0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f`), written via `ioctl(led_fd, ULEDIOC_SETALL, (unsigned long)mask)` only when `level` changes.

#### Daemon Lifecycle, ROMFS Startup, and Headless Diagnostics

*   **Flat Address-Space Singleton**: NuttX executes built-in tasks inside a single flat address space, and `apps/Application.mk` maps `main()` in `bargraph_main.c` to the built-in `bargraph` command. `bargraph_main.c` uses `static volatile sig_atomic_t g_bargraph_running` and `g_bargraph_stop` so repeated CLI invocations print `"bargraph: daemon already running\n"` and exit `0` without spawning duplicates. Otherwise `main()` spawns `bargraph_daemon` via `task_create()` and returns immediately so `/etc/init.d/rcS` completes before `nsh_consolemain()` runs USB enumeration.
*   **Stdio Isolation and `SIGTERM` Lifecycle**: At startup, `bargraph_daemon` redirects FDs `0, 1, 2` to `/dev/null` (`dup2()`) so interactive restarts from `nsh>` do not inherit and hold `/dev/ttyACM0` open. Its `SIGTERM` handler (`SA_SIGINFO`, no `SA_RESTART`) sets `g_bargraph_stop = 1`, breaking either the sampling loop or the fault loop cleanly, clearing all LEDs (`ULEDIOC_SETALL(0UL)`), closing `/dev/userleds`, and resetting `g_bargraph_running = 0` on every exit path (including `/dev/userleds` open/ioctl failure).
*   **ROMFS Preprocessing**: Because `Board.mk` preprocesses `RCSRCS` with `cpp`, `etc/init.d/rcS` uses `#include <nuttx/config.h>`, C `/* ... */` comments, and `#ifdef CONFIG_BARGRAPH` around the `bargraph` command.
*   **RAMLOG Diagnostics and Fault Loop**: Startup (`task_create()` failure) and runtime errors log via `syslog(LOG_ERR, ...)` to RAMLOG (`CONFIG_RAMLOG_SYSLOG=y`, `CONFIG_RAMLOG_BUFSIZE=4096`), readable via `dmesg` once USB NSH attaches. If `/dev/adc0` fails to open or returns `>= 5` consecutive hard read errors (excluding `EINTR` and transient `am_data < 0`), the daemon logs the error and alternates `ULEDIOC_SETALL` between `0x15` and `0x2A` every 250 ms until `SIGTERM` arrives.

## Alternatives Considered

### Alternative A: Modifying `nuttx/boards/arm/rp23xx/xiao-rp2350` In-Place

*   **Pros**: Avoids creating out-of-tree `Kconfig`, `scripts/Make.defs`, and `rp23xx_boardinitialize.c` files.
*   **Reason for Rejection**: Dirties the `nuttx` git submodule and mixes breadboard-specific pin assignments into upstream board files instead of isolating the project under `xiao-rp2350-bargraph/` (`CONFIG_ARCH_BOARD_CUSTOM=y`).

### Alternative B: Driving 8 Breadboard LEDs (Using `D1`/`D2` `GPIO 27–28` or `D6`/`D7` `GPIO 0–1`)

*   **Pros**: Provides 8 bar-graph steps instead of 6.
*   **Reason for Rejection**: `rp23xx_adc.c` `my_reset()` resets `GPIO 26–29` to floating inputs during `adc_register()`, clobbering `D1` (`GPIO 27`) and `D2` (`GPIO 28`), while claiming `D6`/`D7` (`GPIO 0`/`1`) occupies the hardware `UART0` pins. Six LEDs on `D3–D5` and `D8–D10` satisfy the 0–6 bar-graph requirement without touching `GPIO 27–28` or `UART0`.

### Alternative C: Keeping `/dev/adc0` Continuously Open with `ANIOC_RESET_FIFO`

*   **Pros**: Eliminates per-tick `open()` and `close()` syscalls every 50 ms.
*   **Reason for Rejection**: `rp23xx_adc.c` runs free-running conversions whenever `/dev/adc0` is open (`my_ioctl()` returns `-ENOTTY` for `ANIOC_TRIGGER` and has no clock divider), burning CPU cycles during `usleep(50000)` and overflowing the 8-entry upper-half FIFO (`CONFIG_ADC_FIFOSIZE=8`). Per-tick `open`/`read`/`close` stops conversions between ticks.

### Alternative D: Individual `/dev/gpout*` Character Devices (`CONFIG_DEV_GPIO=y`)

*   **Pros**: Avoids writing a custom `rp23xx_userleds.c` pin table.
*   **Reason for Rejection**: Opening six `/dev/gpout*` descriptors alongside stdin/stdout/stderr and `/dev/adc0` forces allocating a second 6-entry descriptor block (`CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=6` in `nuttx/fs/inode/fs_files.c`), requires six `ioctl(GPIOC_WRITE)` syscalls per update, and exposes `GPIO 26–28` if `board.h` retains upstream GPIO macros. `/dev/userleds` updates all 6 LEDs in one `ULEDIOC_SETALL` call over one descriptor.

### Alternative E: Hardware `UART0` Console (`D6`/`D7`)

*   **Pros**: Outputs console messages at boot without waiting for USB enumeration or three Enter keystrokes.
*   **Reason for Rejection**: Requires wiring an external USB-to-UART adapter to `D6`/`D7` (`GPIO 0`/`1`) instead of using the board's built-in USB-C port (`usbnsh`).

### Alternative F: Setting `CONFIG_INIT_ENTRYPOINT` Directly to the Bar-Graph App

*   **Pros**: Eliminates `CONFIG_ETC_ROMFS`, `genromfs`, `xxd`, and `NSH_USBCONSOLE` plumbing.
*   **Reason for Rejection**: Removes `nsh>`, `dmesg`, `ps`, `free`, and interactive daemon control (`kill -15`, restart) over USB-C.

## Non-Functional Requirements

*   **CPU and Interrupt Load**: Opening `/dev/adc0` for one 5-byte read and closing it every 50 ms restricts ADC FIFO interrupts to capturing one sample per tick plus at most one trailing completion interrupt when `g_active_count` reaches `0`, leaving the ADC idle for the remainder of the 50 ms sleep window.
*   **SRAM Footprint**: With 520 KB configured SRAM (`CONFIG_RAM_SIZE=532480` in `usbnsh/defconfig`), `CONFIG_RAMLOG_BUFSIZE=4096` (4 KB) and a 2048-byte daemon stack (`CONFIG_BARGRAPH_STACKSIZE=2048`) add ~6 KB, verified against `memmap_default.ld` memory usage during build.
*   **Rollback and Recovery**: The daemon installs a `SIGTERM` handler so `kill -15 <pid>` clears all LEDs, closes `/dev/userleds`, and exits cleanly. If a firmware build faults at boot, holding `BOOT` while resetting the XIAO RP2350 enters the RP2350 ROM UF2 bootloader for re-flashing.

## Risks & Limits

*   **Breadboard LED Current Budget**: Driving 6 active-high LEDs simultaneously (`0x3f`) on `GPIO 2–7` requires `>= 330 Ω` series resistors (~4–6 mA per pin at 3.3 V) to remain safely within RP2350 per-pin and total IOVDD current limits.
*   **ADC Top-End Saturation and Breadboard Noise**: Nominal thresholds at multiples of `585` (`level 6` entry at `3510 + 24 = 3534`) intentionally reserve the top ~14% of full scale (`3534..4095`) so the bar reaches 6 lit LEDs reliably before the wiper hits the mechanical 3.3 V stop. If breadboard jumper noise exceeds the `±24`-count (~19 mV) hysteresis band, an optional `100 nF` ceramic capacitor from `D0` to `GND` filters high-frequency pickup without software multi-sample complexity.
*   **Exclusive `/dev/adc0` Opener and `0`-Sample `FIFO_ERR` Sign Limitation**: Per-tick `close()` stops conversions only when `g_active_count` drops to `0`, so `bargraph_daemon` must remain the sole opener of `/dev/adc0`. Additionally, `rp23xx_adc.c` signals `RP23XX_ADC_FIFO_ERR` by negating `value` (`value = -value`), so a hardware `FIFO_ERR` coinciding with a raw `0` reading remains `0` and is indistinguishable from a valid `0 V` sample (`level 0`).

## Additional Resources

*   [xiao-rp2350 board.h](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h)
*   [xiao-rp2350 rp23xx_bringup.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_bringup.c)
*   [xiao-rp2350 rp23xx_userleds.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_userleds.c)
*   [xiao-rp2350 usbnsh defconfig](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig)
*   [rp23xx_adc.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/arch/arm/src/rp23xx/rp23xx_adc.c)
*   [nuttx/drivers/analog/adc.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/drivers/analog/adc.c)
*   [rp23xx_common_initialize.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/common/src/rp23xx_common_initialize.c)
*   [rp23xx_common_bringup.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c)
*   [Board.mk](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/Board.mk)
*   [Config.mk](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/tools/Config.mk)
*   [nuttx/boards/Kconfig](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/Kconfig)
*   [apps/system/nsh/nsh_main.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/system/nsh/nsh_main.c)
*   [apps/examples/leds/leds_main.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/examples/leds/leds_main.c)
*   [apps/examples/adc/adc_main.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/examples/adc/adc_main.c)

## Future Improvements

*   Add an ADC sample-rate divider or timer-triggered DMA mode to `rp23xx_adc.c` upstream so continuous-open reads can run at 20 Hz without per-tick `open`/`close`.
*   Add PWM duty-cycle smoothing across intermediate potentiometer positions on spare RP2350 PWM slices.

--------------------------------------------------------------------------------

## Detailed Implementation Plan

All files below are created inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`. No tracked files inside `nuttx/` or `apps/` are edited; only the gitignored `apps/external` symlink, generated `external/Kconfig` / `apps/Kconfig`, and build outputs are created during configuration and compilation.

### Target Files to Create (Phased Delivery)

#### Phase 1: Out-of-Tree Build Wrapper, Custom Board Skeleton, and `defconfig`

##### 1. `xiao-rp2350-bargraph/Makefile` (Create)

*   **Type / Scope**: Top-level GNU Make build and configuration wrapper.
*   **Rationale**: Keeps one-command configuration and building self-contained outside `nuttx/` while managing the `apps/external` symlink and Kconfig regeneration when `external/bargraph/Kconfig` changes.
*   **Proposed Changes**:
    *   `setup` target: Creates or verifies `../apps/external -> ../xiao-rp2350-bargraph/external` (aborting if `../apps/external` exists as a non-symlink directory); if `external/bargraph/Kconfig` is newer than `../apps/Kconfig`, deletes `../apps/Kconfig` and `external/Kconfig` (if present) and runs `$(MAKE) -C ../nuttx olddefconfig` when `../nuttx/.config` already exists so `apps/Directory.mk` regenerates Kconfig files.
    *   `configure` target: Depends on `setup`; runs `cd ../nuttx && ./tools/configure.sh -e -l ../xiao-rp2350-bargraph/board/configs/bargraph`.
    *   `all` (default) target: Depends on `setup`; if `../nuttx/.config` is missing, runs `configure`, then runs `$(MAKE) -C ../nuttx` to produce `../nuttx/nuttx.uf2`.
    *   `clean` target: Runs `$(MAKE) -C ../nuttx clean` only if `../nuttx/Make.defs` and `../nuttx/.config` exist.
    *   `distclean` target: If `../nuttx/Make.defs` and `../nuttx/.config` exist, runs `$(MAKE) -C ../nuttx distclean`; then removes `../apps/external`, `external/Kconfig`, and `../apps/Kconfig`.

##### 2. `xiao-rp2350-bargraph/board/Kconfig` (Create)

*   **Type / Scope**: Custom board Kconfig file.
*   **Rationale**: `nuttx/tools/Config.mk` binds `BOARD_KCONFIG = $(BOARD_DIR)/Kconfig` when `ARCH_BOARD_CUSTOM=y` and that file exists (falling back to `boards/dummy/dummy_kconfig` if absent). Keeping a minimal explicit `board/Kconfig` reserves the board menu slot cleanly.
*   **Proposed Changes**:
    *   Contain only a minimal `if ARCH_BOARD_CUSTOM` / `endif` block with a comment noting board LED support uses `CONFIG_BOARD_CUSTOM_LEDS=y` in `nuttx/boards/Kconfig`.

##### 3. `xiao-rp2350-bargraph/board/include/board.h` (Create)

*   **Type / Scope**: Custom board header (`<arch/board/board.h>`).
*   **Rationale**: Defines RP2350 clock frequencies, omits upstream `GPIO 26–28` generic GPIO macros (`GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1`) so `CONFIG_DEV_GPIO` cannot claim ADC or avoided pins, and sets `BOARD_NLEDS = 6`.
*   **Proposed Changes**:
    *   Retain RP2350 clock macros (`BOARD_XOSC_FREQ` = `12 * MHZ`, `BOARD_PLL_SYS_FREQ` = `150 * MHZ`, `BOARD_PLL_USB_FREQ` = `48 * MHZ`, `BOARD_ADC_FREQ` = `48 * MHZ`, `BOARD_TICK_CLOCK` = `1 * MHZ`, and oscillator startup/delay macros) and `BOARD_GPIO_LED_PIN = 25` from `nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h`.
    *   Omit `"rp23xx_i2cdev.h"`, `"rp23xx_spidev.h"`, `"rp23xx_i2sdev.h"`, and `"rp23xx_spisd.h"` (those headers reside only in `xiao-rp2350/include/`, are outside the custom board include path, and are unused when I2C/SPI/I2S/SPISD are disabled).
    *   Omit `BOARD_NGPIOOUT`, `BOARD_NGPIOIN`, `BOARD_NGPIOINT`, `GPIO_OUT1`, `GPIO_IN1`, and `GPIO_IRQPIN1`.
    *   Define `BOARD_NLEDS` (`6`).
    *   Declare `void rp23xx_boardearlyinitialize(void);` and `void rp23xx_boardinitialize(void);`.

##### 4. `xiao-rp2350-bargraph/board/scripts/Make.defs` (Create)

*   **Type / Scope**: Custom board compiler flags and linker script Makefile fragment.
*   **Rationale**: `tools/configure.sh` symlinks `nuttx/Make.defs` to `$(BOARD_DIR)/scripts/Make.defs`. This file sets ARMv8-M flags and points `ARCHSCRIPT` to the upstream `xiao-rp2350` linker script without duplication.
*   **Proposed Changes**:
    *   Include `$(TOPDIR)/.config`, `$(TOPDIR)/tools/Config.mk`, `$(TOPDIR)/tools/rp23xx/Config.mk`, and `$(TOPDIR)/arch/arm/src/armv8-m/Toolchain.defs`.
    *   Set `LDSCRIPT = memmap_default.ld` (matching `CONFIG_BOOT_RUNFROMFLASH=y`) and `ARCHSCRIPT += $(TOPDIR)$(DELIM)boards$(DELIM)arm$(DELIM)rp23xx$(DELIM)xiao-rp2350$(DELIM)scripts$(DELIM)$(LDSCRIPT)`.
    *   Set `CFLAGS`, `CXXFLAGS`, `CPPFLAGS`, `AFLAGS`, `ARCHCCVERSION`, `ARCHINCLUDES`, `ARCHXXINCLUDES`, `ARCHSCRIPT`, `ARCHPICFLAGS`, and `LDELFFLAGS` matching `nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/Make.defs` (omitting unused `NXFLATLDFLAGS*`).

##### 5. `xiao-rp2350-bargraph/board/configs/bargraph/defconfig` (Create)

*   **Type / Scope**: Compressed NuttX `.config` template derived from `nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig`.
*   **Rationale**: Configures the custom board, common RP2350 helpers, late board init, `ADC0`, 6-LED `userleds`, ROMFS `/etc/init.d`, USB CDC/ACM NSH (with `ps` and `kill`), RAMLOG `dmesg`, and the external `bargraph` app.
*   **Proposed Changes**:
    *   **Removals relative to `usbnsh/defconfig`** (unconditional): Remove `CONFIG_ARCH_BOARD="xiao-rp2350"`, `CONFIG_ARCH_BOARD_XIAO_RP2350=y`, `CONFIG_DEV_GPIO=y`, `CONFIG_EXAMPLES_HELLO=y`, `CONFIG_TESTING_GETPRIME=y`, and `CONFIG_TESTING_OSTEST=y`.
    *   **Retained baseline symbols from `usbnsh/defconfig`**: Keep `CONFIG_ARCH="arm"`, `CONFIG_ARCH_BOARD_COMMON=y`, `CONFIG_ARCH_CHIP="rp23xx"`, `CONFIG_ARCH_CHIP_RP2350=y`, `CONFIG_ARCH_CHIP_RP23XX=y`, `CONFIG_ARCH_CORTEXM33=y`, `CONFIG_ARCH_FAMILY="armv8-m"`, `CONFIG_ARCH_RAMVECTORS=y`, `CONFIG_ARCH_STACKDUMP=y`, `CONFIG_BOARDCTL_RESET=y`, `CONFIG_BOARDCTL_USBDEVCTRL=y`, `CONFIG_BOARD_LOOPSPERMSEC=10450`, `CONFIG_BOOT_RUNFROMFLASH=y`, `CONFIG_BUILTIN=y`, `CONFIG_CDCACM=y`, `CONFIG_CDCACM_CONSOLE=y`, `CONFIG_DEV_NULL=y`, `# CONFIG_DEV_CONSOLE is not set`, `CONFIG_FS_PROCFS=y`, `CONFIG_FS_PROCFS_REGISTER=y`, `CONFIG_INIT_ENTRYPOINT="nsh_main"`, `CONFIG_LIBM=y`, `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=6`, `CONFIG_NSH_ARCHINIT=y`, `CONFIG_NSH_BUILTIN_APPS=y`, `CONFIG_NSH_FILEIOSIZE=512`, `CONFIG_NSH_READLINE=y`, `CONFIG_NSH_USBCONSOLE=y`, `CONFIG_RAM_SIZE=532480`, `CONFIG_RAM_START=0x20000000`, `CONFIG_RAW_BINARY=y`, `# CONFIG_RP23XX_UART0 is not set`, `CONFIG_RR_INTERVAL=200`, `CONFIG_SCHED_WAITPID=y`, `CONFIG_START_DAY=29`, `CONFIG_START_MONTH=1`, `CONFIG_START_YEAR=2024`, `CONFIG_SYMTAB_ORDEREDBYNAME=y`, `CONFIG_SYSTEM_NSH=y`, `CONFIG_USBDEV=y`, and `CONFIG_USBDEV_BUSPOWERED=y` (keeping `ps` and `kill` enabled by leaving `CONFIG_NSH_DISABLE_PS`, `CONFIG_NSH_DISABLE_KILL`, and `CONFIG_DISABLE_ALL_SIGNALS` unset).
    *   **New / modified symbols**:
        *   Custom board & init: `CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_ARCH_BOARD_CUSTOM_NAME="xiao-rp2350-bargraph"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y`, `CONFIG_BOARD_CUSTOM_LEDS=y`, `# CONFIG_ARCH_LEDS is not set`, `CONFIG_BOARD_LATE_INITIALIZE=y`.
        *   Drivers & app: `CONFIG_ANALOG=y`, `CONFIG_ADC=y`, `CONFIG_RP23XX_ADC=y`, `CONFIG_RP23XX_ADC_CHANNEL0=y`, `CONFIG_USERLED=y`, `CONFIG_USERLED_LOWER=y`, `CONFIG_BARGRAPH=y`.
        *   ROMFS & RAMLOG: `CONFIG_FS_ROMFS=y`, `CONFIG_ETC_ROMFS=y`, `# CONFIG_NSH_DISABLESCRIPT is not set`, `CONFIG_RAMLOG=y`, `CONFIG_RAMLOG_SYSLOG=y`, `CONFIG_RAMLOG_BUFSIZE=4096` (`CONFIG_RP23XX_ROMFS_ROMDISK_DEVNAME` unset).

*   **Phase 1 Checkpoint**: `make -C xiao-rp2350-bargraph configure` links `apps/external` and configures `nuttx/.config` with zero errors (verify `CONFIG_ARCH_BOARD_CUSTOM=y` and `CONFIG_BARGRAPH=y`).

#### Phase 2: Board Bringup, Analog Pad `IE` Fix, 6-LED Driver, and ROMFS Startup Scripts

##### 6. `xiao-rp2350-bargraph/board/src/Make.defs` (Create)

*   **Type / Scope**: Board source and ROMFS script build list.
*   **Rationale**: Registers board initialization, bringup, 6-LED driver, and ROMFS `/etc/init.d` files with `nuttx/boards/Board.mk`. With `CONFIG_ARCH_BOARD_COMMON=y`, `tools/configure.sh` symlinks `arch/arm/src/board` to `nuttx/boards/arm/rp23xx/common` and `arch/arm/src/board/board` to `$(BOARD_DIR)/src`, which is why the custom board path uses `board/board`.
*   **Proposed Changes**:
    *   Add `rp23xx_boardinitialize.c` and `rp23xx_bringup.c` to `CSRCS`.
    *   Under `ifneq ($(CONFIG_ARCH_LEDS),y)`, add `rp23xx_userleds.c` to `CSRCS`.
    *   Under `ifeq ($(CONFIG_ETC_ROMFS),y)`, set `RCSRCS = etc/init.d/rc.sysinit etc/init.d/rcS`.
    *   Set `DEPPATH += --dep-path board`, `VPATH += :board`, and `CFLAGS += ${INCDIR_PREFIX}$(TOPDIR)$(DELIM)arch$(DELIM)$(CONFIG_ARCH)$(DELIM)src$(DELIM)board$(DELIM)board`.

##### 7. `xiao-rp2350-bargraph/board/src/rp23xx_pico.h` (Create)

*   **Type / Scope**: Internal board source header.
*   **Rationale**: Centralizes the 6 breadboard LED GPIO numbers and ADC pin range constants shared by `rp23xx_userleds.c`, `rp23xx_bringup.c`, and `rp23xx_boardinitialize.c`, matching the upstream `rp23xx_pico.h` layout.
*   **Proposed Changes**:
    *   Define `GPIO_ADC_BASE` (`26`), `GPIO_ADC_COUNT` (`4`), `GPIO_LED1` (`5`, `D3`), `GPIO_LED2` (`6`, `D4`), `GPIO_LED3` (`7`, `D5`), `GPIO_LED4` (`2`, `D8`), `GPIO_LED5` (`4`, `D9`), and `GPIO_LED6` (`3`, `D10`).
    *   Declare `int rp23xx_bringup(void);`.

##### 8. `xiao-rp2350-bargraph/board/src/rp23xx_boardinitialize.c` (Create)

*   **Type / Scope**: Early, standard, and late board initialization hooks.
*   **Rationale**: Invokes shared RP2350 common initialization and exposes `board_late_initialize()` so `rp23xx_bringup()` runs before NSH executes `/etc/init.d/rcS`.
*   **Proposed Changes**:
    *   `rp23xx_boardearlyinitialize()`: Calls `rp23xx_common_earlyinitialize()` and configures `BOARD_GPIO_LED_PIN` (`GPIO 25`) as output driven `true` (active-low onboard LED off, matching upstream `xiao-rp2350`).
    *   `rp23xx_boardinitialize()`: Calls `rp23xx_common_initialize()` (without unused `CONFIG_RP23XX_PSRAM` / `rp23xx_psramconfig()`).
    *   `board_late_initialize()`: Guarded by `#ifdef CONFIG_BOARD_LATE_INITIALIZE`, calls `(void)rp23xx_bringup();`.

##### 9. `xiao-rp2350-bargraph/board/src/rp23xx_bringup.c` (Create)

*   **Type / Scope**: Peripheral driver bringup and analog pad fix.
*   **Rationale**: Registers `/dev/adc0` via `rp23xx_common_bringup()`, clears `RP23XX_PADS_BANK0_GPIO_IE` across `GPIO 26–29` after `adc_register()` -> `my_reset()` re-enabled `IE=1`, and registers `/dev/userleds`.
*   **Proposed Changes**:
    *   Include `<nuttx/config.h>`, `<nuttx/debug.h>`, `<sys/types.h>`, `<syslog.h>`, `<arch/board/board.h>`, `<nuttx/leds/userled.h>`, `"arm_internal.h"`, `"rp23xx_gpio.h"`, `"rp23xx_common_bringup.h"`, and `"rp23xx_pico.h"`.
    *   In `int rp23xx_bringup(void)`:
        1. Call `int ret = rp23xx_common_bringup();`; if `ret < 0`, log via `syslog(LOG_ERR, ...)` and `return ret;` (noting that `rp23xx_common_bringup()` logs ADC setup failures internally and overwrites `ret` in later mount blocks).
        2. Under `#if defined(CONFIG_ADC) && defined(CONFIG_RP23XX_ADC)`, loop `for (int i = 0; i < GPIO_ADC_COUNT; i++)` and call `clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, RP23XX_PADS_BANK0_GPIO(GPIO_ADC_BASE + i))` to keep digital input buffers disabled on `GPIO 26–29`.
        3. Under `#ifdef CONFIG_USERLED`, call `ret = userled_lower_initialize("/dev/userleds");`; if `ret < 0`, log via `syslog(LOG_ERR, ...)` and `return ret;`.
        4. `return OK;`.

##### 10. `xiao-rp2350-bargraph/board/src/rp23xx_userleds.c` (Create)

*   **Type / Scope**: Multi-LED `userleds` lower-half driver (`!CONFIG_ARCH_LEDS`).
*   **Rationale**: Replaces upstream `xiao-rp2350/src/rp23xx_userleds.c` which hardcodes a single LED (`GPIO 25`) and ignores `g_ledcfg[]` in `board_userled_all()`.
*   **Proposed Changes**:
    *   Wrap implementation in `#ifndef CONFIG_ARCH_LEDS`; include `<nuttx/config.h>`, `<stdint.h>`, `<stdbool.h>`, `<nuttx/board.h>`, `<arch/board/board.h>`, `"rp23xx_gpio.h"`, and `"rp23xx_pico.h"` (omitting unused `CONFIG_PM` / `rp23xx_led_pminitialize`).
    *   Define `static const uint32_t g_ledcfg[BOARD_NLEDS] = { GPIO_LED1, GPIO_LED2, GPIO_LED3, GPIO_LED4, GPIO_LED5, GPIO_LED6 };`.
    *   `uint32_t board_userled_initialize(void)`: Loops `0..BOARD_NLEDS - 1`, calling `rp23xx_gpio_init(g_ledcfg[i])`, `rp23xx_gpio_setdir(g_ledcfg[i], true)`, and `rp23xx_gpio_put(g_ledcfg[i], false)` (active-high off); returns `BOARD_NLEDS`.
    *   `void board_userled(int led, bool ledon)`: If `(unsigned)led < BOARD_NLEDS`, calls `rp23xx_gpio_put(g_ledcfg[led], ledon)`.
    *   `void board_userled_all(uint32_t ledset)`: Loops `0..BOARD_NLEDS - 1`, calling `rp23xx_gpio_put(g_ledcfg[i], (ledset & (1U << i)) != 0)`.

##### 11. `xiao-rp2350-bargraph/board/src/etc/init.d/rc.sysinit` (Create)

*   **Type / Scope**: ROMFS system initialization script processed by `Board.mk`.
*   **Rationale**: Matches upstream `xiao-rp2350` ROMFS layout (`apps/nshlib/nsh_init.c` executes `rc.sysinit` before `rcS` when present).
*   **Proposed Changes**:
    *   Contain `#include <nuttx/config.h>` and Apache license header with C-style `/* ... */` comments.

##### 12. `xiao-rp2350-bargraph/board/src/etc/init.d/rcS` (Create)

*   **Type / Scope**: ROMFS NSH startup script processed by `Board.mk` via `cpp`.
*   **Rationale**: Launches `bargraph` automatically at power-on before `nsh_consolemain()` waits for USB attachment.
*   **Proposed Changes**:
    *   Include `<nuttx/config.h>` and use C-style `/* ... */` comments (no `#` shell comments, which fail `cpp` preprocessing).
    *   Under `#ifdef CONFIG_BARGRAPH`, invoke `bargraph` (which spawns `bargraph_daemon` via `task_create()` and returns immediately without blocking USB NSH).

*   **Phase 2 Checkpoint**: Building without `bargraph` enabled (or before Phase 3 app code) produces a bootable custom-board image with `/dev/adc0`, `/dev/userleds`, and `/etc/init.d/rcS` embedded in `nuttx/boards/arm/rp23xx/common/etctmp.c`.

#### Phase 3: External `bargraph` Daemon Application

##### 13. `xiao-rp2350-bargraph/external/Makefile` and `Make.defs` (Create)

*   **Type / Scope**: `apps/external` directory glue (`apps/` scans `apps/external` automatically when present).
*   **Rationale**: Integrates `external/bargraph` into the NuttX `apps/` build and `preconfig` tree without editing `apps/Makefile` or `apps/Kconfig`. `apps/Directory.mk` auto-generates `external/Kconfig` during `preconfig` and deletes it on `distclean`, so no hand-written `external/Kconfig` is created.
*   **Proposed Changes**:
    *   `external/Makefile`: Set `MENUDESC = "External"` and include `$(APPDIR)/Directory.mk`.
    *   `external/Make.defs`: `include $(wildcard $(APPDIR)/external/*/Make.defs)`.

##### 14. `xiao-rp2350-bargraph/external/bargraph/Kconfig` (Create)

*   **Type / Scope**: Application configuration symbols.
*   **Rationale**: Exposes task parameters, device paths (enabling deterministic CUJ 3 fault testing via `CONFIG_BARGRAPH_ADC_DEVPATH`), sampling interval, and hysteresis deadband to `defconfig` and `rcS` (`CONFIG_BARGRAPH`).
*   **Proposed Changes**:
    *   Define `config BARGRAPH` (`tristate "Potentiometer LED bar graph daemon"`, `default n`, `depends on ADC && USERLED`).
    *   Under `if BARGRAPH`, define `BARGRAPH_PROGNAME` (`"bargraph"`), `BARGRAPH_PRIORITY` (`100`), `BARGRAPH_STACKSIZE` (`2048`), `BARGRAPH_ADC_DEVPATH` (`"/dev/adc0"`), `BARGRAPH_LED_DEVPATH` (`"/dev/userleds"`), `BARGRAPH_INTERVAL_US` (`50000`), and `BARGRAPH_HYSTERESIS` (`24`).

##### 15. `xiao-rp2350-bargraph/external/bargraph/Makefile` and `Make.defs` (Create)

*   **Type / Scope**: Application build registration.
*   **Rationale**: Registers `bargraph` as a built-in NSH application compiled from `bargraph_main.c`.
*   **Proposed Changes**:
    *   `Make.defs`: `ifneq ($(CONFIG_BARGRAPH),)` -> `CONFIGURED_APPS += $(APPDIR)/external/bargraph`.
    *   `Makefile`: Include `$(APPDIR)/Make.defs`, set `PROGNAME = $(CONFIG_BARGRAPH_PROGNAME)`, `PRIORITY = $(CONFIG_BARGRAPH_PRIORITY)`, `STACKSIZE = $(CONFIG_BARGRAPH_STACKSIZE)`, `MODULE = $(CONFIG_BARGRAPH)`, `MAINSRC = bargraph_main.c`, and include `$(APPDIR)/Application.mk`.

##### 16. `xiao-rp2350-bargraph/external/bargraph/bargraph_main.c` (Create)

*   **Type / Scope**: User-space C singleton daemon (`main` + `bargraph_daemon`).
*   **Rationale**: Implements the 20 Hz potentiometer-to-bar-graph loop, per-tick ADC open/read/close shutdown, hysteresis smoothing, stdio isolation, `SIGTERM` shutdown, and headless LED fault blinking.
*   **Proposed Changes**:
    *   State: `static volatile sig_atomic_t g_bargraph_running = 0;` and `static volatile sig_atomic_t g_bargraph_stop = 0;`.
    *   `sigterm_handler(int signo, siginfo_t *siginfo, void *arg)`: Marks unused parameters, sets `g_bargraph_stop = 1` on `SIGTERM`.
    *   `redirect_stdio_null(void)` helper: Opens `/dev/null` (`O_RDWR`); if successful, duplicates onto FDs `0, 1, 2` via `dup2()` and closes the temporary descriptor if `> 2` (so NSH restarts never hold `/dev/ttyACM0` open).
    *   `read_adc_sample(const char *devpath, uint16_t *raw_out)` helper:
        *   Opens `devpath` with `O_RDONLY` (returning `-errno` on failure), reads `sizeof(struct adc_msg_s)` (5 bytes), and closes `fd` immediately.
        *   If `nbytes < 0`: returns `-EINTR` when `errno == EINTR`, otherwise returns `-errno`.
        *   If `nbytes != (ssize_t)sizeof(struct adc_msg_s)`: returns `-EIO`.
        *   If `sample.am_data < 0`: returns `-EAGAIN` (hardware FIFO conversion error).
        *   Otherwise sets `*raw_out = (uint16_t)(sample.am_data >> 19)` and returns `0`.
    *   `quantize_level(uint16_t raw, int current_level)` helper:
        *   Uses 6 nominal thresholds `{ 585, 1170, 1755, 2340, 2925, 3510 }` with deadband `CONFIG_BARGRAPH_HYSTERESIS` (`24`).
        *   Steps `level` up while `level < 6 && raw >= thresholds[level] + hyst`, and down while `level > 0 && raw <= thresholds[level - 1] - hyst`.
    *   `fault_blink_loop(int led_fd)` helper:
        *   Alternates `ioctl(led_fd, ULEDIOC_SETALL, 0x15UL)` and `0x2aUL` every `250000` us while `!g_bargraph_stop` (logging `ioctl` failure via `syslog(LOG_ERR, ...)` and breaking cleanly), then returns to `bargraph_daemon`'s exit cleanup.
    *   `bargraph_daemon(int argc, FAR char *argv[])`:
        *   Calls `UNUSED(argc); UNUSED(argv);` and `redirect_stdio_null();`.
        *   Installs `SIGTERM` `sigaction` with `sa_flags = SA_SIGINFO` (no `SA_RESTART`), logs start via `syslog(LOG_INFO, ...)`.
        *   Opens `CONFIG_BARGRAPH_LED_DEVPATH` (`O_WRONLY`) and clears LEDs via `ioctl(led_fd, ULEDIOC_SETALL, 0UL)`. On open or `ioctl` failure, logs via `syslog(LOG_ERR, ...)`, closes `led_fd` if `>= 0`, resets `g_bargraph_running = 0`, and returns `EXIT_FAILURE`.
        *   Loops while `!g_bargraph_stop`: calls `read_adc_sample()`. If `ret == -EINTR` or `g_bargraph_stop`, exits loop; if `ret == -EAGAIN`, sleeps `CONFIG_BARGRAPH_INTERVAL_US` without incrementing `consecutive_errors`; if `ret < 0`, logs once on error and increments `consecutive_errors`, calling `fault_blink_loop(led_fd)` when `consecutive_errors >= 5` and exiting the loop afterward; on `ret == 0`, resets `consecutive_errors = 0`, updates `current_level = quantize_level(raw, current_level)`, and writes `ioctl(led_fd, ULEDIOC_SETALL, (unsigned long)mask)` on level changes (exiting on `ioctl` failure), then calls `usleep(CONFIG_BARGRAPH_INTERVAL_US)`.
        *   On any exit from the daemon loop: calls `ioctl(led_fd, ULEDIOC_SETALL, 0UL)`, closes `led_fd`, resets `g_bargraph_running = 0`, and returns `EXIT_SUCCESS` (or `EXIT_FAILURE` on fatal `ULEDIOC_SETALL` error).
    *   `main(int argc, FAR char *argv[])`:
        *   Calls `UNUSED(argc); UNUSED(argv);`.
        *   If `g_bargraph_running` is already non-zero, prints `"bargraph: daemon already running\n"` and returns `EXIT_SUCCESS`.
        *   Sets `g_bargraph_stop = 0;` and `g_bargraph_running = 1;` before `task_create("bargraph_daemon", CONFIG_BARGRAPH_PRIORITY, CONFIG_BARGRAPH_STACKSIZE, bargraph_daemon, NULL)`. If `task_create()` returns `< 0`, resets `g_bargraph_running = 0`, logs via `syslog(LOG_ERR, ...)`, prints error, and returns `EXIT_FAILURE`; otherwise returns `EXIT_SUCCESS`.

### Verification Plan

*   **Host Prerequisite Check**:
    *   Before compiling, verify `arm-none-eabi-gcc`, `genromfs`, and `xxd` are installed on the Linux host.
    *   Before flashing via CLI, verify `picotool` is installed (optional if flashing via UF2 drag-and-drop in `BOOTSEL` mode).
*   **Compilation / Build**: Run `make -C /home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph` and verify:
    1. `../apps/external` resolves to `../xiao-rp2350-bargraph/external` and `git status` inside both `nuttx/` and `apps/` reports zero modified tracked files.
    2. `../nuttx/nuttx.uf2` builds cleanly with zero compiler errors/warnings in `xiao-rp2350-bargraph/`.
    3. Generated `nuttx/boards/arm/rp23xx/common/etctmp.c` (accessed via `nuttx/arch/arm/src/board/`) contains the ROMFS image including `/etc/init.d/rcS`.
*   **Hardware / Integration Verification (On XIAO RP2350 Breadboard)**:
    1. Flash `nuttx.uf2` via `BOOTSEL` (`picotool load -f ../nuttx/nuttx.uf2 -x` or UF2 drag-and-drop).
    2. **Autonomous Boot & Quantizer Tracking (CUJ 1)**: Power the board from a 5 V supply with no host serial terminal open. Rotate the potentiometer on `D0` from `0 V` to `3.3 V` and back, verifying 0 to 6 LEDs (`D3`, `D4`, `D5`, `D8`, `D9`, `D10`) step monotonically across all 7 states (`0x00`..`0x3f`) with no flicker near stationary threshold boundaries (`585 ± 24`, `3510 ± 24`, `0`, `4095`).
    3. **USB-C NSH, IRQ Quiescence, & Lifecycle (CUJ 2)**: Plug into the Linux host, open `/dev/ttyACM0`, press Enter 3 times to reach `nsh>`, and verify:
       * `dmesg` shows no boot or driver errors and logs `bargraph_daemon` startup.
       * `cat /proc/interrupts` sampled twice 1 second apart shows ~20 ADC FIFO interrupt bursts per second rather than ~500,000 continuous interrupts/second.
       * `ps` shows `bargraph_daemon` running; running `bargraph` a second time prints `"bargraph: daemon already running"` without spawning a duplicate.
       * `kill -15 <pid>` stops `bargraph_daemon` and turns off all 6 LEDs; running `bargraph` restarts it; unplugging/replugging USB-C still re-enumerates `/dev/ttyACM0` cleanly without FD lockup.
    4. **Fault Indication & Cleanup (CUJ 3)**: Temporarily set `CONFIG_BARGRAPH_ADC_DEVPATH="/dev/adc_missing"` in a test `.config`, rebuild and flash, verify `/dev/userleds` alternates `0x15` / `0x2A` at 250 ms, verify `dmesg` records the `syslog(LOG_ERR, ...)` entry, verify `kill -15 <pid>` stops the fault blink and clears all LEDs, and restore `CONFIG_BARGRAPH_ADC_DEVPATH="/dev/adc0"` + rebuild cleanly.

--------------------------------------------------------------------------------

*Drafted with EGM*