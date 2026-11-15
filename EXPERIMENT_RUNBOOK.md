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
| **Post-Run File Capture** | `docs/vanilla/xiao-rp2350-bargraph/` clean source snapshot + any workspace `.md` or `~/.gemini` brain artifacts (`docs/vanilla/brain/`, if any; Run 1 typically generates zero planning docs). | `docs/egm/` containing root `README.md`, `designs/` (`concepts_brief.md`, `design_draft.md`, `xiao-rp2350-bargraph.md`), `reports/` (`*DRIFT_REPORT.md`, `pass1/*_report.md`, `pass2/*_report.md`), `brain/` artifacts, and `docs/egm/xiao-rp2350-bargraph/` clean source snapshot. |
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
  - **AGY terminal sandbox allowlist**: Because `agy`, `kconfig-conf`, `kconfig-tweak`, and `kconfiglib` (`olddefconfig`) live under `/Users/brett/.local`, configure `read_file(/Users/brett/.local)` and `read_file(/Users/brett/.cargo)` using literal absolute paths (without `~` or `/**`, which the sandbox parser rejects) so they run cleanly inside the default sandbox.
- **Fedora 44 (`Linux`)**:
  - Follow [`docs/designs/TODO.md`](docs/designs/TODO.md): `gcc`, `cpp`, `make`, `bison`, `flex`, `gperf`, `gettext`, `texinfo`, `ncurses-devel`, `/usr/local/bin/kconfig-conf`, `kconfig-tweak`, `kconfig-mconf`, `genromfs`, `xxd`, `picocom`, `arm-none-eabi-gcc-cs`, `arm-none-eabi-gcc-cs-c++`, `arm-none-eabi-binutils-cs`, `arm-none-eabi-newlib`, `cmake`, `libusb1-devel`, and `picotool`.

### Checklist

