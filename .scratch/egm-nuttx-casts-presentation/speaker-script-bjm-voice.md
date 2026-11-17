# Verbatim Speaker Script & Live Cast Commentary (`bjm-voice`)

> **Asset for Ticket 06**: [Draft the Spoken Script and Narrator Voice in bjm-voice](./issues/06-speaker-script-and-narrative-bjm-voice.md)
> **Aligned Deck Blueprint**: [Slide Structure, Stage Beats & 30-Minute Timing Budget Prototype](./slide-structure-prototype.md) (`38` slides · `~30m`)
> **Aligned Cast Curation**: [Cast Curation, Trim & Stop-Point Prototype](./cast-curation-prototype.md) (`7` live recordings · `53` stops · `10m 24s` warped play / `15m 42s` stage time)

---

## 1. Voice, Pacing & Stage Delivery Rules (`bjm-voice`)

1. **Australian English & Conversational Cadence (`~135–145 wpm`)**:
   - Uses Australian spelling and idioms throughout (*G'day*, *AU\$10*, *memorised*, *realise*, *colour*, *labelled*, *modelled*, *centre*, *fibreglass*, *amongst*).
   - Paced at `~140 words per minute` (`~2.3 words/second`) so a `30s` concept slide carries `~65–75` words across its `[On Load]` and `[v-click]` beats—leaving breathing room for visual absorption.
2. **Inline Slide Click Tags (`[On Load]`, `[v-click 1]`, `[v-click 2]`, ...)**:
   - Every non-cast slide (`31` slides) splits its verbatim script across `[On Load]` and each progressive `v-click` beat so your voice triggers the visual reveal rather than reading a wall of text after the fact.
3. **Dual-Layer Live `.cast` Narration (`7` slides, `53` stops, `15m 42s` stage time)**:
   - **Paused Stop-Point Cues (`[Stop N — Paused]`)**: A punchy 1-sentence cue delivered while the player is paused at a Before-Prompt or After-Prompt marker, right before pressing `p`.
   - **Warped Execution Voiceovers (`[Warp Run — ~25s–30s]`)**: Scripted play-by-play narration spoken *while* the terminal scrolls at `6x–79x` warp speed, eliminating dead air on stage.
4. **Community Credits & Candid Engineering Honesty**:
   - Credits **Dave Rensin (`@drensin`)** for the **Elephant-Goldfish Model (EGM)** (*"Weeds Syndrome"*, *"Design is the new code: design with an Elephant, review with a Goldfish"*, shared-context confirmation bias, and *"Humans Own the Outcomes"*).
   - Credits **Matt Pocock** for the open-source `/teach` skill that inspired `/teach-code` (built via `/agy-customizations`), preserving Zone of Proximal Development learning records, **Edward Tufte** right-margin sidenotes, and equal-word-count (`10 words/option`) retrieval quizzes.
   - Owns all five candid non-triumph beats: fair credit to Vanilla Vibe before the `0.7"` `Cols C–H` breadboard straddle (`S08`–`S09`), human + Elephant shared blame on `CONFIG_RP23XX_ADC_DIV` (`S16`–`S18`), Pass 1's fix introducing the `CA-01` `25-byte` FIFO deadlock caught only after `/clear` (`S20`–`S22`), the `S25` breadboard footprint honesty check (`0.6"` XIAO header spacing vs. Vanilla's `0.7"` `Cols C–H` and EGM's `0.3"` `Cols E–F` footprint mismatch alongside EGM's verified electrical/pinout wins), and the `6 Pass / 4 Partial` strict post-build audit (`S26`) plus `Lesson 0006` catching two real `bargraph test` bugs (`S31`).

---

## 2. Act I — The Hook & Real-World Embedded Stress Test (`01-hook-and-setup.md` · `8` Slides · `4m 40s`)

### `S01` — Title Slide (`layout: center` · `25s` · `~55 words`)
- **`[On Load]`**:
  > "G'day everyone, I'm Brett Morgan from Google Cloud. Tonight's talk is about what happens when you drop a coding agent into a massive, unfamiliar C codebase—and how rolling two kinds of living documentation lets both the AI *and* the human sitting at the keyboard actually understand what ships to silicon."

### `S01b` — The Stack: Apache NuttX & Google Antigravity (`layout: default` · `20s` · `~50 words`)
- **`[On Load]`**:
  > "Before we touch any hardware, let's call out the two main players in tonight's experiment. On the left is **Apache NuttX**, a mature, POSIX-compliant real-time operating system you can explore at `nuttx.apache.org`. On the right is **Google Antigravity**, the agentic development platform I used to tame it, available at `antigravity.google`."

