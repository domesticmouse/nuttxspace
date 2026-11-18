# Skills Three Ways — Presentation Map

Label: wayfinder:map
Status: resolved

## Destination

A complete, standalone 40–45 minute Slidev conference deck in `slides-skills-three-ways/` titled **"Skills Three Ways"** presented by **Brett Morgan (Google Cloud)**, following the **Adopt &rarr; Orchestrate &rarr; Author** narrative arc across 5 acts and 41 slides while preserving the existing `"Roll Your Own Docs"` deck in `slides/` untouched.

## Talk Title & Brief

- **Title:** Skills Three Ways
- **Abstract:** Skills are transforming the way developers delegate complex, repetitive engineering workflows to AI systems. This talk unpacks the anatomy of Skills, evaluating how they structure tool use, context framing, and task execution. Attendees will learn how to identify high quality community skills, inspect their implementation details, integrate them into active development environments, and build modular, reusable skills from scratch to accelerate daily software delivery.
- **Target Duration:** 40–45 minutes (`~42m 30s` paced across 41 slides, 7 live `.cast` recordings, HTML planning prototypes, and the 3-slide `./tutorial/index.html` showcase)

## Notes

- **Locked Narrative Structure (Adopt &rarr; Orchestrate &rarr; Author)**:
  1. **Act I — Hardware Hook & Controlled Experiment (`0` Skills vs. Skills Three Ways)** (`S01`–`S08`)
  2. **Act II — Baseline Foil + Skill Anatomy & The Meta-Skill Pivot** (`S09`–`S14`)
  3. **Act III — Way 1: Multi-Agent Workflow Plugins (`/plugin` &rarr; `egm`)** (`S15`–`S28`)
  4. **Act IV — Way 2: Curated Community Skills (`npx skills add mattpocock/skills`) & `/wayfinder`** (`S29`–`S32`)
  5. **Act V — Way 3: Building & Evolving Custom Modular Skills (`/agy-customizations` &rarr; `domesticmouse/skills`) + `/teach-code` Finale** (`S33`–`S41`)
- **Shared Source Assets (Read-Only Reuse)**:
  - Recordings: `recordings/final/{vanilla-vibe,01-bootstrap,02-brainstorm,03-draft,04-review,05-implement,06-teach-code}.cast`
  - Hardware & Logo Visuals: `docs/assets/*`, `docs/vanilla/breadboard_circuit.svg`, `docs/egm/breadboard-wiring.svg`
  - Interactive HTML Prototypes: `.scratch/egm-nuttx-casts-presentation/{cast-curation-prototype,slide-structure-prototype}.html`
  - Interactive Course: `docs/egm/tutorial/` compiled via `build.py --check`
- **Voice & Design Conventions**:
  - `.agents/skills/bjm-voice/SKILL.md`: Australian English (*realise*, *colour*, *labelled*, *synthesise*), curious-explorer peer tone, candid about false starts (`RP23XX_ADC_DIV`, `0.6"` XIAO footprint, `6 Pass / 4 Partial / 0 Fail`).
  - `.agents/skills/slidev-deck/SKILL.md`: Low-density progressive `v-click` slides, clean `<h2>` headings, flush code fences, and conversational spoken notes.

## Decisions so far

- **Deck Isolation (`slides-skills-three-ways/`)**: Keep `slides/` ("Roll Your Own Docs") untouched as a sibling deck; wire `slides-skills-three-ways/scripts/sync-assets.py` to copy casts, SVGs/crops, HTML prototypes into `public/prototypes/`, and compiled tutorial HTML into `public/tutorial/`.
- **Skill Anatomy & Meta-Skill Pivot (`S13`–`S14`)**: X-ray `skills/<name>/SKILL.md` (progressive disclosure YAML pointers, step completion criteria + `references/`, deterministic `scripts/` + `resources/`) right after the `5/10` Vanilla Vibe debrief, then deliver the punchline (*"That schema is just an implementation detail, which the tools already know, thanks to skills"*) pointing to `/agy-customizations` and `/writing-for-agents`.
- **Way 1 (`S15`–`S28`)**: Reframe the 5-cast `egm` journey (`01-bootstrap` through `05-implement`) as adopting a bundled multi-agent plugin pairing `3,241` directory contracts ("The Elephant") with 10 stateless subagent personas ("The Goldfish") and `/clear` amnesia to catch `B-1` and `CA-01` before `10/10` first-try compile, while preserving the candid `0.6"` XIAO footprint check and `6 Pass / 4 Partial / 0 Fail` post-build audit.
- **Way 2 (`S29`–`S32`)**: Highlight supply-chain verification in `skills-lock.json` (`27` skills from `mattpocock/skills` pinned by commit SHA and SHA-256 `computedHash`), inspect the 7-line `/grill-me` micro-skill and `/wayfinder` router skill ("Fog of war" quarantine), tell the meta-story of taming `228m+` (`25 MB`) of raw casts across 7 decision tickets (`issues/01..07.md`), and add clickable pop-out links to `./prototypes/cast-curation-prototype.html` and `./prototypes/slide-structure-prototype.html`.
- **Way 3 & Finale (`S33`–`S41`)**: Pair **Story A** (talk-production skills packaged into `domesticmouse/skills`: `/asciinema`, `/cast-player-html`, `/bjm-voice`, `/slidev-deck`, `/crop-image`, `/agy-brain-explorer`) with **Story B** (solving comprehension debt across `872` lines of `bargraph_main.c` by evolving `/teach` into `/teach-code`, playing `06-teach-code.cast`, touring `./tutorial/index.html` across `S37`–`S39` including Lesson 0006's 2 Phase 5 bug catches, and closing on the Monday-morning flywheel and links).

## Not yet specified

_None — specification verified against `slides-skills-three-ways/` implementation._

## Out of scope

- Modifying or overwriting `slides/` ("Roll Your Own Docs") or mutating canonical `.cast` files in `recordings/final/`.
