# Slide Structure, Stage Beats & 30-Minute Timing Budget Prototype

> **Prototype for Ticket 05**: [Prototype the Slide Structure, Stage Beats, and Timing Budget](./issues/05-slide-structure-and-pacing-outline.md)
> **Interactive Planner**: Open [`slide-structure-prototype.html`](./slide-structure-prototype.html) in a browser to switch between **Preset A (Recommended 33-Slide Deck)**, **Preset B (Live Embedded `<iframe>` Tour)**, and **Preset C (Ultra-Lean 29-Slide Deck)** or tweak individual slide timings.

---

## 1. Macro Pacing & 30-Minute Budget (`29m 51s` / `1,791s` — `9s` Buffer)

| Act | Modular File (`slides/acts/`) | Slides | Live Casts | Slide + Showcase Time | Cast Stage Time (Play + Stops) | Act Total |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: |
| **Act I — Hook & Hardware/RTOS Stress Test** | `01-hook-and-setup.md` | `5` (`S01`–`S05`) | `0` | `3m 15s` (`195s`) | `0m 00s` | **`3m 15s`** |
| **Act II — The Vanilla Vibe Foil** | `02-vanilla-foil.md` | `4` (`S06`–`S09`) | `1` (`vanilla-vibe`) | `1m 30s` (`90s`) | `1m 51s` (`111s`) | **`3m 21s`** |
| **Act III — Rolling Docs for the AI (EGM)** | `03-egm-journey.md` | `17` (`S10`–`S26`) | `5` (`01`–`05`) | `5m 20s` (`320s`) | `11m 47s` (`707s`) | **`17m 07s`** |
| **Act IV — Finale: Docs for the Human (`/teach-code`)** | `04-teach-code-finale.md` | `7` (`S27`–`S33`) | `1` (`06-teach-code`) | `4m 04s` (`244s`) | `2m 04s` (`124s`) | **`6m 08s`** |
| **Total** | **4 Modular Act Files** | **`33` slides** (`26` + `7` casts) | **`7` casts (`53` stops)** | **`14m 09s` (`849s`)** | **`15m 42s` (`942s`)** | **`29m 51s`** |

### Core Visual & Rhythm Rules
1. **Low-Density Slide Discipline (`~2.1` progressive beats per non-cast slide)**: Every concept slide holds **one primary idea** and reveals at most `2`–`3` concise `v-click` beats so only the point currently being spoken appears on screen.
2. **Consistent Cast Bracketing (`Setup → Cast → Debrief`)**: Every `.cast` player slide (`layout: cast-full`) is preceded by a `20s–30s` "What to Watch" setup slide and followed by a visual debrief slide that connects the terminal run back to the hardware/RTOS story.
3. **Zero Low-Level C Code or Tooling Internals**: All slides operate at the hardware pin/interface, RTOS subsystem, and AI workflow level.
4. **Finale Live Lesson Showcase (`S29`–`S31`, `2m 30s` dedicated showcase)**: Immediately after `06-teach-code.cast` finishes building the course, the finale tours the compiled interactive HTML lessons at `docs/egm/tutorial/index.html` (`./tutorial/index.html` in Slidev)—showing the 6-lesson Mermaid story arc, the 3 live JavaScript hardware/math simulators (`breadboard-simulator`, `pwm-segment-visualizer`, `adc-ema-simulator`), the fill-in-the-blank C99 exercises & equal-length quizzes, the printable `@media print` A4 workbench sheets, and the bonus twist where `Lesson 0006` caught 2 real Phase 5 bugs.

---

## 2. Slide-by-Slide Sequence & Progressive `v-click` Beats

### Act I — The Hook & Real-World Embedded Stress Test (`01-hook-and-setup.md` · `5` slides · `3m 15s`)

- **`S01` — Title Slide (`layout: center` · `25s`)**
  - **On Load**: *"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"* · Subtitle: *"Taming Apache NuttX & RP2350 Silicon with the Elephant-Goldfish Model and `/teach-code`"* · **Brett Morgan (Google Cloud)**.
