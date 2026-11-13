# Out-of-Tree XIAO RP2350 Potentiometer LED Bar Graph (xiao-rp2350-bargraph)

*   **Author:** Brett Morgan
*   **Status:** Draft
*   **Date:** 2026-11-13

--------------------------------------------------------------------------------

## Overview

`xiao-rp2350-bargraph` is an out-of-tree Apache NuttX firmware project targeting a Seeed Studio XIAO RP2350 mounted on a solderless breadboard with one analog potentiometer (outer terminals connected to `3.3 V` and `GND`, wiper on `D0`) and six discrete active-high LEDs (`D3–D5`, `D8–D10`). The firmware samples the potentiometer at 20 Hz (every 50 ms) and drives 0 to 6 LEDs proportionally as a bar graph, launching at power-on without an attached host computer while exposing an interactive NuttShell (`nsh>`) console over USB-C (CDC/ACM) when plugged into a host.

All custom board, configuration, startup script, application, and build wrapper files will reside inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph` (proposed directory layout below), leaving tracked files in `nuttx/` and `apps/` unmodified.

## The Problem

Building a hardware-backed application on the XIAO RP2350 in Apache NuttX without modifying upstream submodules exposes four conflicts between the board's pinout, the RP2350 ADC driver (`rp23xx_adc.c`), the NuttShell USB console startup sequence, and the out-of-tree build system:

1.  **ADC Pin Clobbering and Analog Pad Input-Buffer Leakage (`GPIO 26–29`)**: During driver registration (`adc_register()`), `my_reset()` in `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` loops over the 4 GPIO entries in the 5-entry `g_gpio_map` (`{ 26, 27, 28, 29, -1 }`) regardless of which `CONFIG_RP23XX_ADC_CHANNEL*` options are enabled. It reconfigures all four GPIO pins (`GPIO 26–29`) as floating inputs (`RP23XX_GPIO_FUNC_NULL`), overwriting any LED pin setup on `D1` (`GPIO 27`) or `D2` (`GPIO 28`), and re-enables the digital input buffer (`RP23XX_PADS_BANK0_GPIO_IE`) that `rp23xx_common_earlyinitialize()` cleared for analog operation. Leaving `IE=1` on `GPIO 26–29` introduces digital input-buffer leakage on the potentiometer wiper (`GPIO 26`) and leaves unconnected pins `GPIO 27–29` floating with `IE=1`.
2.  **Unthrottled Free-Running ADC FIFO Interrupts and Stale Ring-Buffer Samples**: `rp23xx_adc.c` rejects `ANIOC_TRIGGER` (the upper-half `ioctl` command for software-triggered single conversions, returning `-ENOTTY`), sets no ADC clock divider, and re-arms a new conversion inside its FIFO interrupt handler as long as `/dev/adc0` remains open. Meanwhile, the upper-half ring buffer (`nuttx/drivers/analog/adc.c`, `CONFIG_ADC_FIFOSIZE=8`) drops new samples once full rather than overwriting old ones. A task that keeps `/dev/adc0` open and sleeps for 50 ms leaves the ADC FIFO interrupt firing continuously and reads stale samples from the previous wake cycle.
3.  **Headless Boot vs. Blocking USB-C Console Initialization**: Upstream `xiao-rp2350/configs/usbnsh/defconfig` omits `CONFIG_BOARD_LATE_INITIALIZE=y`, which prevents `rp23xx_bringup()` from registering `/dev/adc0` and `/dev/userleds` at boot. During startup, `nsh_main()` (`apps/system/nsh/nsh_main.c`) calls `nsh_initialize()` (`apps/nshlib/nsh_init.c`), which executes `/etc/init.d/rcS` before `nsh_consolemain()` (`apps/nshlib/nsh_usbconsole.c`) connects USB CDC/ACM and blocks waiting for three Enter keystrokes (`\r` or `\n`, used by `NSH_USBCONSOLE` to detect an open host terminal on `/dev/ttyACM0`). Any foreground task in `rcS` blocks USB console enumeration indefinitely, and because `# CONFIG_DEV_CONSOLE is not set` with `CONFIG_DEV_NULL=y`, `group_setupidlefiles()` (`nuttx/sched/group/group_setupidlefiles.c`) binds boot-time FDs `0, 1, 2` to `/dev/null`, discarding `rcS` stdout/stderr.
4.  **Single-LED Upstream Board Support and Out-of-Tree Build Wiring**: Upstream `xiao-rp2350/src/rp23xx_userleds.c` hardcodes a single onboard LED (`GPIO_LED1` / `BOARD_LED1_BIT`, `GPIO 25`) in `board_userled_all()` rather than iterating over an array of LEDs, and `CONFIG_ARCH_LEDS` defaults to `y` when `ARCH_HAVE_LEDS` is selected, compiling `rp23xx_autoleds.c` instead of `rp23xx_userleds.c`. In addition, `xiao-rp2350/src/CMakeLists.txt` leaves `CONFIG_ETC_ROMFS` unimplemented (`# TODO`).

## Goals

*   Keep all project source files, custom board files, startup scripts, and build wrappers isolated in `xiao-rp2350-bargraph/` with zero modifications to tracked files in `nuttx/` or `apps/`.
*   Register `/dev/adc0` on `D0` (`GPIO 26` / `ADC0`) and clear digital pad input-enable (`IE=0`) across `GPIO 26–29` after `rp23xx_common_bringup()` completes.
*   Expose 6 active-high breadboard LEDs on `D3–D5` (`GPIO 5–7`) and `D8–D10` (`GPIO 2, 4, 3`) as a single bitmask device through `/dev/userleds`.
*   Sample `/dev/adc0` at 20 Hz using per-tick `open` -> `read` -> `close` cycles so the RP2350 ADC FIFO interrupt stops between ticks and each tick reads a fresh conversion.
*   Discard samples flagged with ADC FIFO conversion errors, map valid readings across 7 discrete levels (`0..6`) with hysteresis, and update `/dev/userleds` only when the displayed level changes.
*   Launch a singleton `bargraph` background daemon at boot from ROMFS `/etc/init.d/rcS` without blocking USB CDC/ACM console enumeration (`nsh>`), supporting `SIGTERM` shutdown and restart from NSH.
*   Log boot and runtime errors to RAMLOG (`dmesg`), and blink an alternating fault pattern (`0x15` / `0x2A`) on the 6 hardware LEDs after repeated `/dev/adc0` failures when running headless.

## Non-Goals

*   Supporting boards other than the Seeed Studio XIAO RP2350 in this project.
*   Patching upstream `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` or other tracked files inside `nuttx/` or `apps/` (only the gitignored `apps/external` symlink and standard build outputs are created during compilation).
*   Supporting CMake builds (`xiao-rp2350/src/CMakeLists.txt` lacks `CONFIG_ETC_ROMFS` support).
*   Adding PWM brightness dimming, hardware DMA pacing for the ADC, persistent flash calibration, or networking.

## Open Questions

*   None.

## Critical User Journeys (CUJs)

### CUJ 1: Standalone Power-Up and Potentiometer Tracking

*   **Given**: The XIAO RP2350 is flashed with `nuttx.uf2`, wired on a breadboard with a potentiometer across `3.3 V` and `GND` (wiper on `D0` / `GPIO 26`) and 6 active-high LEDs with series resistors on `D3`, `D4`, `D5`, `D8`, `D9`, and `D10`, and powered from a 5 V USB power supply with no serial terminal open.
*   **When**: The board powers on and the user rotates the potentiometer wiper from `0 V` (`GND`) to `3.3 V` and back.
*   **Then**: `board_late_initialize()` registers `/dev/adc0` and `/dev/userleds`, `/etc/init.d/rcS` spawns the `bargraph` daemon in the background, and the 6 LEDs step monotonically up from 0 lit (`0x00`) to 6 lit (`0x3f`) and back down to `0x00` at 20 Hz with no boundary flicker while input noise stays within the `±24`-count hysteresis band.

### CUJ 2: Interactive Inspection and Control via USB-C NuttShell

