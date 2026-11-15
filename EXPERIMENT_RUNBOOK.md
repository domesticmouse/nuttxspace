# Comparative Experiment Runbook: Vanilla Vibe Coding vs. Full EGM Flow

Working operator checklist for provisioning, recording, post-processing, trace-exporting, and auditing two head-to-head `asciinema` experiments replicating the XIAO RP2350 potentiometer LED bar-graph build from [`docs/tutorials/egm-replication-tutorial.md`](docs/tutorials/egm-replication-tutorial.md):

- **Experiment A (`nuttxspace-vanilla`)**: Upstream `nuttx` (`master`) and `apps` (`master`) checkouts with no EGM architectural `README.md` contracts, using a straight conversational vibe-coding loop.
- **Experiment B (`nuttxspace-egm`)**: `nuttx` (`egm`) and `apps` (`egm`) checkouts (`3,242` `README.md` contracts), using the full Engineering Guidance Model (EGM) skill flow (`/egm-bootstrap`, `/egm-drift-report`, `/egm-brainstorm`, `/egm-draft`, two passes of Consolidated `/egm-review` + `/egm-resolve`, and implementation).

---

## 1. Overview & Experimental Design Matrix

| Dimension | Experiment A: Vanilla Vibe Coding | Experiment B: Full EGM Flow |
| :--- | :--- | :--- |
| **Isolated Workspace** | `~/Documents/GitHub/nuttxspace-vanilla` | `~/Documents/GitHub/nuttxspace-egm` |
| **Why Isolated?** | Must **not** contain `xiao-rp2350-bargraph/` or `docs/` so the agent cannot peek at finished code or prior designs. | Must **not** contain `xiao-rp2350-bargraph/` or `docs/` so every discovery and review finding happens fresh. |
| **Submodule Branches** | `nuttx` on `master` (`7` READMEs)<br>`apps` on `master` (`7` READMEs) | `nuttx` on `egm` (`2,608` `README.md` contracts)<br>`apps` on `egm` (`634` `README.md` contracts) |
| **Starting Baseline** | Functional requirements only (no prescribed GPIO pins, driver quirks, or Kconfig fixes). | Same functional requirements + EGM phase constraints (`One concern at a time`, read nearest `README.md`). |
| **Recording Layout** | Single continuous recording:<br>`recordings/raw/vanilla-vibe.cast` | Five per-phase recordings:<br>`01-bootstrap.cast` … `05-implement.cast` stitched into `recordings/final/egm-flow.cast` |
| **Terminal Geometry** | `120x36` (`-i 2.0`) | `120x36` (`-i 2.0`) |
| **Stop Criterion** | `make -C xiao-rp2350-bargraph test` and `make -C xiao-rp2350-bargraph` produce `nuttx/nuttx.uf2` (or hit a terminal blocker). | Same build/test target after two passes of Consolidated `/egm-review` + `/egm-resolve`. |
| **Off-Screen Capture** | Export via `/agy-brain-explorer` in `nuttxspace` (`nuttxspace-vanilla` -> `docs/prompts-vanilla/`) | Export via `/agy-brain-explorer` in `nuttxspace` (`nuttxspace-egm` -> `docs/prompts-egm/`, incl. Goldfish subagents) |
| **Post-Run Audit** | AGY comparative audit in `nuttxspace` scoring `nuttxspace-vanilla` against the 10-point hardware/RTOS scorecard. | Compare defect count, trace steps, `README.md` usage, and build retries against Vanilla. |

---

## 2. Phase 0: Pre-Flight Toolchain & Environment Checklist

Run these checks **before starting any recording** so missing host packages never derail an experiment mid-cast.

### Common Tools (macOS & Fedora 44)

```bash
# 1. Recording, CLI agent, and Python trace tooling
asciinema --version          # Verified with asciinema 3.2.1 (records asciicast v3)
agy --version                # Antigravity CLI
uv --version                 # Required by agy-brain-explorer.py
python3 --version

# 2. Cross-compiler, build system, ROMFS, and RP2350 UF2 packager
arm-none-eabi-gcc --version
make --version
gcc --version
bison --version
flex --version
gperf --version
genromfs -h | head -n 5
which xxd
picotool version

# 3. Kconfig frontends (needed by NuttX tools/configure.sh & olddefconfig)
which kconfig-conf kconfig-tweak kconfig-mconf
```