- **`S02` — The Hardware Target: XIAO RP2350 & 6-LED Bar Graph (`layout: hardware-callout` · `45s`)**
  - **On Load (Left SVG/Diagram + Right Intro)**: Seeed Studio XIAO RP2350 (`14-pin` castellated module straddling the breadboard centre trench), `10 kΩ` potentiometer on `D0/A0` (`GPIO 26` / `ADC0`), and `6` discrete LEDs (`LED1`–`LED6`: 2 Green, 2 Yellow, 2 Red with `330 Ω` resistors).
  - **`v-click 1`**: **Proportional 6-Segment PWM Display** — smooth brightness ramping across the active segment as you turn the pot.
  - **`v-click 2`**: **Dual Runtime Requirement** — autonomous `50 Hz` background daemon (`bargraphd`) *while* keeping the single USB-C `nsh>` console interactive (`start`, `stop`, `status`, `test`, `set`).
- **`S03` — The RTOS Scale Shock: Why Apache NuttX Breaks "Vibe Coding" (`layout: default` · `40s`)**
  - **On Load**: Contrast flat Arduino `loop()` sketches with **Apache NuttX**—an industrial POSIX RTOS where hardware lives behind character drivers (`/dev/adc0`, `/dev/pwmX`) and `Kconfig` trees.
  - **`v-click 1`**: **The Documentation Gap** — **`3,241` directories** across `nuttx/` (`2,607`) and `apps/` (`634`), shipping with only **`13` upstream `README.md` files** (`0.4%` directory coverage).
  - **`v-click 2`**: **The Consequence** — without a navigable map, coding agents fall back to blind `grep` and training-weight guesswork.
- **`S04` — Three Hardware & RTOS Booby Traps Waiting on the Bench (`layout: default` · `55s`)**
  - **`v-click 1` (Trap 1 — USB-C Console Boot Hang)**: Blocking startup hangs USB-C `nsh>` before enumeration; naive dual-startup scripts create daemon races.
  - **`v-click 2` (Trap 2 — ADC Pin Clobber & Erratum `RP2350-E9`)**: NuttX ADC init resets all 4 ADC pins (`GPIO 26–29`) and leaves digital input buffer `IE=1` enabled—clobbering LEDs on `D1`/`D2` or injecting pad leakage into `D0/A0`.
  - **`v-click 3` (Trap 3 — The `500k IRQ/s` Interrupt Storm)**: Opening `/dev/adc0` triggers continuous `500 kHz` (`2 µs`) interrupts that starve the CPU—while opening/closing at `40 Hz` hits an unguarded ISR teardown race.
- **`S05` — The Controlled Experiment: Same Hardware, Same Prompt, Two Workflows (`layout: default` · `30s`)**
  - **`v-click 1` (Experiment A — Vanilla Vibe Foil)**: Unconstrained Antigravity against raw upstream NuttX (`13` READMEs, `0` custom skills).
  - **`v-click 2` (Experiment B — Roll Your Own Docs)**: Antigravity + **Elephant-Goldfish Model (EGM)** directory contracts & adversarial review + **`/teach-code`** interactive human course.

---

### Act II — The Vanilla Vibe Foil (`02-vanilla-foil.md` · `4` slides · `3m 21s`)

- **`S06` — Act II Setup: Watching Unconstrained Antigravity (`layout: default` · `25s`)**
  - **`v-click 1`**: Paste the exact functional prompt into Antigravity with zero directory contracts or review swarms.
  - **`v-click 2`**: What to watch in `vanilla-vibe.cast`: (1) asks `1` clarifying pin question, (2) sprints through a `26-minute` monolithic build (`0` `README.md` reads), and (3) generates a breadboard wiring SVG.
- **`S07` — LIVE CAST 1: `vanilla-vibe.cast` (`layout: cast-full` · `1m 15s` play + `36s` stops = `1m 51s`)**
  - Starts on **AGY Start Page** (`0. Start`) $\rightarrow$ `Stop 1` Initial Prompt $\rightarrow$ `Stops 2–3` `Q1`/`A1` Pins $\rightarrow$ `Stop 4` `26m` Firmware Build (`29.8s @ 52x`) $\rightarrow$ `Stops 5–6` Breadboard Wiring SVG.