*   **Given**: The board is already running the bar-graph daemon from power-up.
*   **When**: The user connects the XIAO RP2350 USB-C port to a Linux host, opens `/dev/ttyACM0`, presses Enter three times, and runs `dmesg`, `ps`, `bargraph`, `kill -15 <pid>`, and `bargraph`.
*   **Then**: `nsh>` appears without interrupting the running bar graph; `dmesg` displays the RAMLOG boot and daemon startup messages; running `bargraph` while it is already active prints an `"already running"` notice and exits without spawning a duplicate task; sending `SIGTERM` stops the daemon loop, turns off all 6 LEDs, closes `/dev/userleds`, and exits; running `bargraph` again restarts the daemon without holding `/dev/ttyACM0` open.

### CUJ 3: Headless Hardware or Driver Fault Indication

*   **Given**: The board boots without a USB host attached (or is launched from NSH against a missing path via `bargraph /dev/adc_missing`), and `/dev/adc0` fails to open, returns a hard read error, or reports `RP23XX_ADC_FIFO_ERR` (`sample.am_data < 0`) across `>= 5` consecutive 50 ms sampling ticks while `/dev/userleds` is open.
*   **When**: The `bargraph` daemon reaches `>= 5` consecutive failed ticks (isolated 1–4 tick errors or `am_data < 0` glitches reset on the next valid sample).
*   **Then**: The daemon logs the error via `syslog(LOG_ERR, ...)` (persisted in RAMLOG for `dmesg` inspection) and enters a fault loop that alternates the 6 hardware LEDs between `0x15` (`0b010101`) and `0x2A` (`0b101010`) at 250 ms intervals until terminated via `SIGTERM`.

## Technical Plan (Architecture)

### Out-of-Tree Project Layout

All custom board files, kernel configuration, ROMFS startup script, external application code, and build automation will reside inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`:

```text
/home/brett/Documents/GitHub/nuttxspace/
├── nuttx/                             (unchanged upstream submodule)
├── apps/
│   └── external -> ../xiao-rp2350-bargraph/external   (gitignored symlink)
└── xiao-rp2350-bargraph/
    ├── Makefile                       (top-level symlink, configure, and build wrapper)
    ├── board/                         (CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board")
    │   ├── configs/bargraph/defconfig
    │   ├── include/board.h
    │   ├── scripts/Make.defs
    │   └── src/
    │       ├── Make.defs
    │       ├── rp23xx_pico.h
    │       ├── rp23xx_boardinitialize.c
    │       ├── rp23xx_bringup.c
    │       ├── rp23xx_userleds.c
    │       └── etc/init.d/rcS
    └── external/                      (mounted into apps build via apps/external; Kconfig auto-generated by Directory.mk)
        ├── Makefile
        ├── Make.defs
        └── bargraph/
            ├── Makefile
            ├── Make.defs
            ├── Kconfig
            └── bargraph_main.c
```

Four NuttX build-system rules govern this layout:

1.  **Shared RP23XX Common Directory (`BOARD_COMMON_DIR`)**: In `nuttx/tools/Config.mk`, if `$(BOARD_DIR)/../common` exists, the build system binds `BOARD_COMMON_DIR` to that sibling directory instead of `nuttx/boards/arm/rp23xx/common`. Therefore, `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph/common/` MUST NOT exist. Keeping `common/` absent lets `CONFIG_ARCH_BOARD_COMMON=y` resolve directly to `nuttx/boards/arm/rp23xx/common` for `rp23xx_common_earlyinitialize()`, `rp23xx_common_initialize()`, `rp23xx_common_bringup()`, `rp23xx_reset.c`, and `rp23xx_boardctl.c`.
2.  **Automatic `dummy_kconfig` Fallback**: When `$(BOARD_DIR)/Kconfig` is absent, `nuttx/tools/Config.mk` binds `BOARD_KCONFIG` to `boards/dummy/dummy_kconfig`, eliminating the need for an empty `board/Kconfig` file. Similarly, `nsh_sysinitscript()` (`apps/nshlib/nsh_script.c`) ignores a missing `/etc/init.d/rc.sysinit` silently (`log = false`), so only `etc/init.d/rcS` is required in ROMFS.
3.  **Relative Board Path Resolution**: `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"` resolves relative to `nuttx/` (the working directory from which `Makefile` invokes `./tools/configure.sh` and `$(MAKE) -C ../nuttx`).
4.  **Linker Script Reuse**: `board/scripts/Make.defs` sets `ARCHSCRIPT` to `$(TOPDIR)$(DELIM)boards$(DELIM)arm$(DELIM)rp23xx$(DELIM)xiao-rp2350$(DELIM)scripts$(DELIM)$(LDSCRIPT)` (`memmap_default.ld`) rather than copying the upstream RP2350 linker scripts out of tree.

### Hardware Pin Mapping & Setup

| XIAO Pin | RP2350 GPIO | Function / Polarity | NuttX Device | Notes |
| :--- | :--- | :--- | :--- | :--- |
| `D0` | `GPIO 26` | Analog Input (`ADC0`) | `/dev/adc0` | Potentiometer wiper (`0–3.3 V`; outer pins on `3.3 V` & `GND`); `IE` cleared after `rp23xx_common_bringup()`; optional `100 nF` ceramic capacitor from `D0` to `GND` for noisy breadboard jumpers |
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
| 1. Kernel Boot, ROMFS Mount, & Board Bringup                                |
|    rp23xx_boardearlyinitialize() -> rp23xx_common_earlyinitialize()         |
|    nx_bringup() -> nx_romfsetc() -> mounts ROMFS (/etc)                     |
|    board_late_initialize()       -> rp23xx_bringup()                        |
|      1) rp23xx_common_bringup()  -> rp23xx_adc_setup("/dev/adc0", ADC0)     |
|                                     (logs error on < 0, does not abort)     |
|      2) clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, GPIO(26..29))              |
|      3) userled_lower_initialize("/dev/userleds")          [6 outputs low]  |
+-----------------------------------------------------------------------------+
                                      |
                                      v
+-----------------------------------------------------------------------------+
| 2. NSH Initialization & Autonomous Background Launch                        |
|    nsh_main() -> nsh_initialize()                                           |
|      -> executes /etc/init.d/rcS                                            |
|      -> runs "bargraph" -> task_create("bargraph_daemon", ...) returns 0    |
|    nsh_consolemain()                                                        |
|      -> boardctl(BOARDIOC_USBDEV_CONNECT) [attaches USB pull-up]            |
|      -> blocks waiting for 3x Enter (\r or \n) on /dev/ttyACM0              |
+-----------------------------------------------------------------------------+
                                      |
                                      v  (runs concurrently at 20 Hz)
