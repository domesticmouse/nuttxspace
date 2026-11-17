---
title: "Verification, Testing & NSH CLI Cheat Sheet"
page_title: "Verification & CLI Cheat Sheet — XIAO RP2350 Bar Graph"
nav_id: "ref-verify"
nav_label: "Verification & CLI"
badge: "Reference Sheet • Testing & Operations"
subtitle: "Commands for compiling and testing per-lesson companion apps, building the out-of-tree NuttX UF2 firmware, verifying submodule cleanliness, and operating the bargraph NSH CLI."
card:
  title: "**Verification, Testing & NSH CLI Cheat Sheet**"
  summary: "Host C99 unit-test commands, out-of-tree NuttX UF2 build & flash steps, git submodule cleanliness checks, and bargraph NSH subcommands."
---

## 1. Per-Lesson Companion App Verification (`tutorial/apps/`)

Every lesson in `tutorial/lessons/` has a self-contained, host-verifiable C99 companion project under `tutorial/apps/<NNNN-lesson-slug>/`:

```bash caption="Run strict C99 compiler checks and unit tests for all 6 lessons"
make -C apps/0001-breadboard-circuit-and-current-budget test
make -C apps/0002-proportional-bargraph-math-and-pwm-scaling test
make -C apps/0003-q4-ema-filtering-and-adc-extraction test
make -C apps/0004-multichannel-pwm-routing-and-safe-shutdown test
make -C apps/0005-out-of-tree-bsp-and-adc-irq-gating test
make -C apps/0006-bargraphd-daemon-and-nsh-cli test
```

## 2. Course Site Build & Listing Sync Checks (`tutorial/`)

```bash caption="Verify Complete Listing parity and build static HTML site"
.venv/bin/python ../.agents/skills/teach-code/scripts/sync_listing.py --check
.venv/bin/python build.py --check
```

## 3. Out-of-Tree NuttX Firmware Build & Submodule Verification

From the repository root (`nuttxspace-egm/`):

```bash caption="Run host unit tests, build nuttx.uf2, and verify clean submodules"
make -C xiao-rp2350-bargraph test
make -C xiao-rp2350-bargraph
git -C nuttx status --porcelain && git -C apps status --porcelain
```

## 4. Interactive NSH `bargraph` CLI Commands (`/dev/ttyACM0`)

Connect to the Seeed Studio XIAO RP2350 over USB-C (`115200 8N1`) and press `Enter` three times to open the `nsh>` prompt:

Table: `bargraph` Built-In NSH Command Reference

| Command | Target Mode | Operational Behavior |
| :--- | :--- | :--- |
| `bargraph status` | *(Unchanged)* | Prints atomic snapshot: `state`, `raw_adc`, `filtered_adc`, `voltage_mv`, `level_pct`, and `LED1..LED6` duties |
| `bargraph set <0..100>` | `MANUAL` | Locks bar graph to `<0..100>%`, ignoring live potentiometer changes |
| `bargraph test` | `TEST` → `prev_mode` | Sweeps `0% -> 100% -> 0%` across 100 ticks (`2.0 s`), blocks until finished, and restores prior mode |
| `bargraph resume` | `SENSING` | Returns daemon from `MANUAL` override back to live 50 Hz potentiometer sensing |
| `bargraph start` | `SENSING` | Starts `bargraphd` (if stopped) or transitions a running daemon back to `SENSING` mode |
| `bargraph stop` | `STOPPED` | Zeroes all 6 LED duties (`0`), issues `PWMIOC_STOP`, closes `/dev/pwm*` and `/dev/adc0`, and exits daemon |
