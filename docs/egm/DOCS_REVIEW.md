# Run 2 (`nuttxspace-egm`) Phases 0–4 Documentation Review

Technical review of the documentation and design artifacts generated across **Phases 0–4** of **Run 2 (`nuttxspace-egm`)**, archived under [`docs/egm/`](README.md) and [`docs/prompts-egm/`](../prompts-egm/README.md), evaluated against the RP2350 hardware, Apache NuttX's `rp23xx` and `analog` drivers, and [Run 1 (`nuttxspace-vanilla`)](../vanilla/CODE_REVIEW.md).

---

## 1. Inventory of Generated Run 2 Docs & Traces (Phases 0–4)

Across Phases 0–4 (`01-bootstrap.cast` through `04-review.cast`), Run 2 produced **`26` Markdown files** in `docs/egm/` backed by **`22` exported AGY traces** (`5` Elephant parent sessions + `17` Goldfish subagents; `1,524` trajectory steps, `1,052` tool calls, `937` `read_file` calls, and **`205` `README.md` contract reads**):

| Phase | Generated Documents | Role & Key Metrics |
| :--- | :--- | :--- |
| **Phase 0 & 1 (`01-bootstrap`)** | • [`README.md`](README.md)<br>• [`reports/2026-11-16_DRIFT_REPORT.md`](reports/2026-11-16_DRIFT_REPORT.md)<br>• [`reviews/01-bootstrap-review.md`](reviews/01-bootstrap-review.md) | `27`-line root workspace blueprint, `11`-line drift audit report, and Phase 1 presentation review (`72` steps, `40` tool calls, `6` `README.md` reads). |
| **Phase 2 (`02-brainstorm`)** | • [`designs/concepts_brief.md`](designs/concepts_brief.md)<br>• [`reviews/02-brainstorm-review.md`](reviews/02-brainstorm-review.md) | `89`-line concept brief synthesized across 7 Goldfish lenses (`egm-synthesizer`, `34` unique `README.md` contracts read) + Phase 2 review (`343` combined steps, `223` tool calls, `49` `README.md` reads). |
| **Phase 3 (`03-draft`)** | • [`designs/xiao-rp2350-bargraph.v0-draft.md`](designs/xiao-rp2350-bargraph.v0-draft.md)<br>• [`designs/design_draft.md`](designs/design_draft.md)<br>• [`reviews/03-draft-review.md`](reviews/03-draft-review.md) | `693`-line initial 4-section specification (`30` target files) + working draft + Phase 3 review (`107` steps, `64` tool calls, `4` `README.md` reads). |
| **Phase 4 (`04-review`, Pass 1 & Pass 2)** | • [`designs/xiao-rp2350-bargraph.md`](designs/xiao-rp2350-bargraph.md)<br>• [`reports/pass1/`](reports/pass1/goldfish_review_report.md) (`8` files)<br>• [`reports/pass2/`](reports/pass2/goldfish_review_report.md) (`8` files)<br>• [`reviews/04-review-review.md`](reviews/04-review-review.md) | `705`-line Pass 2 hardened spec, `14` stage/consolidated/annotated Goldfish reports (`12` Pass 1 findings + `11` Pass 2 findings), intermediate `pass1`/`pass2` spec snapshots, and Phase 4 review (`1,002` combined steps, `725` tool calls, `146` `README.md` reads). |
| **Session Traces (`01`–`22`)** | • [`docs/prompts-egm/README.md`](../prompts-egm/README.md) (`01`–`22`) | Full transcripts of all `5` Elephant parent sessions and `17` Goldfish subagent conversations across Phases 1–4. |

---

## 2. Architectural Self-Correction Across Phases 1–4

Preserving every intermediate snapshot (`concepts_brief.md` -> `v0-draft` -> `pass1` -> `pass2`) provides an auditable record of how EGM caught latent hardware, driver, Kconfig, and math defects before any C code was written:

