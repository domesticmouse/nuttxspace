# Skills Three Ways — Presentation Specification

- **Deck Path:** `slides-skills-three-ways/`
- **Target Duration:** 40–45 minutes (`~42m 30s` total stage budget: `~26m 48s` slide discussion, anatomy walkthroughs, prototypes & live tutorial showcase + `~15m 42s` across 7 interactive `.cast` recordings with 53 stop points)
- **Slide Count:** 41 slides across 5 modular act files (`acts/01-hook-and-setup.md` through `acts/05-way3-custom-skills-finale.md`)

---

## Act & Timing Summary

| Act | File | Slides | Focus | Est. Stage Time |
| :--- | :--- | :--- | :--- | :--- |
| **Act I** | `acts/01-hook-and-setup.md` | `S01`–`S08` (8) | Apache NuttX scale (`3,241` dirs / `13` READMEs), breadboard wiring, 3 silicon/RTOS traps, `0` skills vs. Three Ways | `~6m 00s` |
| **Act II** | `acts/02-vanilla-and-anatomy.md` | `S09`–`S14` (6) | `vanilla-vibe.cast` (`5/10` scorecard, `0.7"` straddle), `SKILL.md` X-ray, meta-skill pivot (`/agy-customizations`, `/writing-for-agents`) | `~7m 30s` |
| **Act III** | `acts/03-way1-egm-plugin.md` | `S15`–`S28` (14) | Way 1 (`/plugin` &rarr; `egm`), Elephant + Goldfish triad, casts `01`–`05`, `B-1` & `/clear` &rarr; `CA-01`, `10/10` compile, `0.6"` XIAO & `6/4/0` audit | `~16m 00s` |
| **Act IV** | `acts/04-way2-community-skills.md` | `S29`–`S32` (4) | Way 2 (`npx skills add mattpocock/skills`), `skills-lock.json` pinning, 7-line `/grill-me`, `/wayfinder` Fog of war, `25 MB` log meta-story, clickable `/prototype` HTML planners | `~5m 00s` |
| **Act V** | `acts/05-way3-custom-skills-finale.md` | `S33`–`S41` (9) | Way 3 Story A (`domesticmouse/skills` talk tools) & Story B (`/teach` &rarr; `/teach-code`), `06-teach-code.cast`, 3-slide `./tutorial/index.html` tour, flywheel & links | `~8m 00s` |
| **Total** | `slides.md` | **41 slides** | **7 live `.cast` recordings + 2 prototypes + 6-lesson course** | **`~42m 30s`** |

---

## Slide Index

### Act I — Hardware Hook & Controlled Experiment (`acts/01-hook-and-setup.md`)
- `S01` (`center`) — **Skills Three Ways**: Adopting, Orchestrating, and Authoring Modular AI Workflows on Real Silicon
- `S02` (`default`) — The Stack: Apache NuttX & Google Antigravity
- `S03` (`default`) — Apache NuttX at a Glance: Project Scale & Lineage (`75,800+` commits, `12,139 .c` / `8,801 .h`, `3,241` dirs, `13` READMEs)
- `S04` (`default`) — How a Solderless Breadboard Works (`/images/breadboard-scopes.svg`)
- `S05` (`center`) — Physical Breadboard Wiring (`/images/assets-breadboard-wiring.svg`)
- `S06` (`default`) — The Hardware Target: XIAO RP2350 & 6-LED Bar Graph
- `S07` (`default`) — Three Hardware & RTOS Traps Waiting on the Bench
- `S08` (`default`) — The Controlled Experiment: `0` Skills vs. Skills Three Ways

### Act II — Baseline Foil + Skill Anatomy & Meta-Skill Pivot (`acts/02-vanilla-and-anatomy.md`)
- `S09` (`default`) — Baseline Foil: Unconstrained Antigravity ("Weeds Syndrome")
- `S10` (`cast-full`) — Baseline Foil: Vanilla Vibe Coding (`vanilla-vibe.cast`)
- `S11` (`default`) — Vanilla Vibe's Generated Breadboard Wiring (`/images/breadboard_circuit.svg`)
- `S12` (`hardware-callout`) — Debrief: The Lie of "It Compiled!" (`5 / 10` Scorecard, `breadboard-vanilla-crop-ch.svg`)
- `S13` (`skill-breakdown`, `way: mashup`) — X-Raying a Skill: How `skills/<name>/SKILL.md` Works
- `S14` (`center`) — The Meta-Skill Pivot: *"That schema is just an implementation detail, which the tools already know, thanks to skills."* (`/agy-customizations` + `/writing-for-agents`)

