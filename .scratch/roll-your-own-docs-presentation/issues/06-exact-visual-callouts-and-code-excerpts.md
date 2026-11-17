# Specify exact code snippets, report excerpts, and SVG callouts for each visual step

Type: grilling
Label: wayfinder:grilling
Status: resolved
Blocked by: 05

## Question

Across the `214` stage steps (`S001`–`S214`) locked in [Prototype the slide-by-slide outline and timing skeleton](./05-slide-by-slide-outline-prototype.md) ([`outline-prototype.md`](../outline-prototype.md)), which exact file line ranges, `4–8` line code excerpts, Shiki `[Code-step]` line-highlight progressions, Kconfig diffs, Pass 1/Pass 2 Goldfish report cards, telemetry table rows, and breadboard SVG (`docs/vanilla/breadboard_circuit.svg` vs. `docs/egm/breadboard-wiring.svg`) zoom/callout regions should appear on screen so every step stays low-density and grounded in verified repository artefacts?

## Answer

Locked all visual callouts, `3–8` line code excerpts, Goldfish finding cards, telemetry row reveals, and SVG `viewBox` crops across the `214` stage steps (`S001`–`S214`):

### 1. Acts II & III Shiki `[Code-step]` Progressions (`17` steps across `S032`–`S122`, each `3–8` lines)

1. **Act II Silicon Init (`S032`, `S033`, `S035`)**:
   - **`S032`**: `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L466-L483` (`my_reset()`) — 8-line excerpt showing `for (int i = 0; i < ADC_CHANNEL_COUNT; ++i)` iterating over `g_gpio_map[i]` (`GPIO 26–29`), highlighting `#L472-L476`.
   - **`S033`**: `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L478-L480` — advancing the Shiki highlight to `rp23xx_gpio_set_function(a_gpio, RP23XX_GPIO_FUNC_NULL);` (which sets `RP23XX_PADS_BANK0_GPIO_IE`, `IE=1`).
   - **`S035`**: `docs/vanilla/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L42-L54` contrasted beside `docs/egm/xiao-rp2350-bargraph/board/src/rp23xx_bringup.c#L67-L72` (`modreg32(RP23XX_PADS_BANK0_GPIO_OD, RP23XX_PADS_BANK0_GPIO_IE | RP23XX_PADS_BANK0_GPIO_PUE | RP23XX_PADS_BANK0_GPIO_PDE | RP23XX_PADS_BANK0_GPIO_ISO, RP23XX_PADS_BANK0_GPIO(BARGRAPH_ADC_GPIO))`), highlighting Vanilla omitting pad isolation on `GPIO 26`.

2. **Act II `500 kHz` `START_ONCE` Storm & `40 Hz` Teardown Race (`S038`, `S039`, `S042`, `S045`, `S046`)**:
   - **`S038`**: `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L232-L237` (`interrupt_handler()`) — 6-line excerpt highlighting unconditional `get_next_channel();`.
   - **`S039`**: `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L302-L308` (`get_next_channel()`) — 7-line excerpt highlighting `value += RP23XX_ADC_CS_START_ONCE; putreg32(value, RP23XX_ADC_CS);` (`2 µs`/sample = `500,000 IRQ/s`).
   - **`S042`**: `docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L87-L95` (`bargraph_read_adc_once()`) — 8-line excerpt highlighting `open("/dev/adc0")` → `read(fd, &msg, sizeof(msg))` → `close(fd)` at `40 Hz`.
   - **`S045`**: `nuttx/drivers/analog/adc.c#L187-L210` (`adc_close()`) — 8-line condensed excerpt highlighting `nxmutex_lock(&dev->ad_lock)` around `dev->ad_ops->ao_shutdown(dev)` with zero `enter_critical_section()`.
   - **`S046`**: `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L395-L407` (`remove_device()`) — 8-line condensed excerpt highlighting `g_first_device = PRIV(g_first_device)->next_device;` (`#L399`) seven lines before `g_active_count -= 1;` (`#L406`), where an in-flight `RP23XX_ADC_IRQ_FIFO` hits `ASSERT(next != g_current_channel)` (`#L314`).

