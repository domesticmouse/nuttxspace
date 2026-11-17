# Select generated EGM docs and teach-code codelab artifacts to showcase

Type: grilling
Label: wayfinder:grilling
Status: resolved
Blocked by: 01

## Question

Experiment B produced 94 files in `docs/egm/`—from the root `README.md` contract and `2026-11-16_DRIFT_REPORT.md`, through `concepts_brief.md`, `xiao-rp2350-bargraph.md`, and `goldfish_review_report.md`, to the 6-lesson interactive `/teach-code` course in `docs/egm/tutorial/index.html` (with its 3 interactive JS simulators: `breadboard-simulator`, `pwm-segment-visualizer`, `adc-ema-simulator`, 4 reference sheets, and 6 C99 labs).

Which specific generated documents and interactive codelab views should be showcased on stage (and in what order) to support [Narrative arc and 30-minute time budget](./01-narrative-arc-and-time-budget.md) and demonstrate turning AI-assisted codebase ramp-up into durable human documentation?

Specifically, also decide:
- Which concrete artifacts or excerpts from the `/teach` → `/teach-code` skill evolution (via `/agy-customizations`) should be shown alongside the generated EGM docs and `docs/egm/tutorial/` codelab in Act IV (`19:00–25:00`) and Act V (`28:30–30:00`)?

## Answer

### 1. Act III (`08:00–19:00`) Generated EGM Document Trail (*"Docs for the Agent"*)

Pair the Act III.a–III.d cast players with `6` concrete generated artifact moments from `docs/egm/` (and the upstream `README.md` contract baseline) so architectural understanding visibly compounds across Phases 1–5:

1. **Act III.a (`08:00–10:00`, paired with `prototypes/01-bootstrap-cast-player.html` Stop Point 8 freeze-frame)**:
   - Show the 5-section EGM contract anatomy via `nuttx/arch/arm/src/rp23xx/README.md` (`Overview`, `Subdirectories & Boundaries`, `Interface`, `Invariants`, `Verification`) plus the bootstrapped root `docs/egm/README.md` (`28` lines).
   - Pair it with the `11`-line `docs/egm/reports/2026-11-16_DRIFT_REPORT.md` catching `OSINIT_HARDWARE` drift in `nuttx/README.md#L79` vs. `nuttx/sched/init/nx_start.c#L660`.
2. **Act III.b (`10:00–13:00`, paired with `prototypes/02-brainstorm-cast-player.html` Stop Point 16 & `prototypes/03-draft-cast-player.html` Stop Point 6 freeze-frames)**:
   - Show `docs/egm/designs/concepts_brief.md#L16-L35` and `docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md` (`#L33, L242-L243`), highlighting the 7 hardware/driver obstacles surfaced from `34` unique `README.md` reads alongside the authentic false start assuming `RP23XX_ADC_DIV = 0xFFFF00` (`~732 Hz`) would throttle `START_ONCE` interrupts.
3. **Act III.c (`13:00–17:30`, paired with `prototypes/04-review-cast-player.html` live Beats A/B/C)**:
   - Show Pass 1 `docs/egm/reports/pass1/goldfish_review_report.md` (`B-1` proving `RP23XX_ADC_DIV` is a no-op under `START_ONCE`, `B-2` 4x Q4 EMA shift bug, `B-3` missing `CONFIG_ANALOG=y` & non-existent `CONFIG_PWM_MULTICHAN=y`), followed after `/clear` by Pass 2 `docs/egm/reports/pass2/goldfish_review_report.md` (`CA-01` 25-byte `adc_read()` short-read loop) and the resulting Pass 2 hardened spec `docs/egm/designs/xiao-rp2350-bargraph.md`.
4. **Act III.d (`17:30–19:00`, paired with `prototypes/05-implement-cast-player.html` Seg 04 live)**:
   - Show the newly minted out-of-tree `docs/egm/xiao-rp2350-bargraph/README.md` contract (representing the `9` new 5-section contracts rolling the map forward, including documented `CONFIG_BUILD_FLAT=y` exceptions), plus a brief visual flash of `docs/egm/breadboard-wiring.svg` and `docs/egm/BREADBOARD_WIRING.md` (planting the visual seed for the Act V `0.6"` breadboard-straddle retro).

### 2. Act IV (`19:00–25:00`) Codelab Walkthrough Sequence (*"Docs for the Human"*)

After the `~2.0 min` opening skill comparison and `29.8s` live `prototypes/06-teach-code-cast-player.html` Seg 08 playback (Stop Point `8. 6-Lesson Course Generated`), spend `~3.5 min` walking through this `4`-stop browser + code sequence in `docs/egm/tutorial/`:

1. **Stop 1 — Course Landing & Pedagogical Grounding (`~45s`)**:
   - Open the compiled landing page (`docs/egm/tutorial/index.md` → `index.html`) showing the 6-lesson Mermaid story arc, `{{widget:lesson-cards}}`, and `{{widget:reference-cards}}`, paired with `docs/egm/tutorial/MISSION.md` and `docs/egm/tutorial/learning-records/0001-initial-architecture-and-driver-traps.md` (ADR-style learning record calculating the learner's Zone of Proximal Development).
2. **Stop 2 — Interactive Simulator & Tufte Lesson Deep-Dive (`~90s`)**:
   - Open **Lesson 0003** (`docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md`) to demo the live `{{widget:adc-ema-simulator}}` in `docs/egm/tutorial/assets/course.js` (letting the human visually see why discarding `msgs[0]` and rounding `(ema_q4 + 8) >> 4` fixes Pass 1's `B-2` 4x gain bug), alongside Tufte right-margin sidenotes, fill-in-the-blank `:::exercise` (`keep-markup` Prism inputs), and 4-option equal-word-count `:::quiz` (`10` words per option)—with a 10-second visual flash of the other two interactive simulators (`{{widget:breadboard-simulator}}` in `docs/egm/tutorial/lessons/0001-breadboard-circuit-and-current-budget.md#L63` and `{{widget:pwm-segment-visualizer}}` in `docs/egm/tutorial/lessons/0002-proportional-bargraph-math-and-pwm-scaling.md#L47`).
3. **Stop 3 — Hardened Hardware/RTOS Lesson + Printable Reference Sheet (`~45s`)**:
   - Show **Lesson 0005** (`docs/egm/tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md#L58-L69`) explicitly contrasting why `RP23XX_ADC_DIV` fails under `START_ONCE` and teaching NVIC `RP23XX_ADC_IRQ_FIFO` gating + Erratum `RP2350-E9` pad isolation, paired with `docs/egm/tutorial/reference/nuttx-rp2350-driver-traps.md` (`@media print` light-mode override in `docs/egm/tutorial/assets/style.css`).
4. **Stop 4 — Runnable C99 Companion Lab Catching Phase 5 Shortcuts (`~30s`)**:
   - Show **Lesson 0006** (`docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L798`) and session trace `docs/prompts-egm/24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md#L61-L65`, demonstrating how compiling real single-source C99 under `-std=c99 -Wall -Wextra -Werror -pedantic` replaced Phase 5's `Makefile` `printf` duplication, caught a `pthread_cond_wait` tick-sync race (`bargraph_wait_next_tick_locked()`), and exposed `bargraph_start_daemon()` clobbering `MANUAL` mode at the start of `bargraph test`.

### 3. Act IV (`~2.0 min`) & Act V.b (`1.5 min`, `28:30–30:00`) `/teach` → `/teach-code` Skill Evolution Artifacts

1. **Act IV Opening (`~2.0 min` before cast player & browser tour) — Pedagogical Backbone vs. Engineering Verification Gates**:
   - Show a side-by-side comparison of Matt Pocock's `.agents/skills/teach/SKILL.md` (`#L12-L53`, `#L110`) and `.agents/skills/teach-code/SKILL.md` (`#L17-L32`, `#L56-L110`):
     - **Kept intact from `/teach`**: `MISSION.md`, `RESOURCES.md`, ADR-style `learning-records/*.md` (Zone of Proximal Development), printable `reference/*` sheets, Edward Tufte layout, and equal-word-count retrieval quizzes (`Fluency vs Storage Strength`).
     - **Added for real-world code courses in `/teach-code`**: Authoring Markdown (`lessons/*.md`) compiled via `docs/egm/tutorial/build.py` (`build.py --check`), backing every lesson with a runnable `apps/<NNNN-slug>/` project verified under strict compiler/linter gates (`-std=c99 -Wall -Wextra -Werror -pedantic` + `make test`), and enforcing byte-for-byte listing parity via `.agents/skills/teach-code/scripts/sync_listing.py`.
2. **Act V.b (`1.5 min`, `28:30–30:00`) — Closing *"Roll Your Own Skills"* Loop**:
   - Show the 6-minute post-recording fixup trace `docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md#L21-L22`, where inspecting the tutorial HTML surfaced `<2 id="breadboard-topology">` (`HEADING_ID_RE` dropping `h` in `<h\2>`) and unrendered `$...$` TeX markup.
   - Show the resulting guardrails in `docs/egm/tutorial/build.py` (`#L37-L38`, `#L737`, `#L761-L770`: `MALFORMED_TAG_RE` + `UNRENDERED_TEX_RE`) paired with `.agents/skills/teach-code/SKILL.md#L93`, evolved via `.gemini/antigravity/builtin/skills/agy-customizations/SKILL.md` (`/agy-customizations`) so every bug fixed today becomes a compiler-like gate for tomorrow's courses.
