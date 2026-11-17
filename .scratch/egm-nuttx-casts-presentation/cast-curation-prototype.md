# Cast Curation, Trim & Stop-Point Prototype (All 7 Recordings Live)

> **Prototype for Ticket 03**: [Curate the Sequence of Cast Clips and Stop Points Across All Acts](./issues/03-curate-cast-recording-segments.md)
> **Interactive Planner**: Open [`cast-curation-prototype.html`](./cast-curation-prototype.html) in a browser to toggle individual prompt/turn blocks (`Keep Pair`, `Trim Turn`) and watch the 30-minute budget update live.

---

## 1. Three Core Stage Rules for All 7 Recordings

1. **Casts 1–6 Start at the AGY Start Page (`0. Start` = AGY Start Page); Cast 7 (`06-teach-code`) Starts with the `npx skills add` CLI Install**:
   - In `vanilla-vibe` and `01-bootstrap` through `05-implement`, strip the initial shell `agy` launch sequence (`1. Launch agy`) so each player loads on stage already sitting on the settled **Antigravity CLI start screen**, ready for the first prompt (`0. Start` serves as the "Before-Prompt" stop point for Prompt 1).
   - In `06-teach-code`, **keep the opening `npx skills add domesticmouse/skills` sequence** in full (since dynamically installing the custom skill is a key stage beat), then transition straight to the AGY Start Page (`Ready for /teach-code Prompt`).

