# Comparative Experiment Runbook: Vanilla Vibe Coding vs. Full EGM Flow

Working operator checklist for provisioning, recording, post-processing, trace-exporting, and auditing two head-to-head `asciinema` experiments replicating the XIAO RP2350 potentiometer LED bar-graph build from [`docs/tutorials/egm-replication-tutorial.md`](docs/tutorials/egm-replication-tutorial.md):

- **Experiment A (`nuttxspace-vanilla`)**: Upstream `nuttx` (`master`) and `apps` (`master`) checkouts with no EGM architectural `README.md` contracts, using a straight conversational vibe-coding loop.
- **Experiment B (`nuttxspace-egm`)**: `nuttx` (`egm`) and `apps` (`egm`) checkouts (`3,242` `README.md` contracts), starting by installing the EGM plugin (`/egm-review` and friends) via AGY's `/plugin` menu, running the full Engineering Guidance Model (EGM) skill flow (`/egm-bootstrap`, `/egm-drift-report`, `/egm-brainstorm`, `/egm-draft`, two passes of Consolidated `/egm-review` + `/egm-resolve`, and implementation), and finishing by installing `/teach-code` (`npx skills add domesticmouse/skills`) to build a hands-on board + firmware tutorial that navigates the comprehension debt of the generated code.

---

## 1. Overview & Experimental Design Matrix

| Dimension | Experiment A: Vanilla Vibe Coding | Experiment B: Full EGM Flow |
| :--- | :--- | :--- |
| **Isolated Workspace** | `~/Documents/GitHub/nuttxspace-vanilla` | `~/Documents/GitHub/nuttxspace-egm` |
| **Why Isolated?** | Must **not** contain `xiao-rp2350-bargraph/` or `docs/` so the agent cannot peek at finished code or prior designs. | Must **not** contain `xiao-rp2350-bargraph/` or `docs/` so every discovery and review finding happens fresh. |
| **Submodule Branches** | `nuttx` on `master` (`7` READMEs)<br>`apps` on `master` (`7` READMEs) | `nuttx` on `egm` (`2,608` `README.md` contracts)<br>`apps` on `egm` (`634` `README.md` contracts) |
| **Starting Baseline** | Functional requirements only (no prescribed GPIO pins, driver quirks, or Kconfig fixes). | Same functional requirements + EGM phase constraints (`One concern at a time`, read nearest `README.md`). |
| **Recording Layout** | Single continuous recording:<br>`recordings/raw/vanilla-vibe.cast` -> `recordings/final/vanilla-vibe.cast` and standalone HTML player `prototypes/vanilla-vibe-cast-player.html` | Seven separate per-phase recordings:<br>`recordings/raw/00-plugin.cast` … `06-teach-code.cast` -> `recordings/final/00-plugin.cast` … `06-teach-code.cast` and standalone HTML players `prototypes/00-plugin-cast-player.html` … `06-teach-code-cast-player.html` (kept separate for presentation slides between steps) |
| **Terminal Geometry** | `120x36` (`-i 2.0`) | `120x36` (`-i 2.0`) |
| **Stop Criterion** | `make -C xiao-rp2350-bargraph test` and `make -C xiao-rp2350-bargraph` produce `nuttx/nuttx.uf2` (or hit a terminal blocker), followed by generating a breadboard wiring Markdown file and matching SVG diagram. | Same build/test target after two passes of Consolidated `/egm-review` + `/egm-resolve`, followed by generating a breadboard wiring Markdown file and matching SVG diagram, and then installing `/teach-code` (`npx skills add domesticmouse/skills`) to generate a step-by-step tutorial on building both the breadboard circuit and the firmware code by hand. |
| **Off-Screen Capture** | Export via `/agy-brain-explorer` in `nuttxspace` (`nuttxspace-vanilla` -> `docs/prompts-vanilla/`) | Export incrementally after each phase via `/agy-brain-explorer` in `nuttxspace` (`nuttxspace-egm` -> `docs/prompts-egm/`, incl. Goldfish subagents) and review before starting the next phase |
| **Post-Run File Capture** | `docs/vanilla/xiao-rp2350-bargraph/` clean source snapshot + generated breadboard wiring `.md` / `.svg` files + any workspace `.md` or `~/.gemini` brain artifacts (`docs/vanilla/brain/`, if any; Run 1 typically generates zero planning docs). | Incrementally captured & reviewed per phase in `docs/egm/`: root `README.md`, `designs/` (`concepts_brief.md`, `design_draft.md`, `xiao-rp2350-bargraph.md`), `reports/` (`*DRIFT_REPORT.md`, `pass1/*_report.md`, `pass2/*_report.md`), `brain/` artifacts, generated breadboard wiring `.md` / `.svg` files, `tutorials/` (the captured `/teach-code` build-by-hand tutorial), `reviews/` (`00-plugin-review.md` … `06-teach-code-review.md` covering what each step achieved and reviewing its generated docs/code), and `docs/egm/xiao-rp2350-bargraph/` clean source snapshot. |
| **Post-Run Audit** | AGY comparative audit in `nuttxspace` scoring `nuttxspace-vanilla` against the 10-point hardware/RTOS scorecard and verifying its generated wiring Markdown + SVG. | Compare defect count, trace steps, `README.md` usage, build retries, and generated wiring SVG/guide accuracy against Vanilla. |

---

## 2. Phase 0: Pre-Flight Toolchain & Environment Checklist

Run these checks **before starting any recording** so missing host packages never derail an experiment mid-cast.

### Common Tools (macOS & Fedora 44)

```bash
# 1. Recording, CLI agent, and Python trace tooling
asciinema --version          # Verified with asciinema 3.2.1 (records asciicast v3)
agy --version                # Antigravity CLI
npx --version                # Required by `npx skills add domesticmouse/skills` (Run 2 Phase 6)
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
         ~/Documents/GitHub/nuttxspace/docs/egm \
         ~/Documents/GitHub/nuttxspace/docs/egm/tutorials \
         ~/Documents/GitHub/nuttxspace/docs/egm/reviews
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

- [x] `~/Documents/GitHub/nuttxspace-vanilla` has clean `git status`, `nuttx` at `master` (`562db43781`), `apps` at `master` (`ab105e659`), and `14` `README.md` files total. *(Verified post-Run 1: `nuttx` on `562db43781`, `apps` on `ab105e659`, `13` `README.md` files across `nuttx/` and `apps/`, and zero tracked/untracked submodule edits.)*
- [x] `nuttxspace-vanilla` does not contain `xiao-rp2350-bargraph/` or `docs/` yet. *(Confirmed clean prior to Run 1; `## main` at cast event `#0013`, `raw=2.83s`.)*
- [x] Note down any existing `nuttxspace-vanilla` sessions in `~/.gemini` (none should exist before recording). *(Confirmed: `0` prior sessions; only `60c052d5` exists.)*

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

