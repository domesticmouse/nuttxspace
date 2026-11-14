# NuttX Workspace (`nuttxspace`) Documentation

Top-level Apache NuttX workspace repository coordinating the Apache NuttX RTOS kernel ([`nuttx`](https://github.com/domesticmouse/nuttx/blob/egm/README.md)), the companion user-space applications repository ([`apps`](https://github.com/domesticmouse/nuttx-apps/blob/egm/README.md)), out-of-tree target firmware ([`xiao-rp2350-bargraph`](https://github.com/domesticmouse/nuttxspace/tree/main/xiao-rp2350-bargraph)), and the design, review, tutorial, and session-trace documentation in this site.

The workspace builds an out-of-tree custom board and 20 Hz potentiometer-to-6-LED bar-graph daemon (`bargraph`) for the **Seeed Studio XIAO RP2350** without modifying any tracked files inside `nuttx/` or `apps/`.

## Documentation Map

### Tutorials
- [From Zero to `xiao-rp2350-bargraph`](tutorials/xiao-rp2350-bargraph.md) — End-to-end guide for software engineers covering breadboard wiring, out-of-tree NuttX board configuration, RP2350 ADC/LED driver traps, host unit testing, and flashing `nuttx.uf2`.
- [Replicating the NuttX EGM Experiment](tutorials/egm-replication-tutorial.md) — Replay guide and post-mortem analyzing how the Engineering Guidance Model (EGM) workflow (`47` sessions, `4,478` logged steps) navigated `nuttx/` and `apps/` to produce the firmware.

### Architecture & Designs
- [Final Design Spec (`xiao-rp2350-bargraph`)](designs/xiao-rp2350-bargraph.md) — Grounded technical specification detailing hardware pinouts, Kconfig wiring, ISR teardown safety, `O_NONBLOCK` + `poll()` sampling, ROMFS startup, and verification criteria.
- [Concepts Brief](designs/concepts_brief.md) — Plain-English architectural brief produced during `/egm-brainstorm` discovery.
- [Initial Design Draft](designs/design_draft.md) — Pre-review technical draft prior to consolidated EGM review passes.
- [Build Environment Prerequisites (`TODO`)](designs/TODO.md) — Fedora 44 host tooling, `arm-none-eabi-*` cross-compiler, and `picotool` setup checklist.

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
- **`docs`**: Documentation source tree published to GitHub Pages (`tutorials/`, `designs/`, `reports/`, `prompts/`).

### Global Invariants
- `nuttx` and `apps` submodule checkouts in [`.gitmodules`](https://github.com/domesticmouse/nuttxspace/blob/main/.gitmodules) stay on matching `egm` branch revisions (`nuttx/Makefile` and `apps/Makefile` resolve `APPDIR=../apps` relative to `nuttx`).
- Tracked files inside `nuttx/` and `apps/` remain unmodified by out-of-tree projects; `xiao-rp2350-bargraph` integrates via `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"` and the gitignored `apps/external` symlink (`../xiao-rp2350-bargraph/external`).
- `xiao-rp2350-bargraph/common/` must not exist so `CONFIG_ARCH_BOARD_COMMON=y` resolves `BOARD_COMMON_DIR` to `nuttx/boards/arm/rp23xx/common`.

---

## Quickstart & System Verification

### Host Prerequisites (Fedora 44)
See [Build Environment Prerequisites (`TODO`)](designs/TODO.md) for installation instructions:
- Host build and packaging tools: `gcc`, `make`, `bison`, `flex`, `gperf`, `gettext`, `texinfo`, `ncurses-devel`, `genromfs`, `xxd`, `cmake`, `libusb1-devel`, and `picocom`.
- Kconfig frontends: `kconfig-conf`, `kconfig-tweak`, and `kconfig-mconf`.
- ARM bare-metal cross-compiler toolchain: `arm-none-eabi-gcc-cs`, `arm-none-eabi-gcc-cs-c++`, `arm-none-eabi-binutils-cs`, and `arm-none-eabi-newlib` (`arm-none-eabi-gcc` `15.2.0-4.fc44`).
- RP2350 UF2 packaging and flashing tool: `picotool` (`v2.3.1` in `/usr/local/bin/picotool`).

### Host Unit Tests
```bash
make -C xiao-rp2350-bargraph test
```
Compiles and runs `xiao-rp2350-bargraph/tests/test_bargraph_logic`, verifying `decode_adc_sample()`, `quantize_level()` with `±24`-count hysteresis, and `level_to_led_mask()`.

### Firmware Build
```bash
make -C xiao-rp2350-bargraph
```
Configures `xiao-rp2350-bargraph/board/configs/bargraph/defconfig`, builds `nuttx/nuttx`, and converts it to `nuttx/nuttx.uf2` (verified footprint: `FLASH: 117320 B / 2 MB (5.59%)`, `RAM: 15096 B / 512 KB (2.88%)`).

### Hardware Pinout & Flashing (Seeed Studio XIAO RP2350)
- Potentiometer wiper on `D0` (`GPIO 26` / `/dev/adc0`, `0–3.3 V`) and 6 active-high LEDs with `>= 330 Ω` series resistors on `D3–D5` (`GPIO 5, 6, 7`) and `D8–D10` (`GPIO 2, 4, 3`) mapped to `/dev/userleds` bits `0..5` (`0x01..0x20`).
- Flash in `BOOTSEL` mode via `picotool load -f nuttx/nuttx.uf2 -x` or by copying `nuttx/nuttx.uf2` to the `RP2350` USB mass-storage volume.

### Interactive USB-C NuttShell (`nsh>`) Verification
- Connect to `/dev/ttyACM0` (`picocom -b 115200 /dev/ttyACM0`) and press Enter three times to open `nsh>`.
- Inspect boot/daemon logs (`dmesg`), running tasks (`ps`), singleton guard (`bargraph`), graceful shutdown (`kill -15 <pid>`), and headless ADC fault blink (`bargraph /dev/adc_missing` -> alternating `0x15` / `0x2a` at 250 ms).

### Clean Targets
- Clean compiled objects and test binary: `make -C xiao-rp2350-bargraph clean`
- Clean out-of-tree build, generated `Kconfig` files, and `apps/external` symlink state: `make -C xiao-rp2350-bargraph distclean`
