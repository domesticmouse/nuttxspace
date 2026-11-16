**What this proposes**
This document proposes an out-of-tree Apache NuttX board support package (`xiao-rp2350-bargraph/board/`) and built-in user-space application (`xiao-rp2350-bargraph/apps/bargraph/`) for a Seeed Studio XIAO RP2350 connected to a 10 kΩ potentiometer (`D0/A0`, `GPIO26`) and six active-high LEDs (`D1`–`D6`). To keep the `nuttx/` and `apps/` submodules completely unmodified, the build uses `CONFIG_ARCH_BOARD_CUSTOM` and a build-time `apps/external` symlink hook in `board/scripts/Make.defs`. At boot, `board_late_initialize()` calls `rp23xx_bringup()`, which registers `/dev/adc0` and five PWM slices (`/dev/pwm0`, `/dev/pwm2`, `/dev/pwm3`, `/dev/pwm5`, `/dev/pwm6`), applies register-level workarounds for RP2350-E9 analog pad leakage and ADC interrupt pacing (`~732 Hz`), and autostarts a 50 Hz background daemon (`bargraphd`) before USB CDC/ACM NSH initialization blocks waiting for a host connection. An interactive `bargraph` NSH CLI (`start`, `stop`, `status`, `test`, `set <0..100>`, `resume`) synchronizes with `bargraphd` through shared flat-build memory (`struct bargraph_state_s`) protected by a mutex and condition variables.

**How the existing system works, per this document**
The upstream `xiao-rp2350:usbnsh` configuration omits `CONFIG_BOARD_LATE_INITIALIZE=y` (preventing `/dev/adc0` and `/dev/pwm*` registration) and blocks inside `nsh_waitusbready()` until a USB host opens `/dev/ttyACM0` and sends three newline/carriage-return bytes. In the upstream RP23xx drivers, `rp23xx_adc.c` runs free-running conversions at 500 kS/s, shifts 12-bit samples left by 19 bits, resets all enabled ADC channel pins to `GPIO_FUNC_NULL` (which clobbers `D1`/`D2` PWM muxing if `ADC_CHANNEL1`/`2` are enabled), and leaves digital input buffers (`IE=1`) enabled on `GPIO26` (triggering Erratum RP2350-E9 leakage). Meanwhile, `rp23xx_pwm.c` and `drivers/timers/pwm.c` have mismatched Kconfig guards (`CONFIG_PWM_NCHANNELS` vs. `CONFIG_RP23XX_PWM_NCHANNELS`) and channel array indexing rules, truncate 100% duty (`b16ONE = 65536`) to `0` due to a `uint16_t` field in `struct rp23xx_pwm_lowerhalf_s`, and freeze GPIO output levels when `PWMIOC_STOP` clears `RP23XX_PWM_CSR_EN`.

---

### Comprehension Findings

#### 1. Undefined Project-Specific Term: "EGM" / "EGM Blueprint" / "Leaf EGM Documentation"
*   **Severity**: Major
*   **Affected Scope**: [xiao-rp2350-bargraph.md](../../designs/xiao-rp2350-bargraph.md#L36), [xiao-rp2350-bargraph.md](../../designs/xiao-rp2350-bargraph.md#L409-L600), [xiao-rp2350-bargraph.md](../../designs/xiao-rp2350-bargraph.md#L691-L693)
*   **Discrepancy Description & Rationale**: The document repeatedly refers to "EGM `README.md` contracts", "Top-Level Workspace EGM Blueprint Documentation", "Leaf EGM Documentation", and bracketed symbol notation such as `[board.h:BOARD_SYS_FREQ]` (lines 472, 488), but never defines what "EGM" stands for or links to a specification explaining the required structure and formatting rules of an EGM `README.md`. A fresh team member cannot know what makes a `README.md` compliant with "EGM" without an inline definition or link.

#### 2. Ambiguous / Vague Prerequisite for `nuttx.uf2` Generation
*   **Severity**: Nit
*   **Affected Scope**: [xiao-rp2350-bargraph.md](../../designs/xiao-rp2350-bargraph.md#L449-L466), [xiao-rp2350-bargraph.md](../../designs/xiao-rp2350-bargraph.md#L679-L685)
*   **Discrepancy Description & Rationale**: In the Verification Plan (line 679), the document states: `Verify nuttx/nuttx.elf, nuttx/nuttx.bin, and nuttx/nuttx.uf2 (when picotool or UF2 conversion is enabled) are generated`, and line 685 instructs the reader to `Flash nuttx.uf2 to the Seeed Studio XIAO RP2350`. However, Section 7 (`defconfig`, lines 449–466) does not mention whether a UF2 Kconfig option is enabled in `defconfig`, nor does the Verification Plan state whether `picotool` must be installed on the host PATH or how `nuttx.uf2` is produced if UF2 conversion is not enabled by default.

#### 3. Misleading ASCII Wire Routing in Breadboard Diagram (`+3.3V Rail` to `Row 7`)
*   **Severity**: Nit
*   **Affected Scope**: [xiao-rp2350-bargraph.md](../../designs/xiao-rp2350-bargraph.md#L171-L185)
*   **Discrepancy Description & Rationale**: In the `400-Tie-Point Breadboard Layout` ASCII diagram, the vertical line descending from `+3.3V Rail (Red +)` on the left side connects via `+---` directly into `Row 7: +---[D2/A2 GPIO28 (7E)]`, even though the note on the right side of Row 7 says `(Jumper Row 7J to Red (+) 3.3V Rail)` for `[(7F) 3V3]`. Visually connecting the left-hand `+3.3V` line into `7E` (`D2/A2`, `GPIO28`) implies wiring the `3.3V` rail to the `D2` PWM output pin.
