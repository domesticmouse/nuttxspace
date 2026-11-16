# Stage 5: Initial Zoom Out Review Report

## Part 1: Zoom Out Review (`egm-zoom-out`)

### Zoom Out Review Findings for [`docs/designs/xiao-rp2350-bargraph.md`](../../designs/xiao-rp2350-bargraph.md)

Overall, the design document is grounded in the actual hardware and driver behavior of `nuttx/arch/arm/src/rp23xx` ([`rp23xx_pwm.c`](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c), [`rp23xx_adc.c`](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c)) and `nuttx/boards/arm/rp23xx/common` ([`rp23xx_common_bringup.c`](../../../../nuttx/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c)). It also corrects two stale claims from [`concepts_brief.md`](../../designs/concepts_brief.md):
1. `rp23xx_adc.c` actually uses `RP23XX_ADC_CS_START_ONCE` inside `get_next_channel()` (not `START_MANY`), so `ADC_DIV` has no effect and NVIC IRQ gating is genuinely required to prevent the 500 kS/s interrupt storm without modifying `nuttx/`.
2. `drivers/timers/pwm.c` copies `struct pwm_info_s` verbatim via `memcpy()` and does not scan `channels[]` for `channel == 0`.

Below are the Zoom Out architectural, boundary, and simplicity findings.

---