### `S02a` — How a Solderless Breadboard Works (`layout: default` · `40s` · `~95 words`)
- **`[On Load]`**:
  > "Before we wire up our hardware, let's take a quick X-ray look at a solderless breadboard—because its hidden metal spring clips behave a lot like variable scopes in code. Along the top and bottom, the horizontal power rails act like global scope: every hole in the blue minus rail or red plus rail is wired together. In the middle, each 5-hole vertical column—rows `a` through `e` above the trench, and `f` through `j` below it—is an independent local node. And that centre plastic trench is a hard namespace barrier: you seat a dual-row module like the XIAO RP2350 straddling `e` and `f` so its left and right pins never short together."

### `S02b` — Physical Breadboard Wiring: XIAO RP2350 6-LED Bar Graph (`layout: center` · `30s` · `~70 words`)
- **`[On Load]`**:
  > "Here is the full physical wiring layout on a 28-column breadboard. The XIAO RP2350 straddles the centre trench, leaving open tie-points on both sides of every pin. On the left, a ten-kilohm potentiometer feeds `D0/A0` with an optional hundred-nanofarad decoupling capacitor, strictly at `3.3` volts. Across the right, six discrete LEDs—two green, two yellow, two red—each get their own `330`-ohm current-limiting resistor on `D3` through `D5` and `D8` through `D10`, intentionally skipping `D1/D2` and `D6/D7`."

### `S02c` — The Hardware Target: XIAO RP2350 & 6-LED Bar Graph (`layout: default` · `40s` · `~90 words`)
- **`[On Load]`**:
  > "So to recap the target we're building: an AU\$10 Seeed Studio XIAO RP2350 sitting on a 400-tie-point solderless breadboard beside my laptop, wired to a ten-kilohm potentiometer on `D0/A0` and six through-hole LEDs—two green, two yellow, two red."
- **`[v-click 1 — Proportional 6-Segment PWM Display]`**:
  > "Instead of crude on/off steps, I wanted smooth `16-bit` hardware PWM interpolation across all six channels so the active segment ramps proportionally in brightness as you twist the pot."
- **`[v-click 2 — Dual Runtime Requirement]`**:
  > "And it needed to run as an autonomous fifty-hertz background daemon called `bargraphd`, while keeping the single USB-C serial console free for interactive shell commands like `status`, `test`, and `set`."

### `S03` — The RTOS Scale Shock: Why Apache NuttX Breaks "Vibe Coding" (`layout: default` · `40s` · `~95 words`)
- **`[On Load]`**:
  > "In Arduino, that's a thirty-line `loop()` sketch. The catch? I wanted to build it as a proper out-of-tree application inside **Apache NuttX**—thirty years of industrial POSIX real-time operating system C that I have *definitely* not memorised."
- **`[v-click 1 — The Documentation Gap: 3,241 Dirs vs 13 READMEs]`**:
  > "When I checked the upstream submodules—`nuttx/` and `apps/`—they spanned **3,241 directories**, shipping with just **13 `README.md` files**. That is zero-point-four percent directory coverage."
- **`[v-click 2 — The Consequence]`**:
  > "Drop an unguided coding agent into three thousand undocumented directories, and it falls straight back to blind `grep` and training-weight guesswork."

### `S04` — Three Hardware & RTOS Booby Traps Waiting on the Bench (`layout: default` · `55s` · `~125 words`)
- **`[On Load]`**:
  > "Worse still, this particular combination of RP2350 silicon and NuttX drivers hides three brutal booby traps waiting on the bench."
- **`[v-click 1 — Trap 1: USB-C Console Boot Hang]`**:
  > "First: the XIAO has only one USB-C port. If your startup script blocks waiting on the daemon, USB enumeration hangs and bricks your console; if you wire two startup hooks, the daemon races itself on boot."
- **`[v-click 2 — Trap 2: ADC Pin Clobber & Erratum RP2350-E9]`**:
  > "Second: NuttX's RP2350 ADC initialiser unconditionally resets all four ADC-capable pins (`GPIO 26` to `29`) and leaves the digital input buffer `IE=1` enabled—clobbering any LEDs wired to `D1` or `D2` and tripping Silicon Erratum `RP2350-E9` pad leakage on our potentiometer."
- **`[v-click 3 — Trap 3: The 500k IRQ/s Interrupt Storm]`**:
  > "Third: opening `/dev/adc0` fires continuous five-hundred-kilohertz interrupts that starve the CPU cold—and if you try to dodge that by opening and closing the device forty times a second, you hit an unguarded ISR teardown race."

