# Out-of-Tree XIAO RP2350 Potentiometer LED Bar Graph (xiao-rp2350-bargraph)

*   **Author:** Brett Morgan
*   **Status:** Draft
*   **Date:** 2026-11-13

--------------------------------------------------------------------------------

## Overview

`xiao-rp2350-bargraph` is a self-contained Apache NuttX firmware project targeting a Seeed Studio XIAO RP2350 mounted on a solderless breadboard with one analog potentiometer and six discrete active-high LEDs. The firmware reads the potentiometer voltage at 20 Hz (every 50 ms) and lights 0 to 6 LEDs proportionally as a bar graph, starting automatically at power-on without requiring an attached host computer while still exposing an interactive NuttShell (`nsh>`) console over USB-C (CDC/ACM) when plugged into a host.

All board support files, kernel configuration (`defconfig`), startup scripts (`rcS`), user-space daemon source files, and build automation live in `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`, outside the `nuttx/` kernel repository and `apps/` application repository.

## The Problem

Building a hardware-backed application on the XIAO RP2350 in Apache NuttX without modifying upstream submodules exposes four concrete conflicts between the board's pinout, the RP2350 ADC driver (`rp23xx_adc.c`), the NuttShell USB console startup sequence, and the out-of-tree build system:

1.  **ADC Pin Clobbering and Analog Pad Input-Buffer Leakage (`GPIO 26–29`)**: During driver registration (`adc_register()`), `my_reset()` in `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` loops over all four entries in `g_gpio_map` (`GPIO 26, 27, 28, 29`) regardless of which `CONFIG_RP23XX_ADC_CHANNEL*` options are enabled. It reconfigures all four pins as floating inputs (`RP23XX_GPIO_FUNC_NULL`), wiping out any LEDs wired to `D1` (`GPIO 27`) or `D2` (`GPIO 28`), and re-enables the digital input buffer (`RP23XX_PADS_BANK0_GPIO_IE`) that `rp23xx_common_earlyinitialize()` cleared for analog operation. Leaving `IE=1` on `D0` (`GPIO 26`) introduces pad leakage when the potentiometer wiper sits near mid-rail voltages.
2.  **Unthrottled 500 kS/s ADC FIFO Interrupts and Stale Ring-Buffer Samples**: `rp23xx_adc.c` rejects `ANIOC_TRIGGER` (`-ENOTTY`), sets no ADC clock divider, and re-arms a new conversion inside its FIFO interrupt handler as long as `/dev/adc0` remains open. Meanwhile, the upper-half ring buffer (`nuttx/drivers/analog/adc.c`, `CONFIG_ADC_FIFOSIZE=8`) drops new samples once full rather than overwriting old ones. A task that keeps `/dev/adc0` open and sleeps for 50 ms leaves a ~500 kHz interrupt firing continuously and reads stale samples from the previous wake cycle.
3.  **Headless Boot vs. Blocking USB-C Console Initialization**: Upstream `xiao-rp2350/configs/usbnsh/defconfig` omits `CONFIG_BOARD_LATE_INITIALIZE=y`, which prevents `rp23xx_bringup()` from registering `/dev/adc0` and `/dev/userleds` at boot. Furthermore, in `apps/system/nsh/nsh_main.c`, `nsh_initialize()` executes `/etc/init.d/rcS` before `nsh_consolemain()` connects USB CDC/ACM and blocks waiting for three Enter keystrokes on `/dev/ttyACM0`. Any foreground task in `rcS` blocks USB console enumeration forever, and standard output written during `rcS` is discarded to `/dev/null`.
4.  **Single-LED Upstream Board Support and Out-of-Tree Build Wiring**: Upstream `xiao-rp2350/src/rp23xx_userleds.c` hardcodes a single active-low onboard LED (`GPIO 25`) in `board_userled_all()`, and `CONFIG_ARCH_LEDS` defaults to `y` when `ARCH_HAVE_LEDS` is selected, compiling `rp23xx_autoleds.c` instead of `rp23xx_userleds.c`. In addition, `xiao-rp2350/src/CMakeLists.txt` leaves `CONFIG_ETC_ROMFS` unimplemented (`# TODO`).

The proposed solution isolates a custom board definition (`CONFIG_ARCH_BOARD_CUSTOM=y` paired with `CONFIG_ARCH_BOARD_COMMON=y`) and an external user-space daemon (`apps/external` symlink) inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`, built with GNU Make. It maps the potentiometer to `D0` (`GPIO 26` / `ADC0`) with `IE` explicitly cleared after ADC registration, assigns the 6 bar-graph LEDs to purely digital pins (`D3–D5`, `D8–D10`) under `/dev/userleds`, opens/reads/closes `/dev/adc0` on each 50 ms tick to stop the ADC interrupt between samples, and starts the bar-graph daemon in the background from a ROMFS `/etc/init.d/rcS` script with RAMLOG syslog diagnostics and an LED fault-blink fallback.

## Goals

*   Keep all project source files, board definitions, startup scripts, and build scripts inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph` with zero modifications to tracked files in `nuttx/` or `apps/`.
*   Configure `D0` (`GPIO 26` / `ADC0`) as `/dev/adc0` with its digital pad input buffer (`RP23XX_PADS_BANK0_GPIO_IE`) disabled after `rp23xx_common_bringup()` completes.
*   Expose 6 active-high breadboard LEDs on `D3` (`GPIO 5`), `D4` (`GPIO 6`), `D5` (`GPIO 7`), `D8` (`GPIO 2`), `D9` (`GPIO 4`), and `D10` (`GPIO 3`) through `/dev/userleds` (`BOARD_NLEDS=6`, `# CONFIG_ARCH_LEDS is not set`, `CONFIG_USERLED=y`, `CONFIG_USERLED_LOWER=y`).
*   Sample `/dev/adc0` at 20 Hz (~50 ms interval) using an `open` -> `read(sizeof(struct adc_msg_s))` -> `close` cycle per tick so `my_shutdown()` disables the RP2350 ADC FIFO interrupt while the daemon sleeps.
*   Filter out negative FIFO-error samples (`am_data < 0`), shift the upper-half 32-bit reading right by 19 bits (`0..4095`), apply hysteresis/deadband across the 0–6 LED thresholds, and call `ioctl(fd, ULEDIOC_SETALL, ledset)` only when the displayed level changes.
*   Auto-launch the singleton bar-graph daemon (`task_create` + `SIGTERM` handler) at boot from `/etc/init.d/rcS` (`CONFIG_ETC_ROMFS=y`, `CONFIG_BOARD_LATE_INITIALIZE=y`) without blocking USB CDC/ACM console enumeration (`CONFIG_CDCACM_CONSOLE=y`, `CONFIG_NSH_USBCONSOLE=y`).
*   Record boot and runtime diagnostics to `syslog()` backed by `CONFIG_RAMLOG_SYSLOG=y` (inspectable via `dmesg` over NSH) and blink an alternating `0x15` / `0x2A` pattern on `/dev/userleds` if `/dev/adc0` or `/dev/userleds` fails.

