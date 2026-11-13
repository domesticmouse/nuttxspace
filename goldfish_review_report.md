Status: resolved

# Goldfish Annotated Review Report

**Target:** `/home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md` (`1..459`)

## Summary
- **Total Consolidated Findings:** 19 (`2` Blockers, `10` Majors, `7` Nits)
- **Accepted Fixes:** 19 (`CON-1` through `CON-19`)
- **Contested Items:** 0
- **Escalated Items:** 0
- **Declined Items:** 0

## Prioritized Findings & AI Suggestions

### CON-1 [egm-design, egm-zoom-out, egm-inventive-catalyst, egm-style] - Severity: Blocker
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L174-L236), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L425) (`ADC Sampling, Normalization, and Hysteresis`, `Alternative G`, `Non-Functional Requirements`, `Risks & Limits`, `read_adc_sample`)
* **Discrepancy Description:** `adc_close()` in `nuttx/drivers/analog/adc.c` holds only `nxmutex_lock(&dev->ad_lock)` when calling `ao_shutdown()` -> `my_shutdown()` -> `remove_device()`, whereas `remove_device()` in `rp23xx_adc.c` assumes a critical section and sets `g_first_device = NULL` before decrementing `g_active_count`. An active ADC FIFO IRQ preempting between those two writes triggers `ASSERT(next != g_current_channel)` in `get_next_channel()`. Line 226 also lacked the `START_ONCE` timing explanation, and `Alternative G` overstated `rp23xx_common_bringup.c` size and asserted unverified flicker prevention.
* **Rationale:** Preventing the race and calibrating the timing and line-count claims removes a fatal kernel assertion risk.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Wrapping `close(fd)` in `read_adc_sample()` with `enter_critical_section()` / `leave_critical_section()` (`<nuttx/irq.h>`) while `bargraph_daemon` is the sole opener (`dev->ad_lock` uncontended) makes `remove_device()` atomic against `RP23XX_ADC_IRQ_FIFO`, while documenting `RP23XX_ADC_CS_START_ONCE` timing (300 CPU cycles between ISRs) and calibrating `Alternative G`.
* ***Proposed Edit:***
  * Location: `ADC Sampling`, `Alternative G`, `Non-Functional Requirements`, `Risks & Limits`, and `bargraph_main.c` (`read_adc_sample`).
  * Change: Specify `enter_critical_section()` / `leave_critical_section()` around `close(fd)`, add `START_ONCE` inter-ISR cycle budget, and calibrate `Alternative G` line counts and target phrasing.

### CON-2 [egm-codebase-details] - Severity: Blocker
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L21) (`The Problem`, item 3) and [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L326) (`defconfig`)
* **Discrepancy Description:** Line 21 claimed upstream `xiao-rp2350/configs/usbnsh/defconfig` omits `CONFIG_BOARD_LATE_INITIALIZE=y` and thus fails to call `rp23xx_bringup()`. In `nuttx/sched/Kconfig`, `BOARD_LATE_INITIALIZE` defaults to `y`, and `usbnsh/defconfig` leaves it enabled.
* **Rationale:** Contradicted codebase configuration and boot behavior.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Upstream `usbnsh/defconfig` lacks `/dev/adc0` and `/dev/userleds` because `CONFIG_ADC`, `CONFIG_RP23XX_ADC`, `CONFIG_USERLED`, and `CONFIG_USERLED_LOWER` are unset and `CONFIG_ARCH_LEDS` defaults to `y`, not because `BOARD_LATE_INITIALIZE` is disabled.
* ***Proposed Edit:***
  * Location: `The Problem` item 3 and `defconfig` notes.
  * Change: Replace the false `CONFIG_BOARD_LATE_INITIALIZE` diagnosis with the actual missing driver options and note `BOARD_LATE_INITIALIZE` defaults to `y`.

### CON-3 [egm-inventive-catalyst, egm-design, egm-zoom-out] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L59-L63), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L236), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L425)
* **Discrepancy Description:** Opening `/dev/adc0` in blocking mode causes `adc_read()` to wait indefinitely in `nxsem_wait(&fifo->af_sem)` if an ADC conversion interrupt fails to arrive, preventing CUJ 3 fault detection.
* **Rationale:** Reuses `adc_poll()` already present in `nuttx/drivers/analog/adc.c`.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Opening with `O_RDONLY | O_NONBLOCK` and calling `poll()` (`POLLIN`, 10 ms timeout) before `read()` bounds each tick and maps timeouts (`-EAGAIN`) into `consecutive_errors`.
* ***Proposed Edit:***
  * Location: `System Data Flow`, `ADC Sampling`, `Risks & Limits`, and `read_adc_sample`.
  * Change: Specify `O_RDONLY | O_NONBLOCK` and `poll()` with `10 ms` timeout before `read()`.

