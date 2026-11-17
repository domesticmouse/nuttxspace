# Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up — Presentation Map

Label: wayfinder:map
Status: open

## Destination

A complete, slide-by-slide presentation specification and speaker narrative at `.scratch/roll-your-own-docs-presentation/spec.md` for a 30-minute conference talk titled **"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"**, showing general software engineers how to use EGM (`3,241` `README.md` contracts + `/egm-bootstrap`, `/egm-drift-report`, `/egm-brainstorm`, `/egm-draft`, `/egm-review`, `/egm-resolve`) to rapidly ramp up on a large, unfamiliar legacy codebase (Apache NuttX + RP2350 bare-metal drivers, contrasted with `nuttxspace-vanilla` vibe coding) and then turn that codebase understanding into human documentation—a 6-lesson interactive codelab built with `/teach-code` (`docs/egm/tutorial/`).

## Notes

- **Primary narrative angle**: Using EGM to come up to speed with a large codebase with deep history (`nuttx/` + `apps/` + RP2350 hardware), contrasted against flying blind with vanilla vibe coding (`nuttxspace-vanilla`), and then turning that hard-won architectural understanding into durable human documentation (the `/teach-code` interactive codelab in `docs/egm/tutorial/`).
- **EGM foundational lineage (Dave Rensin)**: Credits Dave Rensin (`@drensin`) and his seminal essay [*"Elephants, Goldfish, and the New Golden Age of Software Engineering"*](https://drensin.medium.com/elephants-goldfish-and-the-new-golden-age-of-software-engineering-c33641a48874) (`https://github.com/google/elephant-goldfish-model`, `/Users/brett/.gemini/config/plugins/egm/docs/index.md` & `designs/architecture.md`):
  - **Core thesis**: *"Design is the new code"* and *"Design with an elephant. Review with a goldfish."*
  - **Failure modes of single-session AI coding**: *Hallucination Cascades & Context Pollution* (one thread interviews, designs, and codes, validating its own faulty assumptions), *Premature Implementation ("Weeds Syndrome")*, and *Sycophancy*.
  - **Three roles**: **The Elephant** (*remembers everything*: Steps 1–4 — Context Loading via `README.md` contracts, the *"No Code Yet"* rule, *The Sycophant Challenge* / 7-lens `/egm-brainstorm` shootdown, and `/egm-draft`), **The Goldfish** (*three-second memory*: Steps 5–9 `/egm-review` — stateless, zero-context subagents running Zoom Out, Comprehension, Critic, and Implementation Readiness / Codebase Details checks without conversational bias), and **The Human** (*"Humans own the outcomes"* — resolving trade-offs via `/egm-resolve` and owning the mental model via `/teach-code`).
- **Skill lineage (`/teach` → `/teach-code`)**: `/teach-code` (`.agents/skills/teach-code/SKILL.md`) is derived from Matt Pocock's `/teach` (`.agents/skills/teach/SKILL.md`) through repeated real-world use and gentle application of `/agy-customizations` (`.gemini/antigravity/builtin/skills/agy-customizations/SKILL.md`), adding `build.py --check`, `scripts/sync_listing.py`, and runnable per-lesson `apps/<NNNN-slug>/` companion projects verified by strict compiler/linter gates alongside `/teach`'s `MISSION.md`, `RESOURCES.md`, `learning-records/`, `reference/`, Tufte layout, and equal-word-count retrieval quizzes.
- **Talk format & constraints**: 30-minute conference talk for general software engineers (no prior Apache NuttX or RP2350 knowledge assumed), featuring live warped playback of selected `"m"` chapter markers from `prototypes/*-cast-player.html`.
- **Slide density & pacing rule**: Target a visual transition (a fresh low-density `[Slide]`, a piece-by-piece `[v-click]` reveal, or a Shiki `[Code-step]` line highlight) every **5 to 10 seconds** (`~7.8s` average across `208` visual steps + `6` `[Live-cast]` segments = `214` total stage steps `S001`–`S214`), never putting dense multi-column tables, walls of text, or un-stepped code blocks on screen at once.
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

- [Narrative arc and 30-minute time budget](./issues/01-narrative-arc-and-time-budget.md): 5-act structure (3m Hook, 5m Vanilla Foil, 11m EGM Ramp-Up [2.0/3.0/4.5/1.5m], 6m /teach-code Codelab, 5m Honest Retro & Skill Evolution [3.5/1.5m]); "Roll Your Own Docs" lands for the agent (3,241 README contracts + EGM specs), the human (6-lesson /teach-code codelab), and the skills themselves (Matt Pocock's /teach → /teach-code via /agy-customizations).
- [Select the hero codebase and hardware traps to feature on stage](./issues/02-hero-technical-traps-selection.md): Primary spine is the 500 kHz `START_ONCE` ADC storm (`40 Hz` `close(fd)` race → `RP23XX_ADC_DIV` false start → Pass 1 NVIC IRQ gating `B-1` & Pass 2 25-byte short-read loop `CA-01`), supported by 3 contrast traps (`CONFIG_ANALOG=y` silent drop, `my_reset()` + `RP2350-E9` `IE=1` leakage, `B-2` 4x Q4 EMA bug) and 4 "nobody is immune" self-corrections (`bargraph test` `MANUAL` clobber, `Makefile` `printf` duplication, shared `0.6"` XIAO breadboard straddle miss, `up_enable_irq` before `close(adc_fd)` shutdown edge case).
- [Choreograph live cast-player recordings and stop points](./issues/03-cast-player-choreography.md): `3m 02s` (`181.6s`) total live warped playback across 4 players (`vanilla-vibe` Seg 03–05 `[44.4s @ 2x/52x]`, `04-review` Seg 05 `[29.7s @ 39x]` + Seg 09–10 `B-1` `[18.6s @ 2x]` + Seg 14–16 `/clear` & Seg 18 Pass 2 `[30.1s @ 2x/26x]`, `05-implement` Seg 04 `[29.0s @ 27x]`, `06-teach-code` Seg 08 `[29.8s @ 79x]`); `01-bootstrap` (Stop Point 8), `02-brainstorm` (Stop Point 16), and `03-draft` (Stop Point 6) shown as chapter-pill freeze-frames paired with doc excerpts.
- [Select generated EGM docs and teach-code codelab artifacts to showcase](./issues/04-codelab-and-generated-docs-showcase.md): Act III pairs cast beats with 6 EGM doc moments (`rp23xx/README.md` + `DRIFT_REPORT.md` → `concepts_brief.md` + `v0-draft` false start → Pass 1 `B-1..B-3` & Pass 2 `CA-01` Goldfish reports + hardened spec → `xiao-rp2350-bargraph/README.md` + `breadboard-wiring.svg`); Act IV pairs `/teach` → `/teach-code` skill diff (`~2m`) with a 4-stop `docs/egm/tutorial/` walkthrough (`index.html` + `MISSION.md`/ADR → Lesson 0003 `adc-ema-simulator` + Tufte quiz → Lesson 0005 + printable `Driver Traps` sheet → Lesson 0006 C99 lab fixing `bargraph test`); Act V.b pairs trace `25-98ffad8f` with `build.py` validators + `teach-code/SKILL.md` updates.
- [Prototype the slide-by-slide outline and timing skeleton](./issues/05-slide-by-slide-outline-prototype.md): Locked 214 stage steps (S001–S214) across 26 narrative beats in [outline-prototype.md](./outline-prototype.md) — 208 low-density visual transitions averaging 7.8s (96 [Slide], 88 [v-click], 24 [Code-step], all 7.0s–9.0s) + 6 [Live-cast] segments (181.6s) + 3 freeze-frame players, grounded in Dave Rensin's Elephant / Goldfish / Human framework and Matt Pocock's /teach → /teach-code evolution.
- [Specify exact code snippets, report excerpts, and SVG callouts for each visual step](./issues/06-exact-visual-callouts-and-code-excerpts.md): Locked all 24 Shiki [Code-step] progressions (3–8 lines each across Acts II–V), 3-field Goldfish finding cards (B-1..B-3, CA-01), single-row telemetry/scorecard reveals, breadboard SVG viewBox crops (0 0 1160 1120 full, 150 130 500 500 EGM Cols E&F 0.3", 120 180 320 320 Vanilla c1..h7 0.7"), and contract/codelab callout regions.
- [Shape the spoken narrative, act transitions, and bjm-voice phrasing](./issues/07-speaker-narrative-and-transitions.md): Locked 16–24 word spoken cadence in Australian English (`bjm-voice`), prompt-and-effect voiceover cues for all 6 live cast windows and 3 freeze-frames (never reading off screen), 5 act transitions, fair credit to Vanilla (`D3–D8` & `bargraph_logic.c`), and candid self-corrections (`RP23XX_ADC_DIV` approval, Phase 5 shortcuts, `up_enable_irq` exit race, `0.6"` XIAO breadboard straddle, `25-98ffad8f` HTML gate) plus Dave Rensin & Matt Pocock credits.

## Not yet specified

_None — all remaining steps to the destination are now specified as tickets._

## Out of scope

- Building, styling, or deploying the actual slide deck implementation (happens after `.scratch/roll-your-own-docs-presentation/spec.md` is complete).
- Recording new `asciinema` `.cast` files or modifying the archived experiment snapshots in `docs/vanilla/` and `docs/egm/`.
