# Goldfish Review Report

**Target:** `/Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md` (`Lines 1-381`)

## Reviewer Status

| Reviewer | Status |
| --- | --- |
| `egm-design` | completed |
| `egm-structural` | completed |
| `egm-style` | completed |

**Severity Summary:** 0 Blockers, 1 Major, 2 Nits

---

## Prioritized Findings (Blockers -> Major -> Nits)

### 1. Handle Partial/Short Reads from `adc_read()` During 5-Sample Burst Acquisition - Severity: Major
* **Reviewer Lens:** `egm-design`
* **Affected Scope:** `Lines 243-252` (`### 4.1 ADC Subsystem Mitigations (GPIO26 / ADC0)`) and `Lines 668-674` (`#### 28. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c`)
* **Discrepancy Description**: Section 4.1 states that on each 50 Hz burst, `bargraphd` unmasks `RP23XX_ADC_IRQ_FIFO`, reads `5` `struct adc_msg_s` samples (`25` bytes), and immediately calls `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`. However, in `nuttx/drivers/analog/adc.c` (`adc_read()`, lines 270–408), `read()` blocks on `nxsem_wait(&fifo->af_sem)` only while the upper-half software FIFO is empty (`fifo->af_head == fifo->af_tail`). As soon as the first pending ADC FIFO interrupt fires and `adc_receive()` enqueues 1 sample and posts `af_sem`, `adc_read()` wakes up, copies only the currently buffered sample(s) (`1` `struct adc_msg_s` = `5` bytes if the 2nd conversion is still in flight), and returns `nread = 5` rather than waiting for all `25` bytes. Calling `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` after a single `read()` call would therefore mask the IRQ before `msgs[1..4]` are acquired.
* **Rationale & Recommendation**: Specify in Section 4.1 (lines 245, 249) and Section 28 (line 672) that `bargraphd` loops on `read()` (accumulating `sizeof(struct adc_msg_s)` = `5`-byte increments) until all `5` `struct adc_msg_s` entries (`25` bytes total) have been collected before calling `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`.

### 2. Clarify `prev_mode` Preservation and Concurrent Command Transitions During `BARGRAPH_MODE_TEST` - Severity: Nit
* **Reviewer Lens:** `egm-design`
* **Affected Scope:** `Lines 289-323` (`### 5. Daemon State Machine & IPC Synchronization`)
* **Discrepancy Description**: The `stateDiagram-v2` (lines 289–303) defines transitions out of `TEST` for sweep completion (`TEST --> SENSING`, `TEST --> MANUAL`) and `bargraph stop` (`TEST --> STOPPED`), while the CLI table (lines 313–323) states that `bargraph test` restores `prev_mode` (`SENSING` or `MANUAL`). It does not explicitly state what happens if `bargraph set <0..100>`, `bargraph resume`, or a second `bargraph test` is invoked (e.g., if `bargraph test &` was backgrounded in NSH) while `mode == BARGRAPH_MODE_TEST`.
* **Rationale & Recommendation**: Note in Section 5 that `prev_mode` is captured only when transitioning into `TEST` from `SENSING` or `MANUAL` (never overwriting `prev_mode` with `BARGRAPH_MODE_TEST`), and that issuing `bargraph set <0..100>` or `bargraph resume` during an active `TEST` sweep aborts the sweep (`test_requested = false`, broadcasting `done_cond`) and transitions directly to `MANUAL` or `SENSING`.

### 3. Split Overly Dense Bullet Sentences in Goals and Section 4.2 - Severity: Nit
* **Reviewer Lens:** `egm-style`
* **Affected Scope:** `Lines 36, 269-273` (`## Goals` and `### 4.2 Hardware PWM Subsystem Mitigations`)
* **Discrepancy Description**: Line 36 (`Complete Breadboard & EGM Documentation`) is a single 115-word sentence with nested em-dashes and parentheticals, and lines 269–273 combine upper-half contract rules, `memcpy()` details, lower-half register bitfields, and slice channel assignments into a single multi-clause sentence.
* **Rationale & Recommendation**: Break line 36 and lines 269–273 into two shorter sentences each so the hardware/driver constraints and EGM contract requirements can be scanned quickly.