### CON-4 [egm-zoom-out] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L162-L186), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L300-L434)
* **Discrepancy Description:** `Detailed Subsystem Mechanisms` and `Detailed Implementation Plan` included excessive C-level implementation weeds and verbatim 38-line `defconfig` symbol lists, causing cross-section symbol drift.
* **Rationale:** Architecture sections should describe component boundaries, interfaces, and data flow.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Lifting `Detailed Subsystem Mechanisms` to interfaces/data flow and summarizing `defconfig` deltas eliminates altitude drops and cross-section duplication.
* ***Proposed Edit:***
  * Location: `Detailed Subsystem Mechanisms` and `Detailed Implementation Plan` (`defconfig`, `rp23xx_bringup.c`, `bargraph_main.c`).
  * Change: Keep architectural boundaries in `Detailed Subsystem Mechanisms` and concise contracts in `Detailed Implementation Plan`.

### CON-5 [egm-implementation-readiness, egm-codebase-details] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L423) (`bargraph_main.c`)
* **Discrepancy Description:** `task_create()` prepends `name` as `argv[0]` and shifts the caller's `NULL`-terminated `argv[]` to `argv[1..]`, which was unspecified alongside `task_create()` priority/stack arguments and `sched_unlock()` on early exits.
* **Rationale:** Eliminates argument-indexing bugs and missing lock releases during implementation.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Explicitly documenting `daemon_argv = { (FAR char *)adc_path, NULL }`, reading `argv[1]` in `bargraph_daemon()`, and releasing `sched_unlock()` on every `main()` path enforces the Zero-Decision Rule.
* ***Proposed Edit:***
  * Location: `bargraph_main.c` specification.
  * Change: Specify `task_create()` arguments, `NULL`-terminated `daemon_argv`, `argv[1]` indexing, and `sched_unlock()` + return values on all `main()` paths.

### CON-6 [egm-implementation-readiness, egm-codebase-details, egm-comprehension] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L423-L432) (`bargraph_main.c`)
* **Discrepancy Description:** Pre-loop failure exits (`sigaction`, `/dev/userleds` open/ioctl) bypassed singleton/descriptor cleanup, `/dev/null` open failure was unhandled, and `fault_blink_loop` `ioctl` failure could lose `EXIT_FAILURE`.
* **Rationale:** Every daemon exit path must release hardware/singleton state deterministically.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Routing all exits through a unified `cleanup:` block with `ret_code` preserves `EXIT_FAILURE`, clears/closes `led_fd` if open, and clears `g_bargraph_pid` under `sched_lock()`.
* ***Proposed Edit:***
  * Location: `bargraph_main.c` specification.
  * Change: Add unified `cleanup:` flow, `/dev/null` error handling, and `ret_code = EXIT_FAILURE` preservation.

### CON-7 [egm-implementation-readiness, egm-style, egm-comprehension] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L185), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L422-L451)
* **Discrepancy Description:** Verification Plan CUJ 2 checked `dmesg` for a `bargraph_daemon` startup log that was never emitted in `bargraph_main.c`, and `task_create()` failure logging was missing in item 14.
* **Rationale:** Aligns daemon logging with CUJ 2 verification checks.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Adding `syslog(LOG_INFO, ...)` at daemon startup (after `/dev/userleds` init) and `syslog(LOG_ERR, ...)` on `task_create()` / init errors ensures `dmesg` reflects startup and failures.
* ***Proposed Edit:***
  * Location: `bargraph_main.c` specification.
  * Change: Specify startup `syslog(LOG_INFO, ...)` and error `syslog(LOG_ERR, ...)` calls.

### CON-8 [egm-implementation-readiness, egm-codebase-details, egm-comprehension] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L294), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L396) (`Makefile`)
* **Discrepancy Description:** `apps/Makefile` `Kconfig:` has no prerequisites and `apps/Make.defs` filters out `external/` when `external/Kconfig` exists; `DELIM` is also empty before `configure.sh` creates `nuttx/Make.defs`. Additionally, `preconfig` was undefined.
* **Rationale:** Without `rm -f ../apps/Kconfig external/Kconfig` and proper `configure`/Make ordering, Kconfig updates silently skip `external/bargraph`.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Explicitly removing stale `Kconfig` files, running `olddefconfig` when `.config` exists (and letting `configure` generate them otherwise, or passing `DELIM=/`), fixes Kconfig regeneration.
* ***Proposed Edit:***
  * Location: `Makefile` `setup` target and `external/Makefile` rationale.
  * Change: Specify `rm -f ../apps/Kconfig external/Kconfig`, `olddefconfig` vs. `configure` timing, and briefly define `preconfig`.