### Act III — Way 1: Multi-Agent Workflow Plugins (`acts/03-way1-egm-plugin.md`)
- `S15` (`default`) — Way 1: Multi-Agent Workflow Plugins (`/plugin` &rarr; `egm`)
- `S16` (`skill-breakdown`, `way: 1`) — Way 1 Architecture: Directory Contracts + Bundled Subagent Personas
- `S17` (`default`) — Keeping the Elephant Honest: `/plugin` &rarr; `/egm-bootstrap` &rarr; `/egm-drift-report`
- `S18` (`cast-full`) — Way 1 Phase 1: Bootstrap & Drift Report (`01-bootstrap.cast`)
- `S19` (`default`) — The Specification Funnel: Human + Elephant (`/egm-brainstorm`)
- `S20` (`cast-full`) — Way 1 Phase 2: Grounded Brainstorming (`02-brainstorm.cast`)
- `S21` (`default`) — Section-by-Section Drafting & Planting the `RP23XX_ADC_DIV` Trap
- `S22` (`cast-full`) — Way 1 Phase 3: Section-by-Section Spec Drafting (`03-draft.cast`)
- `S23` (`default`) — Author Bias vs. Stateless Goldfish Swarm (`/egm-review`)
- `S24` (`cast-full`) — Way 1 Phase 4: Two-Pass Goldfish Swarm & `/clear` Amnesia (`04-review.cast`)
- `S25` (`default`) — Second-Order Catch (`CA-01`) &rarr; Boring First-Try Compile
- `S26` (`cast-full`) — Way 1 Phase 5: First-Try Firmware Build & Breadboard SVG (`05-implement.cast`)
- `S27` (`default`) — EGM's Generated Breadboard Wiring (`/images/breadboard-wiring.svg`)
- `S28` (`hardware-callout`) — Way 1 Scorecard: `10 / 10` Hardware Wins & Candid Checks (`breadboard-wiring-crop-ef.svg`, `0.6"` XIAO footprint, `6 Pass / 4 Partial / 0 Fail`)

### Act IV — Way 2: Curated Community Skills & `/wayfinder` (`acts/04-way2-community-skills.md`)
- `S29` (`default`) — Way 2: Curated Community Skills (`npx skills add mattpocock/skills`, `skills-lock.json` SHA-256 pinning)
- `S30` (`skill-breakdown`, `way: 2`) — Way 2 X-Ray: 7-Line Micro-Skills (`/grill-me`) & Router Skills (`/wayfinder`)
- `S31` (`default`) — Meta-Story: Using Skills to Make a Talk About Skills (`228m+` / `25 MB` logs, `.scratch/egm-nuttx-casts-presentation/map.md`, `issues/01..07.md`)
- `S32` (`default`) — High-Fidelity Decisions with `/prototype` (clickable `./prototypes/cast-curation-prototype.html` and `./prototypes/slide-structure-prototype.html`)

### Act V — Way 3: Custom Modular Skills + `/teach-code` Finale (`acts/05-way3-custom-skills-finale.md`)
- `S33` (`skill-breakdown`, `way: 3`) — Way 3 (Story A): Packaging Talk-Production Tooling into `domesticmouse/skills` (`/asciinema`, `/cast-player-html`, `/bjm-voice`, `/slidev-deck`, `/crop-image`, `/agy-brain-explorer`)
- `S34` (`default`) — Way 3 (Story B): The Final Boss — Comprehension Debt (`872` lines of `bargraph_main.c`)
- `S35` (`skill-breakdown`, `way: 3`) — Way 3 (Story B): Evolving Matt Pocock's `/teach` into `/teach-code`
- `S36` (`cast-full`) — Way 3 in Action: Installing & Running `/teach-code` (`06-teach-code.cast`)
- `S37` (`default`) — Finale Showcase 1: Course Hub & 6-Lesson Story Arc (`./tutorial/index.html`)
- `S38` (`default`) — Finale Showcase 2: Interactive JS Simulators, C99 Exercises & Quizzes (`0001`, `0002`, `0003`)
- `S39` (`default`) — Finale Showcase 3: Teaching Dead Ends, Printable Bench Sheets & Bug Catch! (`Lesson 0005`, `A4 Traps Sheet`, `Lesson 0006` 2 real bug catches)
- `S40` (`default`) — The "Skills Three Ways" Flywheel: Monday Morning Habits
- `S41` (`default`) — Links, Open-Source Skills, Interactive Course & Prototypes
