# Goldfish Annotated Review Report

Status: resolved

**Target:** `docs/designs/xiao-rp2350-bargraph.md` (`full file`)

## Prioritized Findings & AI Suggestions

### [Issue 1 (`CA-01`)] [`egm-design` / `egm-codebase-details` / `egm-implementation-readiness`] - Severity: Major
*   **Affected Scope:** [Lines 243-252](docs/designs/xiao-rp2350-bargraph.md#L243-L252) (`### 4.1 ADC Subsystem Mitigations (GPIO26 / ADC0)`) and [Lines 668-674](docs/designs/xiao-rp2350-bargraph.md#L668-L674) (`#### 28. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c`)
*   **Discrepancy Description:** Section 4.1 states that on each 50 Hz burst, `bargraphd` unmasks `RP23XX_ADC_IRQ_FIFO`, reads `5` `struct adc_msg_s` samples (`25` bytes), and immediately calls `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`. However, in `nuttx/drivers/analog/adc.c` (`adc_read()`, lines 270–408), `read()` blocks on `nxsem_wait(&fifo->af_sem)` only while the upper-half software FIFO is empty (`fifo->af_head == fifo->af_tail`). As soon as the first pending ADC FIFO interrupt fires and `adc_receive()` enqueues 1 sample and posts `af_sem`, `adc_read()` wakes up, copies only the currently buffered sample(s) (`1` `struct adc_msg_s` = `5` bytes if the 2nd conversion is still in flight), and returns `nread = 5` rather than waiting for all `25` bytes. Calling `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` after a single `read()` call would therefore mask the IRQ before `msgs[1..4]` are acquired. Additionally, line 673 states "validates `msgs[i].am_data >= 0` for `i = 1..4`" without specifying the fallback behavior if a conversion error bit (`am_data < 0`) occurs on one or more samples.
*   **Rationale:** Specify in Section 4.1 (lines 245, 249) and Section 28 (line 672) that `bargraphd` loops on `read()` (accumulating `sizeof(struct adc_msg_s)` = `5`-byte increments, retrying on `EINTR` and always masking `RP23XX_ADC_IRQ_FIFO` before returning on error) until all `5` `struct adc_msg_s` entries (`25` bytes total) have been collected before calling `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`, and specify that valid samples (`am_data >= 0`) in `msgs[1..4]` are averaged (or if all 4 fresh samples have `am_data < 0`, the previous `filtered_adc` reading is retained).
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** In `nuttx/drivers/analog/adc.c`, `adc_read()` returns immediately once at least one `struct adc_msg_s` (`5` bytes) has been copied out of the upper-half ring buffer. Looping on `read()` until all `5 * sizeof(struct adc_msg_s)` (`25` bytes) are accumulated before masking `RP23XX_ADC_IRQ_FIFO`, handling `EINTR`, and defining the `am_data < 0` fallback prevents reading uninitialized stack entries in `msgs[1..4]`.
*   ***Proposed Edit:***
    *   Location: Lines 245, 249–250 in Section `4.1 ADC Subsystem Mitigations` and Lines 668–674 in Target File `#28` (`bargraph_main.c`).
    *   Change: Specify that `bargraph_sample_adc()` loops over `read(adc_fd, ...)` in `sizeof(struct adc_msg_s)` (`5`-byte) increments (retrying on `EINTR`) until all `5` `struct adc_msg_s` samples (`25` bytes) are collected, always calls `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` before returning on any `read()` error, averages all valid fresh samples (`msgs[i].am_data >= 0` for `i = 1..4`), and retains the previous `filtered_adc` value if all 4 fresh samples have `am_data < 0`.

---

### [Issue 2 (`CA-02`)] [`egm-zoom-out`] - Severity: Major
*   **Affected Scope:** [Lines 209-229](docs/designs/xiao-rp2350-bargraph.md#L209-L229) (`### 3. Boot Sequence`), [Lines 242-247](docs/designs/xiao-rp2350-bargraph.md#L242-L247) (`### 4.1.3 Demand-Driven NVIC IRQ Gating`), and [Lines 400-406, 511-533, 668-674](docs/designs/xiao-rp2350-bargraph.md#L400-L674) (Target Files `#3`, `#16`, `#18`, `#28`)
*   **Discrepancy Description:** The top-level workspace blueprint (`README.md#L16`) and `nuttx/INVIOLABLES.md#L26-L41` define a strict Global Invariant on Kernel/User-Space Separation. Even in `CONFIG_BUILD_FLAT=y`, having user-space code in `xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c` include `<nuttx/irq.h>` and `<arch/rp23xx/irq.h>` to call privileged NVIC functions `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` and `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` directly—and having board-level kernel code (`rp23xx_bringup.c`) directly call a custom application symbol `bargraph_autostart()`—blurs the kernel/user-space boundary.
*   **Rationale:** Explicitly document in Section 4.1.3 and `xiao-rp2350-bargraph/README.md` (`## Invariants`) why flat-build NVIC gating from `bargraphd` and calling `bargraph_autostart()` from `rp23xx_bringup.c` are intentional `CONFIG_BUILD_FLAT=y` exceptions to the workspace's Kernel/User-Space Separation invariant required to keep `nuttx/` and `apps/` unmodified.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Because `nuttx/` and `apps/` cannot be modified (Non-Goal #1) and `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` provides no ioctl to halt the 500 kS/s `START_ONCE` interrupt chain while keeping `/dev/adc0` open, calling `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` in flat build mode (`CONFIG_BUILD_FLAT=y`) and invoking `bargraph_autostart()` from `rp23xx_bringup.c` are deliberate architectural trade-offs that should be explicitly documented in Section 4.1.3 and in `xiao-rp2350-bargraph/README.md` (`## Invariants`).
*   ***Proposed Edit:***
    *   Location: Section `4.1.3` (lines 242–247) and Target File `#3` (`xiao-rp2350-bargraph/README.md`, lines 404–406).
    *   Change: Add an explicit note in Section `4.1.3` and Target File `#3` (`## Invariants`) documenting that direct NVIC gating (`up_enable_irq` / `up_disable_irq` on `RP23XX_ADC_IRQ_FIFO`) in `bargraphd` and `bargraph_autostart()` invocation from `rp23xx_bringup()` are intentional `CONFIG_BUILD_FLAT=y` out-of-tree exceptions to the workspace Kernel/User-Space Separation invariant ([`README.md`](../../README.md)) so that `nuttx/` and `apps/` remain unmodified.

---

### [Issue 3 (`CA-03`)] [`egm-implementation-readiness` / `egm-codebase-details`] - Severity: Nit
*   **Affected Scope:** [Lines 408-567](docs/designs/xiao-rp2350-bargraph.md#L408-L567) (Target Files `#4`, `#6`, `#7`, `#14`, `#19`, `#23`)
*   **Discrepancy Description:** While Target Files `#3` (`xiao-rp2350-bargraph/README.md`), `#9` (`board/include/README.md`), and `#11` (`board/scripts/README.md`) explicitly list their required EGM sections and `[file:symbol]` citations, the remaining `README.md` entries in `### Target Files to Create/Modify` (`#4`, `#6`, `#7`, `#14`, `#19`, `#23`) omit explicit section headers and `[file:symbol]` citation lists matching the workspace blueprint contract (`README.md`) and leaf component contract (`nuttx/boards/arm/rp23xx/xiao-rp2350/src/README.md`).
*   **Rationale:** Explicitly specify the 5 required EGM sections (`Overview`, `Subdirectories & Boundaries` or `Interface`, `Invariants`, `Side Effects`, `Verification`) and the exact `[file:symbol]` citations for Target Files `#4`, `#6`, `#7`, `#14`, `#19`, and `#23` so every `README.md` in `xiao-rp2350-bargraph/` can be authored deterministically.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Specifying the exact 5 EGM sections and `[file:symbol]` tokens for all 9 `README.md` target files eliminates ambiguity during implementation and ensures uniform compliance with the workspace EGM contracts.
*   ***Proposed Edit:***
    *   Location: Target Files `#4`, `#6`, `#7`, `#14`, `#19`, and `#23` in `### Target Files to Create/Modify`.
    *   Change: Add explicit 5-section lists (`## Overview`, `## Subdirectories & Boundaries` or `## Interface`, `## Invariants`, `## Side Effects`, `## Verification`) and concrete `[file:symbol]` citation lists to Target Files `#4`, `#6`, `#7`, `#14`, `#19`, and `#23`.

---

### [Issue 4 (`CA-04`)] [`egm-comprehension` / `egm-zoom-out`] - Severity: Nit
*   **Affected Scope:** [Lines 642-692](docs/designs/xiao-rp2350-bargraph.md#L642-L692) (`#### 28. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c`)
*   **Discrepancy Description:** While the sequence diagram (line 221), CLI table (line 317), and `bargraph_main.c` rationale (line 645) state that `bargraph_autostart()` and `bargraph start` spawn `bargraphd` via `task_create()`, the `Proposed Changes` list for `bargraph_main.c` (lines 646–700) does not explicitly state whether `bargraph_autostart()` blocks on `done_cond` like `bargraph start` or how it passes `CONFIG_BARGRAPH_PRIORITY` and `CONFIG_BARGRAPH_STACKSIZE` to `task_create()`.
*   **Rationale:** Specify a single internal helper (`bargraph_start_daemon()`) called by both `bargraph_autostart()` and `bargraph start` that spawns `bargraphd` via `task_create("bargraphd", CONFIG_BARGRAPH_PRIORITY, CONFIG_BARGRAPH_STACKSIZE, bargraphd_daemon, NULL)` under `g_bargraph.lock` and waits on `done_cond` until `running` is true or `init_err < 0`.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Defining a shared `bargraph_start_daemon()` helper in Target File `#28` answers the comprehension question and avoids duplicating `task_create()` and `done_cond` synchronization logic between `bargraph_autostart()` and `bargraph start`.
*   ***Proposed Edit:***
    *   Location: Target File `#28` (`#### 28. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c`).
    *   Change: Add a bullet for `bargraph_start_daemon()` / `bargraph_autostart()` specifying that both `bargraph_autostart()` and `bargraph start` call `bargraph_start_daemon()`, which locks `g_bargraph.lock`, spawns `task_create("bargraphd", CONFIG_BARGRAPH_PRIORITY, CONFIG_BARGRAPH_STACKSIZE, bargraphd_daemon, NULL)` when `!g_bargraph.running`, and waits on `done_cond` until `running` is true or `init_err < 0`.

---

### [Issue 5 (`CA-05`)] [`egm-design` / `egm-zoom-out`] - Severity: Nit
*   **Affected Scope:** [Lines 289-323](docs/designs/xiao-rp2350-bargraph.md#L289-L323) (`### 5. Daemon State Machine & IPC Synchronization`)
*   **Discrepancy Description:** The `stateDiagram-v2` (lines 289–303) defines transitions out of `TEST` for sweep completion (`TEST --> SENSING`, `TEST --> MANUAL`) and `bargraph stop` (`TEST --> STOPPED`), while the CLI table (lines 313–323) states that `bargraph test` restores `prev_mode` (`SENSING` or `MANUAL`). It does not explicitly state what happens if `bargraph set <0..100>`, `bargraph resume`, or a second `bargraph test` is invoked (e.g., if `bargraph test &` was backgrounded in NSH) while `mode == BARGRAPH_MODE_TEST`.
*   **Rationale:** Note in Section 5 that `prev_mode` is captured only when transitioning into `TEST` from `SENSING` or `MANUAL` (never overwriting `prev_mode` with `BARGRAPH_MODE_TEST`), and that issuing `bargraph set <0..100>` or `bargraph resume` during an active `TEST` sweep aborts the sweep (`test_requested = false`, broadcasting `done_cond`) and transitions directly to `MANUAL` or `SENSING`.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Clarifying that `prev_mode` is only recorded when `mode != BARGRAPH_MODE_TEST` and that `set`/`resume` preempt an active `TEST` sweep prevents a backgrounded `bargraph test &` from trapping the state machine in `BARGRAPH_MODE_TEST`, using the existing fields of `struct bargraph_state_s`.
*   ***Proposed Edit:***
    *   Location: Lines 317–323 (`### 5. Daemon State Machine & IPC Synchronization` CLI table) and Target File `#28` (`TEST` mode bullet).
    *   Change: Specify that `prev_mode` is saved only when `mode != BARGRAPH_MODE_TEST`, and that `bargraph set <0..100>` or `bargraph resume` during `BARGRAPH_MODE_TEST` clears `test_requested = false`, transitions `mode` to `MANUAL` or `SENSING`, signals `wake_cond`, and broadcasts `done_cond`.

---

### [Issue 6 (`CA-06`)] [`egm-zoom-out`] - Severity: Nit
*   **Affected Scope:** [Lines 119-120](docs/designs/xiao-rp2350-bargraph.md#L119-L120) (`### 1. System Architecture`) and [Lines 590-641](docs/designs/xiao-rp2350-bargraph.md#L590-L641) (`#### 27. xiao-rp2350-bargraph/apps/bargraph/bargraph_internal.h`)
*   **Discrepancy Description:** `xiao-rp2350-bargraph/apps/bargraph/` has only a single C source file (`bargraph_main.c`). Creating `bargraph_internal.h` without documenting why a separate header is used for a single-TU app appears redundant unless its purpose for host-side pure math verification (`comprehension_report.md` #2) is stated explicitly.
*   **Rationale:** Either fold `bargraph_internal.h` into `bargraph_main.c`, or if `bargraph_internal.h` is retained so the host-side math verification step can cleanly include the pure math/filter declarations and constants, state that rationale explicitly in Target File `#27`.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Retaining `bargraph_internal.h` allows the host-side pure math verification test (`bargraph_compute_duties` and `bargraph_filter_adc`) and `bargraph_main.c` to share the exact `#define` constants (`BARGRAPH_DUTY_MAX`, `BARGRAPH_ADC_DEADBAND_LOW`, `BARGRAPH_ADC_DEADBAND_HIGH`, etc.) and prototypes cleanly. Stating this rationale in Target File `#27` resolves the finding without losing shared constants during host verification.
*   ***Proposed Edit:***
    *   Location: Target File `#27` (`#### 27. xiao-rp2350-bargraph/apps/bargraph/bargraph_internal.h`, Rationale).
    *   Change: Update Rationale to state that `bargraph_internal.h` isolates the hardware/filter constants, `struct bargraph_state_s`, and pure math/filter function prototypes (`bargraph_compute_duties`, `bargraph_filter_adc`) so they can be shared directly between `bargraph_main.c` and the host-side unit verification harness.

---

### [Issue 7 (`CA-07`)] [`egm-zoom-out`] - Severity: Nit
*   **Affected Scope:** [Lines 455-471](docs/designs/xiao-rp2350-bargraph.md#L455-L471) (Target Files `#9` and `#10`: `xiao-rp2350-bargraph/board/include/README.md` and `board.h`)
*   **Discrepancy Description:** Target File `#10` proposes defining `BARGRAPH_ADC_DEVPATH "/dev/adc0"`, `BARGRAPH_ADC_GPIO 26`, and `BARGRAPH_LED1_GPIO 27` through `BARGRAPH_LED6_GPIO 0` in `board/include/board.h`. However, all PWM and ADC pin assignments are already defined by `CONFIG_RP23XX_PWM*` and `CONFIG_RP23XX_ADC_CHANNEL0` in `defconfig` and consumed by `rp23xx_common_bringup.c`, while `bargraph_main.c` opens `/dev/pwm*` and `/dev/adc0` by VFS device path and never manipulates LED GPIO pins directly.
*   **Rationale:** Keep `board.h` minimal—matching `nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h` minus the unused bus headers—and rely on the Kconfig symbols for PWM GPIO routing, only defining `BARGRAPH_ADC_GPIO 26` (used by `rp23xx_bringup.c` for `RP23XX_PADS_BANK0_GPIO`).
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Removing unused `BARGRAPH_LED1_GPIO`..`BARGRAPH_LED6_GPIO` macros from `board.h` eliminates duplicate pin definitions between `defconfig` and `board.h`, leaving `BARGRAPH_ADC_GPIO 26` as the single board-level pad-isolation macro consumed by `rp23xx_bringup.c`.
*   ***Proposed Edit:***
    *   Location: Target Files `#9` (`board/include/README.md`), `#10` (`board/include/board.h`), and `#18` (`rp23xx_bringup.c`).
    *   Change: Remove `BARGRAPH_LED1_GPIO`..`BARGRAPH_LED6_GPIO` and `BARGRAPH_ADC_DEVPATH` from `board/include/board.h` and `board/include/README.md`, keeping `BARGRAPH_ADC_GPIO 26` (and using `RP23XX_PADS_BANK0_GPIO(BARGRAPH_ADC_GPIO)` in `rp23xx_bringup.c`).

---

### [Issue 8 (`CA-08`)] [`egm-style` / `egm-zoom-out`] - Severity: Nit
*   **Affected Scope:** [Line 36](docs/designs/xiao-rp2350-bargraph.md#L36) (`## Goals`) and [Lines 268-274](docs/designs/xiao-rp2350-bargraph.md#L268-L274) (`### 4.2 Hardware PWM Subsystem Mitigations`)
*   **Discrepancy Description:** Line 36 (`Complete Breadboard & EGM Documentation`) is a single 115-word sentence with nested em-dashes and parentheticals, and lines 269–273 combine upper-half contract rules, `memcpy()` details, lower-half register bitfields, and slice channel assignments into a single multi-clause sentence.
*   **Rationale:** Break line 36 and lines 269–273 into shorter sentences and clarify that populating `channels[0].channel = 1` and `channels[1].channel = 2` satisfies the `nuttx/include/nuttx/timers/pwm.h` contract and `pwm_dump()` debug logging while `drivers/timers/pwm.c` copies `struct pwm_info_s` verbatim via `memcpy()`.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Splitting both dense sentences improves readability and clarifies how `drivers/timers/pwm.c` and `rp23xx_pwm.c` handle `struct pwm_info_s`.
*   ***Proposed Edit:***
    *   Location: Line 36 (`## Goals`) and Lines 269–273 (`### 4.2.2 Two-Channel struct pwm_info_s Population`).
    *   Change: Split each bullet into clear, concise sentences and explicitly note that `channels[0].channel = 1` and `channels[1].channel = 2` satisfy the `pwm.h` header contract and `pwm_dump()` debug logging while `drivers/timers/pwm.c` copies `struct pwm_info_s` via `memcpy()`.

---

### [Issue 9 (`CA-09`)] [`egm-zoom-out`] - Severity: Nit
*   **Affected Scope:** [Lines 649-689](docs/designs/xiao-rp2350-bargraph.md#L649-L689) (`#### 28. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c`)
*   **Discrepancy Description:** Target File `#28` in the `Detailed Implementation Plan` repeats line-by-line C expressions (`*ema_q4 = *ema_q4 - (*ema_q4 >> 2) + ((uint32_t)raw_avg << 2)`, exact integer rounding expressions, etc.) that duplicate the mathematical specifications already presented in Sections 4.1.4, 4.1.5, and 4.2.3.
*   **Rationale:** Reference the mathematical transfer function and Q4 EMA equations already defined in Sections 4.1.4, 4.1.5, and 4.2.3 rather than repeating raw C shift/rounding expressions inline in Target File `#28`.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Referencing the canonical equations in Sections 4.1.4, 4.1.5, and 4.2.3 keeps Target File `#28` concise and eliminates duplicate formula definitions.
*   ***Proposed Edit:***
    *   Location: Lines 649–689 (`#### 28. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c`).
    *   Change: Condense the `bargraph_compute_duties`, `bargraph_filter_adc`, and `bargraphd_daemon` math bullets to reference the transfer function and Q4 EMA equations in Sections 4.1.4, 4.1.5, and 4.2.3 while preserving all behavioral, scaling (`0..3300 mV`, `0..1000` permille, `100`-step `TEST` triangle sweep), and synchronization contracts.

---

### [Issue 10 (`CA-10`)] [`egm-style` / `egm-implementation-readiness`] - Severity: Nit
*   **Affected Scope:** [Lines 708-712](docs/designs/xiao-rp2350-bargraph.md#L708-L712) (`### Verification Plan` -> `Out-of-Tree Configuration & Firmware Build`)
*   **Discrepancy Description:** Line 710 specifies `./tools/configure.sh -l ../xiao-rp2350-bargraph/board/configs/bargraph` (which forces `CONFIG_HOST_LINUX=y`) paired on line 711 with `make -j$(sysctl -n hw.ncpu)` (which uses macOS `sysctl`). In `nuttx/tools/configure.sh` and `nuttx/tools/sethost.sh` (lines 87–114), omitting `-l` auto-detects the host OS (`Darwin` -> `CONFIG_HOST_MACOS=y`, `Linux` -> `CONFIG_HOST_LINUX=y`).
*   **Rationale:** Omit `-l` in `./tools/configure.sh ../xiao-rp2350-bargraph/board/configs/bargraph` and use portable job parallelism (`make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)`) so the verification snippet works cleanly on both macOS and Linux hosts.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Removing `-l` lets `nuttx/tools/sethost.sh` auto-detect macOS or Linux, and `$(nproc 2>/dev/null || sysctl -n hw.ncpu)` works on both platforms.
*   ***Proposed Edit:***
    *   Location: Lines 708–712 (`### Verification Plan`).
    *   Change: Replace `./tools/configure.sh -l ../xiao-rp2350-bargraph/board/configs/bargraph` and `make -j$(sysctl -n hw.ncpu)` with `./tools/configure.sh ../xiao-rp2350-bargraph/board/configs/bargraph` and `make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)`.

---

### [Issue 11 (`CA-11`)] [`egm-comprehension` / `egm-zoom-out`] - Severity: Nit
*   **Affected Scope:** [Lines 716-717](docs/designs/xiao-rp2350-bargraph.md#L716-L717) (`### Verification Plan` -> `Host-Side Pure Math & Transfer Function Unit Verification`)
*   **Discrepancy Description:** The verification step specifies testing the transfer function, `>> 19` shift extraction, Q4 EMA filter, and deadband clamping on the host, but does not state how a reader executes this check (e.g., whether it is run via an ephemeral host `cc` one-liner against `bargraph_compute_duties()`/`bargraph_filter_adc()`).
*   **Rationale:** Specify in `Verification Plan` that the pure helper functions (`bargraph_compute_duties()` and `bargraph_filter_adc()`) are verified via an ephemeral host `cc` invocation targeting `/tmp` (removed immediately after execution) so the repository tree remains strictly focused on the target firmware.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Explicitly stating that the host math check compiles an ephemeral test binary in `/tmp` via host `cc` against `bargraph_internal.h` and removes it after execution makes the verification step self-contained without adding tracked test files to the repository.
*   ***Proposed Edit:***
    *   Location: Lines 716–717 (`### Verification Plan`).
    *   Change: Specify that the pure helper functions (`bargraph_compute_duties()` and `bargraph_filter_adc()`) and `>> 19` sample extraction are verified by compiling an ephemeral host `cc` test harness in `/tmp` (including `bargraph_internal.h`) and removing the temporary binary immediately after execution.
