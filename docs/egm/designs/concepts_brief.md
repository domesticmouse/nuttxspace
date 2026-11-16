# Out-of-Tree Seeed Studio XIAO RP2350 6-Segment PWM LED Bar Graph (`xiao-rp2350-bargraph/`)

* **Author:** Brett Morgan
* **Date:** 2026-11-16

## Overview

This project provides an out-of-tree Apache NuttX custom board configuration and user-space application in `xiao-rp2350-bargraph/` for a Seeed Studio XIAO RP2350 mounted on a solderless breadboard with one linear potentiometer and six discrete LEDs arranged as a bar graph. Both the `nuttx/` and `apps/` git submodules remain unmodified.

At power-on, the board initializes the RP2350 ADC and hardware PWM slices and spawns a background daemon (`bargraphd`) before any USB-C host serial connection is established. The daemon samples the potentiometer on pin `D0/A0` (`GPIO26`, `ADC0` across `0V` to `3.3V`) and drives six active-high LEDs on the left header (`D1` through `D6`: `GPIO27`, `GPIO28`, `GPIO5`, `GPIO6`, `GPIO7`, `GPIO0`) using hardware PWM (`PWM5B`, `PWM6A`, `PWM2B`, `PWM3A`, `PWM3B`, `PWM0A`). Lower segments below the current reading run at 100% duty cycle, the active segment modulates its brightness proportionally to the fractional position within that segment, and upper segments run at 0% duty cycle. When a host computer connects over USB-C CDC/ACM (`/dev/ttyACM0`), NuttShell (`nsh`) provides an interactive console where the `bargraph` command can inspect telemetry or control the daemon (`start`, `stop`, `status`, `test`, `set <0..100>`, and `resume`).

## Analysis

### 1. Hardware & Driver Traps Identified in `nuttx/arch/arm/src/rp23xx`

Review of the upstream RP23xx architecture and common board drivers surfaced seven concrete hardware and driver behaviors that the out-of-tree design must handle:

1. **Missing `CONFIG_BOARD_LATE_INITIALIZE=y` in Upstream `xiao-rp2350:usbnsh`**:
   The in-tree `xiao-rp2350/configs/usbnsh/defconfig` omits `CONFIG_BOARD_LATE_INITIALIZE=y`. Without it, `board_late_initialize()` in `rp23xx_boardinitialize.c` is compiled out and `rp23xx_bringup()` is never called, leaving `/dev/adc0` and `/dev/pwm*` unregistered. The custom board `defconfig` in `xiao-rp2350-bargraph/board/configs/bargraph/defconfig` must enable `CONFIG_BOARD_LATE_INITIALIZE=y`.
2. **Hardcoded `pwm_info_s` Channel Indexing in `rp23xx_pwm.c`**:
   In `nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c`, `setup_pulse()` ignores `info->channels[i].channel` and maps `info->channels[0].duty` directly to hardware channel A (`CC[15:0]`) and `info->channels[1].duty` to hardware channel B (`CC[31:16]`), while the upper-half driver (`nuttx/drivers/timers/pwm.c`) stops scanning `channels[]` if it sees `channel == 0`. Therefore, for every multi-channel PWM slice (`/dev/pwm0`, `/dev/pwm2`, `/dev/pwm3`, `/dev/pwm5`, `/dev/pwm6`), user space must populate both `channels[0].channel = 1` (pin A) and `channels[1].channel = 2` (pin B), placing A-channel LEDs (`D2/PWM6A`, `D4/PWM3A`, `D6/PWM0A`) in `channels[0].duty` and B-channel LEDs (`D1/PWM5B`, `D3/PWM2B`, `D5/PWM3B`) in `channels[1].duty`. Because `D4` (`GPIO6`, `PWM3A`) and `D5` (`GPIO7`, `PWM3B`) share slice 3 (`/dev/pwm3`), the daemon updates `channels[0].duty` and `channels[1].duty` together in a single `PWMIOC_SETCHARACTERISTICS` + `PWMIOC_START` sequence on `/dev/pwm3`.