3. **Act III Drift Report, `RP23XX_ADC_DIV` False Start, & Two-Pass Goldfish Rescue (`S070`, `S071`, `S089`, `S090`, `S108`, `S112`, `S113`, `S121`, `S122`)**:
   - **`S070`**: `docs/egm/reports/2026-11-16_DRIFT_REPORT.md#L9-L11` — 3-line excerpt flagging `nuttx/README.md#L79` omitting `OSINIT_HARDWARE`.
   - **`S071`**: `nuttx/sched/init/nx_start.c#L658-L664` — 6-line excerpt highlighting `g_nx_initstate = OSINIT_HARDWARE;` at `#L660`.
   - **`S089`**: `docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L33` — 4-line wrapped excerpt proposing `RP23XX_ADC_DIV = 0xFFFF00` to throttle conversions.
   - **`S090`**: `docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L242-L245` — 5-line excerpt highlighting `putreg32(0x00ffff00, RP23XX_ADC_DIV)` and `48,000,000 Hz / 65536 ≈ 732.42 Hz`.
   - **`S108`**: `docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L266-L293` (`bargraph_sample_adc()`) — 8-line condensed excerpt highlighting `ioctl(adc_fd, ANIOC_RESET_FIFO, 0)` → `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` → 5-sample burst → `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`.
   - **`S112`**: `docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L253-L254` — 4-line excerpt highlighting buggy `filtered = ema_q4 >> 2` (`[0, 16380]`, `4x` gain saturating at `25%` pot travel).
   - **`S113`**: `docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L661-L671` (`bargraph_filter_adc()`) — 8-line excerpt highlighting `*ema_q4 = (uint32_t)raw_avg << 4;` first-sample seeding and `filtered = (uint16_t)((*ema_q4 + 8u) >> 4);`.
   - **`S121`**: `nuttx/drivers/analog/adc.c#L270-L305` (`adc_read()`) — 7-line condensed excerpt highlighting `while (fifo->af_head == fifo->af_tail)` waking on `nxsem_wait(&fifo->af_sem)` and returning once a single `5`-byte `struct adc_msg_s` lands in the FIFO.
   - **`S122`**: `docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L270-L306` (`bargraph_sample_adc()`) — 8-line condensed excerpt highlighting `while (total_read < bytes_needed)` (`25` bytes, `EINTR`-safe) and `for (i = 1; i < BARGRAPH_ADC_READ_SAMPLES; i++)` discarding stale `msgs[0]`.

### 2. Acts IV & V Self-Correction `[Code-step]` Progressions (`7` steps: `S176`, `S189`–`S190`, `S205`–`S208`)

- **`S176`** (paired with `docs/egm/xiao-rp2350-bargraph/Makefile#L40-L75` `printf` `/tmp/bargraph_test.XXXXXX` duplication on `S174`): Phase 5's `docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L557-L565` and `#L841-L855` (where `bargraph_start_daemon()` at `#L843` resets `MANUAL` to `BARGRAPH_MODE_SENSING` before `prev_mode` is saved at `#L853`) paired with Lesson 0006's `docs/egm/tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L768` (`bargraph_wait_next_tick_locked(uint32_t prev_seq)` predicate + trace `docs/prompts-egm/24-0cc3a75f-build-a-complete-story-arc-tutorial-teaching.md#L61-L65` Steps `133–146`).
- **`S189`**: 6-line excerpt from `docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L388-L391` and `#L527-L531` highlighting `up_enable_irq(RP23XX_ADC_IRQ_FIFO);` called one line before `(void)close(adc_fd);`.
- **`S190`**: 6-line diff swapping those two lines so `(void)close(adc_fd);` runs first while `RP23XX_ADC_IRQ_FIFO` is masked, followed by `up_enable_irq(RP23XX_ADC_IRQ_FIFO);` (triggering `g_active_count == 0` teardown in `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L176-L196`).
- **`S205`**: 4-line excerpt from `docs/prompts-egm/25-98ffad8f-the-html-in-tutorial-contains-unrendered-tex.md#L21-L22` quoting Turn 1 (`unrendered TeX markup` and `<2 id="breadboard-topology">`).
- **`S206`**: 5-line excerpt from `docs/egm/tutorial/build.py#L22-L25` and `#L736-L737` highlighting `HEADING_ID_RE.sub(r'<h\2 id="\4">\3\5', rendered)` alongside raw `$R_S$` TeX leakage.
- **`S207`**: 8-line excerpt from `docs/egm/tutorial/build.py#L37-L38` and `#L761-L770` highlighting `MALFORMED_TAG_RE` (`<[0-9]\b|\{#[a-zA-Z0-9_-]+\}`) and `UNRENDERED_TEX_RE` (`\$[^$\n]+\$|\\(?:approx|pm|sim|Omega|text|le|ge|in)\b`).
- **`S208`**: 6-line wrapped excerpt from `.agents/skills/teach-code/SKILL.md#L93` highlighting the upstreamed rule (`always preserve the leading h in <h\2 id="\4">` + ban raw `$...$` TeX in `build.py --check`).

### 3. Goldfish Report Cards (`S100`, `S105`, `S111`, `S114`, `S120`) & Single-Row Telemetry/Scorecard Reveals (`S178`–`S183`, `S191`–`S193`)

- **3-Field Goldfish Cards** (Reviewer Persona Badge + Discrepancy Gist + Proposed Fix):
  - `S100` & `S105`: `docs/egm/reports/pass1/goldfish_review_report.md#L8-L16` (`[Issue B-1]`).
  - `S111`: `docs/egm/reports/pass1/goldfish_review_report.md#L20-L28` (`[Issue B-2]`).
  - `S114`–`S115`: `docs/egm/reports/pass1/goldfish_review_report.md#L32-L42` (`[Issue B-3]`).
  - `S120`: `docs/egm/reports/pass2/goldfish_review_report.md#L9-L17` (`[Issue 1 (CA-01)]`).
