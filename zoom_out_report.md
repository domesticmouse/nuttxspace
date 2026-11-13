# Goldfish Review Report — Stage 5: Initial Zoom Out Review

**Target:** `docs/designs/xiao-rp2350-bargraph.md` (Lines 1–440, Full File)

## Reviewer Status

| Reviewer | Status |
| --- | --- |
| `egm-zoom-out` | completed |
| `egm-inventive-catalyst` | completed |

## Prioritized Findings (Blockers -> Major -> Nits)

### 1. Premature Implementation Details (Altitude Drops) Across High-Level Sections and Plan - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Goals` (lines 28–35), `Critical User Journeys` (lines 53, 65), `System Data Flow and Boot Sequence` (lines 134–159), and `Detailed Implementation Plan` items 8, 9, and 16 (lines 331–346, 398–420).
* **Discrepancy Description**: High-level sections specify C function calls, register macros (`clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE)`), struct sizes (`sizeof(struct adc_msg_s)`), bit-shift counts (`>> 19`), and hex fault masks (`0x15` / `0x2A`) instead of system outcomes and component boundaries. Item 16 (`bargraph_main.c`, lines 403–420) prescribes private static variable names (`g_bargraph_running`), internal helper decomposition (`sigterm_handler`, `read_adc_sample`, `quantize_level`, `fault_blink_loop`), loop expressions, and literal tuning constants (`{ 585, 1170, 1755, 2340, 2925, 3510 }`, `5` consecutive errors, `250000` us). Items 8–9 list C statements line-by-line rather than component responsibilities.
* **Rationale & Recommendation**: Per the Zoom-Out Standard (Section 3: *Anti-Weeds*), high-level sections should focus on objectives, architectural boundaries, and data flows, leaving low-level helper internals and literal expressions out of the architecture and plan overview.

### 2. YAGNI Violation: Speculative Fault-Blink Sub-Loop and Consecutive-Error State Machine - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Goals` (line 34), `CUJ 3` (lines 61–65), `Daemon Lifecycle` (line 184), `bargraph_main.c` (`fault_blink_loop` and consecutive-error counter, lines 411–416), and `Verification Plan` item 4 (line 436).
* **Discrepancy Description**: `/dev/adc0` is an on-chip RP2350 peripheral registered at boot by `rp23xx_common_bringup()` in a single flat firmware image and cannot hot-unplug at runtime. Building a multi-state consecutive-error counter (`>= 5` failures, filtering transient `-EAGAIN`) and a dedicated 250 ms alternating `0x15` / `0x2A` blink loop (`fault_blink_loop`) solely to surface a build/configuration mistake adds speculative complexity.
* **Rationale & Recommendation**: Logging the error via `syslog(LOG_ERR, ...)` and exiting (or retrying on the regular 50 ms tick) is simpler than maintaining a separate fault-blink state machine.

### 3. Cargo-Culted and Unused Board / Kconfig Boilerplate - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `board/include/board.h` (lines 283–286), `board/src/rp23xx_boardinitialize.c` (line 323), `board/scripts/Make.defs` (lines 295, 297), and `external/bargraph/Kconfig` (lines 382–389).
* **Discrepancy Description**: Several upstream `xiao-rp2350` and `apps/examples/leds` artifacts are carried over without being used:
  * `board/include/board.h` includes `"rp23xx_i2cdev.h"`, `"rp23xx_spidev.h"`, `"rp23xx_i2sdev.h"`, and `"rp23xx_spisd.h"`, claiming `rp23xx_common_bringup.c` requires them. Those headers live in `nuttx/boards/arm/rp23xx/xiao-rp2350/include/` (not `common/`) and are only used when `CONFIG_RP23XX_I2C_DRIVER`, `SPI_DRIVER`, `I2S`, or `SPISD` are enabled, none of which are enabled in `bargraph/defconfig`. Including them out-of-tree breaks the build unless 4 unused headers are copied.
  * `board/include/board.h` defines 12 unused LED macros (`BOARD_LED1`..`BOARD_LED6`, `BOARD_LED1_BIT`..`BOARD_LED6_BIT`), while `rp23xx_userleds.c` uses `BOARD_NLEDS` + `g_ledcfg[]` and `rp23xx_pico.h` defines `GPIO_LED1`..`GPIO_LED6`.
  * `rp23xx_boardinitialize.c` includes `CONFIG_RP23XX_PSRAM` (`rp23xx_psramconfig()`) and `board/scripts/Make.defs` includes `CONFIG_BOOT_COPYTORAM`, `memmap_no_flash.ld`, and `NXFLATLDFLAGS*`, none of which are used.
  * `external/bargraph/Kconfig` exposes 7 Kconfig knobs (`BARGRAPH_PROGNAME`, `PRIORITY`, `STACKSIZE`, `ADC_DEVPATH`, `LED_DEVPATH`, `INTERVAL_US`, `HYSTERESIS`) modeled on multi-board `apps/examples/leds/Kconfig`, even though `Non-Goals` restricts the project to XIAO RP2350 and board bringup hardcodes `/dev/adc0` and `/dev/userleds`.