- **`S08` — Debrief: The Lie of "It Compiled!" (`layout: hardware-callout` · `35s`)**
  - **Left Image**: `breadboard-vanilla-crop-ch.svg` (`docs/vanilla/breadboard_circuit.svg`).
  - **`v-click 1`**: It compiled `nuttx.uf2` and passed `7/7` host math unit tests on the first try—giving a comforting green checkmark.
  - **`v-click 2`**: Look at the physical breadboard SVG: it seated the `0.3"` XIAO module across **Columns C to H (`0.7"` / 6 columns wide)**, burying the tie-points underneath the board!
- **`S09` — Vanilla Vibe Audit Scorecard: `5 / 10` Against the Hardware Traps (`layout: default` · `30s`)**
  - **`v-click 1`**: **Where Vanilla Shone** — pure C math, Q4 EMA filter, and host unit test structure (strong in LLM pre-training).
  - **`v-click 2`**: **Where Vanilla Failed on Silicon (`5 / 10`)** — `0` README reads meant it missed `RP2350-E9` leakage, looped `open()`/`close()` at `40 Hz` into the ISR teardown race, and silently dropped Kconfig symbols.

---

### Act III — Rolling Docs for the AI: The Elephant-Goldfish Model (`03-egm-journey.md` · `17` slides · `17m 07s`)

- **`S10` — Act III Anchor: The Elephant-Goldfish Triad (`layout: default` · `40s`)**
  - **`v-click 1` (The Elephant — Context Memory)**: `3,241` directory-level `README.md` contracts giving the agent a top-down architectural map (`209` reads vs `0` in Vanilla).
  - **`v-click 2` (The Human — Intent & Ownership)**: Makes product and hardware-boundary decisions one question at a time; owns what ships.
  - **`v-click 3` (The Goldfish — Stateless Adversarial Swarm)**: Fresh-context review subagents with zero memory of the drafting struggle—immune to author sycophancy.
- **`S11` — Unpacking The Elephant: Directory Contracts as a Navigable Map (`layout: skill-breakdown` · `40s`)**
  - **`v-click 1` (Beat 1 — Tree Scale)**: `2,607` `README.md` contracts in `nuttx/` + `634` in `apps/` navigated top-down.
  - **`v-click 2` (Beat 2 — Stylized Contract Card `nuttx/arch/arm/src/rp23xx`)**: Reveals *(a) Scope & Public Interfaces* (`rp23xx_adc.c`, `rp23xx_pwm.c`) and *(b) Plain-English Hardware Invariants* (*"ADC init resets all 4 pins `GPIO 26–29` and leaves digital buffer `IE=1` enabled"*).
  - **`v-click 3` (Beat 3 — Child Directory Routing)**: Tells the agent which subdirectory to open next without grepping raw C files.
- **`S12` — Keeping the Elephant Honest: `/plugin` $\rightarrow$ `/egm-bootstrap` $\rightarrow$ `/egm-drift-report` (`layout: default` · `25s`)**
  - **`v-click 1`**: Install `egm` dynamically via `/plugin` inside Antigravity.
  - **`v-click 2`**: Run `/egm-bootstrap` to generate the root workspace `README.md` blueprint.
  - **`v-click 3`**: Run `/egm-drift-report` to verify all `3,241` contracts match `git HEAD` before designing.
- **`S13` — LIVE CAST 2: `01-bootstrap.cast` (`layout: cast-full` · `0m 59s` play + `36s` stops = `1m 35s`)**
  - Starts on **AGY Start Page** (`0. Start`) $\rightarrow$ `Stops 1–3` `/plugin` install `egm` $\rightarrow$ `Stop 4` `/egm-bootstrap` $\rightarrow$ `Stops 5–6` `Q1` & `/egm-drift-report` completion.
- **`S14` — The Specification Funnel: Human + Elephant (`/egm-brainstorm`) (`layout: default` · `30s`)**
  - **`v-click 1`**: Reads `49` `README.md` contracts upfront—spotting the `GPIO 26–29` ADC pin clobber before asking Question 1.
  - **`v-click 2`**: `1-at-a-time` Socratic Q&A (`Q1` Pin Mapping avoiding `GPIO 27–29`, `Q2` Background Daemon keeping USB-C `nsh>`, `Q3` Proportional PWM Brightness).
  - **`v-click 3`**: Readiness checkpoint synthesizes decisions into `concepts_brief.md`.
