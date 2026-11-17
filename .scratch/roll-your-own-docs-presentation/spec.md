# Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up — Presentation Specification

> **Destination Status:** Complete presentation specification and spoken speaker narrative (`S001`–`S214`, `30:00` / `1,800.0s`), compiled from [outline-prototype.md](./outline-prototype.md), [Exact Visual Callouts & Code Excerpts](./issues/06-exact-visual-callouts-and-code-excerpts.md), and [Speaker Narrative & Transitions](./issues/07-speaker-narrative-and-transitions.md).

---

## 1. Executive Overview, Foundational Lineage & Format Contract

- **Title**: *"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"*
- **Speaker**: Brett Morgan (`bjm-voice`: Australian English, curious-explorer / experienced-peer engineering tone, candid about false starts and strict post-audit warts)
- **Target Audience & Runtime**: `30:00` (`1,800.0s`) conference talk for general software engineers; **no prior Apache NuttX or RP2350 hardware knowledge assumed**.
- **Core Thesis — Three Layers of *"Roll Your Own Docs"***:
  1. **Docs for the Agent (Elephant & Goldfish)**: Replacing `13` upstream `nuttx/` + `apps/` `README.md` files with `3,241` bottom-up directory `README.md` contracts (`2,607` in `nuttx/`, `634` in `apps/`) plus EGM drift, brainstorm, draft, and two-pass review specs before writing a line of C.
  2. **Docs for the Human (Who Owns the Outcome)**: Transforming hard-won architectural knowledge into a 6-lesson interactive Edward Tufte codelab (`docs/egm/tutorial/index.md` compiled to `docs/egm/tutorial/index.html`) via `/teach-code`, complete with `3` interactive browser simulators, `4` printable bench reference sheets, equal-word-count retrieval quizzes, and `6` strict C99 companion labs (`docs/egm/tutorial/apps/0001-breadboard-circuit-and-current-budget/` through `0006-bargraphd-daemon-and-nsh-cli/`).
  3. **Docs for the Workflow (Skills That Evolve)**: Starting from Matt Pocock's open-source `/teach` skill (`.agents/skills/teach/SKILL.md`) and evolving it through real-world use and `/agy-customizations` (`/Users/brett/.gemini/antigravity/builtin/skills/agy-customizations/SKILL.md`) into `/teach-code` (`.agents/skills/teach-code/SKILL.md`), folding every production edge case back into deterministic compiler and HTML gates (`build.py --check`, `scripts/sync_listing.py`).