+-----------------------------------------------------------------------------+
| 3. bargraph_daemon 20 Hz Control Loop (stdio redirected to /dev/null)       |
|    open(adc_path, O_RDONLY) -> read(struct adc_msg_s) -> close()            |
|      |                                                                      |
|      +--> [error or am_data < 0] -> consecutive_errors++                    |
|      |      (< 5 ticks: sleep 50 ms; >= 5 ticks: log + 0x15/0x2A blink)     |
|      +--> [valid] -> consecutive_errors = 0; raw = am_data >> 19 (0..4095)  |
|                   -> quantize_while(raw, level, HYSTERESIS=24) -> 0..6      |
|                   -> if level changed: ioctl(led_fd, ULEDIOC_SETALL, mask)  |
|    usleep(50000)  [trailing ADC completion ISR detaches IRQ on count == 0]  |
+-----------------------------------------------------------------------------+
```

### Detailed Subsystem Mechanisms

#### Board Bringup and Analog Pad Input-Buffer Fix

*   **Constraint-Driven `IE` Leakage Workaround**: Because `nuttx/` remains unmodified, `rp23xx_bringup.c` works around `my_reset()` re-enabling `IE=1` during `rp23xx_common_bringup()` by looping over `GPIO 26..29` immediately afterward and calling `clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, RP23XX_PADS_BANK0_GPIO(gpio))`. `clrbits_reg32(v, a)` is defined in `nuttx/arch/arm/src/rp23xx/hardware/rp23xx_memorymap.h` and pulled in transitively via `"rp23xx_gpio.h"` (and `"hardware/rp23xx_pads_bank0.h"`). Any subsequent driver-level `ao_reset()` would re-enable `IE`, so `CONFIG_DEV_GPIO` is left disabled and `GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1` are omitted from `board.h` so no GPIO character driver touches `GPIO 26–29`.
*   **Decoupled Fault-Resilient Bringup**: `rp23xx_bringup()` logs any negative return from `rp23xx_common_bringup()` via `syslog(LOG_ERR, ...)` without returning early, then unconditionally runs the `GPIO 26–29` `IE` clear loop and `userled_lower_initialize("/dev/userleds")` before returning the aggregated error code. That way, even if `/dev/adc0` setup fails at boot, `/dev/userleds` still registers and `bargraph_daemon` can display CUJ 3's fault blink pattern.

#### 6-LED `/dev/userleds` Lower-Half

*   **Driver Selection**: With `# CONFIG_ARCH_LEDS is not set`, `CONFIG_BOARD_CUSTOM_LEDS=y` (`ARCH_HAVE_LEDS`), `CONFIG_USERLED=y`, and `CONFIG_USERLED_LOWER=y`, the board builds `rp23xx_userleds.c` instead of `rp23xx_autoleds.c`.
*   **Pin Mapping and Control**: `board_userled_initialize()` initializes all six pins in `g_ledcfg[6]` (`GPIO 5, 6, 7, 2, 4, 3`) as outputs driven low (`false`) and returns `BOARD_NLEDS` (`6`). `board_userled(int led, bool ledon)` updates index `0..5`, and `board_userled_all(uint32_t ledset)` loops across `0 <= i < BOARD_NLEDS`, writing `(ledset & (1U << i)) != 0` to `g_ledcfg[i]`.

#### ADC Sampling, Normalization, and Hysteresis

*   **Constraint-Driven Per-Tick `open` / `read` / `close` Cycle**: Because `nuttx/` is unmodified (`rp23xx_adc.c` ignores `RP23XX_ADC_DIV` and returns `-ENOTTY` for `ANIOC_TRIGGER`), opening `/dev/adc0` triggers `my_rxint(dev, true)` -> `add_device()`, which enables the RP2350 ADC FIFO, attaches `RP23XX_ADC_IRQ_FIFO`, and starts conversions. Reading `sizeof(struct adc_msg_s)` (5 bytes: 1-byte `am_channel` + 4-byte `int32_t am_data`) matches the `buflen % 5 == 0` branch in `nuttx/drivers/analog/adc.c` (note that `adc.c` also supports `buflen == 4` for raw single-channel `int32_t` reads; 5-byte `struct adc_msg_s` keeps explicit channel framing). Closing `/dev/adc0` immediately afterward triggers `my_shutdown()` -> `remove_device()`, decrementing `g_active_count` to `0` so the next FIFO completion interrupt (`interrupt_handler()`) disables `RP23XX_ADC_CS` and `RP23XX_ADC_INTE` and detaches `RP23XX_ADC_IRQ_FIFO` for the rest of the 50 ms sleep window.
*   **Error Filtering and Scaling**: `rp23xx_adc.c` shifts the 12-bit hardware result left by 19 bits (`value <<= 19`, range `0..0x7ff80000`) and negates `value` when `RP23XX_ADC_FIFO_ERR` is set. `read_adc_sample()` maps `sample.am_data < 0` to `-EAGAIN` and normalizes valid readings to `uint16_t raw = (uint16_t)(sample.am_data >> 19)` (`0..4095`). Every non-`EINTR` failure (including `-EAGAIN`) increments `consecutive_errors` and sleeps 50 ms while `consecutive_errors < 5`; any valid reading resets `consecutive_errors = 0`. Thus 1–4 tick conversion glitches are absorbed without changing the LEDs, while `>= 5` consecutive failed ticks (250 ms) trigger RAMLOG logging and `fault_blink_loop()`.
*   **7-Level Hysteresis Quantizer**: Full scale (`0..4095`) is divided into 7 buckets (`level` `0..6`) with nominal thresholds at multiples of `585` (`585, 1170, 1755, 2340, 2925, 3510`) and hysteresis band `HYSTERESIS = 24` counts (~0.6% of full scale). Using signed `int` arithmetic and `while` loops, `level` steps upward while `level < 6 && (int)raw >= thresholds[level] + hyst`, and downward while `level > 0 && (int)raw <= thresholds[level - 1] - hyst`, so large potentiometer turns converge in one 50 ms tick. The LED bitmask `mask = (level == 0) ? 0U : ((1U << level) - 1U)` (`0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f`) is written via `ioctl(led_fd, ULEDIOC_SETALL, (unsigned long)mask)` only when `level` changes.

#### Daemon Lifecycle, ROMFS Startup, and Headless Diagnostics

*   **Flat Address-Space Singleton**: Modeled on `apps/examples/leds/leds_main.c`, `bargraph_main.c` runs inside NuttX's flat address space and wraps its `g_bargraph_running` / `g_bargraph_pid` check-and-set in `sched_lock()` / `sched_unlock()` (verifying `g_bargraph_pid > 0 && kill(g_bargraph_pid, 0) == 0` so a `SIGKILL`ed task does not lock out future launches). Duplicate invocations print `"bargraph: daemon already running\n"` and exit `0`. An optional CLI argument (`bargraph [adc_devpath]`, matching `apps/examples/adc/adc_main.c`) overrides `/dev/adc0` for runtime CUJ 3 testing without rebuilding.
*   **Stdio Isolation and `SIGTERM` Lifecycle**: While `apps/nshlib/nsh_usbconsole.c` already invokes `nsh_nullstdio()` during boot before USB readiness, `bargraph_daemon` redirects FDs `0, 1, 2` to `/dev/null` via `dup2()` (closing the temporary descriptor if `fd > 2`) so interactive restarts from `nsh>` never hold `/dev/ttyACM0` open. Its `SIGTERM` handler (`SA_SIGINFO`, no `SA_RESTART`, no `syslog()` inside signal context) sets `g_bargraph_stop = 1`, breaking either loop, clearing all LEDs (`ULEDIOC_SETALL(0UL)`), closing `/dev/userleds`, and resetting `g_bargraph_running = 0` on every exit path.
*   **ROMFS Preprocessing and Build Packaging**: `nuttx/boards/Board.mk` preprocesses `etc/init.d/rcS` with `cpp` (requiring `#include <nuttx/config.h>`, C `/* ... */` comments, and `#ifdef CONFIG_BARGRAPH`), packages the tree with `genromfs`, and converts it with `xxd` and `sed` into `etctmp.c` inside `nuttx/boards/arm/rp23xx/common/`.
*   **RAMLOG Diagnostics and Fault Loop**: Startup (`task_create()` failure) and runtime errors log via `syslog(LOG_ERR, ...)` to RAMLOG (`CONFIG_RAMLOG_SYSLOG=y`, `CONFIG_RAMLOG_BUFSIZE=4096`), readable via `dmesg` once USB NSH attaches. After `>= 5` consecutive failed sampling ticks, the daemon logs the error and alternates `ULEDIOC_SETALL` between `0x15` and `0x2A` every 250 ms until `SIGTERM` arrives.

## Alternatives Considered

### Alternative A: Modifying `nuttx/boards/arm/rp23xx/xiao-rp2350` In-Place

*   **Pros**: Avoids creating out-of-tree `scripts/Make.defs` and `rp23xx_boardinitialize.c` files.
*   **Reason for Rejection**: Dirties the `nuttx` git submodule and mixes breadboard-specific pin assignments into upstream board files instead of isolating the project under `xiao-rp2350-bargraph/` (`CONFIG_ARCH_BOARD_CUSTOM=y`).

