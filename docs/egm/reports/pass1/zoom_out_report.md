# Stage 5: Initial Zoom Out Review Report (`zoom_out_report.md`)

**Target Document**: [xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md) (Full File: Lines 1–693)
**Reviewers**: `egm-zoom-out` (Skeptical Architect), `egm-inventive-catalyst` (TRIZ Inventive Catalyst)

---

## Part 1: Skeptical Architect (`egm-zoom-out`) Findings

### 1. `RP23XX_ADC_DIV` Does Not Pace `rp23xx_adc.c` (`START_ONCE`), Leaving the 500 kS/s Interrupt Storm Active
* **Severity**: 🔴 Blocker
* **Affected Scope**: `## The Problem` (Item 7, [line 25](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L25)), `### 4.1 ADC Subsystem Mitigations` (Item 3, [lines 242–245](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L242-L245); Item 4, [lines 246–250](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L246-L250)), `## Non-Functional Requirements` ([lines 377–379](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L377-L379)), and `#### 17. xiao-rp2350-bargraph/board/src/rp23xx_bringup.c` ([lines 537–542](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L537-L542)).
* **Simplicity Rationale**:
  The design states that [rp23xx_adc.c](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L169-L320) runs the ADC in continuous free-running mode (`ADC_CS_START_MANY`) and that writing `putreg32(0x00ffff00, RP23XX_ADC_DIV)` in `rp23xx_bringup.c` paces conversions to `~732.42 Hz`.
  Inspection of [rp23xx_adc.c](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L169-L320) shows that the driver **never** sets `RP23XX_ADC_CS_START_MANY`. Instead, `interrupt_handler()` (line 234) calls `get_next_channel()`, which immediately triggers the next conversion by writing `RP23XX_ADC_CS_START_ONCE` (lines 304–306). On RP2350 hardware, `RP23XX_ADC_DIV` only paces free-running `START_MANY` conversions and has **no effect** on `START_ONCE` triggers. Consequently, keeping `/dev/adc0` open continuously still fires `RP23XX_ADC_IRQ_FIFO` after every 96-cycle (`2 µs`) conversion (~500,000 IRQs/sec).
* **Simpler Alternative**:
  * Either skip `CONFIG_RP23XX_ADC` / `/dev/adc0` altogether and read `ADC0` (`GPIO26`) directly via `RP23XX_ADC_CS` (`START_ONCE`) and `RP23XX_ADC_RESULT` in a ~10-line helper (which eliminates the 500 kHz ISR storm, the `<< 19` shift workaround, `ANIOC_RESET_FIFO`, the `my_reset()` pin-mux side effects, and the `remove_device()` close race in one stroke), **or**
  * If `/dev/adc0` is retained, gate `RP23XX_ADC_INTE` (or `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`) between 50 Hz sample bursts so conversions only occur during the 4-sample read window.