### `S05` — The Controlled Experiment: Same Hardware, Same Prompt, Two Workflows (`layout: default` · `30s` · `~70 words`)
- **`[On Load]`**:
  > "So I ran a controlled experiment with Google Antigravity on the exact same hardware and functional goal."
- **`[v-click 1 — Experiment A: Vanilla Vibe Foil]`**:
  > "In **Experiment A**, we run unconstrained 'Vanilla Vibe' coding against raw upstream NuttX—thirteen READMEs, zero custom skills."
- **`[v-click 2 — Experiment B: Roll Your Own Docs (EGM + /teach-code)]`**:
  > "In **Experiment B**, we roll our own docs: first **Docs for the AI** using the Elephant-Goldfish Model, and then **Docs for the Human** using `/teach-code`."

---

## 3. Act II — The Vanilla Vibe Foil (`02-vanilla-foil.md` · `5` Slides · `3m 41s`)

### `S06` — Act II Setup: Watching Unconstrained Antigravity (`layout: default` · `25s` · `~60 words`)
- **`[On Load]`**:
  > "Let's watch Experiment A first. Keep an eye out for what Google's **Dave Rensin** calls **'Weeds Syndrome'**—diving head-first into writing C code before knowing a single subsystem invariant."
- **`[v-click 1]`**:
  > "We paste the functional prompt straight into Antigravity with zero directory contracts."
- **`[v-click 2]`**:
  > "Watch it ask one quick pin question, sprint through a twenty-six-minute monolithic build without reading a single `README.md`, and spit out a breadboard wiring SVG."

### `S07` — LIVE CAST 1: `vanilla-vibe.cast` (`layout: cast-full` · `1m 15s` play + `36s` stops = `1m 51s`)
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

### `S07a` — Vanilla Vibe's Generated Breadboard Wiring (`layout: default` · `20s` · `~45 words`)
- **`[On Load]`**:
  > "Here is the full breadboard wiring SVG that Vanilla Vibe just generated. At a glance, it looks clean and well-organised—complete with a dark-mode aesthetic, colour-coded jumper wires, and labelled components. Now let's zoom in on the XIAO RP2350 itself to see what actually happened on the board."

### `S08` — Debrief: The Lie of "It Compiled!" (`layout: hardware-callout` · `35s` · `~80 words`)
- **`[On Load]`**:
  > "Here is the SVG Vanilla Vibe just generated."
- **`[v-click 1 — Green Checkmarks on Host]`**:
  > "To be completely fair to Vanilla Vibe, the host C math and unit test scaffolding were genuinely tidy, and `nuttx.uf2` compiled on the first session."
- **`[v-click 2 — The 0.7" Breadboard Straddle across Cols C to H]`**:
  > "Except look closely at where it seated the XIAO module on the breadboard: it stretched a zero-point-three-inch module across **six columns—Columns C to H**! On a physical breadboard, that buries almost every tie-point underneath the fibreglass PCB."

### `S09` — Vanilla Vibe Audit Scorecard: `5 / 10` Against the Hardware Traps (`layout: default` · `30s` · `~70 words`)
- **`[On Load]`**:
  > "When we audited Vanilla Vibe against ten real hardware and RTOS invariants, it scored **five out of ten**."
- **`[v-click 1 — Where Vanilla Shone]`**:
  > "Every pass was generic C knowledge baked into LLM pre-training: fixed-point Q4 EMA filtering, proportional segment slicing, and host unit tests."
- **`[v-click 2 — Where Vanilla Failed on Silicon]`**:
  > "Every failure was an RP2350 or NuttX invariant it never read: `RP2350-E9` pad leakage, looping `open()` and `close()` at forty hertz into the ISR teardown race, and dropped Kconfig symbols."

---

## 4. Act III — Rolling Docs for the AI: The Elephant-Goldfish Model (`03-egm-journey.md` · `18` Slides · `17m 37s`)

### `S10` — Act III Anchor: The Elephant-Goldfish Triad (`layout: default` · `40s` · `~95 words`)
- **`[On Load]`**:
  > "To stop shipping hidden silicon timebombs, I turned to **Dave Rensin's Elephant-Goldfish Model** (`egm`). Dave sums it up in two lines: *'Design is the new code: design with an Elephant, review with a Goldfish.'*"
- **`[v-click 1 — The Elephant (Context Memory)]`**:
  > "**The Elephant** is your long-horizon context memory—powered here by `3,241` directory-level `README.md` contracts so the agent reads `209` architectural contracts instead of zero."
- **`[v-click 2 — The Human (Intent & Ownership)]`**:
  > "**The Human** makes product and hardware-boundary calls one question at a time."
