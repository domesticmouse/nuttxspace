# Goldfish Review Report: Intermediate Zoom Out (Stage 8)

**Target:** `/home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md` (`1..459`, full file)
**Standard:** `/home/brett/.gemini/config/plugins/egm/resources/document_standards/zoom-out-standard.md`
**Prior Stage Reports Reviewed:** `zoom_out_report.md`, `comprehension_report.md`, `critic_report.md`

## Reviewer Status

| Reviewer | Status |
| :--- | :--- |
| `egm-zoom-out` | completed |

## Prioritized Findings (Blockers -> Majors -> Nits)

### 1. Multi-Layer Workarounds for `rp23xx_adc.c` vs. Minimal Single-Shot Board Lower-Half - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `The Problem` (lines 17–22), `Goals` (lines 27, 29), `Detailed Subsystem Mechanisms` (lines 164–177), `Alternatives Considered` (`Alternative G`, lines 219–222), `Risks & Limits` (lines 234–236), `Detailed Implementation Plan` -> Item 8 (`rp23xx_bringup.c`, lines 361–371)
* **Discrepancy Description**: Keeping `CONFIG_RP23XX_ADC=y` forces three stacked workarounds: (a) clearing `RP23XX_PADS_BANK0_GPIO_IE` on `GPIO 26–29` after `my_reset()` undoes `rp23xx_common_earlyinitialize()`; (b) avoiding `D1`/`D2` (`GPIO 27–28`); and (c) running 20 Hz `open()`/`read()`/`close()` cycles to halt free-running ~500 kHz conversions—which `critic_report.md` showed introduces a fatal `remove_device()` / ISR race in `rp23xx_adc.c`.
* **Rationale & Recommendation**: Disabling `CONFIG_RP23XX_ADC` while reusing NuttX's upper-half `nuttx/drivers/analog/adc.c` (`CONFIG_ADC=y`, `adc_register()`, `adc_poll()`) with a minimal single-channel (`ADC0`) single-shot `struct adc_ops_s` lower-half in `board/src/` (`RP23XX_ADC_CS_START_ONCE` via `ANIOC_TRIGGER`) is simpler than the current workaround stack. It removes the post-bringup `IE` clear loop, eliminates 20 Hz `open()`/`close()` churn and the `remove_device()` race, stops 500 kHz interrupt bursts, and preserves the `/dev/adc0` VFS boundary. (If the no-submodule-edit constraint is ever relaxed, fixing `rp23xx_adc.c` upstream remains the cleaner root fix.)

### 2. Premature C-Level Implementation Weeds Across Architecture and Plan Sections - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Technical Plan (Architecture)` -> `Detailed Subsystem Mechanisms` (lines 162–186) and `Detailed Implementation Plan` (lines 300–434)
* **Discrepancy Description**: Both `Detailed Subsystem Mechanisms` and `Detailed Implementation Plan` drop into C-level specifics (exact register macros, framing arithmetic, bit-shift formulas, static variable names, `sigaction` flags, all 38 retained `defconfig` lines verbatim, exact `#include` lists, and line-by-line C statements). Repeating internal function and variable names across sections also caused the cross-section drift flagged in `comprehension_report.md` (`quantize_while` vs. `quantize_level`, `g_bargraph_running` exit cleanup, `syslog` on `task_create` failure).
* **Rationale & Recommendation**: Keep `Detailed Subsystem Mechanisms` at component responsibilities, interfaces, and data flow; summarize `defconfig` deltas and file responsibilities without transcribing C statements.

### 3. Reusing `adc_poll()` for Bounded Stall Detection (Mitigation Alignment Check) - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Critical User Journeys` (`CUJ 3`, lines 59–63), `Risks & Limits` (line 236), `Detailed Implementation Plan` -> Item 14 (`bargraph_main.c`, lines 425, 428–431)
* **Discrepancy Description**: Evaluating the mitigation proposed in `zoom_out_report.md` and `critic_report.md`—using `poll()` (`POLLIN`) with a bounded timeout (and/or `O_NONBLOCK`) before `read()`—reuses `adc_poll()` already implemented in `nuttx/drivers/analog/adc.c` (lines 597–671) rather than inventing a watchdog thread or custom timer.
* **Rationale & Recommendation**: Confirm `CONFIG_ADC_NPOLLWAITERS >= 1` in `defconfig` if needed, and keep the sampling path unified around `poll()` + `read()` without speculative retry frameworks.

