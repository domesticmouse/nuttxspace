# LinkedIn Post Specification — ChurConf Presentation Thank-You

- **Talk**: *"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"*
- **Event**: ChurConf
- **Source Deck & Spec**:
  - [Authoritative Presentation Spec](../egm-nuttx-casts-presentation/spec.md)
  - [Live Slidev Deck Source](../../slides/slides.md)
- **Voice & Style Guide**: [`bjm-voice`](../../.agents/skills/bjm-voice/SKILL.md) — Australian English spelling (*organisers*, *realise*, *behaviour*, *organisation*, *colour*, *amongst*), experienced peer & curious explorer tone, candid humility, concrete links, and zero corporate AI clichés.

---

## Option 1: Primary LinkedIn Post (Self-Contained with Inline Links — Recommended)

Copy-paste ready block for LinkedIn:

```text
Huge thank you to the ChurConf community and organisers for an attentive, lively room during "Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"!

Putting 30 minutes of live terminal recordings on screen—watching Google Antigravity try (and initially stumble) to tame Apache NuttX's 3,241 directories and wire a Seeed Studio XIAO RP2350 potentiometer and 6-LED bar graph—felt a bit like debugging hardware in public. Your reactions made it completely worth it.

Two comments from the room are still rattling around in my head:

1. From an attendee afterwards:
"I wish I'd seen your talk a week ago, it would have saved me a lot of time."
That kind of honest validation makes weeks of fighting breadboards and RTOS configs feel well spent.

2. From the Q&A:
"EGM helps with creation of the specification, but how does EGM handle the AI generated code bloat? What encourages developers to create abstractions in the codebase to control the complexity explosion?"

And here is my candid answer: I don't have a complete answer for that yet.

Inside the Elephant-Goldfish Model (EGM), the zoom-out Goldfish reviewer pushes back on YAGNI and checks for existing reuse at specification time—and our directory-level README.md contracts help keep module boundaries visible. Once the code starts growing across multiple implementation passes, though, nudging an agent toward clean, deep abstractions instead of linear code bloat and shifting behaviour across files is still a hard problem.

As I wrote in my post from a few days ago, I've only been learning EGM for a week myself. None of us have the finished playbook yet—we are all learning how to build with these workflows in real time. A conference room full of sharp engineers pressing on the exact gap you haven't solved is the best part of speaking at events like ChurConf.

If you've found reliable habits—codebase organisation patterns, review prompts, or refactoring loops—that keep AI-assisted codebases compact and well-factored past week one, I'd genuinely love to hear what's working for you in the comments.

For everyone who asked for the slides and the full experiment artefacts:

- Interactive Slidev deck (including all 7 asciinema recordings & 53 stop points): https://domesticmouse.github.io/nuttxspace/slides/
- The generated 6-lesson /teach-code interactive HTML course (with JS hardware & EMA simulators): https://domesticmouse.github.io/nuttxspace/egm/tutorial/index.html
- Full experiment hub, Vanilla vs. EGM Goldfish audit reports & 47 session traces: https://domesticmouse.github.io/nuttxspace/

A big hat-tip to Dave Rensin for the Elephant-Goldfish Model (EGM) and to Matt Pocock for the /teach skill inspiration behind /teach-code.

Thanks again, ChurConf—can't wait to compare notes on what we all learn next!

#ChurConf #EmbeddedSystems #ApacheNuttX #RP2350 #AI #SoftwareEngineering #DeveloperExperience
```

### First Comment (Previous Post Link)

```text
Here is the earlier 1-week EGM learning post I mentioned above: https://lnkd.in/p/gmVX-B74
```

---

## Option 2: Presentation Deck Link in Main Body + Supporting Links in First Comment

Use this option if you prefer keeping only the Slidev presentation deck URL in the main LinkedIn post body (so LinkedIn generates the preview card for the deck rather than quoting the earlier LinkedIn post) and dropping the supporting links into the first comment immediately after posting.

### Main Post Body

