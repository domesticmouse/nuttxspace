# Out-of-Tree XIAO RP2350 Potentiometer LED Bar Graph (xiao-rp2350-bargraph)

* **Author:** Brett Morgan
* **Date:** 2026-11-13

## Overview

This project is a hands-on Apache NuttX learning exercise targeting a Seeed Studio XIAO RP2350 mounted on a breadboard with one potentiometer and 6 discrete LEDs (with series current-limiting resistors to GND). Everything specific to the project lives in `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph`, kept outside the `nuttx` kernel submodule and the `apps` user-space submodule.

The firmware pairs two parts:

1. **An out-of-tree custom board configuration** built with NuttX's custom-board mechanism (`CONFIG_ARCH_BOARD_CUSTOM=y`) while reusing shared RP2350 chip and board helpers (`CONFIG_ARCH_BOARD_COMMON=y`). It maps the potentiometer to `D0` (`GPIO 26` / `ADC0`) at `/dev/adc0`, maps 6 active-high breadboard LEDs (`D3`, `D4`, `D5`, `D8`, `D9`, `D10`) to a single bitmask device at `/dev/userleds`, and provides a NuttShell (`nsh>`) prompt over the USB-C port (USB CDC/ACM serial).
2. **An external user-space C application** connected into the `apps` build via `apps/external`. It starts automatically at boot via an embedded read-only startup script (`/etc/init.d/rcS` in a ROMFS image) before a USB-C terminal attaches, samples `/dev/adc0` every ~50 ms (20 Hz) using `open`/`read`/`close` per tick, and updates the 6-LED bar graph over `/dev/userleds`.

## Analysis

Reviews from all 7 Goldfish lenses (`egm-design`, `egm-codebase-details`, `egm-zoom-out`, `egm-comprehension`, `egm-implementation-readiness`, `egm-structural`, `egm-inventive-catalyst`) surfaced the following hardware, driver, boot-ordering, and build-system constraints, along with the resolutions adopted for `xiao-rp2350-bargraph/`.

### 1. ADC Pin Reset (`GPIO 26–29`) and Analog Pad Input-Enable Conflict
* **The Hazard:** In `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`, `my_reset()` loops over all four entries in `g_gpio_map` (`GPIO 26, 27, 28, 29`) unconditionally when `adc_register()` runs—even when only `CONFIG_RP23XX_ADC_CHANNEL0=y` (`GPIO 26`) is enabled—setting each pin to input with `RP23XX_GPIO_FUNC_NULL`.
  * **LED Clobbering on `D1`/`D2`:** Any LED wired to `D1` (`GPIO 27`) or `D2` (`GPIO 28`) would be wiped back to a floating input on `adc_register()` unless `userled_lower_initialize()` ran strictly afterward and `my_reset()` never ran again.
  * **Digital Input Buffer Left Enabled on `D0`:** `rp23xx_common_earlyinitialize()` (`nuttx/boards/arm/rp23xx/common/src/rp23xx_common_initialize.c`) clears the digital input-enable bit (`RP23XX_PADS_BANK0_GPIO_IE`) on `GPIO 26–29` for analog use, but `my_reset()` calls `rp23xx_gpio_set_function()`, which turns `IE` back on. On RP2350 silicon (related to RP2350-E9 pad behavior), leaving `IE=1` on a mid-rail potentiometer input increases leakage and risks noisy readings.
* **Adopted Resolution:**
  * Use 6 purely digital breadboard pins (`D3` / `GPIO 5`, `D4` / `GPIO 6`, `D5` / `GPIO 7`, `D8` / `GPIO 2`, `D9` / `GPIO 4`, `D10` / `GPIO 3`), leaving `D1`/`D2` (`GPIO 27`/`28`) and `D6`/`D7` (`GPIO 0`/`1`, reserved for `UART0` if needed) unassigned.
  * Explicitly clear `RP23XX_PADS_BANK0_GPIO_IE` on `GPIO 26` after `rp23xx_common_bringup()` completes.
  * Disable `CONFIG_DEV_GPIO` and omit `GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1` (`GPIO 28/27/26`) from the custom `board.h`.