* **Rationale & Recommendation**: Remove the unused includes, macros, PSRAM/NXFLAT boilerplate, and speculative Kconfig symbols; use a single `config BARGRAPH` (`bool`) with compile-time constants in `bargraph_main.c`.

### 4. Redundant `ULEDIOC_SUPPORTED` Query in `bargraph_main.c` - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Implementation Plan` Item 16 (`bargraph_main.c`, line 415).
* **Discrepancy Description**: Querying `ioctl(led_fd, ULEDIOC_SUPPORTED, ...)` and masking to `0x3f` is copied from `apps/examples/leds/leds_main.c` (lines 123–138). The custom board defines `BOARD_NLEDS = 6` (`userled_lower.c` returns `(1 << 6) - 1 = 0x3f`), and line 416 writes `(1U << level) - 1U` directly without consuming the queried mask.
* **Rationale & Recommendation**: Drop the unused `ULEDIOC_SUPPORTED` query rather than duplicating multi-board probing logic from `apps/examples/leds`.

### 5. Conflicting `apps/external` Kconfig Plumbing and Custom Timestamp Cleanup - Severity: Major
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `Detailed Implementation Plan` Item 1 (`Makefile`, line 266) and Item 13 (`external/Makefile`, `external/Make.defs`, `external/Kconfig`, lines 373–381).
* **Discrepancy Description**: Item 13 combines two conflicting NuttX `apps/` mechanisms: `external/Makefile` sets `MENUDESC = "External"` and includes `$(APPDIR)/Directory.mk` (which auto-generates `external/Kconfig` via `apps/tools/mkkconfig.sh` during `preconfig` and deletes it on `distclean`, per `apps/Directory.mk` lines 50–63), while simultaneously proposing a hand-written `external/Kconfig`. Per `apps/Make.defs` line 35, a pre-existing `external/Kconfig` causes `external/` to be filtered out of `CONFIGDIRS` during `preconfig`, yet `Directory.mk`'s `distclean` still deletes it.
* **Rationale & Recommendation**: Omit the hand-written `external/Kconfig` so `Directory.mk` generates and cleans it automatically, and remove the custom `Kconfig` timestamp-deletion logic from `xiao-rp2350-bargraph/Makefile`.

### 6. One-Way Fault Latch Contradicts Autonomous Availability (False Compromise) - Severity: Nit
* **Reviewer Lens:** `egm-inventive-catalyst`
* **Affected Scope:** `CUJ 3: Headless Hardware or Driver Fault Indication` (lines 61–66), `Daemon Lifecycle, ROMFS Startup, and Headless Diagnostics` (line 184), and `16. xiao-rp2350-bargraph/external/bargraph/bargraph_main.c` (`fault_blink_loop` / `bargraph_daemon`, lines 411–417).
* **Discrepancy Description**: After 5 consecutive `/dev/adc0` `open()`/`read()` failures, `bargraph_daemon` exits its sampling loop and enters a one-way `fault_blink_loop(led_fd)` until `SIGTERM`. On a standalone board with no USB host attached, a transient failure leaves the device latched in the fault blink state until a user connects NSH or power-cycles the board.
* **Rationale & Recommendation**: If fault blinking is retained, apply **Self-Service** and **Blessing in Disguise** by re-probing `read_adc_sample()` periodically from the fault-blink cadence and resuming 20 Hz tracking on recovery. *Secondary Liabilities*: Rate-limit or edge-trigger `syslog(LOG_ERR, ...)` so probes do not wrap the 4 KB `CONFIG_RAMLOG_BUFSIZE=4096` buffer, require consecutive valid samples before exiting fault state to prevent oscillation, and accept added state-transition logic in `bargraph_daemon`.

### 7. Unnecessary NSH Backgrounding Config / `task_create()` Duplication - Severity: Nit
* **Reviewer Lens:** `egm-zoom-out`
* **Affected Scope:** `etc/init.d/rcS` (line 360), `board/configs/bargraph/defconfig` (line 369), and `bargraph_main.c` (lines 418–420).
* **Discrepancy Description**: `defconfig` sets `# CONFIG_NSH_DISABLEBG is not set` to allow `&` backgrounding, yet `rcS` invokes `bargraph` without `&` because `bargraph` spawns `bargraph_daemon` internally via `task_create()` (copied from `apps/examples/leds/leds_main.c`).
* **Rationale & Recommendation**: If the singleton CLI check in CUJ 2 is required, `# CONFIG_NSH_DISABLEBG is not set` is unused; without the singleton check, `rcS` could run `bargraph &` and remove `task_create()` boilerplate from `bargraph_main.c`.