### 4.3 Follow-Up Prompt: Breadboard Layout SVG & Circuit Wiring Guide (Post Code Generation)

Once firmware code generation and build/test verification complete (and before exiting `agy`), paste this follow-up prompt verbatim:

```text
Please generate a breadboard wiring description for the circuit matching the firmware.
Provide both a Markdown file with a parts list and wiring description, and a matching SVG diagramming the breadboard circuit.
```

### 4.4 Operator Rules During Experiment A

- **Do not coach embedded fixes**: Answer product-level clarifying questions if asked (e.g., *"6 LEDs is fine"*, *"any free breadboard pins you pick are fine"*, *"start automatically in the background so USB-C NSH still works"*), without hinting at `GPIO 26–29` `my_reset()`, `IE` leakage, `>> 19` shifting, `open/read/close` vs. 500 kHz IRQs, or `enter_critical_section()`.
- **Watch for `apps/external` symlink & clean submodules**: After the agent reports completion (including the generated wiring Markdown and SVG files), run these verification commands **on camera** before exiting the recording:

```bash
make -C xiao-rp2350-bargraph test
make -C xiao-rp2350-bargraph
ls -lh nuttx/nuttx nuttx/nuttx.uf2
git status -s
git -C nuttx status -s
git -C apps status -s
```

- Exit `agy` (`/exit` or `Ctrl+D`), commit the Vanilla result if desired (outside or at end of recording), and exit the recorded shell (`exit` or `Ctrl+D`) so `asciinema` saves `vanilla-vibe.cast`.

### Run 1 Recording Checklist

- [x] `~/Documents/GitHub/nuttxspace/recordings/raw/vanilla-vibe.cast` exists and is non-empty. *(Verified: `3,400,774` bytes / `3.2M`, `asciicast v3` at `120x36`, `21,303` events, `46m 40.0s` raw / `33m 04.3s` at `idle_time_limit=2.0s`.)*
- [x] A breadboard wiring Markdown file (with parts list and wiring description) and matching SVG diagram were generated after code generation. *(Verified: `BREADBOARD_WIRING.md` and `breadboard_circuit.svg` generated in `xiao-rp2350-bargraph/` after Step 211 and validated with `xmllint --noout` at Step 221.)*
- [x] Build/test outcome (`PASS` or exact blocker) and `git status` of `nuttx/` + `apps/` were captured on camera. *(Verified on camera at cast events `#21267`–`#21295`: `make -C xiao-rp2350-bargraph test` passed, `make -C xiao-rp2350-bargraph` built `nuttx/nuttx` `2.7M` and `nuttx/nuttx.uf2` `230K`, `git status -s` showed `?? xiao-rp2350-bargraph/`, and both `nuttx/` and `apps/` remained clean.)*

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
   - Copy `~/Documents/GitHub/nuttxspace-vanilla/xiao-rp2350-bargraph/` into `docs/vanilla/xiao-rp2350-bargraph/`, excluding build outputs (`*.o`, `Make.dep`, `.depend`, `.built`, `.kconfig`, compiled host test binaries such as `tests/test_bargraph*`, and dependency files without excluding the `board/src/etc/init.d/` directory).
   - Capture any generated breadboard wiring `.md` and `.svg` files created outside `nuttx/` and `apps/` (whether inside `xiao-rp2350-bargraph/`, `docs/`, or the workspace root) into `docs/vanilla/`.
   - Check for any other planning, design, or documentation Markdown files created in `~/Documents/GitHub/nuttxspace-vanilla/` (outside `nuttx/` and `apps/`), as well as any Markdown artifacts (`implementation_plan.md`, `task.md`, `walkthrough.md`) inside the canonical session's `~/.gemini/**/brain/<session-id>/` directory. Copy any that exist into `docs/vanilla/` (or `docs/vanilla/brain/`), and explicitly report if Run 1 generated zero planning files.
6. Print a Vanilla telemetry summary: session count (parent vs. subagent), total steps, `read_file` calls, `README.md` reads, `run_command` build/test attempts, and count of planning/design files generated (`0` expected for Vanilla).
```

### 5.3 Prompt: Sanitize, Trim, Mark Stop Points & Build Vanilla HTML Player (`/asciinema` `/cast-player-html`)

> [!WARNING]
> **`trim-cast.py` pitfall on `recordings/raw/vanilla-vibe.cast`**: `trim-cast.py --info` flags `press ctrl+d again to exit` at event `#21252` (`raw=2763.25s`), where the operator exited `agy` **before** running the on-camera final shell verification commands (`make test`, `make`, `ls -lh nuttx/nuttx nuttx/nuttx.uf2`, `git status -s`) at events `#21267`–`#21295` (`raw=2776.26s`–`2780.28s`). Do **not** auto-truncate at `#21252`; preserve events `#21267`–`#21295` and trim only the trailing `19.55s` idle wait at event `#21301` (`raw=2800.01s`) when producing `recordings/final/vanilla-vibe.cast`.

In the same `agy` session inside `~/Documents/GitHub/nuttxspace` (or after `/clear`), invoke `/asciinema` and `/cast-player-html`:

```text
/asciinema /cast-player-html Now clean up `recordings/raw/vanilla-vibe.cast` and build the standalone Vanilla HTML player using the exported traces in `docs/prompts-vanilla/` for context:

1. Sanitize (`sanitize-cast.py`) and trim (`trim-cast.py`) `recordings/raw/vanilla-vibe.cast` into `recordings/final/vanilla-vibe.cast`.
2. Inspect stdout events (`mark-cast.py --search` / `--inspect`) alongside `docs/prompts-vanilla/` to place numbered `"m"` stop points at key milestones (command entry, interactive questions/prompts using the two-stop pattern, post-generation wiring SVG/Markdown prompt, and final `make test` / `make` `.uf2` build verification), then run `--tighten-pastes`.
3. Scaffold `prototypes/vanilla-vibe-cast-player.html` from `.agents/skills/cast-player-html/resources/cast-player-template.html`, update its title/header, embed `EMBEDDED_CAST_GZ_B64` via `mark-cast.py --embed-html`, and verify valid JS syntax (`node --check`) and marker timing (`mark-cast.py --list`).
```

