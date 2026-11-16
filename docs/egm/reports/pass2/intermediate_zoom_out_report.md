# Stage 8: Intermediate Zoom Out Review Report

**Target:** [`docs/designs/xiao-rp2350-bargraph.md`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md) (`Lines 1-727`)
**Prior Reports Reviewed:**
* [`zoom_out_report.md`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/zoom_out_report.md) (Stage 5 Initial Zoom Out)
* [`comprehension_report.md`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/comprehension_report.md) (Stage 6 Comprehension)
* [`critic_report.md`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/critic_report.md) (Stage 7 Critic)

---

## 1. Executive Summary (Zoom Out & Simplicity Audit)

`docs/designs/xiao-rp2350-bargraph.md` remains at its 727-line pre-consolidation baseline awaiting Stage 9 synthesis. Evaluating the full document alongside the Stage 5, Stage 6, and Stage 7 reports against the Zoom-Out Standard (`resources/document_standards/zoom-out-standard.md` criteria: context sufficiency, root-problem focus, anti-weeds, code reuse, and YAGNI enforcement) yields the following conclusions:

1. **No New Architectural Bloat Introduced**: None of the mitigations or clarifications proposed in Stage 6 (`comprehension_report.md`) or Stage 7 (`critic_report.md`) require new files, new Kconfig options, new state machine states, or new struct fields in `struct bargraph_state_s`.
2. **Stage 7's `adc_read()` Loop Fix Is Essential**: Verified against [`nuttx/drivers/analog/adc.c`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/drivers/analog/adc.c#L270-L408), `adc_read()` blocks on `nxsem_wait(&fifo->af_sem)` only while `fifo->af_head == fifo->af_tail` and returns immediately after copying whatever samples are currently queued (`1` `struct adc_msg_s` = `5` bytes on the first wakeup). Looping on `read()` until all `5` samples (`25` bytes) are collected before calling `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` is a mandatory correctness fix, not gold-plating.
3. **Carry-Forward Simplifications from Stage 5**: The 5 findings from [`zoom_out_report.md`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/zoom_out_report.md) remain open in the current draft and can be cleanly combined with the Stage 6 and Stage 7 edits during Stage 9 consolidation.

---

## 2. Zoom Out Audit of Stage 6 & Stage 7 Proposed Additions/Mitigations