- **Row-by-Row Progressive `[v-click]` Telemetry & Scorecard Reveals** (`docs/reports/vanilla-vs-egm-comparison.md#L20-L41`):
  - `S178` Raw Duration (`46m 40s` vs. `154m 37s` Phases 1–5 / `196m 37s` total).
  - `S179` Sessions & Subagents (`1 + 0` vs. `8 + 17`, `25` traces).
  - `S180` `README.md` Contract Reads (`0` vs. `209` `README.md` reads across `1,005` `read_file` calls).
  - `S181` Build Failures Before Clean `.uf2` (`1` Kconfig drop + `1` `xmllint` error vs. `0` firmware build failures; `1` macOS `mktemp` host-test fix at `Step 74`).
  - `S182` Primary 10-Point Scorecard (`5 / 10` vs. `10 / 10`).
  - `S183` Strict Post-Audit Verdict (`4 Pass / 4 Partial / 2 Fail` vs. `6 Pass / 4 Partial / 0 Fail`), with `S191`–`S193` revealing strict partials `#7`, `#8`, and `#10` one badge at a time.

### 4. Breadboard SVG `viewBox` Crops (`S002`–`S003`, `S130`–`S131`, `S196`–`S201`) & Contract/Codelab Crops (`S060`–`S068`, `S079`–`S083`, `S128`–`S129`, `S151`–`S170`)

- **Breadboard SVG `viewBox` Regions**:
  - `S002`–`S003` & `S130`: Full `viewBox="0 0 1160 1120"` overview of `docs/egm/breadboard-wiring.svg` (`10 kΩ` pot on `Rows 1–3`, optional `100 nF` bypass cap on `Row 5B` at `x=162..190, y=341..359`, six `330 Ω` `~3.94 mA`/LED rows on `Rows 14–19`).
  - `S131` & `S200`–`S201`: Zoomed `viewBox="150 130 500 500"` crop of `docs/egm/breadboard-wiring.svg#L121, L280-L333` (`Cols E & F` at `x=340`/`460`, `0.3"` physical span disguised by `120px` stretched trench vs. `35px` pitch).
  - `S198`–`S199`: Zoomed `viewBox="120 180 320 320"` crop of `docs/vanilla/breadboard_circuit.svg#L120-L127, L225-L250` (`c1..c7` at `y=260` and `h1..h7` at `y=420`, `0.7"` span vs. physical `0.6"` / `15.24 mm` XIAO pitch).
- **Contract & Design Brief Callouts**:
  - `S060`–`S064`: Progressive callouts on `nuttx/arch/arm/src/rp23xx/README.md#L1-L71` (`Overview`, `Subdirectories & Boundaries`, `Interface`, `Invariants`, `Verification`).
  - `S068`: `docs/egm/README.md#L7-L18` (`3`-directory boundary + `Global Invariants`).
  - `S079`–`S083`: `docs/egm/designs/concepts_brief.md#L16-L35` (`7` hardware/driver obstacles revealed across four `[v-click]` steps).
  - `S128`–`S129`: `docs/egm/xiao-rp2350-bargraph/README.md#L72-L76` (`Intentional CONFIG_BUILD_FLAT=y Boundary Exceptions` invariant `#L75`, Pass 2 `CA-02`).
- **Act IV `/teach-code` Browser & Markdown Crops (`S151`–`S170`)**:
  - **Stop 1 (`S151`–`S155`)**: `docs/egm/tutorial/index.md` (`index.html` Mermaid 6-lesson story arc, `{{widget:lesson-cards}}`, `{{widget:reference-cards}}`), `docs/egm/tutorial/MISSION.md`, and `docs/egm/tutorial/learning-records/0001-initial-architecture-and-driver-traps.md`.
  - **Stop 2 (`S156`–`S165`)**: `docs/egm/tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.md#L18-L29, L42, L399-L412, L434-L452` (Section 1 + Tufte sidenote `#1`, live `{{widget:adc-ema-simulator}}`, `:::exercise id="ex-0003"`, 10-word `:::quiz id="quiz-0003"` Q2) plus visual flashes of `{{widget:breadboard-simulator}}` (`lessons/0001-breadboard-circuit-and-current-budget.md#L63`) and `{{widget:pwm-segment-visualizer}}` (`lessons/0002-proportional-bargraph-math-and-pwm-scaling.md#L47`).
  - **Stop 3 (`S167`–`S170`)**: `docs/egm/tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md#L58-L69` (`Traps #2, #6, and #7`) paired with `docs/egm/tutorial/reference/nuttx-rp2350-driver-traps.md#L15-L25` and `@media print` light-mode override in `docs/egm/tutorial/assets/style.css`.
