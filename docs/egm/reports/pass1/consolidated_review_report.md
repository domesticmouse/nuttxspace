# Consolidated EGM Review Report (`consolidated_review_report.md`)

**Target Document**: [xiao-rp2350-bargraph.md](../../designs/xiao-rp2350-bargraph.md)
* **Semantic Split**: `FeatureRange`: Lines 1–402 | `DetailedRange`: Lines 403–693
* **Stage Reports**:
  * **Stage 5 (Initial Zoom Out)**: [zoom_out_report.md](zoom_out_report.md)
  * **Stage 6 (Comprehension Test)**: [comprehension_report.md](comprehension_report.md)
  * **Stage 7 (Critic Review)**: [critic_report.md](critic_report.md)
  * **Stage 8 (Intermediate Zoom Out)**: [intermediate_zoom_out_report.md](intermediate_zoom_out_report.md)
  * **Stage 9 (Implementation Readiness Test)**: [implementation_readiness_report.md](implementation_readiness_report.md)

---

## Consolidated Action Items by Priority

### 🔴 Blockers (Must Fix — Breaks Build, Hardware Behavior, or Core Math)

#### B-1. Replace Ineffective `RP23XX_ADC_DIV` Pacing with NVIC IRQ Gating (or Direct Register Sampling) and Calibrate `rp23xx_adc.c` Claims
* **Affected Lines**: [25](../../designs/xiao-rp2350-bargraph.md#L25), [33](../../designs/xiao-rp2350-bargraph.md#L33), [220](../../designs/xiao-rp2350-bargraph.md#L220), [242–250](../../designs/xiao-rp2350-bargraph.md#L242-L250), [378](../../designs/xiao-rp2350-bargraph.md#L378), [391](../../designs/xiao-rp2350-bargraph.md#L391), [509](../../designs/xiao-rp2350-bargraph.md#L509), [538–542](../../designs/xiao-rp2350-bargraph.md#L538-L542), [660–662](../../designs/xiao-rp2350-bargraph.md#L660-L662)
* **Issue**: The design assumes [rp23xx_adc.c](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L169-L320) uses continuous free-running mode (`ADC_CS_START_MANY`) and that writing `putreg32(0x00ffff00, RP23XX_ADC_DIV)` paces conversions to `~732.42 Hz`. In fact, `rp23xx_adc.c` never sets `START_MANY`; `interrupt_handler()` ([line 234](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L234)) unconditionally calls `get_next_channel()`, which writes single-shot `RP23XX_ADC_CS_START_ONCE` ([line 304](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L304)) even when the upper-half software FIFO is full (`au_receive() != OK`). On RP2350 hardware, `RP23XX_ADC_DIV` only paces `START_MANY` conversions and has **zero effect** on `START_ONCE` writes (`2 µs` per conversion), leaving the `500,000 IRQ/s` storm active whenever `/dev/adc0` is held open. Additionally, the ISR in `rp23xx_adc.c` is `interrupt_handler()` (not `adc_interrupt()`), and `remove_device(dev)` is called from `my_shutdown()` / `my_rxint(dev, false)` ([lines 538, 563](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L538)) while `interrupt_handler()` checks `g_active_count == 0` ([line 176](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L176)).
* **Action**:
  1. Remove `putreg32(0x00ffff00, RP23XX_ADC_DIV)` from `rp23xx_bringup.c` and all prose/diagrams across `FeatureRange` and `DetailedRange`.
  2. In `bargraphd` (`CONFIG_BUILD_FLAT=y`), keep `/dev/adc0` open and gate `RP23XX_ADC_IRQ_FIFO` via `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` (`<nuttx/irq.h>` and `<arch/rp23xx/irq.h>`):
     * Immediately after `open("/dev/adc0", O_RDONLY)`, call `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`.
     * On each 50 Hz burst in `bargraph_sample_adc()`: call `ioctl(adc_fd, ANIOC_RESET_FIFO, 0)`, call `up_enable_irq(RP23XX_ADC_IRQ_FIFO)`, read `5` `struct adc_msg_s` entries (`~10 µs`), call `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`, discard `msgs[0]` (latched at the end of the previous cycle), and average `msgs[1..4]`.
     * Before `close(adc_fd)` on shutdown, call `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` so `interrupt_handler()` executes its `g_active_count == 0` teardown cleanly.
  3. Calibrate the prose description of `interrupt_handler()`, `get_next_channel()`, and `my_shutdown()`.

#### B-2. Fix Fixed-Point Q4 EMA Filter Right-Shift Bug (`>> 2` vs `>> 4`) and Seed First Sample
* **Affected Lines**: [253–254](../../designs/xiao-rp2350-bargraph.md#L253-L254), [662](../../designs/xiao-rp2350-bargraph.md#L662)
* **Issue**: Line 254 defines the EMA update as $\text{ema\_q4} \leftarrow \text{ema\_q4} - (\text{ema\_q4} \gg 2) + (\text{raw\_avg} \ll 2)$ and extracts $\text{filtered} = \text{ema\_q4} \gg 2$. At steady state for constant input $R$, $\text{ema\_q4} = 16 R$ (`Q4`), so `ema_q4 >> 2` equals $4 R \in [0, 16380]$ instead of $R \in [0, 4095]$, saturating all 6 LEDs at 100% duty when the potentiometer reaches 25% of travel.
* **Action**: Extract `filtered` by shifting right by 4 with rounding:
  $$\text{ema\_q4} \leftarrow \text{ema\_q4} - (\text{ema\_q4} \gg 2) + (\text{raw\_avg} \ll 2), \quad \text{filtered} = (\text{ema\_q4} + 8) \gg 4$$
  and explicitly specify in Target File 30 (`bargraph_sample_adc`) that on the first sample after daemon startup (`!ema_initialized`), `ema_q4` is seeded to `raw_avg << 4`.

#### B-3. Remove Non-Existent `CONFIG_PWM_MULTICHAN=y` and Add Required `CONFIG_ANALOG=y` in `defconfig`
* **Affected Lines**: [260](../../designs/xiao-rp2350-bargraph.md#L260), [457](../../designs/xiao-rp2350-bargraph.md#L457), [464](../../designs/xiao-rp2350-bargraph.md#L464)
* **Issue**:
  1. `CONFIG_PWM_MULTICHAN` does not exist in [nuttx/drivers/timers/Kconfig](../../../../nuttx/drivers/timers/Kconfig#L37-L71) (only `CONFIG_PWM=y` and `CONFIG_PWM_NCHANNELS=2` exist in generic timer Kconfig, alongside `CONFIG_RP23XX_PWM=y`, `CONFIG_RP23XX_PWM_MULTICHAN=y`, and `CONFIG_RP23XX_PWM_NCHANNELS=2` in `arch/arm/src/rp23xx/Kconfig`). In [nuttx/tools/Unix.mk](../../../../nuttx/tools/Unix.mk#L725-L734), `olddefconfig` aborts with `exit 1` on any unknown Kconfig symbol warning.
  2. In [nuttx/drivers/analog/Kconfig](../../../../nuttx/drivers/analog/Kconfig#L6-L20), `config ADC` is gated inside `if ANALOG`. Without `CONFIG_ANALOG=y` in `defconfig` (line 464), `CONFIG_ADC=y` is dropped during `olddefconfig` and `/dev/adc0` is never compiled or registered.
* **Action**: Remove `CONFIG_PWM_MULTICHAN=y` from lines 260 and 457, and add `CONFIG_ANALOG=y` to line 464.

#### B-4. Calibrate `rp23xx_adc.c` (`my_reset()`), `rp23xx_pwm.c` (`setup_period()`, `pwm_stop()`, `pin` signedness), `drivers/timers/pwm.c`, and `nsh_main.c` Codebase Claims
* **Affected Lines**: [21–24](../../designs/xiao-rp2350-bargraph.md#L21-L24), [236–237](../../designs/xiao-rp2350-bargraph.md#L236-L237), [266–270](../../designs/xiao-rp2350-bargraph.md#L266-L270), [279–280](../../designs/xiao-rp2350-bargraph.md#L279-L280), [364](../../designs/xiao-rp2350-bargraph.md#L364)
* **Issue**:
  1. `my_reset()` in [rp23xx_adc.c:466-483](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L466-L483) loops unconditionally over all 4 ADC GPIOs (`26..29`), setting them to `RP23XX_GPIO_FUNC_NULL` during `adc_register()`. Meanwhile, `rp23xx_pwm_initialize()` does not mux GPIOs during `rp23xx_common_bringup()`; `pwm_setup()` ([rp23xx_pwm.c:214-222](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c#L214-L222)) sets `RP23XX_GPIO_FUNC_PWM` later when `bargraphd` opens `/dev/pwm5` and `/dev/pwm6`. Disabling `CONFIG_RP23XX_ADC_CHANNEL1..3` and `TEMPERATURE` is required so `get_next_channel()` ([rp23xx_adc.c:267](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L267)) only samples channel 0 instead of round-robin multiplexing channels 1–4 into `/dev/adc0`.
  2. `nuttx/drivers/timers/pwm.c` copies `struct pwm_info_s` via `memcpy` without inspecting `channel`; the `channel == 0` (skip) / `-1` (terminate) sentinel is the contract in [include/nuttx/timers/pwm.h:67-70](../../../../nuttx/include/nuttx/timers/pwm.h#L67-L70).
  3. In `setup_period()` ([rp23xx_pwm.c:486-530](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c#L486-L530)), `info.frequency = 1000` at `150 MHz` yields `DIV = 36` (`0x24`, divider `2.25` in 8.4 fixed-point) and `TOP = 65535` (`0xffff`), not `DIV = 16, TOP = 9374`.
  4. In [rp23xx_pwm.h:53](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_pwm.h#L53), `priv->pin[i]` is signed `int8_t`, and `pwm_setup()` checks `if (priv->pin[i] >= 0)` (not `>= 30` unsigned).
  5. `pwm_stop()` clears `1 << priv->num` in `RP23XX_PWM_EN` via `clear_enabled()` ([rp23xx_pwm.c:643-650](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c#L643-L650)), whereas `pwm_shutdown()` on `close()` ([rp23xx_pwm.c:249-318](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c#L249-L318)) switches the pins to `RP23XX_GPIO_FUNC_SIO` and drives them low (`0`).
  6. In [apps/system/nsh/nsh_main.c:71-76](../../../../apps/system/nsh/nsh_main.c#L71-L76), `nsh_initialize()` (which invokes `nsh_initscript()`) runs before `nsh_consolemain()` (`nsh_usbconsole.c`), whereas `nsh_loginscript()` runs after `nsh_waitusbready()`, and when `CONFIG_DEV_CONSOLE` is disabled (`usbnsh`), `nsh_initialize()` runs before USB CDC/ACM stdio is opened while `Board.mk` requires host `genromfs`/`xxd`.
* **Action**: Update these 6 codebase descriptions to match the exact upstream C implementation.

---

### 🟡 Majors (Architectural Simplifications, Comprehension Gaps, & Implementation Readiness Completeness)

#### M-1. Define "EGM" and Link to Local EGM `README.md` Structural Conventions on First Use
* **Affected Lines**: [36](../../designs/xiao-rp2350-bargraph.md#L36), [409–600](../../designs/xiao-rp2350-bargraph.md#L409-L600)
* **Issue**: The document refers to "EGM `README.md` contracts", "EGM Blueprint Documentation", "Leaf EGM Documentation", and `[file:symbol]` references without defining "EGM" or linking to existing workspace exemplars (`README.md`, `nuttx/README.md`, `apps/README.md`).
* **Action**: Define EGM documentation contracts on first mention in `## Goals` (line 36) and reference the workspace root [README.md](../../README.md) and leaf exemplars (such as [nuttx/boards/arm/rp23xx/xiao-rp2350/src/README.md](../../../../nuttx/boards/arm/rp23xx/xiao-rp2350/src/README.md)) so any reader understands the non-leaf (`Overview`, `Subdirectories & Boundaries`, `Invariants`, `Side Effects`, `Verification`) and leaf (`Overview`, `Interface`, `Invariants`, `Side Effects`, `Verification`) layout and `[file:symbol]` citation format.

#### M-2. Guard `apps/external` Symlink Creation During `distclean` and Remove Symlink in `apps/Makefile`
* **Affected Lines**: [141–150](../../designs/xiao-rp2350-bargraph.md#L141-L150), [384](../../designs/xiao-rp2350-bargraph.md#L384), [496](../../designs/xiao-rp2350-bargraph.md#L496), [585](../../designs/xiao-rp2350-bargraph.md#L585)
* **Issue**: During `make distclean`, `nuttx/tools/Unix.mk` executes `$(MAKE) clean_context` after `subdir_distclean`, which re-includes `Make.defs` before deleting `Make.defs`. An unguarded `$(shell ln -s ...)` in `board/scripts/Make.defs` recreates `apps/external` during `clean_context`, leaving `apps/external` behind to pollute subsequent in-tree configurations.
* **Action**: Guard the `$(shell ...)` symlink creation in `xiao-rp2350-bargraph/board/scripts/Make.defs` with `ifeq ($(filter distclean clean_context apps_distclean,$(MAKECMDGOALS)),)` and remove `$(APPDIR)/external` (`rm -f "$(APPDIR)/external"`) in `xiao-rp2350-bargraph/apps/Makefile` on `distclean` (plus add `xiao-rp2350-bargraph/.gitignore` ignoring `*.o`, `.built`, `.depend`, `.kconfig`, `Make.dep`, `.context`).

#### M-3. Omit Unused Board Source Files (`rp23xx_autoleds.c`, `rp23xx_userleds.c`, `rp23xx_gpio.c`) and Simplifiy `bargraph_shutdown_pwm()`
* **Affected Lines**: [23](../../designs/xiao-rp2350-bargraph.md#L23), [108–110](../../designs/xiao-rp2350-bargraph.md#L108-L110), [279–280](../../designs/xiao-rp2350-bargraph.md#L279-L280), [511–520](../../designs/xiao-rp2350-bargraph.md#L511-L520), [545–565](../../designs/xiao-rp2350-bargraph.md#L545-L565), [658–659](../../designs/xiao-rp2350-bargraph.md#L658-L659)
* **Issue**:
  1. `rp23xx_autoleds.c`, `rp23xx_userleds.c`, and `rp23xx_gpio.c` (~750 lines) are never compiled when `CONFIG_ARCH_LEDS`, `CONFIG_USERLED`, and `CONFIG_DEV_GPIO` are unset, and `rp23xx_gpio.c` hardcodes `GPIO26..28` (`D0..D2`), colliding with `ADC0`, `LED1`, and `LED2`.
  2. In `bargraph_shutdown_pwm()`, calling `ioctl(fd, PWMIOC_STOP, 0)` (to reset `upper->started = false` in `drivers/timers/pwm.c:428`) followed by `close(fd)` automatically invokes `pwm_shutdown()` (`rp23xx_pwm.c:249-318`), which disables the PWM slice, configures `pin[0]` and `pin[1]` as outputs driven low (`0`), and switches their mux to `RP23XX_GPIO_FUNC_SIO`—making the extra zero-duty `PWMIOC_SETCHARACTERISTICS` + `PWMIOC_START` + `usleep(2000)` sequence before `PWMIOC_STOP` unnecessary when closing the descriptors (or keep zero-duty only as belt-and-suspenders while noting `pwm_shutdown()` also drives the pins low on `close()`).
* **Action**: Either omit `rp23xx_autoleds.c`, `rp23xx_userleds.c`, and `rp23xx_gpio.c` from `xiao-rp2350-bargraph/board/src/` (`CSRCS = rp23xx_boardinitialize.c rp23xx_bringup.c`) or ensure `BOARD_NGPIOOUT/IN/INT` do not claim `GPIO26..28`; and simplify `bargraph_shutdown_pwm()` / document `pwm_shutdown()` behavior on `close()`.

#### M-4. Specify Complete `defconfig` Deltas, `board.h` Standalone Includes, and `rp23xx_bringup.c` Header Includes
* **Affected Lines**: [426–434](../../designs/xiao-rp2350-bargraph.md#L426-L434), [449–466](../../designs/xiao-rp2350-bargraph.md#L449-L466), [474–482](../../designs/xiao-rp2350-bargraph.md#L474-L482), [535–544](../../designs/xiao-rp2350-bargraph.md#L535-L544), [679](../../designs/xiao-rp2350-bargraph.md#L679)
* **Issue**:
  1. Target File 7 (`defconfig`) must state that `CONFIG_ARCH_BOARD="xiao-rp2350"` and `CONFIG_ARCH_BOARD_XIAO_RP2350=y` are removed from `usbnsh/defconfig`, set `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=12` (since `bargraphd` holds 9 open fds: `0, 1, 2` + `/dev/adc0` + 5 `/dev/pwm*`), and set `CONFIG_RAW_BINARY=y` and `CONFIG_RP23XX_UF2_BINARY=y` (noting that `picotool` in `PATH` or `PICO_SDK_PATH` is required, and calibrating `nuttx/nuttx.elf` on line 679 to `nuttx/nuttx`).
  2. Align Target File 3 (`board/README.md`, line 426) and Target File 4 (`board/Kconfig`, line 433) on `ARCH_BOARD_XIAO_RP2350_BARGRAPH` (define `config ARCH_BOARD_XIAO_RP2350_BARGRAPH` with `default y` inside `if ARCH_BOARD_CUSTOM` in `board/Kconfig`).
  3. Target File 9 (`board.h`) must state that the 4 sibling includes (`rp23xx_i2cdev.h`, `rp23xx_spidev.h`, `rp23xx_i2sdev.h`, `rp23xx_spisd.h`) present in `xiao-rp2350/include/board.h` are omitted.
  4. Target File 17 (`rp23xx_bringup.c`) must explicitly list `#include "arm_internal.h"` and `#include "hardware/rp23xx_pads_bank0.h"`.

#### M-5. Specify `MAINSRC` Entrypoint Naming (`int main`), Telemetry Format, Scaling Formulas, and Single Wait Mechanism in `bargraph_main.c`
* **Affected Lines**: [303–344](../../designs/xiao-rp2350-bargraph.md#L303-L344), [526](../../designs/xiao-rp2350-bargraph.md#L526), [615–667](../../designs/xiao-rp2350-bargraph.md#L615-L667)
* **Issue**:
  1. `Application.mk` compiles `MAINSRC = bargraph_main.c` with `-Dmain=bargraph_main`. Target File 30 should specify defining `int main(int argc, FAR char *argv[])` in `bargraph_main.c` (which the preprocessor renames to `bargraph_main`).
  2. Remove the undecided parenthetical `(`CLOCK_REALTIME` or `usleep` + mutex lock)` on line 664, `"weak/"` on line 526, and `(when not yet started or on update)` on line 657; state the exact synchronization mechanism (`PTHREAD_MUTEX_INITIALIZER` / `PTHREAD_COND_INITIALIZER`, startup error cleanup so `done_cond` never deadlocks, and `clock_gettime(CLOCK_REALTIME, ...)` + `pthread_cond_timedwait()` or `usleep(20000)` polling).
  3. Specify the exact integer formulas for `millivolts`, `permille`, and `total_units` in `SENSING`, `MANUAL`, and `TEST` modes, `bargraph test` behavior when `STOPPED` or interrupted by `stop_requested`, and the concrete `printf` format for `bargraph status`.
  4. Keep the full C struct definition of `struct bargraph_state_s` in `DetailedRange` Target File 29 (`bargraph_internal.h`) and summarize its fields at high altitude in `FeatureRange` Section 5.

---

### 🟢 Nits (Diagram Polish, Links, and Editorial Calibration)

#### N-1. Fix Misleading `+3.3V Rail` Line Into `Row 7` (`D2/A2`) in Breadboard ASCII Diagram
* **Affected Lines**: [171–185](../../designs/xiao-rp2350-bargraph.md#L171-L185)
* **Action**: Remove the left-side vertical `+3.3V Rail` connection into `Row 7: +---[D2/A2 GPIO28 (7E)]` so it does not visually imply connecting `3.3V` to the `D2` PWM output pin (`7E`); `3V3` is on the right side at `7F` (`7J -> Red (+) 3.3V Rail`).

#### N-2. Add `TEST --> STOPPED` Transition in State Machine Diagram
* **Affected Lines**: [288–301](../../designs/xiao-rp2350-bargraph.md#L288-L301)
* **Action**: Add `TEST --> STOPPED : "bargraph stop"` to the Mermaid state diagram and note that each 20 ms step of the 100-step `TEST` sweep checks `stop_requested`.

#### N-3. Structural & Copy Polish (`Additional Resources` Links, Horizontal Rules, and Neutral Phrasing)
* **Affected Lines**: [7–401](../../designs/xiao-rp2350-bargraph.md#L7-L401)
* **Action**:
  1. Convert repository file paths in `### Additional Resources` (lines 390–393) into clickable relative links (`[nuttx/...](../../nuttx/...)`).
  2. Promote `Additional Resources` and `Future Improvements` to `##` (or remove the horizontal divider at line 386) and standardize horizontal rules on `---`.
  3. Replace colloquialisms/repetitions on lines 30 ("produce empty output"), 70 (remove duplicate "cleanly"), 237 ("overwrite"), 255 ("zero duty"), 351 ("channel-array indexing mismatch"), and 371 ("while accepting `bargraph start` as a synonym").

---

## Cross-Stage Traceability Matrix

| Consolidated ID | Priority | Summary | Stage 5 (`zoom_out_report.md`) | Stage 6 (`comprehension_report.md`) | Stage 7 (`critic_report.md`) | Stage 8 (`intermediate_zoom_out_report.md`) | Stage 9 (`implementation_readiness_report.md`) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **B-1** | 🔴 Blocker | `rp23xx_adc.c` uses `START_ONCE` (not `START_MANY`); `RP23XX_ADC_DIV` has no effect; gate `RP23XX_ADC_IRQ_FIFO` via NVIC and calibrate `interrupt_handler()` / `my_shutdown()` | Part 1 #1 (Blocker), Part 2 #1 (Major) | — | Part 1 #1 (Blocker), Part 2 #1 (Nit), Part 3 #1 (Major), Part 3 #4 (Nit) | #1 (Blocker) | Part 1 #1 (Blocker), Part 2 #7 (Nit) |
| **B-2** | 🔴 Blocker | Fixed-point Q4 EMA right-shift bug (`>> 2` scales to `4x` full scale `0..16380`; must be `(ema_q4 + 8) >> 4`) & first-sample seeding | — | — | — | — | Part 1 #4 (Blocker), Part 2 #5 (Major) |
| **B-3** | 🔴 Blocker | Remove non-existent `CONFIG_PWM_MULTICHAN=y` and add required `CONFIG_ANALOG=y` in `defconfig` | — | — | Part 1 #2 (Blocker) | #2 (Blocker) | Part 1 #2 (Blocker), Part 1 #3 (Blocker), Part 2 #3 (Major) |
| **B-4** | 🔴 Blocker | Calibrate codebase claims in `rp23xx_adc.c` (`my_reset()`), `rp23xx_pwm.c` (`setup_period()` `DIV=36, TOP=65535`, `pwm_stop()` `RP23XX_PWM_EN`, signed `pin`), `drivers/timers/pwm.c`, and `nsh_main.c` | Part 1 #5 (Nit) | — | Part 1 #3 (Major), Part 1 #4 (Major), Part 3 #2 (Major), Part 3 #3 (Major) | #6 (Nit) | Part 1 #2 (Blocker), Part 1 #5 (Blocker), Part 1 #6 (Blocker) |
| **M-1** | 🟡 Major | Define "EGM" on first mention and link to workspace EGM `README.md` exemplars | — | #1 (Major) | — | — | — |
| **M-2** | 🟡 Major | Guard `apps/external` symlink creation during `distclean`/`clean_context`, remove symlink on `distclean`, and add `.gitignore` | — | — | Part 1 #5 (Major) | #5.2 (Major) | Part 2 #8 (Nit) |
| **M-3** | 🟡 Major | Remove dead board files (`rp23xx_autoleds.c`, `rp23xx_userleds.c`, `rp23xx_gpio.c` `GPIO26..28` collision) and simplify `bargraph_shutdown_pwm()` (`close(fd)` -> `pwm_shutdown()`) | Part 1 #2 (Major), Part 1 #4 (Major) | — | — | #3 (Major), #4.1 (Major) | Part 2 #9 (Nit) |
| **M-4** | 🟡 Major | Complete `defconfig` symbol removals/additions (`NFILE_DESCRIPTORS_PER_BLOCK=12`, `RAW_BINARY=y`, `UF2_BINARY=y`), align `board/Kconfig`, omit 4 missing includes in `board.h`, list register headers in `rp23xx_bringup.c`, calibrate `nuttx/nuttx` | — | #2 (Nit) | — | — | Part 1 #7 (Major), Part 1 #8 (Major), Part 1 #9.2 (Major), Part 1 #10 (Nit), Part 2 #2 (Major), Part 2 #3 (Major), Part 2 #6 (Major), Part 3 #2 (Major), Part 3 #3 (Nit) |
| **M-5** | 🟡 Major | Specify `MAINSRC` `int main(...)` `-Dmain=bargraph_main` rule, telemetry format & scaling formulas, startup error cleanup, single wait mechanism, and summarize `bargraph_state_s` in `FeatureRange` | Part 1 #3 (Major), Part 2 #2 (Nit) | — | — | #4.2 (Major), #5.1 (Major), #6 (Nit) | Part 1 #9 (Major), Part 2 #1 (Major), Part 2 #4 (Major), Part 2 #5 (Major), Part 3 #1 (Major), Part 3 #4 (Nit), Part 3 #5 (Nit) |
| **N-1** | 🟢 Nit | Fix misleading `+3.3V Rail` connection to `Row 7` (`7E` `D2/A2`) in breadboard ASCII diagram | — | #3 (Nit) | — | — | — |
| **N-2** | 🟢 Nit | Add `TEST --> STOPPED` transition to state machine diagram and preempt `TEST` sweep on `stop_requested` | Part 2 #3 (Nit) | — | Part 1 #6 (Nit) | — | Part 1 #9.3 (Major) |
| **N-3** | 🟢 Nit | Make `Additional Resources` paths clickable links, standardize `---` horizontal rules, and tighten colloquial phrasing | — | — | Part 2 #1 (Nit), Part 2 #2 (Nit), Part 2 #3 (Nit), Part 3 #5 (Nit) | — | — |