### Alternative B: Driving 8 Breadboard LEDs (Using `D1`/`D2` `GPIO 27–28` or `D6`/`D7` `GPIO 0–1`)

*   **Pros**: Provides 8 bar-graph steps instead of 6.
*   **Reason for Rejection**: `rp23xx_adc.c` `my_reset()` resets `GPIO 26–29` to floating inputs during `adc_register()`, clobbering `D1` (`GPIO 27`) and `D2` (`GPIO 28`), while claiming `D6`/`D7` (`GPIO 0`/`1`) occupies the hardware `UART0` pins. Six LEDs on `D3–D5` and `D8–D10` satisfy the 0–6 bar-graph requirement without touching `GPIO 27–28` or `UART0`.

### Alternative C: Keeping `/dev/adc0` Continuously Open with `ANIOC_RESET_FIFO`

*   **Pros**: Eliminates per-tick `open()` and `close()` syscalls every 50 ms.
*   **Reason for Rejection**: `ANIOC_RESET_FIFO` (`nuttx/include/nuttx/analog/ioctl.h`) only resets the upper-half ring buffer head/tail pointers in `nuttx/drivers/analog/adc.c`; it does not pause lower-half conversions (`my_ioctl()` in `rp23xx_adc.c` returns `-ENOTTY` for `ANIOC_TRIGGER` and configures no clock divider). Keeping `/dev/adc0` open leaves the RP2350 ADC firing ~500 kHz FIFO interrupts during `usleep(50000)` while `adc_receive()` drops new samples once the 8-entry upper-half ring buffer fills (`ad_isovr` is not set by `adc_receive()`). Per-tick `open`/`read`/`close` stops conversions between ticks.

### Alternative D: Individual `/dev/gpout*` Character Devices (`CONFIG_DEV_GPIO=y`)

*   **Pros**: Avoids writing a custom `rp23xx_userleds.c` pin table.
*   **Reason for Rejection**: Opening six `/dev/gpout*` descriptors alongside stdin/stdout/stderr and `/dev/adc0` forces allocating a second 6-entry descriptor block (`CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=6` in `nuttx/fs/inode/fs_files.c`), requires six `ioctl(GPIOC_WRITE)` (GPIO output write command) syscalls per update, and exposes `GPIO 26–28` if `board.h` retains upstream GPIO macros. `/dev/userleds` updates all 6 LEDs in one `ULEDIOC_SETALL` call over one descriptor.

### Alternative E: Hardware `UART0` Console (`D6`/`D7`)

*   **Pros**: Outputs console messages at boot without waiting for USB enumeration or three Enter keystrokes.
*   **Reason for Rejection**: Requires wiring an external USB-to-UART adapter to `D6`/`D7` (`GPIO 0`/`1`) instead of using the board's built-in USB-C port (`usbnsh`).

### Alternative F: Setting `CONFIG_INIT_ENTRYPOINT` Directly to the Bar-Graph App

*   **Pros**: Eliminates `CONFIG_ETC_ROMFS`, `genromfs`, `xxd`, and `NSH_USBCONSOLE` configuration.
*   **Reason for Rejection**: Removes `nsh>`, `dmesg`, `ps`, `free`, and interactive daemon control (`kill -15`, restart) over USB-C.

### Alternative G: Board-Local Single-Shot ADC Lower-Half Driver or Software Multi-Sample Averaging

*   **Pros**: A custom RP2350 single-shot ADC driver inside `board/src/` eliminates the sub-millisecond conversion burst during the `open`–`close` window and the trailing-ISR detach delay; software averaging reduces single-sample jitter.
*   **Reason for Rejection**: Reimplementing ~660 lines of `rp23xx_adc.c` and `rp23xx_common_bringup.c` out of tree duplicates an existing upstream SoC driver solely to trim a short per-tick sampling window, and software multi-sample averaging is unnecessary when the `±24`-count hysteresis band (plus optional `100 nF` decoupling capacitor) already prevents boundary flicker.

## Non-Functional Requirements

*   **CPU and Interrupt Load**: Conversions execute only during the per-tick `open("/dev/adc0")`–`close()` window and stop on the first FIFO completion interrupt after `g_active_count` reaches `0` (typically a few interrupts per 50 ms tick when unpreempted; task preemption between `open()` and `close()` extends that active window).
*   **SRAM Footprint**: With 520 KB configured SRAM (`CONFIG_RAM_SIZE=532480` in `usbnsh/defconfig`), `CONFIG_RAMLOG_BUFSIZE=4096` (4 KB) and a 2048-byte daemon stack (`CONFIG_BARGRAPH_STACKSIZE=2048`) add ~6 KB of runtime RAM (excluding `.bss`/`.data` and ROMFS image size), to be verified against the `memmap_default.ld` linker memory report at build time and `ps` stack output on target.
*   **Rollback and Recovery**: The daemon installs a `SIGTERM` handler so `kill -15 <pid>` clears all LEDs, closes `/dev/userleds`, and exits. If a firmware build faults at boot, holding `BOOT` (`BOOTSEL`) while resetting the XIAO RP2350 enters the RP2350 ROM UF2 bootloader for re-flashing.

## Risks & Limits

*   **Breadboard LED Current Budget**: Driving 6 active-high LEDs simultaneously (`0x3f`) on `GPIO 2–7` with `>= 330 Ω` series resistors draws ~4–6 mA per pin (~24–36 mA total), remaining within the RP2350 per-GPIO drive ratings (up to 12 mA max standard drive) and 50 mA total `IOVDD` package limit.
*   **ADC Top-End Saturation, Reference Ripple, and Open-Wiper Floating Input**: Nominal thresholds at multiples of `585` (`level 6` entry at `3510 + 24 = 3534`) reserve the top ~14% of full scale (`3534..4095`) so all 6 LEDs illuminate before the wiper reaches the `3.3 V` mechanical end-stop. Because RP2350 uses `3.3 V` as the ADC reference and `GPIO 26` has `IE=0` and pulls disabled, `3.3 V` rail ripple exceeding ~19 mV (`±24` counts) can still cross boundaries, and an open/disconnected potentiometer wiper floats to arbitrary LED levels without triggering CUJ 3. Mitigation: keep the wiper connected, and fit the optional `100 nF` capacitor (`D0` to `GND`) per Hardware Pin Mapping if jumper pickup exceeds `±24` counts.
*   **Exclusive `/dev/adc0` Ownership, Rapid-Restart Race, and Blocking `read()` Stall**:
    *   Per-tick `close()` halts conversions only when `g_active_count` drops to `0`, and `adc_open()`/`adc_close()` in `adc.c` hold `nxmutex_lock` rather than a critical section while `remove_device()` leaves `RP23XX_ADC_IRQ_FIFO` armed until the trailing ISR runs. Mitigation: keep `bargraph_daemon` as the sole opener (`CONFIG_EXAMPLES_ADC` unset) and avoid sub-millisecond `kill -15` + `bargraph` restart loops.
    *   `adc_read()` blocks in `nxsem_wait(&fifo->af_sem)`; if the ADC hardware clock or FIFO IRQ stalls after `open()` succeeds, `read()` blocks rather than incrementing `consecutive_errors` (whereas `open()` failure or `RP23XX_ADC_FIFO_ERR` triggers CUJ 3).
*   **`0`-Sample `FIFO_ERR` Sign Limitation**: `rp23xx_adc.c` signals `RP23XX_ADC_FIFO_ERR` by negating `value` (`value = -value`), so a hardware `FIFO_ERR` coinciding with a raw `0` reading remains `0` and is indistinguishable from a valid `0 V` sample (`level 0`). Mitigation: sustained conversion faults at non-zero wiper voltages produce negative values across 5 consecutive ticks and enter `fault_blink_loop()`.
*   **RAMLOG Circular Overwrite**: `CONFIG_RAMLOG_BUFSIZE=4096` overwrites older entries in FIFO order if logs exceed 4 KB before USB NSH attaches. Mitigation: log only boot/bringup errors, daemon start/exit, and fault transitions (never per-tick samples).
*   **Security Implications**: Not applicable (standalone, non-networked local breadboard firmware with physical USB-C/GPIO access only).

