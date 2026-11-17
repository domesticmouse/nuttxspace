# Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up — Presentation Map

Label: wayfinder:map
Status: open

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

## Not yet specified

_None — all remaining steps to the destination are now specified as tickets._

## Out of scope

- Low-level C code lines, register manipulation, and kernel driver C syntax debugging.
- Technical explanations of asciinema, pyte, ANSI escape codes, or recording tool internals.
- Re-recording terminal sessions from scratch (existing `.cast` recordings will be trimmed, prompt-tightened, and re-marked per [Curate the Sequence of Cast Clips and Stop Points Across All Acts](./issues/03-curate-cast-recording-segments.md)).
- Implementing the Slidev code or CSS (deferred until the specification in `spec.md` is approved).