### 2. Unnecessary 4-Step `bargraph_shutdown_pwm()` Workaround (`close(fd)` Already Drives Pins Low)
* **Severity**: 🟡 Major
* **Affected Scope**: `## The Problem` (Item 5, [line 23](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L23)), `### 4.2 Hardware PWM Subsystem Mitigations` (Item 4, [lines 279–280](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L279-L280)), and `#### 30. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c` (`bargraph_shutdown_pwm`, [lines 658–659](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L658-L659)).
* **Simplicity Rationale**:
  The design proposes a special shutdown sequence (`bargraph_apply_pwm` with zero duties -> `PWMIOC_START` -> `usleep(2000)` -> `PWMIOC_STOP` -> `close(fd)`) on the premise that stopping PWM leaves the GPIO pads frozen at their last counter level.
  However, in [rp23xx_pwm.c](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c#L249-L318), `pwm_shutdown()`—which is automatically invoked by `pwm_close()` ([pwm.c](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/drivers/timers/pwm.c#L235)) when the descriptor is closed—already calls `pwm_stop(dev)`, configures both `pin[0]` and `pin[1]` as outputs driven low (`rp23xx_gpio_put(pin, 0)` when `INV` is unset), switches their mux to `RP23XX_GPIO_FUNC_SIO`, and clears `priv->duty[i] = 0`. Simply calling `close(fd)` on the 5 PWM file descriptors already forces all 6 LED pins low without the extra `PWMIOC_SETCHARACTERISTICS`, `PWMIOC_START`, `usleep(2000)`, and `PWMIOC_STOP` steps.

### 3. Over-Engineered IPC Synchronization (`wake_cond` + `done_cond`) for a 20 ms (`50 Hz`) Loop
* **Severity**: 🟡 Major
* **Affected Scope**: `### 5. Daemon State Machine & IPC Synchronization` ([lines 286–344](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L286-L344)) and `#### 30. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c` ([lines 664–666](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L664-L666)).
* **Simplicity Rationale**:
  `struct bargraph_state_s` uses a mutex plus **two** condition variables (`wake_cond` and `done_cond`), `pthread_cond_timedwait()`, `stop_requested`, and `test_requested` to wake `bargraphd` early from a `20 ms` sleep and block CLI callers.
  1. Note that the base [defconfig](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig#L29) sets `CONFIG_DISABLE_POSIX_TIMERS=y`.
  2. More fundamentally, `bargraphd` already ticks every `20 ms` (`50 Hz`), which is imperceptible to a human CLI user. Polling the shared state once per `usleep(20000)` tick under a single mutex (with a short `usleep` poll loop in `bargraph stop`/`test` if synchronous completion is desired) eliminates both condition variables and `timespec` deadline math.

### 4. Dead Copy-Pasted Board Files (`rp23xx_gpio.c`, `rp23xx_autoleds.c`, `rp23xx_userleds.c`) in `xiao-rp2350-bargraph/board/src/`
* **Severity**: 🟡 Major
* **Affected Scope**: `### 1. System Architecture & Out-of-Tree Build Topology` ([lines 108–110](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L108-L110)) and `## Detailed Implementation Plan` (Target Files #14, #18, #19, #20, [lines 511–565](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L511-L565)).
* **Simplicity Rationale**:
  The plan creates three C files copied from `xiao-rp2350/src/` (~750 lines total) that are unused by this board configuration:
  * `rp23xx_gpio.c` (`CONFIG_DEV_GPIO`): Not enabled in `defconfig`, and in `xiao-rp2350` it hardcodes `GPIO_OUT1 = 28` (`D2`), `GPIO_IN1 = 27` (`D1`), and `GPIO_IRQPIN1 = 26` (`D0`)—the exact pins used by `LED2`, `LED1`, and `ADC0`. Including it is both YAGNI and a pin-mux hazard if `CONFIG_DEV_GPIO` is ever enabled.
  * `rp23xx_autoleds.c` and `rp23xx_userleds.c`: Neither `CONFIG_ARCH_LEDS` nor `CONFIG_USERLED` is enabled in `xiao-rp2350/configs/usbnsh/defconfig` or in `xiao-rp2350-bargraph/board/configs/bargraph/defconfig`, and `rp23xx_boardearlyinitialize()` in `rp23xx_boardinitialize.c` already initializes the onboard `GPIO25` LED directly.
  Omitting these three files from `xiao-rp2350-bargraph/board/src/` keeps the out-of-tree BSP focused strictly on `Make.defs`, `rp23xx_pico.h`, `rp23xx_boardinitialize.c`, and `rp23xx_bringup.c`.

### 5. Redundant `PWMIOC_START` on Every 50 Hz Update and Inaccurate Upper-Half `pwm.c` Diagnosis
* **Severity**: 🟢 Nit
* **Affected Scope**: `## The Problem` (Item 3, [line 21](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L21)), `### 4.1 ADC Subsystem Mitigations` (Item 1, [line 237](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L237)), `### 4.2 Hardware PWM Subsystem Mitigations` (Item 2, [lines 267–272](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L267-L272)), and `#### 30. xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c` ([line 657](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L657)).
* **Simplicity Rationale**:
  * **Upper-half `pwm.c` behavior**: Line 21 and line 268 state that `nuttx/drivers/timers/pwm.c` stops reading `channels[]` if `channel == 0`. In [pwm.c](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/drivers/timers/pwm.c#L355-L375), `PWMIOC_SETCHARACTERISTICS` performs `memcpy(&upper->info, info, sizeof(struct pwm_info_s))` without inspecting `channel`, and if `upper->started` is already `true`, it immediately calls `lower->ops->start(lower, &upper->info)`. Consequently, calling `ioctl(fd, PWMIOC_START, 0)` on every 50 Hz update (line 272) is a no-op once started (`if (!upper->started)` at line 296 of `pwm.c`) and only needs to be called once at startup.
  * **ADC `my_reset()` behavior**: Line 237 states that `my_reset()` in `rp23xx_adc.c` iterates over *enabled* ADC channels and overwrites `D1`/`D2` PWM muxing set during `rp23xx_common_bringup()`. In fact, `my_reset()` ([rp23xx_adc.c](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L472-L482)) iterates unconditionally over all `g_gpio_map` entries (`GPIO26..29`) regardless of `CONFIG_RP23XX_ADC_CHANNEL*`, and `D1`/`D2` retain `RP23XX_GPIO_FUNC_PWM` because `pwm_setup()` sets the pin mux later when `bargraphd` calls `open("/dev/pwm5")` and `open("/dev/pwm6")`. Disabling `ADC_CHANNEL1..3` in `defconfig` is still necessary to prevent round-robin sampling on channels 1–3, but Section 4.1.1 should reflect the actual driver flow.

---

## Part 2: Inventive Catalyst (`egm-inventive-catalyst`) Findings

### 1. False Compromise Between Continuous `/dev/adc0` Availability and CPU Interrupt Load (`RP23XX_ADC_DIV` vs. `START_ONCE` 500 kS/s IRQ Loop)
* **Severity**: 🟡 Major
* **Affected Scope**: `## The Problem` ([line 25](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L25)), `### 4.1 ADC Subsystem Mitigations (GPIO26 / ADC0)` ([lines 242–250](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L242-L250)), `## Non-Functional Requirements` ([lines 378–379](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L378-L379)), and `## Detailed Implementation Plan` (Items 17 & 30, [lines 538–542](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L538-L542), [lines 660–662](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L660-L662)).
* **False Compromise**:
  The design accepts keeping `/dev/adc0` continuously open while dividing the hardware ADC clock via `putreg32(0x00ffff00, RP23XX_ADC_DIV)` (`~732.42 Hz`) as a compromise to avoid the close/interrupt race in [rp23xx_adc.c](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/arch/arm/src/rp23xx/rp23xx_adc.c) while reducing the 500,000 IRQ/s storm—trading away `~5.5 ms` of blocking acquisition latency on every `20 ms` daemon tick (>27% of each 50 Hz frame spent waiting for 4 slow samples) and still taking `~732` background interrupts/second during idle sleep.
  Because `rp23xx_adc.c` triggers each conversion with `RP23XX_ADC_CS_START_ONCE` inside `interrupt_handler()` -> `get_next_channel()`, `RP23XX_ADC_DIV` has no effect on the conversion rate.
* **Proposed Resolution**:
  Apply **Inversion (TRIZ Pattern #5)** and **Dynamics (TRIZ Pattern #6)** by replacing static hardware clock division with **demand-driven NVIC IRQ gating** (`up_disable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` from `<nuttx/irq.h>`):
  * Immediately after `bargraphd` opens `/dev/adc0` (keeping the descriptor open to avoid the `my_shutdown()` race), call `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`. The single in-flight `START_ONCE` conversion finishes in `2 us` and latches in `RP23XX_ADC_FIFO`, leaving the IRQ pending in the NVIC without entering `interrupt_handler()` or re-triggering `get_next_channel()`. Zero ADC interrupts fire during the `20 ms` sleep window.
  * On each 50 Hz burst in `bargraph_sample_adc()`:
    1. Call `ioctl(adc_fd, ANIOC_RESET_FIFO, 0)` to clear the upper-half software queue.
    2. Call `up_enable_irq(RP23XX_ADC_IRQ_FIFO)`. The pending IRQ immediately services the 1 sample latched at the end of the prior burst and chains `START_ONCE` conversions at `2 us`/sample.
    3. Read `5` `struct adc_msg_s` samples (`~8–10 us` total), discard the first sample (latched at the end of the prior cycle), and average the 4 fresh samples.
    4. Immediately call `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`.
  * Background IRQ rate drops from `500,000 IRQ/s` to `250 IRQ/s` (`99.95%` reduction), and per-tick sample acquisition blocking drops from `5.5 ms` to `< 10 us`.
* **Secondary Liabilities**:
  Calling `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` and `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` from `bargraphd` relies on flat-build execution (`CONFIG_BUILD_FLAT=y`, already a stated invariant in Section 5) and assumes `bargraphd` is the exclusive consumer of `/dev/adc0` while running, plus re-enabling `RP23XX_ADC_IRQ_FIFO` before `close(adc_fd)` on shutdown.

### 2. Unconditional Per-Frame `PWMIOC_SETCHARACTERISTICS` Syscalls Across Static PWM Slices
* **Severity**: 🟢 Nit
* **Affected Scope**: `### 4.2 Hardware PWM Subsystem Mitigations (D1–D6)` ([lines 268–272](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L268-L272)), `## Non-Functional Requirements` ([line 379](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L379)), and `## Detailed Implementation Plan` (Item 30 `bargraph_apply_pwm`, [lines 649–657](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L649-L657)).
* **False Compromise**:
  The design issues 5 `PWMIOC_SETCHARACTERISTICS` (and `PWMIOC_START`) `ioctl` calls on every 20 ms tick (`250–500 ioctls/s` continuously) even when 4 or all 5 slices have unchanged duty cycles.
* **Proposed Resolution**:
  Apply **Preliminary Action / Memoization (TRIZ Pattern #3)** in `bargraph_apply_pwm()` by caching the last-applied `(duty_a, duty_b)` pair and `started` boolean for each of the 5 PWM slices, and only calling `PWMIOC_SETCHARACTERISTICS` when a slice's duty changes (and `PWMIOC_START` once when `!started`).
* **Secondary Liabilities**:
  Adds per-slice shadow state (`uint16_t last_duty[5][2]` and `bool slice_started[5]`) that must be invalidated during `bargraph_shutdown_pwm()`.

### 3. Blocking `TEST` Sweep Loop vs. Immediate State Preemption (`stop` / `set`)
* **Severity**: 🟢 Nit
* **Affected Scope**: `### 5. Daemon State Machine & IPC Synchronization` ([lines 288–301](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L288-L301), [line 343](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L343)) and `## Detailed Implementation Plan` (Item 30 `bargraphd_daemon`, [line 664](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L664)).
* **False Compromise**:
  The state machine diagram ([lines 295–300](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L295-L300)) defines transitions out of `TEST` only upon full sweep completion (`2.0 s`, 100 steps at `20 ms`/step), omitting `TEST -> STOPPED` or `TEST -> MANUAL`.
* **Proposed Resolution**:
  Apply **Segmentation (TRIZ Pattern #1)** by stepping the `TEST` sweep one frame (`test_step` in `0..100`) per iteration of the main 50 Hz loop (or checking `stop_requested` on every 20 ms step inside the sweep), allowing `stop_requested` to abort the sweep and broadcast `done_cond` within a single 20 ms tick.
* **Secondary Liabilities**:
  Requires `bargraphd` to broadcast `done_cond` on both normal sweep completion (`test_step == 100`) and early preemption (`stop_requested == true`).