### 2. Free-Running Full-Speed ADC Interrupts vs. Periodic Polling
* **Inventive Contradiction:** The bar-graph loop needs fresh potentiometer readings at 20 Hz (50 ms cadence) with low CPU and interrupt load, whereas `rp23xx_adc.c` returns `-ENOTTY` for `ANIOC_TRIGGER`, configures no ADC clock divider, and re-arms conversions in its FIFO ISR at ~500 kS/s for as long as `/dev/adc0` remains open. Meanwhile, the upper-half ring buffer (`nuttx/drivers/analog/adc.c`, `CONFIG_ADC_FIFOSIZE=8`) drops new samples once full rather than overwriting old ones, causing stale reads after any `usleep()`.
* **Driver Data Contract Traps:**
  * `adc_read()` chooses record packing from `buflen`: `buflen % 5 == 0` emits 5-byte `struct adc_msg_s` (`am_channel` + `int32_t am_data`), while sizes divisible by 4 or 2 emit raw values without channel headers.
  * `rp23xx_adc.c` shifts the 12-bit sample left by 19 bits (`value <<= 19`, max `0x7ff80000`, negated on `RP23XX_ADC_FIFO_ERR`), not `0..4095`.
* **Adopted Resolution (Dynamics + Extraction) & Trade-offs:**
  * **Open/Flush/Read/Close per tick:** On each ~50 ms polling tick, the app opens `/dev/adc0`, reads `sizeof(struct adc_msg_s)` (5 bytes), and closes `/dev/adc0`. Closing the last descriptor calls `my_shutdown()`, stopping the 500 kHz ADC FIFO interrupt while the task sleeps.
  * *Secondary trade-offs:* Open/close adds minor `open`/`close` and interrupt attach/detach work per 50 ms tick. Negative FIFO-error readings (`am_data < 0`) are discarded, valid readings are normalized via `am_data >> 19` (`0..4095`), and a small hysteresis/deadband prevents boundary noise from flickering the top LED before calling `ULEDIOC_SETALL` (only on level changes).

### 3. Autonomous Boot (`rcS`) vs. USB-C CDC/ACM Console Lifecycle
* **Inventive Contradiction:** The bar graph must run immediately when powered—even without a USB host—while USB CDC/ACM NSH (`CONFIG_CDCACM_CONSOLE=y`, `CONFIG_NSH_USBCONSOLE=y`, `# CONFIG_DEV_CONSOLE is not set`) has no terminal attached during `rcS`.
* **Codebase Mechanics:**
  * `xiao-rp2350/src/rp23xx_boardinitialize.c` gates `board_late_initialize()` (`rp23xx_bringup()`) behind `#ifdef CONFIG_BOARD_LATE_INITIALIZE`, which `usbnsh/defconfig` omits. Without `CONFIG_BOARD_LATE_INITIALIZE=y`, `/dev/adc0` and `/dev/userleds` do not exist when `rcS` runs.
  * `apps/system/nsh/nsh_main.c` runs `nsh_initialize()` (which executes `/etc/init.d/rcS`) *before* `nsh_consolemain()` connects USB (`BOARDIOC_USBDEV_CONNECT`), redirects stdio to `/dev/null`, and blocks in `nsh_waitusbready()` waiting for 3 consecutive Enter keystrokes (`\r` or `\n`) on `/dev/ttyACM0`.
  * If `CONFIG_NSH_DISABLEBG=y` is enabled (such as via `CONFIG_DEFAULT_SMALL=y`), NSH ignores `&` in `rcS` and blocks USB enumeration indefinitely.
* **Adopted Resolution (Extraction + Blessing in Disguise) & Trade-offs:**
  * Enable `CONFIG_BOARD_LATE_INITIALIZE=y`, `CONFIG_NSH_BUILTIN_APPS=y`, `# CONFIG_NSH_DISABLEBG is not set`, and `# CONFIG_NSH_DISABLESCRIPT is not set`.
  * Structure the app with the `task_create()` singleton-daemon + `SIGTERM` pattern (modeled on `apps/examples/leds/leds_main.c`) so `rcS` starts it cleanly without blocking USB setup and manual NSH invocations cannot launch duplicate instances.
  * Route diagnostics to `syslog()` backed by RAMLOG (`CONFIG_RAMLOG_SYSLOG=y`, viewable via `dmesg` once USB NSH attaches after 3x Enter) and blink an alternating `0x15` / `0x2A` pattern across the 6 LEDs on fatal `/dev/adc0` or `/dev/userleds` errors. *Secondary trade-off:* RAMLOG consumes 2–4 KB of SRAM and boot logs require running `dmesg` after connecting USB.

