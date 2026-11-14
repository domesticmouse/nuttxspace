# Out-of-Tree XIAO RP2350 Potentiometer LED Bar Graph (xiao-rp2350-bargraph)

*   **Author:** Brett Morgan
*   **Status:** Draft
*   **Date:** 2026-11-13

--------------------------------------------------------------------------------

## Overview

`xiao-rp2350-bargraph` is an out-of-tree Apache NuttX firmware project targeting a Seeed Studio XIAO RP2350 mounted on a solderless breadboard with one analog potentiometer (outer terminals connected to `3.3 V` and `GND`, wiper on `D0`) and six discrete active-high LEDs (`D3–D5`, `D8–D10`). The firmware samples the potentiometer at 20 Hz (every 50 ms) and drives 0 to 6 LEDs proportionally as a bar graph, launching at power-on without an attached host computer while exposing an interactive NuttShell (`nsh>`) console over USB-C (CDC/ACM) when plugged into a host.

All custom board, configuration, startup script, application, test, and build wrapper files will reside inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`, leaving tracked files in `nuttx/` and `apps/` unmodified.

## The Problem

Building a hardware-backed application on the XIAO RP2350 in Apache NuttX without modifying upstream submodules exposes four conflicts between the board's pinout, the RP2350 ADC driver (`rp23xx_adc.c`), the NuttShell USB console startup sequence, and the out-of-tree build system:

1.  **ADC Pin Clobbering and Analog Pad Input-Buffer Leakage (`GPIO 26–29`)**: During driver registration (`adc_register()`), `my_reset()` in `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` loops over the 4 GPIO entries in the 5-entry `g_gpio_map` (`{ 26, 27, 28, 29, -1 }`) regardless of which `CONFIG_RP23XX_ADC_CHANNEL*` options are enabled. It reconfigures all four GPIO pins (`GPIO 26–29`) as floating inputs (`RP23XX_GPIO_FUNC_NULL`), overwriting any LED pin setup on `D1` (`GPIO 27`) or `D2` (`GPIO 28`), and re-enables the digital input buffer (`RP23XX_PADS_BANK0_GPIO_IE`) that `rp23xx_common_earlyinitialize()` cleared for analog operation. Leaving `IE=1` on `GPIO 26–29` introduces digital input-buffer leakage on the potentiometer wiper (`GPIO 26`) and leaves unconnected pins `GPIO 27–29` floating with `IE=1`.
2.  **Unthrottled Free-Running ADC FIFO Interrupts and Stale Ring-Buffer Samples**: `rp23xx_adc.c` rejects `ANIOC_TRIGGER` (the upper-half `ioctl` command for software-triggered single conversions, returning `-ENOTTY`), sets no ADC clock divider, and re-arms a new conversion inside its FIFO interrupt handler as long as `/dev/adc0` remains open. Meanwhile, the upper-half ring buffer (`nuttx/drivers/analog/adc.c`, `CONFIG_ADC_FIFOSIZE=8`) drops new samples once full rather than overwriting old ones. A task that keeps `/dev/adc0` open and sleeps for 50 ms leaves the ADC FIFO interrupt firing continuously and reads stale samples from the previous wake cycle.
3.  **Headless Boot vs. Blocking USB-C Console Initialization**: Although `CONFIG_BOARD_LATE_INITIALIZE` defaults to `y` in `nuttx/sched/Kconfig` and upstream `xiao-rp2350/src/rp23xx_boardinitialize.c` calls `rp23xx_bringup()`, upstream `xiao-rp2350/configs/usbnsh/defconfig` leaves `CONFIG_ADC`, `CONFIG_RP23XX_ADC`, `CONFIG_USERLED`, and `CONFIG_USERLED_LOWER` unset (and defaults `CONFIG_ARCH_LEDS=y`), so `/dev/adc0` and `/dev/userleds` are not registered at boot. During startup, `nsh_main()` (`apps/system/nsh/nsh_main.c`) calls `nsh_initialize()` (`apps/nshlib/nsh_init.c`), which executes `/etc/init.d/rcS` before `nsh_consolemain()` (`apps/nshlib/nsh_usbconsole.c`) connects USB CDC/ACM and blocks waiting for three Enter keystrokes (`\r` or `\n`, used by `NSH_USBCONSOLE` to detect an open host terminal on `/dev/ttyACM0`). Any foreground task in `rcS` blocks USB console enumeration indefinitely, and because `# CONFIG_DEV_CONSOLE is not set` with `CONFIG_DEV_NULL=y`, `group_setupidlefiles()` (`nuttx/sched/group/group_setupidlefiles.c`) binds boot-time FDs `0, 1, 2` to `/dev/null`, discarding `rcS` stdout/stderr.
4.  **Single-LED Upstream Board Support and Out-of-Tree Build Wiring**: Upstream `xiao-rp2350/src/rp23xx_userleds.c` hardcodes a single onboard LED (`GPIO_LED1` / `BOARD_LED1_BIT`, `GPIO 25`) in `board_userled_all()` rather than iterating over an array of LEDs, and `CONFIG_ARCH_LEDS` defaults to `y` when `ARCH_HAVE_LEDS` is selected, compiling `rp23xx_autoleds.c` instead of `rp23xx_userleds.c`. In addition, `xiao-rp2350/src/CMakeLists.txt` leaves `CONFIG_ETC_ROMFS` unimplemented (`# TODO`).

## Goals

*   Keep all project source files, custom board files, startup scripts, unit tests, and build wrappers isolated in `xiao-rp2350-bargraph/` with zero modifications to tracked files in `nuttx/` or `apps/`.
*   Register `/dev/adc0` on `D0` (`GPIO 26` / `ADC0`) and clear digital pad input-enable (`IE=0`) across `GPIO 26–29` after `rp23xx_common_bringup()` completes.
*   Expose 6 active-high breadboard LEDs on `D3–D5` (`GPIO 5–7`) and `D8–D10` (`GPIO 2, 4, 3`) as a single bitmask device through `/dev/userleds`.
*   Sample `/dev/adc0` at 20 Hz using bounded per-tick `open` -> `poll` -> `read` -> critical-section `close` cycles so the RP2350 ADC FIFO interrupt stops safely between ticks, each tick reads a fresh conversion, and stalled conversions cannot hang the loop.
*   Discard samples flagged with ADC FIFO conversion errors, map valid readings across 7 discrete levels (`0..6`) with hysteresis, and update `/dev/userleds` only when the displayed level changes.
*   Launch a singleton `bargraph` background daemon at boot from ROMFS `/etc/init.d/rcS` without blocking USB CDC/ACM console enumeration (`nsh>`), supporting `SIGTERM` shutdown and restart from NSH.
*   Log boot, daemon lifecycle, and runtime errors to RAMLOG (`dmesg`), and blink an alternating fault pattern (`0x15` / `0x2A`) on the 6 hardware LEDs after repeated `/dev/adc0` failures when running headless.

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

*   **Given**: The board boots without a USB host attached (or is launched from NSH against a missing path via `bargraph /dev/adc_missing`), and `/dev/adc0` fails to open, times out in `poll()`, returns a hard read error, or reports `RP23XX_ADC_FIFO_ERR` (`sample.am_data < 0`) across `>= 5` consecutive 50 ms sampling ticks while `/dev/userleds` is open.
*   **When**: The `bargraph` daemon reaches `>= 5` consecutive failed ticks (isolated 1–4 tick errors or `am_data < 0` glitches reset on the next valid sample).
*   **Then**: The daemon logs the error via `syslog(LOG_ERR, ...)` (persisted in RAMLOG for `dmesg` inspection) and enters a fault loop that alternates the 6 hardware LEDs between `0x15` (`0b010101`) and `0x2A` (`0b101010`) at 250 ms intervals until terminated via `SIGTERM`.

