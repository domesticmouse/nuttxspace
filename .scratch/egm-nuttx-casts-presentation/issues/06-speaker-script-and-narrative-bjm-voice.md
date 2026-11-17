# Draft the Spoken Script and Narrator Voice in bjm-voice

Type: grilling
Status: resolved
Blocked by: 05

## Question

What is the spoken narrative script for the presenter across all acts in `bjm-voice` (Australian English, authentic, pragmatic, candid about AI strengths and failures) to guide the audience through setting up each recording, reacting to the terminal runs, and explaining the key takeaways?

## Answer

Locked the full verbatim spoken script and dual-layer live cast commentary in [`speaker-script-bjm-voice.md`](../speaker-script-bjm-voice.md):

1. **Full Verbatim Script + Dual-Layer Cast Commentary (`~135–145 wpm` in Australian English `bjm-voice`)**:
   - **Concept, Setup & Debrief Slides (`26` slides, `14m 09s`)**: Word-for-word conversational script in Australian English (*G'day*, *AU$8*, *memorised*, *realise*, *colour*, *labelled*, *centre*, *fibreglass*), tagged inline with `[On Load]` and `[v-click 1..N]` beats ready for Slidev `<!-- ... -->` presenter notes.
   - **Live `.cast` Player Slides (`7` slides, `53` stops, `15m 42s` stage time)**: Dual-layer live narration combining (a) **crisp 1-sentence stop-point cues** while paused at each Before-Prompt / After-Prompt marker right before pressing `p`, and (b) **timed `~25s–30s` warped play-by-play voiceovers** during the high-speed runs (`6x`–`79x`) so there is never dead silence on stage.

2. **Opening Hook, Persona Framing & Explicit Community Credits**:
   - **Act I (`S01`–`S05`)**: Opens with *"G'day everyone"* and frames the weekend hardware itch (`AU$8` Seeed Studio XIAO RP2350, `10 kΩ` potentiometer on `D0/A0`, 6 PWM LEDs on a 400-tie-point breadboard) inside **Apache NuttX** (30 years of POSIX RTOS C across `3,241` directories with `13` upstream `README.md` files that Brett cheerfully admits he hasn't memorised), using Google Antigravity as his accomplice.
   - **Dave Rensin (`@drensin`) & EGM Credits**: Credits Dave Rensin for *"Weeds Syndrome"* in Act II (`S06`), the EGM maxim *"Design is the new code: design with an Elephant, review with a Goldfish"* (`S10`), shared-context confirmation bias (`S18`), and *"Humans Own the Outcomes"* (`S27`).
   - **Matt Pocock & Edward Tufte Credits**: Credits Matt Pocock's open-source `/teach` skill as the foundation adapted (via `/agy-customizations`) into `/teach-code` (`S27`, `S30`), preserving Zone of Proximal Development learning records, **Edward Tufte** right-margin sidenotes, and equal-word-count (`10 words/option`) retrieval quizzes alongside strict C99 zero-divergence verification (`sync_listing.py --check` + `build.py --check`).

3. **Four Candid Non-Triumph & Self-Deprecating Ownership Beats**:
   - **Beat 1 (`S08`–`S09`, Vanilla Vibe `5/10`)**: Gives Vanilla Vibe fair credit for clean host C math, Q4 EMA filtering, `7/7` host unit tests, and a first-try `nuttx.uf2` compile before revealing the `0.7"` breadboard straddle across **Columns C to H** (`breadboard-vanilla-crop-ch.svg`) and the `5/10` silicon traps (`RP2350-E9` leakage, `40 Hz` `open()`/`close()` ISR teardown race).
   - **Beat 2 (`S16`–`S18`, Owning the `RP23XX_ADC_DIV` False Start)**: Narrates `03-draft` Stop 4 in the first person, owning how both the Elephant and the human approved `CONFIG_RP23XX_ADC_DIV` before realizing single-shot `START_ONCE` ignores the hardware divider and still fires `500,000 IRQ/s`.
   - **Beat 3 (`S20`–`S22`, `/clear` Amnesia & Second-Order Catch `CA-01`)**: Explains how Pass 1's `B-1` fix (reading a 5-sample / `25-byte` NVIC IRQ-gated burst) created a new deadlock against NuttX's `8-sample` (`7` usable) software FIFO (`CA-01`)—caught only because `/clear` wiped session memory before Pass 2.
   - **Beat 4 (`S26` & `S31`, Strict Post-Build Audit & `Lesson 0006` Bug Twist)**: Balances the `5/10` vs `10/10` primary trap win with the cold post-build Goldfish audit (`6 Pass / 4 Partial / 0 Fail`, flagging `SIGTERM` cleanup and Makefile test scaffolding) at `S26`, and reveals at `S31` how compiling `Lesson 0006`'s strict C99 `pthreads` harness caught 2 real Phase 5 `bargraph test` bugs (`MANUAL`-mode clobber).
