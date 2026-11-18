# NuttX Workspace (`nuttxspace`) Documentation

Top-level Apache NuttX workspace repository coordinating the Apache NuttX RTOS kernel ([`nuttx`](https://github.com/domesticmouse/nuttx/blob/egm/README.md)), the companion user-space applications repository ([`apps`](https://github.com/domesticmouse/nuttx-apps/blob/egm/README.md)), out-of-tree target firmware ([`xiao-rp2350-bargraph`](https://github.com/domesticmouse/nuttxspace/tree/main/xiao-rp2350-bargraph)), and the design, review, tutorial, and session-trace documentation in this site.

The workspace builds an out-of-tree custom board and 20 Hz potentiometer-to-6-LED bar-graph daemon (`bargraph`) for the **Seeed Studio XIAO RP2350** without modifying any tracked files inside `nuttx/` or `apps/`.

## Documentation Map

### Conference Presentation: "Skills Three Ways"
- [**Live 41-Slide Slidev Presentation Deck — Skills Three Ways**](../slides-skills-three-ways/slides.md) ([Slide Overview](slides-skills-three-ways/index.html#/overview) · [Presenter Console](slides-skills-three-ways/index.html#/presenter)) — Standalone 40–45 minute dark-mode deck covering the 5-act **Adopt → Orchestrate → Author** arc (`egm` plugin, `mattpocock/skills` + `/wayfinder`, and `/agy-customizations` + `domesticmouse/skills` + `/teach-code`), reusing all 7 terminal casts, clickable `/prototype` HTML planners, and the live 6-lesson course.

### Conference Presentation: "Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"
- [**Live 33-Slide Slidev Presentation Deck**](../slides/slides.md) ([Slide Overview](slides/index.html#/overview) · [Presenter Console](slides/index.html#/presenter)) — Interactive dark-mode conference deck featuring 7 embedded `asciinema` recordings (`53` stop points), progressive `v-click` reveals, breadboard SVG comparisons, and the live `/teach-code` course showcase.
- [Authoritative Presentation Specification](../.scratch/egm-nuttx-casts-presentation/spec.md) and [Verbatim Speaker Script (`bjm-voice`)](../.scratch/egm-nuttx-casts-presentation/speaker-script-bjm-voice.md) — Complete 30-minute (`29m 51s`) slide-by-slide stage specification and Australian English narrative.
- [Presentation Wayfinder Map](../.scratch/egm-nuttx-casts-presentation/map.md) and Design Tickets: [01 Cast Catalog](../.scratch/egm-nuttx-casts-presentation/issues/01-catalog-cast-recording-stop-points.md) · [02 Hardware & NuttX Staging](../.scratch/egm-nuttx-casts-presentation/issues/02-hardware-and-nuttx-challenge-staging.md) · [03 Cast Curation](../.scratch/egm-nuttx-casts-presentation/issues/03-curate-cast-recording-segments.md) · [04 Staging EGM Mechanics](../.scratch/egm-nuttx-casts-presentation/issues/04-explain-egm-and-antigravity-on-stage.md) · [05 Slide Structure & Pacing](../.scratch/egm-nuttx-casts-presentation/issues/05-slide-structure-and-pacing-outline.md) · [06 Speaker Script](../.scratch/egm-nuttx-casts-presentation/issues/06-speaker-script-and-narrative-bjm-voice.md) · [07 Spec Assembly & Audit](../.scratch/egm-nuttx-casts-presentation/issues/07-assemble-and-verify-presentation-spec.md).
- Interactive Planning Prototypes:
  - [Slide Structure & Timing Planner (`MD`)](../.scratch/egm-nuttx-casts-presentation/slide-structure-prototype.md) · [Interactive Timing Planner (`HTML`)](../.scratch/egm-nuttx-casts-presentation/slide-structure-prototype.html)
  - [Cast Curation & Trim Schedule (`MD`)](../.scratch/egm-nuttx-casts-presentation/cast-curation-prototype.md) · [Interactive Cast Curation Matrix (`HTML`)](../.scratch/egm-nuttx-casts-presentation/cast-curation-prototype.html)

### Comparative Experiment: Vanilla Vibe Coding vs. Full EGM Flow
- [Comparative Experiment Runbook](../EXPERIMENT_RUNBOOK.md) — End-to-end operator checklist, trace correlations, and defect scorecard comparing Vanilla Vibe Coding (`nuttxspace-vanilla`) against the full Engineering Guidance Model workflow (`nuttxspace-egm`).
- [Vanilla vs. EGM Comparison Report](reports/vanilla-vs-egm-comparison.md) — Independent read-only Goldfish audit grading both captured implementations across all 10 hardware/RTOS invariants and reconciling recording/trace telemetry.
- **Standalone `asciinema` HTML Cast Players**:
  - Run 1 (Vanilla): [`vanilla-vibe-cast-player.html`](../prototypes/vanilla-vibe-cast-player.html)
  - Run 2 (EGM Phases 0–6): [`01-bootstrap`](../prototypes/01-bootstrap-cast-player.html) · [`02-brainstorm`](../prototypes/02-brainstorm-cast-player.html) · [`03-draft`](../prototypes/03-draft-cast-player.html) · [`04-review`](../prototypes/04-review-cast-player.html) · [`05-implement`](../prototypes/05-implement-cast-player.html) · [`06-teach-code`](../prototypes/06-teach-code-cast-player.html)
- **Experiment A (`nuttxspace-vanilla`)**:
  - [Vanilla Technical Code Review](vanilla/CODE_REVIEW.md) and [Breadboard Wiring Guide](vanilla/BREADBOARD_WIRING.md)
  - [Captured Vanilla Source Snapshot](vanilla/xiao-rp2350-bargraph/README.md) and [Vanilla Session Trace (`01`)](prompts-vanilla/README.md)
- **Experiment B (`nuttxspace-egm`)**:
  - [Phases 0–6 Documentation & Implementation Review](egm/DOCS_REVIEW.md), [Workspace Blueprint](egm/README.md), and [Breadboard Wiring Guide](egm/BREADBOARD_WIRING.md)
  - Per-Phase Reviews: [01 Bootstrap](egm/reviews/01-bootstrap-review.md) · [02 Brainstorm](egm/reviews/02-brainstorm-review.md) · [03 Draft](egm/reviews/03-draft-review.md) · [04 Review](egm/reviews/04-review-review.md) · [05 Implement](egm/reviews/05-implement-review.md) · [06 `/teach-code`](egm/reviews/06-teach-code-review.md)
  - Design Evolution: [Concepts Brief](egm/designs/concepts_brief.md) -> [Initial Draft](egm/designs/design_draft.md) -> [v0 Spec](egm/designs/xiao-rp2350-bargraph.v0-draft.md) -> [Hardened Design Spec](egm/designs/xiao-rp2350-bargraph.md)
  - Drift & Goldfish Audits: [Drift Report (2026-11-16)](egm/reports/2026-11-16_DRIFT_REPORT.md) · [Pass 1 Goldfish Report](egm/reports/pass1/goldfish_review_report.md) · [Pass 2 Goldfish Report](egm/reports/pass2/goldfish_review_report.md)
  - [Captured EGM Firmware & 9 Contracts](egm/xiao-rp2350-bargraph/README.md) and [EGM Session Traces (`01`–`25`)](prompts-egm/README.md)
  - 6-Lesson `/teach-code` Interactive Course: [Compiled Interactive Tufte HTML](egm/tutorial/index.html) and [Course Overview (`README`)](egm/tutorial/README.md)

### Tutorials
- [From Zero to `xiao-rp2350-bargraph`](tutorials/xiao-rp2350-bargraph.md) — End-to-end guide for software engineers covering breadboard wiring, out-of-tree NuttX board configuration, RP2350 ADC/LED driver traps, host unit testing, and flashing `nuttx.uf2`.
- [Replicating the NuttX EGM Experiment](tutorials/egm-replication-tutorial.md) — Replay guide and post-mortem analyzing how the Engineering Guidance Model (EGM) workflow (`47` sessions, `4,478` logged steps) navigated `nuttx/` and `apps/` to produce the firmware.

### Architecture & Designs
- [Final Design Spec (`xiao-rp2350-bargraph`)](designs/xiao-rp2350-bargraph.md) — Grounded technical specification detailing hardware pinouts, Kconfig wiring, ISR teardown safety, `O_NONBLOCK` + `poll()` sampling, ROMFS startup, and verification criteria.
- [Concepts Brief](designs/concepts_brief.md) — Plain-English architectural brief produced during `/egm-brainstorm` discovery.
- [Initial Design Draft](designs/design_draft.md) — Pre-review technical draft prior to consolidated EGM review passes.
- [Build Environment Prerequisites (`TODO`)](designs/TODO.md) — Fedora 44 and macOS host tooling, `arm-none-eabi-*` cross-compiler, and `picotool` setup checklist.

### EGM Audit & Review Reports
- [Drift Report (2026-11-13)](reports/2026-11-13_DRIFT_REPORT.md) — `/egm-drift-report` consistency check against upstream `nuttx` and `apps` `README.md` contracts.
- [Pass 2 Consolidated Review](reports/consolidated_review_report.md) — Deduplicated findings (`C-01`–`C-22`) from the Pass 2 Goldfish review swarm.
- [Pass 3 Goldfish Review](reports/goldfish_review_report.md) — Final consolidated review (`CON-1`–`CON-19`) catching the ADC FIFO ISR `close()` race and blocking `read()` hang before implementation.
- Individual Persona Reports:
  - [Comprehension Report](reports/comprehension_report.md)
  - [Design Critic Report](reports/critic_report.md)
  - [Implementation Readiness Report](reports/implementation_readiness_report.md)
  - [Zoom Out Report](reports/zoom_out_report.md)
  - [Intermediate Zoom Out Report](reports/intermediate_zoom_out_report.md)

### Session Traces (`47` Exported Conversations)
- [Project Conversation Logs Index](prompts/README.md) — Complete index and per-session Markdown transcripts (`01` through `47`) exported from Antigravity brain traces.

---

## Workspace Structure & Boundaries

- **`apps`** ([`apps/README.md`](https://github.com/domesticmouse/nuttx-apps/blob/egm/README.md)): NuttX user-space application repository containing NuttShell (`nshlib`), built-in system utilities (`system`), networking/graphics/filesystem services, and example/testing applications.
- **`nuttx`** ([`nuttx/README.md`](https://github.com/domesticmouse/nuttx/blob/egm/README.md)): Apache NuttX RTOS kernel, architecture HALs (`arch`), board support packages (`boards`), drivers (`drivers`), filesystems (`fs`), C/C++/math libraries (`libs`), memory manager (`mm`), and networking stack (`net`).
- **`xiao-rp2350-bargraph`** ([`xiao-rp2350-bargraph/Makefile`](https://github.com/domesticmouse/nuttxspace/blob/main/xiao-rp2350-bargraph/Makefile)): Out-of-tree XIAO RP2350 potentiometer-to-6-LED bar-graph project containing custom board support (`board/`), the `bargraph` NSH daemon (`external/bargraph/`), and host unit tests (`tests/test_bargraph_logic.c`).
- **`docs`**: Documentation source tree published to GitHub Pages (`tutorials/`, `designs/`, `reports/`, `prompts/`, `vanilla/`, `prompts-vanilla/`, `egm/`, `prompts-egm/`), paired with [`EXPERIMENT_RUNBOOK.md`](../EXPERIMENT_RUNBOOK.md) and standalone HTML players under `prototypes/`.

### Global Invariants
- `nuttx` and `apps` submodule checkouts in [`.gitmodules`](https://github.com/domesticmouse/nuttxspace/blob/main/.gitmodules) stay on matching `egm` branch revisions (`nuttx/Makefile` and `apps/Makefile` resolve `APPDIR=../apps` relative to `nuttx`).
- Tracked files inside `nuttx/` and `apps/` remain unmodified by out-of-tree projects; `xiao-rp2350-bargraph` integrates via `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"` and the gitignored `apps/external` symlink (`../xiao-rp2350-bargraph/external`).
- `xiao-rp2350-bargraph/common/` must not exist so `CONFIG_ARCH_BOARD_COMMON=y` resolves `BOARD_COMMON_DIR` to `nuttx/boards/arm/rp23xx/common`.

---

## Quickstart & System Verification

### Host Prerequisites (Fedora 44 & macOS)
See [Build Environment Prerequisites (`TODO`)](designs/TODO.md) for installation instructions and macOS compatibility notes:
- **Fedora 44**:
  - Host build and packaging tools: `gcc`, `make`, `bison`, `flex`, `gperf`, `gettext`, `texinfo`, `ncurses-devel`, `genromfs`, `xxd`, `cmake`, `libusb1-devel`, and `picocom`.
  - Kconfig frontends: `kconfig-conf`, `kconfig-tweak`, and `kconfig-mconf`.
  - ARM bare-metal cross-compiler toolchain: `arm-none-eabi-gcc-cs`, `arm-none-eabi-gcc-cs-c++`, `arm-none-eabi-binutils-cs`, and `arm-none-eabi-newlib` (`arm-none-eabi-gcc` `15.2.0-4.fc44`).
  - RP2350 UF2 packaging and flashing tool: `picotool` (`v2.3.1` in `/usr/local/bin/picotool`).
- **macOS (`Darwin`)**:
  - Host build, packaging, and serial tools: Xcode Command Line Tools (`clang`/`gcc`, `make`, `xxd`), `genromfs` (`px4/px4` tap), `flock` (`discoteq/discoteq` tap), `picotool`, and a USB serial terminal (`picocom`, `minicom`, or `tio`).
  - ARM bare-metal cross-compiler toolchain: `arm-none-eabi-gcc` (e.g., Homebrew `--cask gcc-arm-embedded`).
  - Kconfig frontends: `kconfig-conf`, `kconfig-tweak`, and `kconfig-mconf` on `PATH` (invoked with `KCONFIG_LIB=` for `/bin/sh` compatibility).

### Host Unit Tests
```bash
make -C xiao-rp2350-bargraph test
```
Compiles and runs `xiao-rp2350-bargraph/tests/test_bargraph_logic`, verifying `decode_adc_sample()`, `quantize_level()` with `±24`-count hysteresis, and `level_to_led_mask()`.

### Firmware Build
```bash
make -C xiao-rp2350-bargraph
```
Auto-detects macOS (`-m`) vs Linux (`-l`) via `uname -s`, passes `KCONFIG_LIB=` so `nuttx/tools/Unix.mk` uses `kconfig-conf` instead of `kconfiglib`'s bash process substitution (which fails under macOS's default `/bin/sh`), configures `xiao-rp2350-bargraph/board/configs/bargraph/defconfig`, builds `nuttx/nuttx`, and converts it to `nuttx/nuttx.uf2` (verified footprint: `FLASH: 117320 B / 2 MB (5.59%)`, `RAM: 15096 B / 512 KB (2.88%)`).

### Hardware Pinout & Flashing (Seeed Studio XIAO RP2350)
- Potentiometer wiper on `D0` (`GPIO 26` / `/dev/adc0`, `0–3.3 V`) and 6 active-high LEDs with `>= 330 Ω` series resistors on `D3–D5` (`GPIO 5, 6, 7`) and `D8–D10` (`GPIO 2, 4, 3`) mapped to `/dev/userleds` bits `0..5` (`0x01..0x20`).
- Flash in `BOOTSEL` mode via `picotool load -f nuttx/nuttx.uf2 -x` or by copying `nuttx/nuttx.uf2` to the `RP2350` USB mass-storage volume.

### Interactive USB-C NuttShell (`nsh>`) Verification
- Connect to `/dev/ttyACM0` on Linux (`picocom -b 115200 /dev/ttyACM0`) or `/dev/cu.usbmodem*` on macOS (`picocom -b 115200 /dev/cu.usbmodem*`) and press Enter three times to open `nsh>`.
- Inspect boot/daemon logs (`dmesg`), running tasks (`ps`), singleton guard (`bargraph`), graceful shutdown (`kill -15 <pid>`), and headless ADC fault blink (`bargraph /dev/adc_missing` -> alternating `0x15` / `0x2a` at 250 ms).

### Clean Targets
- Clean compiled objects and test binary: `make -C xiao-rp2350-bargraph clean` (also removes out-of-tree `external/bargraph/*.o` and `external/bargraph/.built` even when `nuttx/.config` is not yet present).
- Clean out-of-tree build, generated `Kconfig` files, and `apps/external` symlink state: `make -C xiao-rp2350-bargraph distclean` (also removes `external/.kconfig`, `external/bargraph/*.o`, `external/bargraph/.built`, `external/bargraph/.depend`, and `external/bargraph/Make.dep` even if `nuttx/.config` is not present).
