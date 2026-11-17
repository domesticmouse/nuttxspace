# Assemble and Verify the Presentation Specification at spec.md

Type: task
Status: resolved
Blocked by: 06

## Question

How should all decisions, timings, slide templates, cast stop points, and spoken notes be compiled into the final authoritative `.scratch/egm-nuttx-casts-presentation/spec.md`, verifying that every timing budget adds up, every cast reference is valid, and the presentation strictly adheres to scope?

## Answer

Compiled and verified the complete, authoritative presentation specification at [`spec.md`](../spec.md):

1. **Unified 33-Slide Specification & Verbatim `bjm-voice` Script (`S01`–`S33` across 4 Modular Act Files)**:
   - Consolidated all decisions from Tickets `01`–`06` (`cast-curation-prototype.md`, `slide-structure-prototype.md`, and `speaker-script-bjm-voice.md`) into a single self-contained specification at [`.scratch/egm-nuttx-casts-presentation/spec.md`](../spec.md).
   - Every slide (`S01`–`S33`) specifies its modular file (`01-hook-and-setup.md`, `02-vanilla-foil.md`, `03-egm-journey.md`, `04-teach-code-finale.md`), Slidev layout (`center`, `default`, `hardware-callout`, `skill-breakdown`, `cast-full`), exact duration budget, visual elements (`[On Load]` + `[v-click 1..N]`), and verbatim Australian English (`bjm-voice`) script.

2. **Verified 30-Minute Timing Budget (`29m 51s` / `1,791s` Total · `9s` Buffer under `30m 00s`)**:
   - **Act I (`01-hook-and-setup.md` · `S01`–`S05`)**: `5` non-cast slides = `3m 15s` (`195s`).
   - **Act II (`02-vanilla-foil.md` · `S06`–`S09`)**: `3` non-cast slides (`1m 30s`) + `1` live cast (`vanilla-vibe.cast`, `1m 51s`) = `3m 21s` (`201s`).
   - **Act III (`03-egm-journey.md` · `S10`–`S26`)**: `12` non-cast slides (`5m 20s`) + `5` live casts (`01-bootstrap` through `05-implement`, `11m 47s`) = `17m 07s` (`1,027s`).
   - **Act IV (`04-teach-code-finale.md` · `S27`–`S33`)**: `6` non-cast/showcase slides (`4m 04s`, including `2m 30s` across `S29`–`S31` Finale Showcase) + `1` live cast (`06-teach-code.cast`, `2m 04s`) = `6m 08s` (`368s`).
   - **Totals**: `26` non-cast slides (`849s` / `14m 09s`) + `7` live `.cast` slides (`624.0s` / `10m 24s` warped play + `318.0s` / `5m 18s` across `53` stops = `942s` / `15m 42s`) = **`1,791s` (`29m 51s`)**.

3. **Verified On-Disk Assets, Cast References & Scope Guardrails**:
   - Verified all `7` `.cast` files in `slides/public/casts/`, all `7` HTML players in `prototypes/`, both cropped breadboard SVGs in `slides/public/images/` (`breadboard-vanilla-crop-ch.svg` and `breadboard-wiring-crop-ef.svg`), and the compiled 6-lesson interactive HTML course in `docs/egm/tutorial/` and `slides/public/tutorial/index.html`.
   - Verified strict adherence to scope: zero low-level C driver lines or register code on slides, zero asciinema/`pyte` internal mechanics, low-density progressive `v-click` reveals (`~2.1` beats/slide), and the 3-slide Finale Showcase (`S29`–`S31`) touring `./tutorial/index.html`.