## Non-Goals

*   Supporting boards other than the Seeed Studio XIAO RP2350 in this project.
*   Editing tracked files inside `nuttx/` or `apps/` (only the gitignored `apps/external` symlink and standard build outputs are touched during compilation).
*   Supporting CMake builds (`xiao-rp2350/src/CMakeLists.txt` does not implement `CONFIG_ETC_ROMFS`).
*   Adding PWM brightness dimming, hardware DMA pacing for the ADC, persistent flash calibration, or networking.

## Open Questions

*   None. All hardware pin assignments, driver workarounds, and boot-lifecycle questions from the concept phase are resolved.

## Critical User Journeys (CUJs)

### CUJ 1: Standalone Power-Up and Potentiometer Tracking

*   **Given**: The XIAO RP2350 is flashed with `nuttx.uf2`, wired on a breadboard with a potentiometer on `D0` (`GPIO 26`) and 6 active-high LEDs on `D3`, `D4`, `D5`, `D8`, `D9`, and `D10`, and powered from a 5 V USB power supply with no serial terminal open.
*   **When**: The board powers on and the user rotates the potentiometer wiper from GND (`0 V`) to `3.3 V` and back.
*   **Then**: `board_late_initialize()` registers `/dev/adc0` and `/dev/userleds`, `/etc/init.d/rcS` spawns the `bargraph` daemon in the background, and the 6 LEDs smoothly step from 0 lit (`0x00`) to 6 lit (`0x3f`) at 20 Hz with no boundary flicker when the wiper is stationary and zero background ADC interrupts between 50 ms sample ticks.

### CUJ 2: Interactive Inspection and Control via USB-C NuttShell

*   **Given**: The board is already running the bar-graph daemon from power-up.
*   **When**: The user connects the XIAO RP2350 USB-C port to a Linux host, opens `/dev/ttyACM0`, presses Enter three times, and runs `dmesg`, `ps`, `bargraph`, `kill -15 <pid>`, and `bargraph`.
*   **Then**: `nsh>` appears without interrupting the running bar graph; `dmesg` displays the RAMLOG boot and daemon startup messages; running `bargraph` while it is already active prints an "already running" notice and exits without spawning a duplicate task; sending `SIGTERM` cleanly stops the daemon and turns off the LEDs; running `bargraph` again restarts the daemon.

### CUJ 3: Headless Hardware or Driver Fault Indication

*   **Given**: The board boots without a USB host attached, and `/dev/adc0` fails to open or returns repeated read errors.
*   **When**: The `bargraph` daemon detects the failure on `/dev/adc0` (while `/dev/userleds` is open).
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
    └── external/                      (mounted into apps build via apps/external)
        ├── Makefile
        ├── Make.defs
        ├── Kconfig
        └── bargraph/
            ├── Makefile
            ├── Make.defs
            ├── Kconfig
            └── bargraph_main.c
```

Two NuttX build-system rules govern this layout:

1.  **Shared RP23XX Common Directory (`BOARD_COMMON_DIR`)**: In `nuttx/tools/Config.mk`, if `$(BOARD_DIR)/../common` exists, the build system binds `BOARD_COMMON_DIR` to that sibling directory instead of `nuttx/boards/arm/rp23xx/common`. Therefore, `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph/common/` MUST NOT exist. Keeping `common/` absent lets `CONFIG_ARCH_BOARD_COMMON=y` resolve directly to `nuttx/boards/arm/rp23xx/common` for `rp23xx_common_earlyinitialize()`, `rp23xx_common_initialize()`, `rp23xx_common_bringup()`, `rp23xx_reset.c`, and `rp23xx_boardctl.c`.
2.  **Linker Script Reuse**: `board/scripts/Make.defs` sets `ARCHSCRIPT` to `$(TOPDIR)$(DELIM)boards$(DELIM)arm$(DELIM)rp23xx$(DELIM)xiao-rp2350$(DELIM)scripts$(DELIM)$(LDSCRIPT)` (`memmap_default.ld`, `memmap_copy_to_ram.ld`, or `memmap_no_flash.ld`) rather than copying over 1,000 lines of RP2350 flash/RAM linker scripts out of tree.

### Hardware Pin Mapping

| XIAO Pin | RP2350 GPIO | Function / Polarity | NuttX Device | Notes |
| :--- | :--- | :--- | :--- | :--- |
| `D0` | `GPIO 26` | Analog Input (`ADC0`) | `/dev/adc0` | Potentiometer wiper (`0–3.3 V`); `IE` cleared after `rp23xx_common_bringup()` |
| `D1` | `GPIO 27` | Unassigned | — | Avoided; reset to `FUNC_NULL` input by `rp23xx_adc.c` `my_reset()` |
| `D2` | `GPIO 28` | Unassigned | — | Avoided; reset to `FUNC_NULL` input by `rp23xx_adc.c` `my_reset()` |
| `D3` | `GPIO 5` | Output, Active-High (`LED 1`) | `/dev/userleds` bit `0` (`0x01`) | Series resistor to GND |
| `D4` | `GPIO 6` | Output, Active-High (`LED 2`) | `/dev/userleds` bit `1` (`0x02`) | Series resistor to GND |
| `D5` | `GPIO 7` | Output, Active-High (`LED 3`) | `/dev/userleds` bit `2` (`0x04`) | Series resistor to GND |
| `D6` | `GPIO 0` | Unassigned (`UART0 TX`) | — | Left free (`# CONFIG_RP23XX_UART0 is not set`) |
| `D7` | `GPIO 1` | Unassigned (`UART0 RX`) | — | Left free (`# CONFIG_RP23XX_UART0 is not set`) |
| `D8` | `GPIO 2` | Output, Active-High (`LED 4`) | `/dev/userleds` bit `3` (`0x08`) | Series resistor to GND |
| `D9` | `GPIO 4` | Output, Active-High (`LED 5`) | `/dev/userleds` bit `4` (`0x10`) | Series resistor to GND |
| `D10` | `GPIO 3` | Output, Active-High (`LED 6`) | `/dev/userleds` bit `5` (`0x20`) | Series resistor to GND |
| Onboard | `GPIO 25` | Boot-initialized output | — | Driven low/on by `rp23xx_boardearlyinitialize()`; not part of `/dev/userleds` |

