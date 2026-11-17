# Explaining EGM and Antigravity Mechanics on Stage

Type: grilling
Status: resolved
Blocked by: 02

## Question

How should the visual slides and spoken commentary explain the Elephant-Goldfish Model (the Elephant holding context memory via README contracts, the Goldfish subagents auditing without sycophancy, and the Human owning outcomes) in the context of Antigravity's skills and workflows, without resorting to low-level C code or generic AI slop?

## Answer

1. **Low-Density Visual Rule & Progressive Unpacking (Triad Anchor + Just-in-Time Setup)**:
   - Avoid information-dense slides completely: every concept uses **multiple single-idea slides** or **progressive `v-click` section reveals** so only the exact beat being spoken appears on screen.
   - Open Act III (immediately after the Vanilla Vibe `5/10` debrief) with a low-density **EGM Triad Anchor** contrasting Vanilla Vibe (`0` `README.md` reads, `0` subagents) against the three EGM roles (**The Elephant**, **The Human**, **The Goldfish**), then unpack each role just-in-time right before the recording that demonstrates it live.

2. **Visualising "The Elephant" (`README.md` Contracts & Antigravity Discovery — Before `01-bootstrap` & `02-brainstorm`)**:
   - **Beat 1 — From Blind Grep to a Navigable Map**: Contrast Vanilla Vibe (`0` `README.md` reads, blind file grepping) with EGM's `3,241` directory-level `README.md` contracts (`2,607` in `nuttx/`, `634` in `apps/`) navigated top-down (`209` contract reads across the run).
   - **Beat 2 — Stylized Contract Anatomy (3 Progressive Clicks, No Wall of Text)**: Show a clean card for `nuttx/arch/arm/src/rp23xx` revealing one badge per click:
     1. *Scope & Public Interfaces* (`rp23xx_adc.c`, `rp23xx_pwm.c`).
     2. *Hardware & RTOS Invariants* in plain English (e.g., *"ADC init resets all 4 pins `GPIO 26–29` and leaves digital input buffer `IE=1` enabled"*).
     3. *Child Directory Routing* (how the agent zooms only where needed).
   - **Beat 3 — Keeping the Elephant Honest (`/plugin` $\rightarrow$ `/egm-bootstrap` $\rightarrow$ `/egm-drift-report`)**: A 3-step pipeline slide before `01-bootstrap.cast` showing skill installation, root workspace blueprint generation, and git drift verification.

3. **Framing "The Human + The Elephant" & Planting the `RP23XX_ADC_DIV` False Start (`02-brainstorm` & `03-draft`)**:
   - **Before `02-brainstorm` (3 Click-Steps)**: Show the **Specification Funnel** (`3,241` READMEs $\rightarrow$ `49` read $\rightarrow$ `1-at-a-time` Human Q&A $\rightarrow$ `concepts_brief.md`), emphasising that the human makes product and hardware-boundary decisions (`D0/A0` + 6 LEDs, keeping USB-C `nsh>` alive, PWM brightness) rather than babysitting C syntax.
   - **Before `03-draft` (2 Click-Steps)**: Explain section-by-section drafting (`xiao-rp2350-bargraph.md`) and **foreshadow the false start at Stop 4 (`Q3: Technical Plan`)**, where both the Elephant and Human approve throttling the ADC via `CONFIG_RP23XX_ADC_DIV`—a plausible-sounding assumption that is actually a `500 kHz` IRQ storm landmine in `START_ONCE` mode.
   - **After `03-draft` Debrief (2 Click-Steps)**: Define **Author Bias / Self-Sycophancy**—why the session that wrote a spec will rubber-stamp its own blind spots, setting up the cliffhanger for **The Goldfish** (`04-review`).

4. **Visualising "The Goldfish" (`04-review`: Adversarial Swarm, `/egm-resolve`, and `/clear` Amnesia Loop)**:
   - **Pre-Cast Slide 1 — Anatomy of a Goldfish Swarm (3 Click-Steps)**:
     1. *Stateless & Uninvested*: Fresh subagent context window with zero memory of the brainstorm/draft struggle (zero sycophancy).
     2. *Multi-Angle Adversarial Personas*: Specialised lenses (*Codebase Details*, *Hostile Design Critic*, *Implementation Readiness*, *Zoom-Out Architect*) auditing the spec against `146` `README.md` contracts.
     3. *Synthesizer + Human Resolution (`/egm-resolve`)*: Deduplicating findings into ranked Blockers/Majors (`B-1..B-3`) for `interactive` human `accept`/`reject`.
   - **Pre-Cast Slide 2 — What to Watch in `04-review.cast` (2 Click-Steps)**: Watch Pass 1 (`B-1`) bust the `RP23XX_ADC_DIV` trap with NVIC IRQ gating, and watch `/clear` + `GoldfishContext` run Pass 2 on the updated spec.
   - **Post-Cast Debrief — First-Order vs. Second-Order Catch (2 Click-Steps)**: Show how Pass 1 fixed the hardware trap (`B-1`: 5-sample NVIC IRQ-gated burst) and Pass 2 caught the bug *introduced by Pass 1's fix* (`CA-01`: POSIX `read()` 5-byte short read on `/dev/adc0`) because `/clear` gave Pass 2 fresh eyes while `GoldfishContext` preserved prior decisions.

5. **Framing `05-implement`, the Candid Scorecard Callback, and the `06-teach-code` Lesson Showcase Finale**:
   - **`05-implement` ("When the Spec is Hardened, Coding is Boring")**: Because `4` Blockers and `7` Majors were caught in Markdown, `05-implement.cast` passes `100%` of host unit tests, builds `nuttx/nuttx.uf2` (`347K`) on the **first attempt (`0` retries)**, generates `docs/egm/breadboard-wiring.svg`, and writes `9` new `README.md` contracts in `xiao-rp2350-bargraph/`.
   - **Candid Scorecard Callback (2 Click-Steps)**:
     1. *Primary 10-Trap Scorecard*: `5 / 10` Vanilla vs. **`10 / 10` EGM** (`0` high-severity fails).
     2. *Candid `bjm-voice` Engineering Honesty*: Note that a strict post-implementation Goldfish audit still flagged `4` secondary polish items (`6 Pass / 4 Partial / 0 Fail`, e.g., `SIGTERM` cleanup and Makefile test scaffolding)—proving adversarial review is an ongoing habit, not magic.
   - **Act IV Finale — From "Docs for the AI" to "Docs for the Human" (`06-teach-code` + Live Lesson Showcase)**:
     - Frame Phases 1–5 as rolling our own docs **for the AI** (`README.md` contracts) and Phase 6 (`06-teach-code.cast` via `npx skills add domesticmouse/skills` + `/teach-code`) as rolling our own docs **for the Human**—concluding the talk by **showing off the generated 6-lesson Tufte-style interactive HTML course and printable reference sheets** (`docs/egm/tutorial/index.html`) live on screen.

