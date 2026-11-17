# Prototype the slide-by-slide outline and timing skeleton

Type: prototype
Label: wayfinder:prototype
Status: resolved
Blocked by: 02, 03, 04

## Question

Build a concrete, low-ceremony slide-by-slide outline prototype (`.scratch/roll-your-own-docs-presentation/outline-prototype.md`) mapping every slide's title, act, target timestamp, primary visual/artifact (cast player + stop-point range, doc excerpt, SVG diagram, or codelab widget), and one-line takeaway so we can react to the pacing and flow before writing the full presentation spec, building on the selections from [Select the hero codebase and hardware traps to feature on stage](./02-hero-technical-traps-selection.md), [Choreograph live cast-player recordings and stop points](./03-cast-player-choreography.md), and [Select generated EGM docs and teach-code codelab artifacts to showcase](./04-codelab-and-generated-docs-showcase.md).

## Answer

Locked the high-cadence, low-density outline prototype in [`outline-prototype.md`](../outline-prototype.md):

- **Total structure**: `214` stage steps (`S001`–`S214`) across `26` narrative beats (`Beat 01`–`Beat 26`) in `5` acts (`30:00` / `1,800.0s` total).
- **5–10s visual transition budget**: `208` low-density visual transitions across `1,618.4s` averaging `7.8s` per step (every non-cast step strictly `7.0s–9.0s`):
  - `96` `[Slide]` transitions (`102` total Slidev deck slides including `6` `[Live-cast]` slides)
  - `88` piece-by-piece `[v-click]` reveals
  - `24` Shiki `[Code-step]` line-highlight transitions
- **Cast-player choreography**: `6` `[Live-cast]` playback beats across `4` players totalling `181.6s` (`3m 02s`: `vanilla-vibe` `44.4s`, `04-review` `78.4s` across 3 beats, `05-implement` `29.0s`, `06-teach-code` `29.8s`), plus `3` freeze-frame players (`01-bootstrap`, `02-brainstorm`, `03-draft`).
- **Foundational lineage**: Grounded explicitly in Dave Rensin's [*"Elephants, Goldfish, and the New Golden Age of Software Engineering"*](https://drensin.medium.com/elephants-goldfish-and-the-new-golden-age-of-software-engineering-c33641a48874) (*"Design is the new code"*, *"Design with an elephant. Review with a goldfish."*, *Weeds Syndrome*, *Context Pollution*, Steps 1–4 Elephant, Steps 5–9 Goldfish, and *"Humans Own the Outcomes"*) alongside Matt Pocock's `/teach` → `/teach-code` skill evolution via `/agy-customizations`.