### 4. Custom Board (`xiao-rp2350`), `userleds`, and Out-of-Tree Build Plumbing
* **Multi-LED `rp23xx_userleds.c`:** Upstream `xiao-rp2350/src/rp23xx_userleds.c` hardcodes a single active-low onboard LED (`GPIO 25`) and ignores `g_ledcfg[]` in `board_userled_all()`. Furthermore, `CONFIG_ARCH_LEDS` defaults to `y` when `ARCH_HAVE_LEDS` is selected (`nuttx/boards/Kconfig`), compiling `rp23xx_autoleds.c` instead of `rp23xx_userleds.c`. The custom board sets `# CONFIG_ARCH_LEDS is not set`, `select ARCH_HAVE_LEDS` in `Kconfig`, enables `CONFIG_USERLED=y` and `CONFIG_USERLED_LOWER=y`, and implements active-high `board_userled_initialize()`, `board_userled()`, and `board_userled_all()` across all 6 breadboard LEDs (`D3–D5`, `D8–D10`).
* **`ARCH_BOARD_CUSTOM` + `ARCH_BOARD_COMMON`:** `nuttx/tools/Config.mk` resolves `BOARD_COMMON_DIR` to `nuttx/boards/arm/rp23xx/common` only when `$(BOARD_DIR)/../common` does not exist; `xiao-rp2350-bargraph/` must not place an unrelated `common/` sibling folder next to its board directory. The custom board must provide `Kconfig`, `scripts/Make.defs` (and linker script references), `include/board.h`, `src/Make.defs`, `src/rp23xx_bringup.c`, `src/rp23xx_userleds.c`, and `src/etc/init.d/{rc.sysinit,rcS}`.
* **ROMFS `rcS` & Make Build Tooling:** `xiao-rp2350/src/CMakeLists.txt` leaves ROMFS `etc/init.d/rcS` unimplemented (`# TODO`), so the project standardizes on **GNU Make** with `CONFIG_ETC_ROMFS=y` (`nuttx/boards/Board.mk`), requiring `genromfs`, `xxd`, and `picotool` (for `nuttx.uf2`) on the host. Because `Board.mk` preprocesses `RCSRCS` through `cpp`, `rcS` uses `#include <nuttx/config.h>` and C `/* ... */` comments (or `RCRAWS`). `CONFIG_RP23XX_ROMFS_ROMDISK_DEVNAME` stays disabled to avoid colliding with `CONFIG_ETC_ROMFS`.
* **`apps/external` Wrapper:** Since `apps/external` is gitignored and absent in `apps/`, a top-level `Makefile` or setup script inside `xiao-rp2350-bargraph/` manages the `apps/external` symlink (containing `Makefile`, `Make.defs`, `Kconfig`), refreshes `apps/Kconfig` when `external/Kconfig` changes, and configures `nuttx` via `tools/configure.sh`.

### 5. Ideal Final Result (IFR)
`xiao-rp2350-bargraph/` encapsulates the 6-LED board definition, `rcS` boot script, custom `defconfig`, bar-graph C daemon, and build wrapper with zero edits inside `nuttx/` or `apps/`. At power-up, the 6 breadboard LEDs track the `D0` potentiometer at 20 Hz with zero idle ADC interrupts and zero flicker; plugging in USB-C and pressing Enter three times opens `nsh>` for `dmesg`, `ps`, `free`, and manual daemon control.

## Goals vs. Non-Goals

* **In-Scope Goals:**
  * House all project artifacts in `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph` outside `nuttx/` and `apps/`.
  * Configure the XIAO RP2350 for 1 potentiometer on `D0` (`GPIO 26` / `ADC0` -> `/dev/adc0`), 6 active-high breadboard LEDs on `D3` (`GPIO 5`), `D4` (`GPIO 6`), `D5` (`GPIO 7`), `D8` (`GPIO 2`), `D9` (`GPIO 4`), `D10` (`GPIO 3`) under `/dev/userleds`, and USB-C CDC/ACM NSH (`usbnsh`).
  * Sample `/dev/adc0` via `open`/`read`/`close` at ~50 ms intervals with `struct adc_msg_s`, negative-error filtering, and deadband smoothing, updating `/dev/userleds` via `ULEDIOC_SETALL`.
  * Auto-start the singleton daemon from `/etc/init.d/rcS` (`CONFIG_ETC_ROMFS=y`, `CONFIG_BOARD_LATE_INITIALIZE=y`), log errors to `CONFIG_RAMLOG_SYSLOG=y` (`dmesg`), and blink `0x15` / `0x2A` across the 6 LEDs on device failure.
  * Reuse `apps/examples/adc` and `apps/examples/leds` patterns (`task_create`, `SIGTERM`, `ULEDIOC_SUPPORTED`, `ULEDIOC_SETALL`).