### CON-9 [egm-implementation-readiness] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L283-L442) (`Target Files to Create` vs. `Host Unit Check`)
* **Discrepancy Description:** Verification Plan required a host `gcc` unit test for `quantize_level()` and ADC decoding, but no pure-C header, test source file, or `Makefile` test target existed in `Target Files to Create`.
* **Rationale:** Enforces Target Scope & File Boundary without unapproved refactors.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Extracting pure-C quantizer/decoder helpers into `external/bargraph/bargraph_logic.h` and adding `tests/test_bargraph_logic.c` plus a `make test` target makes host testing self-contained.
* ***Proposed Edit:***
  * Location: Project Layout, `Makefile`, Target Files to Create, and Verification Plan.
  * Change: Add `external/bargraph/bargraph_logic.h`, `tests/test_bargraph_logic.c`, and `Makefile` `test` target.

### CON-10 [egm-comprehension, egm-zoom-out] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L166) (`Board Bringup and Analog Pad Input-Buffer Fix`)
* **Discrepancy Description:** Line 166 used `ao_reset()` without defining it and causally conflated it with disabling `CONFIG_DEV_GPIO`.
* **Rationale:** Improves self-containment and separates ADC driver callbacks from GPIO character driver wiring.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Defining `ao_reset()` as the `adc_ops_s` reset callback (`my_reset()` in `rp23xx_adc.c`) and separating the `CONFIG_DEV_GPIO` / `rp23xx_gpio.c` explanation resolves the ambiguity.
* ***Proposed Edit:***
  * Location: `Board Bringup and Analog Pad Input-Buffer Fix`.
  * Change: Define `ao_reset()` inline and separate it from `CONFIG_DEV_GPIO` omission.

### CON-11 [egm-style] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L182-L183) (`Daemon Lifecycle`)
* **Discrepancy Description:** Line 182 claimed `bargraph [adc_devpath]` matches `apps/examples/adc/adc_main.c` (which uses `-p <devpath>`), and line 183 misstated boot order by claiming `nsh_usbconsole.c` calls `nsh_nullstdio()` before `rcS` runs.
* **Rationale:** Claims about existing upstream code must match the source files.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Calibrating both statements aligns the spec with `adc_main.c`, `nsh_main.c`, `nsh_usbconsole.c`, and `group_setupidlefiles.c`.
* ***Proposed Edit:***
  * Location: `Daemon Lifecycle, ROMFS Startup, and Headless Diagnostics`.
  * Change: Clarify positional `adc_devpath` vs. `adc_main.c`'s `-p` flag, and replace the `nsh_nullstdio()` clause with `group_setupidlefiles()`.

### CON-12 [egm-style] - Severity: Major
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L237), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L283), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L389), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L452-L453)
* **Discrepancy Description:** Subjective `"healthy stack headroom"`, uncalibrated `"~500,000 continuous interrupts/second"`, `"bootable"` Phase 2 image claim before flashing, incomplete `etctmp*` / `.gitignore` description, and non-actionable `"Mitigation:"` label on `0`-sample `FIFO_ERR`.
* **Rationale:** Calibrates verification pass criteria and operational claims to evidence.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Adding concrete numeric thresholds (`<= 1536 B` stack used / `>= 512 B` free, `20–200 IRQs/s` vs. continuous free-running FIFO interrupts) and correcting the `etctmp*`, Phase 2, and `Limitation:` text resolves all five inaccuracies.
* ***Proposed Edit:***
  * Location: `Risks & Limits`, `Makefile`, Phase 2 Checkpoint, and Verification Plan CUJ 2.
  * Change: Apply numeric bounds and calibrated descriptions.

### CON-13 [egm-zoom-out, egm-design] - Severity: Nit
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L182-L183), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L422-L432)
* **Discrepancy Description:** Dual singleton flags (`g_bargraph_running` + `g_bargraph_pid`) duplicated liveness state.
* **Rationale:** Tracking `g_bargraph_pid` alone (`-1` inactive) with PID-checked exit cleanup under `sched_lock()` is simpler and avoids desync.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Removes redundant state while closing the exit-race edge case.
* ***Proposed Edit:***
  * Location: `Daemon Lifecycle` and `bargraph_main.c`.
  * Change: Use `g_bargraph_pid` alone (`-1` when inactive).

