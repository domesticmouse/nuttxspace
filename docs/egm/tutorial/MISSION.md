# Mission: Out-of-Tree Seeed Studio XIAO RP2350 6-Segment PWM LED Bar Graph on Apache NuttX

## Why
Build, wire, and verify a self-contained 6-segment proportional PWM LED bar graph and 50 Hz potentiometer telemetry daemon (`bargraphd`) on the Seeed Studio XIAO RP2350 running Apache NuttX, keeping the upstream `nuttx/` and `apps/` submodules 100% unmodified while conquering seven real-world silicon errata and RTOS driver traps.

## Success looks like
- Assemble the 400-tie-point solderless breadboard circuit (Seeed Studio XIAO RP2350, `10 kΩ` linear potentiometer on `D0/A0`, and six active-high LEDs on `D1`–`D6` with `330 Ω` current-limiting resistors) within a verified `23.6 mA` total LED current budget.
- Implement and unit-test the 16-bit-safe (`0..65535`) 6-segment proportional duty-cycle interpolator (`bargraph_compute_duties`) that prevents `b16ONE` (`65536 -> 0`) `uint16_t` wraparound in `rp23xx_pwm.c`.
- Implement and verify the burst-averaged ADC sample extractor (`>> 19` shift recovery, stale first-sample discard) paired with a multiplication-free Q4 fixed-point Exponential Moving Average filter (`bargraph_filter_adc`) and endpoint deadbands (`<= 32`, `>= 4063`).
- Route six logical LEDs across five multi-channel RP2350 hardware PWM slices (`/dev/pwm0`, `/dev/pwm2`, `/dev/pwm3`, `/dev/pwm5`, `/dev/pwm6`) and enforce zero-before-stop shutdown ordering so GPIO pads never freeze high on `PWMIOC_STOP`.
- Configure an out-of-tree NuttX custom board (`CONFIG_ARCH_BOARD_CUSTOM=y`) and `apps/external` symlink hook that isolate `GPIO26` against Erratum RP2350-E9 digital pad leakage, gate `RP23XX_ADC_IRQ_FIFO` in the NVIC to eliminate the 500 kIRQ/s `START_ONCE` interrupt storm, and autostart `bargraphd` before USB CDC/ACM console wait blocking.
- Ship the complete thread-safe `bargraphd` 50 Hz state machine (`SENSING`, `MANUAL`, `TEST`, `STOPPED`) and interactive `bargraph` NSH CLI utility (`start`, `stop`, `status`, `test`, `set <0..100>`, `resume`) with 100% passing host and firmware verification gates.

## Constraints
- **Zero Submodule Modifications**: `git -C nuttx status --porcelain` and `git -C apps status --porcelain` must remain completely empty across configure, build, test, and `distclean`.
- **Strict C99 & Host-Testable Verification**: Every lesson's companion application in `tutorial/apps/<NNNN-slug>/` must compile cleanly on the host with `cc -std=c99 -Wall -Wextra -Werror -pedantic` and pass automated assertions before integrating with the target firmware.
- **3.3 V Electrical Safety**: All RP2350 GPIO and ADC pads operate strictly at `3.3 V` (`3V3` pin at `Row 7F`), leaving `5V` (`Row 5F`) unconnected and keeping per-pin LED current at `~3.94 mA` (below the 12 mA pad limit).

## Out of scope
- Modifying any tracked source file, header, Kconfig, or Makefile inside `nuttx/` or `apps/`.
- Hardware UART0 console wiring on `D6`/`D7` (`GPIO0` is dedicated to `LED6` via `PWM0A`; interactive console uses USB-C CDC/ACM `/dev/ttyACM0`).
- Persisting manual override settings to flash or remapping GPIO pins dynamically at runtime.
