# Select the hero codebase and hardware traps to feature on stage

Type: grilling
Label: wayfinder:grilling
Status: resolved
Blocked by: none

## Question

Across the 10-point hardware/RTOS scorecard in `EXPERIMENT_RUNBOOK.md` and `docs/reports/vanilla-vs-egm-comparison.md` (plus the 23 Pass 1/Pass 2 Goldfish findings in `docs/egm/reviews/04-review-review.md` and the strict post-audit findings), which 2–3 "hero traps" should we explain to a general software engineering audience so they immediately grasp why legacy codebase comprehension matters without drowning in datasheet minutiae?

Candidate stories include:
1. **The 500 kHz `START_ONCE` ADC IRQ storm & false fix**: Why Vanilla's 40 Hz `open/read/close` exposed an unguarded `remove_device()` ISR teardown race, why EGM Phase 2's initial `RP23XX_ADC_DIV` idea was proved a no-op by Pass 1 Goldfish (`B-1`), and how Pass 2 Goldfish caught the 5-byte `adc_read()` short-read trap (`CA-01`).
2. **The `my_reset()` pin clobber & RP2350-E9 silicon erratum (`IE=1` pad leakage)**: How `rp23xx_adc.c:my_reset()` silently resets `GPIO 26–29` and re-enables the digital input buffer (`IE=1`) on `GPIO 26`, leaking mid-rail analog current unless explicitly cleared after `rp23xx_common_bringup()`.
3. **The silent Kconfig drop & 4x Q4 EMA gain bug**: How missing `CONFIG_ANALOG=y` silently drops `CONFIG_ADC=y` in NuttX (`B-3`), and how Pass 1 caught `ema_q4 >> 2` saturating all 6 LEDs at 25% pot travel (`B-2`).
4. **The "nobody is immune" honest retro**: How the strict Goldfish audit caught that both Vanilla and EGM mis-sized the physical XIAO RP2350 breadboard straddle (`0.7"` and `0.3"` vs. `0.6"`) and caught Phase 5's `bargraph test` `MANUAL` overwrite (which Phase 6 `/teach-code` fixed).

## Answer

### 1. Primary "Through-Line" Technical Spine (Acts II & III)

The **`500 kHz` `START_ONCE` ADC interrupt storm and its chain of traps** acts as the central technical spine threading Act II (`03:00–08:00`) and Act III (`08:00–19:00`), giving general software engineers three instantly recognisable failure modes without datasheet bloat:

