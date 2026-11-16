# Stage 9: Implementation Readiness Test Report (`implementation_readiness_report.md`)

**Target Document**: [xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L403-L693) (`DetailedRange`: Lines 403–693, verified against full document and codebase)
**Reviewers**: `egm-implementation-readiness` (Pragmatic Tech Lead), `egm-codebase-details` (Pedantic Codebase Details Auditor), `egm-style` (Style & Copy Editor)

---

## Part 1: Pedantic Codebase Details Auditor (`egm-codebase-details`) Findings

### 1. `RP23XX_ADC_DIV` Has Zero Effect on `rp23xx_adc.c` (`START_ONCE`), Leaving the 500,000 IRQ/s Storm Active
* **Severity**: 🔴 Blocker
* **Affected Scope**: [xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L25) (lines 25, 33, 220, 243–248, 378, 391, 538, 542, 661–662) vs. [rp23xx_adc.c](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L169-L320) and [hardware/rp23xx_adc.h](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/arch/arm/src/rp23xx/hardware/rp23xx_adc.h#L70-L71)
* **Discrepancy Description & Rationale**:
  1. **False `ADC_CS_START_MANY` Claim**: `rp23xx_adc.c` never sets `RP23XX_ADC_CS_START_MANY`. Instead, `interrupt_handler()` ([rp23xx_adc.c:234](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L234)) unconditionally calls `get_next_channel()` (even when `au_receive()` returns `-ENOMEM` on a full upper-half FIFO at line 222), and `get_next_channel()` (line 304) triggers each conversion in software via `value += RP23XX_ADC_CS_START_ONCE; putreg32(value, RP23XX_ADC_CS);`.
  2. **`RP23XX_ADC_DIV` Only Paces `START_MANY`**: On RP2350 hardware, the `ADC: DIV` register only paces free-running `START_MANY` conversions; single-shot `START_ONCE` conversions execute immediately in 96 `clk_adc` cycles (`2 µs` at `48 MHz`). Consequently, keeping `/dev/adc0` open with `RP23XX_ADC_IRQ_FIFO` enabled in the NVIC causes `interrupt_handler()` to fire every `2 µs` (`500,000 IRQ/s`) non-stop regardless of `RP23XX_ADC_DIV`.
  3. **Non-Existent `adc_interrupt()` Reference**: Lines 247 and 391 refer to `adc_interrupt()` and claim it has an unguarded `remove_device(priv)`. In `rp23xx_adc.c`, the ISR is named `interrupt_handler()` (line 169) and does not call `remove_device()` (`remove_device(dev)` is called from `my_shutdown()` at line 538 and `my_rxint()` at line 563; `interrupt_handler()` checks `if (g_active_count == 0)` at line 176).
  4. **Required Fix**: Update lines 25, 33, 243–248, 378, 391, 538–542, and 661–662 to match `rp23xx_adc.c`. Mask `RP23XX_ADC_IRQ_FIFO` via `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` during the 20 ms sleep window, and inside `bargraph_sample_adc()` call `ioctl(adc_fd, ANIOC_RESET_FIFO, 0)`, unmask `up_enable_irq(RP23XX_ADC_IRQ_FIFO)`, read `5` `struct adc_msg_s` samples (discarding `msgs[0]` which was triggered at the end of the prior burst and averaging fresh `msgs[1..4]`), and call `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` immediately after `read()` returns (or sample `RP23XX_ADC_CS`/`RP23XX_ADC_RESULT` directly).

### 2. Non-Existent `CONFIG_PWM_MULTICHAN` Symbol & Inaccurate `drivers/timers/pwm.c` Claim
* **Severity**: 🔴 Blocker
* **Affected Scope**: [xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L21) (lines 21, 260, 268, 457) vs. [drivers/timers/Kconfig](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/drivers/timers/Kconfig#L37-L71) and [drivers/timers/pwm.c](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/drivers/timers/pwm.c#L105-L448)
* **Discrepancy Description & Rationale**:
  1. `drivers/timers/Kconfig` does **not** define `PWM_MULTICHAN` (it defines `config PWM_NCHANNELS` directly under `if PWM`). Because `nuttx/tools/Unix.mk` treats any `kconfiglib` stderr warning as a fatal error in `make olddefconfig`, including `CONFIG_PWM_MULTICHAN=y` in `defconfig` breaks `./tools/configure.sh`.
  2. `drivers/timers/pwm.c` never inspects `info->channels[i].channel`; it executes `memcpy(&upper->info, info, sizeof(struct pwm_info_s))`. The `channel == 0` / `-1` sentinel is documented in `include/nuttx/timers/pwm.h`.

### 3. Missing Kconfig Dependency (`CONFIG_ANALOG=y`) Prevents `/dev/adc0` from Compiling
* **Severity**: 🔴 Blocker
* **Affected Scope**: [xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L464) (Target File 7, line 464) vs. [drivers/analog/Kconfig](file:///Users/brett/Documents/GitHub/nuttxspace-egm/nuttx/drivers/analog/Kconfig#L6-L20)
* **Discrepancy Description & Rationale**:
  In `nuttx/drivers/analog/Kconfig`, `config ADC` is nested inside `if ANALOG` (`menuconfig ANALOG`, `default n`), and `config RP23XX_ADC` does not `select ANALOG` or `select ADC`. Without `CONFIG_ANALOG=y` in `defconfig`, `olddefconfig` drops `CONFIG_ADC`, `CONFIG_ADC_FIFOSIZE`, and `CONFIG_RP23XX_ADC_CHANNEL0`, so `/dev/adc0` is never compiled or registered.

### 4. Fixed-Point Q4 EMA Formula Bug Scales `filtered` to `4x` Full Scale (`0..16380`)
* **Severity**: 🔴 Blocker
* **Affected Scope**: [xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L254) (Section 4.1.5, line 254; Target File 30, line 662)
* **Discrepancy Description & Rationale**:
  Line 254 specifies:
  $$\text{ema\_q4} \leftarrow \text{ema\_q4} - (\text{ema\_q4} \gg 2) + (\text{raw\_avg} \ll 2), \quad \text{filtered} = \text{ema\_q4} \gg 2$$
  At steady state for a constant input $\text{raw\_avg} = R$, $\text{ema\_q4} / 4 = 4 R \implies \text{ema\_q4} = 16 R = R \ll 4$. Shifting right by `2` (`ema_q4 >> 2`) yields $4 R \in [0, 16380]$ instead of $R \in [0, 4095]$, saturating all 6 LEDs at 100% duty when the potentiometer reaches 25% of travel.
  * **Required Fix**: Either shift right by `4` (`filtered = (ema_q4 + 8) >> 4`, seeded on the first sample with `ema_q4 = raw_avg << 4`), or use a Q2 accumulator (`ema_q2 = ema_q2 - (ema_q2 >> 2) + raw_avg`, `filtered = (ema_q2 + 2) >> 2`, seeded with `ema_q2 = raw_avg << 2`).

### 5. Contradicts the Code in `rp23xx_adc.c` (`my_reset()`) and `rp23xx_pwm.c` (`setup_period()`, `pwm_stop()`)
* **Severity**: 🔴 Blocker
* **Affected Scope**: [xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L23-L24) (lines 23–24, 237, 269, 280)
* **Discrepancy Description & Rationale**:
  1. `my_reset()` in `rp23xx_adc.c` loops over all 4 ADC GPIOs (`26..29`) unconditionally; `D1`/`D2` retain `RP23XX_GPIO_FUNC_PWM` because `pwm_setup()` runs later when `bargraphd` calls `open("/dev/pwm5")` and `open("/dev/pwm6")`. Disabling `ADC_CHANNEL1..3` prevents `get_next_channel()` from round-robin scanning channels 1–3.
  2. `setup_period()` in `rp23xx_pwm.c` at `1000 Hz` / `150 MHz` computes `DIV = 36` (`0x24`, divider `2.25`) and `TOP = 65535` (`0xffff`), not `DIV = 16, TOP = 9374`.
  3. `pwm_stop()` clears `1 << priv->num` in `RP23XX_PWM_EN` via `clear_enabled()`, while `pwm_shutdown()` on `close()` drives pins low via `RP23XX_GPIO_FUNC_SIO`.

### 6. Contradicts the Code in `nsh_main.c` / `nsh_init.c` / `nsh_usbconsole.c`
* **Severity**: 🔴 Blocker
* **Affected Scope**: [xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L364) (Alternative C, line 364)
* **Discrepancy Description & Rationale**:
  In `apps/system/nsh/nsh_main.c`, `nsh_initialize()` (which calls `nsh_initscript()`) runs *before* `nsh_consolemain()` is invoked, whereas `nsh_session()` runs `nsh_loginscript()` after `nsh_waitusbready()`. Calibrate line 364 to reflect the actual call order in `nsh_main.c`, `nsh_init.c`, and `nsh_usbconsole.c`.

### 7. Missing Removal of Conflicting `xiao-rp2350` Board Symbols in `defconfig` & Undefined `ARCH_BOARD_XIAO_RP2350_BARGRAPH`
* **Severity**: 🟡 Major
* **Affected Scope**: [xiao-rp2350-bargraph.md](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L426) (Target Files 3, 4, 7; lines 426–466)
* **Discrepancy Description & Rationale**:
  1. Target File 7 must explicitly remove `CONFIG_ARCH_BOARD="xiao-rp2350"` and `CONFIG_ARCH_BOARD_XIAO_RP2350=y` from the base `usbnsh/defconfig` (as `ARCH_BOARD_XIAO_RP2350` conflicts with `ARCH_BOARD_CUSTOM`).
  2. Target File 3 (line 426) references `ARCH_BOARD_XIAO_RP2350_BARGRAPH`, which is not defined in Target File 4 (`board/Kconfig`, line 433). Align both sections.

### 8. Unmentioned Sibling Header Includes in `board.h` & Missing Register Headers in `rp23xx_bringup.c`
* **Severity**: 🟡 Major
* **Affected Scope**: Target File 9 ([lines 474–482](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L474-L482)) and Target File 17 ([lines 535–544](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L535-L544))
* **Discrepancy Description & Rationale**:
  1. Upstream `xiao-rp2350/include/board.h` includes `"rp23xx_i2cdev.h"`, `"rp23xx_spidev.h"`, `"rp23xx_i2sdev.h"`, and `"rp23xx_spisd.h"`, which do not exist in `xiao-rp2350-bargraph/board/include/`. Target File 9 must state that those four `#include` directives are omitted.
  2. Target File 17 (`rp23xx_bringup.c`) must explicitly list `#include "arm_internal.h"` and `#include "hardware/rp23xx_pads_bank0.h"` (and `#include "hardware/rp23xx_adc.h"` if used) so `modreg32`/`putreg32` and register macros compile.

### 9. Missing Initialization-Failure, Descriptor-Count, & Mode-Transition Edge Cases in `bargraph_main.c`
* **Severity**: 🟡 Major
* **Affected Scope**: Target File 30 ([lines 641–667](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L641-L667))
* **Discrepancy Description & Rationale**:
  1. Specify startup failure cleanup in `bargraphd_daemon()` (close opened fds, set `running = false`, broadcast `done_cond`, and exit so `bargraph start` / `bargraph_autostart()` does not deadlock).
  2. Set `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=12` in `defconfig` (base `usbnsh/defconfig` sets `6`, while `bargraphd` holds `9` open descriptors: `0, 1, 2` + `/dev/adc0` + 5 `/dev/pwm*`).
  3. Specify `bargraph test` behavior when `STOPPED` and immediate preemption on `stop_requested`.
  4. Specify exact integer conversion formulas for `millivolts`, `permille`, and `MANUAL` `total_units`.

### 10. `nuttx.elf` Artifact Name in Verification Plan
* **Severity**: 🟢 Nit
* **Affected Scope**: Verification Plan ([line 679](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L679))
* **Discrepancy Description & Rationale**: The ELF binary generated in `nuttx/` is `nuttx/nuttx`, not `nuttx/nuttx.elf`, and `nuttx/nuttx.bin` requires `CONFIG_RAW_BINARY=y`.

---

## Part 2: Pragmatic Tech Lead (`egm-implementation-readiness`) Findings

### 1. Missing `MAINSRC` Entrypoint Naming Contract (`main` vs `bargraph_main` Macro Expansion Collision)
* **Severity**: 🟡 Major
* **Affected Scope**: Target Files 28, 29, and 30 ([lines 615–667](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L615-L667))
* **Finding & Guidance**:
  In [apps/Application.mk](file:///Users/brett/Documents/GitHub/nuttxspace-egm/apps/Application.mk#L353-L357), every C file in `MAINSRC` is compiled with `-Dmain=$(PROGSYM)_main` (`-Dmain=bargraph_main`). Specify in Target Files 29 and 30 whether `bargraph_main.c` defines `int main(int argc, FAR char *argv[])` (rewritten to `bargraph_main` by `-Dmain=bargraph_main`) and avoid conflicting `main` identifiers.

### 2. `CONFIG_RP23XX_UF2_BINARY` & `CONFIG_RAW_BINARY` Alignment Between `defconfig` and Verification Plan
* **Severity**: 🟡 Major
* **Affected Scope**: Target File 7 ([lines 449–466](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L449-L466)) and Verification Plan ([lines 672–680](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L672-L680))
* **Finding & Guidance**:
  Explicitly specify `CONFIG_RAW_BINARY=y` and `CONFIG_RP23XX_UF2_BINARY=y` in Target File 7 (`defconfig`), note the host `picotool` / `PICO_SDK_PATH` requirement, and replace `nuttx/nuttx.elf` with `nuttx/nuttx` on line 679.

### 3. Complete `defconfig` Symbol Specification
* **Severity**: 🟡 Major
* **Affected Scope**: Target File 7 ([lines 449–466](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L449-L466))
* **Finding & Guidance**:
  Provide the complete `defconfig` symbol list (adding `CONFIG_ANALOG=y`, removing non-existent `CONFIG_PWM_MULTICHAN=y`, and removing `CONFIG_ARCH_BOARD="xiao-rp2350"` / `CONFIG_ARCH_BOARD_XIAO_RP2350=y`).

### 4. Unspecified Telemetry Output Format and Integer Formulas in `bargraph_main.c`
* **Severity**: 🟡 Major
* **Affected Scope**: Target File 30 ([lines 641–667](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L641-L667))
* **Finding & Guidance**:
  Provide the exact `printf` format layout for `bargraph status` and the integer formulas for `millivolts`, `permille`, and `total_units` across `SENSING`, `MANUAL`, and `TEST` modes.

### 5. `bargraphd` Static Initialization, First-Sample EMA Seeding, and Synchronization Pattern
* **Severity**: 🟡 Major
* **Affected Scope**: Target Files 29 and 30 ([lines 622–667](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L622-L667))
* **Finding & Guidance**:
  Specify `PTHREAD_MUTEX_INITIALIZER` / `PTHREAD_COND_INITIALIZER` for `g_bargraph`, first-sample seeding of the EMA accumulator, startup failure handling, and the single deterministic 20 ms wait mechanism in `bargraphd_daemon()`.

### 6. Required `#include` Directives in `rp23xx_bringup.c`
* **Severity**: 🟡 Major
* **Affected Scope**: Target File 17 ([lines 535–544](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L535-L544))
* **Finding & Guidance**:
  Explicitly list `#include "arm_internal.h"`, `#include "hardware/rp23xx_pads_bank0.h"`, and `#include "hardware/rp23xx_adc.h"`.

### 7. Superproject Build Artifact Cleanliness (`xiao-rp2350-bargraph/apps/` Build Artifacts)
* **Severity**: 🟢 Nit
* **Affected Scope**: Target Files 23 and 28 ([lines 580–621](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L580-L621))
* **Finding & Guidance**:
  Note that building `apps/external` generates `.depend`, `Make.dep`, `.built`, and `*.o` inside `xiao-rp2350-bargraph/apps/`; either add a `.gitignore` in `xiao-rp2350-bargraph/` or run `make distclean` to clean build artifacts.

### 8. `GPIO_OUT1`/`GPIO_IN1`/`GPIO_IRQPIN1` Pin Collision in `board.h` and `rp23xx_gpio.c`
* **Severity**: 🟢 Nit
* **Affected Scope**: Target Files 9 and 20 ([lines 474–482, 559–565](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L474-L482))
* **Finding & Guidance**:
  Either omit `rp23xx_gpio.c` (`BOARD_NGPIOOUT 0`, `BOARD_NGPIOIN 0`, `BOARD_NGPIOINT 0`) or ensure `GPIO26..28` are not claimed by `rp23xx_gpio.c`.

---

## Part 3: Style & Copy Editor (`egm-style`) Findings (`DetailedRange`)

### 1. Undecided Wait Mechanism in `bargraphd_daemon` Specification
* **Severity**: 🟡 Major
* **Affected Scope**: Target File 30 ([lines 663–664](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L663-L664))
* **Style Rationale (`CALIBRATE`)**: Remove the self-contradictory parenthetical `(`CLOCK_REALTIME` or `usleep` + mutex lock)` after `pthread_cond_timedwait()` and state the single concrete timing mechanism.

### 2. Defensive Caveat and Uncalibrated Artifact Name in Firmware Build Verification
* **Severity**: 🟡 Major
* **Affected Scope**: Verification Plan ([line 679](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L679))
* **Style Rationale (`CALIBRATE`)**: Replace `(when picotool or UF2 conversion is enabled)` with the concrete host prerequisite (`picotool` in `PATH` or `PICO_SDK_PATH` set), and calibrate `nuttx/nuttx.elf` to `nuttx/nuttx`.

### 3. Inconsistent Kconfig Symbol Claim (`ARCH_BOARD_XIAO_RP2350_BARGRAPH`) vs. Hedged `board/Kconfig` Specification
* **Severity**: 🟢 Nit
* **Affected Scope**: Target Files 3 and 4 ([lines 426–434](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L426-L434))
* **Style Rationale (`CALIBRATE`)**: Remove `(or empty Kconfig guard)` in line 433 and align lines 426 and 433 on the exact `board/Kconfig` contents.

### 4. Undecided Linkage Modifier (`weak/conditional`) in `rp23xx_pico.h`
* **Severity**: 🟢 Nit
* **Affected Scope**: Target File 15 ([line 526](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L526))
* **Style Rationale (`CALIBRATE`)**: Drop `"weak/"` and state directly that `int bargraph_autostart(void)` is declared conditionally under `#ifdef CONFIG_BARGRAPH`.

### 5. Hedged `PWMIOC_START` Invocation Policy in `bargraph_apply_pwm`
* **Severity**: 🟢 Nit
* **Affected Scope**: Target File 30 ([line 657](file:///Users/brett/Documents/GitHub/nuttxspace-egm/docs/designs/xiao-rp2350-bargraph.md#L657))
* **Style Rationale (`CALIBRATE`)**: Drop `(when not yet started or on update)` and state the exact `PWMIOC_START` invocation rule.