### Host-Specific Notes

- **macOS (`Darwin`)**:
  - Install packages if missing:
    - Cross-compiler: `brew install --cask gcc-arm-embedded`
    - ROMFS & UF2: `brew install genromfs picotool` (`/usr/bin/xxd` is built into macOS)
    - Kconfig frontends: verify `kconfig-conf`, `kconfig-tweak`, and `kconfig-mconf` are on `PATH` (for example in `/usr/local/bin` or Homebrew).
  - **NuttX macOS build gotcha**: On macOS, `nuttx/tools/configure.sh` expects `-m` (not `-l`), and `nuttx/tools/Unix.mk` defaults to `kconfiglib` unless `KCONFIG_LIB=` is passed—which can fail under macOS `/bin/sh` (`2> >(...)` process substitution). Watch whether Vanilla discovers `-m` and `KCONFIG_LIB=` cleanly or burns turns debugging `configure.sh`.
- **Fedora 44 (`Linux`)**:
  - Follow [`docs/designs/TODO.md`](docs/designs/TODO.md): `gcc`, `cpp`, `make`, `bison`, `flex`, `gperf`, `gettext`, `texinfo`, `ncurses-devel`, `/usr/local/bin/kconfig-conf`, `kconfig-tweak`, `kconfig-mconf`, `genromfs`, `xxd`, `picocom`, `arm-none-eabi-gcc-cs`, `arm-none-eabi-gcc-cs-c++`, `arm-none-eabi-binutils-cs`, `arm-none-eabi-newlib`, `cmake`, `libusb1-devel`, and `picotool`.

### Checklist

- [ ] `asciinema`, `agy`, and `uv` all execute cleanly from the target terminal shell (`PATH` includes `~/.local/bin`).
- [ ] `arm-none-eabi-gcc`, `genromfs`, `xxd`, `picotool`, and `kconfig-conf` are on `PATH`.
- [ ] You have confirmed the baseline terminal window works at `120x36` (`--window-size 120x36`).

---

## 3. Workspace Provisioning (Fast Local Submodule Cloning)

Store raw and final `.cast` recordings plus HTML players inside this repository (`nuttxspace`), while keeping the two experiment workspaces in clean sibling directories. Using `--reference` reuses the already-fetched Git objects inside `~/Documents/GitHub/nuttxspace/{nuttx,apps}` so submodule creation finishes in seconds.

### 3.1 Create Recording Directories in `nuttxspace`

```bash
mkdir -p ~/Documents/GitHub/nuttxspace/recordings/raw \
         ~/Documents/GitHub/nuttxspace/recordings/final \
         ~/Documents/GitHub/nuttxspace/prototypes
```

### 3.2 Provision Workspace A (`~/Documents/GitHub/nuttxspace-vanilla`, `master`)

```bash
mkdir -p ~/Documents/GitHub/nuttxspace-vanilla
cd ~/Documents/GitHub/nuttxspace-vanilla

git init -b main
cat <<'EOF' > .gitignore
site/
__pycache__/
EOF

git submodule add --reference ~/Documents/GitHub/nuttxspace/nuttx -b master \
  https://github.com/domesticmouse/nuttx nuttx
git submodule add --reference ~/Documents/GitHub/nuttxspace/apps -b master \
  https://github.com/domesticmouse/nuttx-apps apps

# Verify exact branch state and absence of EGM README contracts (expected: 7 + 7 = 14)
git -C nuttx checkout master
git -C apps checkout master
git -C nuttx rev-parse --short HEAD
git -C apps rev-parse --short HEAD
find nuttx apps -name "README.md" | wc -l

git add .gitignore .gitmodules nuttx apps
git commit -m "Provision vanilla NuttX workspace on upstream master"
git status
```

### 3.3 Provision Workspace B (`~/Documents/GitHub/nuttxspace-egm`, `egm`)