- **Foundational Lineage Credits**:
  - **Dave Rensin (`@drensin`)**: [*"Elephants, Goldfish, and the New Golden Age of Software Engineering"*](https://drensin.medium.com/elephants-goldfish-and-the-new-golden-age-of-software-engineering-c33641a48874) (`https://github.com/google/elephant-goldfish-model`, `/Users/brett/.gemini/config/plugins/egm/docs/index.md#L18-L85`) — *"Design is the new code"*, *"Design with an elephant. Review with a goldfish."*, **Weeds Syndrome** (*Premature Implementation*), **Context Pollution & Sycophancy**, **The Elephant** (Steps 1–4), **The Goldfish** (Steps 5–9), and **The Human** (*"Humans own the outcomes"*).
  - **Matt Pocock**: Open-source `/teach` skill (`.agents/skills/teach/SKILL.md#L12-L53, L110`) contributing `MISSION.md`, `RESOURCES.md`, ADR-style `learning-records/*.md` (Zone of Proximal Development), Tufte layout ergonomics, and equal-word-count retrieval quizzes (*Fluency vs. Storage Strength*).
- **Slide Density & Visual Cadence Contract**:
  - **`214` total stage steps (`S001`–`S214`)** across **`26` narrative beats (`Beat 01`–`Beat 26`)** in **`5` acts**.
  - **`208` low-density visual transitions** across `1,618.4s` (`26m 58.4s`), strictly **`7.0s–9.0s`** each (**`7.8s` average**), combining `96 [Slide]`, `88 [v-click]`, and `24 [Code-step]` progressions (`3–8` lines per code block, single headline/stat/card/SVG focus per slide).
  - **`6 [Live-cast]` terminal playback beats** totalling **`181.6s` (`3m 02s`)** across `4` standalone HTML cast players (`102` total Slidev deck slides = `96 [Slide]` + `6 [Live-cast]`), plus `3` chapter-pill **freeze-frame players** (`0s` playback).
  - During every `[Live-cast]` window and freeze-frame, spoken commentary uses **prompt-and-effect framing** (the prompt or slash command issued and its behavioural/architectural impact on the session—never reading scrolling terminal lines off the screen).

---

## 2. Timing Summary by Act & Live Cast-Player Choreography

### 2.1 Timing Summary by Act (`30:00` / `1,800.0s`)

| Act | Section | Time Window | Budget | Beats | Stage Steps | Visual Steps (Avg) | Live Cast Playback | Freeze-Frames |
| :--- | :--- | :---: | ---: | :---: | :---: | :---: | ---: | :---: |
| **Act I** | The Hook & The Title's Double Meaning | `00:00–03:00` | `3.0 min` (`180.0s`) | `01–03` | `S001–S023` (`23`) | `23` (`7.8s`) | `0s` | `—` |
| **Act II** | The Foil: Flying Blind with Vanilla Vibe Coding | `03:00–08:00` | `5.0 min` (`300.0s`) | `04–07` | `S024–S057` (`34`) | `33` (`7.7s`) | `44.4s` (`vanilla-vibe`) | `—` |
| **Act III.a** | Grounding the Map (`01-bootstrap` & Drift Report) | `08:00–10:00` | `2.0 min` (`120.0s`) | `08–09` | `S058–S073` (`16`) | `16` (`7.5s`) | `0s` | `1` (`01-bootstrap`) |
| **Act III.b** | From Vibe to Architecture (`02-brainstorm` & `03-draft`) | `10:00–13:00` | `3.0 min` (`180.0s`) | `10–11` | `S074–S095` (`22`) | `22` (`8.2s`) | `0s` | `2` (`02-brainstorm`, `03-draft`) |
| **Act III.c** | The Goldfish Swarm (`04-review` Pass 1 + `/clear` + Pass 2) | `13:00–17:30` | `4.5 min` (`270.0s`) | `12–15` | `S096–S123` (`28`) | `25` (`7.7s`) | `78.4s` (`04-review`) | `—` |
| **Act III.d** | Boring Implementation (`05-implement`) | `17:30–19:00` | `1.5 min` (`90.0s`) | `16` | `S124–S132` (`9`) | `8` (`7.6s`) | `29.0s` (`05-implement`) | `—` |
| **Act IV** | Rolling Docs for the Human: The `/teach-code` Codelab | `19:00–25:00` | `6.0 min` (`360.0s`) | `17–22` | `S133–S176` (`44`) | `43` (`7.7s`) | `29.8s` (`06-teach-code`) | `—` |
| **Act V.a** | Head-to-Head Telemetry & Shared Blind Spots | `25:00–28:30` | `3.5 min` (`210.0s`) | `23–25` | `S177–S203` (`27`) | `27` (`7.8s`) | `0s` | `—` |
| **Act V.b** | Takeaway: Evolving Your Own `SKILL.md` Workflow | `28:30–30:00` | `1.5 min` (`90.0s`) | `26` | `S204–S214` (`11`) | `11` (`8.2s`) | `0s` | `—` |
| **Total** | — | **`00:00–30:00`** | **`30.0 min` (`1,800.0s`)** | **`26` beats** | **`214` steps** | **`208` (`7.8s` avg)** | **`3m 02s` (`181.6s`) across `4` players** | **`3` players** |

### 2.2 Seven-Player Cast Choreography (`prototypes/`)

| Player | Act & Step | Stop Points | Raw Duration | Stage Mode & Exact Cue |
| :--- | :--- | ---: | ---: | :--- |
| `prototypes/vanilla-vibe-cast-player.html` | Act II (`S026`) | 7 | `31m 57s` (`1,916.6s`) | **Live (`44.4s`)**: Jump (`]`) to Stop Point `2` (`00:02`), press `p` through Seg `03` (`14.1s @ 2x`), Seg `04` (`0.5s @ 2x`), Seg `05` (`29.8s @ 52x` -> auto-pause at `5`). |
| `prototypes/01-bootstrap-cast-player.html` | Act III.a (`S066`) | 11 | `8m 14s` (`493.6s`) | **Freeze-frame (`0s`)**: Stop Point `8. Q1: Run EGM Drift Report?` (`00:38` warped, `7x` badge). |
| `prototypes/02-brainstorm-cast-player.html` | Act III.b (`S075`) | 19 | `41m 32s` (`2,491.7s`) | **Freeze-frame (`0s`)**: Stop Point `16. Q7: Readiness Checkpoint (Review with Goldfishes)` (`02:53` warped, 19-pill top bar). |
| `prototypes/03-draft-cast-player.html` | Act III.b (`S086`) | 11 | `21m 35s` (`1,295.4s`) | **Freeze-frame (`0s`)**: Stop Point `6. Q3: Section Approval — Technical Plan` (`01:30` warped, `15x` badge). |
| `prototypes/04-review-cast-player.html` | Act III.c (`S099`, `S104`, `S119`) | 24 | `62m 13s` (`3,733.1s`) | **Live (`78.4s` across 3 beats)**: **Beat A (`S099`, `29.7s`)** Stop Point `4` -> Seg `05` (`29.7s @ 39x` -> `5`); **Beat B (`S104`, `18.6s`)** Pill `8` -> Seg `09` (`17.1s @ 2x` -> `9. Item 1/12 (B-1)`) + Seg `10` (`1.5s @ 2x` -> `10. Reply: accept`); **Beat C (`S119`, `30.1s`)** Pill `13` -> Seg `14–16` (`1.1s @ 2x` `/clear`) -> Pill `17` -> Seg `18` (`29.0s @ 26x` -> `18`). |
| `prototypes/05-implement-cast-player.html` | Act III.d (`S125`) | 8 | `21m 03s` (`1,262.7s`) | **Live (`29.0s`)**: Jump to Stop Point `3` (`00:04`), press `p` through Seg `04` (`29.0s @ 27x` -> auto-pause at `4. Firmware Built & Tested`). |
| `prototypes/06-teach-code-cast-player.html` | Act IV (`S150`) | 10 | `42m 01s` (`2,520.5s`) | **Live (`29.8s`)**: Jump to Stop Point `7` (`00:10`), press `p` through Seg `08` (`29.8s @ 79x` -> auto-pause at `8. 6-Lesson Course Generated`). |

---

## 3. Beat-by-Beat & Step-by-Step Presentation Specification (`S001`–`S214`)

---

### Act I — The Hook & The Title's Double Meaning (`00:00–03:00`, `3.0 min`, Beats `01–03`, `S001–S023`)

#### Beat 01 (`00:00–01:00`, `60.0s`, `8` steps) — Title & The Naked Hardware/RTOS Cliff

- **`S001` (`00:00–00:08`, `8.0s`) `[Slide]`**: **Title Card** — *"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"* + Brett Morgan.
  - **Visual / Artefact Callout**: Full-bleed dark Slidev title card (`Roll Your Own Docs`, subtitle `Custom AI Skills for Rapid Tech Ramp-Up`, author `Brett Morgan`).
  - **Spoken (`bjm-voice`)**: G'day everyone. Tonight is about dropping a coding agent into an unfamiliar legacy codebase and helping you both understand what actually ships.

- **`S002` (`00:08–00:16`, `8.0s`) `[Slide]`**: **Hardware Preview** — Seeed Studio XIAO RP2350 + 6-LED breadboard (`docs/egm/breadboard-wiring.svg`).
  - **Visual / Artefact Callout**: Full `viewBox="0 0 1160 1120"` overview of [`docs/egm/breadboard-wiring.svg`](../../docs/egm/breadboard-wiring.svg) showing the XIAO RP2350, `10 kΩ` potentiometer on `Rows 1–3`, and six LEDs on `Rows 14–19`.
  - **Spoken (`bjm-voice`)**: It started with weekend hardware curiosity: a tiny AU$ Seeed Studio XIAO RP2350 board on a solderless breadboard beside my laptop.

- **`S003` (`00:16–00:23`, `7.0s`) `[v-click]`**: **Hardware Spec Callout** — Dual-core Cortex-M33 silicon (`RP2350`), potentiometer on `D0/A0`, and 6 PWM LEDs.
  - **Visual / Artefact Callout**: Overlay pinout pills over [`docs/egm/breadboard-wiring.svg`](../../docs/egm/breadboard-wiring.svg) (`viewBox="0 0 1160 1120"`): `D0/A0` (`GPIO 26`, `0–3.3 V`) and `6` PWM LEDs (`330 Ω`, `~3.94 mA` each).
  - **Spoken (`bjm-voice`)**: Nothing exotic on the bench—just one potentiometer wired into `D0/A0` driving a smooth six-LED bar graph via hardware PWM.

- **`S004` (`00:23–00:30`, `7.0s`) `[Slide]`**: **The RTOS Scale Shock** — **Apache NuttX**: `30` years of legacy C across `nuttx/` and `apps/`.
  - **Visual / Artefact Callout**: Clean split-directory hero badge: [`nuttx/`](../../nuttx/README.md) + [`apps/`](../../apps/README.md) (`Apache NuttX` POSIX RTOS).
  - **Spoken (`bjm-voice`)**: The catch is running it inside Apache NuttX, thirty years of deep POSIX RTOS C that I have definitely not memorised.

- **`S005` (`00:30–00:38`, `8.0s`) `[Slide]`**: **Baseline #1 (Big Number)** — Upstream `nuttx` + `apps` `master`: **`13` `README.md` files**.
  - **Visual / Artefact Callout**: Single oversized numeral stat card (`13` `README.md` files across upstream `nuttx` `master` + `apps` `master`), verified in [`docs/reports/vanilla-vs-egm-comparison.md#L33`](../../docs/reports/vanilla-vs-egm-comparison.md#L33).
  - **Spoken (`bjm-voice`)**: Look at upstream `master` across those two submodules: thirteen `README.md` files total. Drop an agent there cold and it flies blind.

- **`S006` (`00:38–00:45`, `7.0s`) `[v-click]`**: **Baseline #2 (Big Contrast)** — Our `egm` baseline: **`3,241` `README.md` contracts** (`2,607` in `nuttx/`, `634` in `apps/`).
  - **Visual / Artefact Callout**: Side-by-side stat reveal adding `3,241` `README.md` contracts (`2,607` in `nuttx/` + `634` in `apps/`), sourced from [`docs/reports/vanilla-vs-egm-comparison.md#L33`](../../docs/reports/vanilla-vs-egm-comparison.md#L33).
  - **Spoken (`bjm-voice`)**: Now compare that against our contract-mapped branch: three thousand, two hundred and forty-one directory `README.md` contracts waiting to be navigated.

- **`S007` (`00:45–00:53`, `8.0s`) `[Slide]`**: **The Hook Contrast** — *"The joy is new silicon on your desk; the pain is your coding agent waking up blind."*
  - **Visual / Artefact Callout**: Two-line typographic contrast quote card (*Joy* vs. *Pain*).
  - **Spoken (`bjm-voice`)**: The joy is fresh silicon on your desk; the pain is watching your AI accomplice wake up blind inside legacy C.

- **`S008` (`00:53–01:00`, `7.0s`) `[v-click]`**: **Experiment Teaser** — Same hardware, same prompt, two runs (`nuttxspace-vanilla` vs. `nuttxspace-egm`).
  - **Visual / Artefact Callout**: Dual experiment badges citing [`EXPERIMENT_RUNBOOK.md`](../../EXPERIMENT_RUNBOOK.md) (`Experiment A: nuttxspace-vanilla` on `13` docs vs. `Experiment B: nuttxspace-egm` on `3,241` docs).
  - **Spoken (`bjm-voice`)**: So I ran a controlled experiment: exact same RP2350 hardware, exact same prompt, recorded end-to-end across two separate workspaces.

#### Beat 02 (`01:00–02:00`, `60.0s`, `8` steps) — The Identical Prompt & The Lie of "It Compiled!"

- **`S009` (`01:00–01:08`, `8.0s`) `[Slide]`**: **The Shared Prompt Card** — Functional brief from `EXPERIMENT_RUNBOOK.md` handed verbatim to both workspaces.
  - **Visual / Artefact Callout**: Prompt summary card referencing [`EXPERIMENT_RUNBOOK.md`](../../EXPERIMENT_RUNBOOK.md) and [`docs/reports/vanilla-vs-egm-comparison.md#L11`](../../docs/reports/vanilla-vs-egm-comparison.md#L11).
  - **Spoken (`bjm-voice`)**: We did not rig the prompt. Both workspaces received the identical functional specification straight out of [`EXPERIMENT_RUNBOOK.md`](../../EXPERIMENT_RUNBOOK.md).

- **`S010` (`01:08–01:15`, `7.0s`) `[v-click]`**: **Constraint 1 (Submodule Hygiene)** — Out-of-tree `xiao-rp2350-bargraph/` custom board + app (`nuttx/` and `apps/` stay spotless).
  - **Visual / Artefact Callout**: Requirement chip `1`: `xiao-rp2350-bargraph/` out-of-tree board & app (`git -C nuttx status` and `git -C apps status` empty).
  - **Spoken (`bjm-voice`)**: Constraint one: everything lives out-of-tree in `xiao-rp2350-bargraph/`, leaving both upstream `nuttx/` and `apps/` git submodules completely untouched.

- **`S011` (`01:15–01:23`, `8.0s`) `[v-click]`**: **Constraint 2 (Firmware Behaviour)** — `D0/A0` pot driving 6 LEDs, autonomous power-on boot, USB-C `nsh>` CLI (`set`, `status`, `stop`, `test`).
  - **Visual / Artefact Callout**: Requirement chip `2`: Autonomous power-on bar graph + interactive USB-C `nsh>` CLI (`start`, `stop`, `status`, `test`, `set`, `resume`).
  - **Spoken (`bjm-voice`)**: Constraint two: power on cleanly without a host computer, track the potentiometer smoothly, and expose an interactive USB serial command line.

- **`S012` (`01:23–01:31`, `8.0s`) `[v-click]`**: **Constraint 3 (Deliverables)** — Host `make test`, flashable `nuttx/nuttx.uf2`, plus Markdown & SVG breadboard wiring guide.
  - **Visual / Artefact Callout**: Requirement chip `3`: Host `make test`, flashable [`nuttx/nuttx.uf2`](../../nuttx/nuttx.uf2), plus Markdown and SVG wiring docs.
  - **Spoken (`bjm-voice`)**: Constraint three: ship host unit tests, a flashable `nuttx.uf2` image, plus a Markdown and SVG breadboard wiring diagram.

- **`S013` (`01:31–01:38`, `7.0s`) `[Slide]`**: **Deceptive Green Badge** — **`BUILD SUCCEEDED: nuttx/nuttx.uf2`**.
  - **Visual / Artefact Callout**: High-contrast terminal green status card: `BUILD SUCCEEDED: nuttx/nuttx.uf2` (`Experiment A` and `Experiment B`).
  - **Spoken (`bjm-voice`)**: Here is the trap: both the unprimed vibe-coding run and the full contract run reached this comforting green build badge.

- **`S014` (`01:38–01:45`, `7.0s`) `[v-click]`**: **It Blinks!** — Twist the potentiometer; all six breadboard LEDs light up smoothly in sequence.
  - **Visual / Artefact Callout**: Six-segment LED progression bar (`0% -> 100%`) illuminating sequentially beneath `BUILD SUCCEEDED`.
  - **Spoken (`bjm-voice`)**: You drag the `.uf2` onto the RP2350, twist the pot, and all six LEDs light up across the breadboard.

- **`S015` (`01:45–01:53`, `8.0s`) `[Slide]`**: **Standalone Quote** — *"In embedded systems—and any deep legacy codebase—'it compiles and blinks' is the most dangerous lie an AI accomplice can tell you."*
  - **Visual / Artefact Callout**: Full-screen centred quote card with subtle red accent border.
  - **Spoken (`bjm-voice`)**: In embedded systems—and any deep legacy codebase—"it compiles and blinks" is the most dangerous lie an AI accomplice can tell you.

- **`S016` (`01:53–02:00`, `7.0s`) `[v-click]`**: **The Hidden Timebombs** — Silicon errata pad leakage (`RP2350-E9`) + a `40 Hz` ISR teardown kernel panic.
  - **Visual / Artefact Callout**: Two crimson warning badges snapping beneath the quote: `RP2350-E9 IE=1 Leakage` and `40 Hz ISR Teardown Race`.
  - **Spoken (`bjm-voice`)**: Underneath that happy blinking demo hid a silicon erratum corruption bug and a forty-hertz interrupt teardown panic waiting to fire.

#### Beat 03 (`02:00–03:00`, `60.0s`, `7` steps) — What "Roll Your Own Docs" Means Across Three Layers

- **`S017` (`02:00–02:09`, `9.0s`) `[Slide]`**: **Foundational Credit (EGM)** — Dave Rensin (`@drensin`): [*"Elephants, Goldfish, and the New Golden Age of Software Engineering"*](https://drensin.medium.com/elephants-goldfish-and-the-new-golden-age-of-software-engineering-c33641a48874).
  - **Visual / Artefact Callout**: Attribution slide citing Dave Rensin (`@drensin`), `github.com/google/elephant-goldfish-model`, and [`/Users/brett/.gemini/config/plugins/egm/docs/index.md#L21-L26`](/Users/brett/.gemini/config/plugins/egm/docs/index.md#L21-L26).
  - **Spoken (`bjm-voice`)**: To stop shipping hidden timebombs, I turned to Dave Rensin's Elephant-Goldfish Model from Google's open-source `elephant-goldfish-model` plugin.

- **`S018` (`02:09–02:17`, `8.0s`) `[v-click]`**: **Rensin's Two Mantras** — *"Design is the new code."* / *"Design with an elephant. Review with a goldfish."*
  - **Visual / Artefact Callout**: Dual stacked quote callouts underneath Rensin's attribution.
  - **Spoken (`bjm-voice`)**: Dave captures the mindset in two lines: design is the new code; design with an elephant, review with a goldfish.

- **`S019` (`02:17–02:26`, `9.0s`) `[Slide]`**: **Layer 1 — Docs for the Elephant & Goldfish** — `3,241` bottom-up `README.md` contracts + EGM drift, brainstorm, draft, and review specs.
  - **Visual / Artefact Callout**: Layer `1` horizontal card (`Agent Layer: 3,241 README.md contracts + EGM design & review specs` in `docs/egm/`).
  - **Spoken (`bjm-voice`)**: That gives us layer one of "Roll Your Own Docs": build contracts and specs so your coding agents stop guessing.

- **`S020` (`02:26–02:35`, `9.0s`) `[v-click]`**: **Layer 2 — Docs for the Human (Who Owns the Outcome)** — 6-lesson interactive Tufte codelab in `docs/egm/tutorial/index.md` / `docs/egm/tutorial/index.html` via `/teach-code`.
  - **Visual / Artefact Callout**: Layer `2` horizontal card (`Human Layer: 6-lesson Tufte codelab` compiled from [`docs/egm/tutorial/index.md`](../../docs/egm/tutorial/index.md) into `docs/egm/tutorial/index.html`).
  - **Spoken (`bjm-voice`)**: Layer two rolls docs for the human who owns the outcome: a six-lesson interactive codelab so you actually understand the firmware.

- **`S021` (`02:35–02:43`, `8.0s`) `[v-click]`**: **Layer 3 — Docs for the Workflow** — Evolving Matt Pocock's `/teach` into `/teach-code` via `/agy-customizations`.
  - **Visual / Artefact Callout**: Layer `3` horizontal card (`Skill Layer: Matt Pocock's` [`.agents/skills/teach/SKILL.md`](../../.agents/skills/teach/SKILL.md) `->` [`.agents/skills/teach-code/SKILL.md`](../../.agents/skills/teach-code/SKILL.md) `via` [`/Users/brett/.gemini/antigravity/builtin/skills/agy-customizations/SKILL.md`](/Users/brett/.gemini/antigravity/builtin/skills/agy-customizations/SKILL.md)).
  - **Spoken (`bjm-voice`)**: And layer three rolls docs for your own workflow, evolving Matt Pocock's `/teach` skill into `/teach-code` through real-world use.

- **`S022` (`02:43–02:52`, `9.0s`) `[Slide]`**: **Three-Layer Architecture Summary** — Agent contracts -> Human comprehension -> Reusable skills.
  - **Visual / Artefact Callout**: Clean 3-step horizontal loop diagram (`1. Agent Contracts` -> `2. Human Codelab` -> `3. Living SKILL.md`).
  - **Spoken (`bjm-voice`)**: Together, those three layers create a tight loop: grounded code, genuine human comprehension, and sharper custom skills for the next repo.

- **`S023` (`02:52–03:00`, `8.0s`) `[v-click]`**: **Act II Pivot** — Next: watch what happens when you skip all three layers and vibe code straight into C.
  - **Visual / Artefact Callout**: Act II transition banner (`Experiment A: nuttxspace-vanilla`).
  - **Spoken (`bjm-voice`)**: Let's roll the tape on what happens when you skip all three layers and vibe code straight into C.

---

### Act II — The Foil: Flying Blind with Vanilla Vibe Coding (`03:00–08:00`, `5.0 min`, Beats `04–07`, `S024–S057`)

#### Beat 04 (`03:00–04:15`, `75.0s`, `5` steps) — Experiment A (`nuttxspace-vanilla`): 46 Minutes of Pure Vibe & "Weeds Syndrome"

- **`S024` (`03:00–03:08`, `8.0s`) `[Slide]`**: **Experiment A Baseline** — `nuttxspace-vanilla` on upstream `master` (`13` `README.md` files), `1` parent session (`60c052d5`).
  - **Visual / Artefact Callout**: Experiment A baseline card citing [`docs/reports/vanilla-vs-egm-comparison.md#L13, L33-L38`](../../docs/reports/vanilla-vs-egm-comparison.md#L13) (`13` `README.md` baseline, `1` chat thread `60c052d5`, `0` subagents).
  - **Spoken (`bjm-voice`)**: Experiment A starts on upstream `master` with thirteen `README.md` files, a single chat session, and zero custom workflow skills.

- **`S025` (`03:08–03:16`, `8.0s`) `[Slide]`**: **Rensin's *"Weeds Syndrome"*** — *Premature Implementation*: skipping architectural boundaries to edit nearby C immediately.
  - **Visual / Artefact Callout**: Concept card citing Dave Rensin's *Premature Implementation ("Weeds Syndrome")*: racing straight from prompt to code before mapping boundaries.
  - **Spoken (`bjm-voice`)**: Watch for what Dave Rensin calls "Weeds Syndrome"—premature implementation, diving straight into writing C files before understanding any subsystem invariants.

- **`S026` (`03:16–04:00.4`, `44.4s`) `[Live-cast]`**: **LIVE CAST (`44.4s`)** on [`prototypes/vanilla-vibe-cast-player.html`](../../prototypes/vanilla-vibe-cast-player.html).
  - **Visual / Artefact Callout**: Switch to [`prototypes/vanilla-vibe-cast-player.html`](../../prototypes/vanilla-vibe-cast-player.html). Jump (`]`) to Stop Point `2. Initial Prompt: XIAO RP2350 Bar Graph` (`00:02` warped), press `p` to run **Seg 03** (`14.1s @ 2x` -> `3. Q1: Breadboard Pins & LED Polarity`), **Seg 04** (`0.5s @ 2x` -> `4. A1: Any Free Breadboard Pins`), and **Seg 05** (`29.8s @ 52x`, compressing `25m 51s` raw -> auto-pauses at `5. Firmware & Tests Built`). Stop at Stop Point `5` (do not play SVG segment).
  - **Spoken (`bjm-voice`)**: Here is the raw prompt from [`EXPERIMENT_RUNBOOK.md`](../../EXPERIMENT_RUNBOOK.md) going straight into an unprimed thread. Look at the effect right away: it asks one quick question about pins and LED polarity, I answer "pick any free pins", and that is the entire design conversation over. Now at fifty-two times speed—compressing nearly twenty-six minutes of single-thread grep-and-code—it pulls sixty-six files out of the repo without reading a single `nuttx/` or `apps/` `README.md` contract. At step 180 `olddefconfig` quietly drops `CONFIG_ADC=y`, it patches Kconfig, and twenty-six minutes later it hands us a green `nuttx.uf2` build.

- **`S027` (`04:00.4–04:08`, `7.6s`) `[Slide]`**: **Vanilla Telemetry Snapshot** — `46m 40s` raw, `225` steps, `0` subagents, **`0` `nuttx`/`apps` `README.md` reads**.
  - **Visual / Artefact Callout**: Four-tile telemetry card from [`docs/reports/vanilla-vs-egm-comparison.md#L34-L38`](../../docs/reports/vanilla-vs-egm-comparison.md#L34-L38): `46m 40.0s` raw | `225` steps | `0` subagents | `0` `nuttx`/`apps` `README.md` reads (`66` `read_file` calls).
  - **Spoken (`bjm-voice`)**: Forty-six minutes forty seconds raw, two hundred and twenty-five steps, zero subagents, and zero architectural contract reads across `nuttx/` or `apps/`.

- **`S028` (`04:08–04:15`, `7.0s`) `[v-click]`**: **The Vibe-Coding Illusion** — `66` `read_file` greps, `1` question, `1` green `.uf2`—so what broke beneath the surface?
  - **Visual / Artefact Callout**: Reference pill linking [`docs/vanilla/CODE_REVIEW.md`](../../docs/vanilla/CODE_REVIEW.md) beneath the `0` contract reads stat.
  - **Spoken (`bjm-voice`)**: On paper that speed looks brilliant. Let's open [`docs/vanilla/CODE_REVIEW.md`](../../docs/vanilla/CODE_REVIEW.md) and inspect what sixty-six blind greps missed underneath.

#### Beat 05 (`04:15–05:15`, `60.0s`, `8` steps) — Supporting Traps #1 & #2: Silent Kconfig Drops & `my_reset()` + Erratum `RP2350-E9`

- **`S029` (`04:15–04:23`, `8.0s`) `[Slide]`**: **Supporting Trap #1 (Build Config)** — Trace Step 180 ([`docs/vanilla/CODE_REVIEW.md`](../../docs/vanilla/CODE_REVIEW.md)): Vanilla enables `CONFIG_ADC=y` in `defconfig`.
  - **Visual / Artefact Callout**: Kconfig diff card citing [`docs/reports/vanilla-vs-egm-comparison.md#L39, L117`](../../docs/reports/vanilla-vs-egm-comparison.md#L39) and `docs/vanilla/xiao-rp2350-bargraph/board/configs/bargraph/defconfig#L15-L16` (`CONFIG_ADC=y` at Step 180).
  - **Spoken (`bjm-voice`)**: First warm-up trap hits NuttX's Kconfig tree: Vanilla dropped `CONFIG_ADC=y` into `defconfig` assuming the ADC driver would compile.

- **`S030` (`04:23–04:30`, `7.0s`) `[v-click]`**: **Silent `olddefconfig` Drop** — Missing `CONFIG_ANALOG=y` silently strips `CONFIG_ADC=y` (while `CONFIG_DEV_GPIO=y` collides on `GPIO 5,6,7,0,1,2`).
  - **Visual / Artefact Callout**: Highlighted warning strip (`olddefconfig` drops `CONFIG_ADC` without `CONFIG_ANALOG=y`; `CONFIG_DEV_GPIO=y` collides on `GPIO 5, 6, 7, 0, 1, 2`).
  - **Spoken (`bjm-voice`)**: Without `CONFIG_ANALOG=y`, `olddefconfig` quietly stripped `CONFIG_ADC=y` out—and Vanilla still left `CONFIG_DEV_GPIO=y` colliding on all six LED pins.

- **`S031` (`04:30–04:38`, `8.0s`) `[Slide]`**: **Supporting Trap #2 (Silicon Init)** — `my_reset()` inside [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c).
  - **Visual / Artefact Callout**: File header callout: [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L466-L483`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L466-L483) (`my_reset()`).
  - **Spoken (`bjm-voice`)**: Trap two hides during kernel peripheral bringup in [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c)—a quiet initialisation helper called `my_reset()` with global pin side effects.

- **`S032` (`04:38–04:45`, `7.0s`) `[Code-step]`**: [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L466-L483`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L466-L483) (`my_reset()`) — Highlights loop iterating across **all four pins (`GPIO 26–29`)**, even when only Channel 0 is enabled.
  - **Visual / Artefact Callout**: Shiki code block (highlight lines `3–6` / source `#L472-L476`):
```c
static void my_reset(struct adc_dev_s *dev)
{
  for (int i = 0; i < ADC_CHANNEL_COUNT; ++i)
    {
      a_gpio = g_gpio_map[i]; /* { 26, 27, 28, 29, -1 } */
      if (a_gpio >= 0)
        {
          rp23xx_gpio_set_function(a_gpio, RP23XX_GPIO_FUNC_NULL);
```
  - **Spoken (`bjm-voice`)**: Even when only Channel 0 is configured, `my_reset()` unconditionally loops across all four pins, `GPIO 26` through `29`, wiping muxing.

- **`S033` (`04:45–04:53`, `8.0s`) `[Code-step]`**: [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L476-L482`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L476-L482) -> `rp23xx_gpio_set_function()` — Highlights unconditionally setting `RP23XX_PADS_BANK0_GPIO_IE` (`IE=1`).
  - **Visual / Artefact Callout**: Shiki highlight advances to `rp23xx_gpio_set_function` (`#L478-L480`):
```c
      if (a_gpio >= 0)
        {
          rp23xx_gpio_setdir(a_gpio, false);
          rp23xx_gpio_set_function(a_gpio, RP23XX_GPIO_FUNC_NULL);
          rp23xx_gpio_set_pulls(a_gpio, false, false);
        } /* set_function() sets RP23XX_PADS_BANK0_GPIO_IE (IE=1) */
```
  - **Spoken (`bjm-voice`)**: Worse, calling `rp23xx_gpio_set_function()` sets `RP23XX_PADS_BANK0_GPIO_IE`, quietly switching the digital input buffer back on right across `GPIO 26` through `29`.

- **`S034` (`04:53–05:00`, `7.0s`) `[Slide]`**: **Silicon Erratum `RP2350-E9`** — Leaving `IE=1` injects input-buffer leakage current into mid-rail analogue potentiometer readings on `GPIO 26`.
  - **Visual / Artefact Callout**: Silicon erratum card (`RP2350-E9`): `IE=1` digital Schmitt buffer leakage distorts `GPIO 26` (`D0/A0`) mid-scale potentiometer voltages (`~1.65 V`).
  - **Spoken (`bjm-voice`)**: On RP2350 silicon, leaving `IE=1` on an analogue input triggers Erratum `RP2350-E9`, leaking digital buffer current into mid-scale potentiometer readings.

- **`S035` (`05:00–05:08`, `8.0s`) `[Code-step]`**: [`docs/vanilla/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L42-L54`](../../docs/vanilla/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L42-L54) vs. [`docs/egm/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L67-L72`](../../docs/egm/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L67-L72) — Highlights Vanilla omitting pad isolation after `rp23xx_common_bringup()`.
  - **Visual / Artefact Callout**: Shiki diff contrasting Vanilla (`#L43-L53`, skips pad register) against EGM (`#L67-L72`, highlight lines `3–8`):
```c
  ret = rp23xx_common_bringup(); /* Vanilla omits pad isolation */
  /* EGM rp23xx_bringup.c#L67-L72: mitigate RP2350-E9 on GPIO26 */
  modreg32(RP23XX_PADS_BANK0_GPIO_OD,
           RP23XX_PADS_BANK0_GPIO_IE |
           RP23XX_PADS_BANK0_GPIO_PUE |
           RP23XX_PADS_BANK0_GPIO_PDE |
           RP23XX_PADS_BANK0_GPIO_ISO,
           RP23XX_PADS_BANK0_GPIO(BARGRAPH_ADC_GPIO));
```
  - **Spoken (`bjm-voice`)**: Vanilla never clears `IE`, pulls, or isolation after `rp23xx_common_bringup()`, shipping `RP2350-E9` analogue pad leakage straight onto the user's bench.

- **`S036` (`05:08–05:15`, `7.0s`) `[v-click]`**: **Trap Summary** — Grep gives you symbol names, never cross-layer silicon errata invariants.
  - **Visual / Artefact Callout**: Callout bar beneath code diff: `Grep -> symbol names | Contracts -> cross-layer silicon invariants`.
  - **Spoken (`bjm-voice`)**: Grep finds you C symbol names; it never warns you about cross-layer silicon errata. Now meet our primary spine trap.

#### Beat 06 (`05:15–07:00`, `105.0s`, `13` steps) — Primary Spine (Part 1): The 500 kHz `START_ONCE` Storm & Vanilla's 40 Hz Teardown Race

- **`S037` (`05:15–05:23`, `8.0s`) `[Slide]`**: **Primary Spine Setup** — Meet the `500 kHz` `START_ONCE` ADC storm inside [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c).
  - **Visual / Artefact Callout**: Through-line badge: `Primary Spine: 500 kHz START_ONCE ADC Storm` ([`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c)).
  - **Spoken (`bjm-voice`)**: This next quirk inside `rp23xx_adc.c` threads through both experiments and exposes why both vibe coding and unreviewed specs break.

- **`S038` (`05:23–05:31`, `8.0s`) `[Code-step]`**: [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L232-L237`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L232-L237) (`interrupt_handler()`) — Highlights unconditional `get_next_channel();`.
  - **Visual / Artefact Callout**: Shiki code block (highlight line `3` / source `#L234`):
```c
  /* Start next channel read */

  get_next_channel();

  return OK;
}
```
  - **Spoken (`bjm-voice`)**: Inside the ADC interrupt handler, every single time a sample finishes, `rp23xx_adc.c` unconditionally calls `get_next_channel()` before returning.

- **`S039` (`05:31–05:39`, `8.0s`) `[Code-step]`**: [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L302-L308`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L302-L308) (`get_next_channel()`) — Highlights back-to-back `RP23XX_ADC_CS_START_ONCE` re-arming inside the ISR.
  - **Visual / Artefact Callout**: Shiki code block (highlight lines `3–5` / source `#L304-L306`):
```c
              /* Start the conversion */

              value += RP23XX_ADC_CS_START_ONCE;

              putreg32(value, RP23XX_ADC_CS);

              return;
```
  - **Spoken (`bjm-voice`)**: And `get_next_channel()` immediately pokes `RP23XX_ADC_CS_START_ONCE` right back into the control register, chaining another conversion as fast as silicon allows.

- **`S040` (`05:39–05:47`, `8.0s`) `[Slide]`**: **The Storm Metric** — **`500 kHz` (`2 µs`/sample) interrupt flood** whenever `/dev/adc0` stays open!
  - **Visual / Artefact Callout**: Oversized metric card: `500,000 IRQ/s` (`96` ADC clock cycles @ `48 MHz` = `2 µs`/sample while `/dev/adc0` is open).
  - **Spoken (`bjm-voice`)**: At forty-eight megahertz and ninety-six cycles per sample, leaving `/dev/adc0` open unleashes five hundred thousand interrupts per second onto the CPU.

- **`S041` (`05:47–05:55`, `8.0s`) `[Slide]`**: **Vanilla's Workaround** — `bargraph_read_adc_once()` in [`docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L80-L103`](../../docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L80-L103).
  - **Visual / Artefact Callout**: Function signature card for `bargraph_read_adc_once()` in [`docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L80-L103`](../../docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L80-L103).
  - **Spoken (`bjm-voice`)**: Vanilla noticed the ADC kept hammering interrupts continuously, so it improvised a neat-looking user-space workaround in `bargraph_read_adc_once()`.

- **`S042` (`05:55–06:03`, `8.0s`) `[Code-step]`**: [`docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L87-L95`](../../docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L87-L95) — Highlights `open("/dev/adc0")` -> blocking `read()` -> `close(fd)` at **`40 Hz` (`25 ms` ticks)**.
  - **Visual / Artefact Callout**: Shiki code block (highlight lines `1, 6, 7` / source `#L87, L93-L94`):
```c
  fd = open(CONFIG_BARGRAPH_ADC_DEVPATH, O_RDONLY);
  if (fd < 0)
    {
      return -errno;
    }
  nbytes = read(fd, &msg, sizeof(msg));
  close(fd);
  if (nbytes != (ssize_t)sizeof(msg))
```
  - **Spoken (`bjm-voice`)**: Forty times every second—every twenty-five milliseconds—Vanilla opens `/dev/adc0`, reads one blocking message, and immediately calls `close(fd)`.

- **`S043` (`06:03–06:11`, `8.0s`) `[v-click]`**: **Why That Looked Clever** — Closing `/dev/adc0` stops conversions between ticks, dodging the `500 kHz` storm.
  - **Visual / Artefact Callout**: Green badge overlay beside code: `CPU load drops + LEDs track smoothly on desk`.
  - **Spoken (`bjm-voice`)**: On the desk, CPU load drops away and the bar graph tracks cleanly. Looks like a sharp engineering workaround, doesn't it?

- **`S044` (`06:11–06:19`, `8.0s`) `[Slide]`**: **Zoom Into Upper-Half Teardown** — [`nuttx/drivers/analog/adc.c#L181-L214`](../../nuttx/drivers/analog/adc.c#L181-L214) (`adc_close()`).
  - **Visual / Artefact Callout**: Call-stack arrow card: `close(fd)` -> `adc_close()` ([`nuttx/drivers/analog/adc.c#L181-L214`](../../nuttx/drivers/analog/adc.c#L181-L214)).
  - **Spoken (`bjm-voice`)**: Until you trace `close(fd)` down into NuttX's upper-half driver in [`nuttx/drivers/analog/adc.c`](../../nuttx/drivers/analog/adc.c) and see what `adc_close()` actually locks.

- **`S045` (`06:19–06:27`, `8.0s`) `[Code-step]`**: [`nuttx/drivers/analog/adc.c#L187-L210`](../../nuttx/drivers/analog/adc.c#L187-L210) (`adc_close()`) — Highlights `nxmutex_lock(&dev->ad_lock)` with **zero `enter_critical_section()`** around `ao_shutdown()`.
  - **Visual / Artefact Callout**: Shiki condensed excerpt (`#L187-L210`, highlight lines `1, 6, 7`):
```c
  ret = nxmutex_lock(&dev->ad_lock);
  if (ret >= 0)
    {
      ...
      dev->ad_ocount = 0;
      dev->ad_ops->ao_shutdown(dev); /* No enter_critical_section()! */
      nxmutex_unlock(&dev->ad_lock);
    }
```
  - **Spoken (`bjm-voice`)**: `adc_close()` holds `nxmutex_lock()`, not `enter_critical_section()`. A mutex blocks competing threads—it does nothing to stop a two-microsecond hardware interrupt.

- **`S046` (`06:27–06:35`, `8.0s`) `[Code-step]`**: [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L395-L407`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L395-L407) (`remove_device()`) — Highlights `g_first_device = PRIV(g_first_device)->next_device;` (`#L399`) seven lines before `g_active_count -= 1;` (`#L406`).
  - **Visual / Artefact Callout**: Shiki condensed excerpt (`#L395-L407`, highlight lines `3, 7`):
```c
  if (dev == g_first_device)
    {
      g_first_device = PRIV(g_first_device)->next_device;
      if (g_first_device != NULL)
        PRIV(g_first_device)->prior_device = NULL;

      g_active_count -= 1; /* 7 lines after g_first_device cleared! */
    }
```
  - **Spoken (`bjm-voice`)**: Inside `remove_device()`, `g_first_device` becomes `NULL` seven lines before `g_active_count` drops. If the ADC interrupt lands inside that tiny window...

- **`S047` (`06:35–06:43`, `8.0s`) `[Slide]`**: **The Teardown Race Diagram** — `RP23XX_ADC_IRQ_FIFO` fires between `#L399` and `#L406` -> `get_next_channel()` hits **`ASSERT(next != g_current_channel)` (`#L314`)**!
  - **Visual / Artefact Callout**: Two-lane race timeline contrasting `#L399 g_first_device = NULL` -> `ISR fires` -> `#L314 ASSERT(next != g_current_channel)` panic vs. `#L406 g_active_count -= 1`.
  - **Spoken (`bjm-voice`)**: ...the interrupt handler runs `get_next_channel()`, finds zero active devices, and crashes straight into `ASSERT(next != g_current_channel)` at line 314.

- **`S048` (`06:43–06:51`, `8.0s`) `[v-click]`**: **Risk Multiplier** — Calling `close(fd)` at **`40 Hz`** rolls those race dice forty times every second!
  - **Visual / Artefact Callout**: Red multiplier pill: `40 Hz close(fd) = 40 race rolls / second`.
  - **Spoken (`bjm-voice`)**: By closing `/dev/adc0` at forty hertz, Vanilla quietly turned a rare shutdown race into forty kernel-panic dice rolls every single second.

- **`S049` (`06:51–07:00`, `9.0s`) `[v-click]`**: **Read Safety Nits** — Blocking `read()` with zero `O_NONBLOCK` / `poll()` timeout and silently swallowed errors in `bargraph_daemon()`.
  - **Visual / Artefact Callout**: Companion safety chip citing [`docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L93, L191-L201`](../../docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L93).
  - **Spoken (`bjm-voice`)**: On top of that, blocking `read()` has zero timeout and `bargraph_daemon()` silently swallows read failures instead of reporting or blinking faults.

#### Beat 07 (`07:00–08:00`, `60.0s`, `8` steps) — Vanilla Audit Verdict: `5 / 10` Primary (`4 Pass / 4 Partial / 2 Fail` Strict)

- **`S050` (`07:00–07:08`, `8.0s`) `[Slide]`**: **Give Credit Where Due** — What Vanilla actually got right ([`docs/vanilla/CODE_REVIEW.md`](../../docs/vanilla/CODE_REVIEW.md) + [`docs/reports/vanilla-vs-egm-comparison.md`](../../docs/reports/vanilla-vs-egm-comparison.md)).
  - **Visual / Artefact Callout**: Balanced scorecard header card: `Credit Where Due — nuttxspace-vanilla Wins`.
  - **Spoken (`bjm-voice`)**: Now, to be fair—no cheap straw-manning here—let's credit what Vanilla vibe coding genuinely got right on the first try.

- **`S051` (`07:08–07:15`, `7.0s`) `[v-click]`**: **Vanilla's Genuine Wins** — LEDs safely on `D3–D8` (dodging `GPIO 26–29` clobber) and clean shared [`src/bargraph_logic.c`](../../docs/vanilla/xiao-rp2350-bargraph/src/bargraph_logic.c) host unit tests.
  - **Visual / Artefact Callout**: Two green check pills: `D3–D8 LED pins dodge GPIO 26–29 clobber` + `Clean shared src/bargraph_logic.c host tests`.
  - **Spoken (`bjm-voice`)**: It parked the LEDs on `D3` through `D8`, dodging `my_reset()`, and wrote a cleaner shared [`bargraph_logic.c`](../../docs/vanilla/xiao-rp2350-bargraph/src/bargraph_logic.c) test split than EGM's first pass.

- **`S052` (`07:15–07:23`, `8.0s`) `[Slide]`**: **Other Concurrency & Lifecycle Gaps** — Dual autostart (`rp23xx_bringup` + `rcS`), unguarded `g_bargraph_state` races, LEDs left frozen on at `stop`.
  - **Visual / Artefact Callout**: Three compact amber/red defect pills citing [`docs/reports/vanilla-vs-egm-comparison.md#L70-L71`](../../docs/reports/vanilla-vs-egm-comparison.md#L70-L71).
  - **Spoken (`bjm-voice`)**: Still, Vanilla also shipped dual autostart (`rp23xx_bringup` plus `rcS`), unlocked global state races, and left LEDs frozen bright on `stop`.

- **`S053` (`07:23–07:30`, `7.0s`) `[v-click]`**: **Two High-Severity Fails** — `RP2350-E9` `IE=1` analogue leakage + `40 Hz` `close(fd)` `remove_device()` ISR race.
  - **Visual / Artefact Callout**: Two crimson `FAIL` badges: `#3 RP2350-E9 IE=1 Leakage` + `#6 40 Hz ISR Teardown Race`.
  - **Spoken (`bjm-voice`)**: Combined with `RP2350-E9` pad leakage and the forty-hertz `close(fd)` race, two high-severity defects sit silently inside a blinking demo.

- **`S054` (`07:30–07:38`, `8.0s`) `[Slide]`**: **Primary Scorecard Verdict** — **Vanilla (`nuttxspace-vanilla`): `5 / 10` Primary Pass**.
  - **Visual / Artefact Callout**: Single large scorecard badge from [`docs/reports/vanilla-vs-egm-comparison.md#L22`](../../docs/reports/vanilla-vs-egm-comparison.md#L22): `Vanilla Primary 10-Point Scorecard: 5 / 10`.
  - **Spoken (`bjm-voice`)**: On our ten-point hardware and RTOS trap scorecard, Vanilla scores five out of ten: impressive surface velocity, unsafe underneath.

- **`S055` (`07:38–07:45`, `7.0s`) `[v-click]`**: **Strict Goldfish Verdict** — **`4 Pass / 4 Partial / 2 Fail`** (downgrading `Make.defs` missing `distclean` filter).
  - **Visual / Artefact Callout**: Second badge row revealing Strict Post-Audit verdict from [`docs/reports/vanilla-vs-egm-comparison.md#L23`](../../docs/reports/vanilla-vs-egm-comparison.md#L23): `Strict Audit: 4 Pass / 4 Partial / 2 Fail`.
  - **Spoken (`bjm-voice`)**: Under a strict hostile audit, that drops to four passes, four partials, and two hard fails across ten invariants.

- **`S056` (`07:45–07:53`, `8.0s`) `[Slide]`**: **Rensin's Diagnosis** — **Context Pollution + Self-Validation**: one thread interviews, guesses, writes C, and grades its own homework.
  - **Visual / Artefact Callout**: Single-chat failure diagram: `Prompt -> Guess -> Code -> Self-Validate` (`Hallucination Cascade & Context Pollution`).
  - **Spoken (`bjm-voice`)**: This is exactly the trap Dave Rensin diagnosed: context pollution and self-validation, where one chat session guesses, writes C, and approves itself.

- **`S057` (`07:53–08:00`, `7.0s`) `[v-click]`**: **Act III Pivot** — What happens when we switch to EGM and roll architectural contracts for the Elephant first?
  - **Visual / Artefact Callout**: Act III transition banner (`Experiment B: nuttxspace-egm`).
  - **Spoken (`bjm-voice`)**: Same hardware, same prompt—now let's roll contracts for the Elephant before writing a single line of C.

---

### Act III — Rolling Docs for the Agent: EGM Codebase Ramp-Up (`08:00–19:00`, `11.0 min`, Beats `08–16`, `S058–S132`)

#### Act III.a — Grounding the Map (`08:00–10:00`, `2.0 min`, Beats `08–09`, `S058–S073`)

##### Beat 08 (`08:00–09:00`, `60.0s`, `8` steps) — Feeding the Elephant (Step 1: Context Loading): Why `3,241` Bottom-Up `README.md` Contracts Beat Grepping Raw C

- **`S058` (`08:00–08:08`, `8.0s`) `[Slide]`**: **Act III Divider** — **Rolling Docs for the Agent: The Elephant-Goldfish Model (`nuttxspace-egm`)**.
  - **Visual / Artefact Callout**: Act III full-screen section header (`Act III — Rolling Docs for the Agent: nuttxspace-egm`).
  - **Spoken (`bjm-voice`)**: Welcome to Experiment B, `nuttxspace-egm`—running the identical prompt inside the Elephant-Goldfish Model across our contract-mapped submodules.

- **`S059` (`08:08–08:15`, `7.0s`) `[v-click]`**: **Rensin Step 1: Feeding the Elephant** — Build cheap, navigable architectural memory across `2,607` `nuttx/` and `634` `apps/` directories via `/egm-bootstrap`.
  - **Visual / Artefact Callout**: Stat strip citing `egm` branch baseline: `2,607 nuttx/` + `634 apps/` = `3,241 README.md` contracts via `/egm-bootstrap`.
  - **Spoken (`bjm-voice`)**: Step one feeds the Elephant cheaply: `3,241` bottom-up `README.md` files summarising every directory across `nuttx/` and `apps/` before anyone starts coding.

- **`S060` (`08:15–08:23`, `8.0s`) `[Slide]`**: **5-Section Contract Anatomy** — Excerpt header of [`nuttx/arch/arm/src/rp23xx/README.md#L1-L71`](../../nuttx/arch/arm/src/rp23xx/README.md#L1-L71).
  - **Visual / Artefact Callout**: Header card of [`nuttx/arch/arm/src/rp23xx/README.md#L1-L5`](../../nuttx/arch/arm/src/rp23xx/README.md#L1-L5) (`# rp23xx` — `30` headers, `40` source files) with 5 empty numbered slots waiting to click open.
  - **Spoken (`bjm-voice`)**: Open [`nuttx/arch/arm/src/rp23xx/README.md`](../../nuttx/arch/arm/src/rp23xx/README.md) and you see the exact five-section contract format shared across all three thousand directories.

- **`S061` (`08:23–08:30`, `7.0s`) `[v-click]`**: **Sections 1–2 (`Overview` + `Subdirectories & Boundaries`)** — Defines ownership boundaries before touching files.
  - **Visual / Artefact Callout**: Reveals [`nuttx/arch/arm/src/rp23xx/README.md#L3-L8`](../../nuttx/arch/arm/src/rp23xx/README.md#L3-L8) (`## Overview`, `## Subdirectories & Boundaries` -> `./hardware/README.md`).
  - **Spoken (`bjm-voice`)**: Sections one and two state what lives here and link child boundaries—so the agent navigates hierarchy without slurping forty files.

- **`S062` (`08:30–08:38`, `8.0s`) `[v-click]`**: **Section 3 (`Interface`)** — Public functions, devices, and Kconfig dependencies exposed upward.
  - **Visual / Artefact Callout**: Reveals [`nuttx/arch/arm/src/rp23xx/README.md#L9-L40`](../../nuttx/arch/arm/src/rp23xx/README.md#L9-L40) (`## Interface` citing `[rp23xx_adc.h:rp23xx_adc_setup]`, `[rp23xx_gpio.h:rp23xx_gpio_set_function]`).
  - **Spoken (`bjm-voice`)**: Section three lists the public interface via `[file:symbol]` citations—giving the agent exact entry points instead of blind string matches.

- **`S063` (`08:38–08:45`, `7.0s`) `[v-click]`**: **Section 4 (`Invariants`)** — Cross-layer rules grep misses (`my_reset()`, `START_ONCE`, `RP2350-E9`).
  - **Visual / Artefact Callout**: Reveals [`nuttx/arch/arm/src/rp23xx/README.md#L42-L55`](../../nuttx/arch/arm/src/rp23xx/README.md#L42-L55) (`## Invariants` citing `[rp23xx_adc.c:ASSERT]`, `enter_critical_section`, compile-time guards).
  - **Spoken (`bjm-voice`)**: Section four documents invariants—assert preconditions, critical-section rules, and silicon constraints that grep never surfaces by itself.

- **`S064` (`08:45–08:53`, `8.0s`) `[v-click]`**: **Section 5 (`Verification`)** — Concrete tests and assertions proving contract truth (paired with `Side Effects` at `#L56-L71`).
  - **Visual / Artefact Callout**: Reveals [`nuttx/arch/arm/src/rp23xx/README.md#L56-L71`](../../nuttx/arch/arm/src/rp23xx/README.md#L56-L71) (`## Side Effects` -> `## Verification`).
  - **Spoken (`bjm-voice`)**: And sections on side effects and verification point directly at MMIO registers touched and tests proving those invariants still hold.

- **`S065` (`08:53–09:00`, `7.0s`) `[Slide]`**: **Bottom-Up Synthesis** — Leaf modules summarise upward into subsystem maps; the Elephant navigates contracts instead of raw C.
  - **Visual / Artefact Callout**: Tree rollup graphic: leaf `README.md` -> `rp23xx/README.md` -> `nuttx/README.md` -> root `README.md`.
  - **Spoken (`bjm-voice`)**: Leaf modules roll upward into subsystem blueprints, compressing thirty years of NuttX architecture into lightweight markdown your Elephant reads in seconds.

##### Beat 09 (`09:00–10:00`, `60.0s`, `8` steps) — Phase 1 (`01-bootstrap`): Anchoring the Workspace & Catching Upstream Drift

- **`S066` (`09:00–09:08`, `8.0s`) `[Slide]`**: **Freeze-Frame (`01-bootstrap`)** — [`prototypes/01-bootstrap-cast-player.html`](../../prototypes/01-bootstrap-cast-player.html) at Stop Point `8. Q1: Run EGM Drift Report?` (`00:38` warped, `7x` speed badge).
  - **Visual / Artefact Callout**: Full-frame screenshot/embed of [`prototypes/01-bootstrap-cast-player.html`](../../prototypes/01-bootstrap-cast-player.html) frozen on pill `8. Q1: Run EGM Drift Report?` (`00:38` warped, `7x` speed badge).
  - **Spoken (`bjm-voice`)**: Invoking `/egm-bootstrap` anchors the workspace root in eight raw minutes, reading `nuttx/` and `apps/` contracts before pausing here on drift detection.

- **`S067` (`09:08–09:15`, `7.0s`) `[v-click]`**: **Phase 1 Telemetry** — `72` steps, `40` tool calls, **`6` `README.md` reads**, `0` lines of C written.
  - **Visual / Artefact Callout**: Four-pill telemetry bar from [`docs/reports/vanilla-vs-egm-comparison.md#L47`](../../docs/reports/vanilla-vs-egm-comparison.md#L47): `72` steps | `40` tool calls (`17` reads / `6` `README.md` contracts) | `0` C files touched.
  - **Spoken (`bjm-voice`)**: Seventy-two steps, six contract reads, zero lines of C—and the Elephant already holds the exact cross-submodule build boundary in memory.

- **`S068` (`09:15–09:23`, `8.0s`) `[Slide]`**: **Bootstrapped Root Contract** — [`docs/egm/README.md#L7-L18`](../../docs/egm/README.md#L7-L18) (`28` lines anchoring the repository layout).
  - **Visual / Artefact Callout**: Excerpt card of [`docs/egm/README.md#L7-L18`](../../docs/egm/README.md#L7-L18) highlighting the `3`-directory boundary (`apps`, `nuttx`, `xiao-rp2350-bargraph`) and `Global Invariants`.
  - **Spoken (`bjm-voice`)**: Out pops a crisp twenty-eight-line root blueprint locking sibling layout, flat kernel/user-space separation, and the monotonic kernel boot chain.

- **`S069` (`09:23–09:30`, `7.0s`) `[Slide]`**: **Trust-But-Verify Hook** — `/egm-drift-report`: compare checked-in markdown contracts directly against live C source.
  - **Visual / Artefact Callout**: Command banner: `/egm-drift-report` (`Markdown Contract <-> Live C Source Audit`).
  - **Spoken (`bjm-voice`)**: Because markdown contracts can go stale over time, Phase 1 immediately follows bootstrap with `/egm-drift-report` against the live C tree.

- **`S070` (`09:30–09:38`, `8.0s`) `[Code-step]`**: [`docs/egm/reports/2026-11-16_DRIFT_REPORT.md#L9-L11`](../../docs/egm/reports/2026-11-16_DRIFT_REPORT.md#L9-L11) (`11` lines) — Highlights `OSINIT_HARDWARE` drift flagged in `nuttx/README.md#L79`.
  - **Visual / Artefact Callout**: Shiki markdown excerpt (`#L9-L11`, highlight line `3` / `#L11`):
```markdown
## nuttx/README.md

- Incomplete State Progression in `Invariants`: Line 79 states that `g_nx_initstate` progresses monotonically through `OSINIT_BOOT`, `OSINIT_TASKLISTS`, `OSINIT_MEMORY`, `OSINIT_OSREADY`, and `OSINIT_IDLELOOP` during `nx_start()`, omitting the intermediate `OSINIT_HARDWARE` state assigned at `nuttx/sched/init/nx_start.c:660` (and checked by `OSINIT_HW_READY()` in `nuttx/include/nuttx/init.h:104`).
```
  - **Spoken (`bjm-voice`)**: Eleven lines of drift report immediately spot a missing enum state in `nuttx/README.md` line 79: `OSINIT_HARDWARE` was omitted.

- **`S071` (`09:38–09:45`, `7.0s`) `[Code-step]`**: [`nuttx/sched/init/nx_start.c#L658-L664`](../../nuttx/sched/init/nx_start.c#L658-L664) — Highlights `g_nx_initstate = OSINIT_HARDWARE;` at `#L660`.
  - **Visual / Artefact Callout**: Shiki C excerpt (`#L658-L664`, highlight line `3` / `#L660`):
```c
  /* Hardware resources are now available */

  g_nx_initstate = OSINIT_HARDWARE;
  sched_trace_mark("HARDWARE");

  /* Setup for Multi-Tasking ************************************************/
```
  - **Spoken (`bjm-voice`)**: It cites `nx_start.c` line 660 verbatim, proving where hardware init finishes and reconciling contract truth before any design begins.

- **`S072` (`09:45–09:53`, `8.0s`) `[Slide]`**: **Phase 1 Payoff** — Grounded map + live drift check in 8 minutes.
  - **Visual / Artefact Callout**: Phase 1 summary badge (`01-bootstrap`: `8m 14s` raw -> grounded root `README.md` + drift resolved).
  - **Spoken (`bjm-voice`)**: Eight minutes of setup buys a verified workspace map and catches stale documentation before we make a single architecture call.

- **`S073` (`09:53–10:00`, `7.0s`) `[v-click]`**: **Phase 2 Transition** — Next: `/egm-brainstorm`—interrogating the design with zero code allowed.
  - **Visual / Artefact Callout**: Act III.b transition pill (`Next -> /egm-brainstorm`).
  - **Spoken (`bjm-voice`)**: Now let's invoke `/egm-brainstorm` and watch the Elephant grow without touching a single line of C.

#### Act III.b — From Vibe to Architecture (`10:00–13:00`, `3.0 min`, Beats `10–11`, `S074–S095`)

##### Beat 10 (`10:00–11:30`, `90.0s`, `11` steps) — Growing the Elephant (Steps 2–3: "No Code Yet" & The Sycophant Challenge via `02-brainstorm`)

- **`S074` (`10:00–10:08`, `8.0s`) `[Slide]`**: **Rensin Step 2: The *"No Code Yet"* Rule** — Strictly prohibit generating C while exploring constraints and trade-offs.
  - **Visual / Artefact Callout**: Rule card citing Rensin's *"No Code Yet"* rule (`/Users/brett/.gemini/config/plugins/egm/`): `0 lines of C permitted during brainstorm`.
  - **Spoken (`bjm-voice`)**: Phase 2 enforces Dave Rensin's "No Code Yet" rule—banning implementation so the Elephant has to ask hard questions before solving anything.

- **`S075` (`10:08–10:16`, `8.0s`) `[Slide]`**: **Freeze-Frame (`02-brainstorm`)** — [`prototypes/02-brainstorm-cast-player.html`](../../prototypes/02-brainstorm-cast-player.html) at Stop Point `16. Q7: Readiness Checkpoint (Review with Goldfishes)` (`02:53` warped).
  - **Visual / Artefact Callout**: Freeze-frame embed of [`prototypes/02-brainstorm-cast-player.html`](../../prototypes/02-brainstorm-cast-player.html) on pill `16. Q7: Readiness Checkpoint (Review with Goldfishes)` (`02:53` warped), highlighting the `19`-pill chapter bar.
  - **Spoken (`bjm-voice`)**: Running `/egm-brainstorm` drives forty-one minutes of structured Q&A across nineteen checkpoints—notice that packed row of chapter pills along the top.

- **`S076` (`10:16–10:24`, `8.0s`) `[v-click]`**: **Interview Depth Contrast** — **19 chapter stop points** of Socratic exploration (`41m 32s` raw) vs. Vanilla's single question.
  - **Visual / Artefact Callout**: Contrast bar: `Vanilla: 1 pin question` vs. `EGM 02-brainstorm: 19 Socratic checkpoints (41m 32s raw)`.
  - **Spoken (`bjm-voice`)**: Compare Vanilla's single "pick any free pins" chat against nineteen checkpoints grilling pin budgets, autostart timing, and PWM slice sharing.

- **`S077` (`10:24–10:32`, `8.0s`) `[v-click]`**: **Contract Reads Counter** — **`49` `README.md` reads** (`216` `read_file` calls) before a design choice locks.
  - **Visual / Artefact Callout**: Stat counter from [`docs/reports/vanilla-vs-egm-comparison.md#L48`](../../docs/reports/vanilla-vs-egm-comparison.md#L48): `49 README.md reads` (`216 read_file calls`).
  - **Spoken (`bjm-voice`)**: Before a single requirement settles, the Elephant reads forty-nine directory contracts and two hundred and sixteen files across `nuttx/` and `apps/`.

- **`S078` (`10:32–10:40`, `8.0s`) `[Slide]`**: **Rensin Step 3: *The Sycophant Challenge*** — 7-lens Goldfish shootdown swarm (Contrarian, Economics, Ergonomics, Migration, SRE, Tech Debt, Inventive Catalyst).
  - **Visual / Artefact Callout**: Seven-lens badge grid (*Contrarian, Economics, Ergonomics, Migration, SRE, Tech Debt, Inventive Catalyst*) implementing Rensin's *Sycophant Challenge*.
  - **Spoken (`bjm-voice`)**: Then comes Rensin's Sycophant Challenge: seven hostile lenses attacking our own hypothesis so the agent stops cheering me on politely.

- **`S079` (`10:40–10:48`, `8.0s`) `[Slide]`**: **Concepts Brief Output** — [`docs/egm/designs/concepts_brief.md#L16-L35`](../../docs/egm/designs/concepts_brief.md#L16-L35) listing **`7` hardware/driver obstacles** upfront.
  - **Visual / Artefact Callout**: Section header card for [`docs/egm/designs/concepts_brief.md#L14-L35`](../../docs/egm/designs/concepts_brief.md#L14-L35): `7 Hardware & Driver Traps Identified in nuttx/arch/arm/src/rp23xx`.
  - **Spoken (`bjm-voice`)**: Every bit of that reading lands in [`concepts_brief.md`](../../docs/egm/designs/concepts_brief.md), listing seven real upstream hardware and driver traps before drafting even starts.

- **`S080` (`10:48–10:56`, `8.0s`) `[v-click]`**: **Obstacles #1 & #2** — `my_reset()` pin clobber (`GPIO 26–29`) + `RP2350-E9` `IE=1` pad isolation after `rp23xx_common_bringup()`.
  - **Visual / Artefact Callout**: Reveals [`docs/egm/designs/concepts_brief.md#L29-L31`](../../docs/egm/designs/concepts_brief.md#L29-L31) (Trap #6: `CHANNEL0` only + clear `IE | PUE | PDE | ISO` + `OD=1` on `GPIO 26`).
  - **Spoken (`bjm-voice`)**: Right there on lines 29 to 31, it catches Vanilla's exact `my_reset()` pin clobber and `RP2350-E9` pad leakage beforehand.

- **`S081` (`10:56–11:04`, `8.0s`) `[v-click]`**: **Obstacle #3** — 5-slice PWM layout (`D1`/`D2` safely re-muxed by opening `/dev/pwm5,6` in `bargraphd` *after* `my_reset()`).
  - **Visual / Artefact Callout**: Reveals [`docs/egm/designs/concepts_brief.md#L21-L27`](../../docs/egm/designs/concepts_brief.md#L21-L27) (`channels[0].channel=1`, `channels[1].channel=2`, `uint16_t` `0..65535` avoiding `b16ONE`, zero-before-stop).
  - **Spoken (`bjm-voice`)**: It also uncovers NuttX's hardcoded `pwm_info_s` indexing, `b16ONE` wraparound turning 100% LEDs dark, and `PWMIOC_STOP` freezing output pins high.

- **`S082` (`11:04–11:12`, `8.0s`) `[v-click]`**: **Obstacle #4** — `500 kHz` `START_ONCE` ADC interrupt storm flagged explicitly as a design hazard.
  - **Visual / Artefact Callout**: Reveals [`docs/egm/designs/concepts_brief.md#L32-L34`](../../docs/egm/designs/concepts_brief.md#L32-L34) (`500,000 IRQ/s` storm + proposed `RP23XX_ADC_DIV = 0xFFFF00` mitigation).
  - **Spoken (`bjm-voice`)**: And at trap seven, it flags our primary spine—the half-megahertz ADC interrupt storm—which sets up our big honest blunder next.

- **`S083` (`11:12–11:21`, `9.0s`) `[v-click]`**: **Obstacles #5–#7** — `BOARD_LATE_INITIALIZE` single boot, clean submodule boundary, and host test separation.
  - **Visual / Artefact Callout**: Reveals [`docs/egm/designs/concepts_brief.md#L18-L20, L38-L44`](../../docs/egm/designs/concepts_brief.md#L18-L20) (`CONFIG_BOARD_LATE_INITIALIZE=y`, `nsh_waitusbready()`, `apps/external` symlink).
  - **Spoken (`bjm-voice`)**: Plus `CONFIG_BOARD_LATE_INITIALIZE=y`, why USB serial blocks init scripts until a laptop connects, and clean `apps/external` submodule isolation.

- **`S084` (`11:21–11:30`, `9.0s`) `[Slide]`**: **Phase 2 Payoff** — Zero lines of C written; seven traps caught early.
  - **Visual / Artefact Callout**: Summary card: `0 lines of C | 7 upstream traps caught in concepts_brief.md`.
  - **Spoken (`bjm-voice`)**: Zero C written, seven real traps caught upfront. Now watch what happens in Phase 3 when the Elephant drafts the full spec.

##### Beat 11 (`11:30–13:00`, `90.0s`, `11` steps) — The Elephant's Blind Spot (Step 4: First Draft Proposal via `03-draft` & Primary Spine Part 2)

- **`S085` (`11:30–11:38`, `8.0s`) `[Slide]`**: **Phase 3 (`/egm-draft`)** — Turning `concepts_brief.md` section-by-section into [`docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md`](../../docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md).
  - **Visual / Artefact Callout**: Workflow card: [`concepts_brief.md`](../../docs/egm/designs/concepts_brief.md) -> `/egm-draft` -> [`docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md`](../../docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md).
  - **Spoken (`bjm-voice`)**: Phase 3 runs `/egm-draft`, expanding that concepts brief section by section into a `693`-line architecture and implementation specification.

- **`S086` (`11:38–11:46`, `8.0s`) `[Slide]`**: **Freeze-Frame (`03-draft`)** — [`prototypes/03-draft-cast-player.html`](../../prototypes/03-draft-cast-player.html) at Stop Point `6. Q3: Section Approval — Technical Plan` (`01:30` warped, `15x` speed badge).
  - **Visual / Artefact Callout**: Freeze-frame embed of [`prototypes/03-draft-cast-player.html`](../../prototypes/03-draft-cast-player.html) paused at Stop Point `6. Q3: Section Approval — Technical Plan` (`01:30` warped, `15x` badge).
  - **Spoken (`bjm-voice`)**: Calling `/egm-draft` pauses explicitly at every section boundary—forcing me, the human, to review and approve the Technical Plan before continuing.

- **`S087` (`11:46–11:54`, `8.0s`) `[v-click]`**: **Semantic Boundaries** — Splits high-level `FeatureRange` architecture from `DetailedRange` implementation steps.
  - **Visual / Artefact Callout**: Two-band spec anatomy marker (`Goals & Architecture` vs. `Target Files #1..#30 Implementation Plan`).
  - **Spoken (`bjm-voice`)**: Splitting high-level architectural choices from file-by-file C instructions makes every single decision easy to read—except when both minds share a blind spot.

- **`S088` (`11:54–12:02`, `8.0s`) `[Slide]`**: **Primary Spine (Part 2): The Honest False Start** — How did our `v0-draft` plan to stop the `500 kHz` `START_ONCE` ADC storm?
  - **Visual / Artefact Callout**: Question card (`Primary Spine Part 2`): `How did v0-draft fix the 500 kHz ADC storm?`
  - **Spoken (`bjm-voice`)**: Remember earlier I promised no unblemished triumph story? Here is where our Elephant—and I—walked straight into a classic trap.

- **`S089` (`12:02–12:10`, `8.0s`) `[Code-step]`**: [`docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L33`](../../docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L33) — Highlights `RP23XX_ADC_DIV = 0xFFFF00` proposed to throttle conversion cadence.
  - **Visual / Artefact Callout**: Shiki wrapped markdown excerpt (`#L33`, highlight line `4`):
```markdown
* **Stable 50 Hz Potentiometer Sensing**: Sample `D0/A0` (`GPIO26`, `ADC0`)
  every 20 ms using a 4-sample burst average after `ANIOC_RESET_FIFO`,
  ... RP2350-E9 digital pad isolation (`IE=0`, `PUE=0`, `PDE=0`, `ISO=0`, `OD=1`),
  and hardware ADC clock pacing (`RP23XX_ADC_DIV = 0xFFFF00`, `~732 Hz`).
```
  - **Spoken (`bjm-voice`)**: The Elephant spotted a hardware divider register in the RP2350 headers—`RP23XX_ADC_DIV`—and proposed pacing the ADC down to seven hundred hertz.

- **`S090` (`12:10–12:18`, `8.0s`) `[Code-step]`**: [`docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L242-L245`](../../docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L242-L245) — Highlights `putreg32(0x00ffff00, RP23XX_ADC_DIV)` and `48,000,000 Hz / 65536 ≈ 732.42 Hz`.
  - **Visual / Artefact Callout**: Shiki excerpt (`#L242-L245`, highlight lines `2–4`):
```markdown
3. **Hardware Conversion Rate Pacing (`RP23XX_ADC_DIV`)**:
   In `rp23xx_bringup.c`, writing `putreg32(0x00ffff00, RP23XX_ADC_DIV)`
   sets the 16-bit integer divider to `65535`, pacing conversions to:
   $$f_{\text{sample}} = 48,000,000\text{ Hz} / 65536 \approx 732.42\text{ Hz}$$
   This reduces ADC interrupt overhead by 99.85%.
```
  - **Spoken (`bjm-voice`)**: Look how convincing that looked: exact hex mask `0x00ffff00`, neat LaTeX frequency arithmetic, `99.85%` interrupt reduction calculated right down to two decimal places.

- **`S091` (`12:18–12:26`, `8.0s`) `[Slide]`**: **Mutual Agreement Illusion** — `53` cumulative `README.md` reads across Phases 2–3, and *both* Brett and the Elephant approved it!
  - **Visual / Artefact Callout**: Approval card: `53 cumulative README.md reads` + `[Human Clicked: Approve Section]`.
  - **Spoken (`bjm-voice`)**: Fifty-three `README.md` reads deep, the prose sounded airtight, the frequency maths checked out, and I nodded along and clicked Approve.

- **`S092` (`12:26–12:34`, `8.0s`) `[v-click]`**: **Verdict Stamp** — **`DEAD WRONG`**.
  - **Visual / Artefact Callout**: Large diagonal crimson stamp across `RP23XX_ADC_DIV = 0xFFFF00`: `DEAD WRONG`.
  - **Spoken (`bjm-voice`)**: And we were both dead wrong. Our tidy divider register would have done literally nothing on real silicon.

- **`S093` (`12:34–12:42`, `8.0s`) `[Slide]`**: **Rensin's Central Warning Quote** — *"A costly failure is a plan the agent and operator agreed on too quickly, because both were looking at the same context."*
  - **Visual / Artefact Callout**: Full-screen quote card framing Dave Rensin's warning about shared context blindness.
  - **Spoken (`bjm-voice`)**: Dave Rensin describes this exact failure: a costly mistake is a plan both agent and operator agreed on inside the same context.

- **`S094` (`12:42–12:51`, `9.0s`) `[v-click]`**: **Why the Elephant Missed It** — Once a long thread adopts an elegant explanation, confirmation bias protects it from scrutiny.
  - **Visual / Artefact Callout**: Diagnosis note beneath quote: `Long context -> plausible assumption -> confirmation bias -> unchallenged C draft`.
  - **Spoken (`bjm-voice`)**: Once our Elephant locked onto `RP23XX_ADC_DIV`, confirmation bias kicked in; it never stopped to check whether `START_ONCE` even reads that divider.

- **`S095` (`12:51–13:00`, `9.0s`) `[v-click]`**: **Act III.c Pivot** — That is why we never hand `v0-draft` to a coder—we hand it to stateless Goldfish.
  - **Visual / Artefact Callout**: Act III.c transition arrow: `v0-draft.md -> /egm-review (Stateless Goldfish Swarm)`.
  - **Spoken (`bjm-voice`)**: Which is precisely why nobody writes C yet. Watch what happens next when zero-context Goldfish review `v0-draft` cold.

#### Act III.c — The Goldfish Swarm (`13:00–17:30`, `4.5 min`, Beats `12–15`, `S096–S123`)

##### Beat 12 (`13:00–14:15`, `75.0s`, `7` steps) — The Goldfish Protocol (Steps 5–9: Stateless 3-Second-Memory Reviewers in `04-review` Pass 1)

- **`S096` (`13:00–13:07.3`, `7.3s`) `[Slide]`**: **Meet the Goldfish** — Fresh, stateless subagents with a **"three-second memory"**: zero brainstorm history, zero loyalty to `v0-draft`.
  - **Visual / Artefact Callout**: Persona card citing [`/Users/brett/.gemini/config/plugins/egm/docs/index.md#L45-L60`](/Users/brett/.gemini/config/plugins/egm/docs/index.md#L45-L60): `The Goldfish — 3-second memory, zero brainstorm bias`.
  - **Spoken (`bjm-voice`)**: A Goldfish is a fresh subagent with zero memory of our brainstorm, zero chat history, and zero emotional loyalty to `v0-draft`.

- **`S097` (`13:07.3–13:15.3`, `8.0s`) `[Slide]`**: **5-Stage `/egm-review` Pipeline (Steps 5–9)** — `Zoom Out` -> `Comprehension` -> `Critic` -> `Intermediate Zoom Out` -> `Implementation Readiness + Codebase Details`.
  - **Visual / Artefact Callout**: Five-stage horizontal pipeline (`5. Zoom Out` -> `6. Comprehension` -> `7. Critic` -> `8. Intermediate Zoom Out` -> `9. Readiness + Codebase Details`).
  - **Spoken (`bjm-voice`)**: `/egm-review` runs five adversarial stages: high-altitude Zoom Out, naive Comprehension, hostile Design Critic, Synthesizer, and line-by-line Codebase Details.

- **`S098` (`13:15.3–13:23.3`, `8.0s`) `[v-click]`**: **Pass 1 Scale Counter** — `16` Goldfish subagents in `04-review` reading **`146` `README.md` contracts** against the draft.
  - **Visual / Artefact Callout**: Counter pills from [`docs/reports/vanilla-vs-egm-comparison.md#L50`](../../docs/reports/vanilla-vs-egm-comparison.md#L50): `16 Goldfish subagents` | `146 README.md reads` | `653 read_file calls`.
  - **Spoken (`bjm-voice`)**: Across Pass 1 and Pass 2, sixteen fresh Goldfish fanned out in parallel, checking every claim against one hundred and forty-six contracts.

- **`S099` (`13:23.3–13:53.0`, `29.7s`) `[Live-cast]`**: **LIVE CAST (`29.7s`, Beat A)** on [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html).
  - **Visual / Artefact Callout**: Launch [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html). Jump to Stop Point `4. Pass 1 Q1: Select Review Execution Mode (Consolidated)` (`00:11` warped) and press `p` to run **Seg 05** (`29.7s @ 39x`, compressing `21m 33s` raw -> auto-pauses at `5. Pass 1 Review Complete`).
  - **Spoken (`bjm-voice`)**: I invoke `/egm-review` on [`xiao-rp2350-bargraph.md`](../../docs/egm/designs/xiao-rp2350-bargraph.md) and choose `consolidated` mode. Look at the effect: instead of one chat thread nodding along with itself, parallel Goldfish subagents spin up cold across five stages, cross-checking every register address, Kconfig guard, and fixed-point equation in our spec directly against NuttX's driver files and `README.md` contracts. Compressing twenty-one and a half minutes at thirty-nine times speed, those stateless reviewers pile into `pass1/goldfish_review_report.md`—and our clean draft is about to look very bruised.

- **`S100` (`13:53.0–14:00.0`, `7.0s`) `[Slide]`**: **Pass 1 Harvest ([`docs/egm/reports/pass1/goldfish_review_report.md`](../../docs/egm/reports/pass1/goldfish_review_report.md))** — **`4` Blockers (`B-1..B-4`)** + **`5` Majors (`M-1..M-5`)** surfaced before any C was written.
  - **Visual / Artefact Callout**: Finding summary badge from [`docs/egm/reports/pass1/goldfish_review_report.md`](../../docs/egm/reports/pass1/goldfish_review_report.md): `Pass 1 Harvest: 4 Blockers (B-1..B-4), 5 Majors (M-1..M-5), 3 Nits`.
  - **Spoken (`bjm-voice`)**: The verdict lands: four Blockers, five Majors, and three Nits—twelve real discrepancies surfaced in markdown before a single line of C existed.

- **`S101` (`14:00.0–14:08.0`, `8.0s`) `[v-click]`**: **The Blocker Payoff** — Every single Blocker would have burned hours at a JTAG debugger or panicked the kernel.
  - **Visual / Artefact Callout**: Severity tag strip highlighting `B-1` (`ADC_DIV no-op`), `B-2` (`4x EMA gain`), `B-3` (`Kconfig drop/abort`), `B-4` (`6 driver details`).
  - **Spoken (`bjm-voice`)**: Every one of those four Blockers would have burned half a weekend with a multimeter and JTAG debugger if we had shipped first.

- **`S102` (`14:08.0–14:15.0`, `7.0s`) `[v-click]`**: **Interactive Resolution Cue** — Watch `/egm-resolve` walk us straight into Item 1 of 12: Blocker `B-1`.
  - **Visual / Artefact Callout**: Command preview pill: `/egm-resolve -> interactive -> Item 1/12 (B-1)`.
  - **Spoken (`bjm-voice`)**: Next, `/egm-resolve` steps through each finding with me in `interactive` mode—starting right at Item 1, Blocker `B-1`.

##### Beat 13 (`14:15–15:30`, `75.0s`, `8` steps) — Primary Spine (Part 3a): Pass 1 `B-1` Proves `RP23XX_ADC_DIV` Is a No-Op Under `START_ONCE`

- **`S103` (`14:15–14:23.4`, `8.4s`) `[Slide]`**: **Primary Spine Payoff Setup** — `egm-codebase-details` checks `v0-draft`'s `RP23XX_ADC_DIV` plan directly against [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c).
  - **Visual / Artefact Callout**: Audit comparison header: `v0-draft.md#L242` vs. [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L169-L320`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L169-L320).
  - **Spoken (`bjm-voice`)**: Because `egm-codebase-details` wakes up cold, it has zero reason to trust our `732 Hz` divider arithmetic. It opens `rp23xx_adc.c` itself.

- **`S104` (`14:23.4–14:42.0`, `18.6s`) `[Live-cast]`**: **LIVE CAST (`18.6s`, Beat B)** on [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html).
  - **Visual / Artefact Callout**: Inside [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html), click pill `8. Reply: interactive (Walk Through 12 Findings)` (`01:10` warped) and press `p` through **Seg 09** (`17.1s @ 2x` -> `9. Item 1/12 (B-1): NVIC IRQ Gating vs. RP23XX_ADC_DIV`) and **Seg 10** (`1.5s @ 2x` -> `10. Reply: accept`).
  - **Spoken (`bjm-voice`)**: Selecting `interactive` mode forces `/egm-resolve` to walk me through each blocker one by one instead of auto-applying blind patches. Right on Item 1 of 12, `B-1` cites lines 169 through 320 of `rp23xx_adc.c` and lays out the replacement: NVIC IRQ gating around each fifty-hertz burst. Watching that evidence appear, I type `accept`—no ego, just cold proof.

- **`S105` (`14:42.0–14:50.0`, `8.0s`) `[Slide]`**: **`B-1` Finding Card ([`docs/egm/reports/pass1/goldfish_review_report.md#L8-L16`](../../docs/egm/reports/pass1/goldfish_review_report.md#L8-L16))** — **`RP23XX_ADC_DIV` only governs free-running `START_MANY` mode.**
  - **Visual / Artefact Callout**: 3-field Goldfish card (`Issue B-1`): **Reviewers:** `egm-zoom-out / egm-inventive-catalyst / egm-design / egm-codebase-details` (`Blocker`) | **Discrepancy:** `rp23xx_adc.c` never sets `START_MANY`; it writes single-shot `START_ONCE` (`#L304`), where `RP23XX_ADC_DIV` has no effect | **Fix:** Remove `RP23XX_ADC_DIV`; gate `RP23XX_ADC_IRQ_FIFO` via NVIC.
  - **Spoken (`bjm-voice`)**: Here is what `B-1` actually proved: the RP2350 `ADC_DIV` register only paces free-running `START_MANY` conversions—it never runs under `START_ONCE`.

- **`S106` (`14:50.0–14:58.0`, `8.0s`) `[v-click]`**: **Hardware Reality** — Under NuttX's `START_ONCE` driver, `RP23XX_ADC_DIV = 0xFFFF00` is a **total no-op**—the `500 kHz` IRQ storm still fires!
  - **Visual / Artefact Callout**: Red consequence callout appended to `B-1` card: `RP23XX_ADC_DIV = 0xFFFF00 -> 0 effect -> 500,000 IRQ/s CPU starvation`.
  - **Spoken (`bjm-voice`)**: Our clever `0xFFFF00` divider was a complete no-op; the Cortex-M33 would still have been drowned under half a million interrupts per second.

- **`S107` (`14:58.0–15:06.0`, `8.0s`) `[Slide]`**: **The Goldfish Solution (`B-1`)** — Keep `/dev/adc0` open (dodging Vanilla's `40 Hz` `close(fd)` teardown race) and gate at the **NVIC** instead.
  - **Visual / Artefact Callout**: Architecture card: `Keep /dev/adc0 open + Mask/Unmask NVIC RP23XX_ADC_IRQ_FIFO (250 IRQ/s total)`.
  - **Spoken (`bjm-voice`)**: Instead of closing `/dev/adc0` like Vanilla did, the Goldfish proposed keeping it persistently open and gating the interrupt line directly in the NVIC.

- **`S108` (`15:06.0–15:14.0`, `8.0s`) `[Code-step]`**: [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L266-L293`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L266-L293) (`bargraph_sample_adc()`) — Highlights gating `RP23XX_ADC_IRQ_FIFO` around a 5-sample burst.
  - **Visual / Artefact Callout**: Shiki condensed excerpt (`#L266-L293`, highlight lines `1, 2, 7`):
```c
  (void)ioctl(adc_fd, ANIOC_RESET_FIFO, 0);
  up_enable_irq(RP23XX_ADC_IRQ_FIFO);
  while (total_read < bytes_needed)
    {
      ssize_t nread = read(adc_fd, buf + total_read, bytes_needed - total_read);
      ... /* ~10 us burst for 5 samples (25 bytes) */
    }
  up_disable_irq(RP23XX_ADC_IRQ_FIFO);
```
  - **Spoken (`bjm-voice`)**: Flush the FIFO, unmask `RP23XX_ADC_IRQ_FIFO` for ten microseconds to pull five samples, then mask it—dropping 500,000 interrupts down to 250.

- **`S109` (`15:14.0–15:22.0`, `8.0s`) `[v-click]`**: **Cost of Discovery** — Caught in spec review: 10 seconds to accept. Caught on hardware: hours with a logic analyser.
  - **Visual / Artefact Callout**: Cost comparison strip: `Spec review catch: 10s | Hardware JTAG / logic analyser catch: hours`.
  - **Spoken (`bjm-voice`)**: Ten seconds to type `accept` in markdown versus hours staring at a stalled Cortex-M33 under a logic analyser.

- **`S110` (`15:22.0–15:30.0`, `8.0s`) `[v-click]`**: **And `B-1` Wasn't Alone** — Two other Pass 1 blockers caught pure-maths and Kconfig failures simultaneously.
  - **Visual / Artefact Callout**: Teaser pill: `Also in Pass 1: B-2 (Fixed-Point Maths) & B-3 (Kconfig)`.
  - **Spoken (`bjm-voice`)**: And `B-1` was only the headline catch. Let's look at `B-2` and `B-3` sitting right underneath it in the same report.

##### Beat 14 (`15:30–16:15`, `45.0s`, `6` steps) — Supporting Trap #3 & Build Guard: Pass 1 `B-2` (4x Q4 EMA Bug) & `B-3` (`CONFIG_ANALOG=y`)

- **`S111` (`15:30–15:38`, `8.0s`) `[Slide]`**: **Supporting Trap #3 (`B-2`)** — Pure logic/maths verification inside [`docs/egm/reports/pass1/goldfish_review_report.md#L20-L28`](../../docs/egm/reports/pass1/goldfish_review_report.md#L20-L28).
  - **Visual / Artefact Callout**: 3-field Goldfish card (`Issue B-2`): **Reviewers:** `egm-codebase-details / egm-implementation-readiness` (`Blocker`) | **Discrepancy:** `ema_q4 >> 2` yields `4R` `[0, 16380]` instead of `[0, 4095]`; missing first-sample seed | **Fix:** Seed `raw_avg << 4`, round `(ema_q4 + 8) >> 4`.
  - **Spoken (`bjm-voice`)**: Goldfish do not only read C files—in Issue `B-2`, one verified our fixed-point exponential moving average algebra by hand.

- **`S112` (`15:38–15:45`, `7.0s`) `[Code-step]`**: [`docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L253-L254`](../../docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L253-L254) — Highlights buggy `filtered = ema_q4 >> 2` (`[0, 16380]`, **`4x` gain**!).
  - **Visual / Artefact Callout**: Shiki excerpt (`#L253-L254`, highlight line `4`):
```c
/* v0-draft.md#L254: Q4 state (alpha = 1/4) */
ema_q4 = ema_q4 - (ema_q4 >> 2) + (raw_avg << 2);
/* Steady state: ema_q4 = 16 * raw_avg (= raw_avg << 4) */
filtered = ema_q4 >> 2; /* BUG: outputs 4 * raw_avg [0..16380]! */
```
  - **Spoken (`bjm-voice`)**: In `v0-draft`, we wrote `ema_q4 >> 2`—leaving a four-times gain error that pegged all six LEDs at 100% by quarter-turn pot travel.

- **`S113` (`15:45–15:53`, `8.0s`) `[Code-step]`**: [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L661-L671`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L661-L671) (`bargraph_filter_adc()`) — Highlights first-sample seeding `raw_avg << 4` and `(ema_q4 + 8u) >> 4`.
  - **Visual / Artefact Callout**: Shiki condensed excerpt (`#L661-L671`, highlight lines `2, 8`):
```c
  if (!(*ema_initialized))
    {
      *ema_q4          = (uint32_t)raw_avg << 4;
      *ema_initialized = true;
    }
  else
    *ema_q4 = *ema_q4 - (*ema_q4 >> 2) + ((uint32_t)raw_avg << 2);
  filtered = (uint16_t)((*ema_q4 + 8u) >> 4);
```
  - **Spoken (`bjm-voice`)**: `B-2` fixed both: seeding `raw_avg << 4` on the first sample so startup never ramps from zero, then rounding `(*ema_q4 + 8u) >> 4`.

- **`S114` (`15:53–16:00`, `7.0s`) `[Slide]`**: **Build-System Guard (`B-3`)** — Pre-build Kconfig check from [`docs/egm/reports/pass1/goldfish_review_report.md#L32-L42`](../../docs/egm/reports/pass1/goldfish_review_report.md#L32-L42).
  - **Visual / Artefact Callout**: 3-field Goldfish card (`Issue B-3`): **Reviewers:** `egm-design / egm-codebase-details / egm-implementation-readiness` (`Blocker`) | **Discrepancy:** Missing `CONFIG_ANALOG=y` silently drops `CONFIG_ADC=y`; `CONFIG_PWM_MULTICHAN=y` does not exist in `timers/Kconfig` | **Fix:** Drop `CONFIG_PWM_MULTICHAN=y`, add `CONFIG_ANALOG=y`.
  - **Spoken (`bjm-voice`)**: Meanwhile, Issue `B-3` audited every single Kconfig symbol in our draft directly against `nuttx/drivers/timers/Kconfig` and `nuttx/drivers/analog/Kconfig`.

- **`S115` (`16:00–16:08`, `8.0s`) `[v-click]`**: **Two Kconfig Catches** — Missing `CONFIG_ANALOG=y` (Vanilla's Step 180 silent drop!) + hallucinated `CONFIG_PWM_MULTICHAN=y`.
  - **Visual / Artefact Callout**: Diff strip: `+ CONFIG_ANALOG=y` (prevents silent `CONFIG_ADC` drop) and `- CONFIG_PWM_MULTICHAN=y` (prevents `olddefconfig` `exit 1`).
  - **Spoken (`bjm-voice`)**: It caught both Vanilla's missing `CONFIG_ANALOG=y` drop and a hallucinated `CONFIG_PWM_MULTICHAN=y` symbol before `configure.sh` ever ran.

- **`S116` (`16:08–16:15`, `7.0s`) `[v-click]`**: **Multi-Angle Defence** — One Goldfish checks silicon errata, another checks fixed-point shifts, another checks Kconfig menus.
  - **Visual / Artefact Callout**: Transition pill: `Next: Why ONE review pass still isn't enough`.
  - **Spoken (`bjm-voice`)**: One pass caught four huge Blockers. Now watch why stopping after a single review pass is still a rookie mistake.

##### Beat 15 (`16:15–17:30`, `75.0s`, `7` steps) — Primary Spine (Part 3b): `/clear` + Pass 2 `CA-01` (The 5-Byte `adc_read()` Short-Read Trap)

- **`S117` (`16:15–16:22.9`, `7.9s`) `[Slide]`**: **The Second-Order Patch Trap** — Fixing `B-1` added a 5-sample `read()` burst to the spec; could our *own patch* hide a new bug?
  - **Visual / Artefact Callout**: Recursive risk card: `Fixing B-1 -> Added 5-sample (25 B) read() burst -> Unreviewed new prose!`.
  - **Spoken (`bjm-voice`)**: Every time you patch a spec to resolve a blocker, you write fresh assumptions that no Goldfish has ever examined.

- **`S118` (`16:22.9–16:29.9`, `7.0s`) `[v-click]`**: **Enter `/clear`** — Archive Pass 1, wipe the parent session memory completely, and launch Pass 2 cold.
  - **Visual / Artefact Callout**: Bold terminal badge: `/clear` (`Archive Pass 1 -> Wipe Parent Context -> Launch Pass 2 Cold`).
  - **Spoken (`bjm-voice`)**: So we archive Pass 1, issue `/clear` to wipe parent memory completely, and run `/egm-review` a second time cold.

- **`S119` (`16:29.9–17:00.0`, `30.1s`) `[Live-cast]`**: **LIVE CAST (`30.1s`, Beat C)** on [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html).
  - **Visual / Artefact Callout**: Inside [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html), click pill `13. Pass 1 Archived — Ready for /clear` (`02:17` warped), press `p` through **Seg 14–16** (`1.1s @ 2x`, showing `/clear` + Pass 2 `/egm-review (with GoldfishContext)`), then click pill `17. Pass 2 Q1: Select Review Execution Mode (Consolidated)` (`02:41` warped) and press `p` through **Seg 18** (`29.0s @ 26x` -> auto-pauses at `18. Pass 2 Review Complete`).
  - **Spoken (`bjm-voice`)**: Watch the exact sequence: Pass 1 is archived, we fire `/clear` so the session that wrote the `B-1` patch cannot grade its own homework, and we launch Pass 2 `/egm-review` in consolidated mode. Immediately at twenty-six times speed, a fresh wave of stateless Goldfish reads our brand-new NVIC IRQ gating section alongside NuttX's upper-half analogue driver `adc.c`—and catches a subtle second-order bug inside our own fix.

- **`S120` (`17:00.0–17:08.0`, `8.0s`) `[Slide]`**: **Pass 2 Catch `CA-01` ([`docs/egm/reports/pass2/goldfish_review_report.md#L9-L17`](../../docs/egm/reports/pass2/goldfish_review_report.md#L9-L17))** — Upper-half `adc_read()` **short-read behaviour**!
  - **Visual / Artefact Callout**: 3-field Goldfish card (`Issue 1 (CA-01)`): **Reviewers:** `egm-design / egm-codebase-details / egm-implementation-readiness` (`Major`) | **Discrepancy:** `adc_read()` wakes on `af_sem` and returns `5` bytes as soon as `1` `struct adc_msg_s` arrives | **Fix:** Loop `read()` (`EINTR`-safe) to `25` bytes before masking IRQ.
  - **Spoken (`bjm-voice`)**: Pass 2 returned `0` Blockers and caught `CA-01`—a sneaky short-read behaviour inside our brand-new `B-1` burst specification.

- **`S121` (`17:08.0–17:15.0`, `7.0s`) `[Code-step]`**: [`nuttx/drivers/analog/adc.c#L270-L305`](../../nuttx/drivers/analog/adc.c#L270-L305) (`adc_read()`) — Highlights `while (fifo->af_head == fifo->af_tail)` waking on `nxsem_wait(&fifo->af_sem)` and returning once `1` `5`-byte message lands.
  - **Visual / Artefact Callout**: Shiki condensed excerpt (`#L270-L305`, highlight lines `1, 3, 7`):
```c
  while (fifo->af_head == fifo->af_tail)
    {
      ret = nxsem_wait(&fifo->af_sem); /* Wakes on 1st sample! */
    }
  /* The FIFO is not empty. Copy all buffered data that will fit
   * in the user buffer. (Returns 5 bytes immediately, not 25!)
   */
```
  - **Spoken (`bjm-voice`)**: Asking POSIX `read()` for twenty-five bytes does not wait for twenty-five bytes; `adc_read()` wakes after one five-byte sample, leaving `msgs[1..4]` as uninitialised stack garbage.

- **`S122` (`17:15.0–17:23.0`, `8.0s`) `[Code-step]`**: [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L270-L306`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L270-L306) (`bargraph_sample_adc()`) — Highlights `while (total_read < bytes_needed)` (`25` B, `EINTR`-safe) and `for (i = 1; ...)` discarding stale `msgs[0]`.
  - **Visual / Artefact Callout**: Shiki condensed excerpt (`#L270-L306`, highlight lines `1–3, 8`):
```c
  while (total_read < bytes_needed) /* 25 bytes = 5 x 5-byte msgs */
    {
      ssize_t nread = read(adc_fd, buf + total_read, bytes_needed - total_read);
      if (nread < 0 && errno == EINTR) continue;
      total_read += (size_t)nread;
    }
  up_disable_irq(RP23XX_ADC_IRQ_FIFO);
  for (i = 1; i < BARGRAPH_ADC_READ_SAMPLES; i++) /* discard msgs[0] */
```
  - **Spoken (`bjm-voice`)**: So the Pass 2 spec locks in the exact `EINTR`-safe twenty-five-byte accumulation loop, masks the IRQ after, and discards stale `msgs[0]`.

- **`S123` (`17:23.0–17:30.0`, `7.0s`) `[v-click]`**: **Two-Pass Spine Summary** — Pass 1 killed `RP23XX_ADC_DIV`; `/clear` + Pass 2 killed the `5`-byte stack-garbage read.
  - **Visual / Artefact Callout**: Two-pass shield card: `Pass 1 (B-1): Killed ADC_DIV | /clear + Pass 2 (CA-01): Killed 5-byte short-read`.
  - **Spoken (`bjm-voice`)**: Pass 1 killed the fake clock divider; `/clear` plus Pass 2 killed the stack-garbage read. Now—finally—we write some C.

#### Act III.d — Boring Implementation (`17:30–19:00`, `1.5 min`, Beat `16`, `S124–S132`)

##### Beat 16 (`17:30–19:00`, `90.0s`, `9` steps) — Phase 5 (`05-implement`): First-Attempt `nuttx.uf2` & Rolling the Map Forward

- **`S124` (`17:30–17:38`, `8.0s`) `[Slide]`**: **Phase 5 (`05-implement`) Setup** — Handing the Pass-2-hardened [`docs/egm/designs/xiao-rp2350-bargraph.md`](../../docs/egm/designs/xiao-rp2350-bargraph.md) spec to an implementation session.
  - **Visual / Artefact Callout**: Phase 5 handoff card: `Pass-2 Spec (705 lines, 0 Blockers) -> Phase 5 Implementer`.
  - **Spoken (`bjm-voice`)**: With four Blockers and seven Majors killed inside markdown, Phase 5 hands [`xiao-rp2350-bargraph.md`](../../docs/egm/designs/xiao-rp2350-bargraph.md) to an implementer so coding stays calm and boring.

- **`S125` (`17:38–18:07`, `29.0s`) `[Live-cast]`**: **LIVE CAST (`29.0s`)** on [`prototypes/05-implement-cast-player.html`](../../prototypes/05-implement-cast-player.html).
  - **Visual / Artefact Callout**: Switch to [`prototypes/05-implement-cast-player.html`](../../prototypes/05-implement-cast-player.html). Jump to Stop Point `3. Prompt: Implement Firmware & Verify Build` (`00:04` warped) and press `p` through **Seg 04** (`29.0s @ 27x`, compressing `13m 04s` raw -> auto-pauses at `4. Firmware Built & Tested`).
  - **Spoken (`bjm-voice`)**: Here is the prompt handing that Pass-2-hardened spec straight to implementation. Notice the behavioural effect compared to Vanilla: there are zero architectural guesses left to improvise mid-flight. At twenty-seven times speed across thirteen raw minutes, the session authors the custom board files, `rp23xx_bringup.c`, `bargraph_main.c`, and nine new directory contracts, fixes one macOS BSD `mktemp` flag in the host test harness, and runs `configure.sh` plus the NuttX ARM build—delivering a clean UF2 firmware binary on the very first compile attempt.

- **`S126` (`18:07–18:15`, `8.0s`) `[Slide]`**: **First-Attempt Build Verdict** — **`nuttx/nuttx.uf2` (`347K` — `177,232 B` FLASH / `13,712 B` RAM): built cleanly on Attempt #1**.
  - **Visual / Artefact Callout**: Build victory badge citing [`docs/reports/vanilla-vs-egm-comparison.md#L39`](../../docs/reports/vanilla-vs-egm-comparison.md#L39): `Attempt #1 Build: nuttx/nuttx.uf2 (347K — 177,232 B FLASH / 13,712 B RAM)`.
  - **Spoken (`bjm-voice`)**: `nuttx/nuttx.uf2`, three hundred forty-seven kilobytes, compiled, linked, and packed cleanly on the very first attempt—zero firmware build failures.

- **`S127` (`18:15–18:22`, `7.0s`) `[v-click]`**: **Telemetry Footnote** — `0` firmware retries (`1` BSD `mktemp` fix in `Makefile` for macOS host `make test` at Step 74; `0` `xmllint` errors).
  - **Visual / Artefact Callout**: Footnote pill (`0 firmware retries | 1 macOS BSD mktemp fix at Step 74 | 0 xmllint errors`).
  - **Spoken (`bjm-voice`)**: Our lone build hiccup was a macOS BSD `mktemp` flag inside the host test target—zero NuttX Kconfig drops or compiler errors.

- **`S128` (`18:22–18:30`, `8.0s`) `[Slide]`**: **Rolling the Map Forward** — [`docs/egm/xiao-rp2350-bargraph/README.md#L72-L76`](../../docs/egm/xiao-rp2350-bargraph/README.md#L72-L76) (`9` new 5-section contracts generated for the new code).
  - **Visual / Artefact Callout**: Contract tree snippet showing `9` newly authored 5-section `README.md` contracts under [`docs/egm/xiao-rp2350-bargraph/`](../../docs/egm/xiao-rp2350-bargraph/README.md).
  - **Spoken (`bjm-voice`)**: Better yet, Phase 5 rolls the map forward—adding nine new five-section `README.md` contracts covering every directory we introduced.

- **`S129` (`18:30–18:37`, `7.0s`) `[v-click]`**: **Honest Contract Boundary (`CA-02`)** — Explicitly documents the `CONFIG_BUILD_FLAT=y` assumption permitting `apps/` to call kernel `up_*_irq()`.
  - **Visual / Artefact Callout**: Highlighted excerpt of [`docs/egm/xiao-rp2350-bargraph/README.md#L75`](../../docs/egm/xiao-rp2350-bargraph/README.md#L75): `Intentional CONFIG_BUILD_FLAT=y Boundary Exceptions`.
  - **Spoken (`bjm-voice`)**: Including Pass 2 `CA-02`: explicitly documenting why flat-build `bargraphd` calls kernel `up_enable_irq` directly without modifying upstream submodules.

- **`S130` (`18:37–18:45`, `8.0s`) `[Slide]`**: **Wiring & SVG Flash** — [`docs/egm/BREADBOARD_WIRING.md`](../../docs/egm/BREADBOARD_WIRING.md) + [`docs/egm/breadboard-wiring.svg`](../../docs/egm/breadboard-wiring.svg) (`330 Ω` resistors, `~3.94 mA`/LED, `100 nF` wiper bypass cap `C1`).
  - **Visual / Artefact Callout**: Full `viewBox="0 0 1160 1120"` overview of [`docs/egm/breadboard-wiring.svg`](../../docs/egm/breadboard-wiring.svg) beside `330 Ω` (`~3.94 mA`/LED, `23.6 mA` total) + `100 nF` bypass cap `C1` badges.
  - **Spoken (`bjm-voice`)**: And it generates a consistent wiring guide and SVG diagram: `330`-ohm resistors, `23.6` milliamps total, plus a `100`-nanofarad wiper capacitor.

- **`S131` (`18:45–18:53`, `8.0s`) `[v-click]`**: **Foreshadowing Seed** — Notice the XIAO RP2350 seated in `Cols E & F` in that SVG? Keep that mental bookmark for Act V.
  - **Visual / Artefact Callout**: Zoomed `viewBox="150 130 500 500"` crop of [`docs/egm/breadboard-wiring.svg#L121, L280-L333`](../../docs/egm/breadboard-wiring.svg#L121) highlighting `Cols E & F` (`x=340` / `x=460`).
  - **Spoken (`bjm-voice`)**: Keep one eye on the XIAO seated in columns `E` and `F` right there—we will measure that with a ruler in Act V.

- **`S132` (`18:53–19:00`, `7.0s`) `[v-click]`**: **Act IV Pivot** — The agent nailed the architecture; now comes the harder question: *do you understand the C you're about to ship?*
  - **Visual / Artefact Callout**: Act IV transition banner (`Next -> Rolling Docs for the Human: /teach-code`).
  - **Spoken (`bjm-voice`)**: The agent nailed the firmware; now comes the harder question—do you actually understand the eight hundred lines of C you're about to ship?

---

### Act IV — Rolling Docs for the Human: The `/teach-code` Codelab (`19:00–25:00`, `6.0 min`, Beats `17–22`, `S133–S176`)

#### Beat 17 (`19:00–20:00`, `60.0s`, `8` steps) — "Humans Own the Outcomes": Comprehension Debt & Matt Pocock's `/teach`

- **`S133` (`19:00–19:08`, `8.0s`) `[Slide]`**: **Act IV Divider** — **Rolling Docs for the Human: Paying Down Comprehension Debt**.
  - **Visual / Artefact Callout**: Act IV full-screen section header (`Act IV — Rolling Docs for the Human: The /teach-code Codelab`).
  - **Spoken (`bjm-voice`)**: Part 2 of "Roll Your Own Docs" flips the camera straight away from the coding agents and back onto us engineers.

- **`S134` (`19:08–19:15`, `7.0s`) `[Slide]`**: **Rensin's Third Role ([`/Users/brett/.gemini/config/plugins/egm/docs/index.md#L61-L75`](/Users/brett/.gemini/config/plugins/egm/docs/index.md#L61-L75))** — **"Humans Own the Outcomes."**
  - **Visual / Artefact Callout**: Role card highlighting Rensin's third pillar: `The Elephant | The Goldfish | The Humans ("Humans own the outcomes")`.
  - **Spoken (`bjm-voice`)**: Dave Rensin pairs the Elephant and Goldfish with a blunt third rule for the person holding the keyboard: humans own the outcomes.

- **`S135` (`19:15–19:23`, `8.0s`) `[v-click]`**: **Verbatim Warning** — *"Sorry! My AI tool just pushed broken code to production..."* isn't an excuse—it was you that pushed it.
  - **Visual / Artefact Callout**: Verbatim excerpt from [`/Users/brett/.gemini/config/plugins/egm/docs/index.md#L63-L65`](/Users/brett/.gemini/config/plugins/egm/docs/index.md#L63-L65).
  - **Spoken (`bjm-voice`)**: As Dave writes, "Sorry, my AI tool just pushed broken code to production" will not fly—it was still you who pushed it.

- **`S136` (`19:23–19:30`, `7.0s`) `[Slide]`**: **The Comprehension Gap** — Our Elephant and Goldfish understand `rp23xx_adc.c`, `RP2350-E9`, and NVIC IRQ gating (`10 / 10` primary pass).
  - **Visual / Artefact Callout**: Contrast meter: `Agent & Repo Docs: 10 / 10 NuttX + RP2350 mastery` vs. `Human mental model: ?`.
  - **Spoken (`bjm-voice`)**: Right now, our repo contracts and Goldfish reports understand `rp23xx_adc.c`, `RP2350-E9` leakage, and NVIC gating cold—scoring ten out of ten.

- **`S137` (`19:30–19:38`, `8.0s`) `[v-click]`**: **The Human Confession** — *You* didn't write those 800 lines of C. When hardware misbehaves in six months, how fast can you reason about them?
  - **Visual / Artefact Callout**: Warning label: `Comprehension Debt = shipping 872 lines of bargraph_main.c you didn't write by hand`.
  - **Spoken (`bjm-voice`)**: Yet I did not write those eight hundred lines of C myself. Come back in six months and that becomes silent comprehension debt.

- **`S138` (`19:38–19:45`, `7.0s`) `[Slide]`**: **Credit Where Due: Matt Pocock's `/teach`** — [`.agents/skills/teach/SKILL.md#L12-L53, L110`](../../.agents/skills/teach/SKILL.md#L12-L53).
  - **Visual / Artefact Callout**: Attribution card crediting Matt Pocock's open-source `/teach` skill ([`.agents/skills/teach/SKILL.md`](../../.agents/skills/teach/SKILL.md)).
  - **Spoken (`bjm-voice`)**: To pay down that debt deliberately, I started from Matt Pocock's brilliant open-source `/teach` skill instead of inventing pedagogy from scratch.

- **`S139` (`19:45–19:53`, `8.0s`) `[v-click]`**: **Cognitive Science Backbone** — Zone of Proximal Development (`learning-records/`), Edward Tufte layout, and retrieval quizzes (*Fluency vs. Storage Strength*).
  - **Visual / Artefact Callout**: Three cognitive pillars from [`.agents/skills/teach/SKILL.md#L17, L34-L53, L81-L89`](../../.agents/skills/teach/SKILL.md#L17): `ZPD learning-records/`, `Tufte sidenotes`, and `Fluency vs. Storage Strength`.
  - **Spoken (`bjm-voice`)**: `/teach` builds durable storage strength over surface fluency using Zone of Proximal Development records, Edward Tufte layout, and active retrieval quizzes.

- **`S140` (`19:53–20:00`, `7.0s`) `[v-click]`**: **Why Static Prose Breaks for Firmware** — C listings inside tutorials silently drift out of sync with real compiling C99 code.
  - **Visual / Artefact Callout**: Drift hazard card: `Hand-pasted tutorial snippets <-> Compiling C99 code` (`Silent Divergence`).
  - **Spoken (`bjm-voice`)**: One catch with teaching embedded C, though: pasted code blocks inside static HTML tutorials quickly drift away from code that actually compiles.

#### Beat 18 (`20:00–21:00`, `60.0s`, `8` steps) — Evolving `/teach` -> `/teach-code` via `/agy-customizations`

- **`S141` (`20:00–20:08`, `8.0s`) `[Slide]`**: **Skill Diff Header** — [`.agents/skills/teach/SKILL.md`](../../.agents/skills/teach/SKILL.md) -> [`.agents/skills/teach-code/SKILL.md`](../../.agents/skills/teach-code/SKILL.md) (`#L17-L32, L56-L110`) via `/agy-customizations`.
  - **Visual / Artefact Callout**: Side-by-side header comparing [`.agents/skills/teach/SKILL.md`](../../.agents/skills/teach/SKILL.md) and [`.agents/skills/teach-code/SKILL.md`](../../.agents/skills/teach-code/SKILL.md) modified via [`/Users/brett/.gemini/antigravity/builtin/skills/agy-customizations/SKILL.md`](/Users/brett/.gemini/antigravity/builtin/skills/agy-customizations/SKILL.md).
  - **Spoken (`bjm-voice`)**: So over repeated real projects I gently shaped Matt's `/teach` skill into `/teach-code` using Antigravity's built-in `/agy-customizations` skill.

- **`S142` (`20:08–20:15`, `7.0s`) `[v-click]`**: **Kept Intact (Structure)** — `MISSION.md`, `RESOURCES.md`, and ADR-style `learning-records/*.md` diagnosing the learner's starting point.
  - **Visual / Artefact Callout**: Preserved structure tags (`MISSION.md`, `RESOURCES.md`, `learning-records/*.md` ZPD ADRs) checked green.
  - **Spoken (`bjm-voice`)**: We kept the soul of `/teach` untouched: `MISSION.md`, verified `RESOURCES.md`, and ADR-style learning records tracking the learner's starting edge.

- **`S143` (`20:15–20:23`, `8.0s`) `[v-click]`**: **Kept Intact (Ergonomics)** — Printable `reference/*` sheets, Tufte right-margin sidenotes, and equal-word-count multiple-choice quizzes.
  - **Visual / Artefact Callout**: Preserved ergonomics tags (`reference/*.md`, Tufte sidenotes, equal-word-count `:::quiz` rules from `.agents/skills/teach/SKILL.md#L110`).
  - **Spoken (`bjm-voice`)**: Same printable reference sheets, same distraction-free Tufte margin notes, and Matt's rule forcing every quiz option to share identical word counts.

- **`S144` (`20:23–20:30`, `7.0s`) `[Slide]`**: **What `/teach-code` Adds** — Compiler-grade verification gates around every lesson and listing.
  - **Visual / Artefact Callout**: Section divider header: `What /teach-code Adds: 3 Compiler-Grade Verification Gates`.
  - **Spoken (`bjm-voice`)**: What `/teach-code` adds—inspired by pragmatic engineering hygiene—is three deterministic compiler-like verification gates wrapped around every lesson.

- **`S145` (`20:30–20:38`, `8.0s`) `[v-click]`**: **Gate #1 (`build.py --check`)** — Validates rendered HTML, Mermaid diagrams, Prism code blocks, and structure in [`docs/egm/tutorial/build.py`](../../docs/egm/tutorial/build.py).
  - **Visual / Artefact Callout**: Gate 1 pill (`build.py --check`: Markdown directives, internal `.md` links, Mermaid syntax, quiz word counts).
  - **Spoken (`bjm-voice`)**: Gate one: lessons are authored in clean Markdown and compiled by `build.py --check`, checking links, quiz lengths, and HTML structure.

- **`S146` (`20:38–20:45`, `7.0s`) `[v-click]`**: **Gate #2 (Runnable `0001`–`0006` Apps)** — Every lesson ships a real C99 companion project (`docs/egm/tutorial/apps/0001-breadboard-circuit-and-current-budget/` through `0006-bargraphd-daemon-and-nsh-cli/`) passing `-std=c99 -Wall -Wextra -Werror -pedantic` + `make test`.
  - **Visual / Artefact Callout**: Gate 2 pill (`tutorial/apps/0001`–`0006`: `cc -std=c99 -Wall -Wextra -Werror -pedantic` + `make test`).
  - **Spoken (`bjm-voice`)**: Gate two: every lesson backs its prose with a runnable C99 project compiled with `-Wall -Wextra -Werror -pedantic` and `make test`.

- **`S147` (`20:45–20:53`, `8.0s`) `[v-click]`**: **Gate #3 (`sync_listing.py`)** — [`.agents/skills/teach-code/scripts/sync_listing.py`](../../.agents/skills/teach-code/scripts/sync_listing.py) guarantees byte-for-byte parity between lesson snippets and C files.
  - **Visual / Artefact Callout**: Gate 3 pill (`scripts/sync_listing.py --check`: byte-for-byte lesson listing <-> `src/main.c` parity).
  - **Spoken (`bjm-voice`)**: Gate three: `sync_listing.py --check` guarantees every full code listing inside a lesson matches its compiling `.c` file byte for byte.

- **`S148` (`20:53–21:00`, `7.0s`) `[Slide]`**: **Skill Evolution Summary** — Pedagogical soul from `/teach` + strict engineering hygiene from `/teach-code`.
  - **Visual / Artefact Callout**: Formula card: `/teach (Pedagogical Soul) + Strict Compiler Gates = /teach-code`.
  - **Spoken (`bjm-voice`)**: Keep the cognitive science, enforce compiler hygiene. Now let's run `/teach-code` against our verified firmware and walk through the course.

#### Beat 19 (`21:00–22:15`, `75.0s`, `7` steps) — Phase 6 (`06-teach-code`) & Browser Tour Stop 1: Course Landing + ZPD Learning Record

- **`S149` (`21:00–21:07.2`, `7.2s`) `[Slide]`**: **Phase 6 Prompt** — Pointing `/teach-code` at our `94` EGM files to weave a 6-lesson story-arc course in [`docs/egm/tutorial/`](../../docs/egm/tutorial/README.md).
  - **Visual / Artefact Callout**: Prompt card showing `/teach-code` ingesting [`docs/egm/`](../../docs/egm/README.md) contracts, Goldfish reports, and verified C into [`docs/egm/tutorial/`](../../docs/egm/tutorial/README.md).
  - **Spoken (`bjm-voice`)**: In Phase 6, I point `/teach-code` at our generated EGM contracts, Goldfish reviews, and firmware to build a six-lesson story arc.

- **`S150` (`21:07.2–21:37.0`, `29.8s`) `[Live-cast]`**: **LIVE CAST (`29.8s`)** on [`prototypes/06-teach-code-cast-player.html`](../../prototypes/06-teach-code-cast-player.html).
  - **Visual / Artefact Callout**: Launch [`prototypes/06-teach-code-cast-player.html`](../../prototypes/06-teach-code-cast-player.html). Jump to Stop Point `7. Prompt: /teach-code Story Arc in ./tutorial/` (`00:10` warped) and press `p` through **Seg 08** (`29.8s @ 79x`, compressing `39m 11s` raw -> auto-pauses at `8. 6-Lesson Course Generated`).
  - **Spoken (`bjm-voice`)**: Here is the prompt asking `/teach-code` to weave our entire hardware and RTOS story arc inside `./tutorial/`. Watch the pipeline effect at seventy-nine times speed across thirty-nine raw minutes: it calculates the Zone of Proximal Development first, builds six progressive C99 companion apps from Lesson `0001` through `0006`—running `-Wall -Wextra -Werror -pedantic` and fixing test failures on each chapter—writes six matching Tufte lessons, crafts three interactive JavaScript simulators plus four reference sheets, and runs `sync_listing.py --check` and `build.py --check` before we switch straight over to Chrome.

- **`S151` (`21:37.0–21:45.0`, `8.0s`) `[Slide]`**: **Browser Tour Stop 1 ([`docs/egm/tutorial/index.md`](../../docs/egm/tutorial/index.md) -> `docs/egm/tutorial/index.html`)** — Live course landing page with the 6-lesson Mermaid story-arc map.
  - **Visual / Artefact Callout**: Live Chrome view of compiled `docs/egm/tutorial/index.html` (authored in [`docs/egm/tutorial/index.md#L13-L20`](../../docs/egm/tutorial/index.md#L13-L20)), showing Figure `0.1` 6-lesson Mermaid story arc.
  - **Spoken (`bjm-voice`)**: Flip straight into Chrome on `docs/egm/tutorial/index.html`—compiled from [`index.md`](../../docs/egm/tutorial/index.md)—opening on a six-lesson Mermaid story-arc map from breadboard to daemon.

- **`S152` (`21:45.0–21:52.0`, `7.0s`) `[v-click]`**: **Course Index Widgets** — `{{widget:lesson-cards}}` (`6` cumulative chapters) + `{{widget:reference-cards}}` (`4` printable sheets).
  - **Visual / Artefact Callout**: Scroll focus on [`docs/egm/tutorial/index.md#L26-L32`](../../docs/egm/tutorial/index.md#L26-L32) rendering the `6` lesson cards (`0001`–`0006`) and `4` printable reference cards.
  - **Spoken (`bjm-voice`)**: Underneath sit auto-generated cards for all six hands-on chapters and four printable bench sheets ready for lookup beside real hardware.

- **`S153` (`21:52.0–22:00.0`, `8.0s`) `[Slide]`**: **Course Mission ([`docs/egm/tutorial/MISSION.md`](../../docs/egm/tutorial/MISSION.md))** — Defines prerequisites, scope, and concrete hardware/RTOS outcomes.
  - **Visual / Artefact Callout**: Excerpt view of [`docs/egm/tutorial/MISSION.md#L3-L22`](../../docs/egm/tutorial/MISSION.md#L3-L22) (`Why`, `Success looks like`, `Constraints`, `Out of scope`).
  - **Spoken (`bjm-voice`)**: [`MISSION.md`](../../docs/egm/tutorial/MISSION.md) grounds every page: zero submodule edits, strict C99 host testing, and safe three-point-three-volt current budgets before touching silicon.

- **`S154` (`22:00.0–22:07.0`, `7.0s`) `[Slide]`**: **ZPD Learning Record ([`docs/egm/tutorial/learning-records/0001-initial-architecture-and-driver-traps.md`](../../docs/egm/tutorial/learning-records/0001-initial-architecture-and-driver-traps.md))** — ADR explicitly modelling the learner's Zone of Proximal Development.
  - **Visual / Artefact Callout**: Full `7`-line ADR card displaying [`docs/egm/tutorial/learning-records/0001-initial-architecture-and-driver-traps.md#L1-L7`](../../docs/egm/tutorial/learning-records/0001-initial-architecture-and-driver-traps.md#L1-L7).
  - **Spoken (`bjm-voice`)**: Just like an engineering ADR, this learning record models the learner's Zone of Proximal Development so concepts stack one clean layer per chapter.

- **`S155` (`22:07.0–22:15.0`, `8.0s`) `[v-click]`**: **Stop 1 Payoff** — Instead of dumping `bargraph_main.c` on page one, Lesson 0001 starts with breadboard physics and ramps up step by step.
  - **Visual / Artefact Callout**: Progression badge (`0001 Physics -> 0002 Scaling -> 0003 Q4 EMA -> 0004 PWM -> 0005 IRQ/E9 -> 0006 bargraphd`).
  - **Spoken (`bjm-voice`)**: Instead of dumping `bargraph_main.c` on page one, Lesson 0001 starts with Ohm's Law. Now let's jump to Stop 2 inside Lesson 0003.

#### Beat 20 (`22:15–23:45`, `90.0s`, `11` steps) — Browser Tour Stop 2: Lesson 0003 `adc-ema-simulator` & Tufte Retrieval Practice

- **`S156` (`22:15–22:23`, `8.0s`) `[Slide]`**: **Browser Tour Stop 2 (Lesson 0003)** — [`docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md`](../../docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md) rendered in clean Tufte layout.
  - **Visual / Artefact Callout**: Lesson 0003 Section 1 header view ([`docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md#L18-L29`](../../docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md#L18-L29)): `19-Bit Left-Shift` + `Stale First Sample (msgs[0])`.
  - **Spoken (`bjm-voice`)**: Remember Pass 1's `4x` Q4 EMA gain bug (`B-2`) and Pass 2's stale sample (`CA-01`)? Lesson 0003 turns both straight into tactile understanding.

- **`S157` (`22:23–22:31`, `8.0s`) `[v-click]`**: **Tufte Sidenotes** — Right-margin notes carry hardware register context beside the C code without breaking reading flow.
  - **Visual / Artefact Callout**: Right-margin zoom on Tufte sidenote `#1` ([`0003-q4-ema-filtering-and-adc-extraction.md#L23`](../../docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md#L23)) explaining `int32_t am_data` bit `31` sign check.
  - **Spoken (`bjm-voice`)**: Tufte right-margin sidenotes sit directly beside the prose—explaining why checking `am_data >= 0` catches hardware FIFO errors without tab hopping.

- **`S158` (`22:31–22:39`, `8.0s`) `[Slide]`**: **Interactive Simulator (`{{widget:adc-ema-simulator}}`)** — Live JS sandbox inside [`docs/egm/tutorial/assets/course.js`](../../docs/egm/tutorial/assets/course.js).
  - **Visual / Artefact Callout**: Live Chrome view of `{{widget:adc-ema-simulator}}` ([`0003-q4-ema-filtering-and-adc-extraction.md#L42`](../../docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md#L42), implemented in [`docs/egm/tutorial/assets/course.js`](../../docs/egm/tutorial/assets/course.js)).
  - **Spoken (`bjm-voice`)**: Rather than passively reading fixed-point bit-shift equations, `course.js` embeds a live `adc-ema-simulator` widget right inside the lesson.

- **`S159` (`22:39–22:47`, `8.0s`) `[v-click]`**: **Live Toggle #1 (Discard `msgs[0]`)** — Visually demonstrates how the latched previous-burst head sample pollutes a sharp step input.
  - **Visual / Artefact Callout**: Simulator control highlight toggling `Discard stale msgs[0]` off vs. on across a potentiometer step edge.
  - **Spoken (`bjm-voice`)**: Flip off "Discard `msgs[0]`" on a step change, and you watch that twenty-millisecond-old hardware sample visibly drag the waveform behind reality.

- **`S160` (`22:47–22:55`, `8.0s`) `[v-click]`**: **Live Toggle #2 (`B-2`: `>> 2` vs. `(ema_q4 + 8) >> 4`)** — Shows the curve slamming to `100%` at quarter-turn pot travel vs. smooth unity gain.
  - **Visual / Artefact Callout**: Simulator control highlight contrasting buggy `ema_q4 >> 2` saturation against `(ema_q4 + 8) >> 4` unity gain (`B-2`).
  - **Spoken (`bjm-voice`)**: Switch `B-2` back to `>> 2`, and the curve pegs to `4095` at a quarter turn—showing why Goldfish `B-2` mattered immediately.

- **`S161` (`22:55–23:03`, `8.0s`) `[Slide]`**: **Fill-in-the-Blank Exercise (`:::exercise`)** — Interactive `keep-markup` Prism inputs right inside the C listing.
  - **Visual / Artefact Callout**: Interactive exercise view [`ex-0003` (`0003-q4-ema-filtering-and-adc-extraction.md#L399-L412`)](../../docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md#L399-L412) with inline Prism blanks (`BARGRAPH_ADC_SHIFT`, `2`, `8u`).
  - **Spoken (`bjm-voice`)**: Right underneath sits an inline `keep-markup` C exercise forcing you to type `BARGRAPH_ADC_SHIFT`, shift `2`, and rounding offset `8u` yourself.

- **`S162` (`23:03–23:11`, `8.0s`) `[Slide]`**: **Equal-Word-Count Quiz (`:::quiz`)** — 4 options, strictly **`10` words per option** testing why `msgs[0]` is discarded.
  - **Visual / Artefact Callout**: Quiz view [`quiz-0003` Q2 (`0003-q4-ema-filtering-and-adc-extraction.md#L434-L452`)](../../docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md#L434-L452) displaying options A–D (`10` words each).
  - **Spoken (`bjm-voice`)**: Then `build.py` checks that all four quiz options contain ten words apiece—eliminating longest-answer guessing cues when testing `msgs[0]` staleness.

- **`S163` (`23:11–23:19`, `8.0s`) `[v-click]`**: **Cognitive Science Payoff** — Forcing recall builds durable *Storage Strength* instead of passive skimming *Fluency*.
  - **Visual / Artefact Callout**: Pedagogical note pill: `Active Recall -> Storage Strength (Not Passive Skimming Fluency)`.
  - **Spoken (`bjm-voice`)**: Answering why sample zero is stale locks that hardware invariant into storage strength instead of letting your eyes glide past prose.

- **`S164` (`23:19–23:27`, `8.0s`) `[Slide]`**: **Simulator Flash #1 (Lesson 0001)** — `{{widget:breadboard-simulator}}` ([`docs/egm/tutorial/lessons/0001-breadboard-circuit-and-current-budget.md#L63`](../../docs/egm/tutorial/lessons/0001-breadboard-circuit-and-current-budget.md#L63)).
  - **Visual / Artefact Callout**: 8-second Chrome flash of `{{widget:breadboard-simulator}}` sweeping wiper voltage (`0..3300 mV`), Thevenin impedance `R_S`, and `23.6 mA` LED current.
  - **Spoken (`bjm-voice`)**: Lesson 0001 includes a live breadboard simulator too—letting you drag the wiper to verify Thevenin source impedance and LED budgets.

- **`S165` (`23:27–23:36`, `9.0s`) `[Slide]`**: **Simulator Flash #2 (Lesson 0002)** — `{{widget:pwm-segment-visualizer}}` ([`docs/egm/tutorial/lessons/0002-proportional-bargraph-math-and-pwm-scaling.md#L47`](../../docs/egm/tutorial/lessons/0002-proportional-bargraph-math-and-pwm-scaling.md#L47)).
  - **Visual / Artefact Callout**: 9-second Chrome flash of `{{widget:pwm-segment-visualizer}}` interpolating `0..393,210` units across `6` `0..65535` PWM channels.
  - **Spoken (`bjm-voice`)**: And Lesson 0002 models all six `0..65535` PWM channels live—making the `b16ONE` `65536`-to-zero truncation bug obvious at a glance.

- **`S166` (`23:36–23:45`, `9.0s`) `[v-click]`**: **Stop 2 Summary** — Three live browser simulators turn embedded math into tactile play.
  - **Visual / Artefact Callout**: Three-widget strip (`breadboard-simulator`, `pwm-segment-visualizer`, `adc-ema-simulator`) -> `Next: Lesson 0005`.
  - **Spoken (`bjm-voice`)**: Three interactive JavaScript simulators turn dry embedded arithmetic into tactile play. Now let's jump forward to Lesson 0005 for the hardware traps.

#### Beat 21 (`23:45–24:30`, `45.0s`, `6` steps) — Browser Tour Stop 3: Lesson 0005 (`START_ONCE` vs `RP23XX_ADC_DIV`) & Printable Bench Sheet

- **`S167` (`23:45–23:53`, `8.0s`) `[Slide]`**: **Browser Tour Stop 3 (Lesson 0005)** — [`docs/egm/tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md#L58-L69`](../../docs/egm/tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md#L58-L69).
  - **Visual / Artefact Callout**: Lesson 0005 browser view displaying Section 2 (`Conquering Traps #2, #6, and #7`).
  - **Spoken (`bjm-voice`)**: Lesson 0005 distils the entire Act II and Act III hardware investigation into one focused chapter—explicitly teaching our `RP23XX_ADC_DIV` false start.

- **`S168` (`23:53–24:00`, `7.0s`) `[v-click]`**: **Traps Taught Explicitly** — Contrasts why `RP23XX_ADC_DIV` fails under `START_ONCE`, teaches NVIC `RP23XX_ADC_IRQ_FIFO` gating, and walks through `RP2350-E9` pad isolation.
  - **Visual / Artefact Callout**: Highlighted prose callouts on [`0005-out-of-tree-bsp-and-adc-irq-gating.md#L64-L68`](../../docs/egm/tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md#L64-L68) (`RP2350-E9` `modreg32` + `START_ONCE` vs `ADC_DIV` + NVIC IRQ gating).
  - **Spoken (`bjm-voice`)**: A great tutorial doesn't hide dead ends—it teaches why `ADC_DIV` ignores `START_ONCE`, so you never repeat that assumption elsewhere.

- **`S169` (`24:00–24:08`, `8.0s`) `[Slide]`**: **Printable Bench Reference Sheet** — [`docs/egm/tutorial/reference/nuttx-rp2350-driver-traps.md#L15-L25`](../../docs/egm/tutorial/reference/nuttx-rp2350-driver-traps.md#L15-L25).
  - **Visual / Artefact Callout**: Table view of [`docs/egm/tutorial/reference/nuttx-rp2350-driver-traps.md#L15-L25`](../../docs/egm/tutorial/reference/nuttx-rp2350-driver-traps.md#L15-L25) condensing all `7` upstream RP23xx/NuttX traps and mitigations.
  - **Spoken (`bjm-voice`)**: Beside the six lessons sit four condensed reference cards, including this one-page lookup table of all seven NuttX and RP2350 traps.

- **`S170` (`24:08–24:15`, `7.0s`) `[v-click]`**: **`@media print` Override** — Light-mode print CSS in [`docs/egm/tutorial/assets/style.css#L1031-L1032`](../../docs/egm/tutorial/assets/style.css#L1031-L1032) so dark-mode pages print crisp on A4 paper.
  - **Visual / Artefact Callout**: Chrome Print Preview toggle triggered by `@layer print { @media print ... }` in [`docs/egm/tutorial/assets/style.css#L1031-L1032`](../../docs/egm/tutorial/assets/style.css#L1031-L1032).
  - **Spoken (`bjm-voice`)**: Hit print, and `@media print` strips dark backgrounds into crisp A4 ink—ready to tape right above your soldering iron.

- **`S171` (`24:15–24:23`, `8.0s`) `[Slide]`**: **Knowledge Loop Completed** — Agent contracts (`3,241`) -> Goldfish audit -> Human codelab + bench cheat sheet.
  - **Visual / Artefact Callout**: Loop summary card: `3,241 Contracts -> 2-Pass Goldfish Audit -> 6-Lesson Codelab + Bench Sheet`.
  - **Spoken (`bjm-voice`)**: That closes the knowledge loop between machine and human: what the Goldfish discovered in markdown now lives inside your own head.

- **`S172` (`24:23–24:30`, `7.0s`) `[v-click]`**: **Surprise Twist Cue** — And forcing Lesson 0006 to compile under strict C99 caught two real firmware bugs we missed in Phase 5!
  - **Visual / Artefact Callout**: Twist banner: `Surprise bonus: Strict C99 Lesson 0006 lab caught 2 Phase 5 bugs!`.
  - **Spoken (`bjm-voice`)**: Better yet, forcing Lesson 0006 to compile real C99 under `-Werror -pedantic` actually caught two bugs inside our Phase 5 code!

#### Beat 22 (`24:30–25:00`, `30.0s`, `4` steps) — Browser Tour Stop 4: Self-Corrections #1 & #2 — How Runnable C99 Labs Caught Two Phase 5 Bugs

- **`S173` (`24:30–24:38`, `8.0s`) `[Slide]`**: **Browser Tour Stop 4 (Lesson 0006 Lab)** — [`docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L798`](../../docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L798).
  - **Visual / Artefact Callout**: File header card for [`docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c`](../../docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c) running host POSIX threads under `-std=c99 -Wall -Wextra -Werror -pedantic`.
  - **Spoken (`bjm-voice`)**: Lesson 0006 compiles the full `bargraphd` state machine on macOS pthreads under strict C99—turning teaching material into an uncompromising integration harness.

- **`S174` (`24:38–24:45`, `7.0s`) `[v-click]`**: **Self-Correction #1 (`Makefile` `printf` Duplication)** — Replaced Phase 5's [`docs/egm/xiao-rp2350-bargraph/Makefile#L40-L75`](../../docs/egm/xiao-rp2350-bargraph/Makefile#L40-L75) `/tmp/bargraph_test.XXXXXX` `printf` synthesis with genuine single-source C99 compilation.
  - **Visual / Artefact Callout**: Side-by-side badge contrasting [`docs/egm/xiao-rp2350-bargraph/Makefile#L40-L75`](../../docs/egm/xiao-rp2350-bargraph/Makefile#L40-L75) (`printf '%s\n' ... > /tmp/bargraph_test.c`) vs. `tutorial/apps/0001..0006/Makefile` single-source `src/main.c` compilation.
  - **Spoken (`bjm-voice`)**: Remember Vanilla's clean `bargraph_logic.c`? In Phase 5, EGM `Makefile` synthesised `/tmp/bargraph_test.c` with `printf`—and `/teach-code` forced genuine single-source compilation.

- **`S175` (`24:45–24:53`, `8.0s`) `[Slide]`**: **Self-Correction #2 (`bargraph test` `MANUAL`-Clobber Bug)** — Excerpt from trace [`docs/prompts-egm/24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md#L61-L65`](../../docs/prompts-egm/24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md#L61-L65).
  - **Visual / Artefact Callout**: Trace snippet from [`24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md#L61-L64`](../../docs/prompts-egm/24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md#L61-L64) (`Step 133/135` tick-sync wait + `Step 144/146` `MANUAL`-clobber assertion catch).
  - **Spoken (`bjm-voice`)**: Worse, exercising `bargraphd` threads inside trace `24-0cc3a75f` hit two surprises: a startup tick-sync `pthread_cond_wait` block and a `MANUAL`-mode clobber bug.

- **`S176` (`24:53–25:00`, `7.0s`) `[Code-step]`**: [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L557-L565, L841-L854`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L841-L854) paired with [`docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L749`](../../docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L749).
  - **Visual / Artefact Callout**: Shiki condensed excerpt highlighting `bargraph_start_daemon()` resetting `MANUAL` to `SENSING` before `prev_mode = g_bargraph.mode` plus `bargraph_wait_next_tick_locked()` (lines `1–2, 5, 7`):
```c
/* Phase 5 bargraph_main.c#L843 -> #L561 resets MANUAL before #L853! */
int ret = bargraph_start_daemon(); /* forces mode = BARGRAPH_MODE_SENSING */
if (g_bargraph.mode != BARGRAPH_MODE_TEST)
  g_bargraph.prev_mode = g_bargraph.mode; /* captures SENSING, not MANUAL! */
/* Lesson 0006 main.c#L743-L749 fixes tick-sync wait predicate: */
static void bargraph_wait_next_tick_locked(uint32_t prev_seq)
{ while (g_bargraph.running && g_bargraph.tick_seq == prev_seq)
    pthread_cond_wait(&g_bargraph.done_cond, &g_bargraph.lock); }
```
  - **Spoken (`bjm-voice`)**: `bargraph test` called `bargraph_start_daemon()`, which reset `MANUAL` to `SENSING` before saving `prev_mode`—proving human docs catch bugs too.

---

### Act V — The Honest Retro & Telemetry Scorecard (`25:00–30:00`, `5.0 min`, Beats `23–26`, `S177–S214`)

#### Act V.a — Head-to-Head Telemetry & Shared Blind Spots (`25:00–28:30`, `3.5 min`, Beats `23–25`, `S177–S203`)

##### Beat 23 (`25:00–26:15`, `75.0s`, `10` steps) — Head-to-Head Telemetry & Dual-Tier Scorecard ([`docs/reports/vanilla-vs-egm-comparison.md`](../../docs/reports/vanilla-vs-egm-comparison.md))

- **`S177` (`25:00–25:08`, `8.0s`) `[Slide]`**: **Act V Divider** — **The Honest Retro: Telemetry, Blind Spots & Skill Evolution**.
  - **Visual / Artefact Callout**: Act V section header (`Act V — Honest Retro, Telemetry & Skill Evolution`), citing [`docs/reports/vanilla-vs-egm-comparison.md`](../../docs/reports/vanilla-vs-egm-comparison.md).
  - **Spoken (`bjm-voice`)**: Now let's put both runs side by side—telemetry, strict post-audit catches, and the physical blunder neither workflow spotted.

- **`S178` (`25:08–25:15`, `7.0s`) `[Slide]`**: **Telemetry Row 1 (Raw Time)** — Vanilla **`46m 40s`** vs. EGM **`154m 37s`** Phases 1–5 firmware (`196m 37s` total incl. `/teach-code`).
  - **Visual / Artefact Callout**: Single-row comparison table (Row 1 from [`docs/reports/vanilla-vs-egm-comparison.md#L34`](../../docs/reports/vanilla-vs-egm-comparison.md#L34)): `Raw Recording Duration` — `46m 40.0s` vs. `154m 36.6s` (Phases 1–5) / `196m 37.1s` (Phases 1–6).
  - **Spoken (`bjm-voice`)**: Let's be completely upfront about wall-clock cost: Vanilla took forty-six minutes forty seconds; EGM firmware took one hundred fifty-four minutes.

- **`S179` (`25:15–25:23`, `8.0s`) `[v-click]`**: **Telemetry Row 2 (Sessions & Subagents)** — **`1` parent (`0` subagents)** vs. **`8` parents + `17` Goldfish subagents** (`25` traces).
  - **Visual / Artefact Callout**: Reveals Row 2 (`Parent + Subagent Sessions` from [`docs/reports/vanilla-vs-egm-comparison.md#L36`](../../docs/reports/vanilla-vs-egm-comparison.md#L36)): `1 + 0` vs. `8 + 17` (`25` exported traces; `6 + 17` across Phases 1–5).
  - **Spoken (`bjm-voice`)**: That reflects the architecture: one monolithic vibe thread versus eight parent sessions orchestrating seventeen zero-context Goldfish subagents across twenty-five traces.

- **`S180` (`25:23–25:30`, `7.0s`) `[v-click]`**: **Telemetry Row 3 (`README.md` Contract Reads)** — **`0` `nuttx`/`apps` contract reads** vs. **`209` `README.md` reads** (`1,005` `read_file` calls).
  - **Visual / Artefact Callout**: Reveals Row 3 (`README.md Contract Reads` from [`docs/reports/vanilla-vs-egm-comparison.md#L38`](../../docs/reports/vanilla-vs-egm-comparison.md#L38)): `0` vs. `209` `README.md` reads (`66` vs. `1,005` `read_file` calls).
  - **Spoken (`bjm-voice`)**: Vanilla read zero `nuttx/` or `apps/` contracts across sixty-six peeks; EGM checked two hundred and nine directory contracts deliberately.

- **`S181` (`25:30–25:38`, `8.0s`) `[v-click]`**: **Telemetry Row 4 (Build Failures Before `.uf2`)** — **`1` Kconfig drop + `1` `xmllint` error** vs. **`0` firmware build failures** (`347K` `.uf2` first try).
  - **Visual / Artefact Callout**: Reveals Row 4 (`Build Failures Before Clean .uf2` from [`docs/reports/vanilla-vs-egm-comparison.md#L39`](../../docs/reports/vanilla-vs-egm-comparison.md#L39)): `1 Kconfig drop + 1 xmllint error` vs. `0 firmware build failures` (`1` BSD `mktemp` fix).
  - **Spoken (`bjm-voice`)**: And upfront verification paid off at compile time: zero firmware build errors and a clean `347K` `.uf2` on attempt one.

- **`S182` (`25:38–25:45`, `7.0s`) `[Slide]`**: **Dual-Tier Scorecard (Primary)** — Vanilla **`5 / 10`** vs. EGM **`10 / 10` Primary Pass**.
  - **Visual / Artefact Callout**: Single-row primary scorecard from [`docs/reports/vanilla-vs-egm-comparison.md#L22`](../../docs/reports/vanilla-vs-egm-comparison.md#L22): `Primary 10-Point Trap Scorecard: 5 / 10 Pass vs. 10 / 10 Primary Pass`.
  - **Spoken (`bjm-voice`)**: Across the ten hardware and RTOS traps we tested, Vanilla scored five out of ten while EGM swept ten out of ten.

- **`S183` (`25:45–25:53`, `8.0s`) `[v-click]`**: **Dual-Tier Scorecard (Strict Post-Audit)** — Vanilla **`4 Pass / 4 Partial / 2 Fail`** vs. EGM **`6 Pass / 4 Partial / 0 Fail`**.
  - **Visual / Artefact Callout**: Second-row strict scorecard from [`docs/reports/vanilla-vs-egm-comparison.md#L23`](../../docs/reports/vanilla-vs-egm-comparison.md#L23): `Strict Post-Audit: 4 Pass / 4 Partial / 2 Fail vs. 6 Pass / 4 Partial / 0 Fail`.
  - **Spoken (`bjm-voice`)**: Then we turned fresh read-only Goldfish loose after Phase 5—and under strict grading, EGM scored six Passes, four Partials, zero Fails.

- **`S184` (`25:53–26:00`, `7.0s`) `[Slide]`**: **The Time Tradeoff** — `~3.3x` longer up front (`155m` vs. `47m`) for firmware alone.
  - **Visual / Artefact Callout**: Time multiplier callout: `Firmware Wall-Clock Ratio: 154m 37s / 46m 40s ≈ 3.3x` (`+108 min` upfront).
  - **Spoken (`bjm-voice`)**: Would I spin up sixteen Goldfish for a one-line CSS tweak? Definitely not. Three-point-three times overhead belongs on unfamiliar legacy complexity.

- **`S185` (`26:00–26:08`, `8.0s`) `[v-click]`**: **What Those Extra `108` Minutes Bought** — Zero high-severity hardware fails, first-try `.uf2`, and a 6-lesson interactive codelab.
  - **Visual / Artefact Callout**: Return-on-investment chips: `0 High-Severity Silicon/ISR Fails | 1st-Attempt .uf2 | 6-Lesson Interactive Codelab`.
  - **Spoken (`bjm-voice`)**: When thirty years of RTOS C meets real silicon errata, spending two calm hours upfront saves weekends chasing ISR panics.

- **`S186` (`26:08–26:15`, `7.0s`) `[v-click]`**: **Now About Those `4` Strict Partials...** — Let's zoom straight into what the read-only Goldfish audit caught on our `10 / 10` firmware.
  - **Visual / Artefact Callout**: Focus highlight on EGM's `4 Partial` badge (`#6, #7, #8, #10` in [`docs/reports/vanilla-vs-egm-comparison.md#L23, L68-L72`](../../docs/reports/vanilla-vs-egm-comparison.md#L23)).
  - **Spoken (`bjm-voice`)**: Now about those four strict partial marks on our own ten-out-of-ten firmware—let's inspect every single one of them candidly.

##### Beat 24 (`26:15–27:30`, `75.0s`, `9` steps) — Self-Correction #3: What the Strict Post-Audit Caught on EGM (`6 Pass / 4 Partial / 0 Fail`)

- **`S187` (`26:15–26:24`, `9.0s`) `[Slide]`**: **Post-Experiment Hostile Audit** — Fresh read-only Goldfish (`egm-codebase-details` + `egm-design`) graded the archived Phase 5 firmware cold.
  - **Visual / Artefact Callout**: Audit scope badge citing [`docs/reports/vanilla-vs-egm-comparison.md#L3-L5`](../../docs/reports/vanilla-vs-egm-comparison.md#L3-L5): `Independent Read-Only Goldfish Audit of Archived Phase 5 C/Makefile/defconfig`.
  - **Spoken (`bjm-voice`)**: We did not cherry-pick a kind reviewer; we launched fresh read-only Goldfish auditors against the archived Phase 5 C files cold.

- **`S188` (`26:24–26:32`, `8.0s`) `[Slide]`**: **Self-Correction #3 (Invariant `#6`)** — [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L389-L390, L529-L530`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L529-L530).
  - **Visual / Artefact Callout**: Invariant `#6` callout card (`Strict: Partial`) pointing at exit and rollback paths in [`bargraph_main.c#L389-L390, L529-L530`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L529-L530).
  - **Spoken (`bjm-voice`)**: Look at Self-Correction #3 under Invariant 6: remember how EGM kept `/dev/adc0` persistently open to avoid Vanilla's `40 Hz` shutdown race?

- **`S189` (`26:32–26:40`, `8.0s`) `[Code-step]`**: [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L388-L390, L527-L530`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L527-L530) — Highlights `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` called **one line *before* `close(adc_fd)`** on exit and startup rollback!
  - **Visual / Artefact Callout**: Shiki excerpt (`#L388-L390` & `#L527-L530`, highlight lines `2, 5`):
```c
/* Startup failure rollback (#L388-L390): */
up_enable_irq(RP23XX_ADC_IRQ_FIFO); /* Unmasks 1 line BEFORE close! */
(void)close(adc_fd);
/* Daemon shutdown on bargraph stop (#L527-L530): */
up_enable_irq(RP23XX_ADC_IRQ_FIFO); /* Starts 2 us conversion early! */
(void)close(adc_fd);
```
  - **Spoken (`bjm-voice`)**: On exit and startup rollback, EGM unmasked `up_enable_irq()` one line *before* `close(adc_fd)`—briefly starting another conversion right before `remove_device()` runs!

- **`S190` (`26:40–26:49`, `9.0s`) `[Code-step]`**: Fix paired with [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L176-L196`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L176-L196) — Call `(void)close(adc_fd)` *first* while masked, then `up_enable_irq()`.
  - **Visual / Artefact Callout**: Shiki diff swapping those two lines (`#L529-L530`, highlight lines `4–5`) while preserving `g_active_count == 0` teardown:
```diff
-  up_enable_irq(RP23XX_ADC_IRQ_FIFO);
   (void)close(adc_fd);
+  up_enable_irq(RP23XX_ADC_IRQ_FIFO); /* Now safe: g_active_count == 0 */
+/* In rp23xx_adc.c#L176-L196: pending IRQ runs clean ADC shutdown */
```
  - **Spoken (`bjm-voice`)**: Swapping those two lines closes `adc_fd` while masked, then unmasks the pending IRQ so `g_active_count == 0` shuts down cleanly.

- **`S191` (`26:49–26:57`, `8.0s`) `[Slide]`**: **Strict Partial #2 (`#7` Kconfig Hygiene)** — Inherited `EXAMPLES_HELLO`, `TESTING_GETPRIME`, and `TESTING_OSTEST` left enabled in `defconfig` + `CONFIG_RAMLOG_SYSLOG=y` omitted.
  - **Visual / Artefact Callout**: Strict Partial badge `#7` citing [`docs/reports/vanilla-vs-egm-comparison.md#L69, L119`](../../docs/reports/vanilla-vs-egm-comparison.md#L69) and `docs/egm/xiao-rp2350-bargraph/board/configs/bargraph/defconfig#L39`.
  - **Spoken (`bjm-voice`)**: Strict Partial 2 on Kconfig hygiene: inherited `EXAMPLES_HELLO` and `OSTEST` stayed compiled into `defconfig`, and `CONFIG_RAMLOG_SYSLOG=y` wasn't enabled for `dmesg`.

- **`S192` (`26:57–27:05`, `8.0s`) `[v-click]`**: **Strict Partial #3 (`#8` Daemon Lifecycle)** — Phase 5 `bargraph test` `MANUAL` reset (fixed in Lesson 0006), `CLOCK_REALTIME` vs. `date -s`, and missing `SIGTERM`.
  - **Visual / Artefact Callout**: Strict Partial badge `#8` citing [`docs/reports/vanilla-vs-egm-comparison.md#L70, L125-L128`](../../docs/reports/vanilla-vs-egm-comparison.md#L70).
  - **Spoken (`bjm-voice`)**: Strict Partial 3 flagged Phase 5's `bargraph test` `MANUAL`-reset bug, `CLOCK_REALTIME` sleep sensitivity if `date -s` runs, and no `SIGTERM` handler.

- **`S193` (`27:05–27:14`, `9.0s`) `[v-click]`**: **Strict Partial #4 (`#10` Test Duplication)** — Phase 5 `Makefile` `printf` `/tmp` duplication (fixed in Lesson 0006) + blocking `read()` without `poll()`.
  - **Visual / Artefact Callout**: Strict Partial badge `#10` citing [`docs/reports/vanilla-vs-egm-comparison.md#L72, L113, L136`](../../docs/reports/vanilla-vs-egm-comparison.md#L72) and [`docs/egm/xiao-rp2350-bargraph/Makefile#L40-L114`](../../docs/egm/xiao-rp2350-bargraph/Makefile#L40-L114).
  - **Spoken (`bjm-voice`)**: And Strict Partial 4 marked Phase 5's `Makefile` `printf` test duplication—the shortcut we resolved directly inside Lesson 0006's C99 harness.

- **`S194` (`27:14–27:22`, `8.0s`) `[Slide]`**: **Honest Engineering Takeaway** — No workflow is magic: a cold audit caught a 1-line shutdown ordering bug inside `10 / 10` primary-pass code.
  - **Visual / Artefact Callout**: Candid truth banner: `No silver bullet: cold read-only audit still caught edge cases in 10/10 code`.
  - **Spoken (`bjm-voice`)**: No workflow is magic. Even after two Goldfish passes, turning auditors onto shipping C surfaced real one-line edge cases immediately.

- **`S195` (`27:22–27:30`, `8.0s`) `[v-click]`**: **Blind-Spot Transition** — Remember that XIAO breadboard SVG from Act III.d? Let's hold a ruler up to both diagrams.
  - **Visual / Artefact Callout**: Ruler transition card pointing to [`docs/vanilla/breadboard_circuit.svg`](../../docs/vanilla/breadboard_circuit.svg) vs. [`docs/egm/breadboard-wiring.svg`](../../docs/egm/breadboard-wiring.svg).
  - **Spoken (`bjm-voice`)**: Now for Self-Correction #4—remember that XIAO RP2350 seated in `Cols E & F` earlier? Let's hold a physical ruler up.

##### Beat 25 (`27:30–28:30`, `60.0s`, `8` steps) — Self-Correction #4: The Shared `0.6"` XIAO Breadboard Straddle Blind Spot

- **`S196` (`27:30–27:38`, `8.0s`) `[Slide]`**: **Self-Correction #4 Setup** — Physical hardware check: how wide is a Seeed Studio XIAO RP2350 across its two header rows?
  - **Visual / Artefact Callout**: Physical dimension prompt card: `Seeed Studio XIAO RP2350 header spacing = ?`.
  - **Spoken (`bjm-voice`)**: Simple mechanical bench question before anyone pushes pins into plastic: how wide is a Seeed Studio XIAO across its two header rows?

- **`S197` (`27:38–27:45`, `7.0s`) `[v-click]`**: **Physical Ground Truth** — **`0.6"` (`15.24 mm`, 6 breadboard holes)** straddling a standard `0.3"` centre ravine (`Cols D & H` or `C & G`).
  - **Visual / Artefact Callout**: Physical caliper spec from [`docs/reports/vanilla-vs-egm-comparison.md#L147`](../../docs/reports/vanilla-vs-egm-comparison.md#L147): `0.6" (15.24 mm) = Cols D & H or C & G across 0.3" trench`.
  - **Spoken (`bjm-voice`)**: Zero-point-six inches—fifteen point two-four millimetres. Across a standard zero-point-three-inch centre ravine, that lands in Columns `D` & `H` or `C` & `G`.

- **`S198` (`27:45–27:53`, `8.0s`) `[Slide]`**: **Vanilla's SVG ([`docs/vanilla/breadboard_circuit.svg#L67-L74`](../../docs/vanilla/breadboard_circuit.svg#L67-L74))** — Seats the XIAO horizontally across `c1..h1`.
  - **Visual / Artefact Callout**: Zoomed `viewBox="120 180 320 320"` crop of [`docs/vanilla/breadboard_circuit.svg#L67-L74, L225-L250`](../../docs/vanilla/breadboard_circuit.svg#L225-L250) showing North pins `c1..c7` (`y=260`) and South pins `h1..h7` (`y=420`).
  - **Spoken (`bjm-voice`)**: First let's check Vanilla's SVG diagram: it seated the XIAO horizontally spanning from row `c` down across the trench to `h`.

- **`S199` (`27:53–28:00`, `7.0s`) `[v-click]`**: **Vanilla Error (`0.7"`)** — Across the `0.3"` ravine, `c` to `h` spans **`0.7"`** (`1` row too wide—your pins won't drop into the holes!).
  - **Visual / Artefact Callout**: Dimension callout overlay: `c -> h = 0.2" + 0.3" + 0.2" = 0.7" (0.1" too wide — won't fit!)`.
  - **Spoken (`bjm-voice`)**: Measure `c` to `h` on a real breadboard: zero-point-seven inches. One row too wide—the header pins literally won't enter the holes.

- **`S200` (`28:00–28:08`, `8.0s`) `[Slide]`**: **EGM's SVG ([`docs/egm/breadboard-wiring.svg#L121, L280-L333`](../../docs/egm/breadboard-wiring.svg#L121))** — Seats the XIAO vertically in **`Cols E & F`**.
  - **Visual / Artefact Callout**: Zoomed `viewBox="150 130 500 500"` crop of [`docs/egm/breadboard-wiring.svg#L121, L280-L333`](../../docs/egm/breadboard-wiring.svg#L280-L333) showing `Col E` (`x=340`) and `Col F` (`x=460`).
  - **Spoken (`bjm-voice`)**: Surely ten-out-of-ten EGM nailed the breadboard geometry? Zoom in on EGM's SVG: it placed the XIAO vertically in Columns `E` and `F`.

- **`S201` (`28:08–28:15`, `7.0s`) `[v-click]`**: **EGM Error (`0.3"`!)** — `Cols E & F` span **`0.3"`** (half-width!), visually disguised in SVG by stretching the centre trench from `35px` to `120px`!
  - **Visual / Artefact Callout**: Red ruler annotation over `viewBox="150 130 500 500"`: `Col E..F = 0.3" (half-width!) — SVG stretched trench 120px vs 35px pitch`.
  - **Spoken (`bjm-voice`)**: `E` to `F` spans zero-point-three inches—half the XIAO's width! Worse, EGM stretched the SVG trench to 120 pixels so it *looked* right.

- **`S202` (`28:15–28:23`, `8.0s`) `[Slide]`**: **The Law of Contracts** — Where we had `README.md` contracts (NuttX + RP2350 silicon), EGM crushed Vanilla (`10/10` vs. `5/10`).
  - **Visual / Artefact Callout**: Boundary split diagram: `Inside Contracts (NuttX + RP2350 Silicon): EGM 10/10 vs Vanilla 5/10`.
  - **Spoken (`bjm-voice`)**: And there is the Law of Contracts: inside our NuttX and silicon contract boundary, EGM crushed Vanilla ten to five.

- **`S203` (`28:23–28:30`, `7.0s`) `[v-click]`**: **The Blind-Spot Corollary** — Where *neither* run had a physical breadboard pitch contract, both hallucinated (`0.7"` and `0.3"` vs. `0.6"`).
  - **Visual / Artefact Callout**: Corollary bar + Act V.b pivot: `Outside Contracts (Breadboard Pitch): Both Hallucinated (0.7" / 0.3" vs 0.6")`.
  - **Spoken (`bjm-voice`)**: Where neither run had a breadboard pitch contract, both hallucinated. Now here is how you turn production warts into `SKILL.md` gates.

#### Act V.b — Takeaway: Evolving Your Own `SKILL.md` Workflow (`28:30–30:00`, `1.5 min`, Beat `26`, `S204–S214`)

##### Beat 26 (`28:30–30:00`, `90.0s`, `11` steps) — Closing the Loop: Evolving Your Own `SKILL.md` Workflow (`25-98ffad8f` -> `build.py` -> `teach-code/SKILL.md`)

- **`S204` (`28:30–28:38`, `8.0s`) `[Slide]`**: **Layer 3 Payoff** — **Closing the *"Roll Your Own Skills"* Loop** (post-recording trace [`docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md`](../../docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md)).
  - **Visual / Artefact Callout**: Loop header card linking trace [`docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md`](../../docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md) (`0:06:45`, `54` steps).
  - **Spoken (`bjm-voice`)**: Remember earlier I said Layer 3 is rolling your own skills? Even after Phase 6 finished recording, Chrome showed two tiny warts.

- **`S205` (`28:38–28:46`, `8.0s`) `[Code-step]`**: [`docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md#L21-L22`](../../docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md#L21-L22) — Highlights Turn 1 inspecting the compiled codelab HTML.
  - **Visual / Artefact Callout**: Shiki wrapped quote excerpt (`#L21-L22`, highlight lines `2–4`):
```markdown
### Turn 1 (Step 0)
> The html in @[tutorial] contains unrendered TeX markup, and
> there is an odd bleed through in the form of a raw xml'ish
> markup in the HTML like `<2 id="breadboard-topology">` that appears in multiple lessons.
```
  - **Spoken (`bjm-voice`)**: Opening the compiled tutorial pages in Chrome revealed two things `build.py` missed: raw dollar-sign TeX markup and `<2 id="breadboard-topology">` heading tags.

- **`S206` (`28:46–28:54`, `8.0s`) `[Code-step]`**: [`docs/egm/tutorial/build.py#L22-L25, L736-L737`](../../docs/egm/tutorial/build.py#L736-L737) — Highlights `HEADING_ID_RE` dropping `h` in `<h\2>` + `$R_S$` raw TeX.
  - **Visual / Artefact Callout**: Shiki diff (`#L22-L25, L736-L737`, highlight line `4`):
```python
HEADING_ID_RE = re.compile(r"(<h([1-6])>)((?:(?!</h\2>).)*?)\s*\{#([a-zA-Z0-9_-]+)\}\s*(</h\2>)")
# Before: HEADING_ID_RE.sub(r'<\2 id="\4">\3\5', rendered) -> emitted <2 id="...">!
# After (#L737):
rendered = HEADING_ID_RE.sub(r'<h\2 id="\4">\3\5', rendered)
# Plus Lesson 0001..0003 raw TeX like $R_S$ -> Unicode / inline code
```
  - **Spoken (`bjm-voice`)**: One missing letter `h` inside the regex replacement emitted `<2 id="...">` tags, while `$R_S$` slipped through without a math renderer.

- **`S207` (`28:54–29:02`, `8.0s`) `[Code-step]`**: [`docs/egm/tutorial/build.py#L37-L38, L761-L770`](../../docs/egm/tutorial/build.py#L761-L770) — Highlights permanent validators `MALFORMED_TAG_RE` + `UNRENDERED_TEX_RE`.
  - **Visual / Artefact Callout**: Shiki excerpt (`#L37-L38, L762-L770`, highlight lines `1–2, 6–7`):
```python
MALFORMED_TAG_RE = re.compile(r"<[0-9]\b|\{#[a-zA-Z0-9_-]+\}")
UNRENDERED_TEX_RE = re.compile(r"\$[^$\n]+\$|\\(?:approx|pm|sim|Omega|text|le|ge|in)\b")
prose_only = STRIP_CODE_AND_PRE_RE.sub("", rendered)
for bad_tag in MALFORMED_TAG_RE.finditer(prose_only):
    errors.append(f"{page.rel_src}: malformed HTML tag ...")
for bad_tex in UNRENDERED_TEX_RE.finditer(prose_only):
    errors.append(f"{page.rel_src}: unrendered TeX markup ...")
```
  - **Spoken (`bjm-voice`)**: Step one: never just patch the symptom—turn both catches into `MALFORMED_TAG_RE` and `UNRENDERED_TEX_RE` compiler gates inside `build.py --check`.

- **`S208` (`29:02–29:10`, `8.0s`) `[Code-step]`**: [`.agents/skills/teach-code/SKILL.md#L93`](../../.agents/skills/teach-code/SKILL.md#L93) — Highlights upstreaming that rule back into `SKILL.md` via `/agy-customizations`.
  - **Visual / Artefact Callout**: Shiki wrapped markdown excerpt (`#L93`, highlight lines `2–5`):
```markdown
- **Heading Anchor Suffixes (`{#id}`) & Clean Math/Symbol Formatting**:
  converting `## Heading {#my-id}` to `<h2 id="my-id">Heading</h2>` — always
  preserve the leading `h` in `<h\2 id="\4">` so malformed `<2 id="...">` tags
  never appear! ... enforce in `build.py --check` that neither raw `{#...}`,
  malformed `<[1-6]`, nor unrendered TeX math (`$...$`) leak into rendered HTML.
```
  - **Spoken (`bjm-voice`)**: Step two: use `/agy-customizations` to fold that exact regex and Unicode invariant upstream into [`.agents/skills/teach-code/SKILL.md`](../../.agents/skills/teach-code/SKILL.md) line 93.

- **`S209` (`29:10–29:18`, `8.0s`) `[Slide]`**: **The Skill Flywheel** — Every production wart becomes a deterministic gate for your next codebase.
  - **Visual / Artefact Callout**: Circular skill-flywheel diagram: `Real Project Bug -> Build/Linter Gate -> SKILL.md Rule -> Cleaner Next Run`.
  - **Spoken (`bjm-voice`)**: Treat your `SKILL.md` files like living engineering craft rather than static downloads—you never pay the same tuition twice.

- **`S210` (`29:18–29:26`, `8.0s`) `[Slide]`**: **Closing Recap #1 — Roll Docs for the Agent** — Build bottom-up `README.md` contracts + Dave Rensin's Elephant & Goldfish workflow before writing C.
  - **Visual / Artefact Callout**: Recap Pillar `1`: `Roll Docs for the Agent -> 3,241 Contracts + Elephant & Goldfish before C`.
  - **Spoken (`bjm-voice`)**: Three habits to try on Monday morning. First: roll directory contracts and run Dave Rensin's Elephant and Goldfish protocol before coding.

- **`S211` (`29:26–29:34`, `8.0s`) `[v-click]`**: **Closing Recap #2 — Roll Docs for the Human** — Turn verified specs into runnable `/teach-code` codelabs because *you* own the outcome.
  - **Visual / Artefact Callout**: Recap Pillar `2`: `Roll Docs for the Human -> Interactive /teach-code Codelab + Strict Labs`.
  - **Spoken (`bjm-voice`)**: Second: roll interactive `/teach-code` codelabs from verified specs so your team—not only the repository—holds the architecture.

- **`S212` (`29:34–29:42`, `8.0s`) `[v-click]`**: **Closing Recap #3 — Roll Your Own Skills** — Start from sharp community skills like Matt Pocock's `/teach` and gently refine them with `/agy-customizations`.
  - **Visual / Artefact Callout**: Recap Pillar `3`: `Roll Your Own Skills -> Matt Pocock's /teach + /agy-customizations -> /teach-code`.
  - **Spoken (`bjm-voice`)**: Third: start from generous community craft like Matt Pocock's `/teach` and sharpen your own `SKILL.md` files every time a project bites.

- **`S213` (`29:42–29:51`, `9.0s`) `[Slide]`**: **Closing Mantra** — *"Design with an elephant. Review with a goldfish. Own the outcome—and keep sharpening the skills."*
  - **Visual / Artefact Callout**: Full-screen final mantra card.
  - **Spoken (`bjm-voice`)**: Design with an elephant. Review with a goldfish. Own the outcome—and keep sharpening the skills that guide them both.

- **`S214` (`29:51–30:00`, `9.0s`) `[Slide]`**: **Links & Resources** — `github.com/google/elephant-goldfish-model`, Dave Rensin's Medium essay, Matt Pocock's `/teach`, and the `nuttxspace` repo + [`docs/egm/tutorial/README.md`](../../docs/egm/tutorial/README.md).
  - **Visual / Artefact Callout**: Clean resource list (`github.com/google/elephant-goldfish-model`, Dave Rensin essay, Matt Pocock `/teach`, `nuttxspace` repo & `docs/egm/tutorial/`).
  - **Spoken (`bjm-voice`)**: Thanks very much everyone—take an Elephant-Goldfish pass onto your gnarliest legacy codebase, and come grab me afterwards for questions!
