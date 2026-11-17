# Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up — Presentation Map

Label: wayfinder:map
Status: resolved

## Destination

A complete, slide-by-slide presentation specification and speaker narrative at `.scratch/egm-nuttx-casts-presentation/spec.md` for a 30-minute conference presentation titled **"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"** presented by **Brett Morgan (Google Cloud)**. The presentation focuses on setting up and explaining the sequence of asciinema terminal recordings that capture how the Elephant-Goldfish Model (EGM) and Antigravity were used to tackle embedded electronics (Seeed XIAO RP2350, `10 kΩ` potentiometer, and 6 discrete LEDs acting as a bar graph) and rapidly learn Apache NuttX, avoiding low-level C driver code and tool internals in favour of the AI problem-solving journey.

## Notes

- **Title**: Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up
- **Presenter**: Brett Morgan (Google Cloud)
- **Target format**: 30-minute conference presentation
- **Focus**: Setting up each hardware/NuttX challenge, playing the relevant asciinema terminal recording clip, and explaining the EGM/Antigravity problem-solving outcome.
- **Core topics**:
  - Embedded electronics: Seeed Studio XIAO RP2350, ADC potentiometer (`D0/A0`), and 6 discrete through-hole LEDs (`LED1`–`LED6`: 2 Green, 2 Yellow, 2 Red) acting as a bar graph on a 400-tie-point breadboard.
  - Apache NuttX: A massive, unfamiliar RTOS with 3,241 directories (`13` upstream `README.md` files) and steep learning curve.
  - Antigravity & EGM: The Elephant-Goldfish Model (Elephant context memory via README contracts, Goldfish review swarm, Human ownership) using custom Antigravity skills.
  - Vanilla Vibe Foil: Contrasting unguided LLM vibe coding against structured EGM ramp-up.
- **Standing Visual & Narrative Preferences**:
  - **Low-density slides**: Prefer multiple single-idea slides or progressive `v-click` section reveals over information-dense slides.
  - **Finale Showcase**: Show off the generated 6-lesson `/teach-code` interactive HTML course (`docs/egm/tutorial/index.html`) at the end of the presentation as the finale ("Docs for the AI $\rightarrow$ Docs for the Human").
- **Explicit exclusions (Out of Scope)**:
  - Low-level C code lines, driver register syntax, and kernel C implementation debugging.
  - asciinema/pyte internal tooling mechanics.
- **Skills to consult**:
  - `bjm-voice` (`.agents/skills/bjm-voice/SKILL.md`): Australian English, conversational, candid, engineering peer tone.
  - `grilling` (`.agents/skills/grilling/SKILL.md`) and `domain-modeling` (`.agents/skills/domain-modeling/SKILL.md`) for HITL tickets (one question at a time).
  - `prototype` (`.agents/skills/prototype/SKILL.md`) for outline and slide structure.
  - `asciinema` (`.agents/skills/asciinema/SKILL.md`) and `cast-player-html` (`.agents/skills/cast-player-html/SKILL.md`) for reference on existing recordings and stop points.

## Decisions so far

<!-- the index: one line per closed ticket, enough to judge relevance, then zoom the link for the detail the ticket holds -->

