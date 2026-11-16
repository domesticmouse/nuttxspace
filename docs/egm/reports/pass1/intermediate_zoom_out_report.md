# Stage 8: Intermediate Zoom Out Review Report (`intermediate_zoom_out_report.md`)

**Target Document**: [xiao-rp2350-bargraph.md](../../designs/xiao-rp2350-bargraph.md) (Full File: Lines 1–693; `FeatureRange`: Lines 1–402, `DetailedRange`: Lines 403–693)
**Reviewer**: `egm-zoom-out` (Skeptical Architect — Intermediate Review)

---

### Architectural & Boundary Assessment Summary

The high-level out-of-tree architecture (`CONFIG_ARCH_BOARD_CUSTOM` paired with `CONFIG_ARCH_BOARD_COMMON=y` and a build-time `apps/external` symlink) is well-suited to the root problem of keeping `nuttx/` and `apps/` clean while starting `bargraphd` before `nsh_waitusbready()` blocks on USB CDC/ACM. However, auditing the full document alongside the Stage 5, Stage 6, and Stage 7 reports exposes two blockers that span the `FeatureRange` / `DetailedRange` boundary, three opportunities to eliminate dead files and unnecessary driver workarounds, and an altitude drop where low-level C structs and bit expressions are duplicated across `FeatureRange` and `DetailedRange`.

---

### Findings