- **`S15` — LIVE CAST 3: `02-brainstorm.cast` (`layout: cast-full` · `1m 46s` play + `54s` stops = `2m 40s`)**
  - Starts on **AGY Start Page** (`0. Start`) $\rightarrow$ `Stop 1` `/egm-brainstorm` prompt $\rightarrow$ `Stops 2–7` `Q1/A1`, `Q2/A2`, `Q3/A3` (with `0.8s` tightened replies) $\rightarrow$ `Stops 8–9` `Q7` Readiness Gate & Goldfish Synthesis (`concepts_brief.md`).
- **`S16` — Section-by-Section Drafting & Planting the `RP23XX_ADC_DIV` Trap (`layout: default` · `25s`)**
  - **`v-click 1`**: `/egm-draft` turns `concepts_brief.md` into `docs/designs/xiao-rp2350-bargraph.md` with section-by-section human approval gates.
  - **`v-click 2`**: **Watch Stop 4 (`Q3: Technical Plan`)** — both the Elephant and the Human approve throttling `/dev/adc0` via `CONFIG_RP23XX_ADC_DIV`. Sounds completely plausible... and it's a trap!
- **`S17` — LIVE CAST 4: `03-draft.cast` (`layout: cast-full` · `1m 55s` play + `36s` stops = `2m 31s`)**
  - Starts on **AGY Start Page** (`0. Start`) $\rightarrow$ `Stop 1` `/egm-draft @concepts_brief.md` $\rightarrow$ `Stops 2–3` `Q1` Context & `Q2` Overview gates $\rightarrow$ `Stop 4` `Q3` Technical Plan gate (`RP23XX_ADC_DIV` approved!) $\rightarrow$ `Stop 5` `v0-draft` promoted.
- **`S18` — Debrief: Author Bias, Self-Sycophancy & Why We Need Goldfish (`layout: default` · `25s`)**
  - **`v-click 1`**: Why `RP23XX_ADC_DIV` is a landmine — NuttX's driver uses single-shot `START_ONCE`, which ignores the hardware pace divider completely and still fires **`500,000 IRQ/s`**!
  - **`v-click 2`**: **Author Bias / Self-Sycophancy** — the session that spent 30 minutes drafting a spec will rubber-stamp its own assumptions. We need a reviewer with amnesia.
- **`S19` — Unpacking The Goldfish: Stateless Adversarial Swarm (`/egm-review`) (`layout: default` · `35s`)**
  - **`v-click 1` (Stateless & Uninvested)**: Fresh subagent context windows that see only the spec and the repo contracts—zero memory of our drafting struggle.
  - **`v-click 2` (Multi-Angle Adversarial Personas)**: *Codebase Details*, *Hostile Design Critic*, *Implementation Readiness*, and *Zoom-Out Architect* audit the spec against `146` `README.md` contracts in parallel.
  - **`v-click 3` (Synthesizer + `/egm-resolve`)**: Deduplicates findings into ranked Blockers (`B-1..B-3`) and Majors (`M-1..M-5`) for interactive human `accept`/`reject`.
- **`S20` — What to Watch in `04-review.cast`: `B-1`, `/clear` Amnesia, and Pass 2 (`layout: default` · `20s`)**
  - **`v-click 1`**: Watch Pass 1 Item `1/12` (`B-1`) catch the `RP23XX_ADC_DIV` trap and replace it with NVIC IRQ gating.
  - **`v-click 2`**: Watch us run `/clear` to wipe session memory, then run Pass 2 with `GoldfishContext` to audit the updated spec.
- **`S21` — LIVE CAST 5: `04-review.cast` (`layout: cast-full` · `2m 21s` play + `1m 18s` stops = `3m 39s`)**
  - Starts on **AGY Start Page** (`0. Start`) $\rightarrow$ `Stops 1–3` Pass 1 `/egm-review` (`146` contracts) $\rightarrow$ `Stops 4–8` `/egm-resolve`, `Reply: interactive`, `B-1` NVIC IRQ gating & `Reply: accept` $\rightarrow$ `Stops 9–11` `Ready for /clear` $\rightarrow$ `Run /clear` $\rightarrow$ `Session Cleared` $\rightarrow$ `Stops 12–13` Pass 2 `/egm-review` catching `CA-01`.