- [x] `asciinema`, `agy`, and `uv` all execute cleanly from the target terminal shell (`PATH` includes `~/.local/bin`). *(Verified 2026-11-15: `asciinema 3.2.1`, `agy 1.5.9`, `uv 0.13.8`, `Python 3.15.2` inside default sandbox.)*
- [x] `arm-none-eabi-gcc`, `genromfs`, `xxd`, `picotool`, and `kconfig-conf` are on `PATH`. *(Verified 2026-11-15: `arm-none-eabi-gcc 15.3.1`, `GNU Make 3.81`, `Apple clang 21.0.0`, `bison 2.3`, `flex 2.6.4`, `gperf 3.0.3`, `genromfs 0.5.2`, `/usr/bin/xxd`, `picotool v2.3.1`; `kconfig-conf`, `kconfig-tweak`, and `olddefconfig` verified in `/Users/brett/.local/bin`. Note: `kconfig-mconf` is not installed, while `kconfiglib`'s `menuconfig` is in `/Users/brett/.local/bin` and automated builds only need `kconfig-conf`/`kconfig-tweak`/`olddefconfig`.)*
- [x] You have confirmed the baseline terminal window works at `120x36` (`--window-size 120x36`). *(Verified 2026-11-15: `asciinema rec -i 2.0 --window-size 120x36` tested cleanly.)*

#### Pre-Flight Verification Status (2026-11-15, macOS Sandbox)

| Component | Verified Value / Path | Status |
| :--- | :--- | :---: |
| **Recording & Agent** | `asciinema 3.2.1`, `agy 1.5.9`, `uv 0.13.8`, `Python 3.15.2` | PASS |
| **Cross-Toolchain & Build** | `arm-none-eabi-gcc 15.3.1`, `GNU Make 3.81`, `Apple clang 21.0.0`, `bison 2.3`, `flex 2.6.4`, `gperf 3.0.3` | PASS |
| **ROMFS & UF2** | `genromfs 0.5.2`, `/usr/bin/xxd`, `picotool v2.3.1` | PASS |
| **Kconfig Frontends** | `kconfig-conf`, `kconfig-tweak`, `olddefconfig` in `/Users/brett/.local/bin` (`kconfig-mconf` not installed; `menuconfig` via `kconfiglib` present) | PASS |
| **Sandbox Allowlist** | `read_file(/Users/brett/.local)` and `read_file(/Users/brett/.cargo)` verified inside default sandbox | PASS |

---

## 3. Run 1 Setup: Provision Workspace A (`nuttxspace-vanilla`, `master`)

Store raw and final `.cast` recordings plus HTML players inside this repository (`nuttxspace`), while keeping the two experiment workspaces in clean sibling directories. Using `--reference` reuses the already-fetched Git objects inside `~/Documents/GitHub/nuttxspace/{nuttx,apps}` so submodule creation finishes in seconds.

### 3.1 Create Recording & Artifact Capture Directories in `nuttxspace`

```bash
mkdir -p ~/Documents/GitHub/nuttxspace/recordings/raw \
         ~/Documents/GitHub/nuttxspace/recordings/final \
         ~/Documents/GitHub/nuttxspace/prototypes \
         ~/Documents/GitHub/nuttxspace/docs/vanilla \
         ~/Documents/GitHub/nuttxspace/docs/egm
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

### Run 1 Provisioning Verification Checklist

- [ ] `~/Documents/GitHub/nuttxspace-vanilla` has clean `git status`, `nuttx` at `master` (`562db43781`), `apps` at `master` (`ab105e659c`), and `14` `README.md` files total.
- [ ] `nuttxspace-vanilla` does not contain `xiao-rp2350-bargraph/` or `docs/` yet.
- [ ] Note down any existing `nuttxspace-vanilla` sessions in `~/.gemini` (none should exist before recording).

---

## 4. Run 1 Recording: Vanilla Vibe Coding (`nuttxspace-vanilla`)

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

### Run 1 Recording Checklist

- [ ] `~/Documents/GitHub/nuttxspace/recordings/raw/vanilla-vibe.cast` exists and is non-empty.
- [ ] Build/test outcome (`PASS` or exact blocker) and `git status` of `nuttx/` + `apps/` were captured on camera.

---

## 5. Run 1 Post-Processing: Vanilla Trace Export, Artifact Capture & Cast HTML Creation

Once `vanilla-vibe.cast` is saved in `recordings/raw/`, switch to **`~/Documents/GitHub/nuttxspace`** and start `agy` (note: this workspace already includes the `/agy-brain-explorer`, `/asciinema`, and `/cast-player-html` skills).

### 5.1 Launch `agy` in `nuttxspace`

```bash
cd ~/Documents/GitHub/nuttxspace
agy
```

### 5.2 Prompt: Correlate `vanilla-vibe.cast`, Clean Dead Takes, Export Vanilla Traces & Capture Vanilla Files (`/agy-brain-explorer`)

Paste this prompt into `agy` (approve `BypassSandbox` when prompted so `/agy-brain-explorer` can read `~/.gemini/**/brain/`):

```text
/agy-brain-explorer I just finished capturing Run 1 (Experiment A: Vanilla Vibe Coding) in `~/Documents/GitHub/nuttxspace-vanilla` and need to match `recordings/raw/vanilla-vibe.cast` to its full Antigravity parent + subagent traces and capture any generated files:

Please:
1. Discover all sessions under `~/.gemini/**/brain/` for project `nuttxspace-vanilla`, including any orphaned or subagent sessions spawned during the run.
2. Read the `.cast` header `timestamp` and duration (`--info` via `trim-cast.py`) for `recordings/raw/vanilla-vibe.cast` and correlate it against the discovered `nuttxspace-vanilla` sessions (`Created (Local)` and `Duration`).
3. List any aborted or superseded takes that do not match the canonical `vanilla-vibe.cast` recording and ask my confirmation before moving them to Trash.
4. Export the canonical Vanilla parent/subagent traces to `docs/prompts-vanilla/`.
5. Capture Run 1's generated files into `docs/vanilla/`:
   - Copy `~/Documents/GitHub/nuttxspace-vanilla/xiao-rp2350-bargraph/` into `docs/vanilla/xiao-rp2350-bargraph/`, excluding build outputs (`*.o`, `*.d`, `.depend`, `Make.dep`, `.built`, `.kconfig`, and compiled host test binaries such as `tests/test_bargraph_logic`).
   - Check for any planning, design, or documentation Markdown files created in `~/Documents/GitHub/nuttxspace-vanilla/` (outside `nuttx/` and `apps/`), as well as any Markdown artifacts (`implementation_plan.md`, `task.md`, `walkthrough.md`) inside the canonical session's `~/.gemini/**/brain/<session-id>/` directory. Copy any that exist into `docs/vanilla/` (or `docs/vanilla/brain/`), and explicitly report if Run 1 generated zero planning files.
6. Print a Vanilla telemetry summary: session count (parent vs. subagent), total steps, `read_file` calls, `README.md` reads, `run_command` build/test attempts, and count of planning/design files generated (`0` expected for Vanilla).
```

### 5.3 Prompt: Sanitize, Trim, Mark Stop Points & Build Vanilla HTML Player (`/asciinema` `/cast-player-html`)

In the same `agy` session inside `~/Documents/GitHub/nuttxspace` (or after `/clear`), invoke `/asciinema` and `/cast-player-html`:

```text
/asciinema /cast-player-html Now clean up `recordings/raw/vanilla-vibe.cast` and build the standalone Vanilla HTML player using the exported traces in `docs/prompts-vanilla/` for context:

1. Sanitize (`sanitize-cast.py`) and trim (`trim-cast.py`) `recordings/raw/vanilla-vibe.cast` into `recordings/final/vanilla-vibe.cast`.
2. Inspect stdout events (`mark-cast.py --search` / `--inspect`) alongside `docs/prompts-vanilla/` to place numbered `"m"` stop points at key milestones (command entry, interactive questions/prompts using the two-stop pattern, and final `make test` / `make` `.uf2` build verification), then run `--tighten-pastes`.
3. Scaffold `prototypes/vanilla-vibe-cast-player.html` from `.agents/skills/cast-player-html/resources/cast-player-template.html`, update its title/header, embed `EMBEDDED_CAST_GZ_B64` via `mark-cast.py --embed-html`, and verify valid JS syntax (`node --check`) and marker timing (`mark-cast.py --list`).
```

### Run 1 Post-Processing Checklist

- [ ] `recordings/raw/vanilla-vibe.cast` is matched to its exact AGY parent session(s) by start timestamp and duration, and any aborted/superseded takes have been confirmed and trashed cleanly.
- [ ] `docs/prompts-vanilla/README.md` exists with complete exported traces.
- [ ] `docs/vanilla/xiao-rp2350-bargraph/` contains a clean snapshot of the Vanilla source tree (with build binaries/objects stripped), and any optional workspace/brain Markdown files have been captured in `docs/vanilla/` (or confirmed absent).
- [ ] `recordings/final/vanilla-vibe.cast` passes `trim-cast.py --info` with zero trailing exit prompts, and `mark-cast.py --list` shows clean, sequentially numbered stop points aligned to command lines and two-stop interactive prompts.
- [ ] `prototypes/vanilla-vibe-cast-player.html` passes `node --check` and opens cleanly via `file://` in a browser.

---

## 6. Run 2 Setup: Provision Workspace B (`nuttxspace-egm`, `egm`)

### 6.1 Provision Workspace B (`~/Documents/GitHub/nuttxspace-egm`, `egm`)

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

### Run 2 Provisioning Verification Checklist

- [ ] `~/Documents/GitHub/nuttxspace-egm` has clean `git status`, `nuttx` at `egm` (`b047ea1315`), `apps` at `egm` (`f711fe028`), and `3,242` `README.md` files total.
- [ ] `nuttxspace-egm` does not contain `xiao-rp2350-bargraph/` or `docs/` yet.
- [ ] Note down any existing `nuttxspace-egm` sessions in `~/.gemini` (none should exist before recording).

---

## 7. Run 2 Recording: Full EGM Flow (`nuttxspace-egm`)

Record **five separate `.cast` files** (one per EGM phase), committing clean checkpoints between phases so any quota interruption or retake only affects that single phase.

> [!TIP]
> Before each phase recording, check `git status` is clean, launch `asciinema rec -i 2.0 --window-size 120x36 --overwrite ...`, start `agy`, and end cleanly with `/exit` + shell `exit`.

---

### 7.1 Recording 1 (`01-bootstrap.cast`) — Phase 1: Workspace Bootstrap & Drift Audit

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
   mkdir -p docs/reports
   mv -f *DRIFT_REPORT.md docs/reports/ 2>/dev/null || true
   git add README.md docs/reports
   git status -s
   git commit -m "Phase 1: bootstrap root README.md and run drift check"
   ```

Exit `agy` and exit the shell to finish `01-bootstrap.cast`.

---

### 7.2 Recording 2 (`02-brainstorm.cast`) — Phase 2: Plain-English Discovery (`/egm-brainstorm`)

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

Note that `/egm-brainstorm` writes `concepts_brief.md` in the workspace root (or `docs/designs/concepts_brief.md`). Exit `agy`, commit it, and exit the shell:

```bash
[ -f concepts_brief.md ] && git add concepts_brief.md
[ -f docs/designs/concepts_brief.md ] && git add docs/designs/concepts_brief.md
git commit -m "Phase 2: finalize brainstorm concepts_brief.md"
```

---

### 7.3 Recording 3 (`03-draft.cast`) — Phase 3: Grounded Technical Spec (`/egm-draft`)

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
- Review and approve each of the four sections via the Section Approval Gate (`Overview & Goals`, `Technical Plan`, `Alternatives Considered`, `Detailed Implementation`), which `/egm-draft` writes to `design_draft.md` in the workspace root before promoting the finalized design spec to `docs/designs/xiao-rp2350-bargraph.md`.
- Confirm saving the finalized design doc to `docs/designs/xiao-rp2350-bargraph.md`.

Exit `agy`, save an untouched pre-review snapshot (`xiao-rp2350-bargraph.v0-draft.md`) alongside `design_draft.md` so Pass 1 and Pass 2 changes can be diffed later, commit, and exit the shell:

```bash
cp docs/designs/xiao-rp2350-bargraph.md docs/designs/xiao-rp2350-bargraph.v0-draft.md
[ -f design_draft.md ] && git add design_draft.md
git add docs/designs/xiao-rp2350-bargraph.md docs/designs/xiao-rp2350-bargraph.v0-draft.md
git commit -m "Phase 3: draft technical design spec docs/designs/xiao-rp2350-bargraph.md"
```

---

### 7.4 Recording 4 (`04-review.cast`) — Phase 4: Two-Pass Consolidated Review & Resolution

Run **at least two** Consolidated `/egm-review` + `/egm-resolve` passes (resetting context with `/clear` between Pass 1 and Pass 2 so Pass 2 operates like a fresh Elephant on the updated spec).

```bash
cd ~/Documents/GitHub/nuttxspace-egm
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/04-review.cast
```

> [!IMPORTANT]
> **Archive Pass 1 reports before starting Pass 2**: Both `/egm-review` and `/egm-resolve` write fixed filenames in the workspace root (`comprehension_report.md`, `critic_report.md`, `intermediate_zoom_out_report.md`, `zoom_out_report.md`, `implementation_readiness_report.md`, `consolidated_review_report.md`, and `goldfish_review_report.md`). Running Pass 2 without moving Pass 1's reports first overwrites Pass 1's findings in place.

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

3. **Archive Pass 1 Reports Before Pass 2:**
   Send a brief follow-up prompt in `agy` (or run a shell escape) to snapshot Pass 1 before clearing context:
   ```text
   Archive the Pass 1 reports (`*_report.md`) and a snapshot of the Pass 1 resolved design spec (`xiao-rp2350-bargraph.pass1.md`) into `docs/reports/pass1/` before starting Pass 2 so Pass 2 does not overwrite them.
   ```

4. **Reset Context & Run Pass 2:**
   ```text
   /clear
   ```
   ```text
   /egm-review @[docs/designs/xiao-rp2350-bargraph.md]

   Instruct all Goldfish reviewer subagents to read the nearest local EGM README.md contracts in nuttx/ and apps/ alongside the referenced .c, .h, .mk, and Kconfig files.
   ```
   - Again select **`Consolidated report with all review rubrics (runs full flow without stopping)`**.
   - Run `/egm-resolve @[consolidated_review_report.md] target @[docs/designs/xiao-rp2350-bargraph.md]` and reply **`auto accept`** (or resolve any contested findings).
   - *(Optional Pass 3: if Pass 2 still surfaces Blockers/Majors like the `close()` vs. ISR teardown race or blocking `adc_read()`, archive Pass 2's `*_report.md` and `xiao-rp2350-bargraph.pass2.md` into `docs/reports/pass2/` first, then run one more `/clear` -> `/egm-review` -> `/egm-resolve` cycle and archive Pass 3 into `docs/reports/pass3/`.)*

Exit `agy`, archive Pass 2's reports and resolved spec snapshot, commit, and exit the shell:

```bash
mkdir -p docs/reports/pass2
cp -f *_report.md docs/reports/ 2>/dev/null || true
mv -f *_report.md docs/reports/pass2/ 2>/dev/null || true
cp docs/designs/xiao-rp2350-bargraph.md docs/reports/pass2/xiao-rp2350-bargraph.pass2.md
git add docs/designs/xiao-rp2350-bargraph.md docs/reports
git commit -m "Phase 4: complete consolidated EGM reviews and resolutions"
```

---

### 7.5 Recording 5 (`05-implement.cast`) — Phase 5: Firmware Implementation & Verification

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

### Run 2 Recording Checklist

- [ ] All 5 raw `.cast` files (`01-bootstrap.cast` through `05-implement.cast`) exist in `~/Documents/GitHub/nuttxspace/recordings/raw/`.
- [ ] `make -C xiao-rp2350-bargraph test` and `make -C xiao-rp2350-bargraph` both succeeded in `nuttxspace-egm`, producing `nuttx/nuttx.uf2`.
- [ ] `git -C nuttx status -s` and `git -C apps status -s` show zero modified tracked files.

---

## 8. Run 2 Post-Processing: EGM Trace Export, Artifact Capture & Cast HTML Creation

Once all five EGM recordings are saved in `recordings/raw/`, switch to **`~/Documents/GitHub/nuttxspace`** and start `agy` (note: this workspace already includes the `/agy-brain-explorer`, `/asciinema`, and `/cast-player-html` skills).

### 8.1 Reference Inventory: Useful Files Generated by EGM Across Phases 1–5

Unlike Run 1 (which typically produces only C/Makefile/Kconfig implementation files), Run 2 generates a structured chain of architectural contracts, design briefs, multi-persona critique logs, and resolution ledgers that explain *why* each firmware decision was made:

| Phase / Skill | Source File(s) in `nuttxspace-egm` | Archive Target in `nuttxspace/docs/egm/` | Why It Is Useful Later |
| :--- | :--- | :--- | :--- |
| **Phase 1** (`/egm-bootstrap`) | `README.md` | `docs/egm/README.md` | Workspace architectural blueprint and submodule boundary contract. |
| **Phase 1** (`/egm-drift-report`) | `*DRIFT_REPORT.md` / `docs/reports/*DRIFT_REPORT.md` | `docs/egm/reports/*DRIFT_REPORT.md` | Verification audit catching hallucinated symbols/variables in `README.md` contracts. |
| **Phase 2** (`/egm-brainstorm`) | `concepts_brief.md` (or `docs/designs/concepts_brief.md`) | `docs/egm/designs/concepts_brief.md` | Plain-English discovery brief synthesized by the 7 Goldfish lenses (`CON-*` hardware constraints, pin choices, daemon lifecycle). |
| **Phase 3** (`/egm-draft`) | `design_draft.md`, `docs/designs/xiao-rp2350-bargraph.v0-draft.md` | `docs/egm/designs/design_draft.md`, `docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md` | Initial 4-section grounded technical specification *before* adversarial review—useful for diffing what `/egm-draft` got right vs. what `/egm-review` caught. |
| **Phase 4** (`/egm-review`, Pass 1 & Pass 2) | `docs/reports/pass1/*_report.md`, `docs/reports/pass2/*_report.md` (`comprehension_report.md`, `critic_report.md`, `structural_report.md` / `style_report.md` if emitted, `intermediate_zoom_out_report.md`, `zoom_out_report.md`, `implementation_readiness_report.md`, `consolidated_review_report.md`) | `docs/egm/reports/pass1/` and `docs/egm/reports/pass2/` | Per-persona and deduplicated critique reports (`C-01`..`C-22`): naive-reader jargon checks (`egm-comprehension`), deep hardware/RTOS/Kconfig/ISR defects (`egm-design` + `egm-codebase-details`), YAGNI/TRIZ simplification (`egm-zoom-out` + `egm-inventive-catalyst`), and build actionability (`egm-implementation-readiness`). |
| **Phase 4** (`/egm-resolve`, Pass 1 & Pass 2) | `goldfish_review_report.md` (per pass) + `xiao-rp2350-bargraph.pass1.md` / `.pass2.md` | `docs/egm/reports/pass1/`, `docs/egm/reports/pass2/`, plus latest `docs/egm/reports/goldfish_review_report.md` | Resolution ledger recording every finding's disposition (`ACCEPT`, `REJECT`, `MODIFY`) and per-pass design snapshots showing progressive defect elimination. |
| **Phase 4** (Final Spec) | `docs/designs/xiao-rp2350-bargraph.md` | `docs/egm/designs/xiao-rp2350-bargraph.md` | The final hardened implementation specification fed into Phase 5. |
| **Phase 5** (Implementation) | `xiao-rp2350-bargraph/` | `docs/egm/xiao-rp2350-bargraph/` | Clean out-of-tree board config, driver bringup, daemon C code, and host unit test source tree (stripped of build binaries/object files). |
| **Optional / Brain Artifacts** | `TODO.md`, `~/.gemini/**/brain/<session-id>/*.md` (`implementation_plan.md`, `task.md`, `walkthrough.md`) | `docs/egm/designs/TODO.md`, `docs/egm/brain/` | Supplemental checklists or Antigravity session brain artifacts produced during any EGM phase. |

### 8.2 Launch `agy` in `nuttxspace`

```bash
cd ~/Documents/GitHub/nuttxspace
agy
```

### 8.3 Prompt: Correlate EGM Phase Casts, Clean Dead Takes, Export EGM Traces & Capture EGM Files (`/agy-brain-explorer`)

Paste this prompt into `agy` (approve `BypassSandbox` when prompted so `/agy-brain-explorer` can read `~/.gemini/**/brain/`):

```text
/agy-brain-explorer I just finished capturing Run 2 (Experiment B: Full EGM Flow) in `~/Documents/GitHub/nuttxspace-egm` and need to match the five EGM phase recordings in `recordings/raw/` (`01-bootstrap.cast`, `02-brainstorm.cast`, `03-draft.cast`, `04-review.cast`, and `05-implement.cast`) to their full Antigravity parent + subagent traces and capture all generated EGM files:

Please:
1. Discover all sessions under `~/.gemini/**/brain/` for project `nuttxspace-egm`, including any orphaned or subagent sessions (including Goldfish subagents) spawned during those runs.
2. Read the `.cast` header `timestamp` and duration (`--info` via `trim-cast.py`) for `recordings/raw/01-bootstrap.cast` through `05-implement.cast` and correlate each `.cast` file against the discovered `nuttxspace-egm` sessions (`Created (Local)` and `Duration`).
3. List any aborted or superseded takes that do not match the canonical recordings and ask my confirmation before moving them to Trash.
4. Export the canonical EGM parent/subagent traces to `docs/prompts-egm/`.
5. Capture all useful generated EGM files from `~/Documents/GitHub/nuttxspace-egm` and the canonical `~/.gemini/**/brain/` sessions into `docs/egm/`:
   - Root architectural contract: copy `README.md` -> `docs/egm/README.md`.
   - Design & planning specs: copy `concepts_brief.md` (root or `docs/designs/`), `design_draft.md`, `docs/designs/xiao-rp2350-bargraph.v0-draft.md`, `docs/designs/xiao-rp2350-bargraph.md`, and `TODO.md` (if present) into `docs/egm/designs/`.
   - Drift, review & resolution reports: copy `docs/reports/` (including `*DRIFT_REPORT.md`, `pass1/*`, `pass2/*`, any `pass3/*`, and top-level `*_report.md` files) and any root-level `*_report.md` / `*DRIFT_REPORT.md` files into `docs/egm/reports/`.
   - Clean implementation snapshot: copy `xiao-rp2350-bargraph/` into `docs/egm/xiao-rp2350-bargraph/`, excluding build outputs (`*.o`, `*.d`, `.depend`, `Make.dep`, `.built`, `.kconfig`, and compiled host test binaries such as `tests/test_bargraph_logic`).
   - Brain artifacts: check the canonical `nuttxspace-egm` session folders under `~/.gemini/**/brain/` for any generated `.md` artifacts (`implementation_plan.md`, `task.md`, `walkthrough.md`) and copy any found into `docs/egm/brain/<session-prefix>/`.
6. Print a side-by-side telemetry summary comparing `docs/prompts-egm/` + `docs/egm/` against `docs/prompts-vanilla/` + `docs/vanilla/`: session count (parent vs. subagent), total steps, `read_file` calls, `README.md` contract reads, `run_command` build/test attempts, and count of planning/design/review files generated.
```

### 8.4 Prompt: Sanitize, Trim, Stitch EGM Phases, Mark Stop Points & Build EGM HTML Player (`/asciinema` `/cast-player-html`)

In the same `agy` session inside `~/Documents/GitHub/nuttxspace` (or after `/clear`), invoke `/asciinema` and `/cast-player-html`:

```text
/asciinema /cast-player-html Now clean up the five EGM phase recordings in `recordings/raw/`, stitch them into `recordings/final/egm-flow.cast`, and build the standalone EGM HTML player using the exported traces in `docs/prompts-egm/` for context:

1. Sanitize (`sanitize-cast.py`) and trim (`trim-cast.py`) `recordings/raw/01-bootstrap.cast`..`05-implement.cast` into `recordings/final/egm-phases/`.
2. Stitch the five trimmed EGM phase casts (`01-bootstrap.cast` through `05-implement.cast`) in order into `recordings/final/egm-flow.cast`, keeping the `asciicast v3` header from `01-bootstrap.cast`, dropping intermediate `"x"` exit events, clearing the screen (`\u001b[2J\u001b[H`) between phases, inserting numbered phase boundary `"m"` markers, and preserving the final `"x"` hold only at the end of `05-implement.cast`.
3. Inspect stdout events (`mark-cast.py --search` / `--inspect`) alongside `docs/prompts-egm/` to place numbered `"m"` stop points at key milestones (command entry, interactive questions/prompts using the two-stop pattern, `/clear` boundaries, review passes, and final `make test` / `make` `.uf2` build verification), then run `--tighten-pastes`.
4. Scaffold `prototypes/egm-flow-cast-player.html` from `.agents/skills/cast-player-html/resources/cast-player-template.html`, update its title/header, embed `EMBEDDED_CAST_GZ_B64` via `mark-cast.py --embed-html`, and verify valid JS syntax (`node --check`) and marker timing (`mark-cast.py --list`). (If `egm-flow.cast` exceeds ~50 stop points, omit `<div class="chapter-pills" id="chapter-pills"></div>` per the `cast-player-html` skill so the player fits inside the 16:9 frame.)
```

### Run 2 Post-Processing Checklist

- [ ] All 5 raw EGM `.cast` files (`01-bootstrap.cast` through `05-implement.cast`) are matched to their exact AGY parent/subagent session(s) by start timestamp and duration, and any aborted/superseded takes have been confirmed and trashed cleanly.
- [ ] `docs/prompts-egm/README.md` exists with complete exported traces (including Goldfish subagents).
- [ ] `docs/egm/` contains `README.md`, `designs/` (`concepts_brief.md`, `design_draft.md`, `xiao-rp2350-bargraph.v0-draft.md`, `xiao-rp2350-bargraph.md`), `reports/` (`*DRIFT_REPORT.md`, `pass1/`, `pass2/`), and a clean `xiao-rp2350-bargraph/` source snapshot (with build binaries/objects stripped).
- [ ] `recordings/final/egm-flow.cast` passes `trim-cast.py --info` with zero trailing exit prompts, and `mark-cast.py --list` shows clean, sequentially numbered stop points aligned to phase boundaries, command lines, and two-stop interactive prompts.
- [ ] `prototypes/egm-flow-cast-player.html` passes `node --check` and opens cleanly via `file://` in a browser.

---

## 9. Final Comparative Step: Goldfish Audit & Defect Scorecard

Once both Run 1 and Run 2 traces are exported and HTML players are packaged, run an audit prompt in `~/Documents/GitHub/nuttxspace` (using fresh Goldfish subagents) to grade the Vanilla implementation against the EGM implementation across all 10 hardware/RTOS traps from [`docs/reports/goldfish_review_report.md`](docs/reports/goldfish_review_report.md) and [`docs/designs/concepts_brief.md`](docs/designs/concepts_brief.md).

### 9.1 Prompt: Side-by-Side Goldfish Audit & Telemetry Report

```text
Spawn `egm-codebase-details` and `egm-design` as fresh read-only auditors (with permission to read `~/Documents/GitHub/nuttxspace-vanilla` and `~/Documents/GitHub/nuttxspace-egm`, without modifying either repo) to compare the two generated `xiao-rp2350-bargraph/` implementations alongside:
- Vanilla traces: `docs/prompts-vanilla/`
- Vanilla captured source & artifacts: `docs/vanilla/` (and `~/Documents/GitHub/nuttxspace-vanilla`)
- EGM traces: `docs/prompts-egm/`
- EGM captured designs, per-pass review reports & source: `docs/egm/` (and `~/Documents/GitHub/nuttxspace-egm`)
- Recording durations & stop points: `recordings/final/vanilla-vibe.cast` and `recordings/final/egm-flow.cast`

Grade both implementations against the 10-point hardware/RTOS scorecard in `EXPERIMENT_RUNBOOK.md` (Section 9.2), cite exact file paths and line numbers for every defect or pass, and write the filled comparison report (with the telemetry table from Section 9.3) to `docs/reports/vanilla-vs-egm-comparison.md`.
```

### 9.2 Side-by-Side 10-Point Defect Scorecard (Reference Rubric)

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

### 9.3 Telemetry Comparison Table (Target Output Schema)

| Metric | Vanilla (`docs/prompts-vanilla`) | EGM (`docs/prompts-egm`) |
| :--- | :--- | :--- |
| **Raw Recording Duration** | | |
| **Playback Duration (`idle=2.0s`)** | | |
| **Parent + Subagent Sessions** | | |
| **Total Trajectory Steps** | | |
| **`read_file` Calls / `README.md` Reads** | | |
| **Build Failures Before Clean `.uf2`** | | |
| **Planning & Review Artifacts Generated** | | |
| **Scorecard Pass Count (`X / 10`)** | | |
