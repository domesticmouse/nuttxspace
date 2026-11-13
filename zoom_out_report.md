# Goldfish Review Report: Initial Zoom Out (Stage 5)

**Target:** `/home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md` (`1..459`, full file)
**Standard:** `/home/brett/.gemini/config/plugins/egm/resources/document_standards/zoom-out-standard.md`

## Reviewer Status

| Reviewer | Status |
| :--- | :--- |
| `egm-zoom-out` | completed |
| `egm-inventive-catalyst` | completed |

## Prioritized Findings (Blockers -> Majors -> Nits)

### 1. Unmodified-Submodule Constraint Forces Multi-Layer Workarounds and `remove_device()` ISR Race - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`, `egm-inventive-catalyst`
* **Affected Scope:** `The Problem` (lines 17–22), `Non-Goals` (line 37), `Detailed Subsystem Mechanisms` (lines 164–177), `Alternatives Considered` (`Alternative G`, lines 219–222), `Risks & Limits` (lines 234–236), and `Makefile` (lines 296–297)
* **Discrepancy Description**: To leave `nuttx/` unmodified, the design treats `CONFIG_RP23XX_ADC=y` (`rp23xx_adc.c`) as an all-or-nothing SoC dependency and stacks local workarounds across four layers: (1) duplicating a custom board tree in `board/`; (2) clearing `RP23XX_PADS_BANK0_GPIO_IE` in `rp23xx_bringup.c` after `my_reset()` sets `IE=1` and resets `GPIO 26–29` (sacrificing `D1`/`D2`); (3) opening and closing `/dev/adc0` at 20 Hz to stop free-running ~500 kHz conversions because `my_ioctl()` rejects `ANIOC_TRIGGER`; and (4) deleting `etctmp.c` in `Makefile`. Furthermore, per-tick `close()` invokes `my_shutdown()` -> `remove_device()` under `nxmutex_lock(&dev->ad_lock)` without a critical section (`adc.c` line 207), creating a race window in `rp23xx_adc.c` (lines 395–407) between `g_first_device = NULL` and `g_active_count -= 1` where an active FIFO IRQ enters `get_next_channel()` and triggers `ASSERT(next != g_current_channel)`.
* **Rationale & Recommendation**:
  * **Option A (Out-of-tree Segmentation + Inversion)**: Keep `CONFIG_ADC=y` (`drivers/analog/adc.c`) and `rp23xx_common_bringup()`, disable `CONFIG_RP23XX_ADC`, and register a minimal single-channel (`ADC0`) `struct adc_ops_s` lower-half in `board/src/` via `adc_register("/dev/adc0", ...)` using `RP23XX_ADC_CS_START_ONCE` triggered by `ANIOC_TRIGGER`. `bargraph_daemon` then keeps `/dev/adc0` open and issues `ioctl(adc_fd, ANIOC_TRIGGER, 0)` + `read()` once per 50 ms tick, eliminating `GPIO 26–29` clobbering, the 500 kHz conversion burst, per-tick `open`/`close`, and the `remove_device()` ISR race without modifying `nuttx/`. *Secondary liabilities*: ~60–90 lines of board-local RP2350 ADC register code coupled to `hardware/rp23xx_adc.h`, scoped to single-channel `ADC0`.
  * **Option B (Root Structural Fix)**: Fix `my_reset()` and add `ANIOC_TRIGGER` / sample-rate control directly in `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` if the no-submodule-patch constraint is relaxed.

### 2. Blocking `/dev/adc0` `read()` Prevents CUJ 3 Detection on Clock or IRQ Stall - Severity: Major
* **Reviewer Lens:** `egm-inventive-catalyst`
* **Affected Scope:** `Critical User Journeys` (`CUJ 3`, lines 59–63), `Detailed Subsystem Mechanisms` (lines 174–178), and `Risks & Limits` (line 236)
* **Discrepancy Description**: `read_adc_sample()` opens `/dev/adc0` in blocking mode (`O_RDONLY`) and calls `read()`, accepting in `Risks & Limits` that if the ADC hardware clock or FIFO IRQ stalls after `open()` succeeds, `read()` blocks inside `nxsem_wait(&fifo->af_sem)` rather than incrementing `consecutive_errors`. This leaves a stalled ADC permanently silent instead of entering CUJ 3's fault blink loop.
* **Rationale & Recommendation**: Apply **Beforehand Cushioning** by opening `/dev/adc0` with `O_RDONLY | O_NONBLOCK` or waiting via `poll()` (`POLLIN`) with a bounded timeout (for example, 25 ms) before `read()`, using `adc_poll()` already implemented in `nuttx/drivers/analog/adc.c` (lines 597–671). Map `poll()` timeout (`0` ready descriptors) to `-ETIMEDOUT`/`-EAGAIN` so `consecutive_errors` reaches 5 within ~250 ms. *Secondary liabilities*: adds one `poll()` syscall and `struct pollfd` stack variable per tick, relies on `CONFIG_ADC_NPOLLWAITERS >= 1`, and requires explicit handling of `POLLERR`/`POLLHUP`/`EINTR`.