```bash
mkdir -p ~/Documents/GitHub/nuttxspace-egm
cd ~/Documents/GitHub/nuttxspace-egm

git init -b main
cat <<'EOF' > .gitignore
site/
__pycache__/
EOF

git submodule add --reference ~/Documents/GitHub/nuttxspace/nuttx -b egm \
  https://github.com/domesticmouse/nuttx nuttx
git submodule add --reference ~/Documents/GitHub/nuttxspace/apps -b egm \
  https://github.com/domesticmouse/nuttx-apps apps

# Verify exact branch state and presence of EGM README contracts (expected: 2608 + 634 = 3242)
git -C nuttx checkout egm
git -C apps checkout egm
git -C nuttx rev-parse --short HEAD
git -C apps rev-parse --short HEAD
find nuttx apps -name "README.md" | wc -l

git add .gitignore .gitmodules nuttx apps
git commit -m "Provision EGM NuttX workspace on egm branch"
git status
```

### Provisioning Verification Checklist

- [ ] `~/Documents/GitHub/nuttxspace-vanilla` has clean `git status`, `nuttx` at `master` (`562db43781`), `apps` at `master` (`ab105e659c`), and `14` `README.md` files total.
- [ ] `~/Documents/GitHub/nuttxspace-egm` has clean `git status`, `nuttx` at `egm` (`b047ea1315`), `apps` at `egm` (`f711fe028`), and `3,242` `README.md` files total.
- [ ] Neither `nuttxspace-vanilla` nor `nuttxspace-egm` contains `xiao-rp2350-bargraph/` or `docs/` yet.
- [ ] Note down any existing `nuttxspace-vanilla` or `nuttxspace-egm` sessions in `~/.gemini` (none should exist before recording).

---

## 4. Experiment A Recording Runbook: Vanilla Vibe Coding (`nuttxspace-vanilla`)

### 4.1 Start the Recording

Do **not** pass `-I` / `--stdin` to `asciinema rec` (keeps raw keystrokes out of the cast).

```bash
cd ~/Documents/GitHub/nuttxspace-vanilla
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/vanilla-vibe.cast
```

Inside the recorded shell, verify the clean starting state and launch `agy`:

```bash
git status -sb
agy
```

### 4.2 Opening Prompt (Paste Verbatim)

Give the Vanilla agent the exact same high-level functional goal without prescribing breadboard pin numbers, ADC shift math, or ISR guards:

```text
I want to build an out-of-tree Apache NuttX application and custom board configuration in a new `xiao-rp2350-bargraph/` directory of this workspace for a Seeed Studio XIAO RP2350 mounted on a breadboard with 1 potentiometer and 6 discrete LEDs acting as a bar graph.

Requirements:
- Keep `nuttx/` and `apps/` submodules completely unmodified (integrate out-of-tree).
- Read the potentiometer via ADC and drive the 6 breadboard LEDs as a bar graph.
- Provide an interactive NuttShell (`nsh>`) console over the USB-C port, while auto-starting the bar-graph application automatically at boot even before USB serial attaches.
- Include a Makefile in `xiao-rp2350-bargraph/` that builds host unit tests (`make -C xiao-rp2350-bargraph test`) and compiles the firmware into `nuttx/nuttx.uf2` (`make -C xiao-rp2350-bargraph`).
```

### 4.3 Operator Rules During Experiment A

- **Do not coach embedded fixes**: Answer product-level clarifying questions if asked (e.g., *"6 LEDs is fine"*, *"any free breadboard pins you pick are fine"*, *"start automatically in the background so USB-C NSH still works"*), without hinting at `GPIO 26–29` `my_reset()`, `IE` leakage, `>> 19` shifting, `open/read/close` vs. 500 kHz IRQs, or `enter_critical_section()`.
- **Watch for `apps/external` symlink & clean submodules**: After the agent reports completion, run these verification commands **on camera** before exiting the recording:

```bash
make -C xiao-rp2350-bargraph test
make -C xiao-rp2350-bargraph
ls -lh nuttx/nuttx nuttx/nuttx.uf2
git -C nuttx status -s
git -C apps status -s
```

