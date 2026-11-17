# Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up — Full Talk & Click Points

> **Speaker:** Brett Morgan (`bjm-voice`)  
> **Runtime:** `30:00` (`1,800.0s`) across `5` Acts, `26` Beats, `102` Slidev Slides, `112` Click Reveals (`88` `v-click` + `24` `Code-step`), and `214` Stage Steps (`S001`–`S214`)  
> **Source Spec:** [spec.md](./spec.md) · **Slidev Deck:** [slides.md](../../slides/slides.md)

## Click & Stage Action Legend

- **`[SLIDE N · Sxxx · mm:ss–mm:ss (ds)]`** (`Space` / `→`): Advance to Slide `N` (`1`–`102`). *(Slide `1` is on screen at `00:00`.)*
- **`[CLICK k/M · Sxxx · mm:ss–mm:ss (ds)]`** (`Space` / `→`): Trigger `v-click` reveal `k` of `M` on the current slide.
- **`[CODE-CLICK k/M · Sxxx · mm:ss–mm:ss (ds)]`** (`Space` / `→`): Trigger Shiki code-block reveal `k` of `M` on the current slide.
- **`[LIVE CAST · SLIDE N · Sxxx · mm:ss–mm:ss (ds)]`** (`]` / `p`): Advance to Slide `N`, then trigger live warped `asciinema-player` segment playback using the listed stop-point and hotkey cues.
- **`[FREEZE-FRAME · SLIDE N · Sxxx · mm:ss–mm:ss (ds)]`** (`Space` / `→`): Advance to pre-positioned static cast-player checkpoint slide (`0s` playback).

---

## Act I — The Hook & The Title's Double Meaning (`00:00–03:00`, `3.0 min`, Beats `01–03`, `S001–S023`)

### Beat 01 (`00:00–01:00`, `60.0s`, `8` steps) — Title & The Naked Hardware/RTOS Cliff

**`[SLIDE 1 · S001 · 00:00–00:08 (8.0s)]`** · Start on screen — **Title Card** — *"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"* + Brett Morgan.

> G'day everyone. Tonight is about dropping a coding agent into an unfamiliar legacy codebase and helping you both understand what actually ships.

**`[SLIDE 2 · S002 · 00:08–00:16 (8.0s)]`** · `Space / →` — **Hardware Preview** — Seeed Studio XIAO RP2350 + 6-LED breadboard (`docs/egm/breadboard-wiring.svg`).

> It started with weekend hardware curiosity: a tiny AU$ Seeed Studio XIAO RP2350 board on a solderless breadboard beside my laptop.

**`[CLICK 1/1 · S003 · 00:16–00:23 (7.0s)]`** · `Space / →` — **Hardware Spec Callout** — Dual-core Cortex-M33 silicon (`RP2350`), potentiometer on `D0/A0`, and 6 PWM LEDs.

> Nothing exotic on the bench—just one potentiometer wired into `D0/A0` driving a smooth six-LED bar graph via hardware PWM.

**`[SLIDE 3 · S004 · 00:23–00:30 (7.0s)]`** · `Space / →` — **The RTOS Scale Shock** — **Apache NuttX**: `30` years of legacy C across `nuttx/` and `apps/`.

> The catch is running it inside Apache NuttX, thirty years of deep POSIX RTOS C that I have definitely not memorised.

**`[SLIDE 4 · S005 · 00:30–00:38 (8.0s)]`** · `Space / →` — **Baseline #1 (Big Number)** — Upstream `nuttx` + `apps` `master`: **`13` `README.md` files**.

> Look at upstream `master` across those two submodules: thirteen `README.md` files total. Drop an agent there cold and it flies blind.

**`[CLICK 1/1 · S006 · 00:38–00:45 (7.0s)]`** · `Space / →` — **Baseline #2 (Big Contrast)** — Our `egm` baseline: **`3,241` `README.md` contracts** (`2,607` in `nuttx/`, `634` in `apps/`).

> Now compare that against our contract-mapped branch: three thousand, two hundred and forty-one directory `README.md` contracts waiting to be navigated.

**`[SLIDE 5 · S007 · 00:45–00:53 (8.0s)]`** · `Space / →` — **The Hook Contrast** — *"The joy is new silicon on your desk; the pain is your coding agent waking up blind."*

> The joy is fresh silicon on your desk; the pain is watching your AI accomplice wake up blind inside legacy C.

**`[CLICK 1/1 · S008 · 00:53–01:00 (7.0s)]`** · `Space / →` — **Experiment Teaser** — Same hardware, same prompt, two runs (`nuttxspace-vanilla` vs. `nuttxspace-egm`).

> So I ran a controlled experiment: exact same RP2350 hardware, exact same prompt, recorded end-to-end across two separate workspaces.

### Beat 02 (`01:00–02:00`, `60.0s`, `8` steps) — The Identical Prompt & The Lie of "It Compiled!"

**`[SLIDE 6 · S009 · 01:00–01:08 (8.0s)]`** · `Space / →` — **The Shared Prompt Card** — Functional brief from `EXPERIMENT_RUNBOOK.md` handed verbatim to both workspaces.

> We did not rig the prompt. Both workspaces received the identical functional specification straight out of [`EXPERIMENT_RUNBOOK.md`](../../EXPERIMENT_RUNBOOK.md).

**`[CLICK 1/3 · S010 · 01:08–01:15 (7.0s)]`** · `Space / →` — **Constraint 1 (Submodule Hygiene)** — Out-of-tree `xiao-rp2350-bargraph/` custom board + app (`nuttx/` and `apps/` stay spotless).

> Constraint one: everything lives out-of-tree in `xiao-rp2350-bargraph/`, leaving both upstream `nuttx/` and `apps/` git submodules completely untouched.

**`[CLICK 2/3 · S011 · 01:15–01:23 (8.0s)]`** · `Space / →` — **Constraint 2 (Firmware Behaviour)** — `D0/A0` pot driving 6 LEDs, autonomous power-on boot, USB-C `nsh>` CLI (`set`, `status`, `stop`, `test`).

> Constraint two: power on cleanly without a host computer, track the potentiometer smoothly, and expose an interactive USB serial command line.

**`[CLICK 3/3 · S012 · 01:23–01:31 (8.0s)]`** · `Space / →` — **Constraint 3 (Deliverables)** — Host `make test`, flashable `nuttx/nuttx.uf2`, plus Markdown & SVG breadboard wiring guide.

> Constraint three: ship host unit tests, a flashable `nuttx.uf2` image, plus a Markdown and SVG breadboard wiring diagram.

**`[SLIDE 7 · S013 · 01:31–01:38 (7.0s)]`** · `Space / →` — **Deceptive Green Badge** — **`BUILD SUCCEEDED: nuttx/nuttx.uf2`**.

> Here is the trap: both the unprimed vibe-coding run and the full contract run reached this comforting green build badge.

**`[CLICK 1/1 · S014 · 01:38–01:45 (7.0s)]`** · `Space / →` — **It Blinks!** — Twist the potentiometer; all six breadboard LEDs light up smoothly in sequence.

> You drag the `.uf2` onto the RP2350, twist the pot, and all six LEDs light up across the breadboard.

**`[SLIDE 8 · S015 · 01:45–01:53 (8.0s)]`** · `Space / →` — **Standalone Quote** — *"In embedded systems—and any deep legacy codebase—'it compiles and blinks' is the most dangerous lie an AI accomplice can tell you."*

> In embedded systems—and any deep legacy codebase—"it compiles and blinks" is the most dangerous lie an AI accomplice can tell you.

**`[CLICK 1/1 · S016 · 01:53–02:00 (7.0s)]`** · `Space / →` — **The Hidden Timebombs** — Silicon errata pad leakage (`RP2350-E9`) + a `40 Hz` ISR teardown kernel panic.

> Underneath that happy blinking demo hid a silicon erratum corruption bug and a forty-hertz interrupt teardown panic waiting to fire.

### Beat 03 (`02:00–03:00`, `60.0s`, `7` steps) — What "Roll Your Own Docs" Means Across Three Layers