3. **Dual Kconfig Guards for Multi-Channel PWM**:
   `rp23xx_common_bringup.c`, `rp23xx_pwmdev.c`, and `rp23xx_pwm.c` gate two-channel initialization on `#if defined(CONFIG_PWM_NCHANNELS) && CONFIG_PWM_NCHANNELS == 2`, whereas `arch/arm/src/rp23xx/Kconfig` defines `CONFIG_RP23XX_PWM_MULTICHAN` and `CONFIG_RP23XX_PWM_NCHANNELS`. Both sets of symbols (`CONFIG_PWM_MULTICHAN=y`, `CONFIG_PWM_NCHANNELS=2`, `CONFIG_RP23XX_PWM_MULTICHAN=y`, `CONFIG_RP23XX_PWM_NCHANNELS=2`) must be enabled together, and unused sibling pins on single-LED slices must be set to `-1` (`CONFIG_RP23XX_PWM0B_GPIO=-1`, `CONFIG_RP23XX_PWM2A_GPIO=-1`, `CONFIG_RP23XX_PWM5A_GPIO=-1`, `CONFIG_RP23XX_PWM6B_GPIO=-1`).
4. **16-Bit Duty Truncation in `rp23xx_pwm.c` (`b16ONE` Wraparound)**:
   `struct rp23xx_pwm_lowerhalf_s` stores `duty[i]` as `uint16_t` rather than `ub16_t` (`uint32_t`), and computes hardware counter compare levels as `((uint64_t)priv->duty[i] * (priv->top + 1)) / 65535`. Passing `b16ONE` (`0x00010000` = `65536`) truncates to `0x0000` (`0%` duty), turning an LED completely off when it reaches 100%. The bargraph daemon must clamp and scale duty cycles strictly to `0 .. 65535` (`0x0000 .. 0xFFFF`).
5. **`PWMIOC_STOP` Freezes Output Level Unless Duty Is Zeroed First**:
   `pwm_stop()` in `rp23xx_pwm.c` only clears `RP23XX_PWM_CSR_EN`, leaving the GPIO pad frozen at whatever high or low state the PWM counter held at that instant. On `bargraph stop`, the daemon must write `duty = 0` via `PWMIOC_SETCHARACTERISTICS` + `PWMIOC_START` before calling `PWMIOC_STOP` and closing the PWM descriptors.
6. **ADC Pin-Mux Reset Conflict on `GPIO27` (`D1`) and `GPIO28` (`D2`) + RP2350-E9 Pad Leakage on `GPIO26` (`D0/A0`)**:
   In `rp23xx_adc.c`, `my_reset()` resets the GPIO function of every enabled ADC channel to `RP23XX_GPIO_FUNC_NULL` when `/dev/adc0` is registered, and `rp23xx_gpio_set_function()` leaves the digital input buffer (`IE=1`) enabled on `GPIO26`. The custom board configuration must:
   - Enable only `CONFIG_RP23XX_ADC_CHANNEL0=y` (keeping `CHANNEL1` on `GPIO27` and `CHANNEL2` on `GPIO28` disabled so ADC initialization never disturbs `D1` or `D2` PWM pin muxing).
   - Clear `IE`, `PUE`, `PDE`, and `ISO` and set `OD` (Output Disable) in `RP23XX_PADS_BANK0_GPIO(26)` during `rp23xx_bringup()` so digital pad leakage (Erratum RP2350-E9) does not distort potentiometer readings mid-scale.
7. **ADC Left-Shift (`<< 19`), Continuous 500 kS/s Interrupt Storm, and FIFO Staleness**:
   `rp23xx_adc.c` runs the ADC in free-running mode (`ADC_CS_START_MANY`) and shifts each 12-bit reading left by 19 bits (`value <<= 19`) in the ISR until the upper-half software FIFO (`CONFIG_ADC_FIFOSIZE`) fills up. At the default 48 MHz ADC clock (`96` cycles/sample), the ADC fires 500,000 interrupts per second whenever the FIFO has space, and once full, `read()` returns stale samples from the head of the FIFO.
   - **Resolution**: In `xiao-rp2350-bargraph/board/src/rp23xx_bringup.c`, set `RP23XX_ADC_DIV` to `0xFFFF00` (`INT = 65535`), pacing hardware conversions to `~732 Hz` (cutting ADC interrupt rate by 99.85% without modifying `nuttx/`). In `bargraphd`, keep `/dev/adc0` open for the life of the daemon (avoiding the unguarded `remove_device()` list removal in `rp23xx_adc.c` on repeated `close()`), issue `ioctl(adc_fd, ANIOC_RESET_FIFO, 0)` before reading a 4-sample burst on each 20 ms tick, verify `msg.am_data >= 0`, and extract the 12-bit sample via `(uint16_t)((msg.am_data >> 19) & 0x0fff)`.

### 2. Autonomous Boot Startup vs. USB-C NSH Console Coexistence