- [Catalog Existing Cast Recordings, Stop Points, and Terminal Events](./issues/01-catalog-cast-recording-stop-points.md): Cataloged all 7 recording prototypes in `prototypes/` with raw vs warped runtimes, segment counts, chapter markers, and narrative beats across baseline vibe coding, EGM Phases 1–5, and the /teach-code course.
- [Staging the Hardware Challenge and NuttX RTOS Complexity on Stage](./issues/02-hardware-and-nuttx-challenge-staging.md): Frame Act I as a real-world engineering stress test using high-level pin/interface diagrams + generated breadboard SVGs (XIAO RP2350, pot on `D0/A0`, 6 discrete LEDs as bar graph), stage NuttX via the `3,241` directories vs `13` READMEs gap plus 3 conceptual booby traps (USB-C boot hang, ADC pin clobber & RP2350-E9 leakage, `500k IRQ/s` storm), and play the `vanilla-vibe` foil immediately after setup (`5/10` scorecard) with a finale callback.
- [Curate the Sequence of Cast Clips and Stop Points Across All Acts](./issues/03-curate-cast-recording-segments.md): Locked Preset 1 cast-heavy live playback across all 7 recordings (`10m 24s` warped play, `53` stops, `~15m 42s` on stage in [cast-curation-prototype.md](./cast-curation-prototype.md)): casts 1–6 start on the AGY Start Page (`06-teach-code` keeps `npx skills add`), every prompt/reply has paired Before/After stop points, hand-typed prompts are tightened to `~0.5s–0.8s`, and low-signal middle/commit tails (`02-brainstorm` `Q4–Q6`, `03-draft` `Q4–Q5`, `04-review` items `2–12` & Pass 2 resolve, and `01`–`05` git commits) are trimmed.
- [Explaining EGM and Antigravity Mechanics on Stage](./issues/04-explain-egm-and-antigravity-on-stage.md): Use low-density slides and progressive `v-click` reveals (1 Triad anchor slide at Act III start + just-in-time setup/debrief slides per cast): unpack the Elephant via 3 visual beats (Tree Map, 3-click stylized Contract Anatomy, Bootstrap/Drift pipeline), plant the `RP23XX_ADC_DIV` false start at `03-draft` Stop 4 to illustrate Author Bias/Sycophancy, stage the Goldfish around `04-review` via 3-click Swarm Anatomy + `B-1` $\rightarrow$ `/clear` $\rightarrow$ `CA-01` second-order catch, deliver the candid `5/10` vs `10/10` (`6 Pass / 4 Partial / 0 Fail` strict) scorecard after `05-implement`, and finish by bridging "Docs for the AI" to "Docs for the Human" (`06-teach-code`) and showing off the generated interactive HTML lessons (`docs/egm/tutorial/index.html`) as the finale.
- [Prototype the Slide Structure, Stage Beats, and Timing Budget](./issues/05-slide-structure-and-pacing-outline.md): Locked Preset A (`33` low-density slides across 4 modular act files in [slide-structure-prototype.md](./slide-structure-prototype.md): `26` progressive `v-click` slides averaging `~2.1` beats/slide + `7` `cast-full` slides) budgeted at `29m 51s` (`14m 09s` slides/showcase + `15m 42s` live casts, `9s` buffer), culminating in a 3-slide Finale Showcase (`S29`–`S31`, `2m 30s`) with a clickable `Open ./tutorial/index.html ↗` launch badge touring the Course Hub, the 3 live JS simulators (`breadboard-simulator`, `pwm-segment-visualizer`, `adc-ema-simulator`) & equal-length quizzes, the printable `@media print` A4 bench sheets, and `Lesson 0006`'s strict C99 bug catch.
- [Draft the Spoken Script and Narrator Voice in bjm-voice](./issues/06-speaker-script-and-narrative-bjm-voice.md): Scripted the full verbatim Australian English (`bjm-voice`, `~140 wpm`) narrative in [speaker-script-bjm-voice.md](./speaker-script-bjm-voice.md) synced to every `[On Load]` / `[v-click]` beat across all `26` concept/setup/debrief/showcase slides plus dual-layer commentary across all `7` live `.cast` slides (`53` paused stop-point cues + `25s–30s` warped play-by-play voiceovers), crediting Dave Rensin (`@drensin`), Matt Pocock (`/teach`), and Edward Tufte while owning all four candid false-start/audit beats.
- [Assemble and Verify the Presentation Specification at spec.md](./issues/07-assemble-and-verify-presentation-spec.md): Compiled and verified the authoritative 33-slide presentation specification at [spec.md](./spec.md), uniting the `29m 51s` timing budget (`14m 09s` slides/showcase + `15m 42s` across `7` live `.cast` recordings and `53` paired stop points), visual layouts and asset paths (`breadboard-vanilla-crop-ch.svg`, `breadboard-wiring-crop-ef.svg`, `./tutorial/index.html`), and verbatim `bjm-voice` speaker script while verifying all scope exclusions.

## Not yet specified

_None — all remaining steps to the destination are now specified as tickets._

## Out of scope

- Low-level C code lines, register manipulation, and kernel driver C syntax debugging.
- Technical explanations of asciinema, pyte, ANSI escape codes, or recording tool internals.
- Re-recording terminal sessions from scratch (existing `.cast` recordings will be trimmed, prompt-tightened, and re-marked per [Curate the Sequence of Cast Clips and Stop Points Across All Acts](./issues/03-curate-cast-recording-segments.md)).
- Implementing the Slidev code or CSS (deferred until the specification in `spec.md` is approved).