### System Data Flow and Boot Sequence

```text
+-----------------------------------------------------------------------------+
| 1. Kernel Boot & Board Bringup                                              |
|    rp23xx_boardearlyinitialize() -> rp23xx_common_earlyinitialize()         |
|    board_late_initialize()       -> rp23xx_bringup()                        |
|      1) rp23xx_common_bringup()  -> rp23xx_adc_setup("/dev/adc0", ADC0)     |
|                                     (adc_register -> my_reset sets IE=1)    |
|      2) clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, GPIO 26)  [clears IE=0]    |
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
| 3. bargraph_daemon 20 Hz Control Loop                                       |
|    open("/dev/adc0", O_RDONLY) -> read(5-byte struct adc_msg_s) -> close()  |
|      |                                                                      |
|      +--> [am_data < 0 (FIFO error)] --> discard sample, sleep 50 ms        |
|      +--> [valid] --> raw = am_data >> 19 (0..4095)                         |
|                   --> quantize(raw, current_level, HYSTERESIS=24) -> 0..6   |
|                   --> if level changed: ioctl(led_fd, ULEDIOC_SETALL, mask) |
|    usleep(50000)                                                            |
+-----------------------------------------------------------------------------+
```

### Detailed Subsystem Mechanisms

#### Board Bringup and Analog Pad Input-Buffer Fix

`rp23xx_common_earlyinitialize()` disables `RP23XX_PADS_BANK0_GPIO_IE` on `GPIO 26–29`, but `rp23xx_common_bringup()` immediately calls `rp23xx_adc_setup("/dev/adc0", true, false, false, false, false)`, which calls `adc_register()`. During registration, `my_reset()` in `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` runs `rp23xx_gpio_set_function(26, RP23XX_GPIO_FUNC_NULL)`, re-enabling `IE=1` on `GPIO 26`. Because `my_reset()` executes only at driver registration time (not on `open()` or `close()`), calling `clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, RP23XX_PADS_BANK0_GPIO(26))` in `rp23xx_bringup.c` immediately after `rp23xx_common_bringup()` keeps `GPIO 26` isolated from digital input buffer leakage for the entire uptime. `CONFIG_DEV_GPIO` is disabled and `GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1` are omitted from `board.h` so no GPIO character driver touches `GPIO 26–28`.

#### 6-LED `/dev/userleds` Lower-Half

With `# CONFIG_ARCH_LEDS is not set`, `CONFIG_BOARD_CUSTOM_LEDS=y` (`ARCH_HAVE_LEDS`), `CONFIG_USERLED=y`, and `CONFIG_USERLED_LOWER=y`, the board builds `rp23xx_userleds.c` instead of `rp23xx_autoleds.c`. `board_userled_initialize()` initializes all six pins in `g_ledcfg[6]` (`GPIO 5, 6, 7, 2, 4, 3`) as outputs driven low (`false`) and returns `BOARD_NLEDS` (`6`). `board_userled(int led, bool ledon)` updates a single index `0..5`, and `board_userled_all(uint32_t ledset)` loops across `0 <= i < BOARD_NLEDS`, writing `(ledset & (1U << i)) != 0` to `g_ledcfg[i]`.

#### ADC Sampling, Normalization, and Hysteresis

*   **Per-Tick `open` / `read` / `close` Cycle**: Opening `/dev/adc0` triggers `my_rxint(dev, true)` -> `add_device()`, which enables the RP2350 ADC FIFO, attaches `RP23XX_ADC_IRQ_FIFO`, and starts conversions. Reading exactly `sizeof(struct adc_msg_s)` (5 bytes: 1-byte `am_channel` + 4-byte `int32_t am_data`) matches the `buflen % 5 == 0` record branch in `nuttx/drivers/analog/adc.c` and collects one fresh conversion. Closing `/dev/adc0` triggers `my_shutdown()` -> `remove_device()`, decrementing `g_active_count` to `0` so the FIFO ISR immediately disables `RP23XX_ADC_CS`, `RP23XX_ADC_INTE`, and detaches `RP23XX_ADC_IRQ_FIFO` before the task calls `usleep(50000)`.
*   **Error Filtering and Scaling**: `rp23xx_adc.c` shifts the 12-bit hardware result left by 19 bits (`value <<= 19`, range `0..0x7ff80000`) and negates `value` when `RP23XX_ADC_FIFO_ERR` is set. Any reading with `sample.am_data < 0` is discarded. Valid readings normalize to `uint16_t raw = (uint16_t)(sample.am_data >> 19)` (`0..4095`).
*   **7-Level Hysteresis Quantizer**: Full scale (`0..4095`) is divided into 7 buckets (`level` `0..6`) with nominal thresholds at multiples of `585` (`585, 1170, 1755, 2340, 2925, 3510`) and a hysteresis band `HYSTERESIS = 24` counts (~0.6% of full scale). Stepping upward from `level` `k` to `k + 1` requires `raw >= threshold[k] + HYSTERESIS`, and stepping downward from `k` to `k - 1` requires `raw <= threshold[k - 1] - HYSTERESIS`. The resulting LED bitmask is `mask = (level == 0) ? 0 : ((1U << level) - 1U)` (`0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f`), written via `ioctl(led_fd, ULEDIOC_SETALL, mask)` only when `level` changes.

#### Daemon Lifecycle, ROMFS Startup, and Headless Diagnostics