### Run 1 Post-Processing Checklist

- [x] `recordings/raw/vanilla-vibe.cast` is matched to its exact AGY parent session(s) by start timestamp and duration, and any aborted/superseded takes have been confirmed and trashed cleanly. *(Matched to single session `60c052d5-d0e1-4595-8065-6dba6563fc22`; `0` subagents; `0` aborted/superseded takes to trash.)*
- [x] `docs/prompts-vanilla/README.md` exists with complete exported traces. *(Exported `docs/prompts-vanilla/README.md` and `docs/prompts-vanilla/01-60c052d5-i-want-to-build-an-out-of.md`.)*
- [x] `docs/vanilla/xiao-rp2350-bargraph/` contains a clean snapshot of the Vanilla source tree (with build binaries/objects stripped), the generated breadboard wiring Markdown and SVG files are captured in `docs/vanilla/`, and any optional workspace/brain Markdown files have been captured in `docs/vanilla/` (or confirmed absent). *(Captured `36` clean files in `docs/vanilla/xiao-rp2350-bargraph/`, including `board/src/etc/init.d/rc.sysinit` and `rcS`, plus `docs/vanilla/BREADBOARD_WIRING.md`, `docs/vanilla/breadboard_circuit.svg`, and `docs/vanilla/CODE_REVIEW.md`; confirmed `0` planning/design/brain Markdown files.)*
- [x] `recordings/final/vanilla-vibe.cast` passes `trim-cast.py --info` with zero trailing exit prompts, and `mark-cast.py --list` shows clean, sequentially numbered stop points aligned to command lines and two-stop interactive prompts. *(Sanitized `3` `OSC 3008/7` sequences, stripped mid-stream `status` check turn `#17274:#17757` with `pyte` viewport verification, trimmed at stop 10 [`Breadboard Wiring Guide & SVG Complete`] via `--cut-index 20781 --hold 3.0`, tightened paste/prompt/typing delays [including `#0332`–`#0380`], dropped stops 1/6/7, and relabeled `7` sequential `"m"` stop points across `31m 56.6s` raw / `31m 55.6s` 1x [`31m 53.6s` / `1913.55s` at final stop point before `3.0s` hold].)*
- [x] `prototypes/vanilla-vibe-cast-player.html` passes `node --check` and opens cleanly via `file://` in a browser. *(Embedded `312,800` chars into `EMBEDDED_CAST_GZ_B64` and verified inline script with `node --check`; ready for `file://` browser playback.)*

#### Run 1 Trace & Recording Cross-Check Summary (2026-11-15)

- **AGY Session**: `60c052d5-d0e1-4595-8065-6dba6563fc22` (`antigravity-cli`, `Gemini 4 Argon (Medium)`), active `17:27:24`–`18:12:57+11:00` (`0:45:33`), `225` total steps, `4` user prompts (Steps `0`, `4`, `206`, `211`), `0` subagents.
- **Tool Telemetry (`135` tool calls)**: `read_file`: `66` (`0` `nuttx/` or `apps/` `README.md` reads; `1` self-authored `xiao-rp2350-bargraph/README.md` read at Step `212`; `6` `.rst` docs under `nuttx/Documentation/`), `run_command`: `35`, `write_file`: `27`, `edit_file`: `6`, `manage_task`: `1`.
- **Cast ↔ Trace Correlation (`recordings/raw/vanilla-vibe.cast`, header `timestamp=1794724031` / `17:27:11+11:00`)**:

| Cast Event(s) | Cast Time (`raw` / `1x`) | Local Time | Trace Step(s) | Milestone |
| :--- | :--- | :--- | :--- | :--- |
| `#0008`–`#0020` | `1.87s`–`4.95s` / `1.87s`–`4.95s` | `17:27:13`–`17:27:16` | *(pre-session)* | `git status -sb` (`## main`) and `agy` (`Antigravity CLI 1.5.9`) launch |
| `#0021`–`#0026` | `11.55s`–`13.00s` / `6.95s`–`8.40s` | `17:27:24` | `Step 0` | Opening prompt pasted and submitted |
| `#0326`–`#0378` | `40.97s`–`90.72s` / `36.37s`–`54.74s` | `17:27:52`–`17:28:42` | `Steps 3–4` | Clarifying Q&A (`"any free breadboard pins you pick are fine..."`) |
| `#17265`–`#17507` | `2375.49s`–`2399.39s` / `1607.32s`–`1631.22s` | `18:06:46`–`18:07:10` | `Steps 206–207` | `734.19s` idle pause post-`Step 205`, followed by `status` check (`manage_task` + `ls -lh`) |
| `#17750`–`#17756` | `2428.92s`–`2429.49s` / `1645.00s`–`1645.57s` | `18:07:40`–`18:07:41` | `Step 211` | Follow-up prompt for breadboard wiring Markdown and SVG diagram |
| `#21252`–`#21259` | `2763.25s`–`2763.84s` / `1974.92s`–`1975.51s` | `18:13:14` | *(exit `agy`)* | `Ctrl+D` exits `agy`, printing `Resume with -c (or command below): agy --conversation=60c052d5-d0e1-4595-8065-6dba6563fc22` |
| `#21267`–`#21295` | `2776.26s`–`2780.28s` / `1978.08s`–`1982.11s` | `18:13:27`–`18:13:31` | *(post-`agy` shell)* | Final on-camera `make test`, `make` (`nuttx/nuttx.uf2` `230K`), and `git status -s` |

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

### 6.2 Pre-Step: Remove the EGM Plugin Before Recording

Because Recording `00-plugin.cast` demonstrates installing the **Elephant-Goldfish Model (`egm`)** plugin (`/egm-bootstrap`, `/egm-drift-report`, `/egm-brainstorm`, `/egm-draft`, `/egm-review`, `/egm-resolve`, `/egm-review-loop`) from AGY's `/plugin` menu on camera, remove any existing global `egm` plugin installation before starting Run 2:

1. **Option A — Remove via AGY's `/plugin` menu (recommended before starting the camera):**
   - Launch `agy`, type `/plugin`, select **Elephant-Goldfish Model (`egm`)**, uninstall/remove it, and exit with `/exit`.
