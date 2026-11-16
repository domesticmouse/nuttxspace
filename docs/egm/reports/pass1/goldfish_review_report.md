# Goldfish Annotated Review Report

**Status:** `resolved`
**Target:** [`docs/designs/xiao-rp2350-bargraph.md`](../../designs/xiao-rp2350-bargraph.md) (`full file`: Lines 1–727)

## Prioritized Findings & AI Suggestions

### [Issue B-1] [`egm-zoom-out` / `egm-inventive-catalyst` / `egm-design` / `egm-codebase-details`] - Severity: Blocker
*   **Affected Scope:** [Lines 25–662](../../designs/xiao-rp2350-bargraph.md#L25-L662) (`## The Problem`, `## Goals`, `### 3. Boot Sequence`, `### 4.1 ADC Subsystem Mitigations`, `## Non-Functional Requirements`, `### Additional Resources`, Target Files #13, #17, #30)
*   **Discrepancy Description:** The design states that `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` runs the ADC in continuous free-running mode (`ADC_CS_START_MANY`) and that writing `putreg32(0x00ffff00, RP23XX_ADC_DIV)` in `rp23xx_bringup.c` paces conversions to `~732.42 Hz`. Inspection of `rp23xx_adc.c` (lines 169–320) shows that the driver never sets `RP23XX_ADC_CS_START_MANY`; instead, `interrupt_handler()` (line 234) unconditionally calls `get_next_channel()`, which writes single-shot `RP23XX_ADC_CS_START_ONCE` (line 304) even when `au_receive()` returns `-ENOMEM` on a full software FIFO. On RP2350 hardware, `RP23XX_ADC_DIV` only paces free-running `START_MANY` conversions and has no effect on `START_ONCE` triggers (`2 µs`/sample), leaving the `500,000 IRQ/s` interrupt storm active while `/dev/adc0` is open. Additionally, the ISR in `rp23xx_adc.c` is `interrupt_handler()` (not `adc_interrupt()`), and `remove_device(dev)` is called from `my_shutdown()` / `my_rxint(dev, false)` (lines 538, 563) while `interrupt_handler()` checks `g_active_count == 0` (line 176).
*   **Rationale:** Keeping `/dev/adc0` open with `RP23XX_ADC_IRQ_FIFO` enabled in the NVIC starves the CPU with `500,000 IRQ/s` regardless of `RP23XX_ADC_DIV`. Gating `RP23XX_ADC_IRQ_FIFO` via `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` (`<nuttx/irq.h>` and `<arch/rp23xx/irq.h>`) around each 50 Hz burst in `bargraphd` (`CONFIG_BUILD_FLAT=y`) eliminates background interrupts during the 20 ms sleep window and reduces total ADC interrupts to `250 IRQ/s` (`5` samples read in `~10 µs` per 50 Hz cycle: 1 stale sample latched at the end of the prior burst discarded + 4 fresh samples averaged).
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Verified against `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` (lines 169–320, 520–570). Removing `RP23XX_ADC_DIV` from `rp23xx_bringup.c` and gating `RP23XX_ADC_IRQ_FIFO` in `bargraphd` (masking immediately after `open("/dev/adc0")`, unmasking during `bargraph_sample_adc()` after `ANIOC_RESET_FIFO` to read 5 samples and average `msgs[1..4]`, masking again before sleeping, and unmasking before `close(adc_fd)` on shutdown) solves the 500 kS/s interrupt storm without modifying `nuttx/`.
*   ***Proposed Edit:***
    *   Location: Lines 25, 33, 220, 242–250, 378–379, 391, 509, 538–542, 660–662
    *   Change: Replace `ADC_CS_START_MANY` / `RP23XX_ADC_DIV = 0x00ffff00` claims and register writes with accurate descriptions of `rp23xx_adc.c`'s `RP23XX_ADC_CS_START_ONCE` ISR loop (`interrupt_handler()` -> `get_next_channel()`), `my_shutdown()` -> `remove_device(dev)`, and NVIC IRQ gating (`up_disable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_enable_irq(RP23XX_ADC_IRQ_FIFO)`) reading 5 samples (discarding `msgs[0]`, averaging `msgs[1..4]`).

---

### [Issue B-2] [`egm-codebase-details` / `egm-implementation-readiness`] - Severity: Blocker
*   **Affected Scope:** [Lines 253–254](../../designs/xiao-rp2350-bargraph.md#L253-L254), [Line 662](../../designs/xiao-rp2350-bargraph.md#L662) (`### 4.1 ADC Subsystem Mitigations`, Target File #30 `bargraph_main.c`)
*   **Discrepancy Description:** Line 254 defines the Q4 EMA recurrence as $\text{ema\_q4} \leftarrow \text{ema\_q4} - (\text{ema\_q4} \gg 2) + (\text{raw\_avg} \ll 2)$ and extracts $\text{filtered} = \text{ema\_q4} \gg 2$. At steady state for a constant input $R$, $\text{ema\_q4} / 4 = 4 R \implies \text{ema\_q4} = 16 R = R \ll 4$. Shifting right by `2` (`ema_q4 >> 2`) produces $4 R \in [0, 16380]$ instead of $R \in [0, 4095]$. Additionally, first-sample seeding of `ema_q4` is not specified.
*   **Rationale:** With `filtered = ema_q4 >> 2`, all 6 LEDs saturate at 100% duty when the potentiometer reaches 25% of travel. Extracting `filtered = (ema_q4 + 8) >> 4` and seeding `ema_q4 = raw_avg << 4` on the first sample restores unity DC gain across `0..4095`.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** The fixed-point Q4 accumulator stores $16 \times \text{raw\_avg}$, so dividing by 16 with rounding requires `(ema_q4 + 8) >> 4`, and seeding `ema_q4 = raw_avg << 4` on the first burst prevents an artificial startup ramp from 0.
*   ***Proposed Edit:***
    *   Location: Lines 254 and 662
    *   Change: Update the output extraction formula to $\text{filtered} = (\text{ema\_q4} + 8) \gg 4$ and specify seeding `ema_q4 = raw_avg << 4` on the first sample (`!ema_initialized`).

---

### [Issue B-3] [`egm-design` / `egm-codebase-details` / `egm-implementation-readiness`] - Severity: Blocker
*   **Affected Scope:** [Line 260](../../designs/xiao-rp2350-bargraph.md#L260), [Line 457](../../designs/xiao-rp2350-bargraph.md#L457), [Line 464](../../designs/xiao-rp2350-bargraph.md#L464) (`### 4.2 Hardware PWM Subsystem Mitigations`, Target File #7 `defconfig`)
*   **Discrepancy Description:**
    1. Lines 260 and 457 specify `CONFIG_PWM_MULTICHAN=y` in `defconfig`, which does not exist in `nuttx/drivers/timers/Kconfig` and causes `olddefconfig` (`nuttx/tools/Unix.mk:725-734`) to abort with `exit 1` on unknown symbol warnings.
    2. Line 464 specifies `CONFIG_ADC=y` without `CONFIG_ANALOG=y`. Because `config ADC` is inside `if ANALOG` (`nuttx/drivers/analog/Kconfig:6-20`), `olddefconfig` drops `CONFIG_ADC=y` unless `CONFIG_ANALOG=y` is set.
*   **Rationale:** Both Kconfig discrepancies break the board configuration and prevent `/dev/adc0` from compiling.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Verified in `nuttx/drivers/timers/Kconfig` and `nuttx/drivers/analog/Kconfig`.
*   ***Proposed Edit:***
    *   Location: Lines 260, 457, and 464
    *   Change: Remove `CONFIG_PWM_MULTICHAN=y` from lines 260 and 457 (retaining `CONFIG_PWM=y`, `CONFIG_PWM_NCHANNELS=2`, `CONFIG_RP23XX_PWM=y`, `CONFIG_RP23XX_PWM_MULTICHAN=y`, `CONFIG_RP23XX_PWM_NCHANNELS=2`), and add `CONFIG_ANALOG=y` on line 464.

---

### [Issue B-4] [`egm-codebase-details` / `egm-design` / `egm-style`] - Severity: Blocker
*   **Affected Scope:** [Lines 21–24](../../designs/xiao-rp2350-bargraph.md#L21-L24), [Lines 236–237](../../designs/xiao-rp2350-bargraph.md#L236-L237), [Lines 266–270](../../designs/xiao-rp2350-bargraph.md#L266-L270), [Lines 279–280](../../designs/xiao-rp2350-bargraph.md#L279-L280), [Line 364](../../designs/xiao-rp2350-bargraph.md#L364) (`## The Problem`, `### 4.1 ADC Subsystem Mitigations`, `### 4.2 Hardware PWM Subsystem Mitigations`, `### Alternative C`)
*   **Discrepancy Description:**
    1. `my_reset()` in `rp23xx_adc.c:466-483` loops unconditionally over all 4 ADC GPIOs (`26..29`), setting them to `RP23XX_GPIO_FUNC_NULL` during `adc_register()`. Meanwhile, `rp23xx_pwm_initialize()` does not mux GPIOs during `rp23xx_common_bringup()`; `pwm_setup()` (`rp23xx_pwm.c:214-222`) sets `RP23XX_GPIO_FUNC_PWM` later when `bargraphd` opens `/dev/pwm5` and `/dev/pwm6`. Disabling `CONFIG_RP23XX_ADC_CHANNEL1..3` and `TEMPERATURE` is required so `get_next_channel()` (`rp23xx_adc.c:267`) only samples channel 0 instead of round-robin multiplexing channels 1–4 into `/dev/adc0`.
    2. `nuttx/drivers/timers/pwm.c` copies `struct pwm_info_s` via `memcpy` without inspecting `channel`; the `channel == 0` (skip) / `-1` (terminate) sentinel is the contract in `nuttx/include/nuttx/timers/pwm.h:67-70`.
    3. In `setup_period()` (`rp23xx_pwm.c:486-530`), `info.frequency = 1000` at `150 MHz` yields `DIV = 36` (`0x24`, divider `2.25` in 8.4 fixed-point) and `TOP = 65535` (`0xffff`), not `DIV = 16, TOP = 9374`.
    4. In `rp23xx_pwm.h:53`, `priv->pin[i]` is signed `int8_t`, and `pwm_setup()` checks `if (priv->pin[i] >= 0)` (not `>= 30` unsigned).
    5. `pwm_stop()` clears `1 << priv->num` in `RP23XX_PWM_EN` via `clear_enabled()` (`rp23xx_pwm.c:643-650`), leaving the pad frozen while open, whereas `pwm_shutdown()` on `close()` (`rp23xx_pwm.c:249-318`) switches the pins to `RP23XX_GPIO_FUNC_SIO` and drives them low (`0`).
    6. In `apps/system/nsh/nsh_main.c:71-76`, `nsh_initialize()` (which invokes `nsh_initscript()`) runs before `nsh_consolemain()` (`nsh_usbconsole.c`), whereas `nsh_loginscript()` runs after `nsh_waitusbready()`, and when `CONFIG_DEV_CONSOLE` is disabled (`usbnsh`), `nsh_initialize()` runs before USB CDC/ACM stdio is opened while `Board.mk` requires host `genromfs` and `xxd`.
*   **Rationale:** Every technical claim about `nuttx/` and `apps/` drivers must accurately match the actual C source code.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** All six discrepancies were verified directly against `rp23xx_adc.c`, `rp23xx_pwm.c`, `rp23xx_pwm.h`, `drivers/timers/pwm.c`, `include/nuttx/timers/pwm.h`, and `apps/system/nsh/nsh_main.c`.
*   ***Proposed Edit:***
    *   Location: Lines 21–24, 236–237, 266–270, 279–280, and 364
    *   Change: Calibrate the six driver descriptions to match the exact upstream code.

---

### [Issue M-1] [`egm-comprehension`] - Severity: Major
*   **Affected Scope:** [Line 36](../../designs/xiao-rp2350-bargraph.md#L36), [Lines 409–600](../../designs/xiao-rp2350-bargraph.md#L409-L600) (`## Goals`, `## Detailed Implementation Plan`)
*   **Discrepancy Description:** The document refers to "EGM `README.md` contracts", "Top-Level Workspace EGM Blueprint Documentation", "Leaf EGM Documentation", and `[file:symbol]` citations without defining what "EGM" stands for or linking to existing workspace exemplars.
*   **Rationale:** A reader unfamiliar with EGM (Elephant / Goldfish Memory) directory `README.md` contracts needs an inline definition and links to the workspace's non-leaf and leaf `README.md` exemplars.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Defining EGM on first mention in `## Goals` (line 36) and linking to `../../README.md` and `../../nuttx/boards/arm/rp23xx/xiao-rp2350/src/README.md` makes the document self-contained.
*   ***Proposed Edit:***
    *   Location: Line 36 and `## Detailed Implementation Plan` intro
    *   Change: Define EGM (Elephant / Goldfish Memory) directory `README.md` contracts (non-leaf blueprints with `Overview`, `Subdirectories & Boundaries`, `Invariants`, `Side Effects`, `Verification`; leaf contracts with `Overview`, `Interface`, `Invariants`, `Side Effects`, `Verification`; and `[file:symbol]` citation syntax) and link to [README.md](../../README.md) and [nuttx/boards/arm/rp23xx/xiao-rp2350/src/README.md](../../../../nuttx/boards/arm/rp23xx/xiao-rp2350/src/README.md).

---

### [Issue M-2] [`egm-design` / `egm-zoom-out` / `egm-implementation-readiness`] - Severity: Major
*   **Affected Scope:** [Lines 141–150](../../designs/xiao-rp2350-bargraph.md#L141-L150), [Line 384](../../designs/xiao-rp2350-bargraph.md#L384), [Line 496](../../designs/xiao-rp2350-bargraph.md#L496), [Line 585](../../designs/xiao-rp2350-bargraph.md#L585) (`### 1. System Architecture`, Target File #11 `board/scripts/Make.defs`, Target File #23 `apps/Makefile`)
*   **Discrepancy Description:** During `make distclean`, `nuttx/tools/Unix.mk` runs `$(MAKE) clean_context` after `subdir_distclean`, which re-includes `Make.defs` before deleting `Make.defs`. An unguarded `$(shell ln -s ...)` in `board/scripts/Make.defs` immediately recreates `apps/external` during `clean_context`, leaving `apps/external` behind to pollute subsequent in-tree configurations. Additionally, building `apps/external` generates `.depend`, `Make.dep`, `.built`, and `*.o` inside `xiao-rp2350-bargraph/apps/` unless ignored by a `.gitignore` or cleaned.
*   **Rationale:** Guarding the `$(shell ...)` symlink creation against `distclean`/`clean_context`/`apps_distclean`, removing `$(APPDIR)/external` during `distclean` in `xiao-rp2350-bargraph/apps/Makefile`, and adding `xiao-rp2350-bargraph/.gitignore` keeps both submodules and the superproject clean.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Prevents `apps/external` from lingering after `make distclean` and keeps `git status --porcelain` clean across the workspace.
*   ***Proposed Edit:***
    *   Location: Lines 85–123, 141–150, 384, 496, 585
    *   Change: Guard the `$(shell ...)` symlink hook in `board/scripts/Make.defs` with `ifeq ($(filter distclean clean_context apps_distclean,$(MAKECMDGOALS)),)`, remove `$(APPDIR)/external` in `xiao-rp2350-bargraph/apps/Makefile`'s `distclean` hook, and include `xiao-rp2350-bargraph/.gitignore` ignoring `*.o`, `.built`, `.depend`, `.kconfig`, `Make.dep`, and `.context`.

---

### [Issue M-3] [`egm-zoom-out` / `egm-implementation-readiness`] - Severity: Major
*   **Affected Scope:** [Line 23](../../designs/xiao-rp2350-bargraph.md#L23), [Lines 108–110](../../designs/xiao-rp2350-bargraph.md#L108-L110), [Lines 279–280](../../designs/xiao-rp2350-bargraph.md#L279-L280), [Lines 511–520](../../designs/xiao-rp2350-bargraph.md#L511-L520), [Lines 545–565](../../designs/xiao-rp2350-bargraph.md#L545-L565), [Lines 658–659](../../designs/xiao-rp2350-bargraph.md#L658-L659) (`### 1. System Architecture`, `### 4.2 Hardware PWM Subsystem Mitigations`, Target Files #13, #14, #18, #19, #20, #30)
*   **Discrepancy Description:**
    1. `rp23xx_autoleds.c`, `rp23xx_userleds.c`, and `rp23xx_gpio.c` (~750 lines) are unused when `CONFIG_ARCH_LEDS`, `CONFIG_USERLED`, and `CONFIG_DEV_GPIO` are disabled, and `rp23xx_gpio.c` from `xiao-rp2350` hardcodes `GPIO26..28` (`D0..D2`), colliding with `ADC0`, `LED1`, and `LED2`.
    2. In `rp23xx_pwm.c`, `pwm_stop()` (`PWMIOC_STOP`) only clears `RP23XX_PWM_EN` (leaving the pad frozen while open), whereas `close(fd)` invokes `pwm_shutdown()` (`rp23xx_pwm.c:249-318`), which calls `pwm_stop()`, configures `pin[0]` and `pin[1]` as outputs driven low (`0`), switches their mux to `RP23XX_GPIO_FUNC_SIO`, and zeros `priv->duty[]`.
*   **Rationale:** Omitting the three unused board C files (`rp23xx_autoleds.c`, `rp23xx_userleds.c`, `rp23xx_gpio.c`) eliminates ~750 lines of dead code and the `GPIO26..28` pin conflict, while simplifying `bargraph_shutdown_pwm()` to issue `PWMIOC_STOP` + `close(fd)` (or noting that `pwm_shutdown()` on `close(fd)` drives the pins low via `RP23XX_GPIO_FUNC_SIO` alongside zeroing duties) removes the artificial `usleep(2000)` delay.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Verified in `nuttx/boards/Kconfig` (`ARCH_BOARD_CUSTOM` does not select `ARCH_HAVE_LEDS`) and `nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c:249-318`.
*   ***Proposed Edit:***
    *   Location: Lines 23, 108–110, 279–280, 511–520, 545–565, 658–659
    *   Change: Remove `rp23xx_autoleds.c`, `rp23xx_userleds.c`, and `rp23xx_gpio.c` from `xiao-rp2350-bargraph/board/src/` (`CSRCS = rp23xx_boardinitialize.c rp23xx_bringup.c`), and simplify `bargraph_shutdown_pwm()` to zero duties, call `PWMIOC_STOP`, and call `close(fd)` (which invokes `pwm_shutdown()` to drive the pins low via `RP23XX_GPIO_FUNC_SIO` without needing `usleep(2000)`).

---

### [Issue M-4] [`egm-codebase-details` / `egm-implementation-readiness` / `egm-style` / `egm-comprehension`] - Severity: Major
*   **Affected Scope:** [Lines 426–434](../../designs/xiao-rp2350-bargraph.md#L426-L434), [Lines 449–466](../../designs/xiao-rp2350-bargraph.md#L449-L466), [Lines 474–482](../../designs/xiao-rp2350-bargraph.md#L474-L482), [Lines 535–544](../../designs/xiao-rp2350-bargraph.md#L535-L544), [Line 679](../../designs/xiao-rp2350-bargraph.md#L679) (Target Files #3, #4, #7, #9, #17, and `### Verification Plan`)
*   **Discrepancy Description:**
    1. Target File #7 (`defconfig`) does not explicitly state that `CONFIG_ARCH_BOARD="xiao-rp2350"` and `CONFIG_ARCH_BOARD_XIAO_RP2350=y` must be removed from `usbnsh/defconfig`, nor does it specify `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=12` (needed for 9 open fds in `bargraphd`), `CONFIG_RAW_BINARY=y`, and `CONFIG_RP23XX_UF2_BINARY=y`.
    2. Target File #3 (`board/README.md`, line 426) references `ARCH_BOARD_XIAO_RP2350_BARGRAPH`, whereas Target File #4 (`board/Kconfig`, line 433) hedges with `Define if ARCH_BOARD_CUSTOM block (or empty Kconfig guard)`.
    3. Target File #9 (`board.h`) does not state that the 4 sibling includes (`rp23xx_i2cdev.h`, `rp23xx_spidev.h`, `rp23xx_i2sdev.h`, `rp23xx_spisd.h`) in `xiao-rp2350/include/board.h` are omitted.
    4. Target File #17 (`rp23xx_bringup.c`) omits the required `#include "arm_internal.h"` and `#include "hardware/rp23xx_pads_bank0.h"` directives.
    5. Verification Plan (line 679) refers to `nuttx/nuttx.elf` instead of `nuttx/nuttx` and hedges on `(when picotool or UF2 conversion is enabled)`.
*   **Rationale:** Resolving these five build/header specifications makes `DetailedRange` directly actionable without build errors.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Eliminates all Kconfig, header-include, fd-block, and UF2 artifact ambiguities in `DetailedRange`.
*   ***Proposed Edit:***
    *   Location: Lines 426–434, 449–466, 474–482, 535–544, 679
    *   Change: Specify complete `defconfig` additions/removals (`CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=12`, `CONFIG_RAW_BINARY=y`, `CONFIG_RP23XX_UF2_BINARY=y`, removing `CONFIG_ARCH_BOARD="xiao-rp2350"` and `CONFIG_ARCH_BOARD_XIAO_RP2350=y`), define `config ARCH_BOARD_XIAO_RP2350_BARGRAPH` (`bool`, `default y`) inside `if ARCH_BOARD_CUSTOM` in `board/Kconfig`, omit the 4 unused sibling includes in `board.h`, list `#include "arm_internal.h"` and `#include "hardware/rp23xx_pads_bank0.h"` in `rp23xx_bringup.c`, and calibrate `nuttx/nuttx.elf` to `nuttx/nuttx` with `picotool` / `PICO_SDK_PATH` as a concrete build prerequisite.

---

### [Issue M-5] [`egm-implementation-readiness` / `egm-codebase-details` / `egm-zoom-out` / `egm-style`] - Severity: Major
*   **Affected Scope:** [Lines 303–344](../../designs/xiao-rp2350-bargraph.md#L303-L344), [Line 526](../../designs/xiao-rp2350-bargraph.md#L526), [Lines 615–667](../../designs/xiao-rp2350-bargraph.md#L615-L667) (`### 5. Daemon State Machine & IPC Synchronization`, Target File #15 `rp23xx_pico.h`, Target Files #28, #29, #30 `bargraph_main.c`)
*   **Discrepancy Description:**
    1. `apps/Application.mk` compiles `MAINSRC = bargraph_main.c` with `-Dmain=bargraph_main`; Target Files #29 and #30 must specify defining `int main(int argc, FAR char *argv[])` in `bargraph_main.c`.
    2. Line 664 contains a self-contradictory hedge `pthread_cond_timedwait() (CLOCK_REALTIME or usleep + mutex lock)`, line 526 hedges `"weak/conditional"`, and line 657 hedges `(when not yet started or on update)`.
    3. Target File #30 omits static initializer rules (`PTHREAD_MUTEX_INITIALIZER` / `PTHREAD_COND_INITIALIZER`), startup error cleanup (closing opened fds, setting `init_err`, broadcasting `done_cond`), exact integer formulas for `millivolts`, `permille`, and `total_units` in `SENSING`/`MANUAL`/`TEST` modes, `bargraph test` behavior when `STOPPED`, and the exact `printf` output format for `bargraph status`.
    4. `FeatureRange` Section 5 (lines 303–332) embeds the full low-level C definition of `enum bargraph_mode_e` and `struct bargraph_state_s`, duplicating `DetailedRange` Target File #29 (`bargraph_internal.h`).
*   **Rationale:** Moving the full C `struct bargraph_state_s` code block from `FeatureRange` Section 5 into `DetailedRange` Target File #29 respects the Zoom-Out altitude boundary, and specifying the exact `int main(...)` signature, wait call (`clock_gettime(CLOCK_REALTIME, ...)` + `pthread_cond_timedwait()`), startup error handshake, integer formulas, and `bargraph status` format makes `bargraph_main.c` unambiguous to implement.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Resolves all implementation readiness and altitude boundary findings in Section 5 and Target Files #15, #28, #29, and #30.
*   ***Proposed Edit:***
    *   Location: Lines 303–344, 526, 615–667
    *   Change: Replace the C code block in `FeatureRange` Section 5 with a concise state table and place the complete `enum bargraph_mode_e` and `struct bargraph_state_s` C definition in `DetailedRange` Target File #29; remove `"weak/"` on line 526; specify `PWMIOC_START` once per slice when `!slice_started[i]`; specify `clock_gettime(CLOCK_REALTIME, &ts)` + `pthread_cond_timedwait(&g_bargraph.wake_cond, &g_bargraph.lock, &ts)` on line 664; and specify `int main(int argc, FAR char *argv[])`, startup error cleanup, integer conversion formulas, and the `bargraph status` `printf` layout in Target File #30.

---

### [Issue N-1] [`egm-comprehension`] - Severity: Nit
*   **Affected Scope:** [Lines 171–185](../../designs/xiao-rp2350-bargraph.md#L171-L185) (`#### 400-Tie-Point Breadboard Layout`)
*   **Discrepancy Description:** In the breadboard ASCII diagram, the vertical line descending from `+3.3V Rail (Red +)` on the left connects via `+---` into `Row 7: +---[D2/A2 GPIO28 (7E)]`, visually implying wiring the `3.3V` rail to the `D2` PWM output pin (`7E`) instead of only to Potentiometer Lug 1 (`Row 1`) and `3V3` (`7F` / `7J` on the right).
*   **Rationale:** Prevents a reader from accidentally shorting the `3.3V` rail to `D2` (`GPIO28`) on the breadboard.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Simple ASCII diagram fix that eliminates a hardware wiring misreading.
*   ***Proposed Edit:***
    *   Location: Lines 174–181
    *   Change: End the left-hand `+3.3V Rail` vertical wire at `Row 1: +---[ Potentiometer Lug 1 (CW / 3.3V) ]` and keep the `3V3` connection strictly on the right side (`Row 7: [(7F) 3V3] +---> Jumper Row 7J to Red (+) 3.3V Rail`).

---

### [Issue N-2] [`egm-design` / `egm-inventive-catalyst`] - Severity: Nit
*   **Affected Scope:** [Lines 288–301](../../designs/xiao-rp2350-bargraph.md#L288-L301), [Line 343](../../designs/xiao-rp2350-bargraph.md#L343) (`### 5. Daemon State Machine & IPC Synchronization`)
*   **Discrepancy Description:** The Mermaid state diagram omits `TEST --> STOPPED : "bargraph stop"` if `bargraph stop` is invoked while a 2.0-second `TEST` sweep is in progress.
*   **Rationale:** Adding `TEST --> STOPPED` and checking `stop_requested` on every 20 ms step of the 100-step sweep ensures `bargraph stop` preempts `TEST` within 20 ms.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Makes the state machine complete and responsive during `TEST` mode.
*   ***Proposed Edit:***
    *   Location: Lines 298–301 and 343
    *   Change: Add `TEST --> STOPPED : "bargraph stop (abort sweep)"` to the Mermaid diagram and state that `bargraph test` auto-starts `bargraphd` if `STOPPED` and aborts immediately on `stop_requested`.

---

### [Issue N-3] [`egm-structural` / `egm-style`] - Severity: Nit
*   **Affected Scope:** [Lines 7–401](../../designs/xiao-rp2350-bargraph.md#L7-L401) (`FeatureRange` formatting and prose)
*   **Discrepancy Description:**
    1. Repository file paths in `### Additional Resources` (lines 390–393) are plain inline code spans rather than clickable relative Markdown links.
    2. `### Additional Resources` and `### Future Improvements` are H3 headings preceded by a top-level horizontal rule at line 386, and horizontal rule syntax alternates between `---` and 80-character hyphen lines.
    3. Minor colloquial/repetitive phrasing on lines 30 ("completely clean"), 70 (duplicate "cleanly"), 237 ("wipe out"), 255 ("complete darkness"), 351 ("quirks"), and 371 ("forgiving").
*   **Rationale:** Improves structural consistency, link navigability, and objective technical tone.
*   ***AI Suggestion:*** PROPOSE FIX
*   ***Justification:*** Quick, low-risk editorial cleanup across `FeatureRange`.
*   ***Proposed Edit:***
    *   Location: Lines 7–401
    *   Change: Convert `Additional Resources` paths to clickable relative links (`[nuttx/...](../../nuttx/...)`), promote `## Additional Resources` and `## Future Improvements` to H2, standardize horizontal rules on `---`, and tighten phrasing on lines 30, 70, 237, 255, 351, and 371.
