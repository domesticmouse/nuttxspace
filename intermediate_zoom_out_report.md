# Goldfish Review Report — Stage 8: Intermediate Zoom Out Review

**Target:** `docs/designs/xiao-rp2350-bargraph.md` (Lines 1–440, Full File)

## Reviewer Status

| Reviewer | Status |
| --- | --- |
| `egm-zoom-out` | completed |

## Baseline Status

`docs/designs/xiao-rp2350-bargraph.md` (lines 1–440) has not yet been revised to address Stage 5 (`zoom_out_report.md`), Stage 6 (`comprehension_report.md`), or Stage 7 (`critic_report.md`). All prior findings—including Critic Blocker #1 (missing `## Risks & Limits` section) and Stage 5 Major findings #1–#5—remain open in the draft.

## Prioritized Findings (Blockers -> Major -> Nits)

### 1. Applying Stage 6/7 Fault-Recovery Mitigations on Top of the Unnecessary Fault-Blink Sub-Loop Compounds YAGNI Over-Engineering - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Goals` (line 34), `CUJ 3` (lines 61–65), `Daemon Lifecycle` (line 184), `Detailed Implementation Plan` Item 16 (`bargraph_main.c`, lines 411–417), `Verification Plan` (line 436); `zoom_out_report.md` (#2, #6) and `critic_report.md` (#3).
* **Simplicity Rationale:** Stage 5 (#2) already flagged that building a consecutive-error counter (`>= 5` failures, filtering `-EAGAIN`) and a separate 250 ms `0x15`/`0x2A` `fault_blink_loop` for an on-chip `/dev/adc0` device that cannot hot-unplug is speculative complexity. Layering Stage 5 Catalyst (#6) and Critic (#3) mitigations onto it—periodic ADC re-probing from the fault loop, consecutive-valid-sample recovery hysteresis, edge-triggered/rate-limited `syslog` to protect the 4 KB RAMLOG buffer, `EINTR` filtering, and short-read handling—transforms a simple 20 Hz polling loop into a multi-state recovery supervisor. Removing the fault-blink state machine altogether (log `syslog(LOG_ERR, ...)` on failure and either exit or retry on the regular 50 ms tick) eliminates the entire sub-loop and all of its secondary state-machine mitigations at once.

### 2. Retaining `task_create()` Singleton Boilerplate Forces Cascading Signal-Race and FD-Inheritance Complexity - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Goals` (line 33), `CUJ 2` (lines 57–59), `System Data Flow` (lines 142–148), `Daemon Lifecycle` (lines 181–182), `etc/init.d/rcS` (line 360), `defconfig` (line 369), `bargraph_main.c` (lines 403–420); `comprehension_report.md` (#1) and `critic_report.md` (#6, #7).
* **Simplicity Rationale:** Copying `task_create()` + static `g_led_daemon_started` from `apps/examples/leds/leds_main.c` (while also leaving `# CONFIG_NSH_DISABLEBG is not set` enabled in `defconfig`) directly triggers three Stage 6/7 complications:
  1. Having to document NuttX's flat shared address space for `static` singleton flags (`comprehension_report.md` #1).
  2. Inheriting `/dev/ttyACM0` FDs `0..2` when `bargraph` is restarted from USB NSH, requiring explicit `/dev/null` FD redirection in `bargraph_daemon` (`critic_report.md` #6).
  3. Fixing a shutdown/restart race by splitting `g_bargraph_started` into `volatile sig_atomic_t` stop-requested vs. active-daemon flags (`critic_report.md` #7).
  Running `bargraph` as a plain single-task loop launched in the background from `rcS` (`bargraph &`, leveraging NSH's built-in backgrounding already enabled in `defconfig`) eliminates `task_create()`, the singleton global state, and the shutdown/restart flag race—unless the CUJ 2 duplicate-invocation notice is an explicit requirement.

### 3. Unremoved Cargo-Culted Boilerplate (`board/Kconfig`, `rp23xx_pico.h`, Unused Headers/Macros/Kconfig) and Risk of Documenting Dead Code - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `board/Kconfig` (lines 271–276), `board/include/board.h` (lines 283–286), `board/scripts/Make.defs` (lines 295, 297), `board/src/rp23xx_pico.h` (lines 309–316), `board/src/rp23xx_boardinitialize.c` (line 323), `external/` Makefile/Kconfig (lines 266, 373–389), `bargraph_main.c` (`ULEDIOC_SUPPORTED`, line 415); `zoom_out_report.md` (#3–#5) and `comprehension_report.md` (#7, #8).
* **Simplicity Rationale:**
  * **Unnecessary `board/Kconfig`**: Item 2 (lines 271–276) claims `board/Kconfig` is required because `nuttx/tools/Config.mk` binds `BOARD_KCONFIG = $(BOARD_DIR)/Kconfig`. In `nuttx/tools/Config.mk` (lines 176–180), when `$(BOARD_DIR)/Kconfig` is absent, `BOARD_KCONFIG` automatically falls back to `boards/dummy/dummy_kconfig`. Creating an empty `board/Kconfig` stub is unnecessary.
  * **Unnecessary `board/src/rp23xx_pico.h`**: Item 6 (lines 309–316) creates an internal header just to hold 7 pin constants and `rp23xx_bringup()`, scattering LED/ADC definitions across three files (`board.h`, `rp23xx_pico.h`, `rp23xx_userleds.c`). Defining `GPIO_LED1..6` locally in `rp23xx_userleds.c` (matching `g_ledcfg[]`) and clearing `GPIO 26–29` in `rp23xx_bringup.c` removes an unneeded file.
  * **Unresolved Stage 5 deletions vs. Stage 6 documentation requests**: Unused `rp23xx_i2cdev.h`/`spidev.h`/`i2sdev.h`/`spisd.h` includes, 12 unused `BOARD_LED*` macros, `CONFIG_RP23XX_PSRAM`/`rp23xx_psramconfig()`, `CONFIG_BOOT_COPYTORAM`/`memmap_no_flash.ld`/`NXFLATLDFLAGS*`, 7 `external/bargraph/Kconfig` knobs, hand-written `external/Kconfig` + Makefile timestamp cleanup (conflicting with `apps/Directory.mk` + `apps/Make.defs` line 35), and the dead `ULEDIOC_SUPPORTED` ioctl remain in the draft. Delete these artifacts rather than adding prose to explain them per `comprehension_report.md` (#6–#8).

### 4. High-Altitude Sections Remain in the Weeds; Stage 6/7 Explanations Should Be Satisfied by Raising Altitude, Not Adding Low-Level Prose - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Goals` (lines 28–35), `CUJ 1–3` (lines 51–66), `System Data Flow and Boot Sequence` (lines 127–161), `Detailed Subsystem Mechanisms` (lines 163–184), `Detailed Implementation Plan` Items 8, 9, 16 (lines 331–346, 398–420); `zoom_out_report.md` (#1), `comprehension_report.md` (#3, #4), `critic_report.md` (#14).
* **Simplicity Rationale:** High-level sections still contain C register calls (`clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE, ...)`), driver-internal symbols (`my_reset()`, `my_shutdown()`, `remove_device()`, `g_active_count`), byte sizes (`sizeof(struct adc_msg_s)`), bit shifts (`>> 19`), and helper decomposition (`read_adc_sample`, `quantize_level`, `{ 585, ... }`). Meanwhile, Stage 6 (#3, #4) flagged those ungrounded tokens as incomprehensible to a new reader. Attempting to satisfy Stage 6 by adding explanations of `rp23xx_adc.c` internal helpers and `clrbits_reg32` parameter order inside `Overview`/`Goals`/`System Data Flow` would bloat the architecture sections further. Instead, lift the high-level sections to component boundaries and observable contracts (fixing `critic_report.md` #14 repetition at the same time), and restrict register/API details to the implementation plan—without prescribing internal private helper functions or literal tuning arrays.

### 5. Hardware Noise Mitigation Should Not Introduce a Software Multi-Sample Filter - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `ADC Sampling, Normalization, and Hysteresis` (lines 175–177) and `critic_report.md` (#9).
* **Simplicity Rationale:** Critic #9 notes that single-sample reads with `HYSTERESIS=24` can flicker under USB VBUS ripple or wiper bounce *"without an RC bypass or multi-sample filter"*. Adding a software multi-sample filter (either multiple `open`/`read`/`close` cycles per 50 ms tick or keeping `/dev/adc0` open longer with an IIR/median buffer) would complicate the minimal single-sample per-tick design. Address Critic #9 through hardware guidance (LED current-limiting resistor sizing, optional RC bypass capacitor on `D0`, and documenting the deliberate `3534..4095` saturation band) rather than adding software filtering state.

### 6. Minor Accuracy / Decisiveness Nits on `rc.sysinit` and Unused `usbnsh` Test Apps - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `board/src/etc/init.d/rc.sysinit` (line 350) and `board/configs/bargraph/defconfig` (line 371).
* **Simplicity Rationale:**
  * Line 350 states `RCSRCS` in `board/src/Make.defs` *"requires both `etc/init.d/rc.sysinit` and `etc/init.d/rcS`"*; `nuttx/boards/Board.mk` (lines 21–33) only processes what `RCSRCS` lists, and `apps/nshlib/nsh_init.c` (lines 151, 163) runs them separately (keeping `rc.sysinit` is harmless if matching upstream `xiao-rp2350`, just state the actual reason).
  * Line 371 says to remove `CONFIG_EXAMPLES_HELLO`, `CONFIG_TESTING_GETPRIME`, and `CONFIG_TESTING_OSTEST` *"if unused"*—decide now and omit them outright since none are used by this firmware.
