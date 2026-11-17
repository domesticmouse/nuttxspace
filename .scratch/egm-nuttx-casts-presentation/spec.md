# Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up — Authoritative Presentation Specification

- **Title**: Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up
- **Subtitle**: Taming Apache NuttX & RP2350 Silicon with the Elephant-Goldfish Model and `/teach-code`
- **Presenter**: Brett Morgan (Google Cloud)
- **Target Format**: 30-minute conference presentation (`38` slides: `31` non-cast + `7` live `.cast` slides · `32m 21s` paced across all framing/SVG additions)
- **Canonical Map**: [`map.md`](./map.md)
- **Supporting Prototypes & Assets**:
  - Cast Curation & Trim Schedule: [`cast-curation-prototype.md`](./cast-curation-prototype.md) · [`cast-curation-prototype.html`](./cast-curation-prototype.html)
  - Slide Structure & Timing Planner: [`slide-structure-prototype.md`](./slide-structure-prototype.md) · [`slide-structure-prototype.html`](./slide-structure-prototype.html)
  - Verbatim `bjm-voice` Script: [`speaker-script-bjm-voice.md`](./speaker-script-bjm-voice.md)

---

## 1. Presentation Thesis, Scope & Visual Rules

### 1.1 Core Narrative Arc
1. **Act I — The Hook & Real-World Embedded Stress Test (`S01`, `S01b`, `S02a`–`S02c`, `S03`–`S05`, `4m 40s`)**: Introduce **Apache NuttX** and **Google Antigravity** (`S01b`), walk through breadboard internal spring-clip scoping (`S02a`) and the 28-column physical wiring layout (`S02b`), then frame the `AU$10` Seeed Studio XIAO RP2350 + `10 kΩ` potentiometer (`D0/A0`) + 6-LED PWM bar graph (`bargraphd` + interactive USB-C `nsh>` CLI) (`S02c`) inside **Apache NuttX** (`3,241` directories vs `13` upstream `README.md` files, `0.4%` coverage) and introduce the **3 hardware/RTOS booby traps** waiting on the bench:
   - *Trap 1 — USB-C Console Boot Hang*: Blocking startup hangs USB-C `nsh>` enumeration; naive dual-startup scripts cause daemon boot races.
   - *Trap 2 — ADC Pin Clobber & Erratum `RP2350-E9` Pad Leakage*: NuttX's RP2350 ADC init resets all 4 ADC pins (`GPIO 26–29`) and leaves the digital input buffer `IE=1` enabled—clobbering LEDs on `D1`/`D2` or injecting pad leakage into `D0/A0`.
   - *Trap 3 — The `500k IRQ/s` Interrupt Storm*: Opening `/dev/adc0` triggers continuous `500 kHz` (`2 µs`) free-running interrupts that starve the CPU—while opening/closing `/dev/adc0` at `40 Hz` hits an unguarded ISR teardown race.
2. **Act II — The Vanilla Vibe Foil (`S06`, `S07`, `S07a`, `S08`, `S09`, `3m 41s`)**: Play `vanilla-vibe.cast` (`1m 51s` stage) showing unconstrained Antigravity succumb to Dave Rensin's *"Weeds Syndrome"* (`0` `README.md` reads, `26m` monolithic code gen), reveal the full generated breadboard SVG (`S07a`, `./images/breadboard_circuit.svg`), and give fair credit for clean host C math and `7/7` host tests before zooming in on the physical `0.7"` (`Cols C–H`) breadboard straddle (`S08`, `./images/breadboard-vanilla-crop-ch.svg`) and the **`5 / 10`** hardware trap scorecard (`S09`).
3. **Act III — Rolling Docs for the AI: The Elephant-Goldfish Model (`S10`–`S24`, `S24a`, `S25`–`S26`, `18m 02s`)**: Introduce Dave Rensin's **Elephant-Goldfish Model (EGM)** (*"Design is the new code: design with an Elephant, review with a Goldfish"*) and step through `01-bootstrap.cast` (`1m 35s`), `02-brainstorm.cast` (`2m 40s`), `03-draft.cast` (`2m 31s`—planting the `CONFIG_RP23XX_ADC_DIV` false start at Stop 4), `04-review.cast` (`3m 39s`—Pass 1 `B-1` NVIC IRQ burst gating $\rightarrow$ `/clear` session amnesia $\rightarrow$ Pass 2 `CA-01` `25-byte` short-read FIFO catch), and `05-implement.cast` (`1m 22s`—1st-attempt `nuttx.uf2` build), reveal the full EGM breadboard schematic (`S24a`, `./images/breadboard-wiring.svg`), run the candid `S25` breadboard reality check (`./images/breadboard-wiring-crop-ef.svg`: verified non-ADC pins, `23.6 mA` budget, and `IE=0` for `RP2350-E9` alongside the `0.6"` XIAO header spacing vs. `0.3"` `Cols E & F` / `0.7"` `Cols C–H` footprint catch), and deliver the **`10 / 10`** primary vs **`6 Pass / 4 Partial / 0 Fail`** strict post-build audit (`S26`).
4. **Act IV — Finale: Rolling Docs for the Human (`/teach-code`) & Live Lesson Showcase (`S27`–`S33`, `5m 58s`)**: Bridge from *"Docs for the AI"* to *"Docs for the Human"* (Dave Rensin's *"Humans Own the Outcomes"* + Matt Pocock's `/teach` adapted via `/agy-customizations` into `/teach-code`). Play `06-teach-code.cast` (`2m 04s` stage, keeping `npx skills add domesticmouse/skills`), then spend `2m 30s` (`S29`–`S31`) showcasing the compiled Edward Tufte interactive HTML course (`./tutorial/index.html`), its 3 live JavaScript simulators, printable `@media print` A4 bench sheets, and the bonus twist where compiling `Lesson 0006`'s strict C99 `pthreads` harness caught 2 real Phase 5 `bargraph test` bugs.

### 1.2 Standing Visual, Narrative & Scope Rules
- **Low-Density Slide Discipline (`~1.8` progressive `v-click` beats/slide)**: Every non-cast slide presents one core idea and uses `0`–`3` concise `v-click` reveals synced directly to the speaker's voice.
- **Consistent Cast Bracketing (`Setup → Live Cast → Debrief`)**: Every `layout: cast-full` slide is bracketed by a `20s–35s` setup slide ("What to Watch") and a visual debrief connecting the terminal run to the hardware/RTOS story.
- **Strict Scope Exclusions (Out of Scope)**:
  - **Zero low-level C code lines**, register bitmask manipulation, or kernel driver C syntax debugging on slides.
  - **Zero asciinema / `pyte` / ANSI escape internals** or terminal recording tool mechanics.