### CON-14 [egm-zoom-out] - Severity: Nit
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L401-L408) (`external/bargraph/Kconfig`)
* **Discrepancy Description:** `BARGRAPH_INTERVAL_US` and `BARGRAPH_HYSTERESIS` in Kconfig were never overridden in `defconfig` and were coupled to hardcoded thresholds in C.
* **Rationale:** Aligns with `apps/examples/leds/Kconfig` (`PROGNAME`, `PRIORITY`, `STACKSIZE`).
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Moving fixed timing and hysteresis constants to `bargraph_logic.h` / `bargraph_main.c` removes YAGNI Kconfig options.
* ***Proposed Edit:***
  * Location: `external/bargraph/Kconfig` and `bargraph_logic.h`.
  * Change: Keep only `BARGRAPH`, `BARGRAPH_PROGNAME`, `BARGRAPH_PRIORITY`, and `BARGRAPH_STACKSIZE` in Kconfig.

### CON-15 [egm-zoom-out] - Severity: Nit
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L166), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L303-L350)
* **Discrepancy Description:** `board/src/rp23xx_pico.h` existed solely for single-consumer pin constants and one `rp23xx_bringup()` prototype, and `board/src/Make.defs` omits `rp23xx_gpio.c`.
* **Rationale:** Eliminates an unneeded file and misattributed `CONFIG_DEV_GPIO` risk.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Declaring `rp23xx_bringup()` in `board.h`, moving `GPIO_ADC_*` to `board.h` (or `rp23xx_bringup.c`), and keeping `GPIO_LED1..6` in `rp23xx_userleds.c` simplifies `board/src/`.
* ***Proposed Edit:***
  * Location: Project Layout, `board.h`, `rp23xx_pico.h` (removed), `rp23xx_bringup.c`, `rp23xx_userleds.c`.
  * Change: Remove `rp23xx_pico.h` and update header declarations.

### CON-16 [egm-zoom-out] - Severity: Nit
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L294-L298) (`Makefile`)
* **Discrepancy Description:** Transitional `"Only when external/Makefile exists"` scaffolding in the permanent `Makefile`, plus unnecessary `etctmp.c` deletion after `all`.
* **Rationale:** Simplifies `Makefile` once Phase 3 exists and avoids forcing ROMFS rebuilds on every incremental `make`.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** `.gitignore` already ignores `etctmp*`, so deleting `etctmp` and `etctmp.c` only in `clean`/`distclean` keeps incremental builds fast and `git status` clean.
* ***Proposed Edit:***
  * Location: `Makefile` target descriptions.
  * Change: Remove transitional Phase 1–2 symlink guard from final `Makefile` and move `etctmp*` cleanup to `clean`/`distclean`.

### CON-17 [egm-comprehension, egm-design] - Severity: Nit
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L142), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L156), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L202)
* **Discrepancy Description:** `quantize_while` vs. `quantize_level` naming mismatch, diagram `task_create(...) returns 0`, and undefined `ad_isovr`.
* **Rationale:** Improves terminology consistency and diagram accuracy.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Quick editorial alignment across diagrams and text.
* ***Proposed Edit:***
  * Location: System Data Flow diagram and `Alternative C`.
  * Change: Use `quantize_level`, distinguish `task_create() > 0` from `main() returns 0`, and gloss `ad_isovr`.

### CON-18 [egm-implementation-readiness] - Severity: Nit
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L307), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L415), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L425), [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L431)
* **Discrepancy Description:** Missing `#ifndef __ASSEMBLY__` in `board.h`, explicit `$(CONFIG_BARGRAPH_*)` bindings in `external/bargraph/Makefile`, `if (fd < 0)` check before `read()`/`close()` in `read_adc_sample()`, and normal tick `ioctl` failure handling.
* **Rationale:** Eliminates minor implementation ambiguities.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Adding these four explicit checks prevents compile/runtime edge bugs.
* ***Proposed Edit:***
  * Location: `board.h`, `external/bargraph/Makefile`, and `bargraph_main.c`.
  * Change: Specify `#ifndef __ASSEMBLY__`, Makefile variable bindings, `fd < 0` early return, and normal tick `ioctl` error exit.

### CON-19 [egm-style] - Severity: Nit
* **Affected Scope:** [xiao-rp2350-bargraph.md](file:///home/brett/Documents/GitHub/nuttxspace/docs/designs/xiao-rp2350-bargraph.md#L166-L435)
* **Discrepancy Description:** Filler phrases (`"Constraint-Driven"`, `"note that"`, `"with zero errors"`), non-actionable `"keep the wiper connected"`, unreachable sub-millisecond restart caveat, informal `"knobs"`, and repeated `"shutdown"`.
* **Rationale:** Tightens engineering prose and removes speculative caveats.
* ***AI Suggestion:*** `PROPOSE FIX`
* ***Justification:*** Copy-editing cleanup improves clarity and precision.
* ***Proposed Edit:***
  * Location: `Detailed Subsystem Mechanisms`, `Risks & Limits`, and `Detailed Implementation Plan`.
  * Change: Apply copy edits across the cited lines.