- Exit `agy` (`/exit` or `Ctrl+D`), commit the Vanilla result if desired (outside or at end of recording), and exit the recorded shell (`exit` or `Ctrl+D`) so `asciinema` saves `vanilla-vibe.cast`.

### Experiment A Checklist

- [ ] `~/Documents/GitHub/nuttxspace/recordings/raw/vanilla-vibe.cast` exists and is non-empty.
- [ ] Build/test outcome (`PASS` or exact blocker) and `git status` of `nuttx/` + `apps/` were captured on camera.

---

## 5. Experiment B Recording Runbook: Full EGM Flow (`nuttxspace-egm`)

Record **five separate `.cast` files** (one per EGM phase), committing clean checkpoints between phases so any quota interruption or retake only affects that single phase.

> [!TIP]
> Before each phase recording, check `git status` is clean, launch `asciinema rec -i 2.0 --window-size 120x36 --overwrite ...`, start `agy`, and end cleanly with `/exit` + shell `exit`.

---

### 5.1 Recording 1 (`01-bootstrap.cast`) — Phase 1: Workspace Bootstrap & Drift Audit

```bash
cd ~/Documents/GitHub/nuttxspace-egm
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/01-bootstrap.cast
```

Inside the recorded shell, run `agy` and send these two prompts sequentially:

1. **Prompt 1 (Bootstrap Root Blueprint):**
   ```text
   /egm-bootstrap create the root README.md contract for this workspace and verify submodule boundaries
   ```
   *(If `/egm-bootstrap` hits a scope confirmation or asks about updating submodule docs, confirm updating **only** the workspace root `README.md`—the `3,242` `nuttx/` and `apps/` contracts are already on `egm`.)*

2. **Prompt 2 (Verify Against Hallucinations):**
   ```text
   /egm-drift-report check the README.md files modified in this session
   ```

3. **On-camera wrap-up & commit** (either via `agy` or back in the shell):
   ```bash
   git add README.md
   # Add any drift report file if created (e.g., docs/reports/*DRIFT_REPORT.md)
   git status -s
   git commit -m "Phase 1: bootstrap root README.md and run drift check"
   ```

Exit `agy` and exit the shell to finish `01-bootstrap.cast`.

---

### 5.2 Recording 2 (`02-brainstorm.cast`) — Phase 2: Plain-English Discovery (`/egm-brainstorm`)

```bash
cd ~/Documents/GitHub/nuttxspace-egm
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/02-brainstorm.cast
```

Launch `agy` and paste the **Opening Prompt** (note the fast-path guardrails from [`docs/tutorials/egm-replication-tutorial.md`](docs/tutorials/egm-replication-tutorial.md)):

```text
/egm-brainstorm I want to build an out-of-tree Apache NuttX application and custom board configuration in a new `xiao-rp2350-bargraph/` directory of this workspace for a Seeed Studio XIAO RP2350 mounted on a breadboard with 1 potentiometer and 6 discrete LEDs acting as a bar graph, with a USB-C NSH console and automatic startup at boot, keeping `nuttx/` and `apps/` unmodified.

Constraints for this session:
- Ask me one concern at a time.
- Instruct any research subagents to read nearest EGM README.md contracts before reading .c/.h files.
```

**Interactive Operator Guide for `/egm-brainstorm`:**
- Answer clarifying questions one concern at a time (e.g., breadboard learning exercise, single board for now, 6 discrete LEDs, USB-C console, auto-start at boot, project directory `xiao-rp2350-bargraph/`).
- At the Readiness Checkpoint Gate, select **`Review with Goldfishes`**.
- After the 7 Goldfish lenses write `concepts_brief.md`, walk through the surfaced hardware concerns one at a time (pin choices avoiding `GPIO 26–29` `my_reset()` and `UART0`, `open`/`read`/`close` per 50 ms tick vs. 500 kHz IRQ, `rcS` + `task_create()` singleton + RAMLOG `dmesg` + `0x15`/`0x2A` error blink).
- At Step 4 Handoff Gate, select **`Proceed to egm-draft (Initialize formal design document)`** (or keep `concepts_brief.md` saved for the next session).

Exit `agy`, commit `concepts_brief.md`, and exit the shell:

```bash
git add concepts_brief.md
git commit -m "Phase 2: finalize brainstorm concepts_brief.md"
```

---

### 5.3 Recording 3 (`03-draft.cast`) — Phase 3: Grounded Technical Spec (`/egm-draft`)

```bash
cd ~/Documents/GitHub/nuttxspace-egm
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/03-draft.cast
```

Launch `agy` and invoke `/egm-draft`:

```text
/egm-draft @[concepts_brief.md] target @[docs/designs/xiao-rp2350-bargraph.md]
```

**Interactive Operator Guide for `/egm-draft`:**
- Confirm ingesting `concepts_brief.md`.
- Review and approve each of the four sections via the Section Approval Gate (`Overview & Goals`, `Technical Plan`, `Alternatives Considered`, `Detailed Implementation`).
- Confirm saving the finalized design doc to `docs/designs/xiao-rp2350-bargraph.md`.

Exit `agy`, commit the drafted design spec, and exit the shell:

```bash
git add docs/designs/xiao-rp2350-bargraph.md
git commit -m "Phase 3: draft technical design spec docs/designs/xiao-rp2350-bargraph.md"
```

---

### 5.4 Recording 4 (`04-review.cast`) — Phase 4: Two-Pass Consolidated Review & Resolution

Run **at least two** Consolidated `/egm-review` + `/egm-resolve` passes (resetting context with `/clear` between Pass 1 and Pass 2 so Pass 2 operates like a fresh Elephant on the updated spec).

```bash
cd ~/Documents/GitHub/nuttxspace-egm
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/04-review.cast
```

Launch `agy` and execute **Pass 1**:

1. **Prompt (Pass 1 Review):**
   ```text
   /egm-review @[docs/designs/xiao-rp2350-bargraph.md]

   Instruct all Goldfish reviewer subagents to read the nearest local EGM README.md contracts in nuttx/ and apps/ alongside the referenced .c, .h, .mk, and Kconfig files.
   ```
   - At the preset gate, choose: **`Consolidated report with all review rubrics (runs full flow without stopping)`**.
   - Wait for Stages 5–9 to finish writing `consolidated_review_report.md` (and individual stage reports).

2. **Prompt (Pass 1 Resolution):**
   ```text
   /egm-resolve @[consolidated_review_report.md] target @[docs/designs/xiao-rp2350-bargraph.md]
   ```
   - Inspect `goldfish_review_report.md`, then reply **`auto accept`** (or **`interactive breakdown`** if you want to highlight critical findings such as Kconfig/ISR bugs on camera).

3. **Reset Context & Run Pass 2:**
   ```text
   /clear
   ```
   ```text
   /egm-review @[docs/designs/xiao-rp2350-bargraph.md]

   Instruct all Goldfish reviewer subagents to read the nearest local EGM README.md contracts in nuttx/ and apps/ alongside the referenced .c, .h, .mk, and Kconfig files.
   ```
   - Again select **`Consolidated report with all review rubrics (runs full flow without stopping)`**.
   - Run `/egm-resolve @[consolidated_review_report.md] target @[docs/designs/xiao-rp2350-bargraph.md]` and reply **`auto accept`** (or resolve any contested findings).
   - *(Optional Pass 3: if Pass 2 still surfaces Blockers/Majors like the `close()` vs. ISR teardown race or blocking `adc_read()`, run one more `/clear` -> `/egm-review` -> `/egm-resolve` cycle.)*

Exit `agy`, commit the reviewed spec and reports, and exit the shell:

```bash
mkdir -p docs/reports
mv -f *_report.md docs/reports/ 2>/dev/null || true
git add docs/designs/xiao-rp2350-bargraph.md docs/reports
git commit -m "Phase 4: complete consolidated EGM reviews and resolutions"
```

---

### 5.5 Recording 5 (`05-implement.cast`) — Phase 5: Firmware Implementation & Verification

```bash
cd ~/Documents/GitHub/nuttxspace-egm
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/05-implement.cast
```

Launch `agy` and trigger implementation from the reviewed spec:

```text
Implement @[docs/designs/xiao-rp2350-bargraph.md] in `xiao-rp2350-bargraph/`, keeping `nuttx/` and `apps/` submodules completely unmodified. After creating the files, run `make -C xiao-rp2350-bargraph test` and `make -C xiao-rp2350-bargraph` and verify `nuttx/nuttx.uf2` builds cleanly and `git -C nuttx status -s` / `git -C apps status -s` show zero tracked modifications.
```

Once `make -C xiao-rp2350-bargraph test` and `make -C xiao-rp2350-bargraph` pass on camera, exit `agy`, commit the generated firmware, and exit the shell:

```bash
git add xiao-rp2350-bargraph
git commit -m "Phase 5: implement xiao-rp2350-bargraph firmware and host tests"
```

### Experiment B Checklist

- [ ] All 5 raw `.cast` files (`01-bootstrap.cast` through `05-implement.cast`) exist in `~/Documents/GitHub/nuttxspace/recordings/raw/`.
- [ ] `make -C xiao-rp2350-bargraph test` and `make -C xiao-rp2350-bargraph` both succeeded in `nuttxspace-egm`, producing `nuttx/nuttx.uf2`.
- [ ] `git -C nuttx status -s` and `git -C apps status -s` show zero modified tracked files.

---

## 6. Post-Recording Step 1: Match & Export AGY Traces (`/agy-brain-explorer`)

Once both recordings are saved in `recordings/raw/`, switch to **`~/Documents/GitHub/nuttxspace`** and start `agy` (note: this workspace already includes the `/agy-brain-explorer`, `/asciinema`, and `/cast-player-html` skills).

### 6.1 Launch `agy` in `nuttxspace`

```bash
cd ~/Documents/GitHub/nuttxspace
agy
```

### 6.2 Prompt: Correlate `.cast` Timestamps, Clean Dead Takes & Export Traces

Paste this prompt into `agy` (approve `BypassSandbox` when prompted so `/agy-brain-explorer` can read `~/.gemini/**/brain/`):

```text
/agy-brain-explorer I just finished capturing two comparative asciinema experiments in sibling workspaces and need to match the recordings in `recordings/raw/` to their full Antigravity parent + subagent traces:

- Vanilla recording: `recordings/raw/vanilla-vibe.cast` (recorded in `~/Documents/GitHub/nuttxspace-vanilla`)
- EGM phase recordings: `recordings/raw/01-bootstrap.cast`, `02-brainstorm.cast`, `03-draft.cast`, `04-review.cast`, and `05-implement.cast` (recorded in `~/Documents/GitHub/nuttxspace-egm`)

Please:
1. Discover all sessions under `~/.gemini/**/brain/` for projects `nuttxspace-vanilla` and `nuttxspace-egm`, including any orphaned or subagent sessions spawned during those runs.
2. Read the `.cast` header `timestamp` and duration (`--info` via `trim-cast.py`) for every recording in `recordings/raw/` and correlate each `.cast` file against the discovered sessions (`Created (Local)` and `Duration`).
3. List any aborted or superseded takes that do not match the canonical recordings and ask my confirmation before moving them to Trash.
4. Export the canonical Vanilla parent/subagent traces to `docs/prompts-vanilla/` and the canonical EGM parent/subagent traces to `docs/prompts-egm/`.
5. Print a side-by-side telemetry summary comparing both experiments: session count (parent vs. subagent), total steps, `read_file` calls, `README.md` contract reads, and `run_command` build/test attempts.
```

### Trace Extraction Checklist

- [ ] Every raw `.cast` file in `recordings/raw/` is matched to its exact AGY parent session(s) by start timestamp and duration.
- [ ] Any aborted/superseded takes have been confirmed and trashed cleanly.
- [ ] `docs/prompts-vanilla/README.md` and `docs/prompts-egm/README.md` exist with complete exported traces.

---

## 7. Post-Recording Step 2: Clean Up `.cast` Files, Stitch EGM & Build HTML Players

In the same `agy` session inside `~/Documents/GitHub/nuttxspace` (or after `/clear`), invoke `/asciinema` and `/cast-player-html` to clean up the recordings, cross-reference stop points with the exported traces, and generate the standalone HTML comparison players.

### 7.1 Prompt: Sanitize, Trim, Stitch, Mark Stop Points & Embed HTML Players