## Technical Plan (Architecture)

### Out-of-Tree Project Layout

All custom board files, kernel configuration, ROMFS startup script, external application code, host unit tests, and build automation will reside inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`:

```text
/home/brett/Documents/GitHub/nuttxspace/
├── nuttx/                             (unchanged upstream submodule)
├── apps/
│   └── external -> ../xiao-rp2350-bargraph/external   (gitignored symlink)
└── xiao-rp2350-bargraph/
    ├── Makefile                       (top-level symlink, configure, build, and host-test wrapper)
    ├── board/                         (CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board")
    │   ├── configs/bargraph/defconfig
    │   ├── include/board.h
    │   ├── scripts/Make.defs
    │   └── src/
    │       ├── Make.defs
    │       ├── rp23xx_boardinitialize.c
    │       ├── rp23xx_bringup.c
    │       ├── rp23xx_userleds.c
    │       └── etc/init.d/rcS
    ├── external/                      (mounted into apps build via apps/external; Kconfig generated by preconfig)
    │   ├── Makefile
    │   ├── Make.defs
    │   └── bargraph/
    │       ├── Makefile
    │       ├── Make.defs
    │       ├── Kconfig
    │       ├── bargraph_logic.h
    │       └── bargraph_main.c
    └── tests/
        └── test_bargraph_logic.c
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
|      2) clear RP23XX_PADS_BANK0_GPIO_IE on GPIO 26..29                      |
|      3) userled_lower_initialize("/dev/userleds")          [6 outputs low]  |
+-----------------------------------------------------------------------------+
                                      |
                                      v
+-----------------------------------------------------------------------------+
| 2. NSH Initialization & Autonomous Background Launch                        |
|    nsh_main() -> nsh_initialize()                                           |
|      -> executes /etc/init.d/rcS                                            |
|      -> runs "bargraph" -> task_create("bargraph_daemon", ...) > 0;         |
|                            main() returns 0                                 |
|    nsh_consolemain()                                                        |
|      -> boardctl(BOARDIOC_USBDEV_CONNECT) [attaches USB pull-up]            |
|      -> blocks waiting for 3x Enter (\r or \n) on /dev/ttyACM0              |
+-----------------------------------------------------------------------------+
                                      |
                                      v  (runs concurrently at 20 Hz)
+-----------------------------------------------------------------------------+
| 3. bargraph_daemon 20 Hz Control Loop (stdio redirected to /dev/null)       |
|    open(adc_path, O_RDONLY | O_NONBLOCK) -> poll(POLLIN, 10 ms)             |
|      -> read(struct adc_msg_s) -> critical-section close(fd)                |
|      |                                                                      |
|      +--> [error, timeout, or am_data < 0] -> consecutive_errors++          |
|      |      (< 5 ticks: sleep 50 ms; >= 5 ticks: log + 0x15/0x2A blink)     |
|      +--> [valid] -> consecutive_errors = 0; raw = am_data >> 19 (0..4095)  |
|                   -> quantize_level(raw, current_level, 24) -> 0..6         |
|                   -> if level changed: ioctl(led_fd, ULEDIOC_SETALL, mask)  |
|    usleep(50000)  [trailing ADC completion ISR detaches IRQ on count == 0]  |
+-----------------------------------------------------------------------------+
```

### Detailed Subsystem Mechanisms

#### Board Bringup and Analog Pad Input-Buffer Fix

*   **`IE` Leakage Workaround**: Because `nuttx/` remains unmodified, `rp23xx_bringup.c` clears the digital input-enable bit (`IE`) on `GPIO 26–29` via `RP23XX_PADS_BANK0_GPIO_IE` immediately after `rp23xx_common_bringup()` completes. In `nuttx/drivers/analog/adc.c`, `adc_register()` invokes the lower-half `struct adc_ops_s` reset callback (`ao_reset`, bound to `my_reset()` in `rp23xx_adc.c`), which re-enables `IE=1` on those four pins. `ao_reset` is only invoked during registration unless explicitly called on an error path, and keeping `CONFIG_DEV_GPIO` disabled—along with omitting `rp23xx_gpio.c` from `board/src/Make.defs` and omitting `GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1` from `board.h`—ensures no GPIO character driver is compiled or bound to `GPIO 26–28`.
*   **Decoupled Fault-Resilient Bringup**: `rp23xx_bringup()` logs any negative return from `rp23xx_common_bringup()` via `syslog(LOG_ERR, ...)` without returning early, then runs the `GPIO 26–29` `IE` clear loop and `userled_lower_initialize("/dev/userleds")` before returning the aggregated status. Even if `/dev/adc0` registration fails at boot, `/dev/userleds` still registers so `bargraph_daemon` can display CUJ 3's fault blink pattern.

#### 6-LED `/dev/userleds` Lower-Half

*   **Driver Selection**: With `# CONFIG_ARCH_LEDS is not set`, `CONFIG_BOARD_CUSTOM_LEDS=y` (`ARCH_HAVE_LEDS`), `CONFIG_USERLED=y`, and `CONFIG_USERLED_LOWER=y`, the board builds `rp23xx_userleds.c` instead of `rp23xx_autoleds.c`.
*   **Pin Mapping and Control**: `board_userled_initialize()` configures all six LED pins (`GPIO 5, 6, 7, 2, 4, 3`) as outputs driven low (`false`) and returns `BOARD_NLEDS` (`6`). `board_userled()` updates a single LED index `0..5`, and `board_userled_all()` updates all `BOARD_NLEDS` pins from the 6-bit mask.

#### ADC Sampling, Normalization, and Hysteresis

*   **Per-Tick `open` / `poll` / `read` / Critical-Section `close` Cycle**: Because `nuttx/` is unmodified (`rp23xx_adc.c` ignores `RP23XX_ADC_DIV` and returns `-ENOTTY` for `ANIOC_TRIGGER`), opening `/dev/adc0` starts conversions and closing it stops them on the next FIFO completion interrupt. Two safeguards make this cycle safe for single-opener 20 Hz operation:
    *   **Critical-Section `close()` Guard**: In `nuttx/drivers/analog/adc.c`, `adc_close()` holds only `nxmutex_lock(&dev->ad_lock)` (not a critical section) when calling `ao_shutdown()` -> `my_shutdown()` -> `remove_device()`. Inside `rp23xx_adc.c`, `remove_device()` assumes it runs in a critical section and clears `g_first_device = NULL` before decrementing `g_active_count`—creating an assertion hazard (`ASSERT(next != g_current_channel)`) if `RP23XX_ADC_IRQ_FIFO` preempts between those two writes. Because `bargraph_daemon` is the sole opener of `/dev/adc0` (`dev->ad_lock` is uncontended and never blocks), wrapping `close(fd)` in `enter_critical_section()` / `leave_critical_section()` (`<nuttx/irq.h>`) ensures `g_first_device` and `g_active_count` update atomically before the trailing FIFO ISR runs and detaches `RP23XX_ADC_IRQ_FIFO`.
    *   **Bounded `poll()` Wait**: Opening `/dev/adc0` with `O_RDONLY | O_NONBLOCK` and waiting up to 10 ms via `poll()` (`POLLIN`, backed by `adc_poll()` in `adc.c` with default `CONFIG_ADC_NPOLLWAITERS=2`) before calling `read()` bounds each sampling tick. If an ADC conversion interrupt fails to arrive, `poll()` times out cleanly instead of blocking indefinitely in `nxsem_wait()`.