In `apps/nshlib/nsh_usbconsole.c`, `nsh_consolemain()` blocks inside `nsh_waitusbready()` until a USB host opens `/dev/ttyACM0` and sends three carriage-return or newline characters. However, kernel `board_late_initialize()` -> `rp23xx_bringup()` runs before `nsh_main` is spawned. By starting `bargraphd` directly at the end of `rp23xx_bringup()` (or via an application autostart helper invoked from `rp23xx_bringup()`), the LED bar graph begins tracking the potentiometer within milliseconds of power-on—even when powered from a standalone 5V USB wall adapter with no host computer attached—and without depending on host `genromfs`/`xxd` tools. When a user later connects over USB-C, `nsh` attaches to `/dev/ttyACM0` and any `bargraph <subcommand>` runs in the user's terminal session, communicating with `bargraphd` through a mutex- and condition-variable-protected shared state structure in NuttX flat memory (`CONFIG_BUILD_FLAT=y`).

### 3. Out-of-Tree Build Integration Without Submodule Changes

To keep `git status` and `git diff` clean inside both `nuttx/` and `apps/`:
- The custom board lives in `xiao-rp2350-bargraph/board/` and sets `CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y`, and `CONFIG_ARCH_BOARD_COMMON=y` so NuttX reuses `nuttx/boards/arm/rp23xx/common` for common peripheral bringup while compiling board-specific code from `xiao-rp2350-bargraph/board/src/`.
- The custom application lives in `xiao-rp2350-bargraph/apps/bargraph/`. Because `apps/.gitignore` explicitly ignores `/external` and `/Kconfig`, `xiao-rp2350-bargraph/board/scripts/Make.defs` ensures a symlink `apps/external -> ../xiao-rp2350-bargraph/apps` exists whenever the board is configured or built, allowing `apps/Makefile` and `apps/tools/mkkconfig.sh` to discover and register `bargraph` as a built-in NSH command without modifying any tracked file in `apps/`.

### 4. Physical Breadboard Layout & Electrical Budget

- **Module Placement**: Straddle the Seeed Studio XIAO RP2350 across the center ravine of a 400-tie-point breadboard (Rows 5–11, Columns E and F) with the USB-C connector facing the top edge (Row 5) for strain-free cable access.
- **Power Rails**: Connect Pin 12 (`3V3`, Row 7F) to the top/left red `+` rail (`3.3V`) and Pin 13 (`GND`, Row 6F) to the top/right blue `-` rail (`GND`). Leave Pin 14 (`5V`) unconnected.
- **Potentiometer (`10 kΩ` linear `B10K`, Rows 1–3)**: Outer lugs to `3.3V` and `GND` rails (~0.33 mA continuous draw); center wiper to `D0/A0` (`GPIO26`, Pin 1 at Row 5A), with an optional `100 nF` ceramic capacitor from Row 5B to `GND` to reduce wiper contact noise.
- **6 Discrete Active-High LEDs (Rows 14–19)**: Jumper `D1` (`GPIO27`, Row 6A), `D2` (`GPIO28`, Row 7A), `D3` (`GPIO5`, Row 8A), `D4` (`GPIO6`, Row 9A), `D5` (`GPIO7`, Row 10A), and `D6` (`GPIO0`, Row 11A) to six adjacent rows (Rows 14–19), each through a `330 Ω` series current-limiting resistor to the LED anode (`+`, long lead) and LED cathode (`-`, short lead/flat edge) to the `GND` rail (~3.9 mA per LED at 100% duty, ~23.6 mA total with all 6 lit).

## Goals vs. Non-Goals

* **In-Scope Goals:**
  * Out-of-tree custom board support package in `xiao-rp2350-bargraph/board/` and out-of-tree built-in application in `xiao-rp2350-bargraph/apps/bargraph/`, leaving `nuttx/` and `apps/` unmodified.
  * Autonomous background startup of `bargraphd` at power-on before USB-C host connection.
  * 50 Hz potentiometer sampling on `D0/A0` (`GPIO26`, `ADC0`) with burst averaging, exponential moving average filtering, endpoint deadband clamping (`0%` and `100%`), and RP2350-E9 analog pad configuration.
  * 6-segment hardware PWM LED bar graph on `D1`–`D6` (`PWM5B`, `PWM6A`, `PWM2B`, `PWM3A`, `PWM3B`, `PWM0A`) with 100% duty on lower segments, proportional PWM duty (`0..65535`) on the active segment, and 0% duty on upper segments.
  * Interactive USB-C CDC/ACM NSH console supporting `bargraph start`, `bargraph stop`, `bargraph status`, `bargraph test`, `bargraph set <0..100>`, and `bargraph resume`.
  * Complete breadboard wiring diagram, bill of materials, and EGM `README.md` contracts for every directory in `xiao-rp2350-bargraph/`.