- **`S22` — Debrief: First-Order Catch (Pass 1) vs. Second-Order Catch (Pass 2) (`layout: default` · `30s`)**
  - **`v-click 1` (First-Order Catch — Pass 1 `B-1`)**: Replaced the `500 kHz` storm with a 5-sample NVIC IRQ-gated burst (discard `1` stale sample + average `4`).
  - **`v-click 2` (Second-Order Catch — Pass 2 `CA-01`)**: Requesting `5` samples (`25 bytes`) from NuttX's `8-sample` FIFO blocks forever after `7` samples! `/clear` gave Pass 2 fresh eyes to catch the bug *introduced by Pass 1's fix*.
- **`S23` — When the Spec is Hardened, Coding is Boring (`05-implement` Setup) (`layout: default` · `20s`)**
  - **`v-click 1`**: `4` Blockers and `7` Majors eliminated in Markdown before writing a single line of C.
  - **`v-click 2`**: Watch `05-implement.cast` pass `100%` of host unit tests and build `nuttx/nuttx.uf2` (`347K`) on the **first attempt (`0` retries)**, plus write `9` new `README.md` contracts.
- **`S24` — LIVE CAST 6: `05-implement.cast` (`layout: cast-full` · `0m 58s` play + `24s` stops = `1m 22s`)**
  - Starts on **AGY Start Page** (`0. Start`) $\rightarrow$ `Stops 1–2` Firmware Implementation & 1st-try `nuttx.uf2` build $\rightarrow$ `Stops 3–4` Breadboard Wiring Guide & SVG complete.
- **`S25` — Physical Breadboard Payoff: Vanilla (`0.7"` Straddle) vs. EGM (`Cols E & F`) (`layout: hardware-callout` · `25s`)**
  - **Left Image**: `breadboard-wiring-crop-ef.svg` (`docs/egm/breadboard-wiring.svg`).
  - **`v-click 1`**: XIAO RP2350 seated across **Columns E & F (`0.3"` center trench)** with open tie-points on both sides, 6 non-ADC PWM pins (`D1, D2, D4, D5, D8, D9`), and `RP2350-E9` pad isolation on `GPIO 26`.
- **`S26` — Candid Scorecard Callback: `5 / 10` vs. `10 / 10` (And Engineering Honesty) (`layout: default` · `30s`)**
  - **`v-click 1` (Primary 10-Trap Scorecard)**: Vanilla Vibe **`5 / 10`** vs. EGM **`10 / 10`** (`0` hardware/RTOS traps hit).
  - **`v-click 2` (Candid Strict Audit — `6 Pass / 4 Partial / 0 Fail`)**: A cold post-build Goldfish audit still flagged `4` polish items (`SIGTERM` cleanup, Makefile test scaffolding)—proving adversarial review is an ongoing engineering habit, not magic.

---

### Act IV — Finale: Rolling Docs for the Human & Live Lesson Showcase (`04-teach-code-finale.md` · `7` slides · `6m 08s`)

- **`S27` — Act IV Finale: From "Docs for the AI" to "Docs for the Human" (`layout: default` · `35s`)**
  - **`v-click 1` (The Comprehension Debt Problem)**: The AI scored `10/10` and shipped `872` lines of `bargraph_main.c`—but Dave Rensin's third rule says *"Humans Own the Outcomes."* Can **you** debug this circuit on the bench six months from now?
  - **`v-click 2` (Rolling Docs for the Human via `/teach-code`)**: Built on Matt Pocock's `/teach` skill and installed dynamically via `npx skills add domesticmouse/skills`—turning our `README.md` contracts, design spec, and Goldfish reports into a 6-lesson interactive HTML course.
- **`S28` — LIVE CAST 7: `06-teach-code.cast` (`layout: cast-full` · `1m 10s` play + `54s` stops = `2m 04s`)**
  - Starts in shell (`0. Start`) $\rightarrow$ `Stops 1–4` `npx skills add domesticmouse/skills` installing `teach-code` $\rightarrow$ `Stop 5` **AGY Start Page** $\rightarrow$ `Stops 6–7` `/teach-code` 6-lesson generation (`29.8s @ 79x`) $\rightarrow$ `Stops 8–9` `sync_listing.py` + `build.py` verification.