*   `bargraph_main()` checks a static `g_bargraph_started` flag so repeated CLI invocations print an "already running" message and exit `0` without launching duplicates. Otherwise it spawns `bargraph_daemon` via `task_create()` and returns immediately so `/etc/init.d/rcS` finishes before `nsh_consolemain()` runs USB enumeration.
*   `bargraph_daemon` installs a `SIGTERM` handler (`sigaction`) that clears `g_bargraph_started` so `kill -15 <pid>` breaks the loop, turns off all LEDs (`ULEDIOC_SETALL(0)`), closes `/dev/userleds`, and exits cleanly.
*   Because `Board.mk` preprocesses `RCSRCS` with `cpp`, `etc/init.d/rcS` uses `#include <nuttx/config.h>`, C `/* ... */` comments, and `#ifdef CONFIG_BARGRAPH` around the `bargraph` command.
*   All runtime errors log via `syslog(LOG_ERR, ...)` to RAMLOG (`CONFIG_RAMLOG_SYSLOG=y`, `CONFIG_RAMLOG_BUFSIZE=4096`), readable via `dmesg` once USB NSH attaches. If `/dev/adc0` fails to open or returns consecutive read errors (`>= 5`), the daemon logs the failure and alternates `ULEDIOC_SETALL` between `0x15` and `0x2A` every 250 ms until `SIGTERM` arrives.

## Alternatives Considered

### Alternative A: Modifying `nuttx/boards/arm/rp23xx/xiao-rp2350` In-Place

*   **Pros**: Requires fewer boilerplate files (`Kconfig`, `scripts/Make.defs`, `rp23xx_boardinitialize.c`) because the upstream board directory is edited directly.
*   **Cons**: Dirties the `nuttx` git submodule, mixes project-specific breadboard pin assignments into upstream board files, and prevents keeping the entire exercise isolated under `xiao-rp2350-bargraph/`.
*   **Reason for Rejection**: Rejected by author: Modifying `xiao-rp2350` directly pollutes the `nuttx` submodule instead of keeping exercise code isolated in a new root directory (`CONFIG_ARCH_BOARD_CUSTOM=y`).

### Alternative B: Driving 8 Breadboard LEDs (Using `D1`/`D2` `GPIO 27–28` or `D6`/`D7` `GPIO 0–1`)

*   **Pros**: Provides two extra bar-graph steps (8 LEDs instead of 6).
*   **Cons**: `rp23xx_adc.c` `my_reset()` resets `GPIO 26–29` unconditionally to floating inputs during `adc_register()`, which clobbers `D1` (`GPIO 27`) and `D2` (`GPIO 28`) unless initialization ordering is tightly constrained and `my_reset()` never fires again. Claiming `D6`/`D7` (`GPIO 0`/`1`) occupies the hardware `UART0` pins.
*   **Reason for Rejection**: Rejected by author: 6 LEDs on `D3` (`GPIO 5`), `D4` (`GPIO 6`), `D5` (`GPIO 7`), `D8` (`GPIO 2`), `D9` (`GPIO 4`), and `D10` (`GPIO 3`) are sufficient and completely avoid `rp23xx_adc.c` `my_reset()` on `GPIO 27–28` and `UART0` on `GPIO 0–1`.

### Alternative C: Keeping `/dev/adc0` Continuously Open with `ANIOC_RESET_FIFO`

*   **Pros**: Eliminates per-tick `open()` and `close()` syscalls and interrupt attach/detach overhead every 50 ms.
*   **Cons**: `rp23xx_adc.c` starts free-running conversions at ~500 kS/s and leaves `RP23XX_ADC_IRQ_FIFO` firing continuously for as long as `/dev/adc0` stays open, burning CPU cycles during every `usleep(50000)` and overflowing the 8-entry upper-half FIFO (`CONFIG_ADC_FIFOSIZE=8`) between reads. (While `ANIOC_RESET_FIFO` is handled in the upper-half `nuttx/drivers/analog/adc.c`, `rp23xx_adc.c` `my_ioctl()` returns `-ENOTTY` for lower-half ioctls and provides no clock divider or trigger throttling.)
*   **Reason for Rejection**: Rejected by author: Keeping `/dev/adc0` open leaves the ~500 kHz ADC FIFO interrupt running continuously in the background during sleep; `open`/`read`/`close` every ~50 ms stops the ADC interrupt between ticks.

### Alternative D: Individual `/dev/gpout*` Character Devices (`CONFIG_DEV_GPIO=y`)

*   **Pros**: Avoids writing a custom `rp23xx_userleds.c` array if pins were exposed individually via `CONFIG_DEV_GPIO`.
*   **Cons**: Opening six `/dev/gpout*` descriptors pushes against `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=6` (alongside stdin/stdout/stderr and `/dev/adc0`), requires six `ioctl(GPIOC_WRITE)` syscalls per update, and leaves `GPIO 26–28` exposed if `board.h` retains upstream `GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1` definitions.
*   **Reason for Rejection**: Rejected by author: Six `/dev/gpout*` descriptors push against `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=6` and require 6 syscalls per update; `/dev/userleds` updates all 6 LEDs in one call (`ULEDIOC_SETALL`) over one descriptor.

### Alternative E: Hardware `UART0` Console (`D6`/`D7`)

*   **Pros**: Provides console output immediately at boot without waiting for USB enumeration or 3x Enter.
*   **Cons**: Requires wiring an external USB-to-UART adapter to `D6`/`D7` (`GPIO 0`/`1`) on the breadboard instead of using the XIAO RP2350's built-in USB-C port.
*   **Reason for Rejection**: Rejected by author: Using USB-C CDC/ACM (`usbnsh`) avoids an external USB-UART adapter and leaves `D6`/`D7` untouched.

### Alternative F: Setting `CONFIG_INIT_ENTRYPOINT` Directly to the Bar-Graph App

*   **Pros**: Eliminates `CONFIG_ETC_ROMFS`, `genromfs`, `xxd`, and `NSH_USBCONSOLE` plumbing.
*   **Cons**: Removes `nsh>`, `dmesg`, `ps`, `free`, and interactive daemon start/stop (`kill -15`) over USB-C, reducing visibility into kernel and driver state during this NuttX learning exercise.
*   **Reason for Rejection**: Rejected by author: Keeping NSH + ROMFS `/etc/init.d/rcS` preserves `dmesg`, `ps`, `free`, and interactive learning over USB-C.

--------------------------------------------------------------------------------

## Non-Functional Requirements