- **`[v-click 3 — The Goldfish (Stateless Adversarial Swarm)]`**:
  > "And **The Goldfish** is a swarm of fresh-context reviewers with three-second memory—zero emotional loyalty to your draft."

### `S11` — Unpacking The Elephant: Directory Contracts as a Navigable Map (`layout: skill-breakdown` · `40s` · `~95 words`)
- **`[On Load]`**:
  > "How do you give an Elephant memory across three thousand directories without blowing out the context window?"
- **`[v-click 1 — Beat 1: Tree Scale]`**:
  > "Every directory in `nuttx/` and `apps/` gets a concise `README.md` contract—`2,607` in the kernel, `634` in user space—forming a top-down tree."
- **`[v-click 2 — Beat 2: Stylized Contract Card (`nuttx/arch/arm/src/rp23xx`)]`**:
  > "Inside `arch/arm/src/rp23xx/README.md`, it doesn't paste C code. It states public interfaces and plain-English hardware invariants—like warning that ADC init resets all four pins `GPIO 26` to `29` and leaves digital buffer `IE=1` enabled."
- **`[v-click 3 — Beat 3: Child Directory Routing]`**:
  > "And each contract routes the agent to the exact child directory it needs next."

### `S12` — Keeping the Elephant Honest: `/plugin` $\rightarrow$ `/egm-bootstrap` $\rightarrow$ `/egm-drift-report` (`layout: default` · `25s` · `~60 words`)
- **`[On Load]`**:
  > "Before we design anything, we wire up the Elephant in three quick commands."
- **`[v-click 1]`**:
  > "First, we install the `egm` plugin live inside Antigravity using `/plugin`."
- **`[v-click 2]`**:
  > "Second, `/egm-bootstrap` synthesises the root workspace `README.md` blueprint."
- **`[v-click 3]`**:
  > "Third, `/egm-drift-report` audits all `3,241` directory contracts against `git HEAD` so stale docs never poison our design."

### `S13` — LIVE CAST 2: `01-bootstrap.cast` (`layout: cast-full` · `0m 59s` play + `36s` stops = `1m 35s`)
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

### `S14` — The Specification Funnel: Human + Elephant (`/egm-brainstorm`) (`layout: default` · `30s` · `~70 words`)
- **`[On Load]`**:
  > "Next comes `/egm-brainstorm`, where the Elephant and the Human narrow down the product shape before any code exists."
- **`[v-click 1 — 49 READMEs Read Upfront]`**:
  > "Before asking Question 1, the Elephant reads `49` `README.md` contracts—spotting the `GPIO 26` to `29` ADC pin clobber immediately."
- **`[v-click 2 — 1-at-a-Time Socratic Q&A]`**:
  > "Then it interviews me one question at a time: pin mapping, USB-C `nsh>` coexistence, and proportional PWM."
- **`[v-click 3 — Goldfish Synthesis]`**:
  > "Finally, a readiness gate synthesises our answers into `concepts_brief.md`."

### `S15` — LIVE CAST 3: `02-brainstorm.cast` (`layout: cast-full` · `1m 46s` play + `54s` stops = `2m 40s`)
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

### `S16` — Section-by-Section Drafting & Planting the `RP23XX_ADC_DIV` Trap (`layout: default` · `25s` · `~60 words`)
- **`[On Load]`**:
  > "In Phase 3, `/egm-draft` turns `concepts_brief.md` into our full engineering specification, `xiao-rp2350-bargraph.md`."
- **`[v-click 1 — Section-by-Section Approval Gates]`**:
  > "Instead of dumping a wall of Markdown, it drafts section by section with human approval gates."
- **`[v-click 2 — Cueing Stop 4's RP23XX_ADC_DIV Trap]`**:
  > "Watch **Stop 4 (`Q3: Technical Plan`)** closely: both the Elephant and I agree to throttle `/dev/adc0` using `CONFIG_RP23XX_ADC_DIV`. Sounds totally plausible—and it's a trap!"

### `S17` — LIVE CAST 4: `03-draft.cast` (`layout: cast-full` · `1m 55s` play + `36s` stops = `2m 31s`)
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

### `S18` — Debrief: Author Bias, Self-Sycophancy & Why We Need Goldfish (`layout: default` · `25s` · `~65 words`)
- **`[On Load]`**:
  > "Why was `CONFIG_RP23XX_ADC_DIV` a landmine?"
- **`[v-click 1 — Why RP23XX_ADC_DIV Fails Under START_ONCE]`**:
  > "Because NuttX's RP2350 ADC driver triggers conversions via single-shot `START_ONCE`—which completely ignores the hardware `DIV` register and still fires **five hundred thousand interrupts per second**!"