#### 1. Unsynchronized ADC Pacing Mitigation Across `FeatureRange` and `DetailedRange` (`RP23XX_ADC_DIV` vs. `START_ONCE` ISR Loop)
* **Severity**: 🔴 Blocker
* **Affected Scope**:
  * `FeatureRange`: `## The Problem` ([line 25](../../designs/xiao-rp2350-bargraph.md#L25)), `## Goals` ([line 33](../../designs/xiao-rp2350-bargraph.md#L33)), `### 3. Boot Sequence` ([line 220](../../designs/xiao-rp2350-bargraph.md#L220)), `### 4.1 ADC Subsystem Mitigations` ([lines 242–250](../../designs/xiao-rp2350-bargraph.md#L242-L250)), `## Non-Functional Requirements` ([lines 377–379](../../designs/xiao-rp2350-bargraph.md#L377-L379)).
  * `DetailedRange`: `#### 13. xiao-rp2350-bargraph/board/src/README.md` ([line 509](../../designs/xiao-rp2350-bargraph.md#L509)), `#### 17. xiao-rp2350-bargraph/board/src/rp23xx_bringup.c` ([lines 538–542](../../designs/xiao-rp2350-bargraph.md#L538-L542)), `#### 30. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c` ([lines 660–662](../../designs/xiao-rp2350-bargraph.md#L660-L662)).
* **Simplicity Rationale**:
  As identified in Stage 5 (Part 1 #1, Part 2 #1) and Stage 7 (Part 1 #1), [rp23xx_adc.c](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L169-L320) never sets `RP23XX_ADC_CS_START_MANY`; instead, `interrupt_handler()` calls `get_next_channel()`, which writes `RP23XX_ADC_CS_START_ONCE` (`2 µs` conversion) after every sample. Consequently, writing `putreg32(0x00ffff00, RP23XX_ADC_DIV)` in `rp23xx_bringup.c` has no effect on the `500,000 IRQ/s` interrupt storm.
  * **Simplicity Evaluation of Proposed Mitigations**: The Stage 5 / Stage 7 mitigation of gating `RP23XX_ADC_IRQ_FIFO` via `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` around each 50 Hz burst in `bargraph_sample_adc()` (keeping `/dev/adc0` open, flushing with `ANIOC_RESET_FIFO`, enabling the IRQ, reading 5 samples, discarding the 1st sample latched at the end of the prior burst, averaging the 4 fresh samples, and disabling the IRQ) is simple and avoids adding any new driver files.
  * **Boundary Requirement**: Stage 7 only audited `FeatureRange` (lines 1–402). When applying this fix, `putreg32(0x00ffff00, RP23XX_ADC_DIV)` must also be removed from `DetailedRange` Target Files #13 and #17 ([lines 509, 538, 542](../../designs/xiao-rp2350-bargraph.md#L509-L542)), and the NVIC IRQ gating + 5-read (discard first, average 4) sequence must be reflected in Target File #30 ([lines 660–662](../../designs/xiao-rp2350-bargraph.md#L660-L662)).

#### 2. Undefined `CONFIG_PWM_MULTICHAN=y` Symbol Remains in `DetailedRange` (`defconfig`, Line 457)
* **Severity**: 🔴 Blocker
* **Affected Scope**:
  * `FeatureRange`: `### 4.2 Hardware PWM Subsystem Mitigations` ([line 260](../../designs/xiao-rp2350-bargraph.md#L260)).
  * `DetailedRange`: `#### 7. xiao-rp2350-bargraph/board/configs/bargraph/defconfig` ([line 457](../../designs/xiao-rp2350-bargraph.md#L457)).
* **Simplicity Rationale**:
  Stage 7 (Part 1 #2) flagged that `CONFIG_PWM_MULTICHAN=y` on line 260 does not exist in [drivers/timers/Kconfig](../../../../nuttx/drivers/timers/Kconfig#L37-L71) and causes `olddefconfig` in [tools/Unix.mk](../../../../nuttx/tools/Unix.mk#L725-L734) to abort with `exit 1`. Both line 260 and line 457 must be updated to list only `CONFIG_PWM=y`, `CONFIG_PWM_NCHANNELS=2`, `CONFIG_RP23XX_PWM=y`, `CONFIG_RP23XX_PWM_MULTICHAN=y`, and `CONFIG_RP23XX_PWM_NCHANNELS=2`.

#### 3. Dead Copy-Pasted Board Files (`rp23xx_autoleds.c`, `rp23xx_userleds.c`, `rp23xx_gpio.c`) and Pin-Conflict Hazard
* **Severity**: 🟡 Major
* **Affected Scope**:
  * `FeatureRange`: `### 1. System Architecture & Out-of-Tree Build Topology` ([lines 108–110](../../designs/xiao-rp2350-bargraph.md#L108-L110)).
  * `DetailedRange`: Target Files #13, #14, #18, #19, and #20 ([lines 511–520, 545–565](../../designs/xiao-rp2350-bargraph.md#L511-L565)).
* **Simplicity Rationale**:
  In [nuttx/boards/Kconfig](../../../../nuttx/boards/Kconfig#L4077-L4120), `CONFIG_ARCH_BOARD_CUSTOM=y` does not select `ARCH_HAVE_LEDS` unless `CONFIG_BOARD_CUSTOM_LEDS=y` is enabled, and neither `CONFIG_USERLED` nor `CONFIG_DEV_GPIO` is enabled in `defconfig`. Nothing in the kernel or `boards/arm/rp23xx/common` calls `board_autoled_*`, `board_userled_*`, or `rp23xx_dev_gpio_init()` when those Kconfig options are off. Furthermore, `xiao-rp2350/src/rp23xx_gpio.c` hardcodes `GPIO26`, `GPIO27`, and `GPIO28`—the exact pins used by `ADC0`, `LED1`, and `LED2`.
  Omitting `rp23xx_autoleds.c`, `rp23xx_userleds.c`, and `rp23xx_gpio.c` from `xiao-rp2350-bargraph/board/src/` (and setting `CSRCS = rp23xx_boardinitialize.c rp23xx_bringup.c` in `board/src/Make.defs`) removes 3 unused files (~750 lines of dead code) and eliminates a latent pin-mux collision.

#### 4. Unnecessary 4-Step `bargraph_shutdown_pwm()` Workaround (`close(fd)` Already Drives Pins Low) and Redundant IPC Condition Variables
* **Severity**: 🟡 Major
* **Affected Scope**:
  * `FeatureRange`: `## The Problem` (Item 5, [line 23](../../designs/xiao-rp2350-bargraph.md#L23)), `### 4.2 Hardware PWM Subsystem Mitigations` (Item 4, [lines 279–280](../../designs/xiao-rp2350-bargraph.md#L279-L280)), `### 5. Daemon State Machine & IPC Synchronization` ([lines 286–344](../../designs/xiao-rp2350-bargraph.md#L286-L344)).
  * `DetailedRange`: Target File #29 ([lines 638–639](../../designs/xiao-rp2350-bargraph.md#L638-L639)) and Target File #30 ([lines 658–666](../../designs/xiao-rp2350-bargraph.md#L658-L666)).
* **Simplicity Rationale**:
  1. **PWM Shutdown**: `close(fd)` in [pwm.c](../../../../nuttx/drivers/timers/pwm.c#L235) already invokes `pwm_shutdown()` in [rp23xx_pwm.c](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c#L249-L318), which calls `pwm_stop()`, configures `pin[0]` and `pin[1]` as outputs driven low (`0`), switches their function mux to `RP23XX_GPIO_FUNC_SIO`, and zeros `priv->duty[]`. Calling `PWMIOC_STOP` (so `upper->started` is reset to `false` in `drivers/timers/pwm.c:428`) followed directly by `close(fd)` drives all 6 LED pins low immediately without needing zero-duty writes, `PWMIOC_START`, and `usleep(2000)`.
  2. **IPC Synchronization**: Polling shared state once per `20 ms` (`50 Hz`) under a single `pthread_mutex_t lock` eliminates `wake_cond` and `done_cond`.

#### 5. `FeatureRange` vs. `DetailedRange` Altitude Boundary & `apps/external` `distclean` Hygiene
* **Severity**: 🟡 Major
* **Affected Scope**:
  * `FeatureRange`: `### 1. System Architecture` ([lines 141–150](../../designs/xiao-rp2350-bargraph.md#L141-L150)), `### 5. Daemon State Machine & IPC Synchronization` ([lines 303–332](../../designs/xiao-rp2350-bargraph.md#L303-L332)).
  * `DetailedRange`: Target File #11 ([lines 491–498](../../designs/xiao-rp2350-bargraph.md#L491-L498)), Target File #23 ([lines 580–586](../../designs/xiao-rp2350-bargraph.md#L580-L586)), Target File #29 ([lines 622–640](../../designs/xiao-rp2350-bargraph.md#L622-L640)).
* **Simplicity Rationale**:
  1. **Altitude Drop in `FeatureRange`**: `### 5. Daemon State Machine & IPC Synchronization` ([lines 303–332](../../designs/xiao-rp2350-bargraph.md#L303-L332)) embeds the full C declaration of `enum bargraph_mode_e` and `struct bargraph_state_s`, which is repeated in `DetailedRange` Target File #29 (`bargraph_internal.h`, [lines 622–640](../../designs/xiao-rp2350-bargraph.md#L622-L640)). Summarize the shared state invariants in prose/table form in `FeatureRange` Section 5 and keep the concrete C struct definition in `DetailedRange` Target File #29.
  2. **Cross-Boundary Synchronization of `apps/external` `distclean` Guard**: Ensure the `MAKECMDGOALS` guard (`ifeq ($(filter distclean clean_context apps_distclean,$(MAKECMDGOALS)),)`) and `distclean` symlink removal are applied consistently in both `FeatureRange` (Section 1, lines 141–150) and `DetailedRange` (Target File #11 `board/scripts/Make.defs` at line 496 and Target File #23 `apps/Makefile` at line 585).

#### 6. Avoid Premature PWM Duty Memoization; Keep Single `PWMIOC_START` on Initialization
* **Severity**: 🟢 Nit
* **Affected Scope**:
  * `FeatureRange`: `### 4.2 Hardware PWM Subsystem Mitigations` ([lines 267–272](../../designs/xiao-rp2350-bargraph.md#L267-L272)).
  * `DetailedRange`: `#### 30. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c` (`bargraph_apply_pwm`, [lines 649–657](../../designs/xiao-rp2350-bargraph.md#L649-L657)).
* **Simplicity Rationale**:
  In [rp23xx_pwm.c](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c#L335-L375), `pwm_start()` already skips `setup_period()` when `priv->frequency == info->frequency` and only writes `RP23XX_PWM_CC` and `RP23XX_PWM_CSR` (`< 5 µs` total across all 5 slices at 50 Hz in flat-build mode). Simply calling `PWMIOC_START` once when the 5 PWM descriptors are opened and issuing `PWMIOC_SETCHARACTERISTICS` on each 50 Hz tick (which automatically calls `lower->ops->start()` when `upper->started` is true, per [pwm.c:371-374](../../../../nuttx/drivers/timers/pwm.c#L371-L374)) keeps `bargraph_apply_pwm()` stateless.
