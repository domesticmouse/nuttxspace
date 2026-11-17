# Curriculum Roadmap & Teaching Notes (`NOTES.md`)

## Learner Context & Preferences
- **Goal**: A complete, self-contained story arc tutorial in `./tutorial/` teaching how to build and verify both the physical 400-tie-point breadboard circuit and the out-of-tree Apache NuttX firmware (`xiao-rp2350-bargraph`).
- **Pedagogical Style**: Block-by-block code explanations followed by complete runnable listings (`### Complete src/main.c Listing`) synchronized with `apps/<NNNN-slug>/src/main.c` via `sync_listing.py`. Clean C99 code without boilerplate license headers inside tutorial snippets and companion apps.
- **Verification Standard**: Every companion app under `./tutorial/apps/<NNNN-slug>/` compiles cleanly on macOS/Linux with `cc -std=c99 -Wall -Wextra -Werror -pedantic` and executes a deterministic assertion suite (`make -C apps/<NNNN-slug> test`). The final lesson also cross-verifies the target NuttX firmware build (`make -C ../xiao-rp2350-bargraph test && make -C ../xiao-rp2350-bargraph`).

## 6-Lesson Story Arc Curriculum

1. **Lesson 0001 — Breadboard Circuit & Electrical Current Budget (`0001-breadboard-circuit-and-current-budget.md`)**
   - Physical circuit topology on a 400-tie-point solderless breadboard (`Rows 1–21`, `Cols A–J`).
   - 3.3 V safety (`Row 7F` `3V3` vs `Row 5F` `5V`), `10 kΩ` potentiometer voltage divider and Thevenin source impedance (`R_S ≤ 2.5 kΩ`), and `330 Ω` active-high LED current budget (`3.94 mA` per segment, `23.6 mA` total).
   - Companion app (`apps/0001-breadboard-circuit-and-current-budget/`): hardware pin/slice mapping table, integer microampere current budget calculator, and 12-bit ADC-to-millivolt/permille conversion math.
   - Interactive simulator: `{{widget:breadboard-simulator}}`.

2. **Lesson 0002 — Proportional 6-Segment Bar Graph Math & 16-Bit PWM Scaling (`0002-proportional-bargraph-math-and-pwm-scaling.md`)**
   - Why standard NuttX `b16ONE` (`65536`) wraps to `0` (`0%` duty) when stored in `rp23xx_pwm_lowerhalf_s`'s `uint16_t duty` field.
   - Designing `bargraph_compute_duties()` across `BARGRAPH_TOTAL_UNITS_MAX = 6 * 65535 = 393210` so lower segments saturate at `65535`, the active segment interpolates linearly across `0..65535`, and upper segments remain `0`.
   - Companion app (`apps/0002-proportional-bargraph-math-and-pwm-scaling/`): `bargraph_compute_duties()` and `bargraph_units_from_adc()` with exhaustive boundary and monotonicity tests.
   - Interactive simulator: `{{widget:pwm-segment-visualizer}}`.

3. **Lesson 0003 — Fixed-Point Q4 EMA Filtering, Deadbands & ADC Sample Extraction (`0003-q4-ema-filtering-and-adc-extraction.md`)**
   - Unpacking `rp23xx_adc.c`'s 19-bit left shift (`(am_data >> 19) & 0x0fff`) and why `msgs[0]` in each 5-sample burst holds a stale conversion from the end of the previous burst.
   - Multiplication-free Q4 fixed-point Exponential Moving Average (`alpha = 1/4`, `ema_q4 = ema_q4 - (ema_q4 >> 2) + (raw << 2)`, rounded via `(ema_q4 + 8) >> 4`) and endpoint deadbands (`<= 32 -> 0`, `>= 4063 -> 4095`).
   - Companion app (`apps/0003-q4-ema-filtering-and-adc-extraction/`): burst extractor/averager and `bargraph_filter_adc()` with step-response and deadband tests.
   - Interactive simulator: `{{widget:adc-ema-simulator}}`.

4. **Lesson 0004 — Multi-Channel RP2350 PWM Routing & Safe Hardware Shutdown (`0004-multichannel-pwm-routing-and-safe-shutdown.md`)**
   - Reconciling the upper-half `drivers/timers/pwm.c` contract (`channel == 0` terminates scanning) with `rp23xx_pwm.c` (`channels[0]` = Ch A, `channels[1]` = Ch B) across 5 PWM slices (`/dev/pwm0`, `/dev/pwm2`, `/dev/pwm3`, `/dev/pwm5`, `/dev/pwm6`), including shared Slice 3 (`D4/PWM3A` + `D5/PWM3B`).
   - Preventing frozen GPIO pads on `PWMIOC_STOP` by writing zero duties (`0, 0`) before `PWMIOC_STOP` and `close()`.
   - Companion app (`apps/0004-multichannel-pwm-routing-and-safe-shutdown/`): `bargraph_update_slice()`, `bargraph_apply_pwm()`, and `bargraph_shutdown_pwm()` verified against a mock POSIX `ioctl` driver layer.

5. **Lesson 0005 — Out-of-Tree NuttX Board Bringup, RP2350-E9 Pad Isolation & NVIC IRQ Gating (`0005-out-of-tree-bsp-and-adc-irq-gating.md`)**
   - Out-of-tree board (`CONFIG_ARCH_BOARD_CUSTOM=y`) and `apps/external` symlink architecture that leaves `nuttx/` and `apps/` untouched.
   - Mitigating Erratum RP2350-E9 digital pad leakage on `GPIO26` (`modreg32` setting `OD=1` and clearing `IE`, `PUE`, `PDE`, `ISO` in `RP23XX_PADS_BANK0_GPIO(26)`).
   - Defeating the 500 kIRQ/s `START_ONCE` self-retriggering ADC interrupt storm via demand-driven NVIC gating (`up_disable_irq` while idle, `ANIOC_RESET_FIFO` + `up_enable_irq` during a 5-sample burst read).
   - Companion app (`apps/0005-out-of-tree-bsp-and-adc-irq-gating/`): `rp23xx_bringup()` register isolation and `bargraph_sample_adc()` demand-gated reader with simulated FIFO/IRQ verification.

6. **Lesson 0006 — The 50 Hz `bargraphd` Daemon State Machine & Thread-Safe NSH CLI (`0006-bargraphd-daemon-and-nsh-cli.md`)**
   - Designing `struct bargraph_state_s` with `pthread_mutex_t lock`, `wake_cond`, and `done_cond` for lock-minimal 50 Hz (`20 ms`) operation across `SENSING`, `MANUAL`, `TEST` (100-step 2-second triangle sweep `0% -> 100% -> 0%`), and `STOPPED` modes.
   - Implementing the `bargraph` CLI (`start`, `stop`, `status`, `test`, `set <0..100>`, `resume`) and verifying both the multi-threaded host companion app and the full target UF2 firmware build (`nuttx.uf2`).
   - Companion app (`apps/0006-bargraphd-daemon-and-nsh-cli/`): complete runnable daemon + CLI harness with end-to-end state-machine and concurrency tests.