**`[SLIDE 9 · S017 · 02:00–02:09 (9.0s)]`** · `Space / →` — **Foundational Credit (EGM)** — Dave Rensin (`@drensin`): [*"Elephants, Goldfish, and the New Golden Age of Software Engineering"*](https://drensin.medium.com/elephants-goldfish-and-the-new-golden-age-of-software-engineering-c33641a48874).

> To stop shipping hidden timebombs, I turned to Dave Rensin's Elephant-Goldfish Model from Google's open-source `elephant-goldfish-model` plugin.

**`[CLICK 1/1 · S018 · 02:09–02:17 (8.0s)]`** · `Space / →` — **Rensin's Two Mantras** — *"Design is the new code."* / *"Design with an elephant. Review with a goldfish."*

> Dave captures the mindset in two lines: design is the new code; design with an elephant, review with a goldfish.

**`[SLIDE 10 · S019 · 02:17–02:26 (9.0s)]`** · `Space / →` — **Layer 1 — Docs for the Elephant & Goldfish** — `3,241` bottom-up `README.md` contracts + EGM drift, brainstorm, draft, and review specs.

> That gives us layer one of "Roll Your Own Docs": build contracts and specs so your coding agents stop guessing.

**`[CLICK 1/2 · S020 · 02:26–02:35 (9.0s)]`** · `Space / →` — **Layer 2 — Docs for the Human (Who Owns the Outcome)** — 6-lesson interactive Tufte codelab in `docs/egm/tutorial/index.md` / `docs/egm/tutorial/index.html` via `/teach-code`.

> Layer two rolls docs for the human who owns the outcome: a six-lesson interactive codelab so you actually understand the firmware.

**`[CLICK 2/2 · S021 · 02:35–02:43 (8.0s)]`** · `Space / →` — **Layer 3 — Docs for the Workflow** — Evolving Matt Pocock's `/teach` into `/teach-code` via `/agy-customizations`.

> And layer three rolls docs for your own workflow, evolving Matt Pocock's `/teach` skill into `/teach-code` through real-world use.

**`[SLIDE 11 · S022 · 02:43–02:52 (9.0s)]`** · `Space / →` — **Three-Layer Architecture Summary** — Agent contracts -> Human comprehension -> Reusable skills.

> Together, those three layers create a tight loop: grounded code, genuine human comprehension, and sharper custom skills for the next repo.

**`[CLICK 1/1 · S023 · 02:52–03:00 (8.0s)]`** · `Space / →` — **Act II Pivot** — Next: watch what happens when you skip all three layers and vibe code straight into C.

> Let's roll the tape on what happens when you skip all three layers and vibe code straight into C.

---

## Act II — The Foil: Flying Blind with Vanilla Vibe Coding (`03:00–08:00`, `5.0 min`, Beats `04–07`, `S024–S057`)

### Beat 04 (`03:00–04:15`, `75.0s`, `5` steps) — Experiment A (`nuttxspace-vanilla`): 46 Minutes of Pure Vibe & "Weeds Syndrome"

**`[SLIDE 12 · S024 · 03:00–03:08 (8.0s)]`** · `Space / →` — **Experiment A Baseline** — `nuttxspace-vanilla` on upstream `master` (`13` `README.md` files), `1` parent session (`60c052d5`).

> Experiment A starts on upstream `master` with thirteen `README.md` files, a single chat session, and zero custom workflow skills.

**`[SLIDE 13 · S025 · 03:08–03:16 (8.0s)]`** · `Space / →` — **Rensin's *"Weeds Syndrome"*** — *Premature Implementation*: skipping architectural boundaries to edit nearby C immediately.

> Watch for what Dave Rensin calls "Weeds Syndrome"—premature implementation, diving straight into writing C files before understanding any subsystem invariants.

**`[LIVE CAST · SLIDE 14 · S026 · 03:16–04:00.4 (44.4s)]`** · `Space / →` to slide, then `]` / `p` — **LIVE CAST (`44.4s`)** on [`prototypes/vanilla-vibe-cast-player.html`](../../prototypes/vanilla-vibe-cast-player.html).
- *Cast Cue:* Switch to [`prototypes/vanilla-vibe-cast-player.html`](../../prototypes/vanilla-vibe-cast-player.html). Jump (`]`) to Stop Point `2. Initial Prompt: XIAO RP2350 Bar Graph` (`00:02` warped), press `p` to run **Seg 03** (`14.1s @ 2x` -> `3. Q1: Breadboard Pins & LED Polarity`), **Seg 04** (`0.5s @ 2x` -> `4. A1: Any Free Breadboard Pins`), and **Seg 05** (`29.8s @ 52x`, compressing `25m 51s` raw -> auto-pauses at `5. Firmware & Tests Built`). Stop at Stop Point `5` (do not play SVG segment).

> Here is the raw prompt from [`EXPERIMENT_RUNBOOK.md`](../../EXPERIMENT_RUNBOOK.md) going straight into an unprimed thread. Look at the effect right away: it asks one quick question about pins and LED polarity, I answer "pick any free pins", and that is the entire design conversation over. Now at fifty-two times speed—compressing nearly twenty-six minutes of single-thread grep-and-code—it pulls sixty-six files out of the repo without reading a single `nuttx/` or `apps/` `README.md` contract. At step 180 `olddefconfig` quietly drops `CONFIG_ADC=y`, it patches Kconfig, and twenty-six minutes later it hands us a green `nuttx.uf2` build.

**`[SLIDE 15 · S027 · 04:00.4–04:08 (7.6s)]`** · `Space / →` — **Vanilla Telemetry Snapshot** — `46m 40s` raw, `225` steps, `0` subagents, **`0` `nuttx`/`apps` `README.md` reads**.

> Forty-six minutes forty seconds raw, two hundred and twenty-five steps, zero subagents, and zero architectural contract reads across `nuttx/` or `apps/`.

**`[CLICK 1/1 · S028 · 04:08–04:15 (7.0s)]`** · `Space / →` — **The Vibe-Coding Illusion** — `66` `read_file` greps, `1` question, `1` green `.uf2`—so what broke beneath the surface?

> On paper that speed looks brilliant. Let's open [`docs/vanilla/CODE_REVIEW.md`](../../docs/vanilla/CODE_REVIEW.md) and inspect what sixty-six blind greps missed underneath.

### Beat 05 (`04:15–05:15`, `60.0s`, `8` steps) — Supporting Traps #1 & #2: Silent Kconfig Drops & `my_reset()` + Erratum `RP2350-E9`

**`[SLIDE 16 · S029 · 04:15–04:23 (8.0s)]`** · `Space / →` — **Supporting Trap #1 (Build Config)** — Trace Step 180 ([`docs/vanilla/CODE_REVIEW.md`](../../docs/vanilla/CODE_REVIEW.md)): Vanilla enables `CONFIG_ADC=y` in `defconfig`.

> First warm-up trap hits NuttX's Kconfig tree: Vanilla dropped `CONFIG_ADC=y` into `defconfig` assuming the ADC driver would compile.

**`[CLICK 1/1 · S030 · 04:23–04:30 (7.0s)]`** · `Space / →` — **Silent `olddefconfig` Drop** — Missing `CONFIG_ANALOG=y` silently strips `CONFIG_ADC=y` (while `CONFIG_DEV_GPIO=y` collides on `GPIO 5,6,7,0,1,2`).

> Without `CONFIG_ANALOG=y`, `olddefconfig` quietly stripped `CONFIG_ADC=y` out—and Vanilla still left `CONFIG_DEV_GPIO=y` colliding on all six LED pins.

**`[SLIDE 17 · S031 · 04:30–04:38 (8.0s)]`** · `Space / →` — **Supporting Trap #2 (Silicon Init)** — `my_reset()` inside [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c).

> Trap two hides during kernel peripheral bringup in [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c)—a quiet initialisation helper called `my_reset()` with global pin side effects.

**`[CODE-CLICK 1/2 · S032 · 04:38–04:45 (7.0s)]`** · `Space / →` — [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L466-L483`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L466-L483) (`my_reset()`) — Highlights loop iterating across **all four pins (`GPIO 26–29`)**, even when only Channel 0 is enabled.

> Even when only Channel 0 is configured, `my_reset()` unconditionally loops across all four pins, `GPIO 26` through `29`, wiping muxing.

**`[CODE-CLICK 2/2 · S033 · 04:45–04:53 (8.0s)]`** · `Space / →` — [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L476-L482`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L476-L482) -> `rp23xx_gpio_set_function()` — Highlights unconditionally setting `RP23XX_PADS_BANK0_GPIO_IE` (`IE=1`).

> Worse, calling `rp23xx_gpio_set_function()` sets `RP23XX_PADS_BANK0_GPIO_IE`, quietly switching the digital input buffer back on right across `GPIO 26` through `29`.

**`[SLIDE 18 · S034 · 04:53–05:00 (7.0s)]`** · `Space / →` — **Silicon Erratum `RP2350-E9`** — Leaving `IE=1` injects input-buffer leakage current into mid-rail analogue potentiometer readings on `GPIO 26`.

> On RP2350 silicon, leaving `IE=1` on an analogue input triggers Erratum `RP2350-E9`, leaking digital buffer current into mid-scale potentiometer readings.

**`[CODE-CLICK 1/2 · S035 · 05:00–05:08 (8.0s)]`** · `Space / →` — [`docs/vanilla/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L42-L54`](../../docs/vanilla/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L42-L54) vs. [`docs/egm/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L67-L72`](../../docs/egm/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L67-L72) — Highlights Vanilla omitting pad isolation after `rp23xx_common_bringup()`.

> Vanilla never clears `IE`, pulls, or isolation after `rp23xx_common_bringup()`, shipping `RP2350-E9` analogue pad leakage straight onto the user's bench.

**`[CLICK 2/2 · S036 · 05:08–05:15 (7.0s)]`** · `Space / →` — **Trap Summary** — Grep gives you symbol names, never cross-layer silicon errata invariants.

> Grep finds you C symbol names; it never warns you about cross-layer silicon errata. Now meet our primary spine trap.

### Beat 06 (`05:15–07:00`, `105.0s`, `13` steps) — Primary Spine (Part 1): The 500 kHz `START_ONCE` Storm & Vanilla's 40 Hz Teardown Race

**`[SLIDE 19 · S037 · 05:15–05:23 (8.0s)]`** · `Space / →` — **Primary Spine Setup** — Meet the `500 kHz` `START_ONCE` ADC storm inside [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c).

> This next quirk inside `rp23xx_adc.c` threads through both experiments and exposes why both vibe coding and unreviewed specs break.

**`[CODE-CLICK 1/2 · S038 · 05:23–05:31 (8.0s)]`** · `Space / →` — [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L232-L237`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L232-L237) (`interrupt_handler()`) — Highlights unconditional `get_next_channel();`.

> Inside the ADC interrupt handler, every single time a sample finishes, `rp23xx_adc.c` unconditionally calls `get_next_channel()` before returning.

**`[CODE-CLICK 2/2 · S039 · 05:31–05:39 (8.0s)]`** · `Space / →` — [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L302-L308`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L302-L308) (`get_next_channel()`) — Highlights back-to-back `RP23XX_ADC_CS_START_ONCE` re-arming inside the ISR.

> And `get_next_channel()` immediately pokes `RP23XX_ADC_CS_START_ONCE` right back into the control register, chaining another conversion as fast as silicon allows.

**`[SLIDE 20 · S040 · 05:39–05:47 (8.0s)]`** · `Space / →` — **The Storm Metric** — **`500 kHz` (`2 µs`/sample) interrupt flood** whenever `/dev/adc0` stays open!

> At forty-eight megahertz and ninety-six cycles per sample, leaving `/dev/adc0` open unleashes five hundred thousand interrupts per second onto the CPU.

**`[SLIDE 21 · S041 · 05:47–05:55 (8.0s)]`** · `Space / →` — **Vanilla's Workaround** — `bargraph_read_adc_once()` in [`docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L80-L103`](../../docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L80-L103).

> Vanilla noticed the ADC kept hammering interrupts continuously, so it improvised a neat-looking user-space workaround in `bargraph_read_adc_once()`.

**`[CODE-CLICK 1/2 · S042 · 05:55–06:03 (8.0s)]`** · `Space / →` — [`docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L87-L95`](../../docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L87-L95) — Highlights `open("/dev/adc0")` -> blocking `read()` -> `close(fd)` at **`40 Hz` (`25 ms` ticks)**.

> Forty times every second—every twenty-five milliseconds—Vanilla opens `/dev/adc0`, reads one blocking message, and immediately calls `close(fd)`.

**`[CLICK 2/2 · S043 · 06:03–06:11 (8.0s)]`** · `Space / →` — **Why That Looked Clever** — Closing `/dev/adc0` stops conversions between ticks, dodging the `500 kHz` storm.

> On the desk, CPU load drops away and the bar graph tracks cleanly. Looks like a sharp engineering workaround, doesn't it?

**`[SLIDE 22 · S044 · 06:11–06:19 (8.0s)]`** · `Space / →` — **Zoom Into Upper-Half Teardown** — [`nuttx/drivers/analog/adc.c#L181-L214`](../../nuttx/drivers/analog/adc.c#L181-L214) (`adc_close()`).

> Until you trace `close(fd)` down into NuttX's upper-half driver in [`nuttx/drivers/analog/adc.c`](../../nuttx/drivers/analog/adc.c) and see what `adc_close()` actually locks.

**`[CODE-CLICK 1/2 · S045 · 06:19–06:27 (8.0s)]`** · `Space / →` — [`nuttx/drivers/analog/adc.c#L187-L210`](../../nuttx/drivers/analog/adc.c#L187-L210) (`adc_close()`) — Highlights `nxmutex_lock(&dev->ad_lock)` with **zero `enter_critical_section()`** around `ao_shutdown()`.

> `adc_close()` holds `nxmutex_lock()`, not `enter_critical_section()`. A mutex blocks competing threads—it does nothing to stop a two-microsecond hardware interrupt.

**`[CODE-CLICK 2/2 · S046 · 06:27–06:35 (8.0s)]`** · `Space / →` — [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L395-L407`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L395-L407) (`remove_device()`) — Highlights `g_first_device = PRIV(g_first_device)->next_device;` (`#L399`) seven lines before `g_active_count -= 1;` (`#L406`).

> Inside `remove_device()`, `g_first_device` becomes `NULL` seven lines before `g_active_count` drops. If the ADC interrupt lands inside that tiny window...

**`[SLIDE 23 · S047 · 06:35–06:43 (8.0s)]`** · `Space / →` — **The Teardown Race Diagram** — `RP23XX_ADC_IRQ_FIFO` fires between `#L399` and `#L406` -> `get_next_channel()` hits **`ASSERT(next != g_current_channel)` (`#L314`)**!

> ...the interrupt handler runs `get_next_channel()`, finds zero active devices, and crashes straight into `ASSERT(next != g_current_channel)` at line 314.

**`[CLICK 1/2 · S048 · 06:43–06:51 (8.0s)]`** · `Space / →` — **Risk Multiplier** — Calling `close(fd)` at **`40 Hz`** rolls those race dice forty times every second!

> By closing `/dev/adc0` at forty hertz, Vanilla quietly turned a rare shutdown race into forty kernel-panic dice rolls every single second.

**`[CLICK 2/2 · S049 · 06:51–07:00 (9.0s)]`** · `Space / →` — **Read Safety Nits** — Blocking `read()` with zero `O_NONBLOCK` / `poll()` timeout and silently swallowed errors in `bargraph_daemon()`.

> On top of that, blocking `read()` has zero timeout and `bargraph_daemon()` silently swallows read failures instead of reporting or blinking faults.

### Beat 07 (`07:00–08:00`, `60.0s`, `8` steps) — Vanilla Audit Verdict: `5 / 10` Primary (`4 Pass / 4 Partial / 2 Fail` Strict)

**`[SLIDE 24 · S050 · 07:00–07:08 (8.0s)]`** · `Space / →` — **Give Credit Where Due** — What Vanilla actually got right ([`docs/vanilla/CODE_REVIEW.md`](../../docs/vanilla/CODE_REVIEW.md) + [`docs/reports/vanilla-vs-egm-comparison.md`](../../docs/reports/vanilla-vs-egm-comparison.md)).

> Now, to be fair—no cheap straw-manning here—let's credit what Vanilla vibe coding genuinely got right on the first try.

**`[CLICK 1/1 · S051 · 07:08–07:15 (7.0s)]`** · `Space / →` — **Vanilla's Genuine Wins** — LEDs safely on `D3–D8` (dodging `GPIO 26–29` clobber) and clean shared [`src/bargraph_logic.c`](../../docs/vanilla/xiao-rp2350-bargraph/src/bargraph_logic.c) host unit tests.

> It parked the LEDs on `D3` through `D8`, dodging `my_reset()`, and wrote a cleaner shared [`bargraph_logic.c`](../../docs/vanilla/xiao-rp2350-bargraph/src/bargraph_logic.c) test split than EGM's first pass.

**`[SLIDE 25 · S052 · 07:15–07:23 (8.0s)]`** · `Space / →` — **Other Concurrency & Lifecycle Gaps** — Dual autostart (`rp23xx_bringup` + `rcS`), unguarded `g_bargraph_state` races, LEDs left frozen on at `stop`.

> Still, Vanilla also shipped dual autostart (`rp23xx_bringup` plus `rcS`), unlocked global state races, and left LEDs frozen bright on `stop`.

**`[CLICK 1/1 · S053 · 07:23–07:30 (7.0s)]`** · `Space / →` — **Two High-Severity Fails** — `RP2350-E9` `IE=1` analogue leakage + `40 Hz` `close(fd)` `remove_device()` ISR race.

> Combined with `RP2350-E9` pad leakage and the forty-hertz `close(fd)` race, two high-severity defects sit silently inside a blinking demo.

**`[SLIDE 26 · S054 · 07:30–07:38 (8.0s)]`** · `Space / →` — **Primary Scorecard Verdict** — **Vanilla (`nuttxspace-vanilla`): `5 / 10` Primary Pass**.

> On our ten-point hardware and RTOS trap scorecard, Vanilla scores five out of ten: impressive surface velocity, unsafe underneath.

**`[CLICK 1/1 · S055 · 07:38–07:45 (7.0s)]`** · `Space / →` — **Strict Goldfish Verdict** — **`4 Pass / 4 Partial / 2 Fail`** (downgrading `Make.defs` missing `distclean` filter).

> Under a strict hostile audit, that drops to four passes, four partials, and two hard fails across ten invariants.

**`[SLIDE 27 · S056 · 07:45–07:53 (8.0s)]`** · `Space / →` — **Rensin's Diagnosis** — **Context Pollution + Self-Validation**: one thread interviews, guesses, writes C, and grades its own homework.

> This is exactly the trap Dave Rensin diagnosed: context pollution and self-validation, where one chat session guesses, writes C, and approves itself.

**`[CLICK 1/1 · S057 · 07:53–08:00 (7.0s)]`** · `Space / →` — **Act III Pivot** — What happens when we switch to EGM and roll architectural contracts for the Elephant first?

> Same hardware, same prompt—now let's roll contracts for the Elephant before writing a single line of C.

---

## Act III — Rolling Docs for the Agent: EGM Codebase Ramp-Up (`08:00–19:00`, `11.0 min`, Beats `08–16`, `S058–S132`)

### Act III.a — Grounding the Map (`08:00–10:00`, `2.0 min`, Beats `08–09`, `S058–S073`)

#### Beat 08 (`08:00–09:00`, `60.0s`, `8` steps) — Feeding the Elephant (Step 1: Context Loading): Why `3,241` Bottom-Up `README.md` Contracts Beat Grepping Raw C

**`[SLIDE 28 · S058 · 08:00–08:08 (8.0s)]`** · `Space / →` — **Act III Divider** — **Rolling Docs for the Agent: The Elephant-Goldfish Model (`nuttxspace-egm`)**.

> Welcome to Experiment B, `nuttxspace-egm`—running the identical prompt inside the Elephant-Goldfish Model across our contract-mapped submodules.

**`[CLICK 1/1 · S059 · 08:08–08:15 (7.0s)]`** · `Space / →` — **Rensin Step 1: Feeding the Elephant** — Build cheap, navigable architectural memory across `2,607` `nuttx/` and `634` `apps/` directories via `/egm-bootstrap`.

> Step one feeds the Elephant cheaply: `3,241` bottom-up `README.md` files summarising every directory across `nuttx/` and `apps/` before anyone starts coding.

**`[SLIDE 29 · S060 · 08:15–08:23 (8.0s)]`** · `Space / →` — **5-Section Contract Anatomy** — Excerpt header of [`nuttx/arch/arm/src/rp23xx/README.md#L1-L71`](../../nuttx/arch/arm/src/rp23xx/README.md#L1-L71).

> Open [`nuttx/arch/arm/src/rp23xx/README.md`](../../nuttx/arch/arm/src/rp23xx/README.md) and you see the exact five-section contract format shared across all three thousand directories.

**`[CLICK 1/4 · S061 · 08:23–08:30 (7.0s)]`** · `Space / →` — **Sections 1–2 (`Overview` + `Subdirectories & Boundaries`)** — Defines ownership boundaries before touching files.

> Sections one and two state what lives here and link child boundaries—so the agent navigates hierarchy without slurping forty files.

**`[CLICK 2/4 · S062 · 08:30–08:38 (8.0s)]`** · `Space / →` — **Section 3 (`Interface`)** — Public functions, devices, and Kconfig dependencies exposed upward.

> Section three lists the public interface via `[file:symbol]` citations—giving the agent exact entry points instead of blind string matches.

**`[CLICK 3/4 · S063 · 08:38–08:45 (7.0s)]`** · `Space / →` — **Section 4 (`Invariants`)** — Cross-layer rules grep misses (`my_reset()`, `START_ONCE`, `RP2350-E9`).

> Section four documents invariants—assert preconditions, critical-section rules, and silicon constraints that grep never surfaces by itself.

**`[CLICK 4/4 · S064 · 08:45–08:53 (8.0s)]`** · `Space / →` — **Section 5 (`Verification`)** — Concrete tests and assertions proving contract truth (paired with `Side Effects` at `#L56-L71`).

> And sections on side effects and verification point directly at MMIO registers touched and tests proving those invariants still hold.

**`[SLIDE 30 · S065 · 08:53–09:00 (7.0s)]`** · `Space / →` — **Bottom-Up Synthesis** — Leaf modules summarise upward into subsystem maps; the Elephant navigates contracts instead of raw C.

> Leaf modules roll upward into subsystem blueprints, compressing thirty years of NuttX architecture into lightweight markdown your Elephant reads in seconds.

#### Beat 09 (`09:00–10:00`, `60.0s`, `8` steps) — Phase 1 (`01-bootstrap`): Anchoring the Workspace & Catching Upstream Drift

**`[FREEZE-FRAME · SLIDE 31 · S066 · 09:00–09:08 (8.0s)]`** · `Space / →` — **Freeze-Frame (`01-bootstrap`)** — [`prototypes/01-bootstrap-cast-player.html`](../../prototypes/01-bootstrap-cast-player.html) at Stop Point `8. Q1: Run EGM Drift Report?` (`00:38` warped, `7x` speed badge).
- *Cast Cue:* Full-frame screenshot/embed of [`prototypes/01-bootstrap-cast-player.html`](../../prototypes/01-bootstrap-cast-player.html) frozen on pill `8. Q1: Run EGM Drift Report?` (`00:38` warped, `7x` speed badge).

> Invoking `/egm-bootstrap` anchors the workspace root in eight raw minutes, reading `nuttx/` and `apps/` contracts before pausing here on drift detection.

**`[CLICK 1/1 · S067 · 09:08–09:15 (7.0s)]`** · `Space / →` — **Phase 1 Telemetry** — `72` steps, `40` tool calls, **`6` `README.md` reads**, `0` lines of C written.

> Seventy-two steps, six contract reads, zero lines of C—and the Elephant already holds the exact cross-submodule build boundary in memory.

**`[SLIDE 32 · S068 · 09:15–09:23 (8.0s)]`** · `Space / →` — **Bootstrapped Root Contract** — [`docs/egm/README.md#L7-L18`](../../docs/egm/README.md#L7-L18) (`28` lines anchoring the repository layout).

> Out pops a crisp twenty-eight-line root blueprint locking sibling layout, flat kernel/user-space separation, and the monotonic kernel boot chain.

**`[SLIDE 33 · S069 · 09:23–09:30 (7.0s)]`** · `Space / →` — **Trust-But-Verify Hook** — `/egm-drift-report`: compare checked-in markdown contracts directly against live C source.

> Because markdown contracts can go stale over time, Phase 1 immediately follows bootstrap with `/egm-drift-report` against the live C tree.

**`[CODE-CLICK 1/2 · S070 · 09:30–09:38 (8.0s)]`** · `Space / →` — [`docs/egm/reports/2026-11-16_DRIFT_REPORT.md#L9-L11`](../../docs/egm/reports/2026-11-16_DRIFT_REPORT.md#L9-L11) (`11` lines) — Highlights `OSINIT_HARDWARE` drift flagged in `nuttx/README.md#L79`.

> Eleven lines of drift report immediately spot a missing enum state in `nuttx/README.md` line 79: `OSINIT_HARDWARE` was omitted.

**`[CODE-CLICK 2/2 · S071 · 09:38–09:45 (7.0s)]`** · `Space / →` — [`nuttx/sched/init/nx_start.c#L658-L664`](../../nuttx/sched/init/nx_start.c#L658-L664) — Highlights `g_nx_initstate = OSINIT_HARDWARE;` at `#L660`.

> It cites `nx_start.c` line 660 verbatim, proving where hardware init finishes and reconciling contract truth before any design begins.

**`[SLIDE 34 · S072 · 09:45–09:53 (8.0s)]`** · `Space / →` — **Phase 1 Payoff** — Grounded map + live drift check in 8 minutes.

> Eight minutes of setup buys a verified workspace map and catches stale documentation before we make a single architecture call.

**`[CLICK 1/1 · S073 · 09:53–10:00 (7.0s)]`** · `Space / →` — **Phase 2 Transition** — Next: `/egm-brainstorm`—interrogating the design with zero code allowed.

> Now let's invoke `/egm-brainstorm` and watch the Elephant grow without touching a single line of C.

### Act III.b — From Vibe to Architecture (`10:00–13:00`, `3.0 min`, Beats `10–11`, `S074–S095`)

#### Beat 10 (`10:00–11:30`, `90.0s`, `11` steps) — Growing the Elephant (Steps 2–3: "No Code Yet" & The Sycophant Challenge via `02-brainstorm`)

**`[SLIDE 35 · S074 · 10:00–10:08 (8.0s)]`** · `Space / →` — **Rensin Step 2: The *"No Code Yet"* Rule** — Strictly prohibit generating C while exploring constraints and trade-offs.

> Phase 2 enforces Dave Rensin's "No Code Yet" rule—banning implementation so the Elephant has to ask hard questions before solving anything.

**`[FREEZE-FRAME · SLIDE 36 · S075 · 10:08–10:16 (8.0s)]`** · `Space / →` — **Freeze-Frame (`02-brainstorm`)** — [`prototypes/02-brainstorm-cast-player.html`](../../prototypes/02-brainstorm-cast-player.html) at Stop Point `16. Q7: Readiness Checkpoint (Review with Goldfishes)` (`02:53` warped).
- *Cast Cue:* Freeze-frame embed of [`prototypes/02-brainstorm-cast-player.html`](../../prototypes/02-brainstorm-cast-player.html) on pill `16. Q7: Readiness Checkpoint (Review with Goldfishes)` (`02:53` warped), highlighting the `19`-pill chapter bar.

> Running `/egm-brainstorm` drives forty-one minutes of structured Q&A across nineteen checkpoints—notice that packed row of chapter pills along the top.

**`[CLICK 1/2 · S076 · 10:16–10:24 (8.0s)]`** · `Space / →` — **Interview Depth Contrast** — **19 chapter stop points** of Socratic exploration (`41m 32s` raw) vs. Vanilla's single question.

> Compare Vanilla's single "pick any free pins" chat against nineteen checkpoints grilling pin budgets, autostart timing, and PWM slice sharing.

**`[CLICK 2/2 · S077 · 10:24–10:32 (8.0s)]`** · `Space / →` — **Contract Reads Counter** — **`49` `README.md` reads** (`216` `read_file` calls) before a design choice locks.

> Before a single requirement settles, the Elephant reads forty-nine directory contracts and two hundred and sixteen files across `nuttx/` and `apps/`.

**`[SLIDE 37 · S078 · 10:32–10:40 (8.0s)]`** · `Space / →` — **Rensin Step 3: *The Sycophant Challenge*** — 7-lens Goldfish shootdown swarm (Contrarian, Economics, Ergonomics, Migration, SRE, Tech Debt, Inventive Catalyst).

> Then comes Rensin's Sycophant Challenge: seven hostile lenses attacking our own hypothesis so the agent stops cheering me on politely.

**`[SLIDE 38 · S079 · 10:40–10:48 (8.0s)]`** · `Space / →` — **Concepts Brief Output** — [`docs/egm/designs/concepts_brief.md#L16-L35`](../../docs/egm/designs/concepts_brief.md#L16-L35) listing **`7` hardware/driver obstacles** upfront.

> Every bit of that reading lands in [`concepts_brief.md`](../../docs/egm/designs/concepts_brief.md), listing seven real upstream hardware and driver traps before drafting even starts.

**`[CLICK 1/4 · S080 · 10:48–10:56 (8.0s)]`** · `Space / →` — **Obstacles #1 & #2** — `my_reset()` pin clobber (`GPIO 26–29`) + `RP2350-E9` `IE=1` pad isolation after `rp23xx_common_bringup()`.

> Right there on lines 29 to 31, it catches Vanilla's exact `my_reset()` pin clobber and `RP2350-E9` pad leakage beforehand.

**`[CLICK 2/4 · S081 · 10:56–11:04 (8.0s)]`** · `Space / →` — **Obstacle #3** — 5-slice PWM layout (`D1`/`D2` safely re-muxed by opening `/dev/pwm5,6` in `bargraphd` *after* `my_reset()`).

> It also uncovers NuttX's hardcoded `pwm_info_s` indexing, `b16ONE` wraparound turning 100% LEDs dark, and `PWMIOC_STOP` freezing output pins high.

**`[CLICK 3/4 · S082 · 11:04–11:12 (8.0s)]`** · `Space / →` — **Obstacle #4** — `500 kHz` `START_ONCE` ADC interrupt storm flagged explicitly as a design hazard.

> And at trap seven, it flags our primary spine—the half-megahertz ADC interrupt storm—which sets up our big honest blunder next.

**`[CLICK 4/4 · S083 · 11:12–11:21 (9.0s)]`** · `Space / →` — **Obstacles #5–#7** — `BOARD_LATE_INITIALIZE` single boot, clean submodule boundary, and host test separation.

> Plus `CONFIG_BOARD_LATE_INITIALIZE=y`, why USB serial blocks init scripts until a laptop connects, and clean `apps/external` submodule isolation.

**`[SLIDE 39 · S084 · 11:21–11:30 (9.0s)]`** · `Space / →` — **Phase 2 Payoff** — Zero lines of C written; seven traps caught early.

> Zero C written, seven real traps caught upfront. Now watch what happens in Phase 3 when the Elephant drafts the full spec.

#### Beat 11 (`11:30–13:00`, `90.0s`, `11` steps) — The Elephant's Blind Spot (Step 4: First Draft Proposal via `03-draft` & Primary Spine Part 2)

**`[SLIDE 40 · S085 · 11:30–11:38 (8.0s)]`** · `Space / →` — **Phase 3 (`/egm-draft`)** — Turning `concepts_brief.md` section-by-section into [`docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md`](../../docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md).

> Phase 3 runs `/egm-draft`, expanding that concepts brief section by section into a `693`-line architecture and implementation specification.

**`[FREEZE-FRAME · SLIDE 41 · S086 · 11:38–11:46 (8.0s)]`** · `Space / →` — **Freeze-Frame (`03-draft`)** — [`prototypes/03-draft-cast-player.html`](../../prototypes/03-draft-cast-player.html) at Stop Point `6. Q3: Section Approval — Technical Plan` (`01:30` warped, `15x` speed badge).
- *Cast Cue:* Freeze-frame embed of [`prototypes/03-draft-cast-player.html`](../../prototypes/03-draft-cast-player.html) paused at Stop Point `6. Q3: Section Approval — Technical Plan` (`01:30` warped, `15x` badge).

> Calling `/egm-draft` pauses explicitly at every section boundary—forcing me, the human, to review and approve the Technical Plan before continuing.

**`[CLICK 1/1 · S087 · 11:46–11:54 (8.0s)]`** · `Space / →` — **Semantic Boundaries** — Splits high-level `FeatureRange` architecture from `DetailedRange` implementation steps.

> Splitting high-level architectural choices from file-by-file C instructions makes every single decision easy to read—except when both minds share a blind spot.

**`[SLIDE 42 · S088 · 11:54–12:02 (8.0s)]`** · `Space / →` — **Primary Spine (Part 2): The Honest False Start** — How did our `v0-draft` plan to stop the `500 kHz` `START_ONCE` ADC storm?

> Remember earlier I promised no unblemished triumph story? Here is where our Elephant—and I—walked straight into a classic trap.

**`[CODE-CLICK 1/2 · S089 · 12:02–12:10 (8.0s)]`** · `Space / →` — [`docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L33`](../../docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L33) — Highlights `RP23XX_ADC_DIV = 0xFFFF00` proposed to throttle conversion cadence.

> The Elephant spotted a hardware divider register in the RP2350 headers—`RP23XX_ADC_DIV`—and proposed pacing the ADC down to seven hundred hertz.

**`[CODE-CLICK 2/2 · S090 · 12:10–12:18 (8.0s)]`** · `Space / →` — [`docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L242-L245`](../../docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L242-L245) — Highlights `putreg32(0x00ffff00, RP23XX_ADC_DIV)` and `48,000,000 Hz / 65536 ≈ 732.42 Hz`.

> Look how convincing that looked: exact hex mask `0x00ffff00`, neat LaTeX frequency arithmetic, `99.85%` interrupt reduction calculated right down to two decimal places.

**`[SLIDE 43 · S091 · 12:18–12:26 (8.0s)]`** · `Space / →` — **Mutual Agreement Illusion** — `53` cumulative `README.md` reads across Phases 2–3, and *both* Brett and the Elephant approved it!

> Fifty-three `README.md` reads deep, the prose sounded airtight, the frequency maths checked out, and I nodded along and clicked Approve.

**`[CLICK 1/1 · S092 · 12:26–12:34 (8.0s)]`** · `Space / →` — **Verdict Stamp** — **`DEAD WRONG`**.

> And we were both dead wrong. Our tidy divider register would have done literally nothing on real silicon.

**`[SLIDE 44 · S093 · 12:34–12:42 (8.0s)]`** · `Space / →` — **Rensin's Central Warning Quote** — *"A costly failure is a plan the agent and operator agreed on too quickly, because both were looking at the same context."*

> Dave Rensin describes this exact failure: a costly mistake is a plan both agent and operator agreed on inside the same context.

**`[CLICK 1/2 · S094 · 12:42–12:51 (9.0s)]`** · `Space / →` — **Why the Elephant Missed It** — Once a long thread adopts an elegant explanation, confirmation bias protects it from scrutiny.

> Once our Elephant locked onto `RP23XX_ADC_DIV`, confirmation bias kicked in; it never stopped to check whether `START_ONCE` even reads that divider.

**`[CLICK 2/2 · S095 · 12:51–13:00 (9.0s)]`** · `Space / →` — **Act III.c Pivot** — That is why we never hand `v0-draft` to a coder—we hand it to stateless Goldfish.

> Which is precisely why nobody writes C yet. Watch what happens next when zero-context Goldfish review `v0-draft` cold.

### Act III.c — The Goldfish Swarm (`13:00–17:30`, `4.5 min`, Beats `12–15`, `S096–S123`)

#### Beat 12 (`13:00–14:15`, `75.0s`, `7` steps) — The Goldfish Protocol (Steps 5–9: Stateless 3-Second-Memory Reviewers in `04-review` Pass 1)

**`[SLIDE 45 · S096 · 13:00–13:07.3 (7.3s)]`** · `Space / →` — **Meet the Goldfish** — Fresh, stateless subagents with a **"three-second memory"**: zero brainstorm history, zero loyalty to `v0-draft`.

> A Goldfish is a fresh subagent with zero memory of our brainstorm, zero chat history, and zero emotional loyalty to `v0-draft`.

**`[SLIDE 46 · S097 · 13:07.3–13:15.3 (8.0s)]`** · `Space / →` — **5-Stage `/egm-review` Pipeline (Steps 5–9)** — `Zoom Out` -> `Comprehension` -> `Critic` -> `Intermediate Zoom Out` -> `Implementation Readiness + Codebase Details`.

> `/egm-review` runs five adversarial stages: high-altitude Zoom Out, naive Comprehension, hostile Design Critic, Synthesizer, and line-by-line Codebase Details.

**`[CLICK 1/1 · S098 · 13:15.3–13:23.3 (8.0s)]`** · `Space / →` — **Pass 1 Scale Counter** — `16` Goldfish subagents in `04-review` reading **`146` `README.md` contracts** against the draft.

> Across Pass 1 and Pass 2, sixteen fresh Goldfish fanned out in parallel, checking every claim against one hundred and forty-six contracts.

**`[LIVE CAST · SLIDE 47 · S099 · 13:23.3–13:53.0 (29.7s)]`** · `Space / →` to slide, then `]` / `p` — **LIVE CAST (`29.7s`, Beat A)** on [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html).
- *Cast Cue:* Launch [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html). Jump to Stop Point `4. Pass 1 Q1: Select Review Execution Mode (Consolidated)` (`00:11` warped) and press `p` to run **Seg 05** (`29.7s @ 39x`, compressing `21m 33s` raw -> auto-pauses at `5. Pass 1 Review Complete`).

> I invoke `/egm-review` on [`xiao-rp2350-bargraph.md`](../../docs/egm/designs/xiao-rp2350-bargraph.md) and choose `consolidated` mode. Look at the effect: instead of one chat thread nodding along with itself, parallel Goldfish subagents spin up cold across five stages, cross-checking every register address, Kconfig guard, and fixed-point equation in our spec directly against NuttX's driver files and `README.md` contracts. Compressing twenty-one and a half minutes at thirty-nine times speed, those stateless reviewers pile into `pass1/goldfish_review_report.md`—and our clean draft is about to look very bruised.

**`[SLIDE 48 · S100 · 13:53.0–14:00.0 (7.0s)]`** · `Space / →` — **Pass 1 Harvest ([`docs/egm/reports/pass1/goldfish_review_report.md`](../../docs/egm/reports/pass1/goldfish_review_report.md))** — **`4` Blockers (`B-1..B-4`)** + **`5` Majors (`M-1..M-5`)** surfaced before any C was written.

> The verdict lands: four Blockers, five Majors, and three Nits—twelve real discrepancies surfaced in markdown before a single line of C existed.

**`[CLICK 1/2 · S101 · 14:00.0–14:08.0 (8.0s)]`** · `Space / →` — **The Blocker Payoff** — Every single Blocker would have burned hours at a JTAG debugger or panicked the kernel.

> Every one of those four Blockers would have burned half a weekend with a multimeter and JTAG debugger if we had shipped first.

**`[CLICK 2/2 · S102 · 14:08.0–14:15.0 (7.0s)]`** · `Space / →` — **Interactive Resolution Cue** — Watch `/egm-resolve` walk us straight into Item 1 of 12: Blocker `B-1`.

> Next, `/egm-resolve` steps through each finding with me in `interactive` mode—starting right at Item 1, Blocker `B-1`.

#### Beat 13 (`14:15–15:30`, `75.0s`, `8` steps) — Primary Spine (Part 3a): Pass 1 `B-1` Proves `RP23XX_ADC_DIV` Is a No-Op Under `START_ONCE`

**`[SLIDE 49 · S103 · 14:15–14:23.4 (8.4s)]`** · `Space / →` — **Primary Spine Payoff Setup** — `egm-codebase-details` checks `v0-draft`'s `RP23XX_ADC_DIV` plan directly against [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c).

> Because `egm-codebase-details` wakes up cold, it has zero reason to trust our `732 Hz` divider arithmetic. It opens `rp23xx_adc.c` itself.

**`[LIVE CAST · SLIDE 50 · S104 · 14:23.4–14:42.0 (18.6s)]`** · `Space / →` to slide, then `]` / `p` — **LIVE CAST (`18.6s`, Beat B)** on [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html).
- *Cast Cue:* Inside [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html), click pill `8. Reply: interactive (Walk Through 12 Findings)` (`01:10` warped) and press `p` through **Seg 09** (`17.1s @ 2x` -> `9. Item 1/12 (B-1): NVIC IRQ Gating vs. RP23XX_ADC_DIV`) and **Seg 10** (`1.5s @ 2x` -> `10. Reply: accept`).

> Selecting `interactive` mode forces `/egm-resolve` to walk me through each blocker one by one instead of auto-applying blind patches. Right on Item 1 of 12, `B-1` cites lines 169 through 320 of `rp23xx_adc.c` and lays out the replacement: NVIC IRQ gating around each fifty-hertz burst. Watching that evidence appear, I type `accept`—no ego, just cold proof.

**`[SLIDE 51 · S105 · 14:42.0–14:50.0 (8.0s)]`** · `Space / →` — **`B-1` Finding Card ([`docs/egm/reports/pass1/goldfish_review_report.md#L8-L16`](../../docs/egm/reports/pass1/goldfish_review_report.md#L8-L16))** — **`RP23XX_ADC_DIV` only governs free-running `START_MANY` mode.**

> Here is what `B-1` actually proved: the RP2350 `ADC_DIV` register only paces free-running `START_MANY` conversions—it never runs under `START_ONCE`.

**`[CLICK 1/1 · S106 · 14:50.0–14:58.0 (8.0s)]`** · `Space / →` — **Hardware Reality** — Under NuttX's `START_ONCE` driver, `RP23XX_ADC_DIV = 0xFFFF00` is a **total no-op**—the `500 kHz` IRQ storm still fires!

> Our clever `0xFFFF00` divider was a complete no-op; the Cortex-M33 would still have been drowned under half a million interrupts per second.

**`[SLIDE 52 · S107 · 14:58.0–15:06.0 (8.0s)]`** · `Space / →` — **The Goldfish Solution (`B-1`)** — Keep `/dev/adc0` open (dodging Vanilla's `40 Hz` `close(fd)` teardown race) and gate at the **NVIC** instead.

> Instead of closing `/dev/adc0` like Vanilla did, the Goldfish proposed keeping it persistently open and gating the interrupt line directly in the NVIC.

**`[CODE-CLICK 1/3 · S108 · 15:06.0–15:14.0 (8.0s)]`** · `Space / →` — [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L266-L293`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L266-L293) (`bargraph_sample_adc()`) — Highlights gating `RP23XX_ADC_IRQ_FIFO` around a 5-sample burst.

> Flush the FIFO, unmask `RP23XX_ADC_IRQ_FIFO` for ten microseconds to pull five samples, then mask it—dropping 500,000 interrupts down to 250.

**`[CLICK 2/3 · S109 · 15:14.0–15:22.0 (8.0s)]`** · `Space / →` — **Cost of Discovery** — Caught in spec review: 10 seconds to accept. Caught on hardware: hours with a logic analyser.

> Ten seconds to type `accept` in markdown versus hours staring at a stalled Cortex-M33 under a logic analyser.

**`[CLICK 3/3 · S110 · 15:22.0–15:30.0 (8.0s)]`** · `Space / →` — **And `B-1` Wasn't Alone** — Two other Pass 1 blockers caught pure-maths and Kconfig failures simultaneously.

> And `B-1` was only the headline catch. Let's look at `B-2` and `B-3` sitting right underneath it in the same report.

#### Beat 14 (`15:30–16:15`, `45.0s`, `6` steps) — Supporting Trap #3 & Build Guard: Pass 1 `B-2` (4x Q4 EMA Bug) & `B-3` (`CONFIG_ANALOG=y`)

**`[SLIDE 53 · S111 · 15:30–15:38 (8.0s)]`** · `Space / →` — **Supporting Trap #3 (`B-2`)** — Pure logic/maths verification inside [`docs/egm/reports/pass1/goldfish_review_report.md#L20-L28`](../../docs/egm/reports/pass1/goldfish_review_report.md#L20-L28).

> Goldfish do not only read C files—in Issue `B-2`, one verified our fixed-point exponential moving average algebra by hand.

**`[CODE-CLICK 1/2 · S112 · 15:38–15:45 (7.0s)]`** · `Space / →` — [`docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L253-L254`](../../docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L253-L254) — Highlights buggy `filtered = ema_q4 >> 2` (`[0, 16380]`, **`4x` gain**!).

> In `v0-draft`, we wrote `ema_q4 >> 2`—leaving a four-times gain error that pegged all six LEDs at 100% by quarter-turn pot travel.

**`[CODE-CLICK 2/2 · S113 · 15:45–15:53 (8.0s)]`** · `Space / →` — [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L661-L671`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L661-L671) (`bargraph_filter_adc()`) — Highlights first-sample seeding `raw_avg << 4` and `(ema_q4 + 8u) >> 4`.

> `B-2` fixed both: seeding `raw_avg << 4` on the first sample so startup never ramps from zero, then rounding `(*ema_q4 + 8u) >> 4`.

**`[SLIDE 54 · S114 · 15:53–16:00 (7.0s)]`** · `Space / →` — **Build-System Guard (`B-3`)** — Pre-build Kconfig check from [`docs/egm/reports/pass1/goldfish_review_report.md#L32-L42`](../../docs/egm/reports/pass1/goldfish_review_report.md#L32-L42).

> Meanwhile, Issue `B-3` audited every single Kconfig symbol in our draft directly against `nuttx/drivers/timers/Kconfig` and `nuttx/drivers/analog/Kconfig`.

**`[CLICK 1/2 · S115 · 16:00–16:08 (8.0s)]`** · `Space / →` — **Two Kconfig Catches** — Missing `CONFIG_ANALOG=y` (Vanilla's Step 180 silent drop!) + hallucinated `CONFIG_PWM_MULTICHAN=y`.

> It caught both Vanilla's missing `CONFIG_ANALOG=y` drop and a hallucinated `CONFIG_PWM_MULTICHAN=y` symbol before `configure.sh` ever ran.

**`[CLICK 2/2 · S116 · 16:08–16:15 (7.0s)]`** · `Space / →` — **Multi-Angle Defence** — One Goldfish checks silicon errata, another checks fixed-point shifts, another checks Kconfig menus.

> One pass caught four huge Blockers. Now watch why stopping after a single review pass is still a rookie mistake.

#### Beat 15 (`16:15–17:30`, `75.0s`, `7` steps) — Primary Spine (Part 3b): `/clear` + Pass 2 `CA-01` (The 5-Byte `adc_read()` Short-Read Trap)

**`[SLIDE 55 · S117 · 16:15–16:22.9 (7.9s)]`** · `Space / →` — **The Second-Order Patch Trap** — Fixing `B-1` added a 5-sample `read()` burst to the spec; could our *own patch* hide a new bug?

> Every time you patch a spec to resolve a blocker, you write fresh assumptions that no Goldfish has ever examined.

**`[CLICK 1/1 · S118 · 16:22.9–16:29.9 (7.0s)]`** · `Space / →` — **Enter `/clear`** — Archive Pass 1, wipe the parent session memory completely, and launch Pass 2 cold.

> So we archive Pass 1, issue `/clear` to wipe parent memory completely, and run `/egm-review` a second time cold.

**`[LIVE CAST · SLIDE 56 · S119 · 16:29.9–17:00.0 (30.1s)]`** · `Space / →` to slide, then `]` / `p` — **LIVE CAST (`30.1s`, Beat C)** on [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html).
- *Cast Cue:* Inside [`prototypes/04-review-cast-player.html`](../../prototypes/04-review-cast-player.html), click pill `13. Pass 1 Archived — Ready for /clear` (`02:17` warped), press `p` through **Seg 14–16** (`1.1s @ 2x`, showing `/clear` + Pass 2 `/egm-review (with GoldfishContext)`), then click pill `17. Pass 2 Q1: Select Review Execution Mode (Consolidated)` (`02:41` warped) and press `p` through **Seg 18** (`29.0s @ 26x` -> auto-pauses at `18. Pass 2 Review Complete`).

> Watch the exact sequence: Pass 1 is archived, we fire `/clear` so the session that wrote the `B-1` patch cannot grade its own homework, and we launch Pass 2 `/egm-review` in consolidated mode. Immediately at twenty-six times speed, a fresh wave of stateless Goldfish reads our brand-new NVIC IRQ gating section alongside NuttX's upper-half analogue driver `adc.c`—and catches a subtle second-order bug inside our own fix.

**`[SLIDE 57 · S120 · 17:00.0–17:08.0 (8.0s)]`** · `Space / →` — **Pass 2 Catch `CA-01` ([`docs/egm/reports/pass2/goldfish_review_report.md#L9-L17`](../../docs/egm/reports/pass2/goldfish_review_report.md#L9-L17))** — Upper-half `adc_read()` **short-read behaviour**!

> Pass 2 returned `0` Blockers and caught `CA-01`—a sneaky short-read behaviour inside our brand-new `B-1` burst specification.

**`[CODE-CLICK 1/3 · S121 · 17:08.0–17:15.0 (7.0s)]`** · `Space / →` — [`nuttx/drivers/analog/adc.c#L270-L305`](../../nuttx/drivers/analog/adc.c#L270-L305) (`adc_read()`) — Highlights `while (fifo->af_head == fifo->af_tail)` waking on `nxsem_wait(&fifo->af_sem)` and returning once `1` `5`-byte message lands.

> Asking POSIX `read()` for twenty-five bytes does not wait for twenty-five bytes; `adc_read()` wakes after one five-byte sample, leaving `msgs[1..4]` as uninitialised stack garbage.

**`[CODE-CLICK 2/3 · S122 · 17:15.0–17:23.0 (8.0s)]`** · `Space / →` — [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L270-L306`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L270-L306) (`bargraph_sample_adc()`) — Highlights `while (total_read < bytes_needed)` (`25` B, `EINTR`-safe) and `for (i = 1; ...)` discarding stale `msgs[0]`.

> So the Pass 2 spec locks in the exact `EINTR`-safe twenty-five-byte accumulation loop, masks the IRQ after, and discards stale `msgs[0]`.

**`[CLICK 3/3 · S123 · 17:23.0–17:30.0 (7.0s)]`** · `Space / →` — **Two-Pass Spine Summary** — Pass 1 killed `RP23XX_ADC_DIV`; `/clear` + Pass 2 killed the `5`-byte stack-garbage read.

> Pass 1 killed the fake clock divider; `/clear` plus Pass 2 killed the stack-garbage read. Now—finally—we write some C.

### Act III.d — Boring Implementation (`17:30–19:00`, `1.5 min`, Beat `16`, `S124–S132`)

#### Beat 16 (`17:30–19:00`, `90.0s`, `9` steps) — Phase 5 (`05-implement`): First-Attempt `nuttx.uf2` & Rolling the Map Forward

**`[SLIDE 58 · S124 · 17:30–17:38 (8.0s)]`** · `Space / →` — **Phase 5 (`05-implement`) Setup** — Handing the Pass-2-hardened [`docs/egm/designs/xiao-rp2350-bargraph.md`](../../docs/egm/designs/xiao-rp2350-bargraph.md) spec to an implementation session.

> With four Blockers and seven Majors killed inside markdown, Phase 5 hands [`xiao-rp2350-bargraph.md`](../../docs/egm/designs/xiao-rp2350-bargraph.md) to an implementer so coding stays calm and boring.

**`[LIVE CAST · SLIDE 59 · S125 · 17:38–18:07 (29.0s)]`** · `Space / →` to slide, then `]` / `p` — **LIVE CAST (`29.0s`)** on [`prototypes/05-implement-cast-player.html`](../../prototypes/05-implement-cast-player.html).
- *Cast Cue:* Switch to [`prototypes/05-implement-cast-player.html`](../../prototypes/05-implement-cast-player.html). Jump to Stop Point `3. Prompt: Implement Firmware & Verify Build` (`00:04` warped) and press `p` through **Seg 04** (`29.0s @ 27x`, compressing `13m 04s` raw -> auto-pauses at `4. Firmware Built & Tested`).

> Here is the prompt handing that Pass-2-hardened spec straight to implementation. Notice the behavioural effect compared to Vanilla: there are zero architectural guesses left to improvise mid-flight. At twenty-seven times speed across thirteen raw minutes, the session authors the custom board files, `rp23xx_bringup.c`, `bargraph_main.c`, and nine new directory contracts, fixes one macOS BSD `mktemp` flag in the host test harness, and runs `configure.sh` plus the NuttX ARM build—delivering a clean UF2 firmware binary on the very first compile attempt.

**`[SLIDE 60 · S126 · 18:07–18:15 (8.0s)]`** · `Space / →` — **First-Attempt Build Verdict** — **`nuttx/nuttx.uf2` (`347K` — `177,232 B` FLASH / `13,712 B` RAM): built cleanly on Attempt #1**.

> `nuttx/nuttx.uf2`, three hundred forty-seven kilobytes, compiled, linked, and packed cleanly on the very first attempt—zero firmware build failures.

**`[CLICK 1/1 · S127 · 18:15–18:22 (7.0s)]`** · `Space / →` — **Telemetry Footnote** — `0` firmware retries (`1` BSD `mktemp` fix in `Makefile` for macOS host `make test` at Step 74; `0` `xmllint` errors).

> Our lone build hiccup was a macOS BSD `mktemp` flag inside the host test target—zero NuttX Kconfig drops or compiler errors.

**`[SLIDE 61 · S128 · 18:22–18:30 (8.0s)]`** · `Space / →` — **Rolling the Map Forward** — [`docs/egm/xiao-rp2350-bargraph/README.md#L72-L76`](../../docs/egm/xiao-rp2350-bargraph/README.md#L72-L76) (`9` new 5-section contracts generated for the new code).

> Better yet, Phase 5 rolls the map forward—adding nine new five-section `README.md` contracts covering every directory we introduced.

**`[CLICK 1/1 · S129 · 18:30–18:37 (7.0s)]`** · `Space / →` — **Honest Contract Boundary (`CA-02`)** — Explicitly documents the `CONFIG_BUILD_FLAT=y` assumption permitting `apps/` to call kernel `up_*_irq()`.

> Including Pass 2 `CA-02`: explicitly documenting why flat-build `bargraphd` calls kernel `up_enable_irq` directly without modifying upstream submodules.

**`[SLIDE 62 · S130 · 18:37–18:45 (8.0s)]`** · `Space / →` — **Wiring & SVG Flash** — [`docs/egm/BREADBOARD_WIRING.md`](../../docs/egm/BREADBOARD_WIRING.md) + [`docs/egm/breadboard-wiring.svg`](../../docs/egm/breadboard-wiring.svg) (`330 Ω` resistors, `~3.94 mA`/LED, `100 nF` wiper bypass cap `C1`).

> And it generates a consistent wiring guide and SVG diagram: `330`-ohm resistors, `23.6` milliamps total, plus a `100`-nanofarad wiper capacitor.

**`[CLICK 1/2 · S131 · 18:45–18:53 (8.0s)]`** · `Space / →` — **Foreshadowing Seed** — Notice the XIAO RP2350 seated in `Cols E & F` in that SVG? Keep that mental bookmark for Act V.

> Keep one eye on the XIAO seated in columns `E` and `F` right there—we will measure that with a ruler in Act V.

**`[CLICK 2/2 · S132 · 18:53–19:00 (7.0s)]`** · `Space / →` — **Act IV Pivot** — The agent nailed the architecture; now comes the harder question: *do you understand the C you're about to ship?*

> The agent nailed the firmware; now comes the harder question—do you actually understand the eight hundred lines of C you're about to ship?

---

## Act IV — Rolling Docs for the Human: The `/teach-code` Codelab (`19:00–25:00`, `6.0 min`, Beats `17–22`, `S133–S176`)

### Beat 17 (`19:00–20:00`, `60.0s`, `8` steps) — "Humans Own the Outcomes": Comprehension Debt & Matt Pocock's `/teach`

**`[SLIDE 63 · S133 · 19:00–19:08 (8.0s)]`** · `Space / →` — **Act IV Divider** — **Rolling Docs for the Human: Paying Down Comprehension Debt**.

> Part 2 of "Roll Your Own Docs" flips the camera straight away from the coding agents and back onto us engineers.

**`[SLIDE 64 · S134 · 19:08–19:15 (7.0s)]`** · `Space / →` — **Rensin's Third Role ([`/Users/brett/.gemini/config/plugins/egm/docs/index.md#L61-L75`](/Users/brett/.gemini/config/plugins/egm/docs/index.md#L61-L75))** — **"Humans Own the Outcomes."**

> Dave Rensin pairs the Elephant and Goldfish with a blunt third rule for the person holding the keyboard: humans own the outcomes.

**`[CLICK 1/1 · S135 · 19:15–19:23 (8.0s)]`** · `Space / →` — **Verbatim Warning** — *"Sorry! My AI tool just pushed broken code to production..."* isn't an excuse—it was you that pushed it.

> As Dave writes, "Sorry, my AI tool just pushed broken code to production" will not fly—it was still you who pushed it.

**`[SLIDE 65 · S136 · 19:23–19:30 (7.0s)]`** · `Space / →` — **The Comprehension Gap** — Our Elephant and Goldfish understand `rp23xx_adc.c`, `RP2350-E9`, and NVIC IRQ gating (`10 / 10` primary pass).

> Right now, our repo contracts and Goldfish reports understand `rp23xx_adc.c`, `RP2350-E9` leakage, and NVIC gating cold—scoring ten out of ten.

**`[CLICK 1/1 · S137 · 19:30–19:38 (8.0s)]`** · `Space / →` — **The Human Confession** — *You* didn't write those 800 lines of C. When hardware misbehaves in six months, how fast can you reason about them?

> Yet I did not write those eight hundred lines of C myself. Come back in six months and that becomes silent comprehension debt.

**`[SLIDE 66 · S138 · 19:38–19:45 (7.0s)]`** · `Space / →` — **Credit Where Due: Matt Pocock's `/teach`** — [`.agents/skills/teach/SKILL.md#L12-L53, L110`](../../.agents/skills/teach/SKILL.md#L12-L53).

> To pay down that debt deliberately, I started from Matt Pocock's brilliant open-source `/teach` skill instead of inventing pedagogy from scratch.

**`[CLICK 1/2 · S139 · 19:45–19:53 (8.0s)]`** · `Space / →` — **Cognitive Science Backbone** — Zone of Proximal Development (`learning-records/`), Edward Tufte layout, and retrieval quizzes (*Fluency vs. Storage Strength*).

> `/teach` builds durable storage strength over surface fluency using Zone of Proximal Development records, Edward Tufte layout, and active retrieval quizzes.

**`[CLICK 2/2 · S140 · 19:53–20:00 (7.0s)]`** · `Space / →` — **Why Static Prose Breaks for Firmware** — C listings inside tutorials silently drift out of sync with real compiling C99 code.

> One catch with teaching embedded C, though: pasted code blocks inside static HTML tutorials quickly drift away from code that actually compiles.

### Beat 18 (`20:00–21:00`, `60.0s`, `8` steps) — Evolving `/teach` -> `/teach-code` via `/agy-customizations`

**`[SLIDE 67 · S141 · 20:00–20:08 (8.0s)]`** · `Space / →` — **Skill Diff Header** — [`.agents/skills/teach/SKILL.md`](../../.agents/skills/teach/SKILL.md) -> [`.agents/skills/teach-code/SKILL.md`](../../.agents/skills/teach-code/SKILL.md) (`#L17-L32, L56-L110`) via `/agy-customizations`.

> So over repeated real projects I gently shaped Matt's `/teach` skill into `/teach-code` using Antigravity's built-in `/agy-customizations` skill.

**`[CLICK 1/2 · S142 · 20:08–20:15 (7.0s)]`** · `Space / →` — **Kept Intact (Structure)** — `MISSION.md`, `RESOURCES.md`, and ADR-style `learning-records/*.md` diagnosing the learner's starting point.

> We kept the soul of `/teach` untouched: `MISSION.md`, verified `RESOURCES.md`, and ADR-style learning records tracking the learner's starting edge.

**`[CLICK 2/2 · S143 · 20:15–20:23 (8.0s)]`** · `Space / →` — **Kept Intact (Ergonomics)** — Printable `reference/*` sheets, Tufte right-margin sidenotes, and equal-word-count multiple-choice quizzes.

> Same printable reference sheets, same distraction-free Tufte margin notes, and Matt's rule forcing every quiz option to share identical word counts.

**`[SLIDE 68 · S144 · 20:23–20:30 (7.0s)]`** · `Space / →` — **What `/teach-code` Adds** — Compiler-grade verification gates around every lesson and listing.

> What `/teach-code` adds—inspired by pragmatic engineering hygiene—is three deterministic compiler-like verification gates wrapped around every lesson.

**`[CLICK 1/3 · S145 · 20:30–20:38 (8.0s)]`** · `Space / →` — **Gate #1 (`build.py --check`)** — Validates rendered HTML, Mermaid diagrams, Prism code blocks, and structure in [`docs/egm/tutorial/build.py`](../../docs/egm/tutorial/build.py).

> Gate one: lessons are authored in clean Markdown and compiled by `build.py --check`, checking links, quiz lengths, and HTML structure.

**`[CLICK 2/3 · S146 · 20:38–20:45 (7.0s)]`** · `Space / →` — **Gate #2 (Runnable `0001`–`0006` Apps)** — Every lesson ships a real C99 companion project (`docs/egm/tutorial/apps/0001-breadboard-circuit-and-current-budget/` through `0006-bargraphd-daemon-and-nsh-cli/`) passing `-std=c99 -Wall -Wextra -Werror -pedantic` + `make test`.

> Gate two: every lesson backs its prose with a runnable C99 project compiled with `-Wall -Wextra -Werror -pedantic` and `make test`.

**`[CLICK 3/3 · S147 · 20:45–20:53 (8.0s)]`** · `Space / →` — **Gate #3 (`sync_listing.py`)** — [`.agents/skills/teach-code/scripts/sync_listing.py`](../../.agents/skills/teach-code/scripts/sync_listing.py) guarantees byte-for-byte parity between lesson snippets and C files.

> Gate three: `sync_listing.py --check` guarantees every full code listing inside a lesson matches its compiling `.c` file byte for byte.

**`[SLIDE 69 · S148 · 20:53–21:00 (7.0s)]`** · `Space / →` — **Skill Evolution Summary** — Pedagogical soul from `/teach` + strict engineering hygiene from `/teach-code`.

> Keep the cognitive science, enforce compiler hygiene. Now let's run `/teach-code` against our verified firmware and walk through the course.

### Beat 19 (`21:00–22:15`, `75.0s`, `7` steps) — Phase 6 (`06-teach-code`) & Browser Tour Stop 1: Course Landing + ZPD Learning Record

**`[SLIDE 70 · S149 · 21:00–21:07.2 (7.2s)]`** · `Space / →` — **Phase 6 Prompt** — Pointing `/teach-code` at our `94` EGM files to weave a 6-lesson story-arc course in [`docs/egm/tutorial/`](../../docs/egm/tutorial/README.md).

> In Phase 6, I point `/teach-code` at our generated EGM contracts, Goldfish reviews, and firmware to build a six-lesson story arc.

**`[LIVE CAST · SLIDE 71 · S150 · 21:07.2–21:37.0 (29.8s)]`** · `Space / →` to slide, then `]` / `p` — **LIVE CAST (`29.8s`)** on [`prototypes/06-teach-code-cast-player.html`](../../prototypes/06-teach-code-cast-player.html).
- *Cast Cue:* Launch [`prototypes/06-teach-code-cast-player.html`](../../prototypes/06-teach-code-cast-player.html). Jump to Stop Point `7. Prompt: /teach-code Story Arc in ./tutorial/` (`00:10` warped) and press `p` through **Seg 08** (`29.8s @ 79x`, compressing `39m 11s` raw -> auto-pauses at `8. 6-Lesson Course Generated`).

> Here is the prompt asking `/teach-code` to weave our entire hardware and RTOS story arc inside `./tutorial/`. Watch the pipeline effect at seventy-nine times speed across thirty-nine raw minutes: it calculates the Zone of Proximal Development first, builds six progressive C99 companion apps from Lesson `0001` through `0006`—running `-Wall -Wextra -Werror -pedantic` and fixing test failures on each chapter—writes six matching Tufte lessons, crafts three interactive JavaScript simulators plus four reference sheets, and runs `sync_listing.py --check` and `build.py --check` before we switch straight over to Chrome.

**`[SLIDE 72 · S151 · 21:37.0–21:45.0 (8.0s)]`** · `Space / →` — **Browser Tour Stop 1 ([`docs/egm/tutorial/index.md`](../../docs/egm/tutorial/index.md) -> `docs/egm/tutorial/index.html`)** — Live course landing page with the 6-lesson Mermaid story-arc map.

> Flip straight into Chrome on `docs/egm/tutorial/index.html`—compiled from [`index.md`](../../docs/egm/tutorial/index.md)—opening on a six-lesson Mermaid story-arc map from breadboard to daemon.

**`[CLICK 1/1 · S152 · 21:45.0–21:52.0 (7.0s)]`** · `Space / →` — **Course Index Widgets** — `{{widget:lesson-cards}}` (`6` cumulative chapters) + `{{widget:reference-cards}}` (`4` printable sheets).

> Underneath sit auto-generated cards for all six hands-on chapters and four printable bench sheets ready for lookup beside real hardware.

**`[SLIDE 73 · S153 · 21:52.0–22:00.0 (8.0s)]`** · `Space / →` — **Course Mission ([`docs/egm/tutorial/MISSION.md`](../../docs/egm/tutorial/MISSION.md))** — Defines prerequisites, scope, and concrete hardware/RTOS outcomes.

> [`MISSION.md`](../../docs/egm/tutorial/MISSION.md) grounds every page: zero submodule edits, strict C99 host testing, and safe three-point-three-volt current budgets before touching silicon.

**`[SLIDE 74 · S154 · 22:00.0–22:07.0 (7.0s)]`** · `Space / →` — **ZPD Learning Record ([`docs/egm/tutorial/learning-records/0001-initial-architecture-and-driver-traps.md`](../../docs/egm/tutorial/learning-records/0001-initial-architecture-and-driver-traps.md))** — ADR explicitly modelling the learner's Zone of Proximal Development.

> Just like an engineering ADR, this learning record models the learner's Zone of Proximal Development so concepts stack one clean layer per chapter.

**`[CLICK 1/1 · S155 · 22:07.0–22:15.0 (8.0s)]`** · `Space / →` — **Stop 1 Payoff** — Instead of dumping `bargraph_main.c` on page one, Lesson 0001 starts with breadboard physics and ramps up step by step.

> Instead of dumping `bargraph_main.c` on page one, Lesson 0001 starts with Ohm's Law. Now let's jump to Stop 2 inside Lesson 0003.

### Beat 20 (`22:15–23:45`, `90.0s`, `11` steps) — Browser Tour Stop 2: Lesson 0003 `adc-ema-simulator` & Tufte Retrieval Practice

**`[SLIDE 75 · S156 · 22:15–22:23 (8.0s)]`** · `Space / →` — **Browser Tour Stop 2 (Lesson 0003)** — [`docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md`](../../docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md) rendered in clean Tufte layout.

> Remember Pass 1's `4x` Q4 EMA gain bug (`B-2`) and Pass 2's stale sample (`CA-01`)? Lesson 0003 turns both straight into tactile understanding.

**`[CLICK 1/1 · S157 · 22:23–22:31 (8.0s)]`** · `Space / →` — **Tufte Sidenotes** — Right-margin notes carry hardware register context beside the C code without breaking reading flow.

> Tufte right-margin sidenotes sit directly beside the prose—explaining why checking `am_data >= 0` catches hardware FIFO errors without tab hopping.

**`[SLIDE 76 · S158 · 22:31–22:39 (8.0s)]`** · `Space / →` — **Interactive Simulator (`{{widget:adc-ema-simulator}}`)** — Live JS sandbox inside [`docs/egm/tutorial/assets/course.js`](../../docs/egm/tutorial/assets/course.js).

> Rather than passively reading fixed-point bit-shift equations, `course.js` embeds a live `adc-ema-simulator` widget right inside the lesson.

**`[CLICK 1/2 · S159 · 22:39–22:47 (8.0s)]`** · `Space / →` — **Live Toggle #1 (Discard `msgs[0]`)** — Visually demonstrates how the latched previous-burst head sample pollutes a sharp step input.

> Flip off "Discard `msgs[0]`" on a step change, and you watch that twenty-millisecond-old hardware sample visibly drag the waveform behind reality.

**`[CLICK 2/2 · S160 · 22:47–22:55 (8.0s)]`** · `Space / →` — **Live Toggle #2 (`B-2`: `>> 2` vs. `(ema_q4 + 8) >> 4`)** — Shows the curve slamming to `100%` at quarter-turn pot travel vs. smooth unity gain.

> Switch `B-2` back to `>> 2`, and the curve pegs to `4095` at a quarter turn—showing why Goldfish `B-2` mattered immediately.

**`[SLIDE 77 · S161 · 22:55–23:03 (8.0s)]`** · `Space / →` — **Fill-in-the-Blank Exercise (`:::exercise`)** — Interactive `keep-markup` Prism inputs right inside the C listing.

> Right underneath sits an inline `keep-markup` C exercise forcing you to type `BARGRAPH_ADC_SHIFT`, shift `2`, and rounding offset `8u` yourself.

**`[SLIDE 78 · S162 · 23:03–23:11 (8.0s)]`** · `Space / →` — **Equal-Word-Count Quiz (`:::quiz`)** — 4 options, strictly **`10` words per option** testing why `msgs[0]` is discarded.

> Then `build.py` checks that all four quiz options contain ten words apiece—eliminating longest-answer guessing cues when testing `msgs[0]` staleness.

**`[CLICK 1/1 · S163 · 23:11–23:19 (8.0s)]`** · `Space / →` — **Cognitive Science Payoff** — Forcing recall builds durable *Storage Strength* instead of passive skimming *Fluency*.

> Answering why sample zero is stale locks that hardware invariant into storage strength instead of letting your eyes glide past prose.

**`[SLIDE 79 · S164 · 23:19–23:27 (8.0s)]`** · `Space / →` — **Simulator Flash #1 (Lesson 0001)** — `{{widget:breadboard-simulator}}` ([`docs/egm/tutorial/lessons/0001-breadboard-circuit-and-current-budget.md#L63`](../../docs/egm/tutorial/lessons/0001-breadboard-circuit-and-current-budget.md#L63)).

> Lesson 0001 includes a live breadboard simulator too—letting you drag the wiper to verify Thevenin source impedance and LED budgets.

**`[SLIDE 80 · S165 · 23:27–23:36 (9.0s)]`** · `Space / →` — **Simulator Flash #2 (Lesson 0002)** — `{{widget:pwm-segment-visualizer}}` ([`docs/egm/tutorial/lessons/0002-proportional-bargraph-math-and-pwm-scaling.md#L47`](../../docs/egm/tutorial/lessons/0002-proportional-bargraph-math-and-pwm-scaling.md#L47)).

> And Lesson 0002 models all six `0..65535` PWM channels live—making the `b16ONE` `65536`-to-zero truncation bug obvious at a glance.

**`[CLICK 1/1 · S166 · 23:36–23:45 (9.0s)]`** · `Space / →` — **Stop 2 Summary** — Three live browser simulators turn embedded math into tactile play.

> Three interactive JavaScript simulators turn dry embedded arithmetic into tactile play. Now let's jump forward to Lesson 0005 for the hardware traps.

### Beat 21 (`23:45–24:30`, `45.0s`, `6` steps) — Browser Tour Stop 3: Lesson 0005 (`START_ONCE` vs `RP23XX_ADC_DIV`) & Printable Bench Sheet

**`[SLIDE 81 · S167 · 23:45–23:53 (8.0s)]`** · `Space / →` — **Browser Tour Stop 3 (Lesson 0005)** — [`docs/egm/tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md#L58-L69`](../../docs/egm/tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md#L58-L69).

> Lesson 0005 distils the entire Act II and Act III hardware investigation into one focused chapter—explicitly teaching our `RP23XX_ADC_DIV` false start.

**`[CLICK 1/1 · S168 · 23:53–24:00 (7.0s)]`** · `Space / →` — **Traps Taught Explicitly** — Contrasts why `RP23XX_ADC_DIV` fails under `START_ONCE`, teaches NVIC `RP23XX_ADC_IRQ_FIFO` gating, and walks through `RP2350-E9` pad isolation.

> A great tutorial doesn't hide dead ends—it teaches why `ADC_DIV` ignores `START_ONCE`, so you never repeat that assumption elsewhere.

**`[SLIDE 82 · S169 · 24:00–24:08 (8.0s)]`** · `Space / →` — **Printable Bench Reference Sheet** — [`docs/egm/tutorial/reference/nuttx-rp2350-driver-traps.md#L15-L25`](../../docs/egm/tutorial/reference/nuttx-rp2350-driver-traps.md#L15-L25).

> Beside the six lessons sit four condensed reference cards, including this one-page lookup table of all seven NuttX and RP2350 traps.

**`[CLICK 1/1 · S170 · 24:08–24:15 (7.0s)]`** · `Space / →` — **`@media print` Override** — Light-mode print CSS in [`docs/egm/tutorial/assets/style.css#L1031-L1032`](../../docs/egm/tutorial/assets/style.css#L1031-L1032) so dark-mode pages print crisp on A4 paper.

> Hit print, and `@media print` strips dark backgrounds into crisp A4 ink—ready to tape right above your soldering iron.

**`[SLIDE 83 · S171 · 24:15–24:23 (8.0s)]`** · `Space / →` — **Knowledge Loop Completed** — Agent contracts (`3,241`) -> Goldfish audit -> Human codelab + bench cheat sheet.

> That closes the knowledge loop between machine and human: what the Goldfish discovered in markdown now lives inside your own head.

**`[CLICK 1/1 · S172 · 24:23–24:30 (7.0s)]`** · `Space / →` — **Surprise Twist Cue** — And forcing Lesson 0006 to compile under strict C99 caught two real firmware bugs we missed in Phase 5!

> Better yet, forcing Lesson 0006 to compile real C99 under `-Werror -pedantic` actually caught two bugs inside our Phase 5 code!

### Beat 22 (`24:30–25:00`, `30.0s`, `4` steps) — Browser Tour Stop 4: Self-Corrections #1 & #2 — How Runnable C99 Labs Caught Two Phase 5 Bugs

**`[SLIDE 84 · S173 · 24:30–24:38 (8.0s)]`** · `Space / →` — **Browser Tour Stop 4 (Lesson 0006 Lab)** — [`docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L798`](../../docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L798).

> Lesson 0006 compiles the full `bargraphd` state machine on macOS pthreads under strict C99—turning teaching material into an uncompromising integration harness.

**`[CLICK 1/1 · S174 · 24:38–24:45 (7.0s)]`** · `Space / →` — **Self-Correction #1 (`Makefile` `printf` Duplication)** — Replaced Phase 5's [`docs/egm/xiao-rp2350-bargraph/Makefile#L40-L75`](../../docs/egm/xiao-rp2350-bargraph/Makefile#L40-L75) `/tmp/bargraph_test.XXXXXX` `printf` synthesis with genuine single-source C99 compilation.

> Remember Vanilla's clean `bargraph_logic.c`? In Phase 5, EGM `Makefile` synthesised `/tmp/bargraph_test.c` with `printf`—and `/teach-code` forced genuine single-source compilation.

**`[SLIDE 85 · S175 · 24:45–24:53 (8.0s)]`** · `Space / →` — **Self-Correction #2 (`bargraph test` `MANUAL`-Clobber Bug)** — Excerpt from trace [`docs/prompts-egm/24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md#L61-L65`](../../docs/prompts-egm/24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md#L61-L65).

> Worse, exercising `bargraphd` threads inside trace `24-0cc3a75f` hit two surprises: a startup tick-sync `pthread_cond_wait` block and a `MANUAL`-mode clobber bug.

**`[CODE-CLICK 1/1 · S176 · 24:53–25:00 (7.0s)]`** · `Space / →` — [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L557-L565, L841-L854`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L841-L854) paired with [`docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L749`](../../docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L749).

> `bargraph test` called `bargraph_start_daemon()`, which reset `MANUAL` to `SENSING` before saving `prev_mode`—proving human docs catch bugs too.

---

## Act V — The Honest Retro & Telemetry Scorecard (`25:00–30:00`, `5.0 min`, Beats `23–26`, `S177–S214`)

### Act V.a — Head-to-Head Telemetry & Shared Blind Spots (`25:00–28:30`, `3.5 min`, Beats `23–25`, `S177–S203`)

#### Beat 23 (`25:00–26:15`, `75.0s`, `10` steps) — Head-to-Head Telemetry & Dual-Tier Scorecard ([`docs/reports/vanilla-vs-egm-comparison.md`](../../docs/reports/vanilla-vs-egm-comparison.md))

**`[SLIDE 86 · S177 · 25:00–25:08 (8.0s)]`** · `Space / →` — **Act V Divider** — **The Honest Retro: Telemetry, Blind Spots & Skill Evolution**.

> Now let's put both runs side by side—telemetry, strict post-audit catches, and the physical blunder neither workflow spotted.

**`[SLIDE 87 · S178 · 25:08–25:15 (7.0s)]`** · `Space / →` — **Telemetry Row 1 (Raw Time)** — Vanilla **`46m 40s`** vs. EGM **`154m 37s`** Phases 1–5 firmware (`196m 37s` total incl. `/teach-code`).

> Let's be completely upfront about wall-clock cost: Vanilla took forty-six minutes forty seconds; EGM firmware took one hundred fifty-four minutes.

**`[CLICK 1/3 · S179 · 25:15–25:23 (8.0s)]`** · `Space / →` — **Telemetry Row 2 (Sessions & Subagents)** — **`1` parent (`0` subagents)** vs. **`8` parents + `17` Goldfish subagents** (`25` traces).

> That reflects the architecture: one monolithic vibe thread versus eight parent sessions orchestrating seventeen zero-context Goldfish subagents across twenty-five traces.

**`[CLICK 2/3 · S180 · 25:23–25:30 (7.0s)]`** · `Space / →` — **Telemetry Row 3 (`README.md` Contract Reads)** — **`0` `nuttx`/`apps` contract reads** vs. **`209` `README.md` reads** (`1,005` `read_file` calls).

> Vanilla read zero `nuttx/` or `apps/` contracts across sixty-six peeks; EGM checked two hundred and nine directory contracts deliberately.

**`[CLICK 3/3 · S181 · 25:30–25:38 (8.0s)]`** · `Space / →` — **Telemetry Row 4 (Build Failures Before `.uf2`)** — **`1` Kconfig drop + `1` `xmllint` error** vs. **`0` firmware build failures** (`347K` `.uf2` first try).

> And upfront verification paid off at compile time: zero firmware build errors and a clean `347K` `.uf2` on attempt one.

**`[SLIDE 88 · S182 · 25:38–25:45 (7.0s)]`** · `Space / →` — **Dual-Tier Scorecard (Primary)** — Vanilla **`5 / 10`** vs. EGM **`10 / 10` Primary Pass**.

> Across the ten hardware and RTOS traps we tested, Vanilla scored five out of ten while EGM swept ten out of ten.

**`[CLICK 1/1 · S183 · 25:45–25:53 (8.0s)]`** · `Space / →` — **Dual-Tier Scorecard (Strict Post-Audit)** — Vanilla **`4 Pass / 4 Partial / 2 Fail`** vs. EGM **`6 Pass / 4 Partial / 0 Fail`**.

> Then we turned fresh read-only Goldfish loose after Phase 5—and under strict grading, EGM scored six Passes, four Partials, zero Fails.

**`[SLIDE 89 · S184 · 25:53–26:00 (7.0s)]`** · `Space / →` — **The Time Tradeoff** — `~3.3x` longer up front (`155m` vs. `47m`) for firmware alone.

> Would I spin up sixteen Goldfish for a one-line CSS tweak? Definitely not. Three-point-three times overhead belongs on unfamiliar legacy complexity.

**`[CLICK 1/2 · S185 · 26:00–26:08 (8.0s)]`** · `Space / →` — **What Those Extra `108` Minutes Bought** — Zero high-severity hardware fails, first-try `.uf2`, and a 6-lesson interactive codelab.

> When thirty years of RTOS C meets real silicon errata, spending two calm hours upfront saves weekends chasing ISR panics.

**`[CLICK 2/2 · S186 · 26:08–26:15 (7.0s)]`** · `Space / →` — **Now About Those `4` Strict Partials...** — Let's zoom straight into what the read-only Goldfish audit caught on our `10 / 10` firmware.

> Now about those four strict partial marks on our own ten-out-of-ten firmware—let's inspect every single one of them candidly.

#### Beat 24 (`26:15–27:30`, `75.0s`, `9` steps) — Self-Correction #3: What the Strict Post-Audit Caught on EGM (`6 Pass / 4 Partial / 0 Fail`)

**`[SLIDE 90 · S187 · 26:15–26:24 (9.0s)]`** · `Space / →` — **Post-Experiment Hostile Audit** — Fresh read-only Goldfish (`egm-codebase-details` + `egm-design`) graded the archived Phase 5 firmware cold.

> We did not cherry-pick a kind reviewer; we launched fresh read-only Goldfish auditors against the archived Phase 5 C files cold.

**`[SLIDE 91 · S188 · 26:24–26:32 (8.0s)]`** · `Space / →` — **Self-Correction #3 (Invariant `#6`)** — [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L389-L390, L529-L530`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L529-L530).

> Look at Self-Correction #3 under Invariant 6: remember how EGM kept `/dev/adc0` persistently open to avoid Vanilla's `40 Hz` shutdown race?

**`[CODE-CLICK 1/2 · S189 · 26:32–26:40 (8.0s)]`** · `Space / →` — [`docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L388-L390, L527-L530`](../../docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L527-L530) — Highlights `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` called **one line *before* `close(adc_fd)`** on exit and startup rollback!

> On exit and startup rollback, EGM unmasked `up_enable_irq()` one line *before* `close(adc_fd)`—briefly starting another conversion right before `remove_device()` runs!

**`[CODE-CLICK 2/2 · S190 · 26:40–26:49 (9.0s)]`** · `Space / →` — Fix paired with [`nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L176-L196`](../../nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L176-L196) — Call `(void)close(adc_fd)` *first* while masked, then `up_enable_irq()`.

> Swapping those two lines closes `adc_fd` while masked, then unmasks the pending IRQ so `g_active_count == 0` shuts down cleanly.

**`[SLIDE 92 · S191 · 26:49–26:57 (8.0s)]`** · `Space / →` — **Strict Partial #2 (`#7` Kconfig Hygiene)** — Inherited `EXAMPLES_HELLO`, `TESTING_GETPRIME`, and `TESTING_OSTEST` left enabled in `defconfig` + `CONFIG_RAMLOG_SYSLOG=y` omitted.

> Strict Partial 2 on Kconfig hygiene: inherited `EXAMPLES_HELLO` and `OSTEST` stayed compiled into `defconfig`, and `CONFIG_RAMLOG_SYSLOG=y` wasn't enabled for `dmesg`.

**`[CLICK 1/2 · S192 · 26:57–27:05 (8.0s)]`** · `Space / →` — **Strict Partial #3 (`#8` Daemon Lifecycle)** — Phase 5 `bargraph test` `MANUAL` reset (fixed in Lesson 0006), `CLOCK_REALTIME` vs. `date -s`, and missing `SIGTERM`.

> Strict Partial 3 flagged Phase 5's `bargraph test` `MANUAL`-reset bug, `CLOCK_REALTIME` sleep sensitivity if `date -s` runs, and no `SIGTERM` handler.

**`[CLICK 2/2 · S193 · 27:05–27:14 (9.0s)]`** · `Space / →` — **Strict Partial #4 (`#10` Test Duplication)** — Phase 5 `Makefile` `printf` `/tmp` duplication (fixed in Lesson 0006) + blocking `read()` without `poll()`.

> And Strict Partial 4 marked Phase 5's `Makefile` `printf` test duplication—the shortcut we resolved directly inside Lesson 0006's C99 harness.

**`[SLIDE 93 · S194 · 27:14–27:22 (8.0s)]`** · `Space / →` — **Honest Engineering Takeaway** — No workflow is magic: a cold audit caught a 1-line shutdown ordering bug inside `10 / 10` primary-pass code.

> No workflow is magic. Even after two Goldfish passes, turning auditors onto shipping C surfaced real one-line edge cases immediately.

**`[CLICK 1/1 · S195 · 27:22–27:30 (8.0s)]`** · `Space / →` — **Blind-Spot Transition** — Remember that XIAO breadboard SVG from Act III.d? Let's hold a ruler up to both diagrams.

> Now for Self-Correction #4—remember that XIAO RP2350 seated in `Cols E & F` earlier? Let's hold a physical ruler up.

#### Beat 25 (`27:30–28:30`, `60.0s`, `8` steps) — Self-Correction #4: The Shared `0.6"` XIAO Breadboard Straddle Blind Spot

**`[SLIDE 94 · S196 · 27:30–27:38 (8.0s)]`** · `Space / →` — **Self-Correction #4 Setup** — Physical hardware check: how wide is a Seeed Studio XIAO RP2350 across its two header rows?

> Simple mechanical bench question before anyone pushes pins into plastic: how wide is a Seeed Studio XIAO across its two header rows?

**`[CLICK 1/1 · S197 · 27:38–27:45 (7.0s)]`** · `Space / →` — **Physical Ground Truth** — **`0.6"` (`15.24 mm`, 6 breadboard holes)** straddling a standard `0.3"` centre ravine (`Cols D & H` or `C & G`).

> Zero-point-six inches—fifteen point two-four millimetres. Across a standard zero-point-three-inch centre ravine, that lands in Columns `D` & `H` or `C` & `G`.

**`[SLIDE 95 · S198 · 27:45–27:53 (8.0s)]`** · `Space / →` — **Vanilla's SVG ([`docs/vanilla/breadboard_circuit.svg#L67-L74`](../../docs/vanilla/breadboard_circuit.svg#L67-L74))** — Seats the XIAO horizontally across `c1..h1`.

> First let's check Vanilla's SVG diagram: it seated the XIAO horizontally spanning from row `c` down across the trench to `h`.

**`[CLICK 1/1 · S199 · 27:53–28:00 (7.0s)]`** · `Space / →` — **Vanilla Error (`0.7"`)** — Across the `0.3"` ravine, `c` to `h` spans **`0.7"`** (`1` row too wide—your pins won't drop into the holes!).

> Measure `c` to `h` on a real breadboard: zero-point-seven inches. One row too wide—the header pins literally won't enter the holes.

**`[SLIDE 96 · S200 · 28:00–28:08 (8.0s)]`** · `Space / →` — **EGM's SVG ([`docs/egm/breadboard-wiring.svg#L121, L280-L333`](../../docs/egm/breadboard-wiring.svg#L121))** — Seats the XIAO vertically in **`Cols E & F`**.

> Surely ten-out-of-ten EGM nailed the breadboard geometry? Zoom in on EGM's SVG: it placed the XIAO vertically in Columns `E` and `F`.

**`[CLICK 1/1 · S201 · 28:08–28:15 (7.0s)]`** · `Space / →` — **EGM Error (`0.3"`!)** — `Cols E & F` span **`0.3"`** (half-width!), visually disguised in SVG by stretching the centre trench from `35px` to `120px`!

> `E` to `F` spans zero-point-three inches—half the XIAO's width! Worse, EGM stretched the SVG trench to 120 pixels so it *looked* right.

**`[SLIDE 97 · S202 · 28:15–28:23 (8.0s)]`** · `Space / →` — **The Law of Contracts** — Where we had `README.md` contracts (NuttX + RP2350 silicon), EGM crushed Vanilla (`10/10` vs. `5/10`).

> And there is the Law of Contracts: inside our NuttX and silicon contract boundary, EGM crushed Vanilla ten to five.

**`[CLICK 1/1 · S203 · 28:23–28:30 (7.0s)]`** · `Space / →` — **The Blind-Spot Corollary** — Where *neither* run had a physical breadboard pitch contract, both hallucinated (`0.7"` and `0.3"` vs. `0.6"`).

> Where neither run had a breadboard pitch contract, both hallucinated. Now here is how you turn production warts into `SKILL.md` gates.

### Act V.b — Takeaway: Evolving Your Own `SKILL.md` Workflow (`28:30–30:00`, `1.5 min`, Beat `26`, `S204–S214`)

#### Beat 26 (`28:30–30:00`, `90.0s`, `11` steps) — Closing the Loop: Evolving Your Own `SKILL.md` Workflow (`25-98ffad8f` -> `build.py` -> `teach-code/SKILL.md`)

**`[SLIDE 98 · S204 · 28:30–28:38 (8.0s)]`** · `Space / →` — **Layer 3 Payoff** — **Closing the *"Roll Your Own Skills"* Loop** (post-recording trace [`docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md`](../../docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md)).

> Remember earlier I said Layer 3 is rolling your own skills? Even after Phase 6 finished recording, Chrome showed two tiny warts.

**`[CODE-CLICK 1/4 · S205 · 28:38–28:46 (8.0s)]`** · `Space / →` — [`docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md#L21-L22`](../../docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md#L21-L22) — Highlights Turn 1 inspecting the compiled codelab HTML.

> Opening the compiled tutorial pages in Chrome revealed two things `build.py` missed: raw dollar-sign TeX markup and `<2 id="breadboard-topology">` heading tags.

**`[CODE-CLICK 2/4 · S206 · 28:46–28:54 (8.0s)]`** · `Space / →` — [`docs/egm/tutorial/build.py#L22-L25, L736-L737`](../../docs/egm/tutorial/build.py#L736-L737) — Highlights `HEADING_ID_RE` dropping `h` in `<h\2>` + `$R_S$` raw TeX.

> One missing letter `h` inside the regex replacement emitted `<2 id="...">` tags, while `$R_S$` slipped through without a math renderer.

**`[CODE-CLICK 3/4 · S207 · 28:54–29:02 (8.0s)]`** · `Space / →` — [`docs/egm/tutorial/build.py#L37-L38, L761-L770`](../../docs/egm/tutorial/build.py#L761-L770) — Highlights permanent validators `MALFORMED_TAG_RE` + `UNRENDERED_TEX_RE`.

> Step one: never just patch the symptom—turn both catches into `MALFORMED_TAG_RE` and `UNRENDERED_TEX_RE` compiler gates inside `build.py --check`.

**`[CODE-CLICK 4/4 · S208 · 29:02–29:10 (8.0s)]`** · `Space / →` — [`.agents/skills/teach-code/SKILL.md#L93`](../../.agents/skills/teach-code/SKILL.md#L93) — Highlights upstreaming that rule back into `SKILL.md` via `/agy-customizations`.

> Step two: use `/agy-customizations` to fold that exact regex and Unicode invariant upstream into [`.agents/skills/teach-code/SKILL.md`](../../.agents/skills/teach-code/SKILL.md) line 93.

**`[SLIDE 99 · S209 · 29:10–29:18 (8.0s)]`** · `Space / →` — **The Skill Flywheel** — Every production wart becomes a deterministic gate for your next codebase.

> Treat your `SKILL.md` files like living engineering craft rather than static downloads—you never pay the same tuition twice.

**`[SLIDE 100 · S210 · 29:18–29:26 (8.0s)]`** · `Space / →` — **Closing Recap #1 — Roll Docs for the Agent** — Build bottom-up `README.md` contracts + Dave Rensin's Elephant & Goldfish workflow before writing C.

> Three habits to try on Monday morning. First: roll directory contracts and run Dave Rensin's Elephant and Goldfish protocol before coding.

**`[CLICK 1/2 · S211 · 29:26–29:34 (8.0s)]`** · `Space / →` — **Closing Recap #2 — Roll Docs for the Human** — Turn verified specs into runnable `/teach-code` codelabs because *you* own the outcome.

> Second: roll interactive `/teach-code` codelabs from verified specs so your team—not only the repository—holds the architecture.

**`[CLICK 2/2 · S212 · 29:34–29:42 (8.0s)]`** · `Space / →` — **Closing Recap #3 — Roll Your Own Skills** — Start from sharp community skills like Matt Pocock's `/teach` and gently refine them with `/agy-customizations`.

> Third: start from generous community craft like Matt Pocock's `/teach` and sharpen your own `SKILL.md` files every time a project bites.

**`[SLIDE 101 · S213 · 29:42–29:51 (9.0s)]`** · `Space / →` — **Closing Mantra** — *"Design with an elephant. Review with a goldfish. Own the outcome—and keep sharpening the skills."*

> Design with an elephant. Review with a goldfish. Own the outcome—and keep sharpening the skills that guide them both.

**`[SLIDE 102 · S214 · 29:51–30:00 (9.0s)]`** · `Space / →` — **Links & Resources** — `github.com/google/elephant-goldfish-model`, Dave Rensin's Medium essay, Matt Pocock's `/teach`, and the `nuttxspace` repo + [`docs/egm/tutorial/README.md`](../../docs/egm/tutorial/README.md).

> Thanks very much everyone—take an Elephant-Goldfish pass onto your gnarliest legacy codebase, and come grab me afterwards for questions!
