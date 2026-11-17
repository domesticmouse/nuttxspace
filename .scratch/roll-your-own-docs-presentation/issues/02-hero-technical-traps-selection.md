# Select the hero codebase and hardware traps to feature on stage

Type: grilling
Label: wayfinder:grilling
Status: open
Blocked by: none

## Question

Across the 10-point hardware/RTOS scorecard in `EXPERIMENT_RUNBOOK.md` and `docs/reports/vanilla-vs-egm-comparison.md` (plus the 23 Pass 1/Pass 2 Goldfish findings in `docs/egm/reviews/04-review-review.md` and the strict post-audit findings), which 2–3 "hero traps" should we explain to a general software engineering audience so they immediately grasp why legacy codebase comprehension matters without drowning in datasheet minutiae?

Candidate stories include:
1. **The 500 kHz `START_ONCE` ADC IRQ storm & false fix**: Why Vanilla's 40 Hz `open/read/close` exposed an unguarded `remove_device()` ISR teardown race, why EGM Phase 2's initial `RP23XX_ADC_DIV` idea was proved a no-op by Pass 1 Goldfish (`B-1`), and how Pass 2 Goldfish caught the 5-byte `adc_read()` short-read trap (`CA-01`).
2. **The `my_reset()` pin clobber & RP2350-E9 silicon erratum (`IE=1` pad leakage)**: How `rp23xx_adc.c:my_reset()` silently resets `GPIO 26–29` and re-enables the digital input buffer (`IE=1`) on `GPIO 26`, leaking mid-rail analog current unless explicitly cleared after `rp23xx_common_bringup()`.
3. **The silent Kconfig drop & 4x Q4 EMA gain bug**: How missing `CONFIG_ANALOG=y` silently drops `CONFIG_ADC=y` in NuttX (`B-3`), and how Pass 1 caught `ema_q4 >> 2` saturating all 6 LEDs at 25% pot travel (`B-2`).
4. **The "nobody is immune" honest retro**: How the strict Goldfish audit caught that both Vanilla and EGM mis-sized the physical XIAO RP2350 breadboard straddle (`0.7"` and `0.3"` vs. `0.6"`) and caught Phase 5's `bargraph test` `MANUAL` overwrite (which Phase 6 `/teach-code` fixed).
