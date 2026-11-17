# Shape the spoken narrative, act transitions, and bjm-voice phrasing

Type: grilling
Label: wayfinder:grilling
Status: resolved
Blocked by: 05, 06

## Question

How should the spoken presenter narrative, act transitions, live cast-player voiceover cues, and candid self-deprecating moments (the `RP23XX_ADC_DIV` Elephant blind spot, Phase 5's `Makefile` `printf` and `bargraph test` `MANUAL` shortcuts caught in Lesson 0006, the 1-line `up_enable_irq` before `close(adc_fd)` strict post-audit catch, and the shared `0.6"` XIAO breadboard straddle miss) be phrased in Brett's Australian-English voice (`bjm-voice`) across the `214` stage steps from [Prototype the slide-by-slide outline and timing skeleton](./05-slide-by-slide-outline-prototype.md) and [Specify exact code snippets, report excerpts, and SVG callouts for each visual step](./06-exact-visual-callouts-and-code-excerpts.md)?

## Answer

### 1. Spoken Cadence, Act I Hook & Five Act Transitions (`S001`–`S214`)

- **Spoken cadence (`bjm-voice`)**:
  - `16–24` words per `7.0s–9.0s` visual step (`~140–160` words/minute spoken aloud) across the `208` visual transitions, written as natural, first-person conversational flow across each beat rather than slide-caption bullets.
  - Consistent Australian English (`realise`, `colour`, `labelled`, `organisation`, `modelled`, `analyse`, `AU$`) with zero corporate AI slop or unblemished-triumph framing.
- **Act I opening hook (`S001`–`S023`)**:
  - Open on the relatable weekend-hardware truth: a crisp `AU$` Seeed Studio XIAO RP2350 and six LEDs on your desk vs. your coding agent waking up blind inside 30 years of Apache NuttX C (`13` upstream `README.md` files vs. `3,241` directory contracts).
  - Spring the deceptive `"BUILD SUCCEEDED: nuttx/nuttx.uf2"` + blinking LEDs trap (*"In embedded systems—and any deep legacy codebase—'it compiles and blinks' is the most dangerous lie an AI accomplice can tell you"*), then establish the three-layer *"Roll Your Own Docs"* frame grounded in Dave Rensin's Elephant / Goldfish / Human roles and Matt Pocock's `/teach`.
- **Five act transitions**:
  - **Act I → Act II (`S023`)**: *"Let's roll the tape on what happens when you skip all three layers and vibe code straight into C."*
  - **Act II → Act III (`S057`)**: *"Same hardware, same prompt—now let's roll contracts for the Elephant before writing a single line of C."*
  - **Act III → Act IV (`S132`)**: *"The agent nailed the firmware; now comes the harder question—do you actually understand the 800 lines of C you're about to ship?"*
  - **Act IV → Act V (`S176`–`S177`)**: *"Now let's put both runs side by side—telemetry, strict post-audit catches, and the physical blunder neither workflow spotted."*
  - **Act V.a → Act V.b (`S203`–`S204`)**: *"And here is how you close the loop—turning every production wart into a deterministic gate in your own `SKILL.md`."*

### 2. Prompt-and-Effect Voiceover Choreography for `[Live-cast]` Windows (`181.6s`) & Freeze-Frames (`0s`)

During every `[Live-cast]` playback window and freeze-frame, presenter commentary talks through **the prompt or slash command fed to the agent and the behavioural/architectural effect it is having on the workflow**—never reading scrolling terminal text off the screen:

1. **`S026` — `prototypes/vanilla-vibe-cast-player.html` (`44.4s`: Seg 03–05 `@ 2x/52x`)**:
   - Talk through handing the raw `EXPERIMENT_RUNBOOK.md` prompt straight to an unprimed thread: it asks one superficial pin question (`Q1`/`A1` at `2x`), skips every architectural invariant (`my_reset()`, `START_ONCE`, `RP2350-E9`), and spends `25m 51s` (`52x`) grepping `66` files with `0` `nuttx`/`apps` `README.md` reads before `olddefconfig` drops `CONFIG_ADC=y` at Step 180 and it ships a deceptive `.uf2`.
2. **`S066`, `S075`, `S086` — Freeze-frames (`0s` playback)**:
   - **`S066` (`01-bootstrap-cast-player.html`, Stop Point `8`)**: Talk through how `/egm-bootstrap` + `/egm-drift-report` anchor the workspace root with `6` contract reads and flag `OSINIT_HARDWARE` drift before planning begins.
   - **`S075` (`02-brainstorm-cast-player.html`, Stop Point `16`)**: Talk through how `/egm-brainstorm` enforces Rensin's *"No Code Yet"* rule and the 7-lens *Sycophant Challenge* across `19` Socratic checkpoints and `49` `README.md` reads.
   - **`S086` (`03-draft-cast-player.html`, Stop Point `6`)**: Talk through how `/egm-draft` pauses section-by-section for human sign-off (`53` cumulative reads)—and where both human and Elephant still click Approve on the `RP23XX_ADC_DIV` false start.
3. **`S099`, `S104`, `S119` — `prototypes/04-review-cast-player.html` (`78.4s` across 3 beats)**:
   - **Beat A (`S099`, `29.7s @ 39x` Seg 05)**: Explain the effect of invoking `/egm-review`—spawning `16` stateless, three-second-memory Goldfish with zero loyalty to `v0-draft` to audit the draft against `146` `README.md` contracts (`21m 33s` raw).
   - **Beat B (`S104`, `18.6s @ 2x` Seg 09–10)**: Explain how `/egm-resolve` in `interactive` mode forces the human to confront each Blocker one at a time as `B-1` dismantles `RP23XX_ADC_DIV` and we type `accept`.
   - **Beat C (`S119`, `30.1s` Seg 14–16 `/clear` `@ 2x` + Seg 18 `@ 26x`)**: Explain why we issue `/clear` before re-running `/egm-review`—wiping parent context so the session that patched `B-1` cannot grade its own homework, allowing Pass 2 (`26x`) to catch the `5`-byte `adc_read()` short-read (`CA-01`).
4. **`S125` — `prototypes/05-implement-cast-player.html` (`29.0s @ 27x` Seg 04)**:
   - Explain the effect of handing a Pass-2-hardened spec to implementation: turning `13m 04s` of C coding into calm, boring execution that builds `nuttx/nuttx.uf2` (`347K`) cleanly on Attempt #1 (`0` firmware retries).
5. **`S150` — `prototypes/06-teach-code-cast-player.html` (`29.8s @ 79x` Seg 08)**:
   - Explain what invoking `/teach-code` triggers across `39m 11s` (`79x`)—synthesising `6` Tufte lessons, `3` interactive JS widgets, `4` printable bench sheets, and running `build.py --check`, `sync_listing.py`, and `6` strict `-Wall -Wextra -Werror -pedantic` C99 companion labs before flipping to Chrome.

### 3. Candid Self-Deprecating Moments, Fair Credit to Vanilla & Peer Acknowledgements (`bjm-voice`)

- **Fair credit to Vanilla (`S050`–`S051`)**: Credit `nuttxspace-vanilla` before scoring it `5 / 10`—it picked `D3–D8` (dodging `GPIO 26–29` clobber) and wrote a cleaner shared `src/bargraph_logic.c` host unit-test split than EGM Phase 5's initial `Makefile`.
- **Shared human + Elephant `RP23XX_ADC_DIV` blind spot (`S088`–`S095`, `S103`–`S106`)**: Deliver in first person as a joint mistake—the Elephant found `RP23XX_ADC_DIV`, wrote out persuasive `732.42 Hz` arithmetic (`0xFFFF00`), and Brett nodded along and clicked Approve before a zero-context Goldfish (`B-1`) read `rp23xx_adc.c` cold and proved `RP23XX_ADC_DIV` is a no-op under `START_ONCE`.
- **Phase 5 shortcuts caught by Lesson 0006 (`S173`–`S176`)**: Candidly own Phase 5's `printf`-synthesised `/tmp/bargraph_test.c` in `Makefile` and the `bargraph test` `MANUAL`-clobber bug (`bargraph_start_daemon()` resetting `MANUAL` to `SENSING` before `prev_mode` was saved), both exposed and fixed when Lesson 0006 (`0006-bargraphd-daemon-and-nsh-cli`) forced a real C99 daemon test suite.
- **Strict post-audit catch (`S187`–`S194`) & shared `0.6"` XIAO breadboard blunder (`S131`, `S196`–`S203`)**:
  - Own the 1-line shutdown race (`up_enable_irq(RP23XX_ADC_IRQ_FIFO)` called one line before `close(adc_fd)` on exit) caught by turning read-only Goldfish loose on our own `10 / 10` primary-pass code.
  - Deliver the `0.6"` breadboard punchline with self-deprecating humour: Vanilla drew `0.7"` (`c1..h1`, one hole too wide), while EGM drew `0.3"` (`Cols E & F`, half-width) and stretched the SVG centre ravine from `35px` to `120px` so the diagram looked right—landing the *Law of Contracts* (contracts eliminate guessing inside their boundary; outside it, verify with a physical ruler).
- **Post-recording HTML wart (`25-98ffad8f`, `S204`–`S209`) & peer credits (`S017`, `S138`, `S212`)**: Show how spotting `<2 id="breadboard-topology">` and raw `$R_S$` TeX in Chrome became permanent `MALFORMED_TAG_RE` + `UNRENDERED_TEX_RE` gates in `build.py` and an upstream guardrail in `teach-code/SKILL.md` via `/agy-customizations`, while warmly crediting Dave Rensin (`@drensin`) for the Elephant-Goldfish Model and Matt Pocock for `/teach`.