* **Out-of-Scope Non-Goals:**
  * Modifying any tracked source, header, Kconfig, or build files inside `nuttx/` or `apps/`.
  * Hardware UART0 console (`D6`/`D7` are used as `GPIO0`/`GPIO1`, with `GPIO0` driving `LED6` via `PWM0A`).
  * Runtime GPIO pin remapping, persistent flash storage of override states, or continuous terminal streaming modes.

## Alternatives Weighed

* **Discrete On/Off GPIO Bar Graph (`/dev/userleds` or `/dev/gpioN`)**: Rejected by author in favor of proportional hardware PWM brightness on the active bar segment.
* **Foreground `INIT_ENTRYPOINT="bargraph_main"`**: Rejected by author because running the bar graph in the foreground or replacing `nsh_main` as the init entrypoint would either block the interactive USB-C NSH console or require duplicating NSH session management inside the application.
* **Launching the Daemon Only from `/etc/init.d/rcS` After USB Console Ready**: Rejected during analysis because `nsh_usbconsole.c` blocks waiting for a USB-C host connection and three Enter keystrokes in `nsh_consolemain()`, and ROMFS generation in `Board.mk` adds host dependencies on `genromfs` and `xxd`. Spawning `bargraphd` during `board_late_initialize()` (`rp23xx_bringup()`) starts the bar graph immediately at power-on with zero external host tool dependencies.
* **Single `bargraph start` vs. Separate `bargraph resume` Command to Exit Manual Override**: Resolved by author to support both `bargraph start` and an explicit `bargraph resume` subcommand to transition from `set <0..100>` (`MANUAL` mode) back to live potentiometer `SENSING` mode.

## Key Decisions & Staging for Next Steps

1. **Pin & PWM Channel Assignment**:
   - Potentiometer wiper: `D0/A0` (`GPIO26`, `/dev/adc0` channel 0).
   - Segment 1 (`0.0–16.7%`): `D1` (`GPIO27`, `/dev/pwm5` channel B -> `info.channels[1]`).
   - Segment 2 (`16.7–33.3%`): `D2` (`GPIO28`, `/dev/pwm6` channel A -> `info.channels[0]`).
   - Segment 3 (`33.3–50.0%`): `D3` (`GPIO5`, `/dev/pwm2` channel B -> `info.channels[1]`).
   - Segment 4 (`50.0–66.7%`): `D4` (`GPIO6`, `/dev/pwm3` channel A -> `info.channels[0]`).
   - Segment 5 (`66.7–83.3%`): `D5` (`GPIO7`, `/dev/pwm3` channel B -> `info.channels[1]`).
   - Segment 6 (`83.3–100.0%`): `D6` (`GPIO0`, `/dev/pwm0` channel A -> `info.channels[0]`).
2. **Driver Mitigations in Out-of-Tree BSP & App**:
   - Set `CONFIG_BOARD_LATE_INITIALIZE=y`, `CONFIG_PWM_MULTICHAN=y`, `CONFIG_PWM_NCHANNELS=2`, `CONFIG_RP23XX_PWM_MULTICHAN=y`, `CONFIG_RP23XX_PWM_NCHANNELS=2`, and `CONFIG_RP23XX_ADC_CHANNEL0=y` (with `ADC_CHANNEL1..3` disabled).
   - Configure `RP23XX_PADS_BANK0_GPIO(26)` (`IE=0`, `PUE=0`, `PDE=0`, `ISO=0`, `OD=1`) and `RP23XX_ADC_DIV = 0xFFFF00` (`~732 Hz` hardware conversion pacing) in `xiao-rp2350-bargraph/board/src/rp23xx_bringup.c`.
   - Use `0 .. 65535` (`uint16_t`) PWM duty scaling, flush stale ADC FIFO entries via `ANIOC_RESET_FIFO` before reading, and shift `am_data >> 19` to recover 12-bit ADC counts.
3. **Daemon State Machine & CLI**:
   - `bargraphd` starts automatically in `SENSING` mode from `rp23xx_bringup()` and coordinates with NSH CLI invocations (`start`, `stop`, `status`, `test`, `set <0..100>`, `resume`) over shared memory guarded by `pthread_mutex_t` and `pthread_cond_t`.