```text
/asciinema /cast-player-html Now clean up the raw recordings in `recordings/raw/` and build standalone HTML comparison players using the exported traces in `docs/prompts-vanilla/` and `docs/prompts-egm/` for context:

1. Sanitize (`sanitize-cast.py`) and trim (`trim-cast.py`) `recordings/raw/vanilla-vibe.cast` into `recordings/final/vanilla-vibe.cast`, and do the same for `01-bootstrap.cast`..`05-implement.cast` into `recordings/final/egm-phases/`.
2. Stitch the five trimmed EGM phase casts (`01-bootstrap.cast` through `05-implement.cast`) in order into `recordings/final/egm-flow.cast`, keeping the `asciicast v3` header from `01-bootstrap.cast`, dropping intermediate `"x"` exit events, clearing the screen (`\u001b[2J\u001b[H`) between phases, inserting numbered phase boundary `"m"` markers, and preserving the final `"x"` hold only at the end of `05-implement.cast`.
3. Inspect stdout events (`mark-cast.py --search` / `--inspect`) alongside `docs/prompts-vanilla/` and `docs/prompts-egm/` to place numbered `"m"` stop points at key milestones (command entry, interactive questions/prompts using the two-stop pattern, `/clear` boundaries, review passes, and final `make test` / `make` `.uf2` build verification), then run `--tighten-pastes`.
4. Scaffold `prototypes/vanilla-vibe-cast-player.html` and `prototypes/egm-flow-cast-player.html` from `.agents/skills/cast-player-html/resources/cast-player-template.html`, update their titles/headers, embed `EMBEDDED_CAST_GZ_B64` via `mark-cast.py --embed-html`, and verify valid JS syntax (`node --check`) and marker timing (`mark-cast.py --list`). (If `egm-flow.cast` exceeds ~50 stop points, omit `<div class="chapter-pills" id="chapter-pills"></div>` per the `cast-player-html` skill so the player fits inside the 16:9 frame.)
```

### Cast Cleanup Checklist

- [ ] `recordings/final/vanilla-vibe.cast` and `recordings/final/egm-flow.cast` pass `trim-cast.py --info` with zero trailing exit prompts.
- [ ] `mark-cast.py --list` shows clean, sequentially numbered stop points aligned to command lines and two-stop interactive prompts.
- [ ] `prototypes/vanilla-vibe-cast-player.html` and `prototypes/egm-flow-cast-player.html` both pass `node --check` and open cleanly via `file://` in a browser.

---

## 8. Post-Recording Step 3: Goldfish Audit & Comparative Scorecard

Once traces are exported and recordings are packaged, run an audit prompt in `~/Documents/GitHub/nuttxspace` (using fresh Goldfish subagents) to grade the Vanilla implementation against the EGM implementation across all 10 hardware/RTOS traps from [`docs/reports/goldfish_review_report.md`](docs/reports/goldfish_review_report.md) and [`docs/designs/concepts_brief.md`](docs/designs/concepts_brief.md).

### 8.1 Prompt: Side-by-Side Goldfish Audit & Telemetry Report

```text
Spawn `egm-codebase-details` and `egm-design` as fresh read-only auditors (with permission to read `~/Documents/GitHub/nuttxspace-vanilla` and `~/Documents/GitHub/nuttxspace-egm`, without modifying either repo) to compare the two generated `xiao-rp2350-bargraph/` implementations alongside:
- Vanilla traces: `docs/prompts-vanilla/`
- EGM traces: `docs/prompts-egm/`
- Recording durations & stop points: `recordings/final/vanilla-vibe.cast` and `recordings/final/egm-flow.cast`

Grade both implementations against the 10-point hardware/RTOS scorecard in `EXPERIMENT_RUNBOOK.md` (Section 8.2), cite exact file paths and line numbers for every defect or pass, and write the filled comparison report (with the telemetry table from Section 8.3) to `docs/reports/vanilla-vs-egm-comparison.md`.
```

### 8.2 Side-by-Side 10-Point Defect Scorecard (Reference Rubric)

| # | Hardware / RTOS Invariant | What Goes Wrong Without Checking Deeply | Vanilla (`nuttxspace-vanilla`) | EGM (`nuttxspace-egm`) |
| :---: | :--- | :--- | :--- | :--- |
| **1** | **Submodule Isolation** | `git -C nuttx status -s` or `git -C apps status -s` shows tracked edits instead of pure out-of-tree integration (`CONFIG_ARCH_BOARD_CUSTOM=y` + `apps/external` symlink). | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |
| **2** | **ADC `my_reset()` vs. LED Pins (`GPIO 26–29`)** | `rp23xx_adc.c:my_reset()` resets all 4 ADC pins (`GPIO 26–29`, i.e. `D0–D2`) to input `NULL` on `adc_register()`. Placing LEDs on `D1`/`D2` clobbers them unless avoided or re-ordered. | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |
| **3** | **RP2350-E9 Pad Input Enable (`IE`)** | `my_reset()` re-enables `RP23XX_PADS_BANK0_GPIO_IE` via `rp23xx_gpio_set_function()`. Failing to clear `IE` after `rp23xx_common_bringup()` leaks mid-rail analog current on `GPIO 26`. | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |
| **4** | **19-Bit ADC Scaling & Error Sign** | `rp23xx_adc.c` shifts the 12-bit sample left (`value <<= 19`) and negates on `RP23XX_ADC_FIFO_ERR`. Dividing `am_data` by `4095` saturates every reading above 0 V to 100%. | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |
| **5** | **500 kHz ADC IRQ Storm & Stale FIFO** | Keeping `/dev/adc0` open during `usleep()` leaves the ~500 kHz FIFO ISR running continuously and fills `CONFIG_ADC_FIFOSIZE=8` with stale readings instead of `open`/`read`/`close` per tick. | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |
| **6** | **`close()` vs. ADC FIFO ISR Teardown Race (`CON-1`)** | `adc_close()` holds only `ad_lock` while `rp23xx_adc.c:remove_device()` modifies `g_first_device` & `g_active_count` without an IRQ critical section, triggering `ASSERT()` unless guarded (e.g. `enter_critical_section()`), plus `O_NONBLOCK` + `poll()`. | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |
| **7** | **Bringup & Driver Kconfig Wiring (`CON-2`)** | Missing `CONFIG_ADC`, `CONFIG_RP23XX_ADC`, `CONFIG_RP23XX_ADC_CHANNEL0`, `CONFIG_USERLED`, `CONFIG_USERLED_LOWER`, or `rp23xx_bringup()` registration leaves `/dev/adc0` or `/dev/userleds` absent when `rcS` runs. | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |
| **8** | **Headless `rcS` Boot + USB-C NSH (`CON-5`)** | Blocking in foreground before `nsh_waitusbready()`, enabling `CONFIG_NSH_DISABLEBG=y`, or mis-indexing `task_create()`'s shifted `argv[1]` breaks either autonomous startup or USB NSH. | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |
| **9** | **Multi-LED `rp23xx_userleds.c` (`CONFIG_ARCH_LEDS`)** | Leaving `CONFIG_ARCH_LEDS=y` compiles `rp23xx_autoleds.c` (single onboard LED) instead of `rp23xx_userleds.c` implementing active-high `board_userled_all()` across 6 LEDs. | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |
| **10** | **Host Unit Test Separation (`CON-9`)** | Coupling hysteresis/quantization logic to `<nuttx/...>` headers prevents `make -C xiao-rp2350-bargraph test` from running on the host C compiler. | `[ ] Pass / [ ] Fail` | `[ ] Pass / [ ] Fail` |

### 8.3 Telemetry Comparison Table (Target Output Schema)

| Metric | Vanilla (`docs/prompts-vanilla`) | EGM (`docs/prompts-egm`) |
| :--- | :--- | :--- |
| **Raw Recording Duration** | | |
| **Playback Duration (`idle=2.0s`)** | | |
| **Parent + Subagent Sessions** | | |
| **Total Trajectory Steps** | | |
| **`read_file` Calls / `README.md` Reads** | | |
| **Build Failures Before Clean `.uf2`** | | |
| **Scorecard Pass Count (`X / 10`)** | | |