- **`[v-click 2 — Dave Rensin on Shared-Context Confirmation Bias]`**:
  > "As Dave Rensin warns, the costliest mistake is a plan both the agent and the human fell in love with inside the same session. Once we agreed on `ADC_DIV`, confirmation bias locked in. We needed a reviewer with amnesia."

### `S19` — Unpacking The Goldfish: Stateless Adversarial Swarm (`/egm-review`) (`layout: default` · `35s` · `~85 words`)
- **`[On Load]`**:
  > "Enter `/egm-review`—the Goldfish swarm."
- **`[v-click 1 — Stateless & Uninvested]`**:
  > "Each Goldfish runs in a brand-new subagent context window. It never saw our brainstorm, has zero sympathy for how hard we worked on `v0-draft`, and owes us zero politeness."
- **`[v-click 2 — Multi-Angle Adversarial Personas (146 README Reads)]`**:
  > "Five specialised personas—*Zoom-Out Architect*, *Comprehension*, *Hostile Design Critic*, *Implementation Readiness*, and *Codebase Details*—audit `v0-draft` in parallel across `146` `README.md` contracts."
- **`[v-click 3 — Synthesizer + /egm-resolve]`**:
  > "Then a Synthesizer deduplicates their findings into ranked Blockers (`B-1..B-3`) and Majors for `/egm-resolve`."

### `S20` — What to Watch in `04-review.cast`: `B-1`, `/clear` Amnesia, and Pass 2 (`layout: default` · `20s` · `~50 words`)
- **`[On Load]`**:
  > "Two things to watch in `04-review.cast`:"
- **`[v-click 1 — Pass 1 Catches B-1]`**:
  > "First, watch Pass 1 immediately flag `RP23XX_ADC_DIV` as Blocker `B-1` and replace it with NVIC IRQ burst gating."
- **`[v-click 2 — /clear Amnesia + Pass 2 Catches CA-01]`**:
  > "Second, watch us run `/clear` to wipe session memory cold—so Pass 2 catches a brand-new bug *introduced by Pass 1's fix*!"

### `S21` — LIVE CAST 5: `04-review.cast` (`layout: cast-full` · `2m 21s` play + `1m 18s` stops = `3m 39s`)
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
- **`[Stop 5 — Pass 1 Resolve Mode Prompt (`18.1s @ 2x`)]`**:
  > "It asks how we want to work through the `12` findings." *(Press `p`)*
- **`[Stop 6 — Reply: interactive (`0.5s @ 2x`)]`**:
  > "Type `interactive` so we inspect each blocker one by one." *(Press `p`)*
- **`[Stop 7 — Item 1/12 (B-1): 500 kHz ADC Interrupt Storm (`17.0s @ 2x`)]`**:
  > "Look at Item `1/12` (`B-1`): the Goldfish busted our `RP23XX_ADC_DIV` assumption cold and recommends gating the NVIC IRQ around a 5-sample burst!" *(Press `p`)*
- **`[Stop 8 — Reply: accept (B-1 Fix) (`0.5s @ 2x`)]`**:
  > "We type `accept` to adopt the 5-sample burst fix." *(Press `p`)*
- **`[Stop 9 — Pass 1 Resolved — Ready for /clear (`29.9s @ 9x`)]`**:
  > "Now all Pass 1 items are patched into the spec. Watch this next move: before Pass 2, we wipe our own session memory." *(Press `p`)*
- **`[Stop 10 — Run /clear (`0.5s @ 2x`)]`**:
  > "Running `/clear` so the agent forgets every justification from Pass 1." *(Press `p`)*
- **`[Stop 11 — Session Cleared — Ready for Pass 2 (`2.3s @ 2x`)]`**:
  > "Clean slate. Now we launch Pass 2." *(Press `p`)*
- **`[Stop 12 — Prompt: Pass 2 /egm-review with GoldfishContext (`4.6s @ 2x`)]`**:
  > "Running Pass 2 `/egm-review` cold against the updated spec." *(Press `p`)*
- **`[Warp Run to Stop 13 (`29.8s @ 57x` — 28m 38s raw)]`**:
  > *(Voiceover during 30s warp)*: "Watch Pass 2 inspect the 5-sample burst fix we *just* accepted in Pass 1. Because these Goldfish have zero memory of Pass 1, they calculate `5 samples × 5 bytes = 25 bytes` against NuttX's `8-sample` software FIFO—and spot a deadlock!"