#### Finding 1: Kernel/User-Space Boundary Violation in User Application (`up_enable_irq` / `up_disable_irq` in `apps/` and `board/` calling into `apps/`)
*   **Severity**: Major
*   **Affected Scope**: Section `3. Boot Sequence` (lines 209–229), Section `4.1.3 Demand-Driven NVIC IRQ Gating` (lines 242–247), Section `Detailed Implementation Plan` Target Files 16, 18, 27, 28 (lines 511–533, 668–674).
*   **Simplicity & Architectural Rationale**:
    The top-level workspace blueprint ([`README.md`](../../README.md#L16)) and [`nuttx/INVIOLABLES.md`](../../../../nuttx/INVIOLABLES.md#L26-L41) define a strict Global Invariant:
    > *"Kernel/User-Space Separation: Portable kernel subsystems in `nuttx/` ... and architecture/board layers (`arch`, `boards`) enforce the boundary rules in `[nuttx/INVIOLABLES.md]`, while user-space applications in `apps/` interact with the OS through public headers in `[nuttx/include/README.md]`, `[nuttx/syscall/syscall.csv:g_stublookup]`, `[nuttx/boards/boardctl.c:boardctl]`, and the built-in application table `[apps/builtin/builtin_list.c:g_builtins]`."*
    
    Even in `CONFIG_BUILD_FLAT=y`, having user-space code in `xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c` include `<nuttx/irq.h>` and `<arch/rp23xx/irq.h>` to call privileged NVIC functions `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` and `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` directly—and having board-level kernel code (`rp23xx_bringup.c`) directly call a custom application symbol `bargraph_autostart()`—blurs the kernel/user-space boundary:
    *   **Alternative / Reuse Opportunity**:
        1. For **autostart**, `apps/builtin` already exports `exec_builtin("bargraph", argv, NULL)` ([`apps/include/builtin/builtin.h`](../../../../apps/include/builtin/builtin.h#L77)), or `rp23xx_bringup.c` can spawn the registered built-in entrypoint without introducing a separate cross-layer `bargraph_autostart()` symbol in `board/src/rp23xx_pico.h`.
        2. If keeping the out-of-tree design strictly confined to `xiao-rp2350-bargraph/`, consider whether the NVIC IRQ gating (or an overriding/custom board-level ADC wrapper or `board_ioctl` hook) belongs in `xiao-rp2350-bargraph/board/src/` rather than inside `apps/bargraph/bargraph_main.c`, or explicitly document in `xiao-rp2350-bargraph/README.md` why flat-build NVIC gating from `bargraphd` is an intentional exception to the workspace's Kernel/User-Space Separation invariant.

---

#### Finding 2: Redundant Two-File Split (`bargraph_internal.h` + `bargraph_main.c`) for a Single-Translation-Unit Application
*   **Severity**: Nit
*   **Affected Scope**: Section `1. System Architecture` (lines 119–120), Section `5. Daemon State Machine` (line 305), and `Detailed Implementation Plan` Target Files 27 & 28 (lines 590–701).
*   **Simplicity Rationale**:
    `xiao-rp2350-bargraph/apps/bargraph/` has only a single C source file (`bargraph_main.c`). Creating `bargraph_internal.h` solely to hold `struct bargraph_state_s` (which is only instantiated as `static struct bargraph_state_s g_bargraph` inside `bargraph_main.c`), internal `#define` constants, and internal helper prototypes (`bargraph_compute_duties`, `bargraph_filter_adc`) that have no second caller translation unit adds unnecessary file/header sprawl.
    *   **Alternative / Reuse Opportunity**: Follow the standard single-file NuttX application pattern (e.g., [`apps/examples/pwm/pwm_main.c`](../../../../apps/examples/pwm/pwm_main.c)) by keeping private constants, types, and `static` helper functions inside `bargraph_main.c` (and if `bargraph_autostart(void)` is kept as a cross-module entrypoint, declare it in `<arch/board/board.h>` where board-level interfaces already live).

---

#### Finding 3: Unused / Redundant `BARGRAPH_LED*_GPIO` and `BARGRAPH_ADC_*` Macros in `board/include/board.h`
*   **Severity**: Nit
*   **Affected Scope**: Section `Detailed Implementation Plan` Target Files 9 & 10 (`xiao-rp2350-bargraph/board/include/board.h`, lines 455–471).
*   **Simplicity Rationale**:
    Target File 10 proposes defining `BARGRAPH_ADC_DEVPATH "/dev/adc0"`, `BARGRAPH_ADC_GPIO 26`, and `BARGRAPH_LED1_GPIO 27` through `BARGRAPH_LED6_GPIO 0` in `board/include/board.h`. However:
    1. All PWM and ADC pin assignments are already defined by `CONFIG_RP23XX_PWM*` and `CONFIG_RP23XX_ADC_CHANNEL0` in `defconfig` (consumed directly by [`rp23xx_common_bringup.c`](../../../../nuttx/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c#L131-L547)).
    2. `bargraph_main.c` opens `/dev/pwm0`, `/dev/pwm2`, `/dev/pwm3`, `/dev/pwm5`, `/dev/pwm6` and `/dev/adc0` by VFS device path and never manipulates LED GPIO pins directly.
    Duplicating pin assignments in both `defconfig` Kconfig symbols and `board.h` macros creates two sources of truth.
    *   **Alternative / Reuse Opportunity**: Keep `board.h` minimal—matching [`nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h`](../../../../nuttx/boards/arm/rp23xx/xiao-rp2350/include/board.h) minus the unused bus headers—and rely on the Kconfig symbols for PWM/ADC GPIO routing (only defining `BARGRAPH_ADC_GPIO 26` if used by `rp23xx_bringup.c` for `RP23XX_PADS_BANK0_GPIO`).

---

#### Finding 4: Redundant `channels[i].channel` Population vs. Upper/Lower-Half Contract
*   **Severity**: Nit
*   **Affected Scope**: Section `The Problem` #3 (line 21) and Section `4.2.2 Two-Channel struct pwm_info_s Population` (lines 268–274).
*   **Simplicity Rationale**:
    The design document accurately notes that [`nuttx/drivers/timers/pwm.c`](../../../../nuttx/drivers/timers/pwm.c#L365) copies `struct pwm_info_s` verbatim via `memcpy()` and [`nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c`](../../../../nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c#L356-L361) indexes `info->channels[0].duty` and `info->channels[1].duty` directly without inspecting `.channel`. Populating `channels[0].channel = 1` and `channels[1].channel = 2` is harmless and good practice for `pwm_dump()` debug logging in `drivers/timers/pwm.c` (lines 112–116), so no code change is needed—just ensure the prose in Section 4.2.2 doesn't imply the upper-half driver rejects `channel == 0` at runtime.

---

#### Finding 5: Altitude Drops in Detailed Implementation Plan (Target File 28)
*   **Severity**: Nit
*   **Affected Scope**: Section `Detailed Implementation Plan` Target File 28 (`bargraph_main.c`, lines 642–700).
*   **Simplicity Rationale**:
    While Section `Technical Plan (Architecture)` maintains a clean architectural altitude, Target File 28 in the `Detailed Implementation Plan` drops into line-by-line C expressions (`*ema_q4 = *ema_q4 - (*ema_q4 >> 2) + ((uint32_t)raw_avg << 2)`, exact integer rounding constants `393210u + 2047u`, local variable names `test_step`, `level_step`, `pwm_started[s]`) that duplicate the mathematical specifications already presented in Sections 4.1.4, 4.1.5, and 4.2.3. Keeping the formulas in Section 4 as the single specification and summarizing Target File 28 by responsibilities and interfaces would keep the plan more concise.

---

## Part 2: Inventive Catalyst Review (`egm-inventive-catalyst`)

No false compromise detected.