2. **Option B — Remove the plugin directory from the shell:**
   ```bash
   rm -rf ~/.gemini/config/plugins/egm
   ls -la ~/.gemini/config/plugins
   ```

Also verify that `/teach-code` is **not** already pre-installed in `~/Documents/GitHub/nuttxspace-egm/.agents/skills/teach-code`, `~/.agents/skills/teach-code`, or `~/.gemini/config/skills/teach-code` so `npx skills add domesticmouse/skills` installs it cleanly on camera in `06-teach-code.cast`:

```bash
ls -d ~/Documents/GitHub/nuttxspace-egm/.agents/skills/teach-code \
      ~/.agents/skills/teach-code \
      ~/.gemini/config/skills/teach-code 2>/dev/null || echo "teach-code absent (ready)"
```

### Run 2 Provisioning Verification Checklist

- [ ] `~/Documents/GitHub/nuttxspace-egm` has clean `git status`, `nuttx` at `egm` (`b047ea1315`), `apps` at `egm` (`f711fe028`), and `3,242` `README.md` files total.
- [ ] `nuttxspace-egm` does not contain `xiao-rp2350-bargraph/` or `docs/` yet.
- [ ] The Elephant-Goldfish Model (`egm`) plugin is removed prior to recording (`~/.gemini/config/plugins/egm` absent / not listed as installed in `/plugin`) so `00-plugin.cast` can demonstrate adding `/egm-review` and friends from scratch.
- [ ] `/teach-code` is not pre-installed in `nuttxspace-egm` so `06-teach-code.cast` can demonstrate installing it via `npx skills add domesticmouse/skills`.
- [ ] Note down any existing `nuttxspace-egm` sessions in `~/.gemini` (none should exist before recording).

---

## 7. Run 2 Recording: Full EGM Flow (`nuttxspace-egm`)

Record **seven separate `.cast` files** (`00-plugin.cast` through `06-teach-code.cast`) and keep them **separate** so they can be played individually in a presentation, interspersed with slides covering what each step achieved and reviewing the generated docs and code. Use an **incremental Record -> Capture -> Review loop**—do **not** record all seven phases back-to-back before inspecting outputs.