*   **Error Filtering and Scaling**: `rp23xx_adc.c` shifts the 12-bit conversion result left by 19 bits into `struct adc_msg_s` (`am_data`) and negates it when `RP23XX_ADC_FIFO_ERR` is set. `decode_adc_sample()` treats `am_data < 0` as `-EAGAIN` and shifts valid readings right by 19 bits (`0..4095`). Every non-`EINTR` failure (`open()` error, `poll()` timeout/error, `read()` error, or `-EAGAIN`) increments `consecutive_errors` across 50 ms ticks; any valid reading resets `consecutive_errors = 0`. Thus 1–4 tick glitches are absorbed without changing the LEDs, while `>= 5` consecutive failed ticks (250 ms) trigger RAMLOG logging and `fault_blink_loop()`.
*   **7-Level Hysteresis Quantizer**: `quantize_level()` divides full scale (`0..4095`) into 7 levels (`0..6`) with thresholds at multiples of `585` (`585, 1170, 1755, 2340, 2925, 3510`) and a `±24`-count hysteresis band (~0.6% of full scale), stepping monotonically across multiple buckets in a single tick on fast potentiometer turns. `level_to_led_mask()` converts the level to a 6-bit mask (`0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f`), and `/dev/userleds` is updated via `ULEDIOC_SETALL` only when the quantized level changes.

#### Daemon Lifecycle, ROMFS Startup, and Headless Diagnostics

*   **Flat Address-Space Singleton**: Modeled on `apps/examples/leds/leds_main.c`, `bargraph_main.c` runs inside NuttX's flat address space and tracks `g_bargraph_pid` (`-1` when inactive) under `sched_lock()` / `sched_unlock()`, verifying `g_bargraph_pid > 0 && kill(g_bargraph_pid, 0) == 0` so a `SIGKILL`ed task does not lock out future launches. Duplicate invocations print `"bargraph: daemon already running\n"` and exit `0`. An optional positional argument (`bargraph [adc_devpath]`) overrides `/dev/adc0` for runtime CUJ 3 testing (serving the same purpose as `-p <devpath>` in `apps/examples/adc/adc_main.c` without requiring `getopt()`).
*   **Stdio Isolation and `SIGTERM` Lifecycle**: Boot-time FDs `0–2` start bound to `/dev/null` via `group_setupidlefiles()`, and `bargraph_daemon` explicitly redirects FDs `0, 1, 2` to `/dev/null` via `dup2()` so interactive launches from `nsh>` never hold `/dev/ttyACM0` open. Its `SIGTERM` handler sets `g_bargraph_stop = 1` without calling non-async-signal-safe functions such as `syslog()`; on every exit path, a unified cleanup routine turns off all LEDs (`ULEDIOC_SETALL(0UL)`), closes `/dev/userleds`, and clears `g_bargraph_pid` under `sched_lock()` if `g_bargraph_pid == getpid()`.
*   **ROMFS Preprocessing and Build Packaging**: `nuttx/boards/Board.mk` preprocesses `etc/init.d/rcS` with `cpp` (requiring `#include <nuttx/config.h>`, C `/* ... */` comments, and `#ifdef CONFIG_BARGRAPH`), packages the tree with `genromfs`, and converts it with `xxd` and `sed` into `etctmp.c` inside `nuttx/boards/arm/rp23xx/common/`.
*   **RAMLOG Diagnostics and Fault Loop**: `bargraph_daemon` logs startup via `syslog(LOG_INFO, ...)`, and `task_create()`, initialization, and sustained runtime errors log via `syslog(LOG_ERR, ...)` to RAMLOG (`CONFIG_RAMLOG_SYSLOG=y`, `CONFIG_RAMLOG_BUFSIZE=4096`), readable via `dmesg` once USB NSH attaches. After `>= 5` consecutive failed sampling ticks, the daemon logs the error and alternates `ULEDIOC_SETALL` between `0x15` and `0x2A` every 250 ms until `SIGTERM` arrives.

## Alternatives Considered

### Alternative A: Modifying `nuttx/boards/arm/rp23xx/xiao-rp2350` In-Place

*   **Pros**: Avoids creating out-of-tree `scripts/Make.defs` and `rp23xx_boardinitialize.c` files.
*   **Reason for Rejection**: Dirties the `nuttx` git submodule and mixes breadboard-specific pin assignments into upstream board files instead of isolating the project under `xiao-rp2350-bargraph/` (`CONFIG_ARCH_BOARD_CUSTOM=y`).

### Alternative B: Driving 8 Breadboard LEDs (Using `D1`/`D2` `GPIO 27–28` or `D6`/`D7` `GPIO 0–1`)

*   **Pros**: Provides 8 bar-graph steps instead of 6.
*   **Reason for Rejection**: `rp23xx_adc.c` `my_reset()` resets `GPIO 26–29` to floating inputs during `adc_register()`, clobbering `D1` (`GPIO 27`) and `D2` (`GPIO 28`), while claiming `D6`/`D7` (`GPIO 0`/`1`) occupies the hardware `UART0` pins. Six LEDs on `D3–D5` and `D8–D10` satisfy the 0–6 bar-graph requirement without touching `GPIO 27–28` or `UART0`.

### Alternative C: Keeping `/dev/adc0` Continuously Open with `ANIOC_RESET_FIFO`