### 3. Premature C-Level Implementation Weeds in Architecture and Plan Sections - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Technical Plan (Architecture)` -> `Detailed Subsystem Mechanisms` (lines 162–186) and `Detailed Implementation Plan` (lines 300–434)
* **Discrepancy Description**: Both the high-level architecture section and the implementation plan drop into C-level weeds rather than staying at component boundaries, interfaces, and data flows. Lines 162–186 inline exact register macros (`clrbits_reg32(...)`), `adc.c` framing (`buflen % 5 == 0`), exact `while` loop conditions and bitmask formulas, static symbol names (`g_bargraph_running`, `g_bargraph_pid`), and `sigaction` flags. Lines 300–434 list all 38 retained `defconfig` lines verbatim, exact `#include` lists, `for` loops, and line-by-line C statements (`memset`, `int err = errno;`, `0x15UL`/`0x2aUL`).
* **Rationale & Recommendation**: Keep `Detailed Subsystem Mechanisms` focused on architectural responsibilities, interfaces, and data flow; move code-level contracts only where strictly needed to eliminate implementer ambiguity in `Detailed Implementation Plan`, and defer exact syntax/boilerplate to source files.

### 4. Redundant Dual Singleton State (`g_bargraph_running` + `g_bargraph_pid`) - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Subsystem Mechanisms` (line 182) and `Detailed Implementation Plan` -> Item 14 (lines 422–423, 432)
* **Discrepancy Description**: `bargraph_main.c` tracks both `g_bargraph_running` and `g_bargraph_pid`, even though `g_bargraph_pid > 0 && kill(g_bargraph_pid, 0) == 0` is already the authoritative liveness check. Maintaining `g_bargraph_running` adds unnecessary synchronization across `main()`, `task_create()` error paths, and daemon exits.
* **Rationale & Recommendation**: Track `g_bargraph_pid` alone (with `-1` representing inactive) under `sched_lock()`/`sched_unlock()`.

### 5. Speculative Kconfig Options Coupled to Hardcoded Quantizer Constants - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Implementation Plan` -> Item 12 (`external/bargraph/Kconfig`, lines 401–408)
* **Discrepancy Description**: `BARGRAPH_INTERVAL_US` (`50000`) and `BARGRAPH_HYSTERESIS` (`24`, `range 0 584`) are exposed as Kconfig options even though `defconfig` never overrides them, the goals/CUJs specify 20 Hz and `±24` counts, and `BARGRAPH_HYSTERESIS` is tightly coupled to hardcoded `585`-step thresholds and a hardcoded `5`-tick fault threshold in `bargraph_main.c`.
* **Rationale & Recommendation**: Keep `PROGNAME`, `PRIORITY`, and `STACKSIZE` in Kconfig (matching `apps/examples/leds/Kconfig`) and define the sampling interval and hysteresis band as constants in `bargraph_main.c`.

### 6. Unnecessary `rp23xx_pico.h` Header and Misattributed `CONFIG_DEV_GPIO` Risk - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Subsystem Mechanisms` (line 166) and `Detailed Implementation Plan` -> Items 2 & 6 (lines 303, 306, 344–350)
* **Discrepancy Description**: Creating `board/src/rp23xx_pico.h` adds a separate internal header for single-consumer constants (`GPIO_ADC_*` is used only in `rp23xx_bringup.c`; `GPIO_LED1..6` only in `rp23xx_userleds.c`) and a single `rp23xx_bringup()` prototype. In addition, lines 166 and 303 state that `GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1` are omitted from `board.h` so `CONFIG_DEV_GPIO` cannot touch `GPIO 26–28`, even though `board/src/Make.defs` (lines 338–342) does not compile `rp23xx_gpio.c` at all.
* **Rationale & Recommendation**: Inline single-consumer pin constants in their respective C files, move `rp23xx_bringup()` declaration to `board.h`, and note accurately that `rp23xx_gpio.c` is excluded from `board/src/Make.defs`.

### 7. Permanent `Makefile` Embeds Phased-Rollout Conditionals - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Implementation Plan` -> Item 1 (`xiao-rp2350-bargraph/Makefile`, line 294)
* **Discrepancy Description**: The `setup` target includes conditional logic (`Only when external/Makefile exists`) solely to avoid a dangling symlink during intermediate Phases 1–2 before Phase 3 files exist.
* **Rationale & Recommendation**: Once Phase 3 is implemented, `external/Makefile` always exists; keep the permanent `Makefile` straightforward or document phase-specific setup commands separately from the final build script.
