# Curate the Sequence of Cast Clips and Stop Points Across All Acts

Type: prototype
Status: resolved
Blocked by: 01, 02

## Question

Which exact clips, chapter stop points, and terminal sequences across Vanilla Vibe, Bootstrap, Brainstorm, Draft, Review, Implement, and Teach-Code will form the live demonstration backbone of the presentation, pairing each with its pre-cast setup and post-cast debrief?

## Answer

Locked **Preset 1 (Cast-Heavy All-7 Live Playback)** in [`cast-curation-prototype.md`](../cast-curation-prototype.md) and [`cast-curation-prototype.html`](../cast-curation-prototype.html):

1. **Three Stage Rules Across All 7 Recordings**:
   - **Start at the AGY Start Page (`0. Start` = Before Prompt 1)**: Casts `1`–`6` (`vanilla-vibe` and `01-bootstrap` through `05-implement`) strip the initial shell `agy` launch sequence so each player opens directly on the settled Antigravity CLI start screen (`0. Start` serves as the "Before-Prompt" stop point for Prompt 1). Cast `7` (`06-teach-code`) preserves the opening `npx skills add domesticmouse/skills` CLI installation sequence (`Stops 0–4`) before landing on the AGY Start Page (`Stop 5`).
   - **Paired Before-Prompt & After-Prompt Stop Points**: Every kept prompt and reply has a **Before-Prompt** stop point (settled empty input box or question) and an **After-Prompt** stop point (prompt/answer filled into the input box, paused before `Enter`).
   - **Fast Prompts (`~0.5s–0.8s` warped / `1.0s–1.6s` raw)**: All hand-typed prompts and answers (`01-bootstrap` Move & Commit, `02-brainstorm` `A1–A6`, `03-draft` `/egm-draft @concepts_brief.md ...`, and `04-review` `Reply: interactive` / `accept`) compress per-keystroke `dt` so typing completes in `< 1s` warped.

2. **Per-Recording Cut List & Stop-Point Budget (`10m 24s` / `624.0s` warped play · `53` stops · `~15m 42s` on stage incl. pauses)**:
   - **Cast 1 — `vanilla-vibe.cast` (`1m 15s` play · `6` stops)**: Starts at AGY Start Page (`0. Start`); keeps Initial Prompt (`Stop 1`), `Q1`/`A1` pin question & reply (`Stops 2–3`), `26m` monolithic firmware build (`Stop 4`), and Breadboard Wiring SVG prompt & completion (`Stops 5–6`).
   - **Cast 2 — `01-bootstrap.cast` (`0m 59s` play · `6` stops)**: Starts at AGY Start Page (`0. Start`); keeps `/plugin` install of `egm` (`Stops 1–3`), `/egm-bootstrap` prompt (`Stop 4`), `Q1: Run EGM Drift Report?` (`Stop 5`), and Drift Report completion (`Stop 6`); trims trailing `Move Drift Report & Commit` turn (`#4070..#5139`).
   - **Cast 3 — `02-brainstorm.cast` (`1m 46s` play · `9` stops)**: Starts at AGY Start Page (`0. Start`); keeps `/egm-brainstorm` prompt (`Stop 1`), `Q1`/`A1` Pin Mapping (`Stops 2–3`), `Q2`/`A2` Boot vs USB-C NSH (`Stops 4–5`), `Q3`/`A3` PWM Bar Graph (`Stops 6–7`), `Q7` Goldfish Readiness Checkpoint (`Stop 8`), and Goldfish Synthesis `concepts_brief.md` (`Stop 9`), with `A1–A3` tightened to `0.8s`; trims middle `Q4–Q6` CLI subcommand negotiation (`#4907..#7395`) and `Stage & Commit` tail (`#11058..#12435`).
   - **Cast 4 — `03-draft.cast` (`1m 55s` play · `6` stops)**: Starts at AGY Start Page (`0. Start`); keeps tightened `/egm-draft` prompt (`Stop 1`), `Q1` Context Verification (`Stop 2`), `Q2` Overview & Goals gate (`Stop 3`), `Q3` Technical Plan gate where the `RP23XX_ADC_DIV` false start enters (`Stop 4`), and `v0-draft` promotion (`Stop 5`); trims `Q4–Q5` section gates (`#6478..#9259`) and `Save v0-draft & Commit` tail (`#13146..#14006`).
   - **Cast 5 — `04-review.cast` (`2m 21s` play · `13` stops)**: Starts at AGY Start Page (`0. Start`); keeps Pass 1 `/egm-review` prompt, mode selection, and `146`-README Goldfish swarm (`Stops 1–3`), Pass 1 `/egm-resolve` prompt & 12 compiled findings (`Stops 4–5`), tightened `Reply: interactive` (`Stop 6`), `Item 1/12 (B-1)` NVIC IRQ gating walk-through & tightened `Reply: accept` (`Stops 7–8`), `Ready for /clear` $\rightarrow$ `Run /clear` $\rightarrow$ `Session Cleared` amnesia reset (`Stops 9–11`), and Pass 2 `/egm-review` prompt & swarm completion catching `CA-01` (`Stops 12–13`); trims items `2–12` + Pass 1 archive (`#14020..#26319`) and Pass 2 auto-resolve/commit tail (`#33141..#37820`).
   - **Cast 6 — `05-implement.cast` (`0m 58s` play · `4` stops)**: Starts at AGY Start Page (`0. Start`); keeps Firmware Implementation prompt & 1st-attempt `make test` + `nuttx.uf2` build (`Stops 1–2`) and Breadboard Wiring Guide & SVG prompt + completion (`Stops 3–4`); trims `Verify Submodules & Commit` tail (`#12402..#14101`).
   - **Cast 7 — `06-teach-code.cast` (`1m 10s` play · `9` stops)**: Starts in shell (`0. Start`) and keeps the full `npx skills add domesticmouse/skills` installation (`Stops 1–4`), transitions to the AGY Start Page (`Stop 5`), and keeps the `/teach-code` 6-lesson generation (`Stops 6–7`) and `sync_listing.py` + `build.py` verification (`Stops 8–9`).