*   **Pros**: Eliminates per-tick `open()` and `close()` syscalls every 50 ms.
*   **Reason for Rejection**: `ANIOC_RESET_FIFO` (`nuttx/include/nuttx/analog/ioctl.h`) only resets the upper-half ring buffer head/tail pointers in `nuttx/drivers/analog/adc.c`; it does not pause lower-half conversions (`my_ioctl()` in `rp23xx_adc.c` returns `-ENOTTY` for `ANIOC_TRIGGER` and configures no clock divider). Keeping `/dev/adc0` open leaves the RP2350 ADC firing continuous FIFO interrupts during `usleep(50000)` while `adc_receive()` drops new samples once the 8-entry upper-half ring buffer fills (`ad_isovr`, `adc.c`'s internal ring-buffer overrun flag, is not set by `adc_receive()`). Per-tick `open`/`poll`/`read`/critical-section `close` stops conversions between ticks.

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

*   **Pros**: A custom RP2350 single-shot `adc_ops_s` lower-half inside `board/src/` (~80–120 lines of RP2350 ADC register code) removes the `open`–`close` window, the critical-section `close()` wrapper, and the post-bringup `IE` clear loop; software averaging reduces single-sample jitter.
*   **Reason for Rejection**: Writing and maintaining custom RP2350 ADC register-level driver code out of tree duplicates hardware access already provided by `rp23xx_adc.c` (663 lines) and `rp23xx_common_bringup.c` (~39 lines for ADC registration) when `enter_critical_section()` around `close(fd)` and the 4-pin `IE` clear loop resolve both `rp23xx_adc.c` quirks cleanly. Software multi-sample averaging is unnecessary because the `±24`-count hysteresis band (plus optional `100 nF` decoupling capacitor) is designed to prevent boundary flicker for noise within `±24` counts.

## Non-Functional Requirements

*   **CPU and Interrupt Load**: Conversions execute only during the per-tick `open("/dev/adc0")`–`close()` window. Inside `rp23xx_adc.c`, `get_next_channel()` starts each 2.0 µs conversion (96 cycles at `BOARD_ADC_FREQ = 48 MHz`) one at a time via `RP23XX_ADC_CS_START_ONCE` at the *end* of `interrupt_handler()`, guaranteeing 300 unhindered CPU cycles (at `BOARD_SYS_FREQ = 150 MHz`) of thread execution between the end of one FIFO ISR and the firing of the next, so `open()` + `poll()` + `read()` + `close()` completes within a few conversions when unpreempted.
*   **SRAM Footprint**: With 520 KB configured SRAM (`CONFIG_RAM_SIZE=532480` in `usbnsh/defconfig`), `CONFIG_RAMLOG_BUFSIZE=4096` (4 KB) and a 2048-byte daemon stack (`CONFIG_BARGRAPH_STACKSIZE=2048`) add ~6 KB of runtime RAM (excluding `.bss`/`.data` and ROMFS image size), to be verified against the `memmap_default.ld` linker memory report at build time and `ps` stack output on target.
*   **Rollback and Recovery**: The daemon installs a `SIGTERM` handler so `kill -15 <pid>` clears all LEDs, closes `/dev/userleds`, and exits. If a firmware build faults at boot, holding `BOOT` (`BOOTSEL`) while resetting the XIAO RP2350 enters the RP2350 ROM UF2 bootloader for re-flashing.

## Risks & Limits

*   **Breadboard LED Current Budget**: Driving 6 active-high LEDs simultaneously (`0x3f`) on `GPIO 2–7` with `>= 330 Ω` series resistors draws ~4–6 mA per pin (~24–36 mA total), remaining within the RP2350 per-GPIO drive ratings (up to 12 mA max standard drive) and 50 mA total `IOVDD` package limit.
*   **ADC Top-End Saturation, Reference Ripple, and Open-Wiper Floating Input**: Nominal thresholds at multiples of `585` (`level 6` entry at `3510 + 24 = 3534`) reserve the top ~14% of full scale (`3534..4095`) so all 6 LEDs illuminate before the wiper reaches the `3.3 V` mechanical end-stop. Because RP2350 uses `3.3 V` as the ADC reference and `GPIO 26` has `IE=0` and pulls disabled, `3.3 V` rail ripple exceeding ~19 mV (`±24` counts) can cross boundaries, and an open/disconnected potentiometer wiper floats to arbitrary LED levels without triggering CUJ 3. Mitigation: fit the optional `100 nF` capacitor (`D0` to `GND`) per Hardware Pin Mapping if jumper pickup exceeds `±24` counts.
*   **Exclusive `/dev/adc0` Ownership and Critical-Section `close()` Contract**:
    *   Because `close(fd)` is wrapped in `enter_critical_section()` / `leave_critical_section()` to make `remove_device()` atomic against `RP23XX_ADC_IRQ_FIFO`, `dev->ad_lock` inside `adc_close()` must never contend. Mitigation: keep `bargraph_daemon` as the sole opener of `/dev/adc0` (`CONFIG_EXAMPLES_ADC` unset), and use `poll()` with a `10 ms` timeout before `read()` so stalled conversions increment `consecutive_errors` rather than blocking indefinitely in `nxsem_wait()`.
*   **`0`-Sample `FIFO_ERR` Sign Limitation**: `rp23xx_adc.c` signals `RP23XX_ADC_FIFO_ERR` by negating `value` (`value = -value`). Limitation: `FIFO_ERR` is detectable only when the raw sample is non-zero; errors coinciding with a raw `0` reading remain `0` and are indistinguishable from a valid `0 V` sample (`level 0`).
*   **RAMLOG Circular Overwrite**: `CONFIG_RAMLOG_BUFSIZE=4096` overwrites older entries in FIFO order if logs exceed 4 KB before USB NSH attaches. Mitigation: log only boot/bringup errors, daemon start/exit, and fault transitions (never per-tick samples).
*   **Security Implications**: Not applicable (standalone, non-networked local breadboard firmware with physical USB-C/GPIO access only).

## Additional Resources

*   [nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/xiao-rp2350/include/board.h)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/Make.defs](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/xiao-rp2350/scripts/Make.defs)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/memmap_default.ld](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/xiao-rp2350/scripts/memmap_default.ld)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_bringup.c](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_bringup.c)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_userleds.c](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/xiao-rp2350/src/rp23xx_userleds.c)
*   [nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig)
*   [nuttx/boards/arm/rp23xx/common/src/rp23xx_common_initialize.c](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/common/src/rp23xx_common_initialize.c)
*   [nuttx/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c)
*   [nuttx/boards/arm/rp23xx/common/src/rp23xx_reset.c](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/common/src/rp23xx_reset.c)
*   [nuttx/boards/arm/rp23xx/common/src/rp23xx_boardctl.c](https://github.com/domesticmouse/nuttx/blob/egm/boards/arm/rp23xx/common/src/rp23xx_boardctl.c)
*   [nuttx/arch/arm/src/rp23xx/rp23xx_adc.c](https://github.com/domesticmouse/nuttx/blob/egm/arch/arm/src/rp23xx/rp23xx_adc.c)
*   [nuttx/arch/arm/src/rp23xx/rp23xx_gpio.h](https://github.com/domesticmouse/nuttx/blob/egm/arch/arm/src/rp23xx/rp23xx_gpio.h)
*   [nuttx/arch/arm/src/rp23xx/hardware/rp23xx_memorymap.h](https://github.com/domesticmouse/nuttx/blob/egm/arch/arm/src/rp23xx/hardware/rp23xx_memorymap.h)
*   [nuttx/drivers/analog/adc.c](https://github.com/domesticmouse/nuttx/blob/egm/drivers/analog/adc.c)
*   [nuttx/fs/inode/fs_files.c](https://github.com/domesticmouse/nuttx/blob/egm/fs/inode/fs_files.c)
*   [nuttx/sched/init/nx_bringup.c](https://github.com/domesticmouse/nuttx/blob/egm/sched/init/nx_bringup.c)
*   [nuttx/sched/group/group_setupidlefiles.c](https://github.com/domesticmouse/nuttx/blob/egm/sched/group/group_setupidlefiles.c)
*   [nuttx/sched/task/task_setup.c](https://github.com/domesticmouse/nuttx/blob/egm/sched/task/task_setup.c)
*   [nuttx/boards/Board.mk](https://github.com/domesticmouse/nuttx/blob/egm/boards/Board.mk)
*   [nuttx/boards/Kconfig](https://github.com/domesticmouse/nuttx/blob/egm/boards/Kconfig)
*   [nuttx/tools/Config.mk](https://github.com/domesticmouse/nuttx/blob/egm/tools/Config.mk)
*   [nuttx/tools/Unix.mk](https://github.com/domesticmouse/nuttx/blob/egm/tools/Unix.mk)
*   [nuttx/tools/rp23xx/Config.mk](https://github.com/domesticmouse/nuttx/blob/egm/tools/rp23xx/Config.mk)
*   [apps/Application.mk](https://github.com/domesticmouse/nuttx-apps/blob/egm/Application.mk)
*   [apps/Directory.mk](https://github.com/domesticmouse/nuttx-apps/blob/egm/Directory.mk)
*   [apps/system/nsh/nsh_main.c](https://github.com/domesticmouse/nuttx-apps/blob/egm/system/nsh/nsh_main.c)
*   [apps/nshlib/nsh_init.c](https://github.com/domesticmouse/nuttx-apps/blob/egm/nshlib/nsh_init.c)
*   [apps/nshlib/nsh_script.c](https://github.com/domesticmouse/nuttx-apps/blob/egm/nshlib/nsh_script.c)
*   [apps/nshlib/nsh_usbconsole.c](https://github.com/domesticmouse/nuttx-apps/blob/egm/nshlib/nsh_usbconsole.c)
*   [apps/examples/leds/leds_main.c](https://github.com/domesticmouse/nuttx-apps/blob/egm/examples/leds/leds_main.c)
*   [apps/examples/adc/adc_main.c](https://github.com/domesticmouse/nuttx-apps/blob/egm/examples/adc/adc_main.c)

## Future Improvements

*   Add an ADC sample-rate divider or timer-triggered DMA mode to `rp23xx_adc.c` upstream so continuous-open reads can run at 20 Hz without per-tick `open`/`close`.
*   Add PWM duty-cycle smoothing across intermediate potentiometer positions on spare RP2350 PWM slices.

--------------------------------------------------------------------------------

## Detailed Implementation Plan

All files below are created inside `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`. No tracked files inside `nuttx/` or `apps/` are edited; only the gitignored `apps/external` symlink, generated `external/Kconfig` / `apps/Kconfig`, and build outputs are created during configuration and compilation. During ROMFS packaging, `nuttx/boards/Board.mk` generates `etctmp/` (`ETCDIR := etctmp`) and `etctmp.c` inside `nuttx/boards/arm/rp23xx/common/`, both of which are already ignored by `nuttx/boards/arm/rp23xx/common/.gitignore` (`etctmp*`) so `git status` remains clean without deleting them on incremental builds.

### Target Files to Create (Phased Delivery)

#### Phase 1: Out-of-Tree Build Wrapper, Custom Board Skeleton, and `defconfig`

##### 1. `xiao-rp2350-bargraph/Makefile` (Create)

*   **Type / Scope**: Top-level GNU Make build, configuration, and host-test wrapper.
*   **Rationale**: Keeps wrapper scripts, custom board files, and application sources outside `nuttx/` while managing the `apps/external` symlink and Kconfig regeneration (`preconfig`, the `apps/` Make target that invokes `apps/tools/mkkconfig.sh` to assemble composite `Kconfig` files).
*   **Proposed Changes**:
    *   `setup` target (requires `external/Makefile`, created in Phase 3):
        *   Creates or verifies the symlink `../apps/external -> ../xiao-rp2350-bargraph/external`, aborting if `../apps/external` exists as a non-symlink directory.
        *   Because `apps/Makefile` defines `Kconfig:` with no prerequisites and `apps/Make.defs` filters out `external/` whenever `external/Kconfig` exists, explicitly runs `rm -f ../apps/Kconfig external/Kconfig` whenever `../apps/external` was just created, `external/Kconfig` is missing, or `external/bargraph/Kconfig` is newer than `../apps/Kconfig`.
        *   If `../nuttx/.config` already exists, runs `$(MAKE) -C ../nuttx olddefconfig` to refresh Kconfig trees (`apps_preconfig` runs `$(MAKE) -C $(APPDIR) preconfig` with `DELIM` defined); before `configure`, `tools/configure.sh` triggers `olddefconfig` directly (if invoking `apps` `preconfig` standalone prior to `configure`, pass `DELIM=/`).
    *   `configure` target: Depends on `setup` (once Phase 3 `external/` exists; during Phase 1–2 validation, run `./tools/configure.sh` before `external/` exists); runs `cd ../nuttx && ./tools/configure.sh -e -l ../xiao-rp2350-bargraph/board/configs/bargraph` (`-e` runs `distclean` if configuration changed; `-l` selects the Linux host environment).
    *   `all` (default) target: Depends on `setup`; runs `configure` if `../nuttx/.config` is missing, then runs `$(MAKE) -C ../nuttx` to produce `../nuttx/nuttx.uf2` (leaving `etctmp/` and `etctmp.c` intact for fast incremental builds).
    *   `test` target: Compiles `tests/test_bargraph_logic.c` with host `gcc -Wall -Wextra -Werror -Iexternal/bargraph` and executes the resulting host binary.
    *   `clean` target: Runs `$(MAKE) -C ../nuttx clean` if `../nuttx/Make.defs` and `../nuttx/.config` exist, and removes `../nuttx/boards/arm/rp23xx/common/etctmp` and `../nuttx/boards/arm/rp23xx/common/etctmp.c` if present.
    *   `distclean` target: Runs `$(MAKE) -C ../nuttx distclean` if configured, then removes `../apps/external`, `external/Kconfig`, `../apps/Kconfig`, host test binaries, and any leftover `etctmp` / `etctmp.c`.

##### 2. `xiao-rp2350-bargraph/board/include/board.h` (Create)

*   **Type / Scope**: Custom board header (`<arch/board/board.h>`).
*   **Rationale**: Defines RP2350 clock frequencies, omits upstream `GPIO 26–28` generic GPIO macros (`GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1`) so no GPIO character driver can claim ADC or avoided pins, sets `BOARD_NLEDS = 6`, and declares board bringup interfaces.
*   **Proposed Changes**:
    *   Retain all 13 clock macros (`MHZ`, `BOARD_XOSC_FREQ`, `BOARD_XOSC_STARTUPDELAY`, `BOARD_PLL_SYS_FREQ`, `BOARD_PLL_USB_FREQ`, `BOARD_REF_FREQ`, `BOARD_SYS_FREQ`, `BOARD_PERI_FREQ`, `BOARD_USB_FREQ`, `BOARD_ADC_FREQ`, `BOARD_HSTX_FREQ`, `BOARD_UART_BASEFREQ`, `BOARD_TICK_CLOCK`) and `BOARD_GPIO_LED_PIN = 25` from `nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h`.
    *   Omit `"rp23xx_i2cdev.h"`, `"rp23xx_spidev.h"`, `"rp23xx_i2sdev.h"`, and `"rp23xx_spisd.h"`, omit `BOARD_NGPIOOUT`/`BOARD_NGPIOIN`/`BOARD_NGPIOINT`/`GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1`, and omit `BOARD_LED1`/`BOARD_LED_YELLOW`/`BOARD_LED1_BIT`/`LED_STARTED`..`LED_PANIC` (`CONFIG_ARCH_LEDS` is disabled).
    *   Define `BOARD_NLEDS` (`6`), `GPIO_ADC_BASE` (`26`), and `GPIO_ADC_COUNT` (`4`).
    *   Under `#ifndef __ASSEMBLY__` (matching upstream `xiao-rp2350/include/board.h`), declare `void rp23xx_boardearlyinitialize(void);`, `void rp23xx_boardinitialize(void);`, and `int rp23xx_bringup(void);`.

##### 3. `xiao-rp2350-bargraph/board/scripts/Make.defs` (Create)

*   **Type / Scope**: Custom board compiler flags and linker script Makefile fragment.
*   **Rationale**: `tools/configure.sh` symlinks `nuttx/Make.defs` to `$(BOARD_DIR)/scripts/Make.defs`.
*   **Proposed Changes**:
    *   Include `$(TOPDIR)/.config`, `$(TOPDIR)/tools/Config.mk`, `$(TOPDIR)/tools/rp23xx/Config.mk`, and `$(TOPDIR)/arch/arm/src/armv8-m/Toolchain.defs`.
    *   Set `LDSCRIPT = memmap_default.ld` (matching `CONFIG_BOOT_RUNFROMFLASH=y`) and `ARCHSCRIPT += $(TOPDIR)$(DELIM)boards$(DELIM)arm$(DELIM)rp23xx$(DELIM)xiao-rp2350$(DELIM)scripts$(DELIM)$(LDSCRIPT)`.
    *   Set `CFLAGS`, `CXXFLAGS`, `CPPFLAGS`, and `AFLAGS` matching `nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/Make.defs` (omitting unused `NXFLATLDFLAGS*`).

##### 4. `xiao-rp2350-bargraph/board/configs/bargraph/defconfig` (Create)

*   **Type / Scope**: Compressed NuttX `.config` template derived from `nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig`.
*   **Rationale**: Configures the custom board, common RP2350 helpers, `ADC0`, 6-LED `userleds`, ROMFS `/etc/init.d/rcS`, USB CDC/ACM NSH, RAMLOG `dmesg`, and (in Phase 3) `CONFIG_BARGRAPH=y`.
*   **Proposed Changes**:
    *   **Removals relative to `usbnsh/defconfig` (5 lines)**: Remove `CONFIG_ARCH_BOARD="xiao-rp2350"`, `CONFIG_ARCH_BOARD_XIAO_RP2350=y`, `CONFIG_EXAMPLES_HELLO=y`, `CONFIG_TESTING_GETPRIME=y`, and `CONFIG_TESTING_OSTEST=y` (`CONFIG_DEV_GPIO` remains unset).
    *   **Retained baseline from `usbnsh/defconfig` (38 lines)**: Keep all remaining 38 `usbnsh/defconfig` lines unchanged (USB CDC/ACM console, `nsh_main`, builtin apps, `ps`, `kill`, signals, `procfs`, RP23XX architecture/clock/RAM settings, and `# CONFIG_RP23XX_UART0 is not set` / `# CONFIG_DEV_CONSOLE is not set`).
    *   **New / modified symbols**:
        *   Custom board & init: `CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_ARCH_BOARD_CUSTOM_NAME="xiao-rp2350-bargraph"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y`, `CONFIG_BOARD_CUSTOM_LEDS=y`, `# CONFIG_ARCH_LEDS is not set`, `CONFIG_BOARD_LATE_INITIALIZE=y` (default `y` in `nuttx/sched/Kconfig`, listed explicitly for clarity), `CONFIG_DEV_NULL=y`.
        *   Drivers: `CONFIG_ANALOG=y`, `CONFIG_ADC=y`, `CONFIG_RP23XX_ADC=y`, `CONFIG_RP23XX_ADC_CHANNEL0=y`, `CONFIG_USERLED=y`, `CONFIG_USERLED_LOWER=y` (`CONFIG_BARGRAPH=y` is added in Phase 3 once `external/bargraph/Kconfig` exists; `CONFIG_ADC_NPOLLWAITERS` defaults to `2`).
        *   ROMFS & RAMLOG: `CONFIG_FS_ROMFS=y`, `CONFIG_ETC_ROMFS=y`, `# CONFIG_NSH_DISABLESCRIPT is not set`, `CONFIG_RAMLOG=y`, `CONFIG_RAMLOG_SYSLOG=y`, `CONFIG_RAMLOG_BUFSIZE=4096`. (`CONFIG_RP23XX_ROMFS_ROMDISK_DEVNAME` remains unset so only `CONFIG_ETC_ROMFS` / `nx_romfsetc()` mounts `/etc`.)

*   **Phase 1 Checkpoint**: Running `cd nuttx && ./tools/configure.sh -e -l ../xiao-rp2350-bargraph/board/configs/bargraph` configures `nuttx/.config` (verify `CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_RP23XX_ADC=y`, `CONFIG_USERLED=y`, `CONFIG_ETC_ROMFS=y`, and `CONFIG_RAMLOG_SYSLOG=y`).

#### Phase 2: Board Bringup, Analog Pad `IE` Fix, 6-LED Driver, and ROMFS Startup Script

##### 5. `xiao-rp2350-bargraph/board/src/Make.defs` (Create)

*   **Type / Scope**: Board source and ROMFS script build list.
*   **Rationale**: Registers board initialization, bringup, 6-LED driver, and ROMFS `/etc/init.d/rcS` with `nuttx/boards/Board.mk` (omitting `rp23xx_gpio.c` so `CONFIG_DEV_GPIO` is never compiled into the custom board). With `CONFIG_ARCH_BOARD_COMMON=y`, `nuttx/tools/Unix.mk` symlinks `arch/arm/src/board` to `nuttx/boards/arm/rp23xx/common` and `arch/arm/src/board/board` to `$(BOARD_DIR)/src`, which is why the custom board include path uses `board/board`.
*   **Proposed Changes**:
    *   Add `rp23xx_boardinitialize.c` and `rp23xx_bringup.c` to `CSRCS`.
    *   Under `ifneq ($(CONFIG_ARCH_LEDS),y)`, add `rp23xx_userleds.c` to `CSRCS`.
    *   Under `ifeq ($(CONFIG_ETC_ROMFS),y)`, set `RCSRCS = etc/init.d/rcS`.
    *   Set `DEPPATH += --dep-path board`, `VPATH += :board`, and `CFLAGS += ${INCDIR_PREFIX}$(TOPDIR)$(DELIM)arch$(DELIM)$(CONFIG_ARCH)$(DELIM)src$(DELIM)board$(DELIM)board`.

##### 6. `xiao-rp2350-bargraph/board/src/rp23xx_boardinitialize.c` (Create)

*   **Type / Scope**: Early, standard, and late board initialization hooks.
*   **Rationale**: Invokes shared RP2350 common initialization and exposes `board_late_initialize()` so `rp23xx_bringup()` runs before NSH executes `/etc/init.d/rcS`.
*   **Proposed Changes**:
    *   `rp23xx_boardearlyinitialize()`: Calls `rp23xx_common_earlyinitialize()` and configures `BOARD_GPIO_LED_PIN` (`GPIO 25`) as output driven `true` (active-low onboard LED off).
    *   `rp23xx_boardinitialize()`: Calls `rp23xx_common_initialize()`.
    *   `board_late_initialize()`: Guarded by `#ifdef CONFIG_BOARD_LATE_INITIALIZE`, calls `(void)rp23xx_bringup();`.

##### 7. `xiao-rp2350-bargraph/board/src/rp23xx_bringup.c` (Create)

*   **Type / Scope**: Peripheral driver bringup and analog pad fix.
*   **Rationale**: Registers `/dev/adc0` via `rp23xx_common_bringup()`, clears `RP23XX_PADS_BANK0_GPIO_IE` across `GPIO 26–29`, and registers `/dev/userleds` even if `/dev/adc0` registration fails.
*   **Proposed Changes**:
    *   In `int rp23xx_bringup(void)`:
        1. Call `rp23xx_common_bringup()`; if `< 0`, log via `syslog(LOG_ERR, ...)` **without returning early**.
        2. Under `#if defined(CONFIG_ADC) && defined(CONFIG_RP23XX_ADC)`, clear `RP23XX_PADS_BANK0_GPIO_IE` via `clrbits_reg32()` across `GPIO_ADC_BASE` (`26`) for `GPIO_ADC_COUNT` (`4`) pins (pulling in `clrbits_reg32` and pad register definitions via `"rp23xx_gpio.h"`).
        3. Under `#ifdef CONFIG_USERLED`, call `userled_lower_initialize("/dev/userleds")`; on `< 0`, log via `syslog(LOG_ERR, ...)` and propagate the error if prior bringup succeeded.
        4. Return the aggregated status (`ret`).

##### 8. `xiao-rp2350-bargraph/board/src/rp23xx_userleds.c` (Create)

*   **Type / Scope**: Multi-LED `userleds` lower-half driver (`!CONFIG_ARCH_LEDS`).
*   **Rationale**: Replaces upstream `xiao-rp2350/src/rp23xx_userleds.c` which hardcodes a single LED (`GPIO 25`) in `board_userled_all()`.
*   **Proposed Changes**:
    *   Wrap implementation in `#ifndef CONFIG_ARCH_LEDS`; define the 6 breadboard LED pins locally (`{ 5, 6, 7, 2, 4, 3 }`) for `BOARD_NLEDS` (`6`).
    *   Implement `board_userled_initialize()` (configure all 6 pins as outputs driven `false`, return `BOARD_NLEDS`), `board_userled(int led, bool ledon)`, and `board_userled_all(uint32_t ledset)`.

##### 9. `xiao-rp2350-bargraph/board/src/etc/init.d/rcS` (Create)

*   **Type / Scope**: ROMFS NSH startup script processed by `Board.mk` via `cpp`.
*   **Rationale**: Launches `bargraph` automatically at power-on before `nsh_consolemain()` waits for USB attachment.
*   **Proposed Changes**:
    *   Include `<nuttx/config.h>` and use C-style `/* ... */` comments (no `#` shell comments).
    *   Under `#ifdef CONFIG_BARGRAPH`, invoke `bargraph`.

*   **Phase 2 Checkpoint**: Running `make -C nuttx` compiles and packages `../nuttx/nuttx.uf2` containing `/dev/adc0`, `/dev/userleds`, and `/etc/init.d/rcS` compiled via `etctmp.c`, leaving `git status` clean in `nuttx/` and `apps/`.

#### Phase 3: External `bargraph` Daemon Application & Host Unit Tests

##### 10. `xiao-rp2350-bargraph/external/Makefile` and `Make.defs` (Create)

*   **Type / Scope**: `apps/external` directory glue (`apps/` scans `apps/external` automatically when present).
*   **Rationale**: Integrates `external/bargraph` into the NuttX `apps/` build and `preconfig` Kconfig assembly tree without editing `apps/Makefile` or `apps/Kconfig`.
*   **Proposed Changes**:
    *   `external/Makefile`: Set `MENUDESC = "External"` and include `$(APPDIR)/Directory.mk`.
    *   `external/Make.defs`: `include $(wildcard $(APPDIR)/external/*/Make.defs)`.

##### 11. `xiao-rp2350-bargraph/external/bargraph/Kconfig` (Create)

*   **Type / Scope**: Application configuration symbols.
*   **Rationale**: Exposes `CONFIG_BARGRAPH` and standard task attributes (`PROGNAME`, `PRIORITY`, `STACKSIZE`) matching `apps/examples/leds/Kconfig`, without adding unnecessary Kconfig options for fixed hardware paths or coupled quantizer constants.
*   **Proposed Changes**:
    *   Define `config BARGRAPH` (`tristate "Potentiometer LED bar graph daemon"`, `default n`, `depends on ADC && USERLED && USERLED_LOWER && BUILTIN`).
    *   Under `if BARGRAPH`, define `BARGRAPH_PROGNAME` (`"bargraph"`), `BARGRAPH_PRIORITY` (`100`, `range 1 255`), and `BARGRAPH_STACKSIZE` (`2048`, `range 1024 8192`).

##### 12. `xiao-rp2350-bargraph/external/bargraph/Makefile` and `Make.defs` (Create)

*   **Type / Scope**: Application build registration.
*   **Rationale**: Registers `bargraph` as a built-in NSH application compiled from `bargraph_main.c`.
*   **Proposed Changes**:
    *   `Make.defs`: `ifneq ($(CONFIG_BARGRAPH),)` -> `CONFIGURED_APPS += $(APPDIR)/external/bargraph`.
    *   `Makefile`: Include `$(APPDIR)/Make.defs`, set `PROGNAME = $(CONFIG_BARGRAPH_PROGNAME)`, `PRIORITY = $(CONFIG_BARGRAPH_PRIORITY)`, `STACKSIZE = $(CONFIG_BARGRAPH_STACKSIZE)`, `MODULE = $(CONFIG_BARGRAPH)`, `MAINSRC = bargraph_main.c`, and include `$(APPDIR)/Application.mk`.

##### 13. `xiao-rp2350-bargraph/external/bargraph/bargraph_logic.h` and `xiao-rp2350-bargraph/tests/test_bargraph_logic.c` (Create)

*   **Type / Scope**: Pure-C quantizer/ADC decoding header (standard C headers only: `<stdint.h>`, `<errno.h>`) and host unit test suite.
*   **Rationale**: Isolates pure math/decoding logic from NuttX headers so `make -C xiao-rp2350-bargraph test` can test the exact production helper functions on the Linux host without duplicating logic.
*   **Proposed Changes**:
    *   `bargraph_logic.h`:
        *   Define constants `BARGRAPH_INTERVAL_US` (`50000`), `BARGRAPH_POLL_TIMEOUT_MS` (`10`), `BARGRAPH_FAULT_TICKS` (`5`), `BARGRAPH_FAULT_BLINK_US` (`250000`), and `BARGRAPH_HYSTERESIS` (`24`).
        *   `decode_adc_sample(int32_t am_data, uint16_t *raw_out)`: Returns `-EAGAIN` if `am_data < 0` (`RP23XX_ADC_FIFO_ERR`); otherwise writes `(uint16_t)(am_data >> 19)` (`0..4095`) and returns `0`.
        *   `quantize_level(uint16_t raw, int current_level, int hyst)`: Applies signed `int` thresholds `{ 585, 1170, 1755, 2340, 2925, 3510 }` with hysteresis `hyst`, stepping up or down across multiple levels in one call and clamping to `0..6`.
        *   `level_to_led_mask(int level)`: Returns `(level <= 0) ? 0U : ((1U << level) - 1U)`.
    *   `tests/test_bargraph_logic.c`: Host `gcc` test program using `<assert.h>` verifying `quantize_level()`, `decode_adc_sample()`, and `level_to_led_mask()` across all boundaries and error cases.

##### 14. `xiao-rp2350-bargraph/external/bargraph/bargraph_main.c` and `defconfig` Update (Create / Update)

*   **Type / Scope**: User-space C singleton daemon (`main` + `bargraph_daemon`) and enabling `CONFIG_BARGRAPH=y` in `board/configs/bargraph/defconfig`.
*   **Rationale**: Implements the 20 Hz potentiometer-to-bar-graph loop, per-tick ADC `open`/`poll`/`read`/critical-section `close` cycle, hysteresis smoothing, stdio isolation, `SIGTERM` shutdown, and headless LED fault blinking.
*   **Proposed Changes**:
    *   **State**: `static pid_t g_bargraph_pid = -1;` and `static volatile sig_atomic_t g_bargraph_stop = 0;` (protected by `sched_lock()` / `sched_unlock()`).
    *   **CLI & `main()`**:
        *   Accepts optional positional `argv[1]` ADC device path override (default `"/dev/adc0"`), building `FAR char * const daemon_argv[] = { (FAR char *)adc_path, NULL };`.
        *   Under `sched_lock()`: if `g_bargraph_pid > 0 && kill(g_bargraph_pid, 0) == 0`, calls `sched_unlock()`, prints `"bargraph: daemon already running\n"`, and returns `EXIT_SUCCESS`. Otherwise sets `g_bargraph_stop = 0`, calls `task_create("bargraph_daemon", CONFIG_BARGRAPH_PRIORITY, CONFIG_BARGRAPH_STACKSIZE, bargraph_daemon, daemon_argv)` (NuttX prepends `"bargraph_daemon"` as `argv[0]` and passes `adc_path` in `argv[1]`), stores `g_bargraph_pid = pid` on `pid > 0`, calls `sched_unlock()`, and if `pid < 0` logs via `syslog(LOG_ERR, ...)` and returns `EXIT_FAILURE`. Returns `EXIT_SUCCESS` on launch.
    *   **Daemon Startup, Stdio Isolation, & Signal Setup (`bargraph_daemon(int argc, FAR char *argv[])`)**:
        *   Reads `adc_path = (argc >= 2 && argv[1] != NULL) ? argv[1] : "/dev/adc0"`.
        *   Initializes `int led_fd = -1; int ret_code = EXIT_SUCCESS;`.
        *   Redirects FDs `0, 1, 2` to `/dev/null` via `open("/dev/null", O_RDWR)` and `dup2()` (closing the temporary descriptor if `fd > 2`); if `open()` or any `dup2()` fails, logs via `syslog(LOG_ERR, ...)`, sets `ret_code = EXIT_FAILURE`, and jumps to `cleanup:`.
        *   Installs `SIGTERM` handler (`SA_SIGINFO`, no `SA_RESTART`; `sigterm_handler` sets only `g_bargraph_stop = 1` without calling `syslog()`); on `sigaction() != 0`, logs via `syslog(LOG_ERR, ...)`, sets `ret_code = EXIT_FAILURE`, and jumps to `cleanup:`.
        *   Opens `"/dev/userleds"` (`O_WRONLY`) and clears LEDs (`ULEDIOC_SETALL(0UL)`); on failure, logs via `syslog(LOG_ERR, ...)`, sets `ret_code = EXIT_FAILURE`, and jumps to `cleanup:`.
        *   Logs startup via `syslog(LOG_INFO, "bargraph: daemon started (pid=%d, adc=%s)\n", (int)getpid(), adc_path)`.
    *   **ADC Read (`read_adc_sample(const char *adc_path, uint16_t *raw_out)`)**:
        *   Opens `adc_path` (`O_RDONLY | O_NONBLOCK`). If `fd < 0`, returns `(errno == EINTR) ? -EINTR : -errno` immediately without calling `read()` or `close()`.
        *   Calls `poll(&pfd, 1, BARGRAPH_POLL_TIMEOUT_MS)` (`POLLIN`). If `poll()` returns `0` (timeout), closes `fd` inside `enter_critical_section()` / `leave_critical_section()` (`<nuttx/irq.h>`) and returns `-EAGAIN`. If `poll() < 0` or `!(pfd.revents & POLLIN)`, captures `err = errno`, closes `fd` inside `enter_critical_section()` / `leave_critical_section()`, and returns `(poll_ret < 0 && err == EINTR) ? -EINTR : (poll_ret < 0 ? -err : -EIO)`.
        *   Reads `sizeof(struct adc_msg_s)` bytes into `sample`, captures `err = errno`, and closes `fd` inside `enter_critical_section()` / `leave_critical_section()`.
        *   Returns `-EINTR` if `nbytes < 0 && err == EINTR`, `-err` on other `nbytes < 0`, `-EIO` if `nbytes != (ssize_t)sizeof(struct adc_msg_s)`, or `decode_adc_sample(sample.am_data, raw_out)`.
    *   **Control Loop, Fault Blink, & Unified Cleanup**:
        *   Initializes `int current_level = 0;` and `int consecutive_errors = 0;`. Each tick calls `read_adc_sample(adc_path, &raw)`:
            *   If `ret == -EINTR` or `g_bargraph_stop`: breaks out of loop.
            *   If `ret < 0`: increments `consecutive_errors`. When `consecutive_errors == BARGRAPH_FAULT_TICKS` (`5`), logs once via `syslog(LOG_ERR, ...)`, runs `fault_blink_loop(led_fd)` (alternating `0x15UL` and `0x2aUL` every `BARGRAPH_FAULT_BLINK_US` while `!g_bargraph_stop`, returning `-1` if `ioctl()` fails), sets `ret_code = EXIT_FAILURE` if `fault_blink_loop` returned `< 0`, and breaks out of the loop; while `consecutive_errors < 5`, sleeps `usleep(BARGRAPH_INTERVAL_US)` before retrying.
            *   If `ret == 0`: resets `consecutive_errors = 0`, calls `quantize_level(raw, current_level, BARGRAPH_HYSTERESIS)`, and if `next_level != current_level` updates `current_level` and writes `level_to_led_mask(current_level)` via `ioctl(led_fd, ULEDIOC_SETALL, ...)` (logging via `syslog(LOG_ERR, ...)`, setting `ret_code = EXIT_FAILURE`, and jumping to `cleanup:` on `ioctl < 0`), then sleeps `usleep(BARGRAPH_INTERVAL_US)`.
        *   At unified `cleanup:` label (reached on every exit path): if `led_fd >= 0`, calls `(void)ioctl(led_fd, ULEDIOC_SETALL, 0UL); close(led_fd);`; under `sched_lock()` / `sched_unlock()`, if `g_bargraph_pid == getpid()` sets `g_bargraph_pid = -1`; returns `ret_code`.
    *   **Enable App in `defconfig`**: Add `CONFIG_BARGRAPH=y` to `board/configs/bargraph/defconfig`.

*   **Phase 3 Checkpoint**: Running `make -C xiao-rp2350-bargraph configure && make -C xiao-rp2350-bargraph` links `../apps/external`, sets `CONFIG_BARGRAPH=y` in `nuttx/.config`, and builds `../nuttx/nuttx.uf2`.

### Verification Plan

*   **Host Prerequisite Check**:
    *   Verify `arm-none-eabi-gcc`, `genromfs`, `xxd`, and `picotool` (or `$PICO_SDK_PATH`) are installed on the Linux host (all four are required to compile and package `nuttx.uf2`; at flash time, either `picotool load` or `BOOTSEL` UF2 drag-and-drop can be used).
*   **Host Unit Check (Quantizer & Decoder)**:
    *   Run `make -C /home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph test` to exercise `quantize_level()` across all 7 levels (`0..6`), ascending/descending `±24` hysteresis boundaries (`585 ± 24`, `3510 ± 24`, `0`, `4095`), multi-step jumps (`0 -> 4095` and `4095 -> 0` in one call), `decode_adc_sample()` (`am_data < 0` rejection and `am_data >> 19` scaling), and `level_to_led_mask()`.
*   **Compilation / Build**: Run `make -C /home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph` and verify:
    1. `../apps/external` resolves to `../xiao-rp2350-bargraph/external`, and `git status` inside both `nuttx/` and `apps/` reports zero modified or untracked files.
    2. `../nuttx/nuttx.uf2` builds with zero compiler errors/warnings in `xiao-rp2350-bargraph/`, and linker output stays within `memmap_default.ld` limits.
    3. Generated ROMFS image includes `/etc/init.d/rcS`.
*   **Hardware / Integration Verification (On XIAO RP2350 Breadboard)**:
    1. Flash `nuttx.uf2` via `BOOTSEL` (`picotool load -f ../nuttx/nuttx.uf2 -x` or UF2 drag-and-drop).
    2. **Autonomous Boot & Quantizer Tracking (CUJ 1)**: Power the board from a 5 V supply with no host serial terminal open. Rotate the potentiometer on `D0` from `0 V` to `3.3 V` and back, verifying 0 to 6 LEDs (`D3`, `D4`, `D5`, `D8`, `D9`, `D10`) step monotonically across all 7 states (`0x00`..`0x3f`) and back down with no flicker near stationary threshold boundaries.
    3. **USB-C NSH, IRQ Reduction, & Lifecycle (CUJ 2)**: Plug into the Linux host, open `/dev/ttyACM0`, press Enter 3 times to reach `nsh>`, and verify:
       * `dmesg` shows no boot or driver errors and displays the `bargraph: daemon started` log entry.
       * `cat /proc/interrupts` sampled twice 1 second apart shows an ADC FIFO IRQ count delta of `20–200` interrupts/second (consistent with 20 Hz `open`/`poll`/`read`/`close` cycles) rather than continuous free-running FIFO interrupts (up to the 500 kSPS hardware conversion ceiling).
       * `ps` shows `bargraph_daemon` running with peak stack usage `<= 1536` bytes out of `2048` bytes (`>= 512` bytes free headroom); running `bargraph` a second time prints `"bargraph: daemon already running"` without spawning a duplicate.
       * `kill -15 <pid>` stops `bargraph_daemon` and turns off all 6 LEDs; running `bargraph` restarts it; unplugging/replugging USB-C re-enumerates `/dev/ttyACM0` without FD lockup.
    4. **Fault Indication & Cleanup (CUJ 3)**: From `nsh>`, run `kill -15 <pid>` and then `bargraph /dev/adc_missing`, verify the 6 LEDs alternate between `0x15` and `0x2A` at 250 ms, verify `dmesg` records the `syslog(LOG_ERR, ...)` entry, and verify `kill -15 <pid>` stops the fault blink and clears all LEDs before restarting `bargraph`.

--------------------------------------------------------------------------------

*Drafted with EGM*
