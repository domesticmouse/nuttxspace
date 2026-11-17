# Choreograph live cast-player recordings and stop points

Type: grilling
Label: wayfinder:grilling
Status: resolved
Blocked by: 01, 02

## Question

We have 7 standalone HTML players in `prototypes/` (`vanilla-vibe-cast-player.html` with 7 stop points, plus `01-bootstrap` through `06-teach-code` with 83 stop points across `16m 02s` of warped playback). In a 30-minute talk, playing every stop point would consume over half the slot.

Which specific players get launched live vs. summarized via slides, and which exact `"m"` stop-point sequences should the speaker step through in each live player to support the decisions made in [Narrative arc and 30-minute time budget](./01-narrative-arc-and-time-budget.md) and [Select the hero codebase and hardware traps to feature on stage](./02-hero-technical-traps-selection.md)?

## Answer

### 1. Seven-Player Timing Inventory (`2.0x` min speed, `30s` max segment cap, `2.0s` idle cap)

| Player | Act | Stop Points | Raw Duration | Full Warped Playback | Stage Mode |
| :--- | :--- | ---: | ---: | ---: | :--- |
| `prototypes/vanilla-vibe-cast-player.html` | Act II | 7 | 31m 57s (`1916.6s`) | 1m 17s (`77.0s`) | **Live (`44.4s`)** |
| `prototypes/01-bootstrap-cast-player.html` | Act III.a | 11 | 8m 14s (`493.6s`) | 1m 39s (`98.8s`) | **Freeze-frame (`0s`)** |
| `prototypes/02-brainstorm-cast-player.html` | Act III.b | 19 | 41m 32s (`2491.7s`) | 3m 48s (`228.0s`) | **Freeze-frame (`0s`)** |
| `prototypes/03-draft-cast-player.html` | Act III.b | 11 | 21m 35s (`1295.4s`) | 3m 20s (`200.4s`) | **Freeze-frame (`0s`)** |
| `prototypes/04-review-cast-player.html` | Act III.c | 24 | 62m 13s (`3733.1s`) | 4m 37s (`277.3s`) | **Live (`78.4s`)** |
| `prototypes/05-implement-cast-player.html` | Act III.d | 8 | 21m 03s (`1262.7s`) | 1m 28s (`88.0s`) | **Live (`29.0s`)** |
| `prototypes/06-teach-code-cast-player.html` | Act IV | 10 | 42m 01s (`2520.5s`) | 1m 10s (`69.5s`) | **Live (`29.8s`)** |
| **Total** | — | **90** | **228m 34s (`13713.6s`)** | **17m 19s (`1039.0s`)** | **`3m 02s` (`181.6s`) live** |

### 2. Live vs. Summarised Split

- **4 Live Warped Players (`181.6s` / `3m 02s` total live playback)**: `vanilla-vibe-cast-player.html`, `04-review-cast-player.html`, `05-implement-cast-player.html`, and `06-teach-code-cast-player.html` run live warped segments (`[` / `]` / `p` + chapter-pill jumps) at the high-drama action beats.
- **3 Freeze-Frame Summarised Players (`0s` live playback)**: `01-bootstrap-cast-player.html`, `02-brainstorm-cast-player.html`, and `03-draft-cast-player.html` are shown as chapter-pill freeze-frames paired with generated document excerpts so Acts III.a–III.b stay crisp inside their `2.0 + 3.0 min` budget.

### 3. Exact Stop-Point Choreography by Act

1. **Act II (`03:00–08:00`) — `prototypes/vanilla-vibe-cast-player.html` (`44.4s` live playback)**
   - Jump (`]`) to Stop Point `2. Initial Prompt: XIAO RP2350 Bar Graph` (`00:02` warped).
   - Press `p` to play **Seg 03** (`14.1s @ 2x` → `3. Q1: Breadboard Pins & LED Polarity`), **Seg 04** (`0.5s @ 2x` → `4. A1: Any Free Breadboard Pins`), and **Seg 05** (`29.8s @ 52x`, compressing `25m 51s` raw → auto-pauses at `5. Firmware & Tests Built`).
   - Stop player at Stop Point `5` (skipping the `29.8s` SVG generation segment) and pivot to the Act II audit slides (`0` `README.md` reads, `500 kHz` `START_ONCE` race, `RP2350-E9`, `CONFIG_ANALOG=y` drop).

