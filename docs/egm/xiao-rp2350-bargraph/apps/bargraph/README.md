# `xiao-rp2350-bargraph/apps/bargraph` 6-Segment PWM LED Bar Graph Application (`apps/bargraph`)

## Overview

Built-in user-space application (`bargraph`) and autonomous background daemon (`bargraphd`) for the Seeed Studio XIAO RP2350 6-segment hardware PWM LED bar graph. Samples the potentiometer on `/dev/adc0` (`D0/A0`, `GPIO26`) at 50 Hz using demand-driven NVIC `RP23XX_ADC_IRQ_FIFO` gating, Q4 exponential moving average filtering, and endpoint deadband clamping, and drives six LEDs (`D1`–`D6`) proportionally across five hardware PWM slices (`/dev/pwm5`, `/dev/pwm6`, `/dev/pwm2`, `/dev/pwm3`, `/dev/pwm0`). Also exposes the interactive `bargraph` NSH CLI command (`start`, `stop`, `status`, `test`, `set <0..100>`, `resume`).

## Interface

- `[Kconfig:BARGRAPH]` Enables the `bargraph` built-in application (`[Kconfig:BARGRAPH_PROGNAME]`, `[Kconfig:BARGRAPH_PRIORITY]`, `[Kconfig:BARGRAPH_STACKSIZE]`).
- `[Make.defs:CONFIGURED_APPS]` Appends `$(APPDIR)/external/bargraph` to `CONFIGURED_APPS` when `CONFIG_BARGRAPH` is enabled.
- `[Makefile:PROGNAME]` Registers the `bargraph` built-in command (`MAINSRC = bargraph_main.c`) via `$(APPDIR)/Application.mk`.
- `[bargraph_internal.h:struct bargraph_state_s]` Shared daemon and CLI synchronization and telemetry state structure (`lock`, `wake_cond`, `done_cond`, `mode`, `prev_mode`, `running`, `stop_requested`, `test_requested`, `init_err`, `manual_pct`, `raw_adc`, `filtered_adc`, `millivolts`, `permille`, `duty[6]`, `daemon_pid`).
- `[bargraph_main.c:bargraph_autostart]` Spawns the `bargraphd` background daemon during `rp23xx_bringup()` before `nsh_main` waits for a USB host connection.
- `[bargraph_main.c:bargraph_compute_duties]` Pure 6-segment piecewise linear transfer function mapping `total_units` (`0..393210`) to `uint16_t` duties (`0..65535`).
- `[bargraph_main.c:bargraph_filter_adc]` First-sample-seeded Q4 EMA filter ($\alpha = 1/4$, `filtered = (ema_q4 + 8) >> 4`) with endpoint deadband clamping (`<= 32` -> `0`, `>= 4063` -> `4095`).
- `[bargraph_main.c:main]` Built-in NSH CLI entrypoint (`bargraph_main`) supporting `start`, `stop`, `status`, `test`, `set <0..100>`, and `resume`.

## Invariants

- `[bargraph_internal.h:BARGRAPH_DUTY_MAX]` Maximum segment duty is `65535` (`0xFFFF`), preventing `uint16_t` truncation of `b16ONE` (`0x10000` -> `0`) in `rp23xx_pwm.c`.
- `[bargraph_main.c:bargraph_sample_adc]` `RP23XX_ADC_IRQ_FIFO` remains masked via `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` whenever `bargraphd` sleeps, is unmasked only after `ANIOC_RESET_FIFO` to read 5 `struct adc_msg_s` entries (`25` bytes, discarding `msgs[0]` and averaging valid `msgs[1..4]` shifted right by `BARGRAPH_ADC_SHIFT` = `19`), and is always re-masked before `bargraph_sample_adc()` returns.
- `[bargraph_main.c:bargraph_apply_pwm]` Every `/dev/pwm*` `struct pwm_info_s` populates both `channels[0].channel = 1` (hardware channel A) and `channels[1].channel = 2` (hardware channel B) at `1000 Hz`.
- `[bargraph_main.c:bargraph_shutdown_pwm]` Stopping `bargraphd` writes `duty = 0` to all 6 channels, issues `PWMIOC_STOP`, and calls `close()` on all 5 PWM descriptors so `pwm_shutdown()` muxes `D1`–`D6` back to `RP23XX_GPIO_FUNC_SIO` driven low (`0`), and calls `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` before `close(adc_fd)`.

## Side Effects

- `[bargraph_main.c:bargraphd_daemon]` Opens `/dev/adc0` and `/dev/pwm0,2,3,5,6`, gates `RP23XX_ADC_IRQ_FIFO` in the NVIC, and updates hardware PWM compare registers at 50 Hz (`20 ms` period).
- `[bargraph_main.c:main]` Synchronizes with `bargraphd` via `g_bargraph.lock`, `wake_cond`, and `done_cond`, printing telemetry snapshots and command status to `stdout`/`stderr`.

## Verification

- Run host unit tests: `make -C xiao-rp2350-bargraph test`
- Run firmware build: `make -C xiao-rp2350-bargraph`
