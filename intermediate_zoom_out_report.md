# Goldfish Review Report — Stage 8: Intermediate Zoom Out

**Target:** `/home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md` (`Lines 1-460`)

## Reviewer Status

| Reviewer | Status |
| --- | --- |
| `egm-zoom-out` | completed |

## Prioritized Findings (Blockers -> Major -> Nits)

### [S8-1] Symptom-Patching Over-Engineering and Wrong Layering in Critic `S7-1` Mitigations - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Lines 151-158, 175, 220, 399` (cross-referencing `critic_report.md` `[S7-1]`)
* **Discrepancy Description**: Critic `[S7-1]` proposes layering `poll()` with a 20-40 ms timeout before `read()`, elevating `CONFIG_BARGRAPH_PRIORITY` above `nsh`, and adding a rapid-restart guard onto per-tick `/dev/adc0` `open`/`read`/`close` to compensate for `rp23xx_adc.c` free-running behavior. Raising a non-critical 20 Hz LED daemon above the interactive shell (`CONFIG_SYSTEM_NSH_PRIORITY`) couples scheduling priorities across unrelated subsystems, and `adc_poll()` in `nuttx/drivers/analog/adc.c` (`L597-671`) acquires `ad_spinlock` twice per tick and requires `CONFIG_ADC_NPOLLWAITERS` solely for a hypothetical post-`open()` IRQ stall.
* **Rationale & Recommendation**: Keep `open`/`read`/`close` hygiene (validating read size and closing `adc_fd` on every error/`EINTR` path) and calibrate NFR line 220 to state what `rp23xx_adc.c` actually guarantees, without adding `poll()` or priority elevation.

### [S8-2] Altitude Drop and Config Duplication Amplified by `S7-4`, `S7-7`, `S6-2`, and `S6-6` - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Lines 254-438` (cross-referencing `[S5-4]`, `[S6-2]`, `[S6-6]`, `[S7-4]`, `[S7-7]`)
* **Discrepancy Description**: `Detailed Implementation Plan` (`L254-438`) already drops into code-level weeds (exact `#include` lists, helper function breakdowns, local variable names, and ~40 unchanged `usbnsh/defconfig` symbols). Critic `[S7-4]` proposes adding a `"Required Kconfig & Build Wiring"` subsection to `FeatureRange` repeating ~15 mostly default NuttX symbols (`CONFIG_ETC_ROMFSMOUNTPT="/etc"`, `!CONFIG_NSH_DISABLE_{DMESG,PS,FREE,KILL}`, `!CONFIG_EXAMPLES_ADC`) and `Make.defs` variables (`DEPPATH`, `VPATH`, `MENUDESC`), while `[S7-7]`, `[S6-2]`, and `[S6-6]` ask for loop-level and variable-initialization detail inside `FeatureRange`.
* **Rationale & Recommendation**: Keep `FeatureRange` focused on component contracts and non-default configuration deltas relative to `usbnsh/defconfig`, and summarize module responsibilities in the plan rather than pre-writing C internals or repeating default Kconfig symbols.

### [S8-3] Guard Against Reinventing `rp23xx_adc.c` and YAGNI Averaging in Critic `S7-5` - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Lines 186-202, 227` (cross-referencing `critic_report.md` `[S7-5]`)
* **Discrepancy Description**: Critic `[S7-5]` recommends adding a board-local single-shot ADC driver in `board/src/` and software multi-sample averaging to `Alternatives Considered`. Writing an out-of-tree RP2350 ADC lower-half driver duplicates `rp23xx_adc.c` (`L612-660`) and `rp23xx_common_bringup.c` (`L509-547`) solely to eliminate a sub-millisecond conversion burst once every 50 ms, and software multi-sample averaging is YAGNI when the `±24`-count hysteresis band already absorbs potentiometer jitter.
* **Rationale & Recommendation**: Evaluate the board-local single-shot ADC driver and multi-sample averaging briefly as *rejected* alternatives only; do not adopt either into the architecture.

### [S8-4] Unresolved Kconfig Over-Parameterization and Rebuild-Based CUJ 3 Test Hook - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Lines 393-400, 456` (cross-referencing `zoom_out_report.md` `[S5-5]`)
* **Discrepancy Description**: `external/bargraph/Kconfig` defines 7 sub-options for fixed breadboard hardware, and `Verification Plan` step 4 (`L456`) requires editing `.config`, rebuilding, and reflashing twice to test CUJ 3.
* **Rationale & Recommendation**: Reuse the optional CLI device path pattern from `apps/examples/adc/adc_main.c` (`L179-182`, e.g., `bargraph /dev/adc_missing`) to test CUJ 3 directly from `nsh>` without rebuilding, and trim speculative Kconfig knobs.

### [S8-5] Unresolved Phase 1 Checkpoint Dependency Inversion and Unnecessary Boilerplate - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Lines 80, 90, 273-279, 306, 312, 366-372, 381` (cross-referencing `[S6-1]`, `[S5-6]`)
* **Discrepancy Description**: Two structural issues remain unresolved:
  1. `Phase 1 Checkpoint` (`L312`) invokes `configure` and asserts `CONFIG_BARGRAPH=y` before Phase 2 (`board/src/Make.defs`) and Phase 3 (`external/bargraph/Kconfig`) exist, creating a dangling `apps/external` symlink and contradicting `Phase 2 Checkpoint` (`L381`).
  2. Empty `board/Kconfig` (`Config.mk` `L176-180` falls back to `boards/dummy/dummy_kconfig`), empty `rc.sysinit`, and unused `CONFIG_LIBM=y` remain in the layout and `defconfig`.
* **Rationale & Recommendation**: Reorder or decouple Phase 1-3 checkpoints so each phase is buildable/configurable, and drop `board/Kconfig`, `rc.sysinit`, and `CONFIG_LIBM=y`.

### [S8-6] Lifecycle Synchronization Creep (`S5-7` + `S7-2`) and Unused 4-Byte `adc_read()` Path (`S5-1`) - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Lines 151-154, 175-176, 181-182, 221, 417-422, 434-437`
* **Discrepancy Description**:
  1. Layering `S7-2`'s `sched_lock()` + `CONFIG_STACK_COLORATION` onto `S5-7`'s `STOPPED`/`RUNNING`/`STOPPING` tri-state FSM + exit semaphore/waitpid over-synchronizes a minimal daemon modeled on `apps/examples/leds/leds_main.c` (`L230-254`).
  2. For single-channel `ADC0` reads, `nuttx/drivers/analog/adc.c` (`L245-248, L306-330`) natively copies raw `int32_t` samples when `buflen == 4`, avoiding 5-byte `struct adc_msg_s` unpacking.
* **Rationale & Recommendation**: Keep singleton protection to a 2-line `sched_lock()` check-and-set (avoiding a custom FSM, extra semaphores, and `CONFIG_STACK_COLORATION` unless stack headroom or restart races require them), and consider using 4-byte `adc_read()` reads.
