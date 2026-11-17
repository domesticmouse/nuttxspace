# Narrative arc and 30-minute time budget

Type: grilling
Label: wayfinder:grilling
Status: resolved
Blocked by: none

## Question

How should the 30-minute talk be structured into acts and time-budgeted so the title *"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"* lands naturally?

Specifically:
- How do we frame the two halves of "Roll Your Own Docs" (Part 1: rolling architectural `README.md` contracts + EGM design/review docs to ramp up on a massive legacy codebase like Apache NuttX; Part 2: rolling an interactive `/teach-code` codelab from that understanding to solve human comprehension debt)?
- How many minutes go to the Hook & Title Bridge, the Vanilla Vibe-Coding Foil (`nuttxspace-vanilla`), the EGM Codebase Ramp-Up (`01-bootstrap` through `05-implement`), the `/teach-code` Codelab Payoff (`06-teach-code`), and the Honest Retro / Telemetry Scorecard (`docs/reports/vanilla-vs-egm-comparison.md`)?


## Answer

### Double Meaning of *"Roll Your Own Docs"*

1. **Roll your own docs for the agent**: Bottom-up architectural `README.md` contracts (`3,241` total: `2,607` in `nuttx/`, `634` in `apps/`) plus EGM drift, brainstorm, draft, and review specs so the agent understands a 30-year legacy RTOS and RP2350 silicon errata before writing a line of C.
2. **Roll your own docs for the human**: Turning that verified architecture into a 6-lesson interactive Tufte codelab (`docs/egm/tutorial/`) via `/teach-code` so the engineer doesn't drown in comprehension debt—while also rolling and evolving the custom skills themselves (Matt Pocock's `/teach` evolving into `/teach-code` through repeated real-world use and gentle application of `/agy-customizations`).

### 5-Act Time Budget (`30.0 min` total)

- **Act I — The Hook & The Title's Double Meaning (`3.0 min`, `00:00–03:00`)**:
  - Open with the joy and pain of diving into a brand-new hardware/RTOS stack (Seeed Studio XIAO RP2350 + Apache NuttX, where upstream carries just `13` `README.md` files across `nuttx/` and `apps/`).
  - Frame the core thesis: docs for the agent first, then docs for the human.
- **Act II — The Foil: Flying Blind with Vanilla Vibe Coding (`5.0 min`, `03:00–08:00`)**:
  - Show `nuttxspace-vanilla` (`1` parent session, `46m 40s` raw, `0` `nuttx`/`apps` `README.md` reads).
  - It compiles and blinks LEDs—which feels like victory—until an audit uncovers silent Kconfig drops (`CONFIG_ADC` without `CONFIG_ANALOG=y`), Erratum `RP2350-E9` `IE=1` leakage on `GPIO 26`, and a 40 Hz `close(fd)` ISR teardown race (`5 / 10` primary scorecard).
- **Act III — Rolling Docs for the Agent: EGM Codebase Ramp-Up (`11.0 min`, `08:00–19:00`)**:
  - **III.a — Grounding the Map (`2.0 min`, `08:00–10:00`)**: `01-bootstrap` & `/egm-drift-report` — why `3,241` bottom-up `README.md` contracts beat grepping raw C, and how the root contract plus drift report anchor the workspace.
  - **III.b — From Vibe to Architecture (`3.0 min`, `10:00–13:00`)**: `02-brainstorm` & `03-draft` (`53` `README.md` reads across both phases) — designing `xiao-rp2350-bargraph.md`, including the honest false start assuming `RP23XX_ADC_DIV` would throttle ADC interrupts.
  - **III.c — The Goldfish Swarm (`4.5 min`, `13:00–17:30`)**: `04-review` (Pass 1 + `/clear` + Pass 2) — fresh-context Goldfish subagents (`16` in `04-review`, `17` across the run) reading `146` `README.md` contracts catch `4` Blockers (`B-1..B-4`, proving `RP23XX_ADC_DIV` is a no-op under `START_ONCE`) and `7` Majors (`M-1..M-5`, `CA-01..CA-02`) before writing a single line of C.
  - **III.d — Boring Implementation (`1.5 min`, `17:30–19:00`)**: `05-implement` — because the traps were caught in the spec, `nuttx/nuttx.uf2` (`347K`) builds cleanly on the very first attempt (`0` firmware build failures, `10 / 10` primary scorecard).
- **Act IV — Rolling Docs for the Human: The `/teach-code` Codelab (`6.0 min`, `19:00–25:00`)**:
  - Confront the comprehension debt of shipping working C code you didn't hand-write.
  - Credit Matt Pocock's `/teach` (`.agents/skills/teach/SKILL.md`) for the pedagogical backbone (`MISSION.md`, Tufte sidenotes, equal-word-count retrieval quizzes, Zone of Proximal Development) and show how repeated real-world use plus gentle `/agy-customizations` refinements evolved it into `/teach-code` (`.agents/skills/teach-code/SKILL.md`): adding `build.py --check`, byte-for-byte `sync_listing.py`, and runnable `apps/0001..0006/` C99 companion labs that caught and fixed two subtle Phase 5 firmware bugs (`Makefile` `printf` test duplication and `bargraph test` clobbering `MANUAL` mode).
- **Act V — The Honest Retro & Telemetry Scorecard (`5.0 min`, `25:00–30:00`)**:
  - **V.a — Head-to-Head Telemetry & Shared Blind Spots (`3.5 min`, `25:00–28:30`)**: Compare `46m 40s` vs `196m 37s` raw, `5 / 10` vs `10 / 10` primary scorecard, and the strict Goldfish post-audit (`4 Pass / 4 Partial / 2 Fail` vs `6 Pass / 4 Partial / 0 Fail` in `docs/reports/vanilla-vs-egm-comparison.md`), including the shared blind spot where *both* agents got the physical `0.6"` XIAO breadboard width wrong (`0.7"` in Vanilla, `0.3"` in EGM).
  - **V.b — Takeaway: Evolving Your Own `SKILL.md` Workflow (`1.5 min`, `28:30–30:00`)**: Show how anyone in the audience can start from sharp community skills like `/teach` and gently iterate them with `/agy-customizations` as real projects rub off the rough edges.