- **`[Stop 13 — Pass 2 Review Complete — Catches CA-01]`**:
  > "Boom—`CA-01` caught in Markdown before a single line of C was written."

### `S22` — Debrief: First-Order Catch (Pass 1) vs. Second-Order Catch (Pass 2) (`layout: default` · `30s` · `~75 words`)
- **`[On Load]`**:
  > "That two-pass loop is my favourite moment in the entire project."
- **`[v-click 1 — First-Order Catch: Pass 1 B-1]`**:
  > "Pass 1 (`B-1`) killed the `500 kHz` interrupt storm by gating the NVIC interrupt around a 5-sample burst—discarding one stale sample and averaging four."
- **`[v-click 2 — Second-Order Catch: Pass 2 CA-01 after /clear]`**:
  > "Wait: NuttX's software ADC FIFO holds `8` samples (`7` usable before full). Reading `5` samples (`25 bytes`) leaves `2` in the queue, so the second `read(25)` blocks forever at `7`! `/clear` gave Pass 2 fresh eyes to catch the bug introduced by Pass 1's fix."

### `S23` — When the Spec is Hardened, Coding is Boring (`05-implement` Setup) (`layout: default` · `20s` · `~50 words`)
- **`[On Load]`**:
  > "Once you eliminate four Blockers and seven Majors in Markdown, coding becomes wonderfully boring."
- **`[v-click 1]`**:
  > "Every register quirk, FIFO read size (`20 bytes`), and pin assignment is already locked."
- **`[v-click 2]`**:
  > "Watch `05-implement.cast` pass `100%` of host tests and build `nuttx.uf2` on the **very first attempt (`0` retries)**—plus write `9` new `README.md` contracts."

### `S24` — LIVE CAST 6: `05-implement.cast` (`layout: cast-full` · `0m 58s` play + `24s` stops = `1m 22s`)
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

### `S24a` — EGM's Generated Breadboard Wiring (`layout: default` · `20s` · `~50 words`)
- **`[On Load]`**:
  > "Here is the full breadboard wiring SVG that EGM generated in one pass. Notice the clean light-mode schematic layout, the XIAO RP2350 seated across the centre trench, the potentiometer on `D0`, and the six colour-coded LEDs on `D1`, `D2`, `D4`, `D5`, `D8`, and `D9`. Now let's zoom in on the XIAO RP2350 itself to verify the physical footprint and pin routing."

### `S25` — Breadboard Reality Check: Electrical Wins vs. Physical Footprint (0.6" XIAO) (`layout: hardware-callout` · `35s` · `~115 words`)
- **`[On Load]`**:
  > "Zooming in on `docs/egm/breadboard-wiring.svg` gives us a great engineering honesty check."
- **`[v-click 1 — Verified Electrical & Pinout Wins]`**:
  > "On the electrical side, EGM nailed it: six non-ADC PWM pins (`D1, D2, D4, D5, D8, D9`), `330`-ohm resistors keeping total draw at `23.6` milliamps, `5V` left unconnected, and `IE` disabled on `GPIO 26` to defeat Erratum `RP2350-E9`."
- **`[v-click 2 — Physical Footprint Catch (0.3" vs. 0.6" XIAO Width)]`**:
  > "But look at the physical footprint: a XIAO RP2350's headers are `0.6` inches apart—meaning it needs Columns `C` and `G`, or `D` and `H`. Vanilla stretched it too wide to Columns `C` and `H` (`0.7` inches), while EGM seated it too narrow in Columns `E` and `F` (`0.3` inches) and hid the mismatch by drawing a wider centre trench in the SVG. Contracts caught every silicon and RTOS invariant, but mechanical dimensions still deserve a real-world check."

### `S26` — Candid Scorecard Callback: `5 / 10` vs. `10 / 10` (And Engineering Honesty) (`layout: default` · `30s` · `~75 words`)
- **`[On Load]`**:
  > "Let's look at the scorecard—and keep ourselves honest."
- **`[v-click 1 — Primary 10-Trap Scorecard: 5/10 vs 10/10]`**:
  > "Across our ten hardware and RTOS traps, Vanilla Vibe scored **five out of ten**; the EGM workflow scored **ten out of ten** with zero build retries."
- **`[v-click 2 — Candid Strict Audit: 6 Pass / 4 Partial / 0 Fail]`**:
  > "But I promised no unblemished triumph narratives: when I ran a cold post-build Goldfish audit against every line of C, it still flagged **four partial polish items** like `SIGTERM` cleanup—proving adversarial review is an ongoing habit, not magic."

---

## 5. Act IV — Finale: Rolling Docs for the Human & Live Lesson Showcase (`04-teach-code-finale.md` · `7` Slides · `6m 08s`)