1. **Act II (Vanilla's workaround-turned-timebomb)**: Upstream `rp23xx_adc.c` triggers single-shot `START_ONCE` conversions at `500 kHz` (`2 µs`/sample) whenever `/dev/adc0` remains open. To dodge the interrupt flood, Vanilla opens `/dev/adc0`, reads one sample, and calls `close(fd)` at `40 Hz`—walking straight into an unguarded `remove_device()` ISR teardown race (`g_first_device = NULL` before `g_active_count -= 1`).
2. **Act III.b (EGM's honest false start)**: Even with `README.md` contracts loaded during `02-brainstorm` and `03-draft`, the main session initially assumes setting `RP23XX_ADC_DIV` will throttle ADC conversions down to `500 Hz`.
3. **Act III.c (Two-pass Goldfish rescue)**:
   - **Pass 1 (`B-1`)**: Fresh-context Goldfish proves from `rp23xx_adc.c` and its contract that `RP23XX_ADC_DIV` only governs free-running `START_MANY` mode and is a total no-op under `START_ONCE`, swapping it for demand-driven NVIC IRQ gating (`up_disable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` around a 5-sample burst).
   - **Pass 2 (`CA-01`, after `/clear`)**: Fresh-context Goldfish catches that upper-half `adc_read()` wakes and returns early as soon as a single `5`-byte `struct adc_msg_s` arrives, requiring a `read()` loop until all `25` bytes (`5` samples) arrive so `msgs[1..4]` aren't uninitialised stack garbage.

### 2. Three Supporting Contrast Traps (Acts II & III.c)

Cap supporting traps in Acts II and III at three rapid-fire examples across build config, silicon/driver init, and pure application maths, leaving remaining scorecard rows as summary table entries:

1. **Build-system contract (`~30s`, Act II & III.c)**: **Silent `CONFIG_ADC=y` Kconfig drop without `CONFIG_ANALOG=y`** — Vanilla hits this at Step 180 when `olddefconfig` silently discards `CONFIG_ADC=y`, whereas Pass 1 Goldfish (`B-3`) catches missing `CONFIG_ANALOG=y` and hallucinated `CONFIG_PWM_MULTICHAN=y` pre-build.
2. **Silicon + driver-init contract (`~60s`, Act II & III.b/c)**: **`rp23xx_adc.c:my_reset()` pin clobber (`GPIO 26–29`) & Erratum `RP2350-E9` (`IE=1` pad leakage)** — Even with only Channel 0 enabled, `my_reset()` loops across all four pins (`GPIO 26–29`) during kernel bringup and calls `rp23xx_gpio_set_function()`, unconditionally setting `IE=1` (digital input buffer enabled). Under silicon erratum `RP2350-E9`, leaving `IE=1` on `GPIO 26` leaks current into mid-rail analogue readings unless `rp23xx_bringup.c` explicitly clears `IE | PUE | PDE | ISO` and sets `OD=1` *after* `rp23xx_common_bringup()` (while opening `/dev/pwm5` and `/dev/pwm6` in `bargraphd` after bringup safely re-muxes `D1`/`D2` on `GPIO 27–28`).
3. **Pure logic/math verification (`~45s`, Act III.c)**: **Pass 1 `B-2` — `4x` Q4 fixed-point EMA right-shift bug** — Shifting `ema_q4 >> 2` yielded `[0, 16380]` (`4x` gain), saturating all 6 LEDs at `25%` potentiometer travel; fixed via first-sample seeding (`ema_q4 = raw_avg << 4`) and `(ema_q4 + 8u) >> 4`.

### 3. Four "Nobody Is Immune" Self-Correction Traps (Acts IV & V)

1. **Act IV (`/teach-code` runnable C99 labs catching Phase 5 shortcuts)**:
   - **`bargraph test` `MANUAL`-clobber bug**: Phase 5's `bargraph_main.c` called `bargraph_start_daemon()` *before* saving `prev_mode = g_bargraph.mode`, resetting an active `MANUAL` override (`bargraph set 50`) back to `SENSING` before `bargraph test` recorded it; caught and fixed in Lesson 0006 (`docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/`).
   - **`Makefile` `printf` unit-test duplication**: Unlike Vanilla's clean shared `src/bargraph_logic.c`, EGM Phase 5's top-level `Makefile` used `printf` to synthesise `/tmp/bargraph_test.c` with duplicated math bodies; `/teach-code` enforced true single-source `-std=c99 -Wall -Wextra -Werror -pedantic` compilation and `scripts/sync_listing.py`.
2. **Act V (Honest Retro & Strict Goldfish Post-Audit)**:
   - **Shared physical `0.6"` XIAO breadboard straddle blind spot**: Without a physical breadboard pitch contract, *both* workflows botched the `0.6"` straddle—Vanilla spanned `c1..h1` (`0.7"`, one row too wide) and EGM squeezed it onto columns `E` & `F` (`0.3"`, half-width, concealed by widening the visual ravine to `120px` in `breadboard-wiring.svg`).
   - **Strict post-audit teardown edge case (`10 / 10` Primary vs `6 Pass / 4 Partial / 0 Fail` Strict)**: On `bargraph stop`, EGM called `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` one line *before* `close(adc_fd)`, briefly re-exposing the `remove_device()` race window on shutdown.