1. **Phase 1 ([`README.md`](README.md) + [`reports/2026-11-16_DRIFT_REPORT.md`](reports/2026-11-16_DRIFT_REPORT.md))**
   - Rolled up the superproject build boundary (`CONFIG_APPS_DIR="../apps"`, `NUTTX_APPS_DIR`, `libapps.a`, and kernel/user-space separation in `INVIOLABLES.md`) across `43` level-1 child contracts without modifying `nuttx/` (`2,607` `README.md` files) or `apps/` (`634` `README.md` files).
   - Running `/egm-drift-report` caught that [`nuttx/README.md`](../../nuttx/README.md#L79) omitted `OSINIT_HARDWARE` from the `g_nx_initstate` progression (`g_nx_initstate = OSINIT_HARDWARE;` in [`nuttx/sched/init/nx_start.c`](../../nuttx/sched/init/nx_start.c#L660)).

2. **Phase 2 ([`designs/concepts_brief.md`](designs/concepts_brief.md))**
   - Six-turn one-concern-at-a-time discovery plus `egm-synthesizer` across 7 Goldfish lenses (`34` unique `README.md` contracts read) surfaced 7 hardware/driver obstacles before specification drafting:
     - Missing `CONFIG_BOARD_LATE_INITIALIZE=y` in upstream `xiao-rp2350:usbnsh`.
     - Hardcoded `pwm_info_s` channel indexing in `rp23xx_pwm.c` (`channels[0]` -> A, `channels[1]` -> B) and shared `/dev/pwm3` slice (`D4` = `PWM3A`, `D5` = `PWM3B`).
     - Dual multi-channel PWM Kconfig guards (`CONFIG_PWM_NCHANNELS=2` vs. `CONFIG_RP23XX_PWM_MULTICHAN` / `CONFIG_RP23XX_PWM_NCHANNELS`).
     - 16-bit duty truncation (`b16ONE` = `65536` wrapping to `0x0000` in `uint16_t`).
     - `PWMIOC_STOP` freezing GPIO output levels unless duty is zeroed and closed.
     - `my_reset()` pin-mux conflict on `GPIO26..29` plus RP2350-E9 digital input buffer (`IE=1`) leakage on `GPIO26`.
     - `<< 19` left shift and continuous `500 kS/s` ADC IRQ behavior in `rp23xx_adc.c`.

3. **Phase 3 ([`designs/xiao-rp2350-bargraph.v0-draft.md`](designs/xiao-rp2350-bargraph.v0-draft.md))**
   - Expanded `concepts_brief.md` into a `693`-line 4-section specification covering out-of-tree `CONFIG_ARCH_BOARD_CUSTOM` integration, `apps/external` symlink discovery, `400`-tie-point breadboard layout, electrical current budget (`~3.94 mA`/LED, `~23.6 mA` total), `bargraphd` state machine, and a `30`-file manifest.
   - Left 4 blockers and 5 major issues in the draft that Phase 4's adversarial Goldfish passes caught.

4. **Phase 4 Pass 1 ([`reports/pass1/goldfish_review_report.md`](reports/pass1/goldfish_review_report.md) — `10` Goldfish subagents, `104` `README.md` reads)**
   - **`B-1` (`RP23XX_ADC_DIV` is a no-op under `START_ONCE`)**: Verified in [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L169-L320) that `interrupt_handler()` unconditionally calls `get_next_channel()`, which writes single-shot `RP23XX_ADC_CS_START_ONCE` (`2 µs`/sample) even when `au_receive()` returns `-ENOMEM`. Because `RP23XX_ADC_DIV` only paces free-running `START_MANY` conversions, Pass 1 replaced `RP23XX_ADC_DIV` with demand-driven NVIC IRQ gating (`up_disable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_enable_irq(RP23XX_ADC_IRQ_FIFO)`), cutting background IRQs during the 20 ms sleep window to zero (`250 IRQ/s` total).
   - **`B-2` (4x Q4 EMA Gain Bug)**: Caught `filtered = ema_q4 >> 2` scaling $R \in [0, 4095]$ to $4R \in [0, 16380]$ (saturating all 6 LEDs at 25% pot travel) and fixed it to `(ema_q4 + 8) >> 4` with first-sample seeding (`ema_q4 = raw_avg << 4`).
   - **`B-3` (Build-Breaking Kconfig Symbols)**: Removed non-existent `CONFIG_PWM_MULTICHAN=y` (which aborts `olddefconfig` with exit 1) and added `CONFIG_ANALOG=y` (the missing parent dependency that silently dropped `CONFIG_ADC=y` in Run 1 Vanilla at Step 180).
   - **`B-4` & `M-1`–`M-5`**: Guarded `apps/external` symlink creation against `distclean clean_context apps_distclean` (`M-2`), removed ~750 lines of unused board files including `rp23xx_gpio.c` colliding on `GPIO26..28` (`M-3`), simplified PWM shutdown to zero-duty + `PWMIOC_STOP` + `close()` (`pwm_shutdown()`, `M-3`), added `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=12` and `CONFIG_RP23XX_UF2_BINARY=y` (`M-4`), and moved `struct bargraph_state_s` into `DetailedRange` (`M-5`).

5. **Phase 4 Pass 2 ([`reports/pass2/goldfish_review_report.md`](reports/pass2/goldfish_review_report.md) — `6` fresh Goldfish subagents after `/clear`, `42` `README.md` reads)**
   - **`CA-01` (`adc_read()` Short-Read Trap)**: Fresh auditors after `/clear` proved from [`nuttx/drivers/analog/adc.c`](../../nuttx/drivers/analog/adc.c#L270-L408) that `adc_read()` wakes from `nxsem_wait(&fifo->af_sem)` and returns as soon as **1** `struct adc_msg_s` (`5` bytes) is queued. Masking `RP23XX_ADC_IRQ_FIFO` after a single `read()` would have left `msgs[1..4]` uninitialized; `bargraph_sample_adc()` now loops on `read()` until all `25` bytes (`5` samples) arrive before masking the IRQ.
   - **`CA-02`–`CA-11`**: Documented the intentional `CONFIG_BUILD_FLAT=y` Kernel/User-Space exception (`CA-02`), standardized 5-section EGM `README.md` specs across all 9 target directories (`CA-03`), unified `bargraph_start_daemon()` (`CA-04`), hardened `TEST` preemption on `set`/`resume` (`CA-05`), trimmed `board.h` to `BARGRAPH_ADC_GPIO 26` (`CA-07`), and removed `-l` from `configure.sh` (`CA-10`).

---

## 3. Remaining Spec Gaps, Link Fixes & Guardrails for Phases 5–6

### 1. Missing `#include <nuttx/arch.h>` in Target File #28 ([`designs/xiao-rp2350-bargraph.md#L656`](designs/xiao-rp2350-bargraph.md#L656))
- **Spec says**: Target File #28 (`bargraph_main.c`) instructs to *"Include `<nuttx/irq.h>` and `<arch/rp23xx/irq.h>`"* for `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` and `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`.
- **Codebase requires**:
  1. `up_enable_irq(int irq)` and `up_disable_irq(int irq)` are declared in [`nuttx/include/nuttx/arch.h`](../../nuttx/include/nuttx/arch.h#L1856) (documented in [`nuttx/include/nuttx/README.md`](../../nuttx/include/nuttx/README.md#L35) as `[arch.h:up_enable_irq]`), **not** `<nuttx/irq.h>`.
  2. [`nuttx/arch/arm/include/rp23xx/irq.h`](../../nuttx/arch/arm/include/rp23xx/irq.h#L23-L25) explicitly notes: *"This file should never be included directly but, rather, only indirectly through nuttx/irq.h"*.
- **Phase 5 Impact**: `bargraph_main.c` must include `<nuttx/arch.h>` alongside `<nuttx/irq.h>` so `up_enable_irq()` and `up_disable_irq()` compile without implicit-declaration warnings or errors.

### 2. Ephemeral `/tmp` Host Test in Spec vs. Phase 5 `Makefile` Requirement ([`designs/xiao-rp2350-bargraph.md#L85-L121`](designs/xiao-rp2350-bargraph.md#L85-L121), [`#L694-L695`](designs/xiao-rp2350-bargraph.md#L694-L695))
- Because the Phase 2 prompt omitted the top-level `Makefile` requirement, the `28`-file manifest in `xiao-rp2350-bargraph.md` omits `xiao-rp2350-bargraph/Makefile` (`CA-11` uses an ephemeral host `cc` check in `/tmp`).
- Because the Phase 5 prompt explicitly requires `make -C xiao-rp2350-bargraph test` and `make -C xiao-rp2350-bargraph` (`nuttx/nuttx.uf2`), Phase 5 adds `xiao-rp2350-bargraph/Makefile` and a persistent host unit test target beyond the 28 files enumerated in the spec (already flagged in [`reviews/04-review-review.md`](reviews/04-review-review.md#L103-L104) and [`EXPERIMENT_RUNBOOK.md`](../../EXPERIMENT_RUNBOOK.md#L649)).

### 3. Superseded `RP23XX_ADC_DIV` Guidance in [`designs/concepts_brief.md#L34`](designs/concepts_brief.md#L34)
- [`designs/concepts_brief.md`](designs/concepts_brief.md#L34) (the authentic Phase 2 snapshot) still describes setting `RP23XX_ADC_DIV = 0xFFFF00` (`~732 Hz`), which Phase 4 Pass 1 (`B-1`) disproved.
- **Phase 6 (`/teach-code`) Guardrail**: Keep `concepts_brief.md` unchanged as a historical artifact, and verify that `./tutorial/` teaches the Pass 2 hardened design from [`designs/xiao-rp2350-bargraph.md`](designs/xiao-rp2350-bargraph.md#L242-L252) (demand-driven NVIC `RP23XX_ADC_IRQ_FIFO` gating + 5-sample `read()` loop + Q4 EMA `(ema_q4 + 8) >> 4` + 5-slice/6-channel hardware PWM).

### 4. Exported Markdown Link Normalization (Resolved)
- When captured from `nuttxspace-egm`, `README.md`, `designs/*.md`, `reports/pass1/*`, `reports/pass2/*`, `reviews/01-bootstrap-review.md`, `docs/prompts-egm/README.md`, and Run 1's `docs/vanilla/BREADBOARD_WIRING.md` / `docs/prompts-vanilla/README.md` contained local `file:///Users/...` URLs and relative-depth links pointing to `nuttxspace-egm` paths.
- All `525` file and relative links across `docs/egm/`, `docs/vanilla/`, [`docs/prompts-egm/README.md`](../prompts-egm/README.md), and [`docs/prompts-vanilla/README.md`](../prompts-vanilla/README.md) were normalized to portable relative repository links (`0` `file://` links and `0` broken relative links remaining), and [`.agents/skills/agy-brain-explorer/scripts/agy-brain-explorer.py`](../../.agents/skills/agy-brain-explorer/scripts/agy-brain-explorer.py#L1920-L1922) was updated to emit relative links in future index exports.

---

## 4. Run 1 Implementation vs. Run 2 Hardened Pass 2 Spec

| Dimension | Run 1 (`nuttxspace-vanilla`, [`CODE_REVIEW.md`](../vanilla/CODE_REVIEW.md)) | Run 2 Hardened Spec ([`xiao-rp2350-bargraph.md`](designs/xiao-rp2350-bargraph.md)) |
| :--- | :--- | :--- |
| **Display Architecture** | Discrete on/off `/dev/userleds` (`7` levels on `D3–D8`) | Proportional `1 kHz` hardware PWM across 5 slices (`D1–D6`: `PWM5B`, `PWM6A`, `PWM2B`, `PWM3A`, `PWM3B`, `PWM0A`) with `0..65535` `uint16_t`-safe duty scaling |
| **`GPIO 26–29` `my_reset()` Pin Clobber** | Avoided (`LEDs` on `D3–D8`) | Avoided (`CONFIG_RP23XX_ADC_CHANNEL0=y` only; `pwm_setup()` muxes `D1`/`D2` to `GPIO_FUNC_PWM` when `bargraphd` opens `/dev/pwm5` & `/dev/pwm6` after bringup) |
| **RP2350-E9 `GPIO26` `IE=1` Pad Leakage** | **Missed** (`IE=1` left active after `adc_register()`) | Mitigated in `rp23xx_bringup()` (`IE=0`, `PUE=0`, `PDE=0`, `ISO=0`, `OD=1` on `GPIO26`) |
| **500 kHz `START_ONCE` ADC IRQ Storm** | Per-tick `open`/`read`/`close` at 40 Hz | Persistent `/dev/adc0` with demand-driven NVIC gating (`up_disable_irq` / `up_enable_irq` on `RP23XX_ADC_IRQ_FIFO`, `250 IRQ/s`) |
| **`adc_close()` / `remove_device()` ISR Race & Short Read** | **Vulnerable** (bare `close(fd)` without critical section; blocking `read()`; errors swallowed) | Avoids per-tick `close()`; loops on `read()` until all `25` bytes (`5` samples) arrive, discards `msgs[0]`, checks `am_data >= 0`, unmasks IRQ before final `close()` |
| **Kconfig & Pin Hygiene** | Hit silent `CONFIG_ADC` drop (Step 180); left `CONFIG_DEV_GPIO=y` duplicating LED pins; copied 10 unused files | `CONFIG_ANALOG=y`, `CONFIG_PWM_NCHANNELS=2` + `CONFIG_RP23XX_PWM_MULTICHAN=y` (no invalid `CONFIG_PWM_MULTICHAN`), unused sibling PWM pins `-1`, unused board C files removed |
| **Boot Sequencing & Thread Safety** | Dual autostart in `rp23xx_bringup()` + hand-rolled `rcS`; unguarded `g_bargraph_state` data race | Single `bargraph_autostart()` -> `bargraph_start_daemon()` before `nsh_waitusbready()`; `pthread_mutex_t` + `pthread_cond_t` state machine (`SENSING`/`MANUAL`/`TEST`/`STOPPED`) |
| **Clean Shutdown on `bargraph stop`** | **Partial** (`led_fd` closed without zeroing LEDs) | Writes `duty = 0`, calls `PWMIOC_STOP`, and calls `close()` (`pwm_shutdown()` -> `SIO` low) |
| **Planning & Review Trail** | `0` planning/review docs; `0` `nuttx`/`apps` `README.md` reads | `26` docs in `docs/egm/`; `205` `README.md` reads across `22` traces (Phases 1–4) |