### 4. Redundant Dual Singleton State (`g_bargraph_running` + `g_bargraph_pid`) - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Subsystem Mechanisms` (lines 182–183) and `Detailed Implementation Plan` -> Item 14 (lines 422–423, 432)
* **Discrepancy Description**: Tracking both `g_bargraph_running` and `g_bargraph_pid` duplicates liveness state when `g_bargraph_pid > 0 && kill(g_bargraph_pid, 0) == 0` is already authoritative.
* **Rationale & Recommendation**: Keep only `g_bargraph_pid` (`-1` when inactive) and apply PID-checked cleanup (`if (g_bargraph_pid == getpid()) g_bargraph_pid = -1;` under `sched_lock()`/`sched_unlock()`) on all daemon exit paths.

### 5. Speculative Kconfig Options Coupled to Hardcoded Constants - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Implementation Plan` -> Item 12 (`external/bargraph/Kconfig`, lines 407, 426, 430–431)
* **Discrepancy Description**: `BARGRAPH_INTERVAL_US` (`50000`) and `BARGRAPH_HYSTERESIS` (`24`, `range 0 584`) are never overridden in `defconfig`, are hardcoded-coupled to `{585, 1170, ...}` thresholds and `consecutive_errors == 5` (250 ms), and add YAGNI config surface.
* **Rationale & Recommendation**: Follow `apps/examples/leds/Kconfig` (`PROGNAME`, `PRIORITY`, `STACKSIZE` only) and keep the 50 ms interval and 24-count hysteresis as local constants in `bargraph_main.c` (while retaining the `bargraph [adc_devpath]` positional CLI override for CUJ 3 testing, once its inaccurate comparison to `adc_main.c`'s `-p` flag is fixed).

### 6. Unnecessary Internal Header `board/src/rp23xx_pico.h` and Misattributed `CONFIG_DEV_GPIO` Risk - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Subsystem Mechanisms` (line 166) and `Detailed Implementation Plan` -> Items 2 & 6 (lines 303, 306, 344–350)
* **Discrepancy Description**: `board/src/rp23xx_pico.h` holds single-consumer constants (`GPIO_LED1..6` used only in `rp23xx_userleds.c`; `GPIO_ADC_*` used only in `rp23xx_bringup.c` and eliminated under a single-shot ADC lower-half) and one `rp23xx_bringup()` prototype. Line 166 also conflates `ao_reset()` with disabling `CONFIG_DEV_GPIO`, while `board/src/Make.defs` does not compile `rp23xx_gpio.c`.
* **Rationale & Recommendation**: Declare `rp23xx_bringup()` in `board/include/board.h`, keep `g_ledcfg` pins local to `rp23xx_userleds.c`, drop `rp23xx_pico.h`, and clean up the `ao_reset()` / `CONFIG_DEV_GPIO` explanation.

### 7. Phased-Rollout Conditional in Permanent `Makefile` (and `etctmp.c` Cleanup Tradeoff) - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Implementation Plan` -> Item 1 (`xiao-rp2350-bargraph/Makefile`, lines 294, 296–298)
* **Discrepancy Description**: Guarding `setup` in `Makefile` with *"Only when `external/Makefile` exists"* is transitional scaffolding for Phases 1–2 before Phase 3 files exist. (Deleting `etctmp.c` after `all` is an acceptable `nuttx/`-unmodified workaround if kept, though note it triggers ROMFS regeneration on every `make`.)
* **Rationale & Recommendation**: Keep the final `Makefile` unconditional once Phase 3 exists.