- **`S29` — FINALE SHOWCASE 1: Course Hub & 6-Lesson Story Arc (`docs/egm/tutorial/index.html`) (`layout: default` · `45s`)**
  - *(Includes direct launch link / preview of compiled `./tutorial/index.html`; or paired with optional full-viewport `S29b` live `<iframe>` in Preset B).*
  - **`v-click 1`**: Tour the compiled Tufte-style landing page (`docs/egm/tutorial/index.html`) and **Figure 0.1 Mermaid Story Arc** (`0001` Breadboard & `23.6 mA` Budget $\rightarrow$ `0002` 16-Bit PWM Scaling $\rightarrow$ `0003` `>> 19` Shift & Q4 EMA $\rightarrow$ `0004` Multi-Channel PWM $\rightarrow$ `0005` Out-of-Tree BSP & IRQ Gating $\rightarrow$ `0006` `50 Hz` `bargraphd` & NSH CLI).
  - **`v-click 2`**: **Zero-Divergence Gate** — every lesson compiles a runnable C99 companion app in `tutorial/apps/0001..0006/` (`-std=c99 -Wall -Wextra -Werror -pedantic`) verified by `sync_listing.py --check` and `build.py --check`.
- **`S30` — FINALE SHOWCASE 2: Interactive JS Simulators, Fill-in-the-Blank C99 & Quizzes (`Lesson 0001–0003`) (`layout: default` · `60s`)**
  - **`v-click 1` (Live Hardware & PWM Simulators)**: Drag the `0..3300 mV` wiper in `{{widget:breadboard-simulator}}` (`Lesson 0001`) and watch 6-channel `0..65535` duty interpolation in `{{widget:pwm-segment-visualizer}}` (`Lesson 0002`).
  - **`v-click 2` (Turning Goldfish Catches into Interactive Play — `Lesson 0003`)**: In `{{widget:adc-ema-simulator}}`, flip *"Discard stale `msgs[0]`"* (`CA-01`) and toggle `>> 2` vs `(ema_q4 + 8) >> 4` (`B-2`) to watch the waveform peg at quarter-turn!
  - **`v-click 3` (Active Recall over Passive Skimming)**: Tufte right-margin sidenotes, Prism `keep-markup` fill-in-the-blank C99 exercises, and equal-word-count (`10 words/option`) retrieval quizzes.
- **`S31` — FINALE SHOWCASE 3: Teaching Dead Ends (`Lesson 0005`), Printable Bench Sheets & Bonus Bug Catch! (`layout: default` · `45s`)**
  - **`v-click 1` (Dead Ends as Curriculum + Workbench Printouts)**: `Lesson 0005` explicitly teaches *why* our `RP23XX_ADC_DIV` false start failed under `START_ONCE`, paired with `4` printable `@media print` A4 reference sheets (`reference/nuttx-rp2350-driver-traps.md`).
  - **`v-click 2` (The Bonus Twist — Docs for Humans Catch Firmware Bugs Too!)**: Forcing `Lesson 0006`'s host `pthreads` harness to compile under strict C99 caught **2 real bugs** we missed in Phase 5 (including a `bargraph test` `MANUAL`-mode clobber)!
- **`S32` — The "Roll Your Own Docs" Flywheel: Three Habits for Monday Morning (`layout: default` · `30s`)**
  - **`v-click 1` (1. Feed the Elephant First)**: Maintain directory-level `README.md` contracts (`/egm-bootstrap` + `/egm-drift-report`) so agents navigate architecture instead of grepping blindly.
  - **`v-click 2` (2. Unleash Stateless Goldfish Before Coding)**: Audit specs with fresh-context subagents (`/egm-review` + `/clear`) to catch hardware and system traps in Markdown.
  - **`v-click 3` (3. Pay Down Comprehension Debt Immediately)**: Run `/teach-code` so a `10/10` AI codebase becomes a `10/10` mental model in your own head.
- **`S33` — Links, Open-Source Skills & Interactive Course (`layout: default` · `19s`)**
  - **On Load**: GitHub repo (`github.com/domesticmouse/nuttxspace`), `egm` plugin, `/teach-code` skill (`npx skills add domesticmouse/skills`), and live interactive course URL (`/nuttxspace/tutorial/index.html`).