*   **CPU and Interrupt Load**: Opening `/dev/adc0` for a single 5-byte read and closing it every 50 ms keeps the RP2350 ADC FIFO interrupt active only long enough to capture one conversion (~2 µs per 50 ms tick) rather than ~500,000 interrupts/second continuously.
*   **SRAM Footprint**: With 512 KB main SRAM (`memmap_default.ld`), `CONFIG_RAMLOG_BUFSIZE=4096` (4 KB) and a 2048-byte daemon stack (`CONFIG_BARGRAPH_STACKSIZE=2048`) fit within the existing `usbnsh` memory budget.
*   **Rollback and Recovery**: The daemon installs a `SIGTERM` handler so `kill -15 <pid>` turns off all LEDs and exits cleanly. If a build introduces a boot fault, holding `BOOT` while resetting the XIAO RP2350 enters the RP2350 ROM UF2 bootloader for flashing a working `nuttx.uf2`.

--------------------------------------------------------------------------------

### Additional Resources

*   [xiao-rp2350 board.h](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h)
*   [xiao-rp2350 rp23xx_bringup.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_bringup.c)
*   [xiao-rp2350 rp23xx_userleds.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_userleds.c)
*   [xiao-rp2350 usbnsh defconfig](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig)
*   [rp23xx_adc.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/arch/arm/src/rp23xx/rp23xx_adc.c)
*   [rp23xx_common_initialize.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/common/src/rp23xx_common_initialize.c)
*   [rp23xx_common_bringup.c](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c)
*   [Board.mk](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/boards/Board.mk)
*   [Config.mk](file:///home/brett/Documents/GitHub/nuttxspace/nuttx/tools/Config.mk)
*   [apps/examples/leds/leds_main.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/examples/leds/leds_main.c)
*   [apps/examples/adc/adc_main.c](file:///home/brett/Documents/GitHub/nuttxspace/apps/examples/adc/adc_main.c)

### Future Improvements

*   Add an optional ADC sample-rate divider or timer-triggered DMA mode to `rp23xx_adc.c` upstream so continuous-open reads can run at 20 Hz without per-tick `open`/`close`.
*   Add optional PWM duty-cycle smoothing across intermediate potentiometer positions on spare RP2350 PWM slices.

--------------------------------------------------------------------------------

## Detailed Implementation Plan

All files below are created inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`. No tracked files inside `nuttx/` or `apps/` are edited; only the gitignored `apps/external` symlink and regenerated `apps/Kconfig` / build outputs are created during configuration and compilation.

### Target Files to Create/Modify

#### 1. `xiao-rp2350-bargraph/Makefile` (Create)

*   **Type / Scope**: Top-level GNU Make build and configuration wrapper.
*   **Rationale**: Keeps one-command configuration and building self-contained outside `nuttx/` while ensuring `apps/external` points to `../xiao-rp2350-bargraph/external` and `apps/Kconfig` is refreshed when `external/` Kconfig files change.
*   **Proposed Changes**:
    *   `setup` target: Creates or verifies the symlink `../apps/external -> ../xiao-rp2350-bargraph/external` (aborting if `../apps/external` exists as a non-symlink directory to avoid clobbering unrelated local files) and deletes `../apps/Kconfig` if older than `external/Kconfig` or `external/bargraph/Kconfig` so `preconfig` regenerates it.
    *   `configure` target: Depends on `setup`; runs `cd ../nuttx && ./tools/configure.sh -e -l ../xiao-rp2350-bargraph/board/configs/bargraph`.
    *   `all` (default) target: Depends on `setup`; if `../nuttx/.config` is missing, runs `configure`, then executes `$(MAKE) -C ../nuttx` to produce `../nuttx/nuttx.uf2`.
    *   `clean` and `distclean` targets: Delegate to `$(MAKE) -C ../nuttx clean` / `distclean`, and on `distclean` remove the `../apps/external` symlink and `../apps/Kconfig`.

#### 2. `xiao-rp2350-bargraph/board/Kconfig` (Create)

*   **Type / Scope**: Custom board Kconfig stub required by `nuttx/tools/Config.mk`.
*   **Rationale**: `nuttx/tools/Config.mk` binds `BOARD_KCONFIG = $(BOARD_DIR)/Kconfig` when `ARCH_BOARD_CUSTOM=y`.
*   **Proposed Changes**:
    *   Provide a minimal `if ARCH_BOARD_CUSTOM` block (empty or board-specific helper symbols if needed), relying on `CONFIG_BOARD_CUSTOM_LEDS=y` in `nuttx/boards/Kconfig` to `select ARCH_HAVE_LEDS`.

#### 3. `xiao-rp2350-bargraph/board/include/board.h` (Create)

*   **Type / Scope**: Custom board header (`<arch/board/board.h>`).
*   **Rationale**: Defines RP2350 clock frequencies, removes upstream `GPIO 26–28` generic GPIO macros (`GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1`) so `CONFIG_DEV_GPIO` cannot claim the potentiometer or avoided ADC pins, and expands the `userleds` interface from 1 onboard LED to 6 breadboard LEDs.
*   **Proposed Changes**:
    *   Retain RP2350 clock macros (`BOARD_XOSC_FREQ` = `12 * MHZ`, `BOARD_PLL_SYS_FREQ` = `150 * MHZ`, `BOARD_PLL_USB_FREQ` = `48 * MHZ`, `BOARD_ADC_FREQ` = `48 * MHZ`, `BOARD_TICK_CLOCK` = `1 * MHZ`, etc.) and `BOARD_GPIO_LED_PIN = 25` from `nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h`.
    *   Include `"rp23xx_i2cdev.h"`, `"rp23xx_spidev.h"`, `"rp23xx_i2sdev.h"`, `"rp23xx_spisd.h"` required by `rp23xx_common_bringup.c`.
    *   Omit `BOARD_NGPIOOUT`, `BOARD_NGPIOIN`, `BOARD_NGPIOINT`, `GPIO_OUT1`, `GPIO_IN1`, and `GPIO_IRQPIN1`.
    *   Define `BOARD_LED1` (`0`) through `BOARD_LED6` (`5`), `BOARD_NLEDS` (`6`), and bitmasks `BOARD_LED1_BIT` (`(1 << BOARD_LED1)`) through `BOARD_LED6_BIT` (`(1 << BOARD_LED6)`).
    *   Declare `void rp23xx_boardearlyinitialize(void);` and `void rp23xx_boardinitialize(void);`.

#### 4. `xiao-rp2350-bargraph/board/scripts/Make.defs` (Create)

*   **Type / Scope**: Custom board compiler flags and linker script Makefile fragment.
*   **Rationale**: `tools/configure.sh` symlinks `nuttx/Make.defs` to `$(BOARD_DIR)/scripts/Make.defs`. This file sets ARMv8-M flags and points `ARCHSCRIPT` to the upstream `xiao-rp2350` linker scripts so flash/RAM layout (`memmap_default.ld`) is reused without duplication.
*   **Proposed Changes**:
    *   Include `$(TOPDIR)/.config`, `$(TOPDIR)/tools/Config.mk`, `$(TOPDIR)/tools/rp23xx/Config.mk`, and `$(TOPDIR)/arch/arm/src/armv8-m/Toolchain.defs`.
    *   Select `LDSCRIPT` (`memmap_default.ld`, `memmap_copy_to_ram.ld`, or `memmap_no_flash.ld`) matching `CONFIG_BOOT_*`.
    *   Set `ARCHSCRIPT += $(TOPDIR)$(DELIM)boards$(DELIM)arm$(DELIM)rp23xx$(DELIM)xiao-rp2350$(DELIM)scripts$(DELIM)$(LDSCRIPT)`.
    *   Set `CFLAGS`, `CXXFLAGS`, `CPPFLAGS`, `AFLAGS`, and `NXFLATLDFLAGS*` identically to `nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/Make.defs`.

#### 5. `xiao-rp2350-bargraph/board/src/Make.defs` (Create)

*   **Type / Scope**: Board source and ROMFS script build list.
*   **Rationale**: Registers board initialization, bringup, 6-LED driver, and ROMFS `/etc/init.d` files with `nuttx/boards/Board.mk`.
*   **Proposed Changes**:
    *   Add `rp23xx_boardinitialize.c` and `rp23xx_bringup.c` to `CSRCS`.
    *   Add `rp23xx_userleds.c` when `CONFIG_ARCH_LEDS` is not `y`.
    *   When `CONFIG_ETC_ROMFS=y`, set `RCSRCS = etc/init.d/rc.sysinit etc/init.d/rcS`.
    *   Set `DEPPATH += --dep-path board`, `VPATH += :board`, and `CFLAGS += ${INCDIR_PREFIX}$(TOPDIR)$(DELIM)arch$(DELIM)$(CONFIG_ARCH)$(DELIM)src$(DELIM)board$(DELIM)board`.

#### 6. `xiao-rp2350-bargraph/board/src/rp23xx_pico.h` (Create)

*   **Type / Scope**: Internal board source header.
*   **Rationale**: Centralizes the 6 breadboard LED GPIO numbers and ADC pin constant shared by `rp23xx_userleds.c`, `rp23xx_bringup.c`, and `rp23xx_boardinitialize.c`.
*   **Proposed Changes**:
    *   Define `GPIO_ADC_POT` (`26`, `D0`), `GPIO_LED1` (`5`, `D3`), `GPIO_LED2` (`6`, `D4`), `GPIO_LED3` (`7`, `D5`), `GPIO_LED4` (`2`, `D8`), `GPIO_LED5` (`4`, `D9`), and `GPIO_LED6` (`3`, `D10`).
    *   Declare `int rp23xx_bringup(void);`.

#### 7. `xiao-rp2350-bargraph/board/src/rp23xx_boardinitialize.c` (Create)

*   **Type / Scope**: Early, standard, and late board initialization hooks.
*   **Rationale**: Invokes shared RP2350 common initialization and exposes `board_late_initialize()` so `rp23xx_bringup()` runs before NSH executes `/etc/init.d/rcS`.
*   **Proposed Changes**:
    *   `rp23xx_boardearlyinitialize()`: Calls `rp23xx_common_earlyinitialize()` and configures `BOARD_GPIO_LED_PIN` (`GPIO 25`) as output driven `true` (active-low off or power-on indicator matching upstream `xiao-rp2350`).
    *   `rp23xx_boardinitialize()`: Calls `rp23xx_common_initialize()` (and `rp23xx_psramconfig()` under `#ifdef CONFIG_RP23XX_PSRAM`).
    *   `board_late_initialize()`: Guarded by `#ifdef CONFIG_BOARD_LATE_INITIALIZE`, calls `rp23xx_bringup()`.

#### 8. `xiao-rp2350-bargraph/board/src/rp23xx_bringup.c` (Create)

*   **Type / Scope**: Peripheral driver bringup and analog pad fix.
*   **Rationale**: Registers `/dev/adc0` via `rp23xx_common_bringup()`, then immediately clears `RP23XX_PADS_BANK0_GPIO_IE` on `GPIO 26` (`D0`) because `adc_register()` -> `my_reset()` re-enabled `IE=1`, and finally registers `/dev/userleds`.
*   **Proposed Changes**:
    *   Include `"arm_internal.h"` and `"rp23xx_gpio.h"` for `clrbits_reg32`, `RP23XX_PADS_BANK0_GPIO_IE`, and `RP23XX_PADS_BANK0_GPIO`.
    *   In `int rp23xx_bringup(void)`:
        1. Call `rp23xx_common_bringup()`; log warning/error if `< 0` and return early.
        2. Under `#if defined(CONFIG_ADC) && defined(CONFIG_RP23XX_ADC) && defined(CONFIG_RP23XX_ADC_CHANNEL0)`, call `clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, RP23XX_PADS_BANK0_GPIO(GPIO_ADC_POT))` to keep the digital input buffer disabled on `D0`.
        3. Under `#ifdef CONFIG_USERLED`, call `userled_lower_initialize("/dev/userleds")` and log via `syslog(LOG_ERR, ...)` on failure.

#### 9. `xiao-rp2350-bargraph/board/src/rp23xx_userleds.c` (Create)

*   **Type / Scope**: Multi-LED `userleds` lower-half driver (`!CONFIG_ARCH_LEDS`).
*   **Rationale**: Replaces upstream `xiao-rp2350/src/rp23xx_userleds.c` which hardcodes a single active-low LED (`GPIO 25`) and ignores `g_ledcfg[]` in `board_userled_all()`.
*   **Proposed Changes**:
    *   Define `static const uint32_t g_ledcfg[BOARD_NLEDS] = { GPIO_LED1, GPIO_LED2, GPIO_LED3, GPIO_LED4, GPIO_LED5, GPIO_LED6 };`.
    *   `uint32_t board_userled_initialize(void)`: Loops `0..BOARD_NLEDS - 1`, calling `rp23xx_gpio_init(g_ledcfg[i])`, `rp23xx_gpio_setdir(g_ledcfg[i], true)`, and `rp23xx_gpio_put(g_ledcfg[i], false)` (active-high off); returns `BOARD_NLEDS`.
    *   `void board_userled(int led, bool ledon)`: If `(unsigned)led < BOARD_NLEDS`, calls `rp23xx_gpio_put(g_ledcfg[led], ledon)`.
    *   `void board_userled_all(uint32_t ledset)`: Loops `0..BOARD_NLEDS - 1`, calling `rp23xx_gpio_put(g_ledcfg[i], (ledset & (1U << i)) != 0)`.

#### 10. `xiao-rp2350-bargraph/board/src/etc/init.d/rc.sysinit` (Create)

*   **Type / Scope**: ROMFS system initialization script processed by `Board.mk`.
*   **Rationale**: `RCSRCS` in `board/src/Make.defs` requires both `etc/init.d/rc.sysinit` and `etc/init.d/rcS`.
*   **Proposed Changes**:
    *   Contain `#include <nuttx/config.h>` and standard Apache license header with C-style `/* ... */` comments.

#### 11. `xiao-rp2350-bargraph/board/src/etc/init.d/rcS` (Create)

*   **Type / Scope**: ROMFS NSH startup script processed by `Board.mk` via `cpp`.
*   **Rationale**: Launches `bargraph` automatically at power-on before `nsh_consolemain()` waits for USB attachment.
*   **Proposed Changes**:
    *   Include `<nuttx/config.h>` and use C-style `/* ... */` comments (no `#` shell comments, which fail `cpp` preprocessing).
    *   Under `#ifdef CONFIG_BARGRAPH`, invoke `bargraph` (which spawns `bargraph_daemon` via `task_create()` and returns immediately, avoiding blocking NSH even without `&`).

#### 12. `xiao-rp2350-bargraph/board/configs/bargraph/defconfig` (Create)

*   **Type / Scope**: Compressed NuttX `.config` template derived from `xiao-rp2350/configs/usbnsh/defconfig`.
*   **Rationale**: Enables custom board selection, common RP2350 helpers, late board init, ADC0, 6-LED `userleds`, ROMFS `/etc/init.d`, USB CDC/ACM NSH, RAMLOG `dmesg`, and the external `bargraph` app.
*   **Proposed Changes**:
    *   Custom board & common plumbing: `CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_ARCH_BOARD_CUSTOM_NAME="xiao-rp2350-bargraph"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y`, `CONFIG_ARCH_BOARD_COMMON=y`, `CONFIG_BOARD_CUSTOM_LEDS=y`, `# CONFIG_ARCH_LEDS is not set`, `CONFIG_BOARD_LATE_INITIALIZE=y`.
    *   ADC & LED drivers: `CONFIG_ANALOG=y`, `CONFIG_ADC=y`, `CONFIG_RP23XX_ADC=y`, `CONFIG_RP23XX_ADC_CHANNEL0=y`, `CONFIG_USERLED=y`, `CONFIG_USERLED_LOWER=y` (`CONFIG_DEV_GPIO` disabled).
    *   ROMFS & NSH scripts: `CONFIG_FS_ROMFS=y`, `CONFIG_ETC_ROMFS=y`, `CONFIG_NSH_BUILTIN_APPS=y`, `# CONFIG_NSH_DISABLEBG is not set`, `# CONFIG_NSH_DISABLESCRIPT is not set` (`CONFIG_RP23XX_ROMFS_ROMDISK_DEVNAME` unset).
    *   USB CDC/ACM console & RAMLOG diagnostics: `CONFIG_USBDEV=y`, `CONFIG_USBDEV_BUSPOWERED=y`, `CONFIG_CDCACM=y`, `CONFIG_CDCACM_CONSOLE=y`, `CONFIG_NSH_USBCONSOLE=y`, `# CONFIG_DEV_CONSOLE is not set`, `# CONFIG_RP23XX_UART0 is not set`, `CONFIG_RAMLOG=y`, `CONFIG_RAMLOG_SYSLOG=y`, `CONFIG_RAMLOG_BUFSIZE=4096`, `CONFIG_SYSTEM_NSH=y`.
    *   Application & cleanups: `CONFIG_BARGRAPH=y` (remove unnecessary `CONFIG_EXAMPLES_HELLO`, `CONFIG_TESTING_GETPRIME`, `CONFIG_TESTING_OSTEST` from `usbnsh` if unused to reduce flash/RAM footprint).

#### 13. `xiao-rp2350-bargraph/external/Makefile`, `Make.defs`, and `Kconfig` (Create)

*   **Type / Scope**: `apps/external` directory glue (`apps/` scans `apps/external` automatically when present).
*   **Rationale**: Integrates `external/bargraph` into the NuttX `apps/` build and `menuconfig`/`preconfig` tree without editing `apps/Makefile` or `apps/Kconfig`.
*   **Proposed Changes**:
    *   `external/Makefile`: Set `MENUDESC = "External"` and include `$(APPDIR)/Directory.mk`.
    *   `external/Make.defs`: `include $(wildcard $(APPDIR)/external/*/Make.defs)`.
    *   `external/Kconfig`: `source "$(APPDIR)/external/bargraph/Kconfig"` (or generated via `$(DIR)` / `Directory.mk` rules — verify exact `apps/examples/Kconfig` pattern during implementation).

#### 14. `xiao-rp2350-bargraph/external/bargraph/Kconfig` (Create)

*   **Type / Scope**: Application configuration symbols.
*   **Rationale**: Exposes configurable task parameters and device paths to `defconfig` and `rcS` (`CONFIG_BARGRAPH`).
*   **Proposed Changes**:
    *   Define `config BARGRAPH` (`tristate "Potentiometer LED bar graph daemon"`, `depends on ADC && USERLED`).
    *   Under `if BARGRAPH`, define `BARGRAPH_PROGNAME` (`"bargraph"`), `BARGRAPH_PRIORITY` (`100`), `BARGRAPH_STACKSIZE` (`2048`), `BARGRAPH_ADC_DEVPATH` (`"/dev/adc0"`), `BARGRAPH_LED_DEVPATH` (`"/dev/userleds"`), `BARGRAPH_INTERVAL_US` (`50000`), and `BARGRAPH_HYSTERESIS` (`24`).

#### 15. `xiao-rp2350-bargraph/external/bargraph/Makefile` and `Make.defs` (Create)

*   **Type / Scope**: Application build registration.
*   **Rationale**: Registers `bargraph` as a built-in NSH application compiled from `bargraph_main.c`.
*   **Proposed Changes**:
    *   `Make.defs`: `ifneq ($(CONFIG_BARGRAPH),)` -> `CONFIGURED_APPS += $(APPDIR)/external/bargraph`.
    *   `Makefile`: Include `$(APPDIR)/Make.defs`, set `PROGNAME = $(CONFIG_BARGRAPH_PROGNAME)`, `PRIORITY = $(CONFIG_BARGRAPH_PRIORITY)`, `STACKSIZE = $(CONFIG_BARGRAPH_STACKSIZE)`, `MODULE = $(CONFIG_BARGRAPH)`, `MAINSRC = bargraph_main.c`, and include `$(APPDIR)/Application.mk`.

#### 16. `xiao-rp2350-bargraph/external/bargraph/bargraph_main.c` (Create)

*   **Type / Scope**: User-space C singleton daemon (`main` + `bargraph_daemon`).
*   **Rationale**: Implements the 20 Hz potentiometer-to-bar-graph loop, per-tick ADC open/read/close power-and-ISR shutdown, hysteresis smoothing, `SIGTERM` shutdown, and headless LED fault blinking.
*   **Proposed Changes**:
    *   State: `static volatile bool g_bargraph_running = false;`
    *   `sigterm_handler(int signo, siginfo_t *siginfo, void *arg)`: Sets `g_bargraph_running = false` on `SIGTERM`.
    *   `read_adc_sample(const char *devpath, uint16_t *raw_out)` helper:
        *   Opens `devpath` with `O_RDONLY`, reads `sizeof(struct adc_msg_s)` (5 bytes), closes `fd` immediately.
        *   Returns `-EAGAIN` if `am_data < 0` (hardware FIFO conversion error); otherwise sets `*raw_out = (uint16_t)(sample.am_data >> 19)` and returns `0`; returns `-errno` on open/read failure.
    *   `quantize_level(uint16_t raw, int current_level)` helper:
        *   Uses 6 nominal thresholds `{ 585, 1170, 1755, 2340, 2925, 3510 }` with deadband `CONFIG_BARGRAPH_HYSTERESIS` (`24`).
        *   Steps `level` up while `level < 6 && raw >= thresholds[level] + hyst`, and down while `level > 0 && raw <= thresholds[level - 1] - hyst`.
    *   `fault_blink_loop(int led_fd)` helper:
        *   Alternates `ioctl(led_fd, ULEDIOC_SETALL, 0x15)` and `0x2A` every `250000` us while `g_bargraph_running` is `true`.
    *   `bargraph_daemon(int argc, FAR char *argv[])`:
        *   Installs `SIGTERM` `sigaction`, logs start via `syslog(LOG_INFO, ...)`.
        *   Opens `CONFIG_BARGRAPH_LED_DEVPATH` (`O_WRONLY`), queries `ULEDIOC_SUPPORTED` and masks to `0x3f`, clears LEDs via `ULEDIOC_SETALL(0)`. On LED device failure, logs via `syslog(LOG_ERR, ...)` and exits `EXIT_FAILURE`.
        *   Loops while `g_bargraph_running`: calls `read_adc_sample()`, tracks consecutive hard errors (entering `fault_blink_loop(led_fd)` after 5 consecutive open/read failures while ignoring transient `-EAGAIN`), updates `current_level` and calls `ULEDIOC_SETALL((1U << level) - 1U)` only when `level` changes, then sleeps `usleep(CONFIG_BARGRAPH_INTERVAL_US)`.
        *   On exit (`SIGTERM`), calls `ULEDIOC_SETALL(0)`, closes `led_fd`, resets `g_bargraph_running = false`, and returns `EXIT_SUCCESS`.
    *   `main(int argc, FAR char *argv[])`:
        *   If `g_bargraph_running` is already `true`, prints `"bargraph: daemon already running\n"` and returns `EXIT_SUCCESS`.
        *   Sets `g_bargraph_running = true` before `task_create("bargraph_daemon", CONFIG_BARGRAPH_PRIORITY, CONFIG_BARGRAPH_STACKSIZE, bargraph_daemon, NULL)` (resetting `g_bargraph_running = false` if `task_create()` returns `< 0`) and returns `EXIT_SUCCESS`.

### Verification Plan

*   **Host Prerequisite Check**: Verify `arm-none-eabi-gcc`, `genromfs`, `xxd`, and `picotool` are installed on the Linux host before compiling.
*   **Compilation / Build**: Run `make -C /home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph` and verify:
    1. `../apps/external` resolves to `../xiao-rp2350-bargraph/external` and `git status` inside both `nuttx/` and `apps/` reports zero modified tracked files.
    2. `../nuttx/nuttx.uf2` builds cleanly with zero compiler errors/warnings in `xiao-rp2350-bargraph/`.
    3. Generated `etctmp.c` inside `nuttx/boards/arm/rp23xx/` contains the ROMFS image including `/etc/init.d/rcS`.
*   **Hardware / Integration Verification (On XIAO RP2350 Breadboard)**:
    1. Flash `nuttx.uf2` via `BOOTSEL` (`picotool load -f ../nuttx/nuttx.uf2 -x` or UF2 drag-and-drop).
    2. **Autonomous Boot (CUJ 1)**: Power the board from a 5 V supply with no host serial terminal open. Rotate the potentiometer on `D0` from end to end and verify 0 to 6 LEDs (`D3`, `D4`, `D5`, `D8`, `D9`, `D10`) light sequentially with no flicker when stationary.
    3. **USB-C NSH & Lifecycle (CUJ 2)**: Plug into the Linux host, open `/dev/ttyACM0`, press Enter 3 times to reach `nsh>`, and verify:
       * `dmesg` shows clean boot and `bargraph_daemon` startup with no `/dev/adc0` or `/dev/userleds` errors.
       * `ps` shows `bargraph_daemon` running; running `bargraph` a second time prints `"bargraph: daemon already running"` without spawning a duplicate.
       * `kill -15 <pid>` stops `bargraph_daemon` and turns off all 6 LEDs; running `bargraph` restarts it.
    4. **Fault Indication (CUJ 3)**: Verify via a test build or fault-injection check (e.g., temporary invalid `CONFIG_BARGRAPH_ADC_DEVPATH`) that `/dev/userleds` alternates `0x15` / `0x2A` and `dmesg` records the `syslog(LOG_ERR, ...)` entry.

--------------------------------------------------------------------------------

*Drafted with EGM*
