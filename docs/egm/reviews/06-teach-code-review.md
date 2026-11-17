# Phase 6 Review (`06-teach-code.cast`): Navigating Comprehension Debt with `/teach-code`

> **Presentation Context**: Phase 6 (installing `/teach-code` via `npx skills add domesticmouse/skills`, generating the 6-lesson interactive hardware + NuttX firmware course in `./tutorial/`, validating `sync_listing.py --check` + `build.py --check` + 6 C99 unit-test suites, and committing `b95aad7` — plus post-recording HTML/TeX fixup session `98ffad8f`) recording for Experiment B (`nuttxspace-egm`), packaged as [`recordings/final/06-teach-code.cast`](../../../recordings/final/06-teach-code.cast) and [`prototypes/06-teach-code-cast-player.html`](../../../prototypes/06-teach-code-cast-player.html).

---

## 1. What This Step Achieved

- **Installed `/teach-code` On Camera**: Ran `npx skills add domesticmouse/skills` (`skills@1.9.0`) inside `~/Documents/GitHub/nuttxspace-egm`, selected `teach-code` with project scope, and installed it cleanly into `.agents/skills/teach-code/` (`skills-lock.json`).
- **Generated a Self-Contained 6-Lesson Interactive Story Arc in `./tutorial/` (`Steps 0–164`)**: Launched `agy` (`Antigravity CLI 1.5.9`, `Gemini 4 Argon (Medium)`) and invoked `/teach-code Build a complete story arc tutorial, teaching me how to build this app and circuit. Please place the complete tutorial series in ./tutorial/`. Built all `33` captured course files across [`MISSION.md`](../tutorial/MISSION.md), [`RESOURCES.md`](../tutorial/RESOURCES.md), [`NOTES.md`](../tutorial/NOTES.md), [`README.md`](../tutorial/README.md), [`.gitignore`](../tutorial/.gitignore), [`index.md`](../tutorial/index.md), [`build.py`](../tutorial/build.py), [`0001-initial-architecture-and-driver-traps.md`](../tutorial/learning-records/0001-initial-architecture-and-driver-traps.md), Tufte dark-mode assets ([`style.css`](../tutorial/assets/style.css), [`course.js`](../tutorial/assets/course.js), [`breadboard-wiring.svg`](../tutorial/assets/breadboard-wiring.svg)), `4` printable reference sheets under [`reference/`](../tutorial/reference/GLOSSARY.md), `6` sequential lessons under [`lessons/`](../tutorial/lessons/0001-breadboard-circuit-and-current-budget.md), and `6` progressively grown C99 companion apps (`0001`–`0006`) under [`apps/`](../tutorial/apps/0001-breadboard-circuit-and-current-budget/src/main.c).
- **Verified Parity, Static Site Compilation, Companion C99 Suites & Committed (`Steps 165–178`)**: Passed `sync_listing.py --root tutorial --check`, `build.py --out-dir tutorial/_site --check` (`11` compiled pages: `1` index, `6` lessons, `4` reference sheets), and all `6` companion app `make test` gates (`cc -std=c99 -Wall -Wextra -Werror -pedantic`), updated [`tutorial/.gitignore`](../tutorial/.gitignore) to exclude `apps/*/bargraph_lesson`, removed temporary `tutorial/_site/`, and committed to `nuttxspace-egm` (`main` commit `b95aad7`, `41 files changed, 10,864 insertions(+)`).
- **Post-Recording Off-Camera HTML/TeX Fixup (`98ffad8f`)**: Immediately after recording `06-teach-code.cast`, a `54`-step follow-up session ([`25-98ffad8f`](../../prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md)) fixed a heading-anchor regex bug in [`build.py`](../tutorial/build.py#L737) (`HEADING_ID_RE` previously omitted `h` and emitted `<2 id="...">` tags in rendered HTML), added `MALFORMED_TAG_RE` (`<[0-9]\b|\{#[a-zA-Z0-9_-]+\}`) and `UNRENDERED_TEX_RE` validation to [`build.py`](../tutorial/build.py#L37-L38) [`#L761-L770`](../tutorial/build.py#L761-L770), replaced raw `$...$` TeX expressions with inline code and Unicode across `lessons/0001..0003*.md`, `reference/{GLOSSARY.md,hardware-pinout-and-wiring.md}`, and `NOTES.md`, and updated the `teach-code` skill instructions in both workspaces.

---

## 2. Recorded Cast & Standalone Player

- **Raw Recording**: [`recordings/raw/06-teach-code.cast`](../../../recordings/raw/06-teach-code.cast) (`5.5M`, `asciicast v3`, `120x36` `xterm-ghostty`, `idle_time_limit=2.0s`, header timestamp `1794871011` / `2026-11-17T10:16:51+11:00`) — `28,037` events (`28,036` stdout, `0` stdin, `0` markers, `1` exit), `97m 06.8s` (`5,826.80s`) raw / `42m 15.5s` (`2,535.45s`) at `1x` (`idle=2.0s`).
- **Final Recording**: [`recordings/final/06-teach-code.cast`](../../../recordings/final/06-teach-code.cast) (`5.5M`, `asciicast v3`, `120x36`, `idle_time_limit=2.0s`)
  - **Events**: `28,031` total (`28,020` stdout, `0` stdin, `10` `"m"` markers, `1` exit hold of `3.0s`)
  - **Duration**: `42m 00.5s` (`2,520.48s`) raw / `41m 59.5s` (`2,519.48s`) at `1x` (`idle=2.0s`) / `69.5s` (`1m 09.5s`) cumulative warped playback across `10` adaptive segments (`2x`–`79x`)
  - **Post-Processing**: Stripped `3` `OSC 7` metadata sequences (`sanitize-cast.py`), trimmed `17` trailing exit-prompt events (`press ctrl+d again to exit`, `Resume with -c ...`, and zsh prompt return) with a `3.0s` final hold (`trim-cast.py`), tightened `18` shell paste, interactive `npx skills` selector, `agy` launch, `mwg-guide-*.md` `File access` approval, and prompt/submit pauses (`--tighten-pastes` + `--set-dt` including `#1655` `13.87s->1.00s`, `#1766` `87.75s->1.00s`, and `#26393` `2568.21s->1.00s`), added `10` sequential `"m"` stop points with Stop 1 `[UNREV]` newline split (`mark-cast.py`), and verified `0` remaining trailing exit prompts (`trim-cast.py --info`).
- **Standalone HTML Player**: [`prototypes/06-teach-code-cast-player.html`](../../../prototypes/06-teach-code-cast-player.html) (`539K`, `511,852`-char `EMBEDDED_CAST_GZ_B64`, inline JS verified with `node --check`).
- **Visual / TUI Notes**: All `10` final stop-point frames (`#0009`, `#0090`, `#0111`, `#0135`, `#0139`, `#0169`, `#0171`, `#26400`, `#26406`, `#28028`) were rendered and verified with `pyte` (`uv run --with pyte`). During `0cc3a75f`, the operator approved two TUI `File access` prompts for `/private/var/folders/.../mwg-guide-*.md` (from `modern-web-guidance@latest`) at raw `#1655` and `#1766`, and `git commit` retried once with `BypassSandbox: true` (`Step 174`) after `.git/index.lock` sandbox refusal.

### Stop-Point Schedule (`mark-cast.py --list`)

| # | Event | Raw Time | 1x Time | Segment | Warped Cumul. | Label |
| :---: | :---: | :---: | :---: | :---: | :---: | :--- |
| **1** | `#0010` | `1.49s` | `1.49s` | `0.7s @ 2x` | `0.7s` | `1. Run npx skills add domesticmouse/skills` |
| **2** | `#0091` | `10.57s` | `10.57s` | `4.5s @ 2x` | `5.3s` | `2. Select teach-code Skill` |
| **3** | `#0112` | `13.58s` | `13.58s` | `1.5s @ 2x` | `6.8s` | `3. Confirm Project Installation Scope` |
| **4** | `#0136` | `14.98s` | `14.98s` | `0.7s @ 2x` | `7.5s` | `4. teach-code Installed — Ready to Launch agy` |
| **5** | `#0140` | `16.18s` | `16.18s` | `0.6s @ 2x` | `8.1s` | `5. Launch agy` |
| **6** | `#0170` | `19.36s` | `19.36s` | `1.6s @ 2x` | `9.7s` | `6. Ready for /teach-code Prompt` |
| **7** | `#0172` | `20.36s` | `20.36s` | `0.5s @ 2x` | `10.2s` | `7. Prompt: /teach-code Story Arc in ./tutorial/` |
| **8** | `#26401` | `2370.95s` | `2370.95s` | `29.8s @ 79x` | `39.9s` | `8. 6-Lesson Course Generated — Ready for Verify & Commit Prompt` |
| **9** | `#26407` | `2371.95s` | `2371.95s` | `0.5s @ 2x` | `40.4s` | `9. Prompt: Verify sync_listing.py, build.py & Commit` |
| **10** | `#28029` | `2517.48s` | `2517.48s` | `29.1s @ 5x` | `69.5s` | `10. Tutorial Course Verified & Committed (b95aad7)` |

---

## 3. AGY Trace Telemetry

- **On-Camera Canonical Session (`24-0cc3a75f`)**: `0cc3a75f-3187-413b-90bf-a6f607f93900` (`antigravity-cli`, `Gemini 4 Argon (Medium)`), exported to [`24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md`](../../prompts-egm/24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md).
  - **Timing & Scale**: `2026-11-17T10:17:43+11:00` -> `11:43` (`1:25:53`), `179` total steps (`Steps 0–178`), `2` user turns (`Step 0`, `Step 165`), `0` subagents.
  - **Tool Breakdown (`107` calls)**: `run_command`: `38`, `read_file`: `31` (`2` `README.md` contract reads [`README.md` and `xiao-rp2350-bargraph/README.md`], `11` Phase 5 firmware/board/wiring file reads, `2` design spec reads [`xiao-rp2350-bargraph.md`, `concepts_brief.md`], `8` `teach-code` skill/reference/script reads, `5` `modern-web-guidance` temp-guide reads, `1` `tutorial/.gitignore` read, `2` additional `SKILL.md`/`bargraph_main.c` re-reads), `write_file`: `28`, `edit_file`: `9`, `manage_task`: `1`.
  - **In-Session Self-Corrections**: Caught a deadlock in Lesson 0006's multi-threaded test harness (`Step 133` `manage_task` kill -> `Steps 135–148` added `bargraph_wait_next_tick_locked()` sequence predicate and preserved `MANUAL` mode across `bargraph test` so `main()` passed cleanly) and fixed a hyphenated quiz option word-count mismatch in Lesson 0003 (`Step 156`).
- **Off-Camera Post-Recording Fixup Session (`25-98ffad8f`)**: `98ffad8f-04bf-43bf-b0c3-e6a6c36a0562` (`antigravity-cli`, `Gemini 4 Argon (Medium)`), exported to [`25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md`](../../prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md).
  - **Timing & Scale**: `2026-11-17T12:17` -> `12:23` (`0:06:45`), `54` total steps, `1` user turn (`Step 0`), `0` subagents.
  - **Tool Breakdown (`35` calls)**: `run_command`: `12`, `edit_file`: `12`, `read_file`: `11` (`0` `README.md` reads).
- **Combined Phase 6 Totals**: `2` parent sessions (`0` subagents, `0` aborted takes), `233` steps (`179` on-camera + `54` fixup), `142` tool calls (`run_command`: `50`, `read_file`: `42` [`2` `README.md` contract reads], `write_file`: `28`, `edit_file`: `21`, `manage_task`: `1`).

---

## 4. Review of Generated Docs & Code

### Artifacts Captured (`33` files in [`docs/egm/tutorial/`](../tutorial/README.md))
- **Course Scaffolding & Site Builder (`8` files)**: [`MISSION.md`](../tutorial/MISSION.md), [`RESOURCES.md`](../tutorial/RESOURCES.md), [`NOTES.md`](../tutorial/NOTES.md), [`README.md`](../tutorial/README.md), [`.gitignore`](../tutorial/.gitignore), [`index.md`](../tutorial/index.md), [`build.py`](../tutorial/build.py) (`1,042` lines), and [`0001-initial-architecture-and-driver-traps.md`](../tutorial/learning-records/0001-initial-architecture-and-driver-traps.md).
- **Shared Dark-Mode Assets & Wiring Diagram (`3` files)**: [`assets/style.css`](../tutorial/assets/style.css) (`1,067` lines using `@layer base, layout, components, simulators, utilities, print`, `color-scheme: dark`, `@media print` light override, `scrollbar-color` + `scrollbar-width: thin`), [`assets/course.js`](../tutorial/assets/course.js) (`411` lines powering `breadboard-simulator`, `pwm-segment-visualizer`, `adc-ema-simulator`, Prism.js `keep-markup`, Mermaid dark theme, sidenotes, exercises, quizzes, and persisted verification checklists), and [`assets/breadboard-wiring.svg`](../tutorial/assets/breadboard-wiring.svg).
- **Printable Reference Sheets (`4` files)**: [`GLOSSARY.md`](../tutorial/reference/GLOSSARY.md) -> `glossary.html`, [`hardware-pinout-and-wiring.md`](../tutorial/reference/hardware-pinout-and-wiring.md), [`nuttx-rp2350-driver-traps.md`](../tutorial/reference/nuttx-rp2350-driver-traps.md), and [`verification-and-cli-cheatsheet.md`](../tutorial/reference/verification-and-cli-cheatsheet.md).
- **6-Lesson Story Arc (`lessons/0001`–`0006`) & Companion C99 Apps (`apps/0001`–`0006`, `18` files)**:
  1. [`0001-breadboard-circuit-and-current-budget.md`](../tutorial/lessons/0001-breadboard-circuit-and-current-budget.md) + [`apps/0001.../src/main.c`](../tutorial/apps/0001-breadboard-circuit-and-current-budget/src/main.c) (`115` lines — `single=3939 uA, total=23634 uA`).
  2. [`0002-proportional-bargraph-math-and-pwm-scaling.md`](../tutorial/lessons/0002-proportional-bargraph-math-and-pwm-scaling.md) + [`apps/0002.../src/main.c`](../tutorial/apps/0002-proportional-bargraph-math-and-pwm-scaling/src/main.c) (`176` lines — `max_units=393210, seg3_partial=12345`).
  3. [`0003-q4-ema-filtering-and-adc-extraction.md`](../tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md) + [`apps/0003.../src/main.c`](../tutorial/apps/0003-q4-ema-filtering-and-adc-extraction/src/main.c) (`261` lines — `burst_avg=2003, step1=1250, step2=1438`).
  4. [`0004-multichannel-pwm-routing-and-safe-shutdown.md`](../tutorial/lessons/0004-multichannel-pwm-routing-and-safe-shutdown.md) + [`apps/0004.../src/main.c`](../tutorial/apps/0004-multichannel-pwm-routing-and-safe-shutdown/src/main.c) (`446` lines — `slice3_A=4000, slice3_B=5000`).
  5. [`0005-out-of-tree-bsp-and-adc-irq-gating.md`](../tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md) + [`apps/0005.../src/main.c`](../tutorial/apps/0005-out-of-tree-bsp-and-adc-irq-gating/src/main.c) (`597` lines — `pad=0x90, raw_adc=3006, irq_en=0`).
  6. [`0006-bargraphd-daemon-and-nsh-cli.md`](../tutorial/lessons/0006-bargraphd-daemon-and-nsh-cli.md) + [`apps/0006.../src/main.c`](../tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c) (`958` lines — `state=STOPPED, test_peaks=1`).

### Phase 5 Guardrail & Verification Check (`docs/egm/tutorial/`)
- **Direct C99 Compilation (No Inline `Makefile` Duplication)**: Unlike [`xiao-rp2350-bargraph/Makefile`](../xiao-rp2350-bargraph/Makefile), every companion app `Makefile` compiles `src/main.c` directly with `cc -std=c99 -Wall -Wextra -Werror -pedantic -O2` (`-pthread` on Lesson 0006), and all `6` `make test` suites pass inside `docs/egm/tutorial/`.
- **Pass 2 Hardened Architecture (Not `RP23XX_ADC_DIV`)**: [`0005-out-of-tree-bsp-and-adc-irq-gating.md`](../tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md#L67-L68) explicitly contrasts why `ADC_DIV` has no effect on single-shot `START_ONCE` writes and teaches demand-driven NVIC `RP23XX_ADC_IRQ_FIFO` gating alongside Erratum `RP2350-E9` `GPIO26` pad isolation (`OD=1`, `IE=PUE=PDE=ISO=0`), `5`-sample `read()` loop discarding `msgs[0]`, `(ema_q4 + 8) >> 4` Q4 EMA filtering, and `5`-slice / `6`-channel hardware PWM (`0..65535`).
- **Exact Pinout & `3.3 V` Safety Consistency**: [`hardware-pinout-and-wiring.md`](../tutorial/reference/hardware-pinout-and-wiring.md#L35-L47) leaves unassigned right-header pins `D7`–`D10` out (avoiding the Phase 5 `GPIO2, 4, 3, 1` comment mismatch), keeps `5V` (`Row 5F`) unconnected, and preserves `330 Ω` (`~3.94 mA`/LED, `~23.6 mA` total) and exact breadboard coordinates matching [`breadboard-wiring.svg`](../tutorial/assets/breadboard-wiring.svg).
- **Automated Tutorial Checks Passed After Capture**: Both `sync_listing.py --root docs/egm/tutorial --check` (byte-for-byte parity across all `6` lessons) and `build.py --out-dir docs/egm/tutorial/_site --check` (`11` pages, zero malformed tags, zero unrendered TeX, matching quiz option word counts, valid internal links) exited `0`, and temporary `_site/` was removed cleanly.

### Minor Observation (Non-Blocking)
- **Placeholder External URLs in `tutorial/RESOURCES.md`**: [`docs/egm/tutorial/RESOURCES.md`](../tutorial/RESOURCES.md#L11-L14) links the two local workspace guides (`xiao-rp2350-bargraph/BREADBOARD_WIRING.md` and `docs/designs/xiao-rp2350-bargraph.md`) to `https://github.com/apache/nuttx` as placeholder URLs rather than relative repo paths.

### Verdict
- **Phase 6 Complete (`01-bootstrap` through `06-teach-code` Ready for Final Section 9 Audit)**. All `6` Run 2 casts, `6` standalone HTML players, `25` exported AGY traces, `6` per-phase presentation reviews, and the captured `33`-file `/teach-code` interactive tutorial workspace pass automated verification cleanly.