2. **Strict Before-Prompt & After-Prompt Stop Points for Every Prompt / Reply**:
   - **Before-Prompt Stop (`Ready for ...` / `Q...`)**: Pauses with the empty prompt box (or the agent's settled question) on screen so you can frame what you are about to ask or answer.
   - **After-Prompt Stop (`Prompt: ...` / `A...` / `Reply: ...`)**: Pressing `p` quickly fills the prompt or answer into the input box and pauses **before `Enter` is submitted**, so the audience can read the exact prompt/reply on screen.
   - Pressing `p` again submits `Enter` and runs the agent at warped speed (`<= 30s` cap) to the next Before-Prompt stop (or the final completion stop).

3. **Fast Prompts — Even When Typed by Hand (`~0.5s–0.8s` warped / `1.0s–1.6s` raw)**:
   - All hand-typed prompts and answers have their per-keystroke `dt` compressed so typing never drags on stage:
     - `01-bootstrap`: `Prompt: Move Drift Report & Commit` (`27.6s` raw $\rightarrow$ `1.5s` raw / `0.8s @ 2x`)
     - `02-brainstorm`: Hand-typed answers `A1` (`22.4s`), `A2` (`11.5s`), `A3` (`9.7s`), `A4` (`20.9s`), `A5` (`32.9s`), `A6` (`20.7s`) ($\rightarrow$ `1.5s` raw / `0.8s @ 2x` each, saving `~53s` of dead typing time)
     - `03-draft`: `Prompt: /egm-draft @concepts_brief.md ...` (`8.7s` raw $\rightarrow$ `1.5s` raw / `0.8s @ 2x`)
     - `04-review`: `Reply: interactive` (`2.9s` raw $\rightarrow$ `1.0s` raw / `0.5s @ 2x`) and `Reply: accept` (`3.0s` raw $\rightarrow$ `1.0s` raw / `0.5s @ 2x`)

---

## 2. Summary Table Across All 7 Recordings (Cast-Heavy Budget)

| # | Recording | Act | Start State (`0. Start`) | Trimmed Stops (Paired Before/After) | Warped Play | Stage Time (Play + Pauses) | Trimmed Tails / Mid-Stream Cuts |
| :-: | :--- | :--- | :--- | :---: | :---: | :---: | :--- |
| **1** | `vanilla-vibe` | Act II (Foil) | AGY Start Page | **`6` stops** (`3` prompt pairs) | `1m 15s` (`75.2s`) | **`~1m 51s`** | Shell launch & trailing `Ctrl+D` exit stripped; keeps Initial Prompt, `Q1`/`A1` pin prompt, and Breadboard SVG prompt. |
| **2** | `01-bootstrap` | Act III.a (Elephant) | AGY Start Page | **`6` stops** (`/plugin` + `/egm-bootstrap` + `Q1`) | `0m 59s` (`59.1s`) | **`~1m 35s`** | Shell `agy` launch & trailing `Move Drift Report & Commit` turn (`#4070..#5139`) stripped. |
| **3** | `02-brainstorm` | Act III.b (Brainstorm) | AGY Start Page | **`9` stops** *(or `15` stops w/ `Q4–Q6`)* | `1m 46s` (`105.6s`) | **`~2m 40s`** | Shell `agy` launch, `Q4–Q6` CLI subcommand minutiae (`#4907..#7395`), and `Stage & Commit` tail (`#11058..#12435`) stripped; hand-typed `A1–A3` tightened to `0.8s`. |
| **4** | `03-draft` | Act III.c (Drafting) | AGY Start Page | **`6` stops** *(or `10` stops w/ `Q4–Q5`)* | `1m 55s` (`115.0s`) | **`~2m 31s`** | Shell `agy` launch, `Q4–Q5` section approvals (`#6478..#9259`), and `Save v0-draft & Commit` tail (`#13146..#14006`) stripped; `/egm-draft` prompt tightened to `0.8s`. |
| **5** | `04-review` | Act III.d (Goldfish) | AGY Start Page | **`13` stops** (Pass 1 + `B-1` + `/clear` + Pass 2) | `2m 21s` (`141.4s`) | **`~3m 39s`** | Shell `agy` launch, items `2–12` walk-through + Pass 1 archive (`#14020..#26319`), and Pass 2 auto-resolve/commit tail (`#33141..#37820`) stripped. |
| **6** | `05-implement` | Act III.e (Implement) | AGY Start Page | **`4` stops** (`2` prompt pairs) | `0m 58s` (`58.2s`) | **`~1m 22s`** | Shell `agy` launch & trailing `Verify Submodules & Commit` turn (`#12402..#14101`) stripped; keeps Firmware Build & Wiring SVG prompts. |
| **7** | `06-teach-code` | Act IV (Codelab) | Shell (`npx skills add`) $\rightarrow$ AGY Start Page | **`9` stops** (`npx skills add` + `2` AGY prompt pairs) | `1m 10s` (`69.5s`) | **`~2m 04s`** | Keeps full `npx skills add domesticmouse/skills` install (`#0000..#0136`), transitions to AGY Start Page (`Ready for /teach-code`), and runs `/teach-code` + verification prompts. |
| **Total** | **All 7 Casts** | **Acts II–IV** | **Casts 1–6 AGY Start · Cast 7 Skill Install** | **`53` stops** | **`10m 24s` (`624.0s`)** | **`~15m 42s`** (`10m 24s` play + `5m 18s` pauses) | **`~15m 42s` in live casts / `~14m 18s` for Setup, Debrief, Hook & Finale slides.** |

*(Note: Keeping `Q4–Q6` in `02-brainstorm` and `Q4–Q5` in `03-draft` adds `10` stops and `~1m 36s` of warped playback, bringing the total to `11m 50s` warped play — you can toggle both in [`cast-curation-prototype.html`](./cast-curation-prototype.html).)*

---

## 3. Recording-by-Recording Stop Points (Before & After Every Prompt)

### Cast 1: `vanilla-vibe.cast` (`1m 15s` warped · `6` stops)
- **`0. Start` (Before Prompt 1)**: Settled on the **AGY Start Page** (`Ready for Initial Prompt`; shell launch `#0000..#0022` stripped).
- **`1. Initial Prompt: XIAO RP2350 Bar Graph` (After Prompt 1 — `0.5s @ 2x`)**: Full functional prompt pasted into the AGY input box, paused before `Enter`.
- **`2. Q1: Breadboard Pins & LED Polarity` (Before Reply 1 — `14.1s @ 2x`)**: Agent finishes initial thought and asks its single question (`D0/A0` + which 6 LED pins?).
- **`3. A1: Any Free Breadboard Pins` (After Reply 1 — `0.5s @ 2x`)**: Human reply (`"any free breadboard pins you pick, active-high..."`) in input box, paused before `Enter`.
- **`4. Firmware & Tests Built — Ready for Wiring Prompt` (Before Prompt 2 — `29.8s @ 52x`)**: Compresses `25m 51s` of uninterrupted monolithic code generation (`0` `README.md` reads) into `29.8s`, landing on the settled prompt box.
- **`5. Prompt: Breadboard Wiring Guide & SVG` (After Prompt 2 — `0.5s @ 2x`)**: Second prompt in input box, paused before `Enter`.
- **`6. Breadboard Wiring Guide & SVG Complete` (End — `29.8s @ 11x`)**: Generates `BREADBOARD_WIRING.md` and `breadboard_circuit.svg`.

---

### Cast 2: `01-bootstrap.cast` (`0m 59s` warped · `6` stops)
- **`0. Start` (Before `/plugin`)**: Settled on the **AGY Start Page** (`Ready for /plugin`; shell launch `#0000..#0020` stripped).
- **`1. Run /plugin` (After `/plugin` — `0.7s @ 2x`)**: `/plugin` typed in input box, paused before `Enter`.
- **`2. Select egm in Discover` (Before Install — `2.4s @ 2x`)**: Plugin manager open with `egm` highlighted.
- **`3. egm Plugin Installed — Ready for /egm-bootstrap` (Before Prompt 1 — `3.0s @ 2x`)**: Plugin installed, back at the settled AGY prompt box.
- **`4. Prompt: /egm-bootstrap` (After Prompt 1 — `0.5s @ 2x`)**: `/egm-bootstrap Create the root README.md...` in the input box, paused before `Enter`.
- **`5. Q1: Run EGM Drift Report?` (Before Reply 1 — `29.1s @ 7x`)**: Root `README.md` blueprint generated; `ask_user` widget asks whether to run the drift report (`✓ Yes` highlighted).
- **`6. Root Blueprint & EGM Drift Report Complete` (End — `25.3s @ 6x`)**: Confirming `Yes` runs `/egm-drift-report` across `3,241` directories and finishes at the settled prompt (`Move & Commit` tail `#4070..#5139` stripped).

---

### Cast 3: `02-brainstorm.cast` (`1m 46s` warped · `9` stops; or `2m 27s` · `15` stops w/ `Q4–Q6`)
- **`0. Start` (Before Prompt 1)**: Settled on the **AGY Start Page** (`Ready for /egm-brainstorm`; shell launch `#0000..#0039` stripped).
- **`1. Prompt: /egm-brainstorm` (After Prompt 1 — `0.5s @ 2x`)**: `/egm-brainstorm I want to build an out-of-tree Apache NuttX...` in input box, paused before `Enter`.
- **`2. Q1: Breadboard Pin Mapping & Reference Voltage` (Before Reply 1 — `29.5s @ 14x`)**: Agent reads `49` `README.md` contracts (`412.5s` raw) and asks `Q1`.
- **`3. A1: Layout Connections & Design Breadboard` (After Reply 1 — `0.8s @ 2x`, tightened from `22.4s` raw)**: Hand-typed reply appears quickly in the input box, paused before `Enter`.
- **`4. Q2: Boot Startup Architecture vs. USB-C NSH` (Before Reply 2 — `12.3s @ 2x`)**: Agent asks how to coexist with USB-C `nsh>` on boot.
- **`5. A2: Spawn in Background, Keep USB-C NSH` (After Reply 2 — `0.8s @ 2x`, tightened from `11.5s` raw)**: Hand-typed reply appears quickly in the input box, paused before `Enter`.
- **`6. Q3: LED Bar Graph Display Mapping` (Before Reply 3 — `4.3s @ 2x`)**: Agent asks how the 6 LEDs should represent the pot value.
- **`7. A3: PWM Brightness on Active Segment` (After Reply 3 — `0.8s @ 2x`, tightened from `9.7s` raw)**: Reply selected/typed, paused before `Enter`.
- *(Optional mid-stream cut: `Q4`/`A4`, `Q5`/`A5`, `Q6`/`A6` CLI subcommand negotiation `#4907..#7395` stripped—or kept with `0.8s` tightened `A4–A6` replies).*
- **`8. Q7: Readiness Checkpoint — Review with Goldfishes` (Before Reply 7 — `28.2s @ 7x`)**: Brainstorm readiness gate (`✓ Yes, synthesize and review with Goldfishes` highlighted).
- **`9. Goldfish Synthesis Complete — concepts_brief.md` (End — `28.4s @ 14x`)**: Compresses `23m 24s` raw of Goldfish synthesis into `28.4s`, producing `concepts_brief.md` (`Stage & Commit` tail `#11058..#12435` stripped).

---

### Cast 4: `03-draft.cast` (`1m 55s` warped · `6` stops; or `2m 50s` · `8` stops w/ `Q4–Q5`)
- **`0. Start` (Before Prompt 1)**: Settled on the **AGY Start Page** (`Ready for /egm-draft`; shell launch `#0000..#0019` stripped).
- **`1. Prompt: /egm-draft @concepts_brief.md target docs/designs/xiao-rp2350-bargraph.md` (After Prompt 1 — `0.8s @ 2x`, tightened from `8.7s` raw)**: Hand-typed `/egm-draft` command and `@concepts_brief.md` autocomplete zipped to `0.8s`, paused before `Enter`.
- **`2. Q1: Confirm Context & Start Drafting` (Before/After Selection — `28.3s @ 2x`)**: Agent loads `51` files and presents the `ask_user` confirmation (`✓ Proceed to Step 2` highlighted).
- **`3. Q2: Section Approval — Overview & Goals` (Before/After Selection — `27.3s @ 3x`)**: First section drafted; `ask_user` approval gate (`✓ Approve Overview & Goals` highlighted).
- **`4. Q3: Section Approval — Technical Plan` (Before/After Selection — `29.1s @ 15x`)**: Compresses `436.7s` of architecture drafting into `29.1s`, pausing at the Technical Plan gate where the `RP23XX_ADC_DIV` false-start assumption is approved.
- *(Optional mid-stream cut: `Q4` Alternatives & `Q5` Detailed Implementation gates `#6478..#9259` stripped—or kept).*
- **`5. Draft Reconciled & Promoted to v0-draft` (End — `29.0s @ 13x`)**: Final spec promoted to `docs/designs/xiao-rp2350-bargraph.md` (`Save v0-draft & Commit` tail `#13146..#14006` stripped).

---

### Cast 5: `04-review.cast` (`2m 21s` warped · `13` stops)
- **`0. Start` (Before Prompt 1)**: Settled on the **AGY Start Page** (`Ready for Pass 1 /egm-review`; shell launch `#0000..#0036` stripped).
- **`1. Prompt: Pass 1 /egm-review` (After Prompt 1 — `0.6s @ 2x`)**: `/egm-review @docs/designs/xiao-rp2350-bargraph.md` in input box, paused before `Enter`.
- **`2. Pass 1 Q1: Select Review Execution Mode (Consolidated)` (`8.0s @ 2x`)**: `ask_user` selector paused with `Consolidated` highlighted.
- **`3. Pass 1 Review Complete — Ready for /egm-resolve` (Before Prompt 2 — `29.7s @ 39x`)**: Compresses `21m 33s` raw (`146` `README.md` contract reads by parallel Goldfish subagents) into `29.7s`.
- **`4. Prompt: Pass 1 /egm-resolve` (After Prompt 2 — `0.4s @ 2x`)**: `/egm-resolve` in input box, paused before `Enter`.
- **`5. Pass 1 Audit Findings Compiled — Choose Resolution Mode` (Before Reply 1 — `26.8s @ 6x`)**: 12 findings (`B-1..B-3`, `M-1..M-5`, `N-1..N-4`) compiled; waiting for resolution mode at `? for shortcuts`.
- **`6. Reply: interactive (Walk Through 12 Findings)` (After Reply 1 — `0.5s @ 2x`, tightened from `2.9s` raw)**: `interactive` typed in input box, paused before `Enter`.
- **`7. Item 1/12 (B-1): NVIC IRQ Gating vs. RP23XX_ADC_DIV` (Before Reply 2 — `17.1s @ 2x`)**: Goldfish explains why `RP23XX_ADC_DIV` is ignored under `START_ONCE` (`500 kHz` IRQ storm) and proposes NVIC IRQ gating.
- **`8. Reply: accept (Accept Item 1)` (After Reply 2 — `0.5s @ 2x`, tightened from `3.0s` raw)**: `accept` typed in input box, paused before `Enter`.
- *(Mid-stream cut: Items `2–12` walk-through and Pass 1 archive prompt `#14020..#26319` stripped so we land directly at `Ready for /clear`).*
- **`9. Pass 1 Resolved — Ready for /clear` (Before `/clear` — `5.0s @ 2x`)**: Settled prompt box ready to wipe session memory.
- **`10. Run /clear` (After `/clear` — `0.4s @ 2x`)**: `/clear` in input box, paused before `Enter`.
- **`11. Session Cleared — Ready for Pass 2 /egm-review` (Before Prompt 3 — `0.1s @ 2x`)**: Fresh AGY start banner after `/clear` amnesia reset.
- **`12. Prompt: Pass 2 /egm-review (with GoldfishContext)` (After Prompt 3 — `0.6s @ 2x`)**: Pass 2 `/egm-review` prompt in input box, paused before `Enter`.
- **`13. Pass 2 Review Complete — 11 Secondary Findings (CA-01 Short Read)` (End — `29.0s @ 26x`)**: Compresses Pass 2 Goldfish audit into `29.0s`, catching the `25-byte` short-read loop (`CA-01`) (`Pass 2 /egm-resolve & Commit` tail `#33141..#37820` stripped).

---

### Cast 6: `05-implement.cast` (`0m 58s` warped · `4` stops)
- **`0. Start` (Before Prompt 1)**: Settled on the **AGY Start Page** (`Ready for Implementation Prompt`; shell launch `#0000..#0052` stripped).
- **`1. Prompt: Implement Firmware & Verify Build` (After Prompt 1 — `0.5s @ 2x`)**: Implementation prompt in input box, paused before `Enter`.
- **`2. Firmware Built & Tested — Ready for Wiring Prompt` (Before Prompt 2 — `29.0s @ 27x`)**: Compresses `13m 04s` raw into `29.0s`—showing `100%` host unit tests passing and `nuttx/nuttx.uf2` (`347K`) building on the **first attempt** (`0` retries), paused at the empty prompt box.
- **`3. Prompt: Generate Breadboard Wiring Guide & SVG` (After Prompt 2 — `0.5s @ 2x`)**: Wiring guide & SVG prompt in input box, paused before `Enter`.
- **`4. Breadboard Wiring & SVG Complete` (End — `28.2s @ 11x`)**: Generates `BREADBOARD_WIRING.md` and `breadboard-wiring.svg` (`Verify Submodules & Commit` tail `#12402..#14101` stripped).

---

### Cast 7: `06-teach-code.cast` (`1m 10s` warped · `9` stops)
- **`0. Start` (Before `npx skills add`)**: Shell prompt ready to install the custom skill.
- **`1. Run npx skills add domesticmouse/skills` (After Command — `0.7s @ 2x`)**: Command in shell, paused before `Enter`.
- **`2. Select teach-code Skill` (`4.5s @ 2x`)**: Interactive skill picker paused on `teach-code`.
- **`3. Confirm Project Installation Scope` (`1.5s @ 2x`)**: Confirming project installation scope (`.agents/skills/teach-code`).
- **`4. teach-code Installed` (`0.7s @ 2x`)**: Skill installation complete in shell.
- **`5. AGY Start Page — Ready for /teach-code Prompt` (Before Prompt 1 — `2.2s @ 2x`)**: Launches `agy` and settles on the **AGY Start Page** ready for `/teach-code`.
- **`6. Prompt: /teach-code Story Arc in ./tutorial/` (After Prompt 1 — `0.5s @ 2x`)**: `/teach-code` prompt in input box, paused before `Enter`.
- **`7. 6-Lesson Course Generated — Ready for Verify Prompt` (Before Prompt 2 — `29.8s @ 79x`)**: Compresses `39m 11s` raw into `29.8s`, generating all 6 interactive HTML/Markdown lessons, reference sheets, and C99 companion apps, paused at the empty prompt box.
- **`8. Prompt: Verify sync_listing.py, build.py & Commit` (After Prompt 2 — `0.5s @ 2x`)**: Verification prompt in input box, paused before `Enter`.
- **`9. Tutorial Course Verified & Committed` (End — `29.1s @ 5x`)**: Runs `sync_listing.py` and `build.py` verification gates cleanly.