```text
Huge thank you to the ChurConf community and organisers for an attentive, lively room during "Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"!

Putting 30 minutes of live terminal recordings on screen—watching Google Antigravity try (and initially stumble) to tame Apache NuttX's 3,241 directories and wire a Seeed Studio XIAO RP2350 potentiometer and 6-LED bar graph—felt a bit like debugging hardware in public. Your questions made it completely worth it.

Two reactions from the room have stayed with me:

1. From an attendee afterwards:
"I wish I'd seen your talk a week ago, it would have saved me a lot of time."

2. From the Q&A:
"EGM helps with creation of the specification, but how does EGM handle the AI generated code bloat? What encourages developers to create abstractions in the codebase to control the complexity explosion?"

Truthfully, I don't have a complete answer for that yet.

EGM's zoom-out Goldfish reviewer pushes back on YAGNI and checks for reuse before implementation begins, and directory-level README.md contracts help keep module boundaries visible. Actually stopping an agent from piling up shallow helpers and scattered behaviour as a codebase evolves over days and weeks, though, is still an open challenge.

As I shared in my earlier post, I've only been learning EGM for a week myself. We are all figuring this out in real time—and having a room full of sharp engineers ask the exact architectural question you haven't cracked yet is why conferences like ChurConf matter so much.

How are you nudging coding agents toward clean abstractions instead of linear code bloat? Would love to compare notes in the comments.

Massive thanks to Dave Rensin for the Elephant-Goldfish Model and Matt Pocock for the /teach skill inspiration behind /teach-code.

Interactive Slidev deck (all 7 terminal casts included): https://domesticmouse.github.io/nuttxspace/slides/

Links to the generated /teach-code course, the full experiment hub & GitHub repo, and my earlier 1-week EGM post are in the first comment below!

#ChurConf #EmbeddedSystems #ApacheNuttX #RP2350 #AI #SoftwareEngineering #DeveloperExperience
```

### Ready-to-Paste First Comment

```text
Here are the supporting links from the ChurConf talk ("Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"):

- Generated 6-lesson /teach-code interactive HTML course (Edward Tufte layout + live JS hardware simulators): https://domesticmouse.github.io/nuttxspace/egm/tutorial/index.html
- Documentation & comparative experiment hub (Vanilla Vibe vs. EGM Goldfish reports & 47 exported traces): https://domesticmouse.github.io/nuttxspace/
- Source repository on GitHub: https://github.com/domesticmouse/nuttxspace
- My earlier 1-week EGM learning post: https://lnkd.in/p/gmVX-B74
```

---

## Visual Attachment & Carousel Recommendations

Attach visual context so the post stands out in the feed and gives attendees an immediate visual cue from the talk.

### Single-Image Attachment (Quickest)
- **Screenshot of Slide `S01` (Title Slide)** from [`slides/acts/01-hook-and-setup.md`](../../slides/acts/01-hook-and-setup.md#L6-L22), **or**
- **Screenshot of Slide `S26` (`Candid Scorecard Callback: 5 / 10 vs. 10 / 10`)** from [`slides/acts/03-egm-journey.md`](../../slides/acts/03-egm-journey.md#L486-L513), which captures the candid engineering contrast (*"No Unblemished Triumph Narratives"*) at a glance.

### 3-Image Carousel / Multi-Photo Post (Recommended)
1. **Image 1 — The Hook**: Screenshot of **Slide `S01`** (*"Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up"*, [`slides/acts/01-hook-and-setup.md`](../../slides/acts/01-hook-and-setup.md#L6-L22)).
2. **Image 2 — The Real-World Proof**: Either **Slide `S24a` / `docs/egm/breadboard-wiring.svg`** ([`docs/egm/breadboard-wiring.svg`](../../docs/egm/breadboard-wiring.svg) / [`docs/assets/breadboard-wiring.svg`](../../docs/assets/breadboard-wiring.svg)) or **Slide `S26`** (`5 / 10` Vanilla vs. `10 / 10` EGM scorecard in [`slides/acts/03-egm-journey.md`](../../slides/acts/03-egm-journey.md#L486-L513)) showing how directory contracts and adversarial Goldfish reviews prevented hardware traps—while still leaving real physical and code-structure questions open.
3. **Image 3 — Docs for the Human**: Screenshot of the compiled 6-lesson `/teach-code` Tufte HTML landing page ([`docs/egm/tutorial/index.md`](../../docs/egm/tutorial/index.md), deployed at `https://domesticmouse.github.io/nuttxspace/egm/tutorial/index.html` and linked on **Slide `S29`** in [`slides/acts/04-teach-code-finale.md`](../../slides/acts/04-teach-code-finale.md#L52-L72)).

---

## `bjm-voice` Verification Checklist

1. **Australian English**: Uses *organisers*, *behaviour*, *organisation*, and Australian conversational phrasing (*"rattling around in my head"*, *"a bit like debugging hardware in public"*).
2. **No Corporate AI Slop**: Zero instances of *furthermore*, *moreover*, *testament to*, *tapestry*, *game-changer*, *delve*, or *unleash*.
3. **No Unblemished Triumph Narrative**: Explicitly pairs the *"saved me a lot of time"* compliment with the unanswered code-bloat Q&A challenge and the candid admission of being only one week into learning EGM.
4. **Grounded in Real Links & Artefacts**: Links directly to the live Slidev deck (`https://domesticmouse.github.io/nuttxspace/slides/`), `/teach-code` course (`https://domesticmouse.github.io/nuttxspace/egm/tutorial/index.html`), documentation hub (`https://domesticmouse.github.io/nuttxspace/`), GitHub repository (`https://github.com/domesticmouse/nuttxspace`), and the earlier LinkedIn post (`https://lnkd.in/p/gmVX-B74`).