### `S27` — Act IV Finale: From "Docs for the AI" to "Docs for the Human" (`layout: default` · `35s` · `~85 words`)
- **`[On Load]`**:
  > "That brings us to Act IV—and the biggest trap in AI-assisted engineering."
- **`[v-click 1 — Comprehension Debt & Rensin's Third Rule]`**:
  > "The AI just scored `10/10` and shipped `872` lines of `bargraph_main.c`. Brilliant—except Dave Rensin's third rule says **'Humans Own the Outcomes.'** If a wire pops loose on the bench in six months, *you* have to debug that firmware."
- **`[v-click 2 — Matt Pocock's /teach $\rightarrow$ /teach-code]`**:
  > "To pay down **comprehension debt**, I adapted **Matt Pocock's** brilliant `/teach` skill (using `/agy-customizations`) into **`/teach-code`**—turning our `README.md` contracts, design spec, and Goldfish reports into a six-lesson interactive **Edward Tufte** HTML course."

### `S28` — LIVE CAST 7: `06-teach-code.cast` (`layout: cast-full` · `1m 10s` play + `54s` stops = `2m 04s`)
- **`[Stop 0 — Shell Prompt (Before npx skills add)]`**:
  > "This recording starts in the terminal so you can see how `teach-code` installs dynamically." *(Press `p`)*
- **`[Stop 1 — Run npx skills add domesticmouse/skills (`1.4s @ 2x`)]`**:
  > "Running `npx skills add domesticmouse/skills`." *(Press `p`)*
- **`[Stop 2 — Select teach-code Skill (`10.0s @ 2x`)]`**:
  > "We select `teach-code` from the repository." *(Press `p`)*
- **`[Stop 3 — Select Target Agent (`2.9s @ 2x`)]`**:
  > "Target Antigravity in the workspace." *(Press `p`)*
- **`[Stop 4 — Confirm Installation Scope (`2.4s @ 2x`)]`**:
  > "Confirm installation." *(Press `p`)*
- **`[Stop 5 — Ready for /teach-code Prompt (AGY Start Page — `3.4s @ 2x`)]`**:
  > "Now we're inside Antigravity. Let's ask `/teach-code` to build the full six-lesson course." *(Press `p`)*
- **`[Stop 6 — Prompt: /teach-code 6-Lesson Interactive Course (`0.6s @ 2x`)]`**:
  > "Prompting `/teach-code` to turn our specs, contracts, and Goldfish catches into six self-contained HTML lessons with runnable C99 companion apps." *(Press `p`)*
- **`[Warp Run to Stop 7 (`29.8s @ 79x` — compressing 39m 18s raw)]`**:
  > *(Voiceover during 30s warp)*: "We're watching thirty-nine raw minutes at seventy-nine times speed. It maps the learner's Zone of Proximal Development, builds six runnable C99 companion apps in `tutorial/apps/0001` through `0006` under `-Wall -Wextra -Werror -pedantic`, crafts three interactive JavaScript simulators and four printable reference sheets, and compiles the Tufte HTML lessons."
- **`[Stop 7 — 6 Interactive Lessons & C99 Apps Generated — Ready for Verify]`**:
  > "All six lessons generated. Now let's run the zero-divergence verification check." *(Press `p`)*
- **`[Stop 8 — Prompt: Verify Lesson Snippets & Build (`0.6s @ 2x`)]`**:
  > "Running `sync_listing.py --check` and `build.py --check`." *(Press `p`)*
- **`[Warp Run to Stop 9 (`18.4s @ 33x`)]`**:
  > *(Voiceover during 18s warp)*: "It verifies that every C snippet inside the Markdown lessons is byte-for-byte identical to the compiled C99 test binaries."
- **`[Stop 9 — All 6 Lessons & C99 Targets Verified]`**:
  > "Zero drift. Now let's tour the compiled interactive course!"

### `S29` — FINALE SHOWCASE 1: Course Hub & 6-Lesson Story Arc (`docs/egm/tutorial/index.html`) (`layout: default` · `45s` · `~105 words`)
- **`[On Load]`**:
  > "Here is the compiled course landing page at `docs/egm/tutorial/index.html`—and we can pop it open directly from the slide badge."
- **`[v-click 1 — Tufte Landing Page & Figure 0.1 Mermaid Story Arc]`**:
  > "Notice the clean **Edward Tufte** typography and the **Figure 0.1 Mermaid story arc**: it walks a human engineer from `Lesson 0001` (breadboard geometry and the `23.6 mA` current budget) through PWM scaling, Q4 EMA filtering, and out-of-tree NuttX BSP bring-up, all the way to `Lesson 0006` (`bargraphd` and the NSH CLI)."