### Item A: Partial/Short `read()` Handling in 5-Sample ADC Burst (from `critic_report.md` #1)
* **Severity**: Major (Correctness requirement; low complexity impact)
* **Affected Scope**: Section `4.1 ADC Subsystem Mitigations` ([xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L243-L252)) and `Detailed Implementation Plan` Target File 28 ([xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L668-L674)).
* **Simplicity Rationale & Guardrail**:
  * In [`nuttx/drivers/analog/adc.c`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/drivers/analog/adc.c#L270-L408), `adc_read()` wakes as soon as the first FIFO IRQ calls `adc_receive()` and posts `af_sem`, returning `5` bytes (`1` `struct adc_msg_s`) if the remaining conversions are still in flight.
  * **Guardrail**: Document this concisely at the behavioral level ("looping on `read()` in `sizeof(struct adc_msg_s)` (`5`-byte) increments until all `5` samples (`25` bytes) are collected before calling `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`") without adding multi-line C loop pseudocode to Target File 28.

### Item B: `prev_mode` Preservation & Concurrent Command Preemption During `TEST` Mode (from `critic_report.md` #2)
* **Severity**: Nit (State-machine edge case; zero structural additions)
* **Affected Scope**: Section `5. Daemon State Machine & IPC Synchronization` ([xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L289-L323)).
* **Simplicity Rationale & Guardrail**:
  * If a user runs `bargraph test &` in the background and then issues `bargraph test`, `bargraph set <0..100>`, or `bargraph resume`, overwriting `prev_mode` when `mode == BARGRAPH_MODE_TEST` would trap the daemon in `TEST` mode upon sweep completion.
  * **Guardrail**: Solve this using the existing fields of `struct bargraph_state_s`—do **not** add new flags or states. Simply specify that:
    1. `prev_mode` is saved only when `mode != BARGRAPH_MODE_TEST`.
    2. `bargraph set <0..100>` and `bargraph resume` clear `test_requested = false`, set `mode` to `MANUAL` or `SENSING`, signal `wake_cond`, and broadcast `done_cond` so any waiting `bargraph test` CLI invocation wakes and exits cleanly.

### Item C: `bargraph_autostart()` and `bargraph start` Deduplication (from `comprehension_report.md` #1 & `zoom_out_report.md` #1)
* **Severity**: Nit (Opportunity to reduce code duplication)
* **Affected Scope**: Section `3. Boot Sequence` ([xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L207-L228)) and Target File 28 ([xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L642-L692)).
* **Simplicity Rationale & Guardrail**:
  * `comprehension_report.md` #1 notes that the `Proposed Changes` for `bargraph_main.c` do not state how `bargraph_autostart()` spawns `bargraphd` vs. `bargraph start`.
  * **Guardrail**: Rather than specifying two separate startup flows, specify a single internal helper (`bargraph_start_daemon()`) called by both `bargraph_autostart()` and `bargraph start` that spawns `bargraphd` via `task_create("bargraphd", CONFIG_BARGRAPH_PRIORITY, CONFIG_BARGRAPH_STACKSIZE, bargraphd_daemon, NULL)` under `g_bargraph.lock` and waits on `done_cond` until `running` is true or `init_err < 0`.

### Item D: Host-Side Math Unit Verification Without New Tracked Test Files (from `comprehension_report.md` #2)
* **Severity**: Nit (YAGNI guardrail on verification)
* **Affected Scope**: Section `Verification Plan` ([xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L716-L717)).
* **Simplicity Rationale & Guardrail**:
  * `comprehension_report.md` #2 asks how a reader executes the host-side pure math verification step.
  * **Guardrail**: Do **not** add a new tracked unit-test source file or host build target inside `xiao-rp2350-bargraph/`. Specify in `Verification Plan` that the pure helper functions (`bargraph_compute_duties()` and `bargraph_filter_adc()`) are verified via an ephemeral host `cc` invocation targeting `/tmp` (removed immediately after execution) so the repository tree remains strictly focused on the target firmware.

---

## 3. Open Stage 5 Zoom Out Findings Carried Forward to Stage 9

For completeness, the following findings from [`zoom_out_report.md`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/zoom_out_report.md) remain open in lines 1..727 of [`docs/designs/xiao-rp2350-bargraph.md`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md):

1. **Kernel/User-Space Boundary Documentation (`up_enable_irq` / `up_disable_irq` and `bargraph_autostart()`) — Severity: Major**
   * **Scope**: Lines 209–229, 242–247, 511–533, 668–674.
   * **Action for Stage 9**: Because `nuttx/` and `apps/` cannot be modified (Non-Goal #1) and `rp23xx_adc.c` triggers `START_ONCE` inside `interrupt_handler()` without an ioctl to pause conversions, either move the NVIC IRQ gating / ADC burst helper into `xiao-rp2350-bargraph/board/src/` OR explicitly document in Section 4.1.3 and `xiao-rp2350-bargraph/README.md` (`## Invariants`) that calling `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` from `bargraphd` and calling `bargraph_autostart()` from `rp23xx_bringup()` are deliberate `CONFIG_BUILD_FLAT=y` out-of-tree mitigations required to keep `nuttx/` unmodified (referencing [`README.md`](file:///Users/brett/Documents/GitHub/nuttxspace-egm/README.md#L16)).
2. **Single-Translation-Unit Header Split (`bargraph_internal.h` + `bargraph_main.c`) — Severity: Nit**
   * **Scope**: Lines 119–120, 305, 590–701.
   * **Action for Stage 9**: Consider folding `bargraph_internal.h` into `bargraph_main.c` (declaring `bargraph_autostart(void)` in `board.h` or `rp23xx_pico.h`), or if `bargraph_internal.h` is retained so the host-side math verification step (`comprehension_report.md` #2) can cleanly test the pure math/filter declarations, state that rationale explicitly in Target File 27.
3. **Duplicate GPIO/Device Macros in `board/include/board.h` vs. `defconfig` — Severity: Nit**
   * **Scope**: Lines 455–471 (Target Files 9 & 10).
   * **Action for Stage 9**: Avoid duplicating pin numbers in `board.h` (`BARGRAPH_LED1_GPIO`..`BARGRAPH_LED6_GPIO`) that are already governed by `CONFIG_RP23XX_PWM*` in `defconfig` and are unused by `bargraph_main.c`. Keep only `BARGRAPH_ADC_GPIO 26` (used by `rp23xx_bringup.c` for `RP23XX_PADS_BANK0_GPIO(26)`) alongside the standard clock and LED definitions in `board.h`.
4. **Upper-Half `channels[i].channel` Prose Clarification & Sentence Splitting — Severity: Nit**
   * **Scope**: Lines 21, 36, 268–274 (combines `zoom_out_report.md` #4 and `critic_report.md` #3).
   * **Action for Stage 9**: Split the dense sentences at line 36 and lines 269–273, and clarify that populating `channels[0].channel = 1` and `channels[1].channel = 2` satisfies the `nuttx/include/nuttx/timers/pwm.h` contract and `pwm_dump()` logging while `drivers/timers/pwm.c` copies `struct pwm_info_s` verbatim via `memcpy()`.
5. **Trim C-Expression Weeds in Target File 28 (`bargraph_main.c`) — Severity: Nit**
   * **Scope**: Lines 646–690 (Target File 28).
   * **Action for Stage 9**: Reference the mathematical transfer function and Q4 EMA equations already defined in Sections 4.1.4, 4.1.5, and 4.2.3 rather than repeating raw C shift/rounding expressions inline in Target File 28.
