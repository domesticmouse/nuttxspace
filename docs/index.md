# Roll Your Own Docs & NuttX Workspace (`nuttxspace`)

Companion site and source workspace for the conference talk **"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"**.

Drop an AI coding agent cold into **Apache NuttX**—nearly 20 years of C across `3,241` directories with only `13` upstream `README.md` files—and ask it to build out-of-tree firmware for a **Seeed Studio XIAO RP2350** wired to a potentiometer and six breadboard LEDs, and it will happily compile code hiding real silicon and interrupt traps. This repository captures both sides of fixing that problem:

1. **Docs for the AI** — using Dave Rensin's **Elephant-Goldfish Model (EGM)** to map the codebase with directory-level `README.md` contracts (*the Elephant*) and stress-test specifications with stateless, zero-context reviewer subagents (*the Goldfish*) before writing C.
2. **Docs for the Human** — using `/teach-code` (adapted from Matt Pocock's `/teach`) to turn the finished firmware into a self-contained, interactive 6-lesson course so the engineer at the keyboard actually owns the mental model.

## Start Here

| If you want to… | Jump straight to |
| :--- | :--- |
| **Walk through the talk** | [**Live Slidev Presentation Deck**](../slides/slides.md) · [Verbatim Speaker Script](../.scratch/egm-nuttx-casts-presentation/speaker-script-bjm-voice.md) |
| **Explore the `/teach-code` course** | [**Interactive 6-Lesson Course (Tufte HTML)**](egm/tutorial/index.html) *(hosted site)* · [Course Overview & Source](egm/tutorial/README.md) |
| **See Vanilla vs. EGM side by side** | [**Vanilla vs. EGM Comparison Report**](reports/vanilla-vs-egm-comparison.md) · [Comparative Experiment Runbook](../EXPERIMENT_RUNBOOK.md) |
| **Wire and flash a XIAO RP2350** | [**From Zero to `xiao-rp2350-bargraph`**](tutorials/xiao-rp2350-bargraph.md) · [Quickstart](#quickstart-building-the-live-pilot-firmware) |

---

## Conference Talk: "Roll Your Own Docs"

- [**Live Slidev Presentation Deck**](../slides/slides.md) ([Slide Overview](../slides/slides.md#/overview) · [Presenter Console](../slides/slides.md#/presenter)) — Interactive dark-mode conference deck with `7` embedded `asciinema` recordings (`53` stop points), progressive `v-click` reveals, breadboard SVG comparisons, and the `/teach-code` course showcase.
- [**Verbatim Speaker Script (`bjm-voice`)**](../.scratch/egm-nuttx-casts-presentation/speaker-script-bjm-voice.md) — Full spoken narrative in Australian English with live cast voiceover cues.
- [**Authoritative Presentation Specification**](../.scratch/egm-nuttx-casts-presentation/spec.md) — Slide-by-slide staging rules, timing budget, and cast trim specifications.

### Talk Planning & Staging Artefacts

- [Presentation Wayfinder Map](../.scratch/egm-nuttx-casts-presentation/map.md) and staging tickets: [01 Cast Catalog](../.scratch/egm-nuttx-casts-presentation/issues/01-catalog-cast-recording-stop-points.md) · [02 Hardware & NuttX Staging](../.scratch/egm-nuttx-casts-presentation/issues/02-hardware-and-nuttx-challenge-staging.md) · [03 Cast Curation](../.scratch/egm-nuttx-casts-presentation/issues/03-curate-cast-recording-segments.md) · [04 Staging EGM Mechanics](../.scratch/egm-nuttx-casts-presentation/issues/04-explain-egm-and-antigravity-on-stage.md) · [05 Slide Structure & Pacing](../.scratch/egm-nuttx-casts-presentation/issues/05-slide-structure-and-pacing-outline.md) · [06 Speaker Script](../.scratch/egm-nuttx-casts-presentation/issues/06-speaker-script-and-narrative-bjm-voice.md) · [07 Spec Assembly & Audit](../.scratch/egm-nuttx-casts-presentation/issues/07-assemble-and-verify-presentation-spec.md)
- Interactive planning prototypes: [Slide Structure & Timing Planner (`MD`)](../.scratch/egm-nuttx-casts-presentation/slide-structure-prototype.md) · [Timing Planner (`HTML`)](../.scratch/egm-nuttx-casts-presentation/slide-structure-prototype.html) · [Cast Curation Schedule (`MD`)](../.scratch/egm-nuttx-casts-presentation/cast-curation-prototype.md) · [Cast Curation Matrix (`HTML`)](../.scratch/egm-nuttx-casts-presentation/cast-curation-prototype.html)

---

## Comparative Experiment: Vanilla Vibe vs. Full EGM

To test EGM fairly against unconstrained vibe coding, the same XIAO RP2350 bar-graph prompt was run in two isolated workspaces—**Experiment A (`nuttxspace-vanilla`)** on upstream `master` (`13` `README.md` files) and **Experiment B (`nuttxspace-egm`)** on the `egm` branch (`3,241` directory `README.md` contracts)—with full `asciinema` recordings and exported Antigravity session traces.

- [**Vanilla vs. EGM Comparison Report**](reports/vanilla-vs-egm-comparison.md) — Independent read-only Goldfish audit scoring both captured builds across 10 hardware/RTOS traps (`5/10` with two high-severity defects for Vanilla vs. `10/10` primary pass and `6 Pass / 4 Partial / 0 Fail` strict audit for EGM) and reconciling recording and trace telemetry.
- [**Comparative Experiment Runbook**](../EXPERIMENT_RUNBOOK.md) — End-to-end operator checklist, recording workflow, trace correlations, and scoring rubric.

### Standalone `asciinema` Cast Players

- **Run 1 (Vanilla Vibe):** [`vanilla-vibe-cast-player.html`](../prototypes/vanilla-vibe-cast-player.html)
- **Run 2 (EGM Phases 0–6):** [`01-bootstrap`](../prototypes/01-bootstrap-cast-player.html) *(Phase 0 `/plugin` install + Phase 1 bootstrap & drift report)* · [`02-brainstorm`](../prototypes/02-brainstorm-cast-player.html) · [`03-draft`](../prototypes/03-draft-cast-player.html) · [`04-review`](../prototypes/04-review-cast-player.html) · [`05-implement`](../prototypes/05-implement-cast-player.html) · [`06-teach-code`](../prototypes/06-teach-code-cast-player.html)

### Experiment B: Full EGM Flow & `/teach-code` (`nuttxspace-egm`)

Featured on stage: a **50 Hz proportional 16-bit PWM** bar-graph daemon (`bargraphd` on `D1–D6`) that compiled `nuttx.uf2` on the first attempt, followed by a generated 6-lesson interactive course.

- **6-Lesson `/teach-code` Course:** [**Interactive Tufte HTML Course**](egm/tutorial/index.html) *(hosted site)* · [Course Overview (`README`)](egm/tutorial/README.md)
- **Overall & Per-Phase Reviews:** [Phases 0–6 Documentation & Implementation Review](egm/DOCS_REVIEW.md) · [Workspace Blueprint](egm/README.md) · [Breadboard Wiring Guide](egm/BREADBOARD_WIRING.md)
  - Phase reviews: [01 Bootstrap](egm/reviews/01-bootstrap-review.md) · [02 Brainstorm](egm/reviews/02-brainstorm-review.md) · [03 Draft](egm/reviews/03-draft-review.md) · [04 Review](egm/reviews/04-review-review.md) · [05 Implement](egm/reviews/05-implement-review.md) · [06 `/teach-code`](egm/reviews/06-teach-code-review.md)
- **Specification Evolution:** [Concepts Brief](egm/designs/concepts_brief.md) → [Initial Draft](egm/designs/design_draft.md) → [v0 Spec](egm/designs/xiao-rp2350-bargraph.v0-draft.md) → [Hardened Design Spec](egm/designs/xiao-rp2350-bargraph.md)
- **Drift & Goldfish Audits:** [Drift Report (2026-11-16)](egm/reports/2026-11-16_DRIFT_REPORT.md) · [Pass 1 Goldfish Report](egm/reports/pass1/goldfish_review_report.md) · [Pass 2 Goldfish Report](egm/reports/pass2/goldfish_review_report.md)
- **Captured Source & Traces:** [Captured EGM Firmware & 9 Contracts](egm/xiao-rp2350-bargraph/README.md) · [EGM Session Traces (`01`–`25`)](prompts-egm/README.md)

### Experiment A: Vanilla Vibe Coding (`nuttxspace-vanilla`)

Single-session unconstrained build using discrete `/dev/userleds` (`bargraph_d` on `D3–D8`), archived unmodified for comparison.

- [Vanilla Technical Code Review](vanilla/CODE_REVIEW.md) · [Breadboard Wiring Guide](vanilla/BREADBOARD_WIRING.md)
- [Captured Vanilla Source Snapshot](vanilla/xiao-rp2350-bargraph/README.md) · [Vanilla Session Trace (`01`)](prompts-vanilla/README.md)

---

## Original Pilot Build & Tutorials (Live Workspace)

This repository contains **three** separate XIAO RP2350 bar-graph implementations—two archived snapshots from the controlled experiment above, plus the **initial pilot firmware** that lives directly in [`xiao-rp2350-bargraph/`](../xiao-rp2350-bargraph) and builds out of the box in this workspace:

| Build | Location | Behaviour & Pins | Traces |
| :--- | :--- | :--- | :--- |
| **Original Pilot (live)** | [`xiao-rp2350-bargraph/`](../xiao-rp2350-bargraph) | 20 Hz discrete `/dev/userleds` (`bargraph` on `D3–D5`, `D8–D10`) | [`prompts/`](prompts/README.md) (`47` sessions) |
| **Experiment A (snapshot)** | [`docs/vanilla/xiao-rp2350-bargraph/`](vanilla/xiao-rp2350-bargraph/README.md) | Discrete `/dev/userleds` (`bargraph_d` on `D3–D8`) | [`prompts-vanilla/`](prompts-vanilla/README.md) (`1` session) |
| **Experiment B (snapshot)** | [`docs/egm/xiao-rp2350-bargraph/`](egm/xiao-rp2350-bargraph/README.md) | 50 Hz proportional 16-bit PWM (`bargraphd` on `D1–D6`) | [`prompts-egm/`](prompts-egm/README.md) (`25` sessions) |

The tutorials, design documents, review reports, and `47` session traces below all belong to the **original pilot run** (`2026-11-13`) that preceded the recorded experiment.

### Long-Form Tutorials

- [**From Zero to `xiao-rp2350-bargraph`**](tutorials/xiao-rp2350-bargraph.md) — End-to-end guide for software engineers covering breadboard wiring, out-of-tree NuttX board configuration, RP2350 ADC/LED driver traps, host unit testing, and flashing `nuttx.uf2`.
- [**Replicating the NuttX EGM Experiment**](tutorials/egm-replication-tutorial.md) — Replay guide and candid post-mortem analysing how the Elephant-Goldfish Model workflow (`47` sessions, `4,478` logged steps) navigated `nuttx/` and `apps/` to produce the pilot firmware.

### Pilot Designs, Goldfish Audits & Session Traces

- **Architecture & Designs:** [Final Design Spec (`xiao-rp2350-bargraph`)](designs/xiao-rp2350-bargraph.md) · [Concepts Brief](designs/concepts_brief.md) · [Initial Design Draft](designs/design_draft.md) · [Build Environment Setup (Fedora 44 & macOS)](designs/TODO.md)
- **EGM Drift & Consolidated Audits:** [Drift Report (2026-11-13)](reports/2026-11-13_DRIFT_REPORT.md) · [Pass 2 Consolidated Review (`C-01`–`C-22`)](reports/consolidated_review_report.md) · [Pass 3 Goldfish Review (`CON-1`–`CON-19`)](reports/goldfish_review_report.md) *(caught the ADC FIFO ISR `close()` race and blocking `read()` hang before coding)*
- **Individual Pilot Persona Reports:** [Comprehension](reports/comprehension_report.md) · [Design Critic](reports/critic_report.md) · [Implementation Readiness](reports/implementation_readiness_report.md) · [Zoom Out](reports/zoom_out_report.md) · [Intermediate Zoom Out](reports/intermediate_zoom_out_report.md)
- **Exported Conversations:** [Project Conversation Logs Index (`01`–`47`)](prompts/README.md)

---

## Quickstart: Building the Live Pilot Firmware

These commands build and verify the live out-of-tree pilot firmware in [`xiao-rp2350-bargraph/`](../xiao-rp2350-bargraph) (`20 Hz` discrete `/dev/userleds` daemon `bargraph`) without modifying any tracked files inside `nuttx/` or `apps/`.

### Prerequisites

Install the host toolchain for **Fedora 44** or **macOS** by following [Build Environment Setup (Fedora 44 & macOS)](designs/TODO.md):

- **Core tools:** `arm-none-eabi-gcc` bare-metal cross-compiler, `make`, host `gcc`/`clang`, `bison`, `flex`, `gperf`, `genromfs`, `xxd`, Kconfig frontends (`kconfig-conf`, `kconfig-tweak`, `kconfig-mconf`), `picotool`, and a USB serial terminal (`picocom`, `minicom`, or `tio`).
- **macOS notes:** `flock` is also required, and [`xiao-rp2350-bargraph/Makefile`](../xiao-rp2350-bargraph/Makefile) handles `-m` vs. `-l` host detection and `KCONFIG_LIB=` automatically (see [Build Environment Setup](designs/TODO.md) for details).

### Build, Test & Clean

```bash
# Run host unit tests (decode_adc_sample, ±24-count quantize_level hysteresis, level_to_led_mask)
make -C xiao-rp2350-bargraph test

# Configure and build nuttx/nuttx.uf2 (verified footprint: 117,320 B FLASH / 15,096 B RAM)
make -C xiao-rp2350-bargraph

# Clean objects/test binary, or reset generated Kconfig files and the apps/external symlink
make -C xiao-rp2350-bargraph clean
make -C xiao-rp2350-bargraph distclean
```

### Wire, Flash & Verify on Hardware

- **Pinout (Seeed Studio XIAO RP2350):** Potentiometer wiper on `D0` (`GPIO 26` / `/dev/adc0`, `0–3.3 V`), plus 6 active-high LEDs with `>= 330 Ω` series resistors on `D3–D5` (`GPIO 5, 6, 7`) and `D8–D10` (`GPIO 2, 4, 3`) mapped to `/dev/userleds` bits `0..5` (`0x01..0x20`).
- **Flash in `BOOTSEL` mode:** Run `picotool load -f nuttx/nuttx.uf2 -x`, or copy `nuttx/nuttx.uf2` onto the `RP2350` USB mass-storage volume.
- **Interactive USB-C NuttShell (`nsh>`):** Connect at `115200` baud (`/dev/ttyACM0` on Linux, `/dev/cu.usbmodem*` on macOS) and press Enter three times to open `nsh>`. Verify boot logs (`dmesg`), running tasks (`ps`), singleton guard (`bargraph`), graceful shutdown (`kill -15 <pid>`), and headless ADC fault blink (`bargraph /dev/adc_missing` → alternating `0x15` / `0x2a` at `250 ms`).

---

## Repository Layout & Workspace Invariants

- **`slides/`** ([`slides/slides.md`](../slides/slides.md)): Slidev source for the *"Roll Your Own Docs"* conference deck, built into `/slides/` on GitHub Pages.
- **`prototypes/`** & **`recordings/`** ([`recordings/final/`](../recordings/final)): Standalone HTML `asciinema` cast players and their underlying `asciicast v3` raw/final session recordings.
- **`docs/`**: MkDocs Material documentation source tree (tutorials, pilot designs/reports/traces, and archived Vanilla/EGM experiment snapshots), paired with [`EXPERIMENT_RUNBOOK.md`](../EXPERIMENT_RUNBOOK.md) and presentation staging files in `.scratch/egm-nuttx-casts-presentation/`.
- **`xiao-rp2350-bargraph/`** ([`Makefile`](../xiao-rp2350-bargraph/Makefile)): Live out-of-tree pilot firmware containing custom board support (`board/`), the `bargraph` NSH daemon (`external/bargraph/`), and host unit tests (`tests/test_bargraph_logic.c`).
- **`nuttx/`** ([`nuttx/README.md`](https://github.com/domesticmouse/nuttx/blob/egm/README.md)) & **`apps/`** ([`apps/README.md`](https://github.com/domesticmouse/nuttx-apps/blob/egm/README.md)): Apache NuttX RTOS kernel and companion user-space application submodules, pinned to matching `egm` branch revisions in [`.gitmodules`](../.gitmodules).
- **`pico-sdk-experiment/`** ([`TODO.md`](../pico-sdk-experiment/TODO.md)): Minimal bare-metal Raspberry Pi Pico SDK baseline experiment for the XIAO RP2350.

### Global Invariants

- `nuttx` and `apps` submodule checkouts in [`.gitmodules`](../.gitmodules) stay on matching `egm` branch revisions (`nuttx/Makefile` and `apps/Makefile` resolve `APPDIR=../apps` relative to `nuttx`).
- Tracked files inside `nuttx/` and `apps/` remain unmodified by out-of-tree projects; `xiao-rp2350-bargraph` integrates via `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"` and the gitignored `apps/external` symlink (`../xiao-rp2350-bargraph/external`).
- `xiao-rp2350-bargraph/common/` must not exist so `CONFIG_ARCH_BOARD_COMMON=y` resolves `BOARD_COMMON_DIR` to `nuttx/boards/arm/rp23xx/common`.

---

Licensed under the [Apache License, Version 2.0](../LICENSE). This is not an official Google product.