After each phase finishes and is committed in `~/Documents/GitHub/nuttxspace-egm`, switch to `~/Documents/GitHub/nuttxspace` and run [Section 8.2](#82-per-phase-capture-trace-export-standalone-player--review-prompt-run-after-each-phase-0006) **before starting the next recording** so you produce and inspect:
1. **The recorded `.cast` and standalone HTML player**: Sanitize, trim, and mark `recordings/raw/0X-<phase>.cast` -> `recordings/final/0X-<phase>.cast` and build `prototypes/0X-<phase>-cast-player.html` (verified with `pyte` `--render` / `node --check` for TUI redraw quirks or stray turns).
2. **The AGY trace(s)**: Export that phase's parent + Goldfish subagent traces into `docs/prompts-egm/` and audit tool calls / `README.md` contract reads.
3. **The generated docs, code, and presentation review**: Copy that phase's artifacts into `docs/egm/` and write `docs/egm/reviews/0X-<phase>-review.md` summarizing what the step achieved and reviewing its generated docs/code.

> [!TIP]
> Before each phase recording, check `git status` is clean, launch `asciinema rec -i 2.0 --window-size 120x36 --overwrite ...`, start `agy`, and end cleanly with `/exit` + shell `exit`.

---

### 7.1 Recording 0 (`00-plugin.cast`) — Phase 0: Adding `/egm-review` & Friends via AGY's `/plugin` Menu

```bash
cd ~/Documents/GitHub/nuttxspace-egm
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/00-plugin.cast
```

Inside the recorded shell, verify the clean starting state and launch `agy`:

```bash
git status -sb
agy
```

Inside `agy`:
1. Run `/plugin` to open AGY's interactive plugin manager menu.
2. Browse/select **Elephant-Goldfish Model** (`egm` / `antigravity-plugins-official/plugin/egm`) and install/enable it on camera.
3. Verify `/egm-review` and friends (`/egm-bootstrap`, `/egm-drift-report`, `/egm-brainstorm`, `/egm-draft`, `/egm-review`, `/egm-resolve`, `/egm-review-loop`) appear in slash-command completion by typing `/egm`.
4. Exit `agy` (`/exit` or `Ctrl+D`) and exit the recorded shell (`exit` or `Ctrl+D`) to finish `00-plugin.cast`.

**Post-Phase 0 Capture & Review Gate (run [Section 8.2](#82-per-phase-capture-trace-export-standalone-player--review-prompt-run-after-each-phase-0006) before Phase 1):**
- **Cast & Player**: Sanitize, trim, and mark `recordings/raw/00-plugin.cast` -> `recordings/final/00-plugin.cast` and build `prototypes/00-plugin-cast-player.html`; verify `/plugin` installation of `egm` and `/egm-*` slash-command autocomplete render cleanly on camera.
- **AGY Trace & State**: Check `~/.gemini/**/brain/` for any session entry created for `nuttxspace-egm` during `/plugin` (expect TUI-only setup with zero model turns) and verify `~/.gemini/config/plugins/egm` is installed and active.
- **Docs, Code & Slide Review**: Confirm `nuttxspace-egm` working tree remains clean (`git status -sb`) and capture the step summary in `docs/egm/reviews/00-plugin-review.md`.

---

### 7.2 Recording 1 (`01-bootstrap.cast`) — Phase 1: Workspace Bootstrap & Drift Audit

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

**Post-Phase 1 Capture & Review Gate (run [Section 8.2](#82-per-phase-capture-trace-export-standalone-player--review-prompt-run-after-each-phase-0006) before Phase 2):**
- **Cast & Player**: Sanitize, trim, and mark `recordings/raw/01-bootstrap.cast` -> `recordings/final/01-bootstrap.cast` and build `prototypes/01-bootstrap-cast-player.html`.
- **AGY Trace**: Export Phase 1 trace(s) to `docs/prompts-egm/` and inspect tool calls and drift-check behavior.
- **Docs, Code & Slide Review**: Capture `README.md` -> `docs/egm/README.md` and `docs/reports/*DRIFT_REPORT.md` -> `docs/egm/reports/`; review root `README.md` submodule boundaries and any flagged drift findings in `docs/egm/reviews/01-bootstrap-review.md`.

---

### 7.3 Recording 2 (`02-brainstorm.cast`) — Phase 2: Plain-English Discovery (`/egm-brainstorm`)

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

**Post-Phase 2 Capture & Review Gate (run [Section 8.2](#82-per-phase-capture-trace-export-standalone-player--review-prompt-run-after-each-phase-0006) before Phase 3):**
- **Cast & Player**: Sanitize, trim, and mark `recordings/raw/02-brainstorm.cast` -> `recordings/final/02-brainstorm.cast` (verifying two-stop interactive Q&A timing) and build `prototypes/02-brainstorm-cast-player.html`.
- **AGY Trace**: Export Phase 2 parent + 7 Goldfish subagent traces to `docs/prompts-egm/` and check how many `nuttx/` and `apps/` `README.md` contracts the Goldfishes read.
- **Docs, Code & Slide Review**: Capture `concepts_brief.md` -> `docs/egm/designs/concepts_brief.md`; review surfaced hardware/RTOS constraints (`CON-*`, pin assignments, daemon lifecycle) in `docs/egm/reviews/02-brainstorm-review.md` before drafting.

---

### 7.4 Recording 3 (`03-draft.cast`) — Phase 3: Grounded Technical Spec (`/egm-draft`)

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

**Post-Phase 3 Capture & Review Gate (run [Section 8.2](#82-per-phase-capture-trace-export-standalone-player--review-prompt-run-after-each-phase-0006) before Phase 4):**
- **Cast & Player**: Sanitize, trim, and mark `recordings/raw/03-draft.cast` -> `recordings/final/03-draft.cast` and build `prototypes/03-draft-cast-player.html`.
- **AGY Trace**: Export Phase 3 trace(s) to `docs/prompts-egm/` and inspect which upstream files and `README.md` contracts grounded the draft.
- **Docs, Code & Slide Review**: Capture `design_draft.md`, `xiao-rp2350-bargraph.v0-draft.md`, and `xiao-rp2350-bargraph.md` -> `docs/egm/designs/`; review the initial 4-section specification in `docs/egm/reviews/03-draft-review.md` before running adversarial review passes.

---

### 7.5 Recording 4 (`04-review.cast`) — Phase 4: Two-Pass Consolidated Review & Resolution

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

**Post-Phase 4 Capture & Review Gate (run [Section 8.2](#82-per-phase-capture-trace-export-standalone-player--review-prompt-run-after-each-phase-0006) before Phase 5):**
- **Cast & Player**: Sanitize, trim, and mark `recordings/raw/04-review.cast` -> `recordings/final/04-review.cast` (verifying `/clear` boundary and resolution prompts) and build `prototypes/04-review-cast-player.html`.
- **AGY Trace**: Export Phase 4 parent + Pass 1 / Pass 2 Goldfish reviewer traces to `docs/prompts-egm/` and audit `README.md` contract reads and per-persona findings.
- **Docs, Code & Slide Review**: Capture `docs/reports/pass1/*`, `docs/reports/pass2/*`, and the updated `docs/designs/xiao-rp2350-bargraph.md` -> `docs/egm/`; review all accepted/rejected dispositions and confirm critical RTOS/hardware traps (`CON-1`..`CON-10`) are fixed in `docs/egm/reviews/04-review-review.md` before writing C code.

---

### 7.6 Recording 5 (`05-implement.cast`) — Phase 5: Firmware Implementation & Verification

```bash
cd ~/Documents/GitHub/nuttxspace-egm
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/05-implement.cast
```

Launch `agy` and send these two prompts sequentially:

1. **Prompt 1 (Firmware Implementation & Build Verification):**
   ```text
   Implement @[docs/designs/xiao-rp2350-bargraph.md] in `xiao-rp2350-bargraph/`, keeping `nuttx/` and `apps/` submodules completely unmodified. After creating the files, run `make -C xiao-rp2350-bargraph test` and `make -C xiao-rp2350-bargraph` and verify `nuttx/nuttx.uf2` builds cleanly and `git -C nuttx status -s` / `git -C apps status -s` show zero tracked modifications.
   ```

2. **Prompt 2 (Breadboard Layout SVG & Circuit Wiring Guide — Post Code Generation):**
   Once firmware implementation and build/test verification pass, paste this follow-up prompt verbatim before exiting `agy`:
   ```text
   Please generate a breadboard wiring description for the circuit matching the firmware.
   Provide both a Markdown file with a parts list and wiring description, and a matching SVG diagramming the breadboard circuit.
   ```

Once `make -C xiao-rp2350-bargraph test`, `make -C xiao-rp2350-bargraph`, and the generated breadboard wiring Markdown + SVG files are verified on camera, exit `agy`, commit the generated firmware and wiring docs, and exit the shell:

```bash
git status -s
git add xiao-rp2350-bargraph
find . -maxdepth 3 \( -name "*.svg" -o -name "*.md" \) -not -path "./nuttx/*" -not -path "./apps/*" -exec git add {} +
git commit -m "Phase 5: implement xiao-rp2350-bargraph firmware, host tests, and wiring docs"
```

**Post-Phase 5 Capture & Review Gate (run [Section 8.2](#82-per-phase-capture-trace-export-standalone-player--review-prompt-run-after-each-phase-0006) before Phase 6):**
- **Cast & Player**: Sanitize, trim, and mark `recordings/raw/05-implement.cast` -> `recordings/final/05-implement.cast` (verifying `make test`, `make`, and wiring SVG/Markdown generation on camera) and build `prototypes/05-implement-cast-player.html`.
- **AGY Trace**: Export Phase 5 trace(s) to `docs/prompts-egm/` and inspect build retries, test runs, and implementation steps.
- **Docs, Code & Slide Review**: Capture clean `xiao-rp2350-bargraph/` source snapshot and generated breadboard wiring `.md`/`.svg` -> `docs/egm/`; review the generated C code, Kconfig, Makefile, host unit tests, and wiring diagram against the hardened spec in `docs/egm/reviews/05-implement-review.md` before running `/teach-code`.

---

### 7.7 Recording 6 (`06-teach-code.cast`) — Phase 6: Navigating Comprehension Debt with `/teach-code`

After generating the board bringup, driver glue, and application code, install `/teach-code` from `domesticmouse/skills` and use it to navigate the comprehension debt of AI-generated firmware by producing a step-by-step tutorial on how to construct both the physical breadboard circuit and the NuttX custom board + application code by hand.

```bash
cd ~/Documents/GitHub/nuttxspace-egm
asciinema rec -i 2.0 --window-size 120x36 --overwrite \
  ~/Documents/GitHub/nuttxspace/recordings/raw/06-teach-code.cast
```

1. **Step 1 — Install `/teach-code` in the recorded shell:**
   ```bash
   npx skills add domesticmouse/skills
   ```
   *(If `npx skills` prompts interactively for skills, target agent, or scope, select `teach-code` for Antigravity / workspace or global installation.)*

2. **Step 2 — Launch `agy` and invoke `/teach-code`:**
   ```bash
   agy
   ```
   Paste this prompt verbatim:
   ```text
   /teach-code Build a hands-on tutorial in `docs/tutorials/xiao-rp2350-bargraph.md` that teaches a software engineer how to build both the breadboard circuit and the `xiao-rp2350-bargraph/` custom board and application code by hand from scratch. Walk through the hardware wiring, Kconfig/Makefiles, board bringup, and C daemon incrementally so the reader can navigate the comprehension debt of the generated code.
   ```
   *(Answer any interactive `/teach-code` scoping questions if asked—confirming a software-engineer audience, covering both the breadboard wiring and building the code by hand, and writing the tutorial to `docs/tutorials/xiao-rp2350-bargraph.md`.)*

3. **Step 3 — On-camera wrap-up & commit in `nuttxspace-egm`:**
   Exit `agy` (`/exit` or `Ctrl+D`), inspect the generated tutorial, commit, and exit the recorded shell (`exit` or `Ctrl+D`) to save `06-teach-code.cast`:
   ```bash
   ls -lh docs/tutorials/
   git status -s
   git add docs/tutorials .agents 2>/dev/null || git add docs/tutorials
   git commit -m "Phase 6: install /teach-code and generate hand-built board and code tutorial"
   ```

**Post-Phase 6 Capture & Review Gate (run [Section 8.2](#82-per-phase-capture-trace-export-standalone-player--review-prompt-run-after-each-phase-0006) before final Run 2 reconciliation):**
- **Cast & Player**: Sanitize, trim, and mark `recordings/raw/06-teach-code.cast` -> `recordings/final/06-teach-code.cast` (verifying `npx skills add domesticmouse/skills` and `/teach-code` output) and build `prototypes/06-teach-code-cast-player.html`.
- **AGY Trace**: Export Phase 6 `/teach-code` trace(s) to `docs/prompts-egm/` and inspect which generated board/code/wiring files it read.
- **Docs, Code & Slide Review**: Capture `docs/tutorials/*` -> `docs/egm/tutorials/`; review the generated hand-built board + code tutorial for technical accuracy, safety rules (`3V3`, series resistors, pin choices), and completeness in `docs/egm/reviews/06-teach-code-review.md` before final Run 2 reconciliation.

### Run 2 Recording & Per-Phase Review Checklist

- [ ] All 7 raw `.cast` files (`00-plugin.cast` through `06-teach-code.cast`) exist in `~/Documents/GitHub/nuttxspace/recordings/raw/` and were kept **separate**, with each phase producing `recordings/final/0X-*.cast`, `prototypes/0X-*-cast-player.html`, and `docs/egm/reviews/0X-*-review.md` immediately after recording.
- [ ] `00-plugin.cast` captures installing the Elephant-Goldfish Model (`egm`) plugin (`/egm-review` and friends) via AGY's `/plugin` menu.
- [ ] `make -C xiao-rp2350-bargraph test` and `make -C xiao-rp2350-bargraph` both succeeded in `nuttxspace-egm`, producing `nuttx/nuttx.uf2`.
- [ ] A breadboard wiring Markdown file (with parts list and wiring description) and matching SVG diagram were generated in Phase 5 after code generation.
- [ ] `06-teach-code.cast` captures installing `/teach-code` via `npx skills add domesticmouse/skills` and generating the build-by-hand board + code tutorial in `nuttxspace-egm` (`docs/tutorials/`), captured into `docs/egm/tutorials/`.
- [ ] `git -C nuttx status -s` and `git -C apps status -s` show zero modified tracked files.

---

## 8. Run 2 Per-Phase Capture, Review & Standalone Player Workflow

Instead of waiting until all seven recordings are finished—or stitching them into one file—keep all seven recordings (`00-plugin.cast` through `06-teach-code.cast`) **separate** so each recording can be played individually in a presentation alongside slides on what that step achieved and reviews of the generated docs and code. Run [Section 8.2](#82-per-phase-capture-trace-export-standalone-player--review-prompt-run-after-each-phase-0006) in **`~/Documents/GitHub/nuttxspace`** immediately after **each** phase recording finishes, then run [Section 8.3](#83-full-run-batch-reconciliation--tutorial-capture-check) once all seven phases are done.

### 8.1 Reference Inventory: Useful Files Generated & Reviewed Across Phases 0–6

Unlike Run 1 (which typically produces only C/Makefile/Kconfig implementation files), Run 2 generates a structured chain of architectural contracts, design briefs, multi-persona critique logs, resolution ledgers, a `/teach-code` tutorial, and per-phase presentation reviews:

| Phase / Skill | Source File(s) in `nuttxspace-egm` | Archive Target in `nuttxspace/` | What to Review After This Phase |
| :--- | :--- | :--- | :--- |
| **Phase 0** (`/plugin` menu) | `00-plugin.cast` (`~/.gemini/config/plugins/egm/`) | `recordings/final/00-plugin.cast`, `prototypes/00-plugin-cast-player.html`, `docs/egm/reviews/00-plugin-review.md` | Confirm `00-plugin.cast` cleanly shows installing `egm` via `/plugin` and `/egm-*` autocomplete; verify `~/.gemini/config/plugins/egm` is active. |
| **Phase 1** (`/egm-bootstrap` + `/egm-drift-report`) | `README.md`, `*DRIFT_REPORT.md` / `docs/reports/*DRIFT_REPORT.md` | `docs/egm/README.md`, `docs/egm/reports/*DRIFT_REPORT.md` | Inspect root `README.md` submodule boundaries, drift audit findings, and Phase 1 AGY trace before brainstorming. |
| **Phase 2** (`/egm-brainstorm`) | `concepts_brief.md` (or `docs/designs/concepts_brief.md`) | `docs/egm/designs/concepts_brief.md` | Inspect 7 Goldfish subagent traces (`README.md` contract read count) and review surfaced `CON-*` hardware/RTOS constraints. |
| **Phase 3** (`/egm-draft`) | `design_draft.md`, `docs/designs/xiao-rp2350-bargraph.v0-draft.md`, `docs/designs/xiao-rp2350-bargraph.md` | `docs/egm/designs/` | Review the initial 4-section spec and verify upstream NuttX grounding in the Phase 3 trace before adversarial review. |
| **Phase 4** (`/egm-review` + `/egm-resolve`, Pass 1 & Pass 2) | `docs/reports/pass1/*_report.md`, `docs/reports/pass2/*_report.md`, `goldfish_review_report.md`, `docs/designs/xiao-rp2350-bargraph.md` | `docs/egm/reports/pass1/`, `docs/egm/reports/pass2/`, `docs/egm/designs/xiao-rp2350-bargraph.md` | Audit Pass 1/Pass 2 Goldfish traces (`README.md` reads) and verify all critical traps (`CON-1`..`CON-10`) are resolved in the spec before C implementation. |
| **Phase 5** (Implementation) | `xiao-rp2350-bargraph/` + generated wiring `.md`/`.svg` (in `xiao-rp2350-bargraph/`, `docs/`, or root) | `docs/egm/xiao-rp2350-bargraph/` (and `docs/egm/` for root/docs wiring files) | Review generated C code, Kconfig, Makefile, host tests, build/test trace steps, and breadboard SVG/Markdown wiring accuracy. |
| **Phase 6** (`/teach-code`) | `docs/tutorials/*.md` (e.g., `docs/tutorials/xiao-rp2350-bargraph.md`) | `docs/egm/tutorials/` | Review `/teach-code` trace and verify the build-by-hand board + firmware tutorial accurately explains the generated code and hardware safety rules. |
| **All Phases (`00`–`06`)** (Presentation Reviews & Players) | `recordings/raw/0X-*.cast` + exported traces + generated artifacts | `recordings/final/0X-*.cast`, `prototypes/0X-*-cast-player.html`, `docs/egm/reviews/0X-*-review.md` | Separate per-step recordings, standalone HTML players, and slide-ready achievement + doc/code reviews for every step. |
| **Optional / Brain Artifacts** | `TODO.md`, `~/.gemini/**/brain/<session-id>/*.md` (`implementation_plan.md`, `task.md`, `walkthrough.md`) | `docs/egm/designs/TODO.md`, `docs/egm/brain/` | Inspect any supplemental checklists or Antigravity session brain artifacts produced during the phase. |

### 8.2 Per-Phase Capture, Trace Export, Standalone Player & Review Prompt (Run After Each Phase 00–06)

After finishing and committing a phase recording (`00-plugin.cast` … `06-teach-code.cast`) in `~/Documents/GitHub/nuttxspace-egm`, launch `agy` inside `~/Documents/GitHub/nuttxspace`:

```bash
cd ~/Documents/GitHub/nuttxspace
agy
```

Paste this prompt (replacing `<0X-phase>` and `<Phase N>` with the phase you just recorded—for example `00-plugin` / `Phase 0`; approve `BypassSandbox` when prompted so `/agy-brain-explorer` can read `~/.gemini/**/brain/`):

```text
/agy-brain-explorer /asciinema /cast-player-html I just finished capturing Run 2 `<0X-phase>.cast` (`<Phase N>`) in `~/Documents/GitHub/nuttxspace-egm`. Keep this recording separate for presentation playback and complete the per-phase capture, player build, and review before moving on to the next phase:

1. **Correlate & Export AGY Traces**:
   - Discover sessions under `~/.gemini/**/brain/` for project `nuttxspace-egm` matching the timestamp/duration (`trim-cast.py --info`) of `recordings/raw/<0X-phase>.cast` (including any Goldfish subagents spawned in this phase; note `00-plugin.cast` may be a TUI-only `/plugin` interaction with zero model turns).
   - Flag any aborted or superseded takes from this phase and ask confirmation before moving them to Trash.
   - Export the matched parent + subagent traces incrementally into `docs/prompts-egm/` and refresh `docs/prompts-egm/README.md`.
2. **Capture This Phase's Generated Docs & Code into `docs/egm/`**:
   - Copy any new or updated artifacts produced in `<Phase N>` from `~/Documents/GitHub/nuttxspace-egm` (and `~/.gemini/**/brain/` session `.md` artifacts) into `docs/egm/` per the Section 8.1 mapping (`README.md`, `designs/`, `reports/pass1/`, `reports/pass2/`, clean `xiao-rp2350-bargraph/` without build objects/binaries while preserving `board/src/etc/init.d/`, breadboard wiring `.md`/`.svg`, and `docs/tutorials/` -> `docs/egm/tutorials/`).
3. **Clean, Mark & Build Standalone HTML Player for `<0X-phase>`**:
   - Sanitize (`sanitize-cast.py`), trim (`trim-cast.py`), tighten pastes (`--tighten-pastes`), and place numbered `"m"` stop points (`mark-cast.py`) on `recordings/raw/<0X-phase>.cast` into `recordings/final/<0X-phase>.cast` (using `uv run --with pyte` `--render` if any mid-stream strip/verification is needed).
   - Scaffold `prototypes/<0X-phase>-cast-player.html` from `.agents/skills/cast-player-html/resources/cast-player-template.html`, update its title/header, embed `EMBEDDED_CAST_GZ_B64` via `mark-cast.py --embed-html`, and verify `trim-cast.py --info`, `mark-cast.py --list`, and `node --check`.
4. **Write Presentation Phase Review (`docs/egm/reviews/<0X-phase>-review.md`) & Print Summary**:
   - Write `docs/egm/reviews/<0X-phase>-review.md` structured so I can build presentation slides directly between recordings:
     - **What This Step Achieved**: Concise summary of the phase goal, user prompts/gates, and concrete outputs.
     - **Recorded Cast & Player**: `recordings/final/<0X-phase>.cast` and `prototypes/<0X-phase>-cast-player.html`, raw vs. 1x duration, stop-point list, and any visual/TUI redraw notes.
     - **AGY Trace Telemetry**: Parent + subagent session count, total steps, `read_file` calls, `nuttx/`/`apps/` `README.md` contract reads, and command outcomes.
     - **Review of Generated Docs & Code**: Technical review of the docs, specs, review reports, C code, wiring SVG/Markdown, or `/teach-code` tutorial generated in this step (strengths, defects/gaps spotted, and whether we are ready for the next phase or should retake).
```

### 8.3 Full-Run Batch Reconciliation & Tutorial Capture Check

Once all seven phases (`00-plugin.cast` through `06-teach-code.cast`) have passed their per-phase Section 8.2 reviews, verify that all separate final casts, standalone HTML players, presentation reviews, and the generated `/teach-code` tutorial are cleanly captured in `~/Documents/GitHub/nuttxspace`:

```bash
mkdir -p ~/Documents/GitHub/nuttxspace/docs/egm/tutorials
cp -R ~/Documents/GitHub/nuttxspace-egm/docs/tutorials/* \
  ~/Documents/GitHub/nuttxspace/docs/egm/tutorials/
ls -lh ~/Documents/GitHub/nuttxspace/docs/egm/tutorials/
ls -lh ~/Documents/GitHub/nuttxspace/recordings/final/0*.cast \
       ~/Documents/GitHub/nuttxspace/prototypes/0*-cast-player.html \
       ~/Documents/GitHub/nuttxspace/docs/egm/reviews/
```

If any cross-phase reconciliation or telemetry rollup across all 7 casts is needed, run one final `/agy-brain-explorer` pass in `~/Documents/GitHub/nuttxspace`:

```text
/agy-brain-explorer Reconcile all seven separate Run 2 recordings (`00-plugin.cast` through `06-teach-code.cast`) against `docs/prompts-egm/`, `docs/egm/`, and `docs/egm/reviews/`, confirm zero unexported sessions or missed artifacts (including `docs/egm/tutorials/`), and print the final Run 2 vs. Run 1 (`docs/prompts-vanilla/` + `docs/vanilla/`) telemetry comparison table.
```

### Run 2 Post-Processing Checklist

- [ ] All 7 raw EGM `.cast` files (`00-plugin.cast` through `06-teach-code.cast`) remain **separate** and were each sanitized into `recordings/final/00-plugin.cast`–`06-teach-code.cast`, matched to their exact AGY parent/subagent session(s) (`00-plugin.cast` accounted for as `/plugin` setup), and reviewed immediately after recording, with any aborted/superseded takes confirmed and trashed cleanly.
- [ ] All 7 final casts pass `trim-cast.py --info` with zero trailing exit prompts, and `mark-cast.py --list` shows clean, sequentially numbered stop points on each recording.
- [ ] All 7 standalone HTML players (`prototypes/00-plugin-cast-player.html` through `prototypes/06-teach-code-cast-player.html`) pass `node --check` and open cleanly via `file://` in a browser.
- [ ] `docs/egm/reviews/00-plugin-review.md` through `docs/egm/reviews/06-teach-code-review.md` exist with step achievements, trace telemetry, and technical reviews of the generated docs/code for presentation slides.
- [ ] `docs/prompts-egm/README.md` exists with complete exported traces (including Goldfish subagents and the `/teach-code` session).
- [ ] `docs/egm/` contains `README.md`, `designs/` (`concepts_brief.md`, `design_draft.md`, `xiao-rp2350-bargraph.v0-draft.md`, `xiao-rp2350-bargraph.md`), `reports/` (`*DRIFT_REPORT.md`, `pass1/`, `pass2/`), `tutorials/` (the captured `/teach-code` tutorial on building both the board and the code by hand), `reviews/`, a clean `xiao-rp2350-bargraph/` source snapshot (with build binaries/objects stripped), and the generated breadboard wiring Markdown and SVG files.

---

## 9. Final Comparative Step: Goldfish Audit & Defect Scorecard

Once both Run 1 and Run 2 traces are exported and HTML players are packaged, run an audit prompt in `~/Documents/GitHub/nuttxspace` (using fresh Goldfish subagents) to grade the Vanilla implementation against the EGM implementation across all 10 hardware/RTOS traps from [`docs/reports/goldfish_review_report.md`](docs/reports/goldfish_review_report.md) and [`docs/designs/concepts_brief.md`](docs/designs/concepts_brief.md).

### 9.1 Prompt: Side-by-Side Goldfish Audit & Telemetry Report

```text
Spawn `egm-codebase-details` and `egm-design` as fresh read-only auditors (with permission to read `~/Documents/GitHub/nuttxspace-vanilla` and `~/Documents/GitHub/nuttxspace-egm`, without modifying either repo) to compare the two generated `xiao-rp2350-bargraph/` implementations alongside:
- Vanilla traces: `docs/prompts-vanilla/`
- Vanilla captured source & artifacts: `docs/vanilla/` (and `~/Documents/GitHub/nuttxspace-vanilla`)
- EGM traces: `docs/prompts-egm/`
- EGM captured designs, per-pass review reports & source: `docs/egm/` (including `docs/egm/reviews/` and `~/Documents/GitHub/nuttxspace-egm`)
- Recording durations & stop points: `recordings/final/vanilla-vibe.cast` and `recordings/final/00-plugin.cast` through `recordings/final/06-teach-code.cast`

Grade both implementations against the 10-point hardware/RTOS scorecard in `EXPERIMENT_RUNBOOK.md` (Section 9.2), verify each generated breadboard wiring Markdown file and SVG diagram matches its own firmware pins and hardware safety rules (`3V3` vs. `5V`, avoiding `GPIO 26–29` for LEDs, series resistors), cite exact file paths and line numbers for every defect or pass, and write the filled comparison report (with the telemetry table from Section 9.3) to `docs/reports/vanilla-vs-egm-comparison.md`.
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
| **Raw Recording Duration** | `46m 40.0s` (`2,800.03s`; `45m 33s` inside `agy`) | |
| **Playback Duration (`idle=2.0s`)** | `33m 04.3s` (`1,984.30s`) | |
| **Parent + Subagent Sessions** | `1` parent (`60c052d5`) + `0` subagents | |
| **Total Trajectory Steps** | `225` steps (`4` user turns, `135` tool calls) | |
| **`read_file` Calls / `README.md` Reads** | `66` `read_file` / `0` `nuttx` or `apps` `README.md` reads (`1` self-authored `README.md`, `6` `.rst` docs) | |
| **Build Failures Before Clean `.uf2`** | `1` silent Kconfig drop (Step 180 missing `CONFIG_ANALOG=y` dropped `CONFIG_ADC`; fixed at Step 196) + `1` SVG `xmllint` error (Step 219) | |
| **Planning & Review Artifacts Generated** | `0` planning/design/brain files | |
| **Scorecard Pass Count (`X / 10`)** | Pending Section 9 audit | |
