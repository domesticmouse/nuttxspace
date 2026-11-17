# Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up — Presentation Map

Label: wayfinder:map
Status: open

## Destination

A complete, slide-by-slide presentation specification and speaker narrative at `.scratch/egm-nuttx-casts-presentation/spec.md` for a 30-minute conference presentation titled **"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"** presented by **Brett Morgan (Google Cloud)**. The presentation focuses on setting up and explaining the sequence of asciinema terminal recordings that capture how the Elephant-Goldfish Model (EGM) and Antigravity were used to tackle embedded electronics (Seeed XIAO RP2350, potentiometer, LED bargraph) and rapidly learn Apache NuttX, avoiding low-level C driver code and tool internals in favour of the AI problem-solving journey.

## Notes

- **Title**: Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up
- **Presenter**: Brett Morgan (Google Cloud)
- **Target format**: 30-minute conference presentation
- **Focus**: Setting up each hardware/NuttX challenge, playing the relevant asciinema terminal recording clip, and explaining the EGM/Antigravity problem-solving outcome.
- **Core topics**:
  - Embedded electronics: Seeed Studio XIAO RP2350, ADC potentiometer, LED bargraph circuit.
  - Apache NuttX: A massive, unfamiliar RTOS with 3,000+ directories and steep learning curve.
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

## Not yet specified

- Exact timing split between spoken setup slides vs live recording playback in the final deck.
- Specific visual assets/diagrams for the breadboard circuit (e.g. SVG photos/renderings vs block diagrams).

## Out of scope

- Low-level C code lines, register manipulation, and kernel driver C syntax debugging.
- Technical explanations of asciinema, pyte, ANSI escape codes, or recording tool internals.
- Re-recording terminal sessions or modifying raw `.cast` files.
- Implementing the Slidev code or CSS (deferred until the specification in `spec.md` is approved).