2. **Acts III.a & III.b (`08:00–13:00`) — Freeze-frame stop points for the 3 summarised players (`0s` live playback)**
   - **`prototypes/01-bootstrap-cast-player.html` (Act III.a)**: Freeze-frame at Stop Point `8. Q1: Run EGM Drift Report?` (`00:38` warped, showing the `7x` speed badge after ingesting the root contracts), paired with the root `README.md` and drift report excerpt.
   - **`prototypes/02-brainstorm-cast-player.html` (Act III.b)**: Freeze-frame at Stop Point `16. Q7: Readiness Checkpoint (Review with Goldfishes)` (`02:53` warped, highlighting the 19-pill Socratic interview bar vs. Vanilla's single question).
   - **`prototypes/03-draft-cast-player.html` (Act III.b)**: Freeze-frame at Stop Point `6. Q3: Section Approval — Technical Plan` (`01:30` warped, `15x` badge), paired with the `v0-draft` spec callout containing the `RP23XX_ADC_DIV` false-start assumption.

3. **Act III.c (`13:00–17:30`) — `prototypes/04-review-cast-player.html` (`78.4s` live playback across 3 beats)**
   - **Beat A — Pass 1 Goldfish Swarm (`29.7s`)**: Jump to Stop Point `4. Pass 1 Q1: Select Review Execution Mode (Consolidated)` (`00:11` warped) and press `p` to play **Seg 05** (`29.7s @ 39x`, compressing `21m 33s` raw across `146` `README.md` reads → auto-pauses at `5. Pass 1 Review Complete`).
   - **Beat B — Interactive `B-1` Rescue (`18.6s`)**: Click pill `8. Reply: interactive (Walk Through 12 Findings)` (`01:10` warped) and press `p` through **Seg 09** (`17.1s @ 2x` → `9. Item 1/12 (B-1): NVIC IRQ Gating vs. RP23XX_ADC_DIV`, proving `RP23XX_ADC_DIV` is a no-op under `START_ONCE`) and **Seg 10** (`1.5s @ 2x` → `10. Reply: accept`).
   - **Beat C — `/clear` Context Wipe & Pass 2 Swarm (`30.1s`)**: Click pill `13. Pass 1 Archived — Ready for /clear` (`02:17` warped) and press `p` through **Seg 14–16** (`1.1s @ 2x`, showing `/clear` + `Pass 2 /egm-review (with GoldfishContext)`), then click pill `17. Pass 2 Q1: Select Review Execution Mode (Consolidated)` (`02:41` warped) and press `p` to play **Seg 18** (`29.0s @ 26x` → auto-pauses at `18. Pass 2 Review Complete`, catching `CA-01`).

4. **Act III.d (`17:30–19:00`) — `prototypes/05-implement-cast-player.html` (`29.0s` live playback)**
   - Jump to Stop Point `3. Prompt: Implement Firmware & Verify Build` (`00:04` warped) and press `p` to play **Seg 04** (`29.0s @ 27x`, compressing `13m 04s` raw → auto-pauses at `4. Firmware Built & Tested`, with `nuttx/nuttx.uf2` (`347K`) building cleanly on the first attempt).

5. **Act IV (`19:00–25:00`) — `prototypes/06-teach-code-cast-player.html` (`29.8s` live playback)**
   - Jump to Stop Point `7. Prompt: /teach-code Story Arc in ./tutorial/` (`00:10` warped) and press `p` to play **Seg 08** (`29.8s @ 79x`, compressing `39m 11s` raw → auto-pauses at `8. 6-Lesson Course Generated`), then switch straight to the live browser codelab (`docs/egm/tutorial/index.html`).
