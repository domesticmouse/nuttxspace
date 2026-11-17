# Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up — Presentation Map

Label: wayfinder:map
Status: open

## Destination

A complete, slide-by-slide presentation specification and speaker narrative at `.scratch/roll-your-own-docs-presentation/spec.md` for a 30-minute conference talk titled **"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"**, showing general software engineers how to use EGM (`3,241` `README.md` contracts + `/egm-bootstrap`, `/egm-drift-report`, `/egm-brainstorm`, `/egm-draft`, `/egm-review`, `/egm-resolve`) to rapidly ramp up on a large, unfamiliar legacy codebase (Apache NuttX + RP2350 bare-metal drivers, contrasted with `nuttxspace-vanilla` vibe coding) and then turn that codebase understanding into human documentation—a 6-lesson interactive codelab built with `/teach-code` (`docs/egm/tutorial/`).

## Notes

- **Primary narrative angle**: Using EGM to come up to speed with a large codebase with deep history (`nuttx/` + `apps/` + RP2350 hardware), contrasted against flying blind with vanilla vibe coding (`nuttxspace-vanilla`), and then turning that hard-won architectural understanding into durable human documentation (the `/teach-code` interactive codelab in `docs/egm/tutorial/`).
- **Talk format & constraints**: 30-minute conference talk for general software engineers (no prior Apache NuttX or RP2350 knowledge assumed), featuring live warped playback of selected `"m"` chapter markers from `prototypes/*-cast-player.html`.
- **Skills every session should consult**:
  - `bjm-voice` (`.agents/skills/bjm-voice/SKILL.md`): Australian English spelling (*realise*, *colour*, *labelled*, *organisation*), curious-explorer / experienced-peer tone, honest about false starts and strict-audit warts, zero corporate AI slop.
  - `grilling` (`.agents/skills/grilling/SKILL.md`) and `domain-modeling` (`.agents/skills/domain-modeling/SKILL.md`) for HITL decision tickets (always ask clarifying questions **one at a time** per user global rule).
  - `prototype` (`.agents/skills/prototype/SKILL.md`) for outline/structure prototyping tickets.
- **Primary source artifacts in repo**:
  - `EXPERIMENT_RUNBOOK.md`
  - `docs/reports/vanilla-vs-egm-comparison.md`
  - `docs/egm/DOCS_REVIEW.md` and `docs/egm/reviews/01-bootstrap-review.md` .. `06-teach-code-review.md`
  - `docs/vanilla/CODE_REVIEW.md`, `docs/vanilla/BREADBOARD_WIRING.md`, `docs/vanilla/breadboard_circuit.svg`
  - `docs/egm/BREADBOARD_WIRING.md`, `docs/egm/breadboard-wiring.svg`, `docs/egm/tutorial/`
  - `prototypes/vanilla-vibe-cast-player.html` and `prototypes/01-bootstrap-cast-player.html` .. `06-teach-code-cast-player.html`

## Decisions so far

<!-- the index: one line per closed ticket, enough to judge relevance, then zoom the link for the detail the ticket holds -->

## Not yet specified

- Detailed slide-by-slide speaker script, transitions, and Australian-English phrasing in Brett's voice (`bjm-voice`) once the slide outline prototype is settled.
- Exact visual callout specifications for each slide (which code snippets, Kconfig diffs, Goldfish report excerpts, telemetry comparison rows, and breadboard SVG callouts appear on screen).
- Final assembly and verification of `.scratch/roll-your-own-docs-presentation/spec.md` once the outline and visual callout decisions are locked.

## Out of scope

- Building, styling, or deploying the actual slide deck implementation (happens after `.scratch/roll-your-own-docs-presentation/spec.md` is complete).
- Recording new `asciinema` `.cast` files or modifying the archived experiment snapshots in `docs/vanilla/` and `docs/egm/`.