## Additional Resources

*   [nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/Make.defs](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/Make.defs)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/memmap_default.ld](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/memmap_default.ld)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_bringup.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_bringup.c)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_userleds.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_userleds.c)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig)
*   [nuttx/boards/arm/rp23xx/common/src/rp23xx_common_initialize.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/common/src/rp23xx_common_initialize.c)
*   [nuttx/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c)
*   [nuttx/boards/arm/rp23xx/common/src/rp23xx_reset.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/common/src/rp23xx_reset.c)
*   [nuttx/boards/arm/rp23xx/common/src/rp23xx_boardctl.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/common/src/rp23xx_boardctl.c)
*   [nuttx/arch/arm/src/rp23xx/rp23xx_adc.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/arch/arm/src/rp23xx/rp23xx_adc.c)
*   [nuttx/arch/arm/src/rp23xx/rp23xx_gpio.h](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/arch/arm/src/rp23xx/rp23xx_gpio.h)
*   [nuttx/arch/arm/src/rp23xx/hardware/rp23xx_memorymap.h](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/arch/arm/src/rp23xx/hardware/rp23xx_memorymap.h)
*   [nuttx/drivers/analog/adc.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/drivers/analog/adc.c)
*   [nuttx/fs/inode/fs_files.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/fs/inode/fs_files.c)
*   [nuttx/sched/init/nx_bringup.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/sched/init/nx_bringup.c)
*   [nuttx/sched/group/group_setupidlefiles.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/sched/group/group_setupidlefiles.c)
*   [nuttx/boards/Board.mk](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/Board.mk)
*   [nuttx/boards/Kconfig](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/Kconfig)
*   [nuttx/tools/Config.mk](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/tools/Config.mk)
*   [nuttx/tools/Unix.mk](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/tools/Unix.mk)
*   [nuttx/tools/rp23xx/Config.mk](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/tools/rp23xx/Config.mk)
*   [apps/Application.mk](file:///home/brett/Documents/GitHub/nuttxspace/apps/Application.mk)
*   [apps/Directory.mk](file:///home/brett/Documents/GitHub/nuttxspace/apps/Directory.mk)
*   [apps/system/nsh/nsh_main.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/system/nsh/nsh_main.c)
*   [apps/nshlib/nsh_init.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/nshlib/nsh_init.c)
*   [apps/nshlib/nsh_script.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/nshlib/nsh_script.c)
*   [apps/nshlib/nsh_usbconsole.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/nshlib/nsh_usbconsole.c)
*   [apps/examples/leds/leds_main.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/examples/leds/leds_main.c)
*   [apps/examples/adc/adc_main.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/examples/adc/adc_main.c)

## Future Improvements

*   Add an ADC sample-rate divider or timer-triggered DMA mode to `rp23xx_adc.c` upstream so continuous-open reads can run at 20 Hz without per-tick `open`/`close`.
*   Add PWM duty-cycle smoothing across intermediate potentiometer positions on spare RP2350 PWM slices.

--------------------------------------------------------------------------------

## Detailed Implementation Plan

All files below are created inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`. No tracked files inside `nuttx/` or `apps/` are edited; only the gitignored `apps/external` symlink, generated `external/Kconfig` / `apps/Kconfig`, and build outputs are created during configuration and compilation (the build also generates temporary `nuttx/boards/arm/rp23xx/common/etctmp.c`, which `Makefile` removes or cleans so `git status` remains clean).

### Target Files to Create (Phased Delivery)

#### Phase 1: Out-of-Tree Build Wrapper, Custom Board Skeleton, and `defconfig`

##### 1. `xiao-rp2350-bargraph/Makefile` (Create)

*   **Type / Scope**: Top-level GNU Make build and configuration wrapper.
*   **Rationale**: Keeps one-command configuration and building self-contained outside `nuttx/` while managing the `apps/external` symlink and Kconfig regeneration safely across phases.
*   **Proposed Changes**:
    *   `setup` target: Only when `external/Makefile` exists (preventing a dangling symlink during Phases 1–2), creates or verifies `../apps/external -> ../xiao-rp2350-bargraph/external` (aborting if `../apps/external` exists as a non-symlink directory); forces regeneration of `../apps/Kconfig` and `external/Kconfig` whenever `../apps/external` was just created, `external/Kconfig` is missing, or `external/bargraph/Kconfig` is newer than `../apps/Kconfig` (running `$(MAKE) -C ../nuttx olddefconfig` if `../nuttx/.config` already exists).
    *   `configure` target: Depends on `setup`; runs `cd ../nuttx && ./tools/configure.sh -e -l ../xiao-rp2350-bargraph/board/configs/bargraph` (`-e` runs `distclean` if configuration changed; `-l` selects the Linux host environment).
    *   `all` (default) target: Depends on `setup`; runs `configure` if `../nuttx/.config` is missing, runs `$(MAKE) -C ../nuttx` to produce `../nuttx/nuttx.uf2`, and removes temporary untracked `../nuttx/boards/arm/rp23xx/common/etctmp.c` if generated so `nuttx/` stays untracked-clean.
    *   `clean` target: Runs `$(MAKE) -C ../nuttx clean` if `../nuttx/Make.defs` and `../nuttx/.config` exist, and removes `../nuttx/boards/arm/rp23xx/common/etctmp.c` if present.
    *   `distclean` target: Runs `$(MAKE) -C ../nuttx distclean` if configured, then removes `../apps/external`, `external/Kconfig`, `../apps/Kconfig`, and any leftover `etctmp.c`.

##### 2. `xiao-rp2350-bargraph/board/include/board.h` (Create)

*   **Type / Scope**: Custom board header (`<arch/board/board.h>`).
*   **Rationale**: Defines RP2350 clock frequencies, omits upstream `GPIO 26–28` generic GPIO macros (`GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1`) so `CONFIG_DEV_GPIO` cannot claim ADC or avoided pins, and sets `BOARD_NLEDS = 6`.
*   **Proposed Changes**:
    *   Retain all 13 clock macros (`MHZ`, `BOARD_XOSC_FREQ`, `BOARD_XOSC_STARTUPDELAY`, `BOARD_PLL_SYS_FREQ`, `BOARD_PLL_USB_FREQ`, `BOARD_REF_FREQ`, `BOARD_SYS_FREQ`, `BOARD_PERI_FREQ`, `BOARD_USB_FREQ`, `BOARD_ADC_FREQ`, `BOARD_HSTX_FREQ`, `BOARD_UART_BASEFREQ`, `BOARD_TICK_CLOCK`) and `BOARD_GPIO_LED_PIN = 25` from `nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h`.
    *   Omit `"rp23xx_i2cdev.h"`, `"rp23xx_spidev.h"`, `"rp23xx_i2sdev.h"`, and `"rp23xx_spisd.h"`, omit `BOARD_NGPIOOUT`/`BOARD_NGPIOIN`/`BOARD_NGPIOINT`/`GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1`, and omit `BOARD_LED1`/`BOARD_LED_YELLOW`/`BOARD_LED1_BIT`/`LED_STARTED`..`LED_PANIC` (`CONFIG_ARCH_LEDS` is disabled).
    *   Define `BOARD_NLEDS` (`6`) and declare `void rp23xx_boardearlyinitialize(void);` and `void rp23xx_boardinitialize(void);`.

##### 3. `xiao-rp2350-bargraph/board/scripts/Make.defs` (Create)

*   **Type / Scope**: Custom board compiler flags and linker script Makefile fragment.
*   **Rationale**: `tools/configure.sh` symlinks `nuttx/Make.defs` to `$(BOARD_DIR)/scripts/Make.defs`.
*   **Proposed Changes**:
    *   Include `$(TOPDIR)/.config`, `$(TOPDIR)/tools/Config.mk`, `$(TOPDIR)/tools/rp23xx/Config.mk`, and `$(TOPDIR)/arch/arm/src/armv8-m/Toolchain.defs`.
    *   Set `LDSCRIPT = memmap_default.ld` (matching `CONFIG_BOOT_RUNFROMFLASH=y`) and `ARCHSCRIPT += $(TOPDIR)$(DELIM)boards$(DELIM)arm$(DELIM)rp23xx$(DELIM)xiao-rp2350$(DELIM)scripts$(DELIM)$(LDSCRIPT)`.
    *   Set `CFLAGS`, `CXXFLAGS`, `CPPFLAGS`, and `AFLAGS` matching `nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/Make.defs` (omitting unused `NXFLATLDFLAGS*`).

##### 4. `xiao-rp2350-bargraph/board/configs/bargraph/defconfig` (Create)

*   **Type / Scope**: Compressed NuttX `.config` template derived from `nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig`.
*   **Rationale**: Configures the custom board, common RP2350 helpers, late board init, `ADC0`, 6-LED `userleds`, ROMFS `/etc/init.d/rcS`, USB CDC/ACM NSH, RAMLOG `dmesg`, and (in Phase 3) `CONFIG_BARGRAPH=y`.
*   **Proposed Changes**:
    *   **Removals relative to `usbnsh/defconfig` (5 lines)**: Remove `CONFIG_ARCH_BOARD="xiao-rp2350"`, `CONFIG_ARCH_BOARD_XIAO_RP2350=y`, `CONFIG_EXAMPLES_HELLO=y`, `CONFIG_TESTING_GETPRIME=y`, and `CONFIG_TESTING_OSTEST=y` (`CONFIG_DEV_GPIO` is not in `usbnsh/defconfig` and remains unset).
    *   **Retained baseline symbols from `usbnsh/defconfig` (exact 38 remaining lines)**: `# CONFIG_DEV_CONSOLE is not set`, `# CONFIG_NSH_ARGCAT is not set`, `# CONFIG_NSH_CMDOPT_HEXDUMP is not set`, `# CONFIG_NSH_DISABLE_DATE is not set`, `# CONFIG_NSH_DISABLE_LOSMART is not set`, `# CONFIG_RP23XX_UART0 is not set`, `CONFIG_ARCH="arm"`, `CONFIG_ARCH_BOARD_COMMON=y`, `CONFIG_ARCH_CHIP="rp23xx"`, `CONFIG_ARCH_CHIP_RP23XX=y`, `CONFIG_ARCH_RAMVECTORS=y`, `CONFIG_ARCH_STACKDUMP=y`, `CONFIG_BOARDCTL_RESET=y`, `CONFIG_BOARD_LOOPSPERMSEC=10450`, `CONFIG_BUILTIN=y`, `CONFIG_CDCACM=y`, `CONFIG_CDCACM_CONSOLE=y`, `CONFIG_DEBUG_FULLOPT=y`, `CONFIG_DEBUG_SYMBOLS=y`, `CONFIG_DISABLE_POSIX_TIMERS=y`, `CONFIG_FS_PROCFS=y`, `CONFIG_FS_PROCFS_REGISTER=y`, `CONFIG_INIT_ENTRYPOINT="nsh_main"`, `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=6`, `CONFIG_NSH_BUILTIN_APPS=y`, `CONFIG_NSH_READLINE=y`, `CONFIG_NSH_USBCONSOLE=y`, `CONFIG_RAM_SIZE=532480`, `CONFIG_RAM_START=0x20000000`, `CONFIG_READLINE_CMD_HISTORY=y`, `CONFIG_RR_INTERVAL=200`, `CONFIG_SCHED_WAITPID=y`, `CONFIG_START_DAY=9`, `CONFIG_START_MONTH=2`, `CONFIG_START_YEAR=2021`, `CONFIG_SYSTEM_NSH=y`, `CONFIG_USBDEV=y`, and `CONFIG_USBDEV_BUSPOWERED=y` (keeping `ps` and `kill` enabled by leaving `CONFIG_NSH_DISABLE_PS`, `CONFIG_NSH_DISABLE_KILL`, and `CONFIG_DISABLE_ALL_SIGNALS` unset).
    *   **New / modified symbols**:
        *   Custom board & init: `CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_ARCH_BOARD_CUSTOM_NAME="xiao-rp2350-bargraph"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y`, `CONFIG_BOARD_CUSTOM_LEDS=y`, `# CONFIG_ARCH_LEDS is not set`, `CONFIG_BOARD_LATE_INITIALIZE=y`, `CONFIG_DEV_NULL=y`.
        *   Drivers: `CONFIG_ANALOG=y`, `CONFIG_ADC=y`, `CONFIG_RP23XX_ADC=y`, `CONFIG_RP23XX_ADC_CHANNEL0=y`, `CONFIG_USERLED=y`, `CONFIG_USERLED_LOWER=y` (`CONFIG_BARGRAPH=y` is added in Phase 3 once `external/bargraph/Kconfig` exists).
        *   ROMFS & RAMLOG: `CONFIG_FS_ROMFS=y`, `CONFIG_ETC_ROMFS=y`, `# CONFIG_NSH_DISABLESCRIPT is not set`, `CONFIG_RAMLOG=y`, `CONFIG_RAMLOG_SYSLOG=y`, `CONFIG_RAMLOG_BUFSIZE=4096`. (`CONFIG_RP23XX_ROMFS_ROMDISK_DEVNAME`, which configures `rp23xx_common_bringup.c`'s separate `romdisk_register(..., rp23xx_romfs_img, ...)` mount, remains unset so only `CONFIG_ETC_ROMFS` / `nx_romfsetc()` mounts `/etc`.)

*   **Phase 1 Checkpoint**: `make -C xiao-rp2350-bargraph configure` configures `nuttx/.config` with zero errors (verify `CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_RP23XX_ADC=y`, `CONFIG_USERLED=y`, `CONFIG_ETC_ROMFS=y`, and `CONFIG_RAMLOG_SYSLOG=y` in `nuttx/.config`).

#### Phase 2: Board Bringup, Analog Pad `IE` Fix, 6-LED Driver, and ROMFS Startup Script

##### 5. `xiao-rp2350-bargraph/board/src/Make.defs` (Create)

*   **Type / Scope**: Board source and ROMFS script build list.
*   **Rationale**: Registers board initialization, bringup, 6-LED driver, and ROMFS `/etc/init.d/rcS` with `nuttx/boards/Board.mk`. With `CONFIG_ARCH_BOARD_COMMON=y`, `nuttx/tools/Unix.mk` (during `make` `context`) symlinks `arch/arm/src/board` to `nuttx/boards/arm/rp23xx/common` and `arch/arm/src/board/board` to `$(BOARD_DIR)/src`, which is why the custom board path uses `board/board`.
*   **Proposed Changes**:
    *   Add `rp23xx_boardinitialize.c` and `rp23xx_bringup.c` to `CSRCS`.
    *   Under `ifneq ($(CONFIG_ARCH_LEDS),y)`, add `rp23xx_userleds.c` to `CSRCS`.
    *   Under `ifeq ($(CONFIG_ETC_ROMFS),y)`, set `RCSRCS = etc/init.d/rcS`.
    *   Set `DEPPATH += --dep-path board`, `VPATH += :board`, and `CFLAGS += ${INCDIR_PREFIX}$(TOPDIR)$(DELIM)arch$(DELIM)$(CONFIG_ARCH)$(DELIM)src$(DELIM)board$(DELIM)board`.

##### 6. `xiao-rp2350-bargraph/board/src/rp23xx_pico.h` (Create)

*   **Type / Scope**: Internal board source header.
*   **Rationale**: Centralizes the 6 breadboard LED GPIO numbers and ADC pin range constants shared by `rp23xx_userleds.c`, `rp23xx_bringup.c`, and `rp23xx_boardinitialize.c`.
*   **Proposed Changes**:
    *   Define `GPIO_ADC_BASE` (`26`), `GPIO_ADC_COUNT` (`4`), `GPIO_LED1` (`5`), `GPIO_LED2` (`6`), `GPIO_LED3` (`7`), `GPIO_LED4` (`2`), `GPIO_LED5` (`4`), and `GPIO_LED6` (`3`).
    *   Declare `int rp23xx_bringup(void);`.

##### 7. `xiao-rp2350-bargraph/board/src/rp23xx_boardinitialize.c` (Create)

*   **Type / Scope**: Early, standard, and late board initialization hooks.
*   **Rationale**: Invokes shared RP2350 common initialization and exposes `board_late_initialize()` so `rp23xx_bringup()` runs before NSH executes `/etc/init.d/rcS`.
*   **Proposed Changes**:
    *   `rp23xx_boardearlyinitialize()`: Calls `rp23xx_common_earlyinitialize()` and configures `BOARD_GPIO_LED_PIN` (`GPIO 25`) as output driven `true` (active-low onboard LED off).
    *   `rp23xx_boardinitialize()`: Calls `rp23xx_common_initialize()`.
    *   `board_late_initialize()`: Guarded by `#ifdef CONFIG_BOARD_LATE_INITIALIZE`, calls `(void)rp23xx_bringup();`.

##### 8. `xiao-rp2350-bargraph/board/src/rp23xx_bringup.c` (Create)

*   **Type / Scope**: Peripheral driver bringup and analog pad fix.
*   **Rationale**: Registers `/dev/adc0` via `rp23xx_common_bringup()`, clears `RP23XX_PADS_BANK0_GPIO_IE` across `GPIO 26–29`, and registers `/dev/userleds` even if `/dev/adc0` registration fails.
*   **Proposed Changes**:
    *   Include `<nuttx/config.h>`, `<nuttx/debug.h>`, `<sys/types.h>`, `<syslog.h>`, `<arch/board/board.h>`, `<nuttx/leds/userled.h>`, `"rp23xx_gpio.h"` (which brings in `hardware/rp23xx_memorymap.h` for `clrbits_reg32()` and `hardware/rp23xx_pads_bank0.h`), `"rp23xx_common_bringup.h"`, and `"rp23xx_pico.h"`.
    *   In `int rp23xx_bringup(void)`:
        1. Call `int ret = rp23xx_common_bringup();`; if `ret < 0`, log via `syslog(LOG_ERR, ...)` **without returning early** (noting that `rp23xx_common_bringup()` returns ADC setup status when timer/WS2812/watchdog/ROMFS blocks are disabled).
        2. Under `#if defined(CONFIG_ADC) && defined(CONFIG_RP23XX_ADC)`, loop `for (int i = 0; i < GPIO_ADC_COUNT; i++)` and call `clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, RP23XX_PADS_BANK0_GPIO(GPIO_ADC_BASE + i))`.
        3. Under `#ifdef CONFIG_USERLED`, call `int led_ret = userled_lower_initialize("/dev/userleds");`; if `led_ret < 0`, log via `syslog(LOG_ERR, ...)` and update `if (ret == OK) ret = led_ret;`.
        4. `return ret;` (note: upstream `xiao-rp2350/src/rp23xx_bringup.c` returns `OK` even on `userled_lower_initialize()` failure, whereas this board intentionally returns the aggregated `ret`).

##### 9. `xiao-rp2350-bargraph/board/src/rp23xx_userleds.c` (Create)

*   **Type / Scope**: Multi-LED `userleds` lower-half driver (`!CONFIG_ARCH_LEDS`).
*   **Rationale**: Replaces upstream `xiao-rp2350/src/rp23xx_userleds.c` which hardcodes a single LED (`GPIO 25`) in `board_userled_all()`.
*   **Proposed Changes**:
    *   Wrap implementation in `#ifndef CONFIG_ARCH_LEDS`; use `static const uint32_t g_ledcfg[BOARD_NLEDS] = { GPIO_LED1, GPIO_LED2, GPIO_LED3, GPIO_LED4, GPIO_LED5, GPIO_LED6 };`.
    *   Implement `board_userled_initialize()` (configure all 6 pins as outputs driven `false`, return `BOARD_NLEDS`), `board_userled(int led, bool ledon)`, and `board_userled_all(uint32_t ledset)`.

##### 10. `xiao-rp2350-bargraph/board/src/etc/init.d/rcS` (Create)

*   **Type / Scope**: ROMFS NSH startup script processed by `Board.mk` via `cpp`.
*   **Rationale**: Launches `bargraph` automatically at power-on before `nsh_consolemain()` waits for USB attachment.
*   **Proposed Changes**:
    *   Include `<nuttx/config.h>` and use C-style `/* ... */` comments (no `#` shell comments).
    *   Under `#ifdef CONFIG_BARGRAPH`, invoke `bargraph`.

*   **Phase 2 Checkpoint**: Running `make -C xiao-rp2350-bargraph` builds a bootable `../nuttx/nuttx.uf2` image with `/dev/adc0`, `/dev/userleds`, and `/etc/init.d/rcS` compiled via `etctmp.c`, leaving `git status` clean in `nuttx/` and `apps/`.

#### Phase 3: External `bargraph` Daemon Application

##### 11. `xiao-rp2350-bargraph/external/Makefile` and `Make.defs` (Create)

*   **Type / Scope**: `apps/external` directory glue (`apps/` scans `apps/external` automatically when present).
*   **Rationale**: Integrates `external/bargraph` into the NuttX `apps/` build and `preconfig` tree without editing `apps/Makefile` or `apps/Kconfig`.
*   **Proposed Changes**:
    *   `external/Makefile`: Set `MENUDESC = "External"` and include `$(APPDIR)/Directory.mk`.
    *   `external/Make.defs`: `include $(wildcard $(APPDIR)/external/*/Make.defs)`.

##### 12. `xiao-rp2350-bargraph/external/bargraph/Kconfig` (Create)

*   **Type / Scope**: Application configuration symbols.
*   **Rationale**: Exposes `CONFIG_BARGRAPH` and bounded daemon parameters to `defconfig` and `rcS` (keeping fixed hardware paths `/dev/adc0` and `/dev/userleds` as constants/CLI override rather than speculative Kconfig knobs).
*   **Proposed Changes**:
    *   Define `config BARGRAPH` (`tristate "Potentiometer LED bar graph daemon"`, `default n`, `depends on ADC && USERLED && USERLED_LOWER && BUILTIN`).
    *   Under `if BARGRAPH`, define `BARGRAPH_PROGNAME` (`"bargraph"`), `BARGRAPH_PRIORITY` (`100`, `range 1 255`), `BARGRAPH_STACKSIZE` (`2048`, `range 1024 8192`), `BARGRAPH_INTERVAL_US` (`50000`, `range 1000 1000000`), and `BARGRAPH_HYSTERESIS` (`24`, `range 0 584`).

##### 13. `xiao-rp2350-bargraph/external/bargraph/Makefile` and `Make.defs` (Create)

*   **Type / Scope**: Application build registration.
*   **Rationale**: Registers `bargraph` as a built-in NSH application compiled from `bargraph_main.c`.
*   **Proposed Changes**:
    *   `Make.defs`: `ifneq ($(CONFIG_BARGRAPH),)` -> `CONFIGURED_APPS += $(APPDIR)/external/bargraph`.
    *   `Makefile`: Include `$(APPDIR)/Make.defs`, set `PROGNAME`, `PRIORITY`, `STACKSIZE`, `MODULE`, `MAINSRC = bargraph_main.c`, and include `$(APPDIR)/Application.mk`.

##### 14. `xiao-rp2350-bargraph/external/bargraph/bargraph_main.c` and `defconfig` Update (Create / Update)

*   **Type / Scope**: User-space C singleton daemon (`main` + `bargraph_daemon`) and enabling `CONFIG_BARGRAPH=y` in `board/configs/bargraph/defconfig`.
*   **Rationale**: Implements the 20 Hz potentiometer-to-bar-graph loop, per-tick ADC open/read/close shutdown, hysteresis smoothing, stdio isolation, `SIGTERM` shutdown, and headless LED fault blinking.
*   **Proposed Changes**:
    *   **State**: `static volatile sig_atomic_t g_bargraph_running = 0;`, `static volatile sig_atomic_t g_bargraph_stop = 0;`, and `static pid_t g_bargraph_pid = -1;` (protected by `sched_lock()` / `sched_unlock()` in `main()`).
    *   **CLI & `main()`**: Accepts optional `argv[1]` ADC device path override (default `"/dev/adc0"`, passed as `argv` to `task_create()`). Under `sched_lock()`, if `g_bargraph_running != 0 && g_bargraph_pid > 0 && kill(g_bargraph_pid, 0) == 0`, prints `"bargraph: daemon already running\n"` and returns `EXIT_SUCCESS`. Otherwise sets `g_bargraph_stop = 0`, `g_bargraph_running = 1`, spawns `bargraph_daemon` via `task_create()`, records `g_bargraph_pid`, and resets `g_bargraph_running = 0` on `task_create() < 0`.
    *   **Stdio & Signal Setup**: `bargraph_daemon` redirects FDs `0, 1, 2` to `/dev/null` via `dup2()` and closes the opened descriptor if `fd > 2`. Initializes `struct sigaction act` with `memset(&act, 0, sizeof(act))`, `sigemptyset(&act.sa_mask)`, `act.sa_sigaction = sigterm_handler`, and `act.sa_flags = SA_SIGINFO` (no `SA_RESTART`), checking `sigaction(SIGTERM, &act, NULL) != 0` for error exit (`sigterm_handler` sets only `g_bargraph_stop = 1`, without calling `syslog()`).
    *   **ADC Read (`read_adc_sample`)**: Opens `adc_path` (`O_RDONLY`), reads `sizeof(struct adc_msg_s)` (5 bytes), captures `int err = errno;` immediately before `close(fd)`, returns `-EINTR` if `nbytes < 0 && err == EINTR`, `-err` on other `nbytes < 0`, `-EIO` if `nbytes != (ssize_t)sizeof(struct adc_msg_s)`, `-EAGAIN` if `sample.am_data < 0`, or sets `*raw_out = (uint16_t)(sample.am_data >> 19)` and returns `0`.
    *   **Quantizer (`quantize_level`)**: Uses signed `int` thresholds `{ 585, 1170, 1755, 2340, 2925, 3510 }` and `hyst = CONFIG_BARGRAPH_HYSTERESIS`: loops `while (level < 6 && (int)raw >= thresholds[level] + hyst) level++;` and `while (level > 0 && (int)raw <= thresholds[level - 1] - hyst) level--;`.
    *   **Daemon Loop & Fault Blink**: Initializes `int current_level = 0;` and `int consecutive_errors = 0;`. Opens `"/dev/userleds"` (`O_WRONLY`) and clears LEDs (`ULEDIOC_SETALL(0UL)`), exiting `EXIT_FAILURE` on open/`ioctl` error. Each tick:
        *   Calls `read_adc_sample(adc_path, &raw)`.
        *   If `ret == -EINTR` or `g_bargraph_stop`: exits loop.
        *   If `ret < 0` (including `-EAGAIN`): increments `consecutive_errors`. When `consecutive_errors == 5`, logs once via `syslog(LOG_ERR, ...)`, runs `fault_blink_loop(led_fd)` (alternating `0x15UL` and `0x2aUL` every `250000` us while `!g_bargraph_stop`, exiting the loop on `ioctl` failure), and exits the daemon loop; while `consecutive_errors < 5`, sleeps `usleep(CONFIG_BARGRAPH_INTERVAL_US)` before retrying.
        *   If `ret == 0`: resets `consecutive_errors = 0`, updates `current_level = quantize_level(raw, current_level)`, and if `current_level` changed writes `mask = (current_level == 0) ? 0U : ((1U << current_level) - 1U)` via `ioctl(led_fd, ULEDIOC_SETALL, (unsigned long)mask)`, then calls `usleep(CONFIG_BARGRAPH_INTERVAL_US)`.
        *   On any loop exit: clears LEDs (`ULEDIOC_SETALL(0UL)`), closes `led_fd`, resets `g_bargraph_pid = -1` and `g_bargraph_running = 0`, and returns `EXIT_SUCCESS` (or `EXIT_FAILURE` on `ULEDIOC_SETALL` failure).
    *   **Enable App in `defconfig`**: Add `CONFIG_BARGRAPH=y` to `board/configs/bargraph/defconfig`.

*   **Phase 3 Checkpoint**: Running `make -C xiao-rp2350-bargraph configure && make -C xiao-rp2350-bargraph` links `../apps/external`, sets `CONFIG_BARGRAPH=y` in `nuttx/.config`, and builds `../nuttx/nuttx.uf2` with zero errors.

### Verification Plan

*   **Host Prerequisite Check**:
    *   Verify `arm-none-eabi-gcc`, `genromfs`, `xxd`, and `picotool` (or `$PICO_SDK_PATH`) are installed on the Linux host (all four are required to compile and package `nuttx.uf2`; at flash time, either `picotool load` or `BOOTSEL` UF2 drag-and-drop can be used).
*   **Host Unit Check (Quantizer & Decoder)**:
    *   Run a host C test (`gcc`) exercising `quantize_level()` across all 7 levels (`0..6`), ascending/descending `±24` hysteresis boundaries (`585 ± 24`, `3510 ± 24`, `0`, `4095`), multi-step jumps (`0 -> 4095` and `4095 -> 0` in one call), `sample.am_data < 0` rejection, and `am_data >> 19` scaling.
*   **Compilation / Build**: Run `make -C /home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph` and verify:
    1. `../apps/external` resolves to `../xiao-rp2350-bargraph/external`, and `git status` inside both `nuttx/` and `apps/` reports zero modified or untracked files.
    2. `../nuttx/nuttx.uf2` builds with zero compiler errors/warnings in `xiao-rp2350-bargraph/`, and linker output stays within `memmap_default.ld` limits.
    3. Generated ROMFS image includes `/etc/init.d/rcS`.
*   **Hardware / Integration Verification (On XIAO RP2350 Breadboard)**:
    1. Flash `nuttx.uf2` via `BOOTSEL` (`picotool load -f ../nuttx/nuttx.uf2 -x` or UF2 drag-and-drop).
    2. **Autonomous Boot & Quantizer Tracking (CUJ 1)**: Power the board from a 5 V supply with no host serial terminal open. Rotate the potentiometer on `D0` from `0 V` to `3.3 V` and back, verifying 0 to 6 LEDs (`D3`, `D4`, `D5`, `D8`, `D9`, `D10`) step monotonically across all 7 states (`0x00`..`0x3f`) and back down with no flicker near stationary threshold boundaries.
    3. **USB-C NSH, IRQ Reduction, & Lifecycle (CUJ 2)**: Plug into the Linux host, open `/dev/ttyACM0`, press Enter 3 times to reach `nsh>`, and verify:
       * `dmesg` shows no boot or driver errors and logs `bargraph_daemon` startup.
       * `cat /proc/interrupts` sampled twice 1 second apart shows an ADC FIFO IRQ count delta on the order of tens to low hundreds of interrupts/second (matching 20 Hz `open`/`read`/`close` cycles) rather than ~500,000 continuous interrupts/second.
       * `ps` shows `bargraph_daemon` running with healthy stack headroom; running `bargraph` a second time prints `"bargraph: daemon already running"` without spawning a duplicate.
       * `kill -15 <pid>` stops `bargraph_daemon` and turns off all 6 LEDs; running `bargraph` restarts it; unplugging/replugging USB-C re-enumerates `/dev/ttyACM0` without FD lockup.
    4. **Fault Indication & Cleanup (CUJ 3)**: From `nsh>`, run `kill -15 <pid>` and then `bargraph /dev/adc_missing`, verify the 6 LEDs alternate between `0x15` and `0x2A` at 250 ms, verify `dmesg` records the `syslog(LOG_ERR, ...)` entry, and verify `kill -15 <pid>` stops the fault blink and clears all LEDs before restarting `bargraph`.

--------------------------------------------------------------------------------

*Drafted with EGM*