- **Narrator Voice (`bjm-voice`)**: Australian English (*G'day*, *AU\$10*, *memorised*, *realise*, *colour-coded*, *well-organised*, *labelled*, *modelled*, *centre*, *fibreglass*, *amongst*), conversational engineering peer tone (`~140 wpm`), crediting Dave Rensin (`@drensin`), Matt Pocock (`/teach`), and Edward Tufte while candidly owning all five false-start/audit/footprint beats.

---

## 2. Verified Timing Budget & Macro Pacing Matrix (`38` Slides)

> **Pacing Note**: Adding the five visual context/SVG slides (`S01b`, `S02a`–`S02c`, `S07a`, `S24a`) and reframing `S25` brings total paced runtime to `32m 21s`, which compresses cleanly inside a 30-minute stage slot by trimming `~2m 21s` from stop-point pause budgets or overview commentary.

### 2.1 Act-by-Act Budget (`32m 21s` / `1,941s` Paced Total)

| Act | Modular File (`slides/acts/`) | Slide Range | Non-Cast Slides | Live `.cast` Slides | Slide + Showcase Time | Live Cast Stage Time (Play + Pauses) | Act Total |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **Act I — Hook & Hardware/RTOS Stress Test** | `01-hook-and-setup.md` | `S01`–`S05` (`+S01b, S02a–c`) | `8` | `0` | `4m 40s` (`280s`) | `0m 00s` (`0s`) | **`4m 40s` (`280s`)** |
| **Act II — The Vanilla Vibe Foil** | `02-vanilla-foil.md` | `S06`–`S09` (`+S07a`) | `4` | `1` (`S07`) | `1m 50s` (`110s`) | `1m 51s` (`111s`) | **`3m 41s` (`221s`)** |
| **Act III — Rolling Docs for the AI (EGM)** | `03-egm-journey.md` | `S10`–`S26` (`+S24a`) | `13` | `5` (`S13, S15, S17, S21, S24`) | `6m 15s` (`375s`) | `11m 47s` (`707s`) | **`18m 02s` (`1,082s`)** |
| **Act IV — Finale: Docs for the Human (`/teach-code`)** | `04-teach-code-finale.md` | `S27`–`S33` | `6` | `1` (`S28`) | `3m 54s` (`234s`) | `2m 04s` (`124s`) | **`5m 58s` (`358s`)** |
| **Total** | **4 Modular Act Files** | **`38` Slides** | **`31` slides** | **`7` casts (`53` stops)** | **`16m 39s` (`999s`)** | **`15m 42s` (`942s`)** | **`32m 21s` (`1,941s`)** |

### 2.2 Per-Slide Timing Verification Table (`38` Slides)

| Slide | Act File | Title | Layout | `v-click`s | Duration (`s`) | Cumulative |
| :-: | :--- | :--- | :--- | :-: | :-: | :-: |
| `S01` | `01-hook-and-setup.md` | Title Slide: Roll Your Own Docs | `center` | `0` | `25s` | `00:25` |
| `S01b` | `01-hook-and-setup.md` | The Stack: Apache NuttX & Google Antigravity | `default` | `0` | `20s` | `00:45` |
| `S02a` | `01-hook-and-setup.md` | How a Solderless Breadboard Works | `default` | `0` | `40s` | `01:25` |
| `S02b` | `01-hook-and-setup.md` | Physical Breadboard Wiring: XIAO RP2350 6-LED Bar Graph | `center` | `0` | `30s` | `01:55` |
| `S02c` | `01-hook-and-setup.md` | The Hardware Target: XIAO RP2350 & 6-LED Bar Graph | `default` | `2` | `40s` | `02:35` |
| `S03` | `01-hook-and-setup.md` | The RTOS Scale Shock: Why Apache NuttX Breaks "Vibe Coding" | `default` | `2` | `40s` | `03:15` |
| `S04` | `01-hook-and-setup.md` | Three Hardware & RTOS Booby Traps Waiting on the Bench | `default` | `3` | `55s` | `04:10` |
| `S05` | `01-hook-and-setup.md` | The Controlled Experiment: Same Hardware, Same Prompt, Two Workflows | `default` | `2` | `30s` | `04:40` |
| `S06` | `02-vanilla-foil.md` | Act II Setup: Watching Unconstrained Antigravity ("Weeds Syndrome") | `default` | `2` | `25s` | `05:05` |
| `S07` | `02-vanilla-foil.md` | **LIVE CAST 1**: `vanilla-vibe.cast` (`6` stops · `75s` play + `36s` stops) | `cast-full` | `6 stops` | `111s` | `06:56` |
| `S07a` | `02-vanilla-foil.md` | Vanilla Vibe's Generated Breadboard Wiring | `default` | `0` | `20s` | `07:16` |
| `S08` | `02-vanilla-foil.md` | Debrief: The Lie of "It Compiled!" (`Cols C–H` `0.7"` Straddle) | `hardware-callout` | `2` | `35s` | `07:51` |
| `S09` | `02-vanilla-foil.md` | Vanilla Vibe Audit Scorecard: `5 / 10` Against the Hardware Traps | `default` | `2` | `30s` | `08:21` |
| `S10` | `03-egm-journey.md` | Act III Anchor: The Elephant-Goldfish Triad | `default` | `3` | `40s` | `09:01` |
| `S11` | `03-egm-journey.md` | Unpacking The Elephant: Directory Contracts as a Navigable Map | `skill-breakdown` | `3` | `40s` | `09:41` |
| `S12` | `03-egm-journey.md` | Keeping the Elephant Honest: `/plugin` $\rightarrow$ `/egm-bootstrap` $\rightarrow$ `/egm-drift-report` | `default` | `3` | `25s` | `10:06` |
| `S13` | `03-egm-journey.md` | **LIVE CAST 2**: `01-bootstrap.cast` (`6` stops · `59s` play + `36s` stops) | `cast-full` | `6 stops` | `95s` | `11:41` |
| `S14` | `03-egm-journey.md` | The Specification Funnel: Human + Elephant (`/egm-brainstorm`) | `default` | `3` | `30s` | `12:11` |
| `S15` | `03-egm-journey.md` | **LIVE CAST 3**: `02-brainstorm.cast` (`9` stops · `106s` play + `54s` stops) | `cast-full` | `9 stops` | `160s` | `14:51` |
| `S16` | `03-egm-journey.md` | Section-by-Section Drafting & Planting the `RP23XX_ADC_DIV` Trap | `default` | `2` | `25s` | `15:16` |
| `S17` | `03-egm-journey.md` | **LIVE CAST 4**: `03-draft.cast` (`6` stops · `115s` play + `36s` stops) | `cast-full` | `6 stops` | `151s` | `17:47` |
| `S18` | `03-egm-journey.md` | Debrief: Author Bias, Self-Sycophancy & Why We Need Goldfish | `default` | `2` | `25s` | `18:12` |
| `S19` | `03-egm-journey.md` | Unpacking The Goldfish: Stateless Adversarial Swarm (`/egm-review`) | `default` | `3` | `35s` | `18:47` |
| `S20` | `03-egm-journey.md` | What to Watch in `04-review.cast`: `B-1`, `/clear` Amnesia, and Pass 2 | `default` | `2` | `20s` | `19:07` |
| `S21` | `03-egm-journey.md` | **LIVE CAST 5**: `04-review.cast` (`13` stops · `141s` play + `78s` stops) | `cast-full` | `13 stops` | `219s` | `22:46` |
| `S22` | `03-egm-journey.md` | Debrief: First-Order Catch (`B-1`) vs. Second-Order Catch (`CA-01`) | `default` | `2` | `30s` | `23:16` |
| `S23` | `03-egm-journey.md` | When the Spec is Hardened, Coding is Boring (`05-implement` Setup) | `default` | `2` | `20s` | `23:36` |
| `S24` | `03-egm-journey.md` | **LIVE CAST 6**: `05-implement.cast` (`4` stops · `58s` play + `24s` stops) | `cast-full` | `4 stops` | `82s` | `24:58` |
| `S24a` | `03-egm-journey.md` | EGM's Generated Breadboard Wiring | `default` | `0` | `20s` | `25:18` |
| `S25` | `03-egm-journey.md` | Breadboard Reality Check: Electrical Wins vs. Physical Footprint (`0.6"` XIAO) | `hardware-callout` | `2` | `35s` | `25:53` |
| `S26` | `03-egm-journey.md` | Candid Scorecard Callback: `5 / 10` vs. `10 / 10` (And Engineering Honesty) | `default` | `2` | `30s` | `26:23` |
| `S27` | `04-teach-code-finale.md` | Act IV Finale: From "Docs for the AI" to "Docs for the Human" | `default` | `2` | `35s` | `26:58` |
| `S28` | `04-teach-code-finale.md` | **LIVE CAST 7**: `06-teach-code.cast` (`9` stops · `70s` play + `54s` stops) | `cast-full` | `9 stops` | `124s` | `29:02` |
| `S29` | `04-teach-code-finale.md` | **FINALE SHOWCASE 1**: Course Hub & 6-Lesson Story Arc (`./tutorial/index.html`) | `default` | `2` | `45s` | `29:47` |
| `S30` | `04-teach-code-finale.md` | **FINALE SHOWCASE 2**: Interactive JS Simulators, C99 Exercises & Quizzes | `default` | `3` | `60s` | `30:47` |
| `S31` | `04-teach-code-finale.md` | **FINALE SHOWCASE 3**: Teaching Dead Ends, Printable Bench Sheets & Bug Catch! | `default` | `2` | `45s` | `31:32` |
| `S32` | `04-teach-code-finale.md` | The "Roll Your Own Docs" Flywheel: Three Habits for Monday Morning | `default` | `3` | `30s` | `32:02` |
| `S33` | `04-teach-code-finale.md` | Links, Open-Source Skills & Interactive Course | `default` | `0` | `19s` | `32:21` |

---

## 3. Curated `.cast` Recording Specifications & Stop-Point Schedule

### 3.1 Three Stage Rules Across All 7 Recordings
1. **Start State (`0. Start` = AGY Start Page for Casts 1–6; Shell `npx skills add` for Cast 7)**:
   - Casts `1`–`6` (`vanilla-vibe.cast` and `01-bootstrap.cast` through `05-implement.cast`) strip the opening shell `agy` launch sequence so each player loads directly on the settled **Antigravity CLI start screen** (`0. Start` acts as the Before-Prompt stop point for Prompt 1).
   - Cast `7` (`06-teach-code.cast`) preserves the opening `npx skills add domesticmouse/skills` CLI installation sequence (`Stops 0–4`) before launching `agy` and settling on the AGY Start Page (`Stop 5`).
2. **Paired Before-Prompt & After-Prompt Stop Points**:
   - Every kept prompt and reply has a **Before-Prompt** stop point (settled empty input box or agent question) and an **After-Prompt** stop point (prompt/answer filled into the input box, paused before `Enter` is submitted).
3. **Tightened Hand-Typed Prompts (`~0.5s–0.8s` warped / `1.0s–1.6s` raw)**:
   - Hand-typed prompts and replies (`02-brainstorm` `A1–A3`, `03-draft` `/egm-draft @concepts_brief.md ...`, `04-review` `Reply: interactive` and `Reply: accept`) compress per-keystroke `dt` so typing completes in `< 1s` warped.

### 3.2 Master Cast Budget Summary (`10m 24s` Warped Play + `5m 18s` Pauses = `15m 42s` Stage Time)

| Cast # | Slide | File (`slides/public/casts/`) | HTML Prototype (`prototypes/`) | Raw Duration | Warped Play (`s`) | Stop Points | Pause Budget (`~6s`/stop) | Total Stage Time | Trimmed Sections |
| :-: | :-: | :--- | :--- | :-: | :-: | :-: | :-: | :-: | :--- |
| **1** | `S07` | `vanilla-vibe.cast` | `vanilla-vibe-cast-player.html` | `31m 57s` | `75.2s` (`1m 15s`) | `6` | `36s` | **`1m 51s` (`111s`)** | Shell `agy` launch (`#0000..#0022`) & trailing `Ctrl+D` exit stripped. |
| **2** | `S13` | `01-bootstrap.cast` | `01-bootstrap-cast-player.html` | `08m 14s` | `59.1s` (`0m 59s`) | `6` | `36s` | **`1m 35s` (`95s`)** | Shell `agy` launch (`#0000..#0020`) & `Move Drift Report & Commit` tail (`#4070..#5139`) stripped. |
| **3** | `S15` | `02-brainstorm.cast` | `02-brainstorm-cast-player.html` | `41m 32s` | `105.6s` (`1m 46s`) | `9` | `54s` | **`2m 40s` (`160s`)** | Shell `agy` launch (`#0000..#0039`), middle `Q4–Q6` CLI subcommand negotiation (`#4907..#7395`), and `Stage & Commit` tail (`#11058..#12435`) stripped; `A1–A3` tightened to `0.8s`. |
| **4** | `S17` | `03-draft.cast` | `03-draft-cast-player.html` | `21m 35s` | `115.0s` (`1m 55s`) | `6` *(5 markers + 0.Start)* | `36s` | **`2m 31s` (`151s`)** | Shell `agy` launch (`#0000..#0019`), `Q4–Q5` section gates (`#6478..#9259`), and `Save v0-draft & Commit` tail (`#13146..#14006`) stripped; `/egm-draft` prompt tightened to `0.8s`. |
| **5** | `S21` | `04-review.cast` | `04-review-cast-player.html` | `62m 13s` | `141.4s` (`2m 21s`) | `13` | `78s` | **`3m 39s` (`219s`)** | Shell `agy` launch (`#0000..#0036`), Pass 1 items `2–12` walk-through + archive (`#14020..#26319`), and Pass 2 auto-resolve/commit tail (`#33141..#37820`) stripped; `interactive` & `accept` tightened to `0.5s`. |
| **6** | `S24` | `05-implement.cast` | `05-implement-cast-player.html` | `21m 03s` | `58.2s` (`0m 58s`) | `4` | `24s` | **`1m 22s` (`82s`)** | Shell `agy` launch (`#0000..#0052`) & `Verify Submodules & Commit` tail (`#12402..#14101`) stripped. |
| **7** | `S28` | `06-teach-code.cast` | `06-teach-code-cast-player.html` | `42m 00s` | `69.5s` (`1m 10s`) | `9` | `54s` | **`2m 04s` (`124s`)** | Keeps full `npx skills add domesticmouse/skills` install (`#0000..#0136`), transitions to AGY Start Page (`Stop 5`), and runs `/teach-code` + verification prompts. |
| **Total** | — | **7 `.cast` Files** | **7 HTML Prototypes** | **`3h 48m 34s`** | **`624.0s` (`10m 24s`)** | **`53` stops** | **`318s` (`5m 18s`)** | **`15m 42s` (`942s`)** | **Compresses `3h 48m` of real terminal engineering into `10m 24s` of warped live playback.** |

---

## 4. Complete Slide-by-Slide Specification & Verbatim Speaker Script (`38` Slides: `S01`–`S33`)

### Act I — The Hook & Real-World Embedded Stress Test (`slides/acts/01-hook-and-setup.md` · `8` Slides · `4m 40s`)

---

#### `S01` — Title Slide (`01-hook-and-setup.md` · `layout: center` · `25s` · `0` `v-click`s)
- **Visual Specification**:
  - **Title**: `Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up`
  - **Subtitle**: `Taming Apache NuttX & RP2350 Silicon with the Elephant-Goldfish Model and /teach-code`
  - **Presenter Badge**: `Brett Morgan · Developer Relations Engineer, Google Cloud`
- **Verbatim Speaker Script (`bjm-voice` · `~55 words`)**:
  - **`[On Load]`**:
    > "G'day everyone, I'm Brett Morgan from Google Cloud. Tonight's talk is about what happens when you drop a coding agent into a massive, unfamiliar C codebase—and how rolling two kinds of living documentation lets both the AI *and* the human sitting at the keyboard actually understand what ships to silicon."

---

#### `S01b` — The Stack: Apache NuttX & Google Antigravity (`01-hook-and-setup.md` · `layout: default` · `20s` · `0` `v-click`s)
- **Visual Specification**:
  - **2-Column Platform Cards**:
    - **Left Card**: **Apache NuttX** (`./images/nuttx-logo.png` on a light card backdrop, Real-Time Operating System, `https://nuttx.apache.org/`).
    - **Right Card**: **Google Antigravity** (`./images/antigravity-icon__full-color.png` on a matching light card backdrop, Agentic Development Platform, `https://antigravity.google/`).
- **Verbatim Speaker Script (`bjm-voice` · `~50 words`)**:
  - **`[On Load]`**:
    > "Before we touch any hardware, let's call out the two main players in tonight's experiment. On the left is **Apache NuttX**, a mature, POSIX-compliant real-time operating system you can explore at `nuttx.apache.org`. On the right is **Google Antigravity**, the agentic development platform I used to tame it, available at `antigravity.google`."

---

#### `S02a` — How a Solderless Breadboard Works (`01-hook-and-setup.md` · `layout: default` · `40s` · `0` `v-click`s)
- **Visual Specification**:
  - **Full-Width Diagram**: `./images/breadboard-scopes.svg` showing horizontal power rails (global scope), 5-hole vertical tie-point strips (`a`–`e` and `f`–`j`, local nodes), and the centre trench namespace barrier.
- **Verbatim Speaker Script (`bjm-voice` · `~95 words`)**:
  - **`[On Load]`**:
    > "Before we wire up our hardware, let's take a quick X-ray look at a solderless breadboard—because its hidden metal spring clips behave a lot like variable scopes in code. Along the top and bottom, the horizontal power rails act like global scope: every hole in the blue minus rail or red plus rail is wired together. In the middle, each 5-hole vertical column—rows `a` through `e` above the trench, and `f` through `j` below it—is an independent local node. And that centre plastic trench is a hard namespace barrier: you seat a dual-row module like the XIAO RP2350 straddling `e` and `f` so its left and right pins never short together."

---

#### `S02b` — Physical Breadboard Wiring: XIAO RP2350 6-LED Bar Graph (`01-hook-and-setup.md` · `layout: center` · `30s` · `0` `v-click`s)
- **Visual Specification**:
  - **Full-Height Centred Schematic**: `./images/assets-breadboard-wiring.svg` showing the full 28-column breadboard layout with the XIAO RP2350 straddling the centre trench, the `10 kΩ` potentiometer and `100 nF` decoupling capacitor on `D0/A0` at `3.3 V`, and six discrete LEDs (2 Green, 2 Yellow, 2 Red) with `330 Ω` resistors on `D3`–`D5` and `D8`–`D10` (skipping `D1/D2` and `D6/D7`).
- **Verbatim Speaker Script (`bjm-voice` · `~70 words`)**:
  - **`[On Load]`**:
    > "Here is the full physical wiring layout on a 28-column breadboard. The XIAO RP2350 straddles the centre trench, leaving open tie-points on both sides of every pin. On the left, a ten-kilohm potentiometer feeds `D0/A0` with an optional hundred-nanofarad decoupling capacitor, strictly at `3.3` volts. Across the right, six discrete LEDs—two green, two yellow, two red—each get their own `330`-ohm current-limiting resistor on `D3` through `D5` and `D8` through `D10`, intentionally skipping `D1/D2` and `D6/D7`."

---

#### `S02c` — The Hardware Target: XIAO RP2350 & 6-LED Bar Graph (`01-hook-and-setup.md` · `layout: default` · `40s` · `2` `v-click`s)
- **Visual Specification**:
  - **Stacked Target Cards (`[On Load]` + `2` `v-click` cards)**:
    - **`[On Load]`**: **Bench Hardware** — `AU$10` Seeed Studio XIAO RP2350 + `10 kΩ` linear potentiometer on `D0/A0` (`GPIO 26 / ADC0`) + `6` through-hole LEDs (`2` Green, `2` Yellow, `2` Red with `330 Ω` resistors) on a 400-tie-point solderless breadboard.
    - **`[v-click 1]`**: **Proportional 6-Segment PWM Display** — Smooth `16-bit` hardware PWM brightness interpolation across the active segment as you turn the knob.
    - **`[v-click 2]`**: **Dual Runtime Requirement** — Autonomous `50 Hz` background daemon (`bargraphd`) *while* keeping the single USB-C `nsh>` serial console interactive for live CLI commands (`start`, `stop`, `status`, `test`, `set`).
- **Verbatim Speaker Script (`bjm-voice` · `~90 words`)**:
  - **`[On Load]`**:
    > "So to recap the target we're building: an AU\$10 Seeed Studio XIAO RP2350 sitting on a 400-tie-point solderless breadboard beside my laptop, wired to a ten-kilohm potentiometer on `D0/A0` and six through-hole LEDs—two green, two yellow, two red."
  - **`[v-click 1 — Proportional 6-Segment PWM Display]`**:
    > "Instead of crude on/off steps, I wanted smooth `16-bit` hardware PWM interpolation across all six channels so the active segment ramps proportionally in brightness as you twist the pot."
  - **`[v-click 2 — Dual Runtime Requirement]`**:
    > "And it needed to run as an autonomous fifty-hertz background daemon called `bargraphd`, while keeping the single USB-C serial console free for interactive shell commands like `status`, `test`, and `set`."

---

#### `S03` — The RTOS Scale Shock: Why Apache NuttX Breaks "Vibe Coding" (`01-hook-and-setup.md` · `layout: default` · `40s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: Contrast card — **Flat Arduino `loop()` Sketch** (`1` file, direct register/pin calls) vs. **Apache NuttX RTOS** (strict POSIX character drivers `/dev/adc0`, `/dev/pwmX`, `Kconfig` trees, and out-of-tree board overlays).
  - **`[v-click 1]`**: **The Documentation Gap** — **`3,241` directories** across `nuttx/` (`2,607`) and `apps/` (`634`), shipping with only **`13` upstream `README.md` files** (**`0.4%` directory coverage**).
  - **`[v-click 2]`**: **The Consequence** — Without a navigable architectural map, coding agents fall straight back to blind `grep` and training-weight guesswork.
- **Verbatim Speaker Script (`bjm-voice` · `~95 words`)**:
  - **`[On Load]`**:
    > "In Arduino, that's a thirty-line `loop()` sketch. The catch? I wanted to build it as a proper out-of-tree application inside **Apache NuttX**—thirty years of industrial POSIX real-time operating system C that I have *definitely* not memorised."
  - **`[v-click 1 — The Documentation Gap: 3,241 Dirs vs 13 READMEs]`**:
    > "When I checked the upstream submodules—`nuttx/` and `apps/`—they spanned **3,241 directories**, shipping with just **13 `README.md` files**. That is zero-point-four percent directory coverage."
  - **`[v-click 2 — The Consequence]`**:
    > "Drop an unguided coding agent into three thousand undocumented directories, and it falls straight back to blind `grep` and training-weight guesswork."

---

#### `S04` — Three Hardware & RTOS Booby Traps Waiting on the Bench (`01-hook-and-setup.md` · `layout: default` · `55s` · `3` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: Header — **Three Real-World Silicon & RTOS Hazards (Zero C Code Required to Understand)**
  - **`[v-click 1]`**: **Trap 1 — USB-C Console Boot Hang**: Single USB-C port (`CDC/ACM`). Blocking startup hangs `nsh>` enumeration; dual startup hooks trigger a boot daemon race.
  - **`[v-click 2]`**: **Trap 2 — ADC Pin Clobber & Erratum `RP2350-E9` Pad Leakage**: NuttX ADC init resets all `4` ADC pins (`GPIO 26–29`) and leaves digital input buffer `IE=1` enabled—clobbering LEDs on `D1`/`D2` or leaking current into `D0/A0`.
  - **`[v-click 3]`**: **Trap 3 — The `500k IRQ/s` Interrupt Storm**: Opening `/dev/adc0` triggers continuous `500 kHz` (`2 µs`) free-running interrupts that starve the CPU—while opening/closing at `40 Hz` hits an unguarded ISR teardown race.
- **Verbatim Speaker Script (`bjm-voice` · `~125 words`)**:
  - **`[On Load]`**:
    > "Worse still, this particular combination of RP2350 silicon and NuttX drivers hides three brutal booby traps waiting on the bench."
  - **`[v-click 1 — Trap 1: USB-C Console Boot Hang]`**:
    > "First: the XIAO has only one USB-C port. If your startup script blocks waiting on the daemon, USB enumeration hangs and bricks your console; if you wire two startup hooks, the daemon races itself on boot."
  - **`[v-click 2 — Trap 2: ADC Pin Clobber & Erratum RP2350-E9]`**:
    > "Second: NuttX's RP2350 ADC initialiser unconditionally resets all four ADC-capable pins (`GPIO 26` to `29`) and leaves the digital input buffer `IE=1` enabled—clobbering any LEDs wired to `D1` or `D2` and tripping Silicon Erratum `RP2350-E9` pad leakage on our potentiometer."
  - **`[v-click 3 — Trap 3: The 500k IRQ/s Interrupt Storm]`**:
    > "Third: opening `/dev/adc0` fires continuous five-hundred-kilohertz interrupts that starve the CPU cold—and if you try to dodge that by opening and closing the device forty times a second, you hit an unguarded ISR teardown race."

---

#### `S05` — The Controlled Experiment: Same Hardware, Same Prompt, Two Workflows (`01-hook-and-setup.md` · `layout: default` · `30s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: Same bench hardware, same functional goal, same coding agent (**Google Antigravity**).
  - **`[v-click 1]`**: **Experiment A — The Vanilla Vibe Foil**: Unconstrained Antigravity against raw upstream NuttX (`13` READMEs, `0` custom skills).
  - **`[v-click 2]`**: **Experiment B — Roll Your Own Docs**:
    1. **Docs for the AI**: **Elephant-Goldfish Model (`egm`)** directory contracts + stateless adversarial review swarm.
    2. **Docs for the Human**: **`/teach-code`** 6-lesson interactive HTML course & runnable C99 verification.
- **Verbatim Speaker Script (`bjm-voice` · `~70 words`)**:
  - **`[On Load]`**:
    > "So I ran a controlled experiment with Google Antigravity on the exact same hardware and functional goal."
  - **`[v-click 1 — Experiment A: Vanilla Vibe Foil]`**:
    > "In **Experiment A**, we run unconstrained 'Vanilla Vibe' coding against raw upstream NuttX—thirteen READMEs, zero custom skills."
  - **`[v-click 2 — Experiment B: Roll Your Own Docs (EGM + /teach-code)]`**:
    > "In **Experiment B**, we roll our own docs: first **Docs for the AI** using the Elephant-Goldfish Model, and then **Docs for the Human** using `/teach-code`."

---

### Act II — The Vanilla Vibe Foil (`slides/acts/02-vanilla-foil.md` · `5` Slides · `3m 41s`)

---

#### `S06` — Act II Setup: Watching Unconstrained Antigravity (`02-vanilla-foil.md` · `layout: default` · `25s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Dave Rensin's "Weeds Syndrome"** — Diving head-first into writing implementation code before understanding system invariants.
  - **`[v-click 1]`**: **The Input** — Paste the complete functional bar-graph prompt into Antigravity with zero directory contracts or review gates.
  - **`[v-click 2]`**: **What to Watch in `vanilla-vibe.cast` (`1m 15s` warped)**:
    - Asks `1` quick pin question (`Q1`/`A1`), then charges into a **`26-minute` monolithic code-gen sprint** (`0` `README.md` reads) and generates a breadboard SVG.
- **Verbatim Speaker Script (`bjm-voice` · `~60 words`)**:
  - **`[On Load]`**:
    > "Let's watch Experiment A first. Keep an eye out for what Google's **Dave Rensin** calls **'Weeds Syndrome'**—diving head-first into writing C code before knowing a single subsystem invariant."
  - **`[v-click 1]`**:
    > "We paste the functional prompt straight into Antigravity with zero directory contracts."
  - **`[v-click 2]`**:
    > "Watch it ask one quick pin question, sprint through a twenty-six-minute monolithic build without reading a single `README.md`, and spit out a breadboard wiring SVG."

---

#### `S07` — LIVE CAST 1: `vanilla-vibe.cast` (`02-vanilla-foil.md` · `layout: cast-full` · `1m 51s` stage = `1m 15s` play + `36s` stops)
- **Visual & Player Specification**:
  - **Component**: `<AsciinemaPlayer src="./casts/vanilla-vibe.cast" />` (backed by `prototypes/vanilla-vibe-cast-player.html`).
  - **Stop Points (`6` stops · Starts on AGY Start Page)**:
    - `0. Start` (`0.0s`): Settled on **AGY Start Page** (`Ready for Initial Prompt`; shell launch `#0000..#0022` stripped).
    - `Stop 1` (`+0.5s @ 2x`): `1. Initial Prompt: XIAO RP2350 Bar Graph` (After Prompt 1, paused before `Enter`).
    - `Stop 2` (`+14.1s @ 2x`): `2. Q1: Breadboard Pins & LED Polarity` (Before Reply 1).
    - `Stop 3` (`+0.5s @ 2x`): `3. A1: Any Free Breadboard Pins` (After Reply 1, paused before `Enter`).
    - `Stop 4` (`+29.8s @ 52x` — compresses `25m 51s` raw): `4. Firmware & Tests Built — Ready for Wiring Prompt` (Before Prompt 2).
    - `Stop 5` (`+0.5s @ 2x`): `5. Prompt: Breadboard Wiring Guide & SVG` (After Prompt 2, paused before `Enter`).
    - `Stop 6` (`+29.8s @ 11x`): `6. Breadboard Wiring Guide & SVG Complete` (End).
- **Dual-Layer Live Cast Commentary (`bjm-voice`)**:
  - **`[Stop 0 — AGY Start Page (Before Prompt 1)]`**:
    > "Here we are on the Antigravity start screen. Let's drop in the bar graph prompt." *(Press `p`)*
  - **`[Stop 1 — Initial Prompt Pasted (`0.5s @ 2x`)]`**:
    > "There's the full prompt—potentiometer on `D0/A0`, six PWM LEDs, background daemon, and NSH CLI. Let's send it." *(Press `p`)*
  - **`[Warp Run to Stop 2 (`14.1s @ 2x`)]`**:
    > *(Voiceover while agent thinks)*: "Notice it immediately probes the top-level directory, sees `nuttx/` and `apps/`, and decides it already knows enough to ask one clarifying question."
  - **`[Stop 2 — Q1: Breadboard Pins & LED Polarity (Before Reply 1)]`**:
    > "It asks which six pins we want for the LEDs and whether they're active-high." *(Press `p`)*
  - **`[Stop 3 — A1: Any Free Breadboard Pins (`0.5s @ 2x`)]`**:
    > "I give it rope: 'Pick any free breadboard pins you like, active-high, and generate the wiring guide.' Off it goes." *(Press `p`)*
  - **`[Warp Run to Stop 4 (`29.8s @ 52x` — compressing 25m 51s raw)]`**:
    > *(Voiceover during the 30s warp)*: "We're watching twenty-six raw minutes compressed at fifty-two times speed. This is textbook Weeds Syndrome: zero `README.md` reads, straight into creating Kconfig files, writing a monolithic C loop, running `cmake`, hitting compiler errors, patching headers on the fly, and running seven host unit tests until `nuttx.uf2` links cleanly."
  - **`[Stop 4 — Firmware & Tests Built (Before Prompt 2)]`**:
    > "Green checkmarks everywhere—`nuttx.uf2` built and seven out of seven unit tests passed! Now let's ask for the breadboard wiring SVG." *(Press `p`)*
  - **`[Stop 5 — Prompt: Breadboard Wiring Guide & SVG (`0.5s @ 2x`)]`**:
    > "Asking it to render `breadboard_circuit.svg` for our physical build." *(Press `p`)*
  - **`[Warp Run to Stop 6 (`29.8s @ 11x`)]`**:
    > *(Voiceover during the 30s warp)*: "It writes a Python generator and renders a gorgeous-looking dark-mode breadboard SVG. Everything looks like an unblemished triumph—until you pick up the actual hardware."
  - **`[Stop 6 — Breadboard Wiring Guide & SVG Complete]`**:
    > "Done. Let's inspect what it actually built."

---

#### `S07a` — Vanilla Vibe's Generated Breadboard Wiring (`02-vanilla-foil.md` · `layout: default` · `20s` · `0` `v-click`s)
- **Visual Specification**:
  - **Full-Width Diagram**: `./images/breadboard_circuit.svg` displaying the complete dark-mode breadboard wiring SVG generated by Vanilla Vibe (`docs/vanilla/breadboard_circuit.svg`).
- **Verbatim Speaker Script (`bjm-voice` · `~45 words`)**:
  - **`[On Load]`**:
    > "Here is the full breadboard wiring SVG that Vanilla Vibe just generated. At a glance, it looks clean and well-organised—complete with a dark-mode aesthetic, colour-coded jumper wires, and labelled components. Now let's zoom in on the XIAO RP2350 itself to see what actually happened on the board."

---

#### `S08` — Debrief: The Lie of "It Compiled!" (`02-vanilla-foil.md` · `layout: hardware-callout` · `35s` · `2` `v-click`s)
- **Visual Specification**:
  - **Left Pane (`hardware-callout` SVG)**: `./images/breadboard-vanilla-crop-ch.svg` (cropped close-up of `docs/vanilla/breadboard_circuit.svg` showing the XIAO RP2350 straddling Columns C through H).
  - **Right Pane**:
    - **`[v-click 1]`**: **Fair Credit on Host** — Clean C fixed-point math, `7/7` host unit tests passing, and a compiled `nuttx.uf2` binary on the first session.
    - **`[v-click 2]`**: **Physical Reality Check (`0.7"` Straddle Across `Cols C–H`)** — Stretched a `0.3"` wide module across **6 breadboard columns (`C` to `H`)**, burying almost every tie-point underneath the fibreglass PCB!
- **Verbatim Speaker Script (`bjm-voice` · `~80 words`)**:
  - **`[On Load]`**:
    > "Here is the SVG Vanilla Vibe just generated."
  - **`[v-click 1 — Green Checkmarks on Host]`**:
    > "To be completely fair to Vanilla Vibe, the host C math and unit test scaffolding were genuinely tidy, and `nuttx.uf2` compiled on the first session."
  - **`[v-click 2 — The 0.7" Breadboard Straddle across Cols C to H]`**:
    > "Except look closely at where it seated the XIAO module on the breadboard: it stretched a zero-point-three-inch module across **six columns—Columns C to H**! On a physical breadboard, that buries almost every tie-point underneath the fibreglass PCB."

---

#### `S09` — Vanilla Vibe Audit Scorecard: `5 / 10` Against the Hardware Traps (`02-vanilla-foil.md` · `layout: default` · `30s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: Headline Scorecard Badge — **Vanilla Vibe: `5 / 10` (`0` `README.md` reads)**.
  - **`[v-click 1]`**: **Where Vanilla Shone (`5` Passes — Pre-Training Strengths)** — Fixed-point Q4 EMA filter, proportional segment math, host unit test structure, and basic POSIX CLI parsing.
  - **`[v-click 2]`**: **Where Vanilla Failed on Silicon (`5` Failures — Unread Invariants)** — Tripped Erratum `RP2350-E9` pad leakage (`IE=1`), looped `open()`/`close()` at `40 Hz` into the unguarded ISR teardown race, straddled `Cols C–H`, and dropped Kconfig symbols.
- **Verbatim Speaker Script (`bjm-voice` · `~70 words`)**:
  - **`[On Load]`**:
    > "When we audited Vanilla Vibe against ten real hardware and RTOS invariants, it scored **five out of ten**."
  - **`[v-click 1 — Where Vanilla Shone]`**:
    > "Every pass was generic C knowledge baked into LLM pre-training: fixed-point Q4 EMA filtering, proportional segment slicing, and host unit tests."
  - **`[v-click 2 — Where Vanilla Failed on Silicon]`**:
    > "Every failure was an RP2350 or NuttX invariant it never read: `RP2350-E9` pad leakage, looping `open()` and `close()` at forty hertz into the ISR teardown race, and dropped Kconfig symbols."

---

### Act III — Rolling Docs for the AI: The Elephant-Goldfish Model (`slides/acts/03-egm-journey.md` · `18` Slides · `18m 02s`)

---

#### `S10` — Act III Anchor: The Elephant-Goldfish Triad (`03-egm-journey.md` · `layout: default` · `40s` · `3` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Dave Rensin (`@drensin`) — The Elephant-Goldfish Model (`egm`)**: *"Design is the new code: design with an Elephant, review with a Goldfish."*
  - **`[v-click 1]`**: **1. The Elephant (Context Memory)** — `3,241` directory-level `README.md` contracts giving the agent a top-down architectural map (`209` contract reads vs `0` in Vanilla).
  - **`[v-click 2]`**: **2. The Human (Intent & Ownership)** — Makes product and hardware-boundary calls one question at a time; owns the final outcome.
  - **`[v-click 3]`**: **3. The Goldfish (Stateless Adversarial Swarm)** — Fresh-context review subagents with 3-second memory—zero emotional loyalty or sycophancy toward the draft.
- **Verbatim Speaker Script (`bjm-voice` · `~95 words`)**:
  - **`[On Load]`**:
    > "To stop shipping hidden silicon timebombs, I turned to **Dave Rensin's Elephant-Goldfish Model** (`egm`). Dave sums it up in two lines: *'Design is the new code: design with an Elephant, review with a Goldfish.'*"
  - **`[v-click 1 — The Elephant (Context Memory)]`**:
    > "**The Elephant** is your long-horizon context memory—powered here by `3,241` directory-level `README.md` contracts so the agent reads `209` architectural contracts instead of zero."
  - **`[v-click 2 — The Human (Intent & Ownership)]`**:
    > "**The Human** makes product and hardware-boundary calls one question at a time."
  - **`[v-click 3 — The Goldfish (Stateless Adversarial Swarm)]`**:
    > "And **The Goldfish** is a swarm of fresh-context reviewers with three-second memory—zero emotional loyalty to your draft."

---

#### `S11` — Unpacking The Elephant: Directory Contracts as a Navigable Map (`03-egm-journey.md` · `layout: skill-breakdown` · `40s` · `3` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: Header — **How the Elephant Remembers `3,241` Directories Without Blowing the Context Window**
  - **`[v-click 1]`**: **Beat 1 — Top-Down Tree Map**: `2,607` `README.md` contracts in `nuttx/` + `634` in `apps/` navigated hierarchically from the root.
  - **`[v-click 2]`**: **Beat 2 — Stylized Contract Card (`nuttx/arch/arm/src/rp23xx/README.md`)**:
    - *Public Interfaces*: `rp23xx_adc.c`, `rp23xx_pwm.c`
    - *Plain-English Hardware Invariant*: `"ADC init resets all 4 pins GPIO 26–29 and leaves digital input buffer IE=1 enabled."`
  - **`[v-click 3]`**: **Beat 3 — Child Directory Routing**: Points the agent directly to the relevant board/driver subdirectory instead of grepping raw C files.
- **Verbatim Speaker Script (`bjm-voice` · `~95 words`)**:
  - **`[On Load]`**:
    > "How do you give an Elephant memory across three thousand directories without blowing out the context window?"
  - **`[v-click 1 — Beat 1: Tree Scale]`**:
    > "Every directory in `nuttx/` and `apps/` gets a concise `README.md` contract—`2,607` in the kernel, `634` in user space—forming a top-down tree."
  - **`[v-click 2 — Beat 2: Stylized Contract Card (`nuttx/arch/arm/src/rp23xx`)]`**:
    > "Inside `arch/arm/src/rp23xx/README.md`, it doesn't paste C code. It states public interfaces and plain-English hardware invariants—like warning that ADC init resets all four pins `GPIO 26` to `29` and leaves digital buffer `IE=1` enabled."
  - **`[v-click 3 — Beat 3: Child Directory Routing]`**:
    > "And each contract routes the agent to the exact child directory it needs next."

---

#### `S12` — Keeping the Elephant Honest: `/plugin` $\rightarrow$ `/egm-bootstrap` $\rightarrow$ `/egm-drift-report` (`03-egm-journey.md` · `layout: default` · `25s` · `3` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Phase 1 Setup — Wiring & Verifying the Elephant (`01-bootstrap.cast`)**
  - **`[v-click 1]`**: **Step 1 — `/plugin`**: Install the `egm` plugin dynamically from the Antigravity marketplace.
  - **`[v-click 2]`**: **Step 2 — `/egm-bootstrap`**: Synthesise the root workspace `README.md` architectural blueprint linking `nuttx/` and `apps/`.
  - **`[v-click 3]`**: **Step 3 — `/egm-drift-report`**: Verify all `3,241` directory contracts against `git HEAD` (`0` drifted contracts) before designing.
- **Verbatim Speaker Script (`bjm-voice` · `~60 words`)**:
  - **`[On Load]`**:
    > "Before we design anything, we wire up the Elephant in three quick commands."
  - **`[v-click 1]`**:
    > "First, we install the `egm` plugin live inside Antigravity using `/plugin`."
  - **`[v-click 2]`**:
    > "Second, `/egm-bootstrap` synthesises the root workspace `README.md` blueprint."
  - **`[v-click 3]`**:
    > "Third, `/egm-drift-report` audits all `3,241` directory contracts against `git HEAD` so stale docs never poison our design."

---

#### `S13` — LIVE CAST 2: `01-bootstrap.cast` (`03-egm-journey.md` · `layout: cast-full` · `1m 35s` stage = `0m 59s` play + `36s` stops)
- **Visual & Player Specification**:
  - **Component**: `<AsciinemaPlayer src="./casts/01-bootstrap.cast" />` (backed by `prototypes/01-bootstrap-cast-player.html`).
  - **Stop Points (`6` stops · Starts on AGY Start Page)**:
    - `0. Start` (`0.0s`): Settled on **AGY Start Page** (`Ready for /plugin`; shell launch `#0000..#0020` stripped).
    - `Stop 1` (`+0.7s @ 2x`): `1. Run /plugin` (After `/plugin`, paused before `Enter`).
    - `Stop 2` (`+2.4s @ 2x`): `2. Select egm in Discover` (Plugin manager open with `egm` highlighted).
    - `Stop 3` (`+3.0s @ 2x`): `3. egm Plugin Installed — Ready for /egm-bootstrap` (Before Prompt 1).
    - `Stop 4` (`+0.5s @ 2x`): `4. Prompt: /egm-bootstrap` (After Prompt 1, paused before `Enter`).
    - `Stop 5` (`+29.1s @ 7x`): `5. Q1: Run EGM Drift Report?` (`ask_user` widget with `✓ Yes` highlighted).
    - `Stop 6` (`+25.3s @ 6x`): `6. Root Blueprint & EGM Drift Report Complete` (End; `Move & Commit` tail `#4070..#5139` stripped).
- **Dual-Layer Live Cast Commentary (`bjm-voice`)**:
  - **`[Stop 0 — AGY Start Page (Before /plugin)]`**:
    > "Back on the Antigravity start screen—let's install `egm`." *(Press `p`)*
  - **`[Stop 1 — Run /plugin (`0.7s @ 2x`)]`**:
    > "`/plugin` opens the built-in marketplace." *(Press `p`)*
  - **`[Stop 2 — Select egm in Discover (`2.4s @ 2x`)]`**:
    > "We select `egm` from Discover and install it." *(Press `p`)*
  - **`[Stop 3 — egm Installed — Ready for /egm-bootstrap (`3.0s @ 2x`)]`**:
    > "Now all the `/egm-*` slash commands are live. Let's queue up `/egm-bootstrap`." *(Press `p`)*
  - **`[Stop 4 — Prompt: /egm-bootstrap (`0.5s @ 2x`)]`**:
    > "We ask `/egm-bootstrap` to generate our root `README.md` linking `nuttx/` and `apps/`." *(Press `p`)*
  - **`[Warp Run to Stop 5 (`29.1s @ 7x`)]`**:
    > *(Voiceover during 29s warp)*: "Watch the Elephant read the top-level submodule contracts first—no blind grepping—and assemble the root architectural map of the workspace."
  - **`[Stop 5 — Q1: Run EGM Drift Report?]`**:
    > "Root blueprint is written, and it asks if we want to run `/egm-drift-report` across the tree. Yes please." *(Press `p`)*
  - **`[Warp Run to Stop 6 (`25.3s @ 6x`)]`**:
    > *(Voiceover during 25s warp)*: "In twenty-five warped seconds it verifies all `3,241` directories against git history: zero drifted contracts. The Elephant's map is verified."
  - **`[Stop 6 — Root Blueprint & EGM Drift Report Complete]`**:
    > "Now the Elephant is ready to brainstorm."

---

#### `S14` — The Specification Funnel: Human + Elephant (`/egm-brainstorm`) (`03-egm-journey.md` · `layout: default` · `30s` · `3` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Phase 2 Setup — `/egm-brainstorm` (`02-brainstorm.cast`)**
  - **`[v-click 1]`**: **1. Reads `49` `README.md` Contracts Upfront** — Traverses `boards/arm/rp23xx/xiao-rp2350` and `arch/arm/src/rp23xx`, spotting the `GPIO 26–29` ADC pin clobber *before* asking Question 1.
  - **`[v-click 2]`**: **2. `1-at-a-Time` Socratic Q&A** — Human locks product boundaries (`Q1` Non-ADC PWM pins, `Q2` Background daemon keeping USB-C `nsh>`, `Q3` Proportional PWM brightness).
  - **`[v-click 3]`**: **3. Goldfish Readiness Checkpoint** — Synthesizes agreed architecture into `concepts_brief.md` with zero C code written.
- **Verbatim Speaker Script (`bjm-voice` · `~70 words`)**:
  - **`[On Load]`**:
    > "Next comes `/egm-brainstorm`, where the Elephant and the Human narrow down the product shape before any code exists."
  - **`[v-click 1 — 49 READMEs Read Upfront]`**:
    > "Before asking Question 1, the Elephant reads `49` `README.md` contracts—spotting the `GPIO 26` to `29` ADC pin clobber immediately."
  - **`[v-click 2 — 1-at-a-Time Socratic Q&A]`**:
    > "Then it interviews me one question at a time: pin mapping, USB-C `nsh>` coexistence, and proportional PWM."
  - **`[v-click 3 — Goldfish Synthesis]`**:
    > "Finally, a readiness gate synthesises our answers into `concepts_brief.md`."

---

#### `S15` — LIVE CAST 3: `02-brainstorm.cast` (`03-egm-journey.md` · `layout: cast-full` · `2m 40s` stage = `1m 46s` play + `54s` stops)
- **Visual & Player Specification**:
  - **Component**: `<AsciinemaPlayer src="./casts/02-brainstorm.cast" />` (backed by `prototypes/02-brainstorm-cast-player.html`).
  - **Stop Points (`9` stops · Starts on AGY Start Page · `Q4–Q6` & Commit tail trimmed · `A1–A3` tightened to `0.8s`)**:
    - `0. Start` (`0.0s`): Settled on **AGY Start Page** (`Ready for /egm-brainstorm`; shell launch `#0000..#0039` stripped).
    - `Stop 1` (`+0.5s @ 2x`): `1. Prompt: /egm-brainstorm` (After Prompt 1, paused before `Enter`).
    - `Stop 2` (`+29.5s @ 14x` — `49` README reads): `2. Q1: Breadboard Pin Mapping & Reference Voltage` (Before Reply 1).
    - `Stop 3` (`+0.8s @ 2x`, tightened from `22.4s` raw): `3. A1: Layout Connections & Design Breadboard` (After Reply 1).
    - `Stop 4` (`+12.3s @ 2x`): `4. Q2: Boot Startup Architecture vs. USB-C NSH` (Before Reply 2).
    - `Stop 5` (`+0.8s @ 2x`, tightened from `11.5s` raw): `5. A2: Spawn in Background, Keep USB-C NSH` (After Reply 2).
    - `Stop 6` (`+4.3s @ 2x`): `6. Q3: LED Bar Graph Display Mapping` (Before Reply 3).
    - `Stop 7` (`+0.8s @ 2x`, tightened from `9.7s` raw): `7. A3: PWM Brightness on Active Segment` (After Reply 3).
    - `Stop 8` (`+28.2s @ 7x`; `Q4–Q6` `#4907..#7395` stripped): `8. Q7: Readiness Checkpoint — Review with Goldfishes` (Before Reply 7).
    - `Stop 9` (`+28.4s @ 14x`): `9. Goldfish Synthesis Complete — concepts_brief.md` (End; `Stage & Commit` tail `#11058..#12435` stripped).
- **Dual-Layer Live Cast Commentary (`bjm-voice`)**:
  - **`[Stop 0 — AGY Start Page (Before /egm-brainstorm)]`**:
    > "Let's kick off `/egm-brainstorm` with our hardware goal." *(Press `p`)*
  - **`[Stop 1 — Prompt: /egm-brainstorm (`0.5s @ 2x`)]`**:
    > "Same goal as Vanilla Vibe, prefixed with `/egm-brainstorm`." *(Press `p`)*
  - **`[Warp Run to Stop 2 (`29.5s @ 14x` — 49 README reads)]`**:
    > *(Voiceover during 30s warp)*: "Look at the tool calls flying past: instead of writing C, it traverses `49` directory `README.md` contracts across `boards/arm/rp23xx/xiao-rp2350` and `arch/arm/src/rp23xx`, discovering upfront that `GPIO 27`, `28`, and `29` get clobbered by ADC init."
  - **`[Stop 2 — Q1: Breadboard Pin Mapping & Reference Voltage]`**:
    > "Look at `Q1`: because it read the contract, it explicitly warns us to keep our LEDs off the ADC pins!" *(Press `p`)*
  - **`[Stop 3 — A1: Layout Connections & Design Breadboard (`0.8s @ 2x`)]`**:
    > "We tell it to pick six non-ADC PWM pins and design a clean breadboard layout." *(Press `p`)*
  - **`[Stop 4 — Q2: Boot Startup Architecture vs. USB-C NSH (`12.3s @ 2x`)]`**:
    > "`Q2` spots Trap 1: how should `bargraphd` start on boot without hanging the single USB-C `nsh>` console?" *(Press `p`)*
  - **`[Stop 5 — A2: Spawn in Background, Keep USB-C NSH (`0.8s @ 2x`)]`**:
    > "Spawn `bargraphd` in the background and keep USB-C `nsh>` interactive." *(Press `p`)*
  - **`[Stop 6 — Q3: LED Bar Graph Display Mapping (`4.3s @ 2x`)]`**:
    > "`Q3` asks how the six LEDs should visually map the pot reading." *(Press `p`)*
  - **`[Stop 7 — A3: PWM Brightness on Active Segment (`0.8s @ 2x`)]`**:
    > "Lower segments full on, active segment ramping smoothly via hardware PWM." *(Press `p`)*
  - **`[Warp Run to Stop 8 (`28.2s @ 7x`)]`**:
    > *(Voiceover during 28s warp)*: "After locking in the CLI commands, it reaches the brainstorm readiness gate and asks to run a quick Goldfish sanity check before writing `concepts_brief.md`."
  - **`[Stop 8 — Q7: Readiness Checkpoint — Review with Goldfishes]`**:
    > "We select 'Yes, synthesize and review with Goldfishes'." *(Press `p`)*
  - **`[Warp Run to Stop 9 (`28.4s @ 14x`)]`**:
    > *(Voiceover during 28s warp)*: "In twenty-eight warped seconds, fresh subagents verify the brief against the contracts and write `concepts_brief.md`."
  - **`[Stop 9 — Goldfish Synthesis Complete — concepts_brief.md]`**:
    > "Now we have a grounded concept brief—still zero lines of C written."

---

#### `S16` — Section-by-Section Drafting & Planting the `RP23XX_ADC_DIV` Trap (`03-egm-journey.md` · `layout: default` · `25s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Phase 3 Setup — `/egm-draft` (`03-draft.cast` $\rightarrow$ `docs/designs/xiao-rp2350-bargraph.md`)**
  - **`[v-click 1]`**: **Section-by-Section Human Approval Gates** — Loads `51` contract/header files and drafts Overview, Technical Plan, Alternatives, and Implementation with interactive sign-off.
  - **`[v-click 2]`**: **Cueing the False Start at Stop 4 (`Q3: Technical Plan`)** — Watch both the Elephant and the Human approve throttling `/dev/adc0` via `CONFIG_RP23XX_ADC_DIV`. Sounds completely plausible... and it's a trap!
- **Verbatim Speaker Script (`bjm-voice` · `~60 words`)**:
  - **`[On Load]`**:
    > "In Phase 3, `/egm-draft` turns `concepts_brief.md` into our full engineering specification, `xiao-rp2350-bargraph.md`."
  - **`[v-click 1 — Section-by-Section Approval Gates]`**:
    > "Instead of dumping a wall of Markdown, it drafts section by section with human approval gates."
  - **`[v-click 2 — Cueing Stop 4's RP23XX_ADC_DIV Trap]`**:
    > "Watch **Stop 4 (`Q3: Technical Plan`)** closely: both the Elephant and I agree to throttle `/dev/adc0` using `CONFIG_RP23XX_ADC_DIV`. Sounds totally plausible—and it's a trap!"

---

#### `S17` — LIVE CAST 4: `03-draft.cast` (`03-egm-journey.md` · `layout: cast-full` · `2m 31s` stage = `1m 55s` play + `36s` stops)
- **Visual & Player Specification**:
  - **Component**: `<AsciinemaPlayer src="./casts/03-draft.cast" />` (backed by `prototypes/03-draft-cast-player.html`).
  - **Stop Points (`6` stops · Starts on AGY Start Page · `Q4–Q5` & Commit tail trimmed)**:
    - `0. Start` (`0.0s`): Settled on **AGY Start Page** (`Ready for /egm-draft`; shell launch `#0000..#0019` stripped).
    - `Stop 1` (`+0.8s @ 2x`, tightened from `8.7s` raw): `1. Prompt: /egm-draft @concepts_brief.md target docs/designs/xiao-rp2350-bargraph.md` (After Prompt 1).
    - `Stop 2` (`+28.3s @ 2x`): `2. Q1: Confirm Context & Start Drafting` (`✓ Proceed to Step 2` highlighted).
    - `Stop 3` (`+27.3s @ 3x`): `3. Q2: Section Approval — Overview & Goals` (`✓ Approve Overview & Goals` highlighted).
    - `Stop 4` (`+29.1s @ 15x`): `4. Q3: Section Approval — Technical Plan` (`RP23XX_ADC_DIV` false-start assumption approved!).
    - `Stop 5` (`+29.0s @ 13x`; `Q4–Q5` `#6478..#9259` & Commit tail `#13146..#14006` stripped): `5. Draft Reconciled & Promoted to v0-draft` (End).
- **Dual-Layer Live Cast Commentary (`bjm-voice`)**:
  - **`[Stop 0 — AGY Start Page (Before /egm-draft)]`**:
    > "Let's run `/egm-draft` against `@concepts_brief.md`." *(Press `p`)*
  - **`[Stop 1 — Prompt: /egm-draft @concepts_brief.md (`0.8s @ 2x`)]`**:
    > "Targeting `docs/designs/xiao-rp2350-bargraph.md`." *(Press `p`)*
  - **`[Warp Run to Stop 2 (`28.3s @ 2x`)]`**:
    > *(Voiceover during 28s warp)*: "The Elephant loads `51` contract and header files into context so every pin number, PWM slice, and Kconfig symbol in the draft is grounded."
  - **`[Stop 2 — Q1: Confirm Context & Start Drafting]`**:
    > "Context confirmed—proceed to drafting." *(Press `p`)*
  - **`[Warp Run to Stop 3 (`27.3s @ 3x`)]`**:
    > *(Voiceover during 27s warp)*: "It drafts Section 1—Overview, Goals, and Non-Goals—and pauses for human sign-off."
  - **`[Stop 3 — Q2: Section Approval — Overview & Goals]`**:
    > "Overview and Goals look spot-on. Approved." *(Press `p`)*
  - **`[Warp Run to Stop 4 (`29.1s @ 15x`)]`**:
    > *(Voiceover during 29s warp)*: "Now watch it draft the Technical Plan. It sees the `500 kHz` free-running interrupt hazard and proposes setting `CONFIG_RP23XX_ADC_DIV` to slow down hardware pacing."
  - **`[Stop 4 — Q3: Section Approval — Technical Plan (The False Start!)]`**:
    > "Here is the confession moment: I read that on screen, thought *'beauty, hardware clock divider, makes total sense,'* and hit Approve!" *(Press `p`)*
  - **`[Warp Run to Stop 5 (`29.0s @ 13x`)]`**:
    > *(Voiceover during 29s warp)*: "It finishes the remaining sections, reconciles the document, and promotes `v0-draft`."
  - **`[Stop 5 — Draft Reconciled & Promoted to v0-draft]`**:
    > "Let's pause and look at why that approval was dead wrong."

---

#### `S18` — Debrief: Author Bias, Self-Sycophancy & Why We Need Goldfish (`03-egm-journey.md` · `layout: default` · `25s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Why `CONFIG_RP23XX_ADC_DIV` Was a Landmine**
  - **`[v-click 1]`**: **The Silicon Reality** — NuttX's RP2350 ADC driver triggers conversions via single-shot `START_ONCE`, which *completely ignores* the hardware `DIV` register and still fires **`500,000 IRQ/s`**!
  - **`[v-click 2]`**: **Author Bias / Shared-Context Confirmation Bias (Dave Rensin)** — The session that spent 30 minutes co-authoring a spec will rubber-stamp its own blind spots. We need a reviewer with amnesia.
- **Verbatim Speaker Script (`bjm-voice` · `~65 words`)**:
  - **`[On Load]`**:
    > "Why was `CONFIG_RP23XX_ADC_DIV` a landmine?"
  - **`[v-click 1 — Why RP23XX_ADC_DIV Fails Under START_ONCE]`**:
    > "Because NuttX's RP2350 ADC driver triggers conversions via single-shot `START_ONCE`—which completely ignores the hardware `DIV` register and still fires **five hundred thousand interrupts per second**!"
  - **`[v-click 2 — Dave Rensin on Shared-Context Confirmation Bias]`**:
    > "As Dave Rensin warns, the costliest mistake is a plan both the agent and the human fell in love with inside the same session. Once we agreed on `ADC_DIV`, confirmation bias locked in. We needed a reviewer with amnesia."

---

#### `S19` — Unpacking The Goldfish: Stateless Adversarial Swarm (`/egm-review`) (`03-egm-journey.md` · `layout: default` · `35s` · `3` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **How `/egm-review` Defeats Author Sycophancy**
  - **`[v-click 1]`**: **1. Stateless & Uninvested** — Fresh subagent context windows that see only the spec and the repo contracts—zero memory of the brainstorm/drafting struggle.
  - **`[v-click 2]`**: **2. Multi-Angle Adversarial Personas (`146` Contract Reads)** — *Zoom-Out Architect*, *Comprehension*, *Hostile Design Critic*, *Implementation Readiness*, and *Codebase Details* audit `v0-draft` in parallel.
  - **`[v-click 3]`**: **3. Synthesizer + `/egm-resolve`** — Deduplicates findings into ranked Blockers (`B-1..B-3`) and Majors (`M-1..M-5`) for interactive human `accept`/`reject`.
- **Verbatim Speaker Script (`bjm-voice` · `~85 words`)**:
  - **`[On Load]`**:
    > "Enter `/egm-review`—the Goldfish swarm."
  - **`[v-click 1 — Stateless & Uninvested]`**:
    > "Each Goldfish runs in a brand-new subagent context window. It never saw our brainstorm, has zero sympathy for how hard we worked on `v0-draft`, and owes us zero politeness."
  - **`[v-click 2 — Multi-Angle Adversarial Personas (146 README Reads)]`**:
    > "Five specialised personas—*Zoom-Out Architect*, *Comprehension*, *Hostile Design Critic*, *Implementation Readiness*, and *Codebase Details*—audit `v0-draft` in parallel across `146` `README.md` contracts."
  - **`[v-click 3 — Synthesizer + /egm-resolve]`**:
    > "Then a Synthesizer deduplicates their findings into ranked Blockers (`B-1..B-3`) and Majors for `/egm-resolve`."

---

#### `S20` — What to Watch in `04-review.cast`: `B-1`, `/clear` Amnesia, and Pass 2 (`03-egm-journey.md` · `layout: default` · `20s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Two Crucial Beats in `04-review.cast` (`2m 21s` warped)**
  - **`[v-click 1]`**: **Beat 1 — Pass 1 Catches Blocker `B-1`**: Goldfish busts `RP23XX_ADC_DIV` cold and replaces it with NVIC IRQ gating around a 5-sample burst.
  - **`[v-click 2]`**: **Beat 2 — `/clear` Amnesia + Pass 2 Catches `CA-01`**: Wiping session memory with `/clear` before Pass 2 exposes a brand-new deadlock *introduced by Pass 1's fix*!
- **Verbatim Speaker Script (`bjm-voice` · `~50 words`)**:
  - **`[On Load]`**:
    > "Two things to watch in `04-review.cast`:"
  - **`[v-click 1 — Pass 1 Catches B-1]`**:
    > "First, watch Pass 1 immediately flag `RP23XX_ADC_DIV` as Blocker `B-1` and replace it with NVIC IRQ burst gating."
  - **`[v-click 2 — /clear Amnesia + Pass 2 Catches CA-01]`**:
    > "Second, watch us run `/clear` to wipe session memory cold—so Pass 2 catches a brand-new bug *introduced by Pass 1's fix*!"

---

#### `S21` — LIVE CAST 5: `04-review.cast` (`03-egm-journey.md` · `layout: cast-full` · `3m 39s` stage = `2m 21s` play + `1m 18s` stops)
- **Visual & Player Specification**:
  - **Component**: `<AsciinemaPlayer src="./casts/04-review.cast" />` (backed by `prototypes/04-review-cast-player.html`).
  - **Stop Points (`13` stops · Starts on AGY Start Page · Items `2–12` & Pass 2 resolve tail trimmed · `interactive` & `accept` tightened to `0.5s`)**:
    - `0. Start` (`0.0s`): Settled on **AGY Start Page** (`Ready for Pass 1 /egm-review`; shell launch `#0000..#0036` stripped).
    - `Stop 1` (`+0.6s @ 2x`): `1. Prompt: Pass 1 /egm-review` (After Prompt 1, paused before `Enter`).
    - `Stop 2` (`+8.0s @ 2x`): `2. Pass 1 Q1: Select Review Execution Mode (Consolidated)` (`Consolidated` highlighted).
    - `Stop 3` (`+29.7s @ 39x` — compresses `21m 33s` raw, `146` README reads): `3. Pass 1 Review Complete — Ready for /egm-resolve` (Before Prompt 2).
    - `Stop 4` (`+0.4s @ 2x`): `4. Prompt: Pass 1 /egm-resolve` (After Prompt 2, paused before `Enter`).
    - `Stop 5` (`+26.8s @ 6x`): `5. Pass 1 Audit Findings Compiled — Choose Resolution Mode` (Before Reply 1).
    - `Stop 6` (`+0.5s @ 2x`, tightened from `2.9s` raw): `6. Reply: interactive (Walk Through 12 Findings)` (After Reply 1).
    - `Stop 7` (`+17.1s @ 2x`): `7. Item 1/12 (B-1): NVIC IRQ Gating vs. RP23XX_ADC_DIV` (Before Reply 2).
    - `Stop 8` (`+0.5s @ 2x`, tightened from `3.0s` raw): `8. Reply: accept (Accept Item 1)` (After Reply 2).
    - `Stop 9` (`+5.0s @ 2x`; items `2–12` & Pass 1 archive `#14020..#26319` stripped): `9. Pass 1 Resolved — Ready for /clear` (Before `/clear`).
    - `Stop 10` (`+0.4s @ 2x`): `10. Run /clear` (After `/clear`, paused before `Enter`).
    - `Stop 11` (`+0.1s @ 2x`): `11. Session Cleared — Ready for Pass 2 /egm-review` (Before Prompt 3).
    - `Stop 12` (`+0.6s @ 2x`): `12. Prompt: Pass 2 /egm-review (with GoldfishContext)` (After Prompt 3, paused before `Enter`).
    - `Stop 13` (`+29.0s @ 26x`; Pass 2 resolve/commit tail `#33141..#37820` stripped): `13. Pass 2 Review Complete — 11 Secondary Findings (CA-01 Short Read)` (End).
- **Dual-Layer Live Cast Commentary (`bjm-voice`)**:
  - **`[Stop 0 — AGY Start Page (Before Pass 1 /egm-review)]`**:
    > "Let's unleash the Pass 1 Goldfish swarm on `v0-draft`." *(Press `p`)*
  - **`[Stop 1 — Prompt: Pass 1 /egm-review (`0.6s @ 2x`)]`**:
    > "Running `/egm-review @docs/designs/xiao-rp2350-bargraph.md`." *(Press `p`)*
  - **`[Stop 2 — Pass 1 Q1: Select Review Execution Mode (Consolidated) (`8.0s @ 2x`)]`**:
    > "We choose Consolidated mode to fan out the Goldfish reviewers in parallel." *(Press `p`)*
  - **`[Warp Run to Stop 3 (`29.7s @ 39x` — 21m 33s raw, 146 README reads)]`**:
    > *(Voiceover during 30s warp)*: "Twenty-one raw minutes compressed at thirty-nine times speed. Watch the parallel subagents fan out across `146` contracts and driver files, cross-checking every claim in `v0-draft` against the actual RP2350 silicon registers."
  - **`[Stop 3 — Pass 1 Review Complete — Ready for /egm-resolve]`**:
    > "Pass 1 found `3` Blockers and `5` Majors. Let's run `/egm-resolve`." *(Press `p`)*
  - **`[Stop 4 — Prompt: Pass 1 /egm-resolve (`0.4s @ 2x`)]`**:
    > "Starting interactive resolution." *(Press `p`)*
  - **`[Stop 5 — Pass 1 Audit Findings Compiled — Choose Resolution Mode (`26.8s @ 6x`)]`**:
    > "It asks how we want to work through the `12` findings." *(Press `p`)*
  - **`[Stop 6 — Reply: interactive (`0.5s @ 2x`)]`**:
    > "Type `interactive` so we inspect each blocker one by one." *(Press `p`)*
  - **`[Stop 7 — Item 1/12 (B-1): NVIC IRQ Gating vs. RP23XX_ADC_DIV (`17.1s @ 2x`)]`**:
    > "Look at Item `1/12` (`B-1`): the Goldfish busted our `RP23XX_ADC_DIV` assumption cold and recommends gating the NVIC IRQ around a 5-sample burst!" *(Press `p`)*
  - **`[Stop 8 — Reply: accept (Accept Item 1) (`0.5s @ 2x`)]`**:
    > "We type `accept` to adopt the 5-sample burst fix." *(Press `p`)*
  - **`[Stop 9 — Pass 1 Resolved — Ready for /clear (`5.0s @ 2x`)]`**:
    > "Now all Pass 1 items are patched into the spec. Watch this next move: before Pass 2, we wipe our own session memory." *(Press `p`)*
  - **`[Stop 10 — Run /clear (`0.4s @ 2x`)]`**:
    > "Running `/clear` so the agent forgets every justification from Pass 1." *(Press `p`)*
  - **`[Stop 11 — Session Cleared — Ready for Pass 2 (`0.1s @ 2x`)]`**:
    > "Clean slate. Now we launch Pass 2." *(Press `p`)*
  - **`[Stop 12 — Prompt: Pass 2 /egm-review (with GoldfishContext) (`0.6s @ 2x`)]`**:
    > "Running Pass 2 `/egm-review` cold against the updated spec." *(Press `p`)*
  - **`[Warp Run to Stop 13 (`29.0s @ 26x`)]`**:
    > *(Voiceover during 29s warp)*: "Watch Pass 2 inspect the 5-sample burst fix we *just* accepted in Pass 1. Because these Goldfish have zero memory of Pass 1, they calculate `5 samples × 5 bytes = 25 bytes` against NuttX's `8-sample` software FIFO—and spot a deadlock!"
  - **`[Stop 13 — Pass 2 Review Complete — Catches CA-01]`**:
    > "Boom—`CA-01` caught in Markdown before a single line of C was written."

---

#### `S22` — Debrief: First-Order Catch (Pass 1) vs. Second-Order Catch (Pass 2) (`03-egm-journey.md` · `layout: default` · `30s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Why `/clear` Between Review Passes Matters**
  - **`[v-click 1]`**: **First-Order Catch (Pass 1 `B-1`)** — Replaced the `500 kHz` interrupt storm with a 5-sample NVIC IRQ-gated burst (discard `1` stale sample + average `4`).
  - **`[v-click 2]`**: **Second-Order Catch (Pass 2 `CA-01` after `/clear`)** — NuttX's software ADC FIFO holds `8` samples (`7` usable before full). Reading `5` samples (`25 bytes`) leaves `2` in the queue, so the second `read(25)` blocks forever at `7`! Fixed by reading `20 bytes` (`4` samples) per burst.
- **Verbatim Speaker Script (`bjm-voice` · `~75 words`)**:
  - **`[On Load]`**:
    > "That two-pass loop is my favourite moment in the entire project."
  - **`[v-click 1 — First-Order Catch: Pass 1 B-1]`**:
    > "Pass 1 (`B-1`) killed the `500 kHz` interrupt storm by gating the NVIC interrupt around a 5-sample burst—discarding one stale sample and averaging four."
  - **`[v-click 2 — Second-Order Catch: Pass 2 CA-01 after /clear]`**:
    > "Wait: NuttX's software ADC FIFO holds `8` samples (`7` usable before full). Reading `5` samples (`25 bytes`) leaves `2` in the queue, so the second `read(25)` blocks forever at `7`! `/clear` gave Pass 2 fresh eyes to catch the bug introduced by Pass 1's fix."

---

#### `S23` — When the Spec is Hardened, Coding is Boring (`05-implement` Setup) (`03-egm-journey.md` · `layout: default` · `20s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Phase 5 Setup — Implementing from the Hardened Spec (`05-implement.cast`)**
  - **`[v-click 1]`**: **`4` Blockers + `7` Majors Eliminated in Markdown** — Every register quirk, `20-byte` FIFO read size, and non-ADC pin assignment is locked before writing C.
  - **`[v-click 2]`**: **What to Watch (`58s` warped)** — `100%` host unit tests pass, `nuttx/nuttx.uf2` (`347K`) compiles on the **first attempt (`0` retries)**, and `9` new `README.md` contracts are written to keep the Elephant in sync.
- **Verbatim Speaker Script (`bjm-voice` · `~50 words`)**:
  - **`[On Load]`**:
    > "Once you eliminate four Blockers and seven Majors in Markdown, coding becomes wonderfully boring."
  - **`[v-click 1]`**:
    > "Every register quirk, FIFO read size (`20 bytes`), and pin assignment is already locked."
  - **`[v-click 2]`**:
    > "Watch `05-implement.cast` pass `100%` of host tests and build `nuttx.uf2` on the **very first attempt (`0` retries)**—plus write `9` new `README.md` contracts."

---

#### `S24` — LIVE CAST 6: `05-implement.cast` (`03-egm-journey.md` · `layout: cast-full` · `1m 22s` stage = `0m 58s` play + `24s` stops)
- **Visual & Player Specification**:
  - **Component**: `<AsciinemaPlayer src="./casts/05-implement.cast" />` (backed by `prototypes/05-implement-cast-player.html`).
  - **Stop Points (`4` stops · Starts on AGY Start Page · Commit tail trimmed)**:
    - `0. Start` (`0.0s`): Settled on **AGY Start Page** (`Ready for Implementation Prompt`; shell launch `#0000..#0052` stripped).
    - `Stop 1` (`+0.5s @ 2x`): `1. Prompt: Implement Firmware & Verify Build` (After Prompt 1, paused before `Enter`).
    - `Stop 2` (`+29.0s @ 27x` — compresses `13m 04s` raw): `2. Firmware Built & Tested — Ready for Wiring Prompt` (Before Prompt 2; `100%` host tests + 1st-attempt `nuttx.uf2` build).
    - `Stop 3` (`+0.5s @ 2x`): `3. Prompt: Generate Breadboard Wiring Guide & SVG` (After Prompt 2, paused before `Enter`).
    - `Stop 4` (`+28.2s @ 11x`; Commit tail `#12402..#14101` stripped): `4. Breadboard Wiring & SVG Complete` (End).
- **Dual-Layer Live Cast Commentary (`bjm-voice`)**:
  - **`[Stop 0 — AGY Start Page (Before Firmware Prompt)]`**:
    > "Let's hand the hardened `v1` spec to Antigravity." *(Press `p`)*
  - **`[Stop 1 — Prompt: Implement Firmware from Approved Spec (`0.5s @ 2x`)]`**:
    > "Prompting it to implement `xiao-rp2350-bargraph.md` and build `nuttx.uf2`." *(Press `p`)*
  - **`[Warp Run to Stop 2 (`29.0s @ 27x` — 13m 04s raw)]`**:
    > *(Voiceover during 29s warp)*: "At twenty-seven times speed: it wires the out-of-tree board overlays, writes `bargraph_main.c` and the pure math core, passes `6/6` host unit test suites first go, compiles `nuttx.uf2` (`347K`) with zero retries, and updates `9` directory `README.md` contracts so the Elephant stays in sync."
  - **`[Stop 2 — Firmware, Unit Tests & nuttx.uf2 Built — Ready for Wiring Prompt]`**:
    > "First-try firmware build complete! Now let's ask for the breadboard wiring guide and SVG." *(Press `p`)*
  - **`[Stop 3 — Prompt: Breadboard Wiring Guide & SVG (`0.5s @ 2x`)]`**:
    > "Generating `docs/egm/breadboard-wiring.svg` from our verified pin table." *(Press `p`)*
  - **`[Warp Run to Stop 4 (`28.2s @ 11x`)]`**:
    > *(Voiceover during 28s warp)*: "Because the spec already locked the mechanical footprint and non-ADC pins, it generates a physically buildable breadboard layout in one pass."
  - **`[Stop 4 — Breadboard Wiring Guide & SVG Complete]`**:
    > "Let's compare the physical breadboard result."

---

#### `S24a` — EGM's Generated Breadboard Wiring (`03-egm-journey.md` · `layout: default` · `20s` · `0` `v-click`s)
- **Visual Specification**:
  - **Full-Width Diagram**: `./images/breadboard-wiring.svg` displaying the complete light-mode breadboard schematic generated by EGM (`docs/egm/breadboard-wiring.svg`).
- **Verbatim Speaker Script (`bjm-voice` · `~50 words`)**:
  - **`[On Load]`**:
    > "Here is the full breadboard wiring SVG that EGM generated in one pass. Notice the clean light-mode schematic layout, the XIAO RP2350 seated across the centre trench, the potentiometer on `D0`, and the six colour-coded LEDs on `D1`, `D2`, `D4`, `D5`, `D8`, and `D9`. Now let's zoom in on the XIAO RP2350 itself to verify the physical footprint and pin routing."

---

#### `S25` — Breadboard Reality Check: Electrical Wins vs. Physical Footprint (`0.6"` XIAO) (`03-egm-journey.md` · `layout: hardware-callout` · `35s` · `2` `v-click`s)
- **Visual Specification**:
  - **Left Pane (`hardware-callout` SVG)**: `./images/breadboard-wiring-crop-ef.svg` (cropped close-up of `docs/egm/breadboard-wiring.svg` showing the XIAO RP2350 drawn across Columns E & F with a visually widened `0.3"` centre trench).
  - **Right Pane**:
    - **`[v-click 1]`**: **Verified Electrical & Pinout Wins** — Routes LEDs to `6` non-ADC PWM pins (`D1, D2, D4, D5, D8, D9`), leaves `5V` unconnected, budgets `23.6 mA` with `330 Ω` resistors, and disables digital input buffer `IE=0` on `GPIO 26` to defeat Erratum `RP2350-E9`.
    - **`[v-click 2]`**: **Physical Footprint Catch (`0.3"` vs. `0.6"` XIAO Width)** — Both runs missed the `0.6"` (`15.24 mm`) XIAO header spacing (`Cols C & G` or `D & H`): Vanilla spanned `0.7"` (`Cols C–H`), while EGM seated it in `Cols E & F (0.3")`—masked in the SVG by widening the visual trench.
- **Verbatim Speaker Script (`bjm-voice` · `~115 words`)**:
  - **`[On Load]`**:
    > "Zooming in on `docs/egm/breadboard-wiring.svg` gives us a great engineering honesty check."
  - **`[v-click 1 — Verified Electrical & Pinout Wins]`**:
    > "On the electrical side, EGM nailed it: six non-ADC PWM pins (`D1, D2, D4, D5, D8, D9`), `330`-ohm resistors keeping total draw at `23.6` milliamps, `5V` left unconnected, and `IE` disabled on `GPIO 26` to defeat Erratum `RP2350-E9`."
  - **`[v-click 2 — Physical Footprint Catch (0.3" vs. 0.6" XIAO Width)]`**:
    > "But look at the physical footprint: a XIAO RP2350's headers are `0.6` inches apart—meaning it needs Columns `C` and `G`, or `D` and `H`. Vanilla stretched it too wide to Columns `C` and `H` (`0.7` inches), while EGM seated it too narrow in Columns `E` and `F` (`0.3` inches) and hid the mismatch by drawing a wider centre trench in the SVG. Contracts caught every silicon and RTOS invariant, but mechanical dimensions still deserve a real-world check."

---

#### `S26` — Candid Scorecard Callback: `5 / 10` vs. `10 / 10` (And Engineering Honesty) (`03-egm-journey.md` · `layout: default` · `30s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **Experiment A vs. Experiment B — The Bench Scorecard**
  - **`[v-click 1]`**: **Primary 10-Trap Scorecard**: Vanilla Vibe **`5 / 10`** vs. EGM **`10 / 10`** (`0` hardware/RTOS traps hit, `0` build retries).
  - **`[v-click 2]`**: **Candid Strict Audit (`6 Pass / 4 Partial / 0 Fail`)** — A cold post-build Goldfish audit against the finished C code still flagged `4` secondary polish items (`SIGTERM` cleanup, Makefile test scaffolding)—proving adversarial review is an ongoing engineering habit, not magic.
- **Verbatim Speaker Script (`bjm-voice` · `~75 words`)**:
  - **`[On Load]`**:
    > "Let's look at the scorecard—and keep ourselves honest."
  - **`[v-click 1 — Primary 10-Trap Scorecard: 5/10 vs 10/10]`**:
    > "Across our ten hardware and RTOS traps, Vanilla Vibe scored **five out of ten**; the EGM workflow scored **ten out of ten** with zero build retries."
  - **`[v-click 2 — Candid Strict Audit: 6 Pass / 4 Partial / 0 Fail]`**:
    > "But I promised no unblemished triumph narratives: when I ran a cold post-build Goldfish audit against every line of C, it still flagged **four partial polish items** like `SIGTERM` cleanup—proving adversarial review is an ongoing habit, not magic."

---

### Act IV — Finale: Rolling Docs for the Human & Live Lesson Showcase (`slides/acts/04-teach-code-finale.md` · `7` Slides · `5m 58s`)

---

#### `S27` — Act IV Finale: From "Docs for the AI" to "Docs for the Human" (`04-teach-code-finale.md` · `layout: default` · `35s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **The Final Boss of AI Engineering: Comprehension Debt**
  - **`[v-click 1]`**: **Dave Rensin's Third Rule — *"Humans Own the Outcomes"* **: The AI scored `10/10` and shipped `872` lines of `bargraph_main.c`—but if a wire pops loose in six months, *you* have to debug it.
  - **`[v-click 2]`**: **Rolling Docs for the Human (`/teach-code`)**: Adapted from **Matt Pocock's** open-source `/teach` skill (using `/agy-customizations`) and installed via `npx skills add domesticmouse/skills`—turning our `README.md` contracts, design spec, and Goldfish reports into a 6-lesson **Edward Tufte** interactive HTML course.
- **Verbatim Speaker Script (`bjm-voice` · `~85 words`)**:
  - **`[On Load]`**:
    > "That brings us to Act IV—and the biggest trap in AI-assisted engineering."
  - **`[v-click 1 — Comprehension Debt & Rensin's Third Rule]`**:
    > "The AI just scored `10/10` and shipped `872` lines of `bargraph_main.c`. Brilliant—except Dave Rensin's third rule says **'Humans Own the Outcomes.'** If a wire pops loose on the bench in six months, *you* have to debug that firmware."
  - **`[v-click 2 — Matt Pocock's /teach → /teach-code]`**:
    > "To pay down **comprehension debt**, I adapted **Matt Pocock's** brilliant `/teach` skill (using `/agy-customizations`) into **`/teach-code`**—turning our `README.md` contracts, design spec, and Goldfish reports into a six-lesson interactive **Edward Tufte** HTML course."

---

#### `S28` — LIVE CAST 7: `06-teach-code.cast` (`04-teach-code-finale.md` · `layout: cast-full` · `2m 04s` stage = `1m 10s` play + `54s` stops)
- **Visual & Player Specification**:
  - **Component**: `<AsciinemaPlayer src="./casts/06-teach-code.cast" />` (backed by `prototypes/06-teach-code-cast-player.html`).
  - **Stop Points (`9` stops · Keeps opening `npx skills add` CLI install `Stops 0–4` $\rightarrow$ AGY Start Page `Stop 5`)**:
    - `0. Start` (`0.0s`): Shell prompt ready to install the custom skill (`Before npx skills add`).
    - `Stop 1` (`+0.7s @ 2x`): `1. Run npx skills add domesticmouse/skills` (After Command, paused before `Enter`).
    - `Stop 2` (`+4.5s @ 2x`): `2. Select teach-code Skill` (Interactive picker paused on `teach-code`).
    - `Stop 3` (`+1.5s @ 2x`): `3. Confirm Project Installation Scope` (`.agents/skills/teach-code`).
    - `Stop 4` (`+0.7s @ 2x`): `4. teach-code Installed` (Shell install complete).
    - `Stop 5` (`+2.2s @ 2x`): `5. AGY Start Page — Ready for /teach-code Prompt` (Before Prompt 1).
    - `Stop 6` (`+0.5s @ 2x`): `6. Prompt: /teach-code Story Arc in ./tutorial/` (After Prompt 1, paused before `Enter`).
    - `Stop 7` (`+29.8s @ 79x` — compresses `39m 11s` raw): `7. 6-Lesson Course Generated — Ready for Verify Prompt` (Before Prompt 2).
    - `Stop 8` (`+0.5s @ 2x`): `8. Prompt: Verify sync_listing.py, build.py & Commit` (After Prompt 2, paused before `Enter`).
    - `Stop 9` (`+29.1s @ 5x`): `9. Tutorial Course Verified & Committed` (End).
- **Dual-Layer Live Cast Commentary (`bjm-voice`)**:
  - **`[Stop 0 — Shell Prompt (Before npx skills add)]`**:
    > "This recording starts in the terminal so you can see how `teach-code` installs dynamically." *(Press `p`)*
  - **`[Stop 1 — Run npx skills add domesticmouse/skills (`0.7s @ 2x`)]`**:
    > "Running `npx skills add domesticmouse/skills`." *(Press `p`)*
  - **`[Stop 2 — Select teach-code Skill (`4.5s @ 2x`)]`**:
    > "We select `teach-code` from the repository." *(Press `p`)*
  - **`[Stop 3 — Confirm Project Installation Scope (`1.5s @ 2x`)]`**:
    > "Target Antigravity in the project workspace." *(Press `p`)*
  - **`[Stop 4 — teach-code Installed (`0.7s @ 2x`)]`**:
    > "Skill installed. Now let's launch Antigravity." *(Press `p`)*
  - **`[Stop 5 — AGY Start Page — Ready for /teach-code Prompt (`2.2s @ 2x`)]`**:
    > "Now we're inside Antigravity. Let's ask `/teach-code` to build the full six-lesson course." *(Press `p`)*
  - **`[Stop 6 — Prompt: /teach-code Story Arc in ./tutorial/ (`0.5s @ 2x`)]`**:
    > "Prompting `/teach-code` to turn our specs, contracts, and Goldfish catches into six self-contained HTML lessons with runnable C99 companion apps." *(Press `p`)*
  - **`[Warp Run to Stop 7 (`29.8s @ 79x` — compressing 39m 11s raw)]`**:
    > *(Voiceover during 30s warp)*: "We're watching thirty-nine raw minutes at seventy-nine times speed. It maps the learner's Zone of Proximal Development, builds six runnable C99 companion apps in `tutorial/apps/0001` through `0006` under `-Wall -Wextra -Werror -pedantic`, crafts three interactive JavaScript simulators and four printable reference sheets, and compiles the Tufte HTML lessons."
  - **`[Stop 7 — 6-Lesson Course Generated — Ready for Verify Prompt]`**:
    > "All six lessons generated. Now let's run the zero-divergence verification check." *(Press `p`)*
  - **`[Stop 8 — Prompt: Verify sync_listing.py, build.py & Commit (`0.5s @ 2x`)]`**:
    > "Running `sync_listing.py --check` and `build.py --check`." *(Press `p`)*
  - **`[Warp Run to Stop 9 (`29.1s @ 5x`)]`**:
    > *(Voiceover during 29s warp)*: "It verifies that every C snippet inside the Markdown lessons is byte-for-byte identical to the compiled C99 test binaries."
  - **`[Stop 9 — Tutorial Course Verified & Committed]`**:
    > "Zero drift. Now let's tour the compiled interactive course!"

---

#### `S29` — FINALE SHOWCASE 1: Course Hub & 6-Lesson Story Arc (`docs/egm/tutorial/index.html`) (`04-teach-code-finale.md` · `layout: default` · `45s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: Header with clickable launch badge `<a href="./tutorial/index.html" target="_blank">Open ./tutorial/index.html ↗</a>` + visual preview of the compiled Edward Tufte course hub.
  - **`[v-click 1]`**: **Figure 0.1 Mermaid Story Arc (`Lesson 0001` $\rightarrow$ `Lesson 0006`)**:
    - `0001` Breadboard Geometry & `23.6 mA` Current Budget $\rightarrow$ `0002` 16-Bit Proportional PWM Scaling $\rightarrow$ `0003` `>> 19` Bit-Shift & Q4 Fixed-Point EMA $\rightarrow$ `0004` Multi-Channel PWM State $\rightarrow$ `0005` Out-of-Tree NuttX BSP & NVIC IRQ Gating $\rightarrow$ `0006` `50 Hz` `bargraphd` Daemon & NSH CLI.
  - **`[v-click 2]`**: **Zero-Divergence C99 Verification Gate** — Runnable companion apps in `tutorial/apps/0001..0006/` compiled under `-std=c99 -Wall -Wextra -Werror -pedantic` and verified byte-for-byte via `sync_listing.py --check` + `build.py --check`.
- **Verbatim Speaker Script (`bjm-voice` · `~105 words`)**:
  - **`[On Load]`**:
    > "Here is the compiled course landing page at `docs/egm/tutorial/index.html`—and we can pop it open directly from the slide badge."
  - **`[v-click 1 — Tufte Landing Page & Figure 0.1 Mermaid Story Arc]`**:
    > "Notice the clean **Edward Tufte** typography and the **Figure 0.1 Mermaid story arc**: it walks a human engineer from `Lesson 0001` (breadboard geometry and the `23.6 mA` current budget) through PWM scaling, Q4 EMA filtering, and out-of-tree NuttX BSP bring-up, all the way to `Lesson 0006` (`bargraphd` and the NSH CLI)."
  - **`[v-click 2 — Zero-Divergence C99 Gate]`**:
    > "Crucially, tutorials usually rot the moment code changes. Here, `sync_listing.py --check` guarantees every lesson snippet compiles cleanly under strict C99 (`-Wall -Wextra -Werror -pedantic`)."

---

#### `S30` — FINALE SHOWCASE 2: Interactive JS Simulators, Fill-in-the-Blank C99 & Quizzes (`Lesson 0001–0003`) (`04-teach-code-finale.md` · `layout: default` · `60s` · `3` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: Header — **Active Recall & Tactile Hardware Simulation Inside the Browser** (with direct links to `./tutorial/lessons/0001-breadboard-circuit-and-current-budget.html`, `0002-proportional-bargraph-math-and-pwm-scaling.html`, and `0003-q4-ema-filtering-and-adc-extraction.html`).
  - **`[v-click 1]`**: **Live Hardware & PWM Simulators (`Lesson 0001` & `0002`)** — Drag the `0..3300 mV` wiper in `breadboard-simulator` (watching per-pin mA and `Cols E & F` geometry) and scrub 6-channel `0..65535` duty cycles in `pwm-segment-visualizer`.
  - **`[v-click 2]`**: **Turning Goldfish Catches into Interactive Play (`Lesson 0003` `adc-ema-simulator`)** — Toggle *"Discard stale `msgs[0]`"* (`CA-01`) and flip `>> 2` vs `(ema_q4 + 8) >> 4` (`B-2`) to watch the bar graph peg at a quarter-turn of the knob!
  - **`[v-click 3]`**: **Tufte Sidenotes, Fill-in-the-Blank C99 & Equal-Length Quizzes** — Right-margin sidenotes, Prism `keep-markup` C99 exercises, and Matt Pocock's retrieval quizzes where every option has the exact same word count (`10 words/option`).
- **Verbatim Speaker Script (`bjm-voice` · `~140 words`)**:
  - **`[On Load]`**:
    > "Inside the lessons, you don't just passively skim prose—you play with the hardware physics and the exact bugs the Goldfish caught."
  - **`[v-click 1 — Live Hardware & PWM Simulators (Lessons 0001 & 0002)]`**:
    > "In `Lesson 0001`'s `breadboard-simulator`, you drag the `0` to `3300 mV` potentiometer wiper and watch per-pin milliamps update live; in `Lesson 0002`'s `pwm-segment-visualizer`, you scrub across all six `16-bit` (`0..65535`) PWM duty cycles."
  - **`[v-click 2 — Turning Goldfish Catches into Interactive Play (Lesson 0003)]`**:
    > "Better yet, look at `Lesson 0003`'s `adc-ema-simulator`: you can toggle a checkbox for Pass 2's **`CA-01` stale `msgs[0]` sample** or flip Pass 1's **`B-2` `>> 2` vs `>> 4` shift bug** and watch the LED bar graph peg at a quarter-turn of the knob!"
  - **`[v-click 3 — Tufte Sidenotes, Fill-in-the-Blank C99 & Equal-Length Quizzes]`**:
    > "Right alongside sit Tufte margin sidenotes, fill-in-the-blank C99 exercises, and Matt Pocock's retrieval quizzes where every multiple-choice option is forced to the exact same word count (`10 words`) so you can't guess the longest answer."

---

#### `S31` — FINALE SHOWCASE 3: Teaching Dead Ends (`Lesson 0005`), Printable Bench Sheets & Bonus Bug Catch! (`04-teach-code-finale.md` · `layout: default` · `45s` · `2` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: Header — **Why Teaching the Code Makes the Engineering Better** (with links to `./tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.html` and `./tutorial/reference/nuttx-rp2350-driver-traps.html`).
  - **`[v-click 1]`**: **Dead Ends as Curriculum + Printable `@media print` A4 Bench Sheets** — `Lesson 0005` explicitly teaches *why* `CONFIG_RP23XX_ADC_DIV` failed under `START_ONCE`, paired with `4` single-page A4 reference sheets (`nuttx-rp2350-driver-traps.md`) formatted for the workbench.
  - **`[v-click 2]`**: **The Bonus Twist — Docs for Humans Catch Firmware Bugs Too!** — Compiling `Lesson 0006`'s strict C99 `pthreads` harness caught **2 real bugs** missed in Phase 5 (including `bargraph test` forgetting to set `MANUAL` mode so the `50 Hz` loop clobbered the LED sweep after `20 ms`).
- **Verbatim Speaker Script (`bjm-voice` · `~105 words`)**:
  - **`[On Load]`**:
    > "Two final touches in the course really surprised me."
  - **`[v-click 1 — Dead Ends as Curriculum + Printable A4 Bench Sheets]`**:
    > "First, `Lesson 0005` doesn't hide our `RP23XX_ADC_DIV` false start—it teaches *why* `START_ONCE` ignores the hardware divider, paired with four `@media print` A4 workbench reference sheets (`nuttx-rp2350-driver-traps.md`) you can print out and tape beside your oscilloscope."
  - **`[v-click 2 — The Bonus Twist: Docs for Humans Catch Firmware Bugs Too!]`**:
    > "Second, the bonus twist: when `/teach-code` compiled `Lesson 0006`'s strict C99 `pthreads` harness to teach the daemon state machine, the tutorial compiler caught **two real bugs** we missed in Phase 5—including `bargraph test` forgetting to set `MANUAL` mode!"

---

#### `S32` — The "Roll Your Own Docs" Flywheel: Three Habits for Monday Morning (`04-teach-code-finale.md` · `layout: default` · `30s` · `3` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: **The "Roll Your Own Docs" Flywheel**
  - **`[v-click 1]`**: **1. Feed the Elephant First (`Docs for the AI`)** — Maintain directory-level `README.md` contracts (`/egm-bootstrap` + `/egm-drift-report`) so agents navigate architecture instead of grepping blind.
  - **`[v-click 2]`**: **2. Unleash Stateless Goldfish Before Coding** — Audit specs with fresh-context subagents (`/egm-review` + `/clear`) to catch second-order hardware traps in Markdown.
  - **`[v-click 3]`**: **3. Pay Down Comprehension Debt Immediately (`Docs for the Human`)** — Run `/teach-code` so a `10/10` AI build becomes a `10/10` mental model in your own head.
- **Verbatim Speaker Script (`bjm-voice` · `~70 words`)**:
  - **`[On Load]`**:
    > "That's the **Roll Your Own Docs** flywheel—three habits you can steal on Monday morning:"
  - **`[v-click 1 — 1. Feed the Elephant First]`**:
    > "**One: Feed the Elephant first.** Maintain directory-level `README.md` contracts (`/egm-bootstrap` and `/egm-drift-report`) so your agent navigates architecture instead of grepping blind."
  - **`[v-click 2 — 2. Unleash Stateless Goldfish Before Coding]`**:
    > "**Two: Unleash stateless Goldfish before coding.** Use `/egm-review` and `/clear` to catch second-order hardware traps in Markdown."
  - **`[v-click 3 — 3. Pay Down Comprehension Debt Immediately]`**:
    > "**Three: Pay down comprehension debt immediately.** Run `/teach-code` so a `10/10` AI build becomes a `10/10` mental model in your own head."

---

#### `S33` — Links, Open-Source Skills & Interactive Course (`04-teach-code-finale.md` · `layout: default` · `19s` · `0` `v-click`s)
- **Visual Specification**:
  - **`[On Load]`**: Resource Cards & Links:
    - **Repository & Slidev Deck**: `github.com/domesticmouse/nuttxspace`
    - **Elephant-Goldfish Model (`egm`)**: Credit **Dave Rensin (`@drensin`)** · `/plugin` in Antigravity
    - **`/teach-code` Skill**: Credit **Matt Pocock (`/teach`)** · `npx skills add domesticmouse/skills`
    - **Live 6-Lesson Interactive Course**: `<a href="./tutorial/index.html" target="_blank">/nuttxspace/tutorial/index.html ↗</a>`
- **Verbatim Speaker Script (`bjm-voice` · `~45 words`)**:
  - **`[On Load]`**:
    > "Everything you saw tonight—the `nuttxspace` repo, Dave Rensin's `egm` plugin, the `/teach-code` skill (`npx skills add domesticmouse/skills`), all seven terminal casts, and the live six-lesson interactive course—is open source at these links. Thanks heaps for listening, and I'd love to take your questions!"

---

## 5. Verification & Audit Summary

1. **Timing Budget Verification (`PASS`)**:
   - **Non-Cast Slides (`31` slides)**: `280s` (Act I) + `110s` (Act II) + `375s` (Act III) + `234s` (Act IV, incl. `150s` across `S29`–`S31` Finale Showcase) = **`999s` (`16m 39s`)**.
   - **Live Cast Slides (`7` slides, `53` stops)**: `624.0s` (`10m 24s`) warped playback + `318.0s` (`5m 18s`) stop-point pause budget = **`942s` (`15m 42s`)**.
   - **Combined Presentation Total**: `999s + 942s = 1,941s` (**`32m 21s`** across `38` slides, compressible to `30m 00s` by trimming `~2m 21s` from pause budgets and framing commentary).
2. **Asset & Cast Reference Verification (`PASS`)**:
   - All `7` `.cast` files verified in `slides/public/casts/` (`vanilla-vibe.cast`, `01-bootstrap.cast`, `02-brainstorm.cast`, `03-draft.cast`, `04-review.cast`, `05-implement.cast`, `06-teach-code.cast`) and `7` standalone HTML players verified in `prototypes/`.
   - All `8` image/SVG assets verified in `slides/public/images/` (`nuttx-logo.png`, `antigravity-lockup.svg`, `breadboard-scopes.svg`, `assets-breadboard-wiring.svg`, `breadboard_circuit.svg`, `breadboard-vanilla-crop-ch.svg`, `breadboard-wiring.svg`, and `breadboard-wiring-crop-ef.svg`).
   - Compiled 6-lesson interactive HTML course verified in `docs/egm/tutorial/` and `slides/public/tutorial/index.html` (`lessons/0001..0006.html` and `reference/*.html`).
3. **Scope & Standing Preference Verification (`PASS`)**:
   - Zero low-level C driver lines or register manipulation code on slides.
   - Zero asciinema/`pyte` internal tooling mechanics in the presentation.
   - Low-density slide rule (`~1.8` progressive `v-click` beats per non-cast slide; `0`–`3` clicks max) enforced across all `31` non-cast slides.
   - Finale showcase (`S29`–`S31`) prominently tours the generated `/teach-code` interactive HTML lessons at `./tutorial/index.html`.