* **Out-of-Scope Non-Goals:**
  * Porting to other Raspberry Pi Pico / RP2350 boards in this initial exercise.
  * Modifying files inside `nuttx/` or `apps/` (beyond the gitignored `apps/external` symlink and normal build outputs).
  * Supporting CMake builds for `CONFIG_ETC_ROMFS`.
  * Networking, wireless, PWM brightness control, or persistent flash calibration.

## Alternatives Weighed

1. **Modifying `nuttx/boards/arm/rp23xx/xiao-rp2350` in-place vs. Out-of-Tree Custom Board (`CONFIG_ARCH_BOARD_CUSTOM=y`):**
   * *Rejected by author:* Modifying `xiao-rp2350` directly pollutes the `nuttx` submodule instead of keeping exercise code isolated in a new root directory.
2. **8 LEDs (claiming `D1`/`D2` `GPIO 27–28` or `D6`/`D7` `GPIO 0–1`) vs. 6 LEDs (`D3–D5`, `D8–D10`):**
   * *Rejected by author:* 6 LEDs on `D3` (`GPIO 5`), `D4` (`GPIO 6`), `D5` (`GPIO 7`), `D8` (`GPIO 2`), `D9` (`GPIO 4`), and `D10` (`GPIO 3`) are sufficient and completely avoid `rp23xx_adc.c`'s `my_reset()` on `GPIO 27–28` and `UART0` on `GPIO 0–1`.
3. **Keeping `/dev/adc0` continuously open with `ANIOC_RESET_FIFO` vs. `open`/`read`/`close` per polling tick:**
   * *Rejected by author:* Keeping `/dev/adc0` open leaves the ~500 kHz ADC FIFO interrupt running continuously in the background during sleep; `open`/`read`/`close` every ~50 ms stops the ADC interrupt between ticks.
4. **Individual `/dev/gpout*` GPIO nodes vs. `/dev/userleds` (`ULEDIOC_SETALL`):**
   * *Weighed:* Six `/dev/gpout*` descriptors push against `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=6` and require 6 syscalls per update; `/dev/userleds` updates all 6 LEDs in one call over one descriptor.
5. **Hardware `UART0` console (`D6`/`D7`) vs. USB-C CDC/ACM console (`usbnsh`):**
   * *Rejected by author:* Using USB-C avoids an external USB-UART adapter and leaves `D6`/`D7` untouched.
6. **Replacing `CONFIG_INIT_ENTRYPOINT` directly vs. NSH + ROMFS `/etc/init.d/rcS`:**
   * *Weighed:* Pointing `CONFIG_INIT_ENTRYPOINT` directly at the app skips `ETC_ROMFS`, `genromfs`, `xxd`, and `NSH_USBCONSOLE`, but keeping NSH + `rcS` preserves `dmesg`, `ps`, `free`, and interactive learning over USB-C.

## Key Decisions & Staging for Next Steps

All brainstorm open questions are resolved for `/egm-draft`:

1. **Project Location:** `/home/brett/Documents/GitHub/nuttxspace/xiao-rp2350-bargraph` (containing the custom board directory, external app directory, and setup/build wrapper).
2. **Hardware Pin Map (XIAO RP2350):**
   * Potentiometer: `D0` (`GPIO 26` / `ADC0` -> `/dev/adc0`, `IE` cleared after `rp23xx_common_bringup()`).
   * 6 Active-High Bar-Graph LEDs (`/dev/userleds`, `BOARD_NLEDS=6`): `D3` (`GPIO 5`), `D4` (`GPIO 6`), `D5` (`GPIO 7`), `D8` (`GPIO 2`), `D9` (`GPIO 4`), `D10` (`GPIO 3`).
3. **Sampling & Control Loop:** 20 Hz (~50 ms) polling loop using `open("/dev/adc0")` -> `read()` (`sizeof(struct adc_msg_s)`) -> `close()`, negative error filtering, `am_data >> 19` scaling with hysteresis/deadband, and `ULEDIOC_SETALL` updates on `/dev/userleds`.
4. **Boot, Console, & Observability:** GNU Make build (`CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_ARCH_BOARD_COMMON=y`, `CONFIG_BOARD_LATE_INITIALIZE=y`, `CONFIG_ETC_ROMFS=y`, `CONFIG_CDCACM_CONSOLE=y`, `CONFIG_NSH_USBCONSOLE=y`, `# CONFIG_DEV_CONSOLE is not set`, `# CONFIG_RP23XX_UART0 is not set`), `task_create()` singleton daemon launched from `/etc/init.d/rcS`, `CONFIG_RAMLOG_SYSLOG=y` (`dmesg`), and alternating `0x15` / `0x2A` LED blink on fatal device errors.