- **`[v-click 2 — Zero-Divergence C99 Gate]`**:
  > "Crucially, tutorials usually rot the moment code changes. Here, `sync_listing.py --check` guarantees every lesson snippet compiles cleanly under strict C99 (`-Wall -Wextra -Werror -pedantic`)."

### `S30` — FINALE SHOWCASE 2: Interactive JS Simulators, Fill-in-the-Blank C99 & Quizzes (`Lesson 0001–0003`) (`layout: default` · `60s` · `~140 words`)
- **`[On Load]`**:
  > "Inside the lessons, you don't just passively skim prose—you play with the hardware physics and the exact bugs the Goldfish caught."
- **`[v-click 1 — Live Hardware & PWM Simulators (Lessons 0001 & 0002)]`**:
  > "In `Lesson 0001`'s `breadboard-simulator`, you drag the `0` to `3300 mV` potentiometer wiper and watch per-pin milliamps update live; in `Lesson 0002`'s `pwm-segment-visualizer`, you scrub across all six `16-bit` (`0..65535`) PWM duty cycles."
- **`[v-click 2 — Turning Goldfish Catches into Interactive Play (Lesson 0003)]`**:
  > "Better yet, look at `Lesson 0003`'s `adc-ema-simulator`: you can toggle a checkbox for Pass 2's **`CA-01` stale `msgs[0]` sample** or flip Pass 1's **`B-2` `>> 2` vs `>> 4` shift bug** and watch the LED bar graph peg at a quarter-turn of the knob!"
- **`[v-click 3 — Tufte Sidenotes, Fill-in-the-Blank C99 & Equal-Length Quizzes]`**:
  > "Right alongside sit Tufte margin sidenotes, fill-in-the-blank C99 exercises, and Matt Pocock's retrieval quizzes where every multiple-choice option is forced to the exact same word count (`10 words`) so you can't guess the longest answer."

### `S31` — FINALE SHOWCASE 3: Teaching Dead Ends (`Lesson 0005`), Printable Bench Sheets & Bonus Bug Catch! (`layout: default` · `45s` · `~105 words`)
- **`[On Load]`**:
  > "Two final touches in the course really surprised me."
- **`[v-click 1 — Dead Ends as Curriculum + Printable A4 Bench Sheets]`**:
  > "First, `Lesson 0005` doesn't hide our `RP23XX_ADC_DIV` false start—it teaches *why* `START_ONCE` ignores the hardware divider, paired with four `@media print` A4 workbench reference sheets (`nuttx-rp2350-driver-traps.md`) you can print out and tape beside your oscilloscope."
- **`[v-click 2 — The Bonus Twist: Docs for Humans Catch Firmware Bugs Too!]`**:
  > "Second, the bonus twist: when `/teach-code` compiled `Lesson 0006`'s strict C99 `pthreads` harness to teach the daemon state machine, the tutorial compiler caught **two real bugs** we missed in Phase 5—including `bargraph test` forgetting to set `MANUAL` mode!"

### `S32` — The "Roll Your Own Docs" Flywheel: Three Habits for Monday Morning (`layout: default` · `30s` · `~70 words`)
- **`[On Load]`**:
  > "That's the **Roll Your Own Docs** flywheel—three habits you can steal on Monday morning:"
- **`[v-click 1 — 1. Feed the Elephant First]`**:
  > "**One: Feed the Elephant first.** Maintain directory-level `README.md` contracts (`/egm-bootstrap` and `/egm-drift-report`) so your agent navigates architecture instead of grepping blind."
- **`[v-click 2 — 2. Unleash Stateless Goldfish Before Coding]`**:
  > "**Two: Unleash stateless Goldfish before coding.** Use `/egm-review` and `/clear` to catch second-order hardware traps in Markdown."
- **`[v-click 3 — 3. Pay Down Comprehension Debt Immediately]`**:
  > "**Three: Pay down comprehension debt immediately.** Run `/teach-code` so a `10/10` AI build becomes a `10/10` mental model in your own head."

### `S33` — Links, Open-Source Skills & Interactive Course (`layout: default` · `19s` · `~45 words`)
- **`[On Load]`**:
  > "Everything you saw tonight—the `nuttxspace` repo, Dave Rensin's `egm` plugin, the `/teach-code` skill (`npx skills add domesticmouse/skills`), all seven terminal casts, and the live six-lesson interactive course—is open source at these links. Thanks heaps for listening, and I'd love to take your questions!"
