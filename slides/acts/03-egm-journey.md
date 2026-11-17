---
layout: default
---

<!-- S10 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Act III Anchor: The Elephant-Goldfish Triad</h2>
</div>

<div class="p-4 rounded-xl bg-cyan-950/15 border border-cyan-500/30 mb-5">
  <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Dave Rensin (@drensin) &middot; The Elephant-Goldfish Model (egm)</div>
  <div class="text-base font-semibold text-white mt-1 italic">
    &ldquo;Design is the new code: design with an Elephant, review with a Goldfish.&rdquo;
  </div>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">The Elephant (Context Memory)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li><code class="text-cyan-300">3,241</code> directory-level <code class="text-gray-300">README.md</code> contracts giving the agent a top-down architectural map (<code class="text-emerald-300">209</code> contract reads vs. <code class="text-red-300">0</code> in Vanilla).</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">The Human (Intent &amp; Ownership)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Makes product and hardware-boundary calls one question at a time; owns the final outcome.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">The Goldfish (Stateless Adversarial Swarm)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Fresh-context review subagents with 3-second memory&mdash;zero emotional loyalty or sycophancy toward the draft.</li>
    </ul>
  </li>
</ol>

<!--
To stop shipping hidden silicon timebombs, I turned to Dave Rensin's Elephant-Goldfish Model (egm). Dave sums it up in two lines: "Design is the new code: design with an Elephant, review with a Goldfish." The Elephant is your long-horizon context memory—powered here by 3,241 directory-level README.md contracts so the agent reads 209 architectural contracts instead of zero. The Human makes product and hardware-boundary calls one question at a time. And The Goldfish is a swarm of fresh-context reviewers with three-second memory—zero emotional loyalty to your draft.
-->

---
layout: skill-breakdown
way: 1
title: "Unpacking The Elephant: Directory Contracts as a Navigable Map"
---

<!-- S11 -->
<div class="space-y-4">
  <div class="text-sm text-gray-300">
    How the Elephant remembers <code class="text-cyan-300">3,241</code> directories without blowing out the context window:
  </div>

  <div v-click class="p-4 rounded-xl bg-gray-900/80 border border-gray-800">
    <div class="text-xs font-mono text-cyan-400 uppercase mb-1">Beat 1 &middot; Top-Down Tree Map</div>
    <p class="text-sm text-gray-200 m-0">
      <code class="text-cyan-300">2,607</code> <code class="text-gray-300">README.md</code> contracts in <code class="text-gray-300">nuttx/</code> + <code class="text-cyan-300">634</code> in <code class="text-gray-300">apps/</code> navigated hierarchically from the workspace root.
    </p>
  </div>

  <div v-click class="p-4 rounded-xl bg-cyan-950/20 border border-cyan-500/35 space-y-2">
    <div class="text-xs font-mono text-cyan-300 uppercase">Beat 2 &middot; Stylized Contract Card (nuttx/arch/arm/src/rp23xx/README.md)</div>
    <div class="text-xs text-gray-200 space-y-1 font-mono">
      <div><span class="text-gray-400">Public Interfaces:</span> <span class="text-white">rp23xx_adc.c, rp23xx_pwm.c</span></div>
      <div><span class="text-amber-300">Hardware Invariant:</span> <span class="text-gray-100">&ldquo;ADC init resets all 4 pins GPIO 26&ndash;29 and leaves digital input buffer IE=1 enabled.&rdquo;</span></div>
    </div>
  </div>

  <div v-click class="p-4 rounded-xl bg-gray-900/80 border border-gray-800">
    <div class="text-xs font-mono text-emerald-400 uppercase mb-1">Beat 3 &middot; Child Directory Routing</div>
    <p class="text-sm text-gray-200 m-0">
      Points the agent directly to the relevant board or driver subdirectory instead of grepping raw C files.
    </p>
  </div>
</div>

<!--
How do you give an Elephant memory across three thousand directories without blowing out the context window? Every directory in nuttx/ and apps/ gets a concise README.md contract—2,607 in the kernel, 634 in user space—forming a top-down tree. Inside arch/arm/src/rp23xx/README.md, it doesn't paste C code. It states public interfaces and plain-English hardware invariants—like warning that ADC init resets all four pins GPIO 26 to 29 and leaves digital buffer IE=1 enabled. And each contract routes the agent to the exact child directory it needs next.
-->

---
layout: default
---

<!-- S12 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Keeping the Elephant Honest: <code class="text-cyan-300">/plugin</code> &rarr; <code class="text-cyan-300">/egm-bootstrap</code> &rarr; <code class="text-cyan-300">/egm-drift-report</code></h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Phase 1 Setup &mdash; Wiring &amp; Verifying the Elephant (<code class="text-cyan-300">01-bootstrap.cast</code>):
</p>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Step 1 &mdash; <code class="text-cyan-300">/plugin</code></strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Install the <code class="text-cyan-300">egm</code> plugin dynamically from the built-in Antigravity marketplace.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Step 2 &mdash; <code class="text-cyan-300">/egm-bootstrap</code></strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Synthesise the root workspace <code class="text-gray-300">README.md</code> architectural blueprint linking <code class="text-gray-300">nuttx/</code> and <code class="text-gray-300">apps/</code>.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Step 3 &mdash; <code class="text-cyan-300">/egm-drift-report</code></strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Verify all <code class="text-emerald-300">3,241</code> directory contracts against <code class="text-gray-300">git HEAD</code> (<code class="text-emerald-300">0</code> drifted contracts) before designing.</li>
    </ul>
  </li>
</ol>

<!--
Before we design anything, we wire up the Elephant in three quick commands. First, we install the egm plugin live inside Antigravity using /plugin. Second, /egm-bootstrap synthesises the root workspace README.md blueprint. Third, /egm-drift-report audits all 3,241 directory contracts against git HEAD so stale docs never poison our design.
-->

---
layout: cast-full
title: "Phase 1: Bootstrap & Drift Report (01-bootstrap.cast)"
cast: /casts/01-bootstrap.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S13 -->
<!--
Back on the Antigravity start screen—let's install egm. /plugin opens the built-in marketplace. We select egm from Discover and install it. Now all the /egm-* slash commands are live. Let's queue up /egm-bootstrap. We ask /egm-bootstrap to generate our root README.md linking nuttx/ and apps/. Watch the Elephant read the top-level submodule contracts first—no blind grepping—and assemble the root architectural map of the workspace. Root blueprint is written, and it asks if we want to run /egm-drift-report across the tree. Yes please. In twenty-five warped seconds it verifies all 3,241 directories against git history: zero drifted contracts. The Elephant's map is verified. Now the Elephant is ready to brainstorm.
-->

---
layout: default
---

<!-- S14 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Specification Funnel: Human + Elephant (<code class="text-cyan-300">/egm-brainstorm</code>)</h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Phase 2 Setup &mdash; Grounded Ideation Before Any Code Exists (<code class="text-cyan-300">02-brainstorm.cast</code>):
</p>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Reads <code class="text-emerald-300">49</code> <code class="text-gray-300">README.md</code> Contracts Upfront</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Traverses <code class="text-gray-300">boards/arm/rp23xx/xiao-rp2350</code> and <code class="text-gray-300">arch/arm/src/rp23xx</code>, spotting the <code class="text-amber-300">GPIO 26&ndash;29</code> ADC pin clobber <em>before</em> asking Question 1.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white"><code class="text-cyan-300">1-at-a-Time</code> Socratic Q&amp;A</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Human locks product boundaries: <code class="text-gray-300">Q1</code> Non-ADC PWM pins, <code class="text-gray-300">Q2</code> Background daemon keeping USB-C <code class="text-cyan-300">nsh&gt;</code>, <code class="text-gray-300">Q3</code> Proportional PWM brightness.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Goldfish Readiness Checkpoint</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Synthesises agreed architecture into <code class="text-emerald-300">concepts_brief.md</code> with zero C code written.</li>
    </ul>
  </li>
</ol>

<!--
Next comes /egm-brainstorm, where the Elephant and the Human narrow down the product shape before any code exists. Before asking Question 1, the Elephant reads 49 README.md contracts—spotting the GPIO 26 to 29 ADC pin clobber immediately. Then it interviews me one question at a time: pin mapping, USB-C nsh> coexistence, and proportional PWM. Finally, a readiness gate synthesises our answers into concepts_brief.md.
-->

---
layout: cast-full
title: "Phase 2: Grounded Brainstorming (02-brainstorm.cast)"
cast: /casts/02-brainstorm.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S15 -->
<!--
Let's kick off /egm-brainstorm with our hardware goal. Same goal as Vanilla Vibe, prefixed with /egm-brainstorm. Look at the tool calls flying past: instead of writing C, it traverses 49 directory README.md contracts across boards/arm/rp23xx/xiao-rp2350 and arch/arm/src/rp23xx, discovering upfront that GPIO 27, 28, and 29 get clobbered by ADC init. Look at Q1: because it read the contract, it explicitly warns us to keep our LEDs off the ADC pins! We tell it to pick six non-ADC PWM pins and design a clean breadboard layout. Q2 spots Trap 1: how should bargraphd start on boot without hanging the single USB-C nsh> console? Spawn bargraphd in the background and keep USB-C nsh> interactive. Q3 asks how the six LEDs should visually map the pot reading. Lower segments full on, active segment ramping smoothly via hardware PWM. After locking in the CLI commands, it reaches the brainstorm readiness gate and asks to run a quick Goldfish sanity check before writing concepts_brief.md. We select "Yes, synthesize and review with Goldfishes". In twenty-eight warped seconds, fresh subagents verify the brief against the contracts and write concepts_brief.md. Now we have a grounded concept brief—still zero lines of C written.
-->

---
layout: default
---

<!-- S16 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Section-by-Section Drafting &amp; Planting the <code class="text-amber-300">RP23XX_ADC_DIV</code> Trap</h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Phase 3 Setup &mdash; <code class="text-cyan-300">/egm-draft</code> (<code class="text-cyan-300">03-draft.cast</code> &rarr; <code class="text-gray-300">docs/designs/xiao-rp2350-bargraph.md</code>):
</p>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Section-by-Section Human Approval Gates</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Loads <code class="text-cyan-300">51</code> contract and header files, drafting Overview, Technical Plan, Alternatives, and Implementation with interactive sign-off.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-amber-300">Cueing the False Start at Stop 4 (<code class="text-gray-200">Q3: Technical Plan</code>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Watch both the Elephant and the Human approve throttling <code class="text-cyan-300">/dev/adc0</code> via <code class="text-amber-300">CONFIG_RP23XX_ADC_DIV</code>. Sounds completely plausible&hellip; and it&rsquo;s a trap!</li>
    </ul>
  </li>
</ol>

<!--
In Phase 3, /egm-draft turns concepts_brief.md into our full engineering specification, xiao-rp2350-bargraph.md. Instead of dumping a wall of Markdown, it drafts section by section with human approval gates. Watch Stop 4 (Q3: Technical Plan) closely: both the Elephant and I agree to throttle /dev/adc0 using CONFIG_RP23XX_ADC_DIV. Sounds totally plausible—and it's a trap!
-->

---
layout: cast-full
title: "Phase 3: Section-by-Section Spec Drafting (03-draft.cast)"
cast: /casts/03-draft.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S17 -->
<!--
Let's run /egm-draft against @concepts_brief.md. Targeting docs/designs/xiao-rp2350-bargraph.md. The Elephant loads 51 contract and header files into context so every pin number, PWM slice, and Kconfig symbol in the draft is grounded. Context confirmed—proceed to drafting. It drafts Section 1—Overview, Goals, and Non-Goals—and pauses for human sign-off. Overview and Goals look spot-on. Approved. Now watch it draft the Technical Plan. It sees the 500 kHz free-running interrupt hazard and proposes setting CONFIG_RP23XX_ADC_DIV to slow down hardware pacing. Here is the confession moment: I read that on screen, thought "beauty, hardware clock divider, makes total sense," and hit Approve! It finishes the remaining sections, reconciles the document, and promotes v0-draft. Let's pause and look at why that approval was dead wrong.
-->

---
layout: default
---

<!-- S18 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Debrief: Author Bias, Self-Sycophancy &amp; Why We Need Goldfish</h2>
</div>

<div class="p-4 rounded-xl bg-gray-900/80 border border-gray-800 mb-5">
  <div class="text-sm font-semibold text-white">Why <code class="text-amber-300">CONFIG_RP23XX_ADC_DIV</code> Was a Hidden Silicon Landmine</div>
</div>

<div class="grid grid-cols-2 gap-6">
  <div v-click class="p-5 rounded-xl bg-red-950/15 border border-red-500/30 space-y-2">
    <div class="text-xs font-mono text-red-400 uppercase">The Silicon Reality</div>
    <h3 class="text-lg font-bold text-white m-0"><code class="text-red-300">START_ONCE</code> Ignores <code class="text-gray-300">DIV</code></h3>
    <p class="text-sm text-gray-300 m-0 leading-relaxed">
      NuttX&rsquo;s RP2350 ADC driver triggers conversions via single-shot <code class="text-red-300">START_ONCE</code>&mdash;which <em>completely ignores</em> the hardware <code class="text-gray-300">DIV</code> register and still fires <strong class="text-red-300">500,000 IRQ/s</strong>!
    </p>
  </div>

  <div v-click class="p-5 rounded-xl bg-amber-950/15 border border-amber-500/30 space-y-2">
    <div class="text-xs font-mono text-amber-400 uppercase">Dave Rensin &middot; Author Bias</div>
    <h3 class="text-lg font-bold text-white m-0">Shared-Context Confirmation Bias</h3>
    <p class="text-sm text-gray-300 m-0 leading-relaxed">
      The session that spent 30 minutes co-authoring a spec falls in love with its own plan and rubber-stamps its blind spots. <strong class="text-white">We need a reviewer with amnesia.</strong>
    </p>
  </div>
</div>

<!--
Why was CONFIG_RP23XX_ADC_DIV a landmine? Because NuttX's RP2350 ADC driver triggers conversions via single-shot START_ONCE—which completely ignores the hardware DIV register and still fires five hundred thousand interrupts per second! As Dave Rensin warns, the costliest mistake is a plan both the agent and the human fell in love with inside the same session. Once we agreed on ADC_DIV, confirmation bias locked in. We needed a reviewer with amnesia.
-->

---
layout: default
---

<!-- S19 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Unpacking The Goldfish: Stateless Adversarial Swarm (<code class="text-cyan-300">/egm-review</code>)</h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  How <code class="text-cyan-300">/egm-review</code> defeats author sycophancy:
</p>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Stateless &amp; Uninvested</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Fresh subagent context windows that see only the spec and the repository contracts&mdash;zero memory of our brainstorm or drafting struggle.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Multi-Angle Adversarial Personas (<code class="text-emerald-300">146</code> Contract Reads)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li><em>Zoom-Out Architect</em>, <em>Comprehension</em>, <em>Hostile Design Critic</em>, <em>Implementation Readiness</em>, and <em>Codebase Details</em> audit <code class="text-gray-300">v0-draft</code> in parallel.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Synthesizer + <code class="text-cyan-300">/egm-resolve</code></strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Deduplicates findings into ranked Blockers (<code class="text-red-300">B-1..B-3</code>) and Majors (<code class="text-amber-300">M-1..M-5</code>) for interactive human <code class="text-emerald-300">accept</code>/<code class="text-gray-300">reject</code>.</li>
    </ul>
  </li>
</ol>

<!--
Enter /egm-review—the Goldfish swarm. Each Goldfish runs in a brand-new subagent context window. It never saw our brainstorm, has zero sympathy for how hard we worked on v0-draft, and owes us zero politeness. Five specialised personas—Zoom-Out Architect, Comprehension, Hostile Design Critic, Implementation Readiness, and Codebase Details—audit v0-draft in parallel across 146 README.md contracts. Then a Synthesizer deduplicates their findings into ranked Blockers (B-1..B-3) and Majors for /egm-resolve.
-->

---
layout: default
---

<!-- S20 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">What to Watch in <code class="text-cyan-300">04-review.cast</code>: <code class="text-red-300">B-1</code>, <code class="text-cyan-300">/clear</code> Amnesia, and Pass 2</h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Two crucial beats in <code class="text-cyan-300">04-review.cast</code> (<code class="text-gray-300">2m 21s</code> warped):
</p>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Beat 1 &mdash; Pass 1 Catches Blocker <code class="text-red-300">B-1</code></strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Goldfish busts <code class="text-amber-300">RP23XX_ADC_DIV</code> cold and replaces it with <strong class="text-emerald-300">NVIC IRQ gating</strong> around a 5-sample burst.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Beat 2 &mdash; <code class="text-cyan-300">/clear</code> Amnesia + Pass 2 Catches <code class="text-amber-300">CA-01</code></strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Wiping session memory with <code class="text-cyan-300">/clear</code> before Pass 2 exposes a brand-new FIFO deadlock <em>introduced by Pass 1&rsquo;s fix</em>!</li>
    </ul>
  </li>
</ol>

<!--
Two things to watch in 04-review.cast: First, watch Pass 1 immediately flag RP23XX_ADC_DIV as Blocker B-1 and replace it with NVIC IRQ burst gating. Second, watch us run /clear to wipe session memory cold—so Pass 2 catches a brand-new bug introduced by Pass 1's fix!
-->

---
layout: cast-full
title: "Phase 4: Two-Pass Goldfish Swarm & /clear Amnesia (04-review.cast)"
cast: /casts/04-review.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S21 -->
<!--
Let's unleash the Pass 1 Goldfish swarm on v0-draft. Running /egm-review @docs/designs/xiao-rp2350-bargraph.md. We choose Consolidated mode to fan out the Goldfish reviewers in parallel. Twenty-one raw minutes compressed at thirty-nine times speed. Watch the parallel subagents fan out across 146 contracts and driver files, cross-checking every claim in v0-draft against the actual RP2350 silicon registers. Pass 1 found 3 Blockers and 5 Majors. Let's run /egm-resolve. Starting interactive resolution. It asks how we want to work through the 12 findings. Type interactive so we inspect each blocker one by one. Look at Item 1/12 (B-1): the Goldfish busted our RP23XX_ADC_DIV assumption cold and recommends gating the NVIC IRQ around a 5-sample burst! We type accept to adopt the 5-sample burst fix. Now all Pass 1 items are patched into the spec. Watch this next move: before Pass 2, we wipe our own session memory. Running /clear so the agent forgets every justification from Pass 1. Clean slate. Now we launch Pass 2. Running Pass 2 /egm-review cold against the updated spec. Watch Pass 2 inspect the 5-sample burst fix we just accepted in Pass 1. Because these Goldfish have zero memory of Pass 1, they calculate 5 samples times 5 bytes equals 25 bytes against NuttX's 8-sample software FIFO—and spot a deadlock! Boom—CA-01 caught in Markdown before a single line of C was written.
-->

---
layout: default
---

<!-- S22 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Debrief: First-Order Catch (<code class="text-red-300">B-1</code>) vs. Second-Order Catch (<code class="text-amber-300">CA-01</code>)</h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Why <code class="text-cyan-300">/clear</code> between review passes matters:
</p>

<div class="grid grid-cols-2 gap-6">
  <div v-click class="p-5 rounded-xl bg-cyan-950/15 border border-cyan-500/30 space-y-2">
    <div class="text-xs font-mono text-cyan-400 uppercase">First-Order Catch &middot; Pass 1 (B-1)</div>
    <h3 class="text-lg font-bold text-white m-0">NVIC IRQ Burst Gating</h3>
    <p class="text-sm text-gray-300 m-0 leading-relaxed">
      Replaced the <code class="text-red-300">500 kHz</code> interrupt storm with a 5-sample NVIC IRQ-gated burst (discard <code class="text-gray-300">1</code> stale sample + average <code class="text-gray-300">4</code>).
    </p>
  </div>

  <div v-click class="p-5 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">Second-Order Catch &middot; Pass 2 (CA-01 after /clear)</div>
    <h3 class="text-lg font-bold text-white m-0">The <code class="text-amber-300">25-Byte</code> Short-Read Deadlock</h3>
    <p class="text-sm text-gray-300 m-0 leading-relaxed">
      NuttX&rsquo;s software ADC FIFO holds <code class="text-gray-300">8</code> samples (<code class="text-amber-300">7</code> usable before full). Reading <code class="text-red-300">5</code> samples (<code class="text-red-300">25 B</code>) leaves <code class="text-gray-300">2</code> in the queue, so the second <code class="text-red-300">read(25)</code> blocks forever at <code class="text-amber-300">7</code>! Fixed by reading <strong class="text-emerald-300">20 bytes (4 samples)</strong> per burst.
    </p>
  </div>
</div>

<!--
That two-pass loop is my favourite moment in the entire project. Pass 1 (B-1) killed the 500 kHz interrupt storm by gating the NVIC interrupt around a 5-sample burst—discarding one stale sample and averaging four. Wait: NuttX's software ADC FIFO holds 8 samples (7 usable before full). Reading 5 samples (25 bytes) leaves 2 in the queue, so the second read(25) blocks forever at 7! /clear gave Pass 2 fresh eyes to catch the bug introduced by Pass 1's fix.
-->

---
layout: default
---

<!-- S23 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">When the Spec is Hardened, Coding is Boring (<code class="text-cyan-300">05-implement</code> Setup)</h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Phase 5 Setup &mdash; Implementing from the Hardened Spec (<code class="text-cyan-300">05-implement.cast</code>):
</p>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white"><code class="text-emerald-300">4</code> Blockers + <code class="text-emerald-300">7</code> Majors Eliminated in Markdown</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Every register quirk, <code class="text-emerald-300">20-byte</code> FIFO read size, and non-ADC pin assignment is locked before writing C.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">What to Watch (<code class="text-gray-300">58s</code> warped)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li><code class="text-emerald-300">100%</code> host unit tests pass, <code class="text-emerald-300">nuttx/nuttx.uf2</code> (<code class="text-gray-300">347K</code>) compiles on the <strong class="text-white">first attempt (<code class="text-emerald-300">0</code> retries)</strong>, and <code class="text-cyan-300">9</code> new <code class="text-gray-300">README.md</code> contracts are written to keep the Elephant in sync.</li>
    </ul>
  </li>
</ol>

<!--
Once you eliminate four Blockers and seven Majors in Markdown, coding becomes wonderfully boring. Every register quirk, FIFO read size (20 bytes), and pin assignment is already locked. Watch 05-implement.cast pass 100% of host tests and build nuttx.uf2 on the very first attempt (0 retries)—plus write 9 new README.md contracts.
-->

---
layout: cast-full
title: "Phase 5: First-Try Firmware Build & Breadboard SVG (05-implement.cast)"
cast: /casts/05-implement.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S24 -->
<!--
Let's hand the hardened v1 spec to Antigravity. Prompting it to implement xiao-rp2350-bargraph.md and build nuttx.uf2. At twenty-seven times speed: it wires the out-of-tree board overlays, writes bargraph_main.c and the pure math core, passes 6/6 host unit test suites first go, compiles nuttx.uf2 (347K) with zero retries, and updates 9 directory README.md contracts so the Elephant stays in sync. First-try firmware build complete! Now let's ask for the breadboard wiring guide and SVG. Generating docs/egm/breadboard-wiring.svg from our verified pin table. Because the spec already locked the mechanical footprint and non-ADC pins, it generates a physically buildable breadboard layout in one pass. Let's compare the physical breadboard result.
-->

---
layout: default
---

<!-- S24a -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">EGM&rsquo;s Generated Breadboard Wiring</h2>
</div>

<div class="w-full flex items-center justify-center">
  <img src="/images/breadboard-wiring.svg" alt="EGM Generated Breadboard Wiring SVG" class="w-full max-h-[430px] object-contain rounded-xl border border-gray-800 shadow-lg" />
</div>

<!--
Here is the full breadboard wiring SVG that EGM generated in one pass. Notice the clean light-mode schematic layout, the XIAO RP2350 seated across the centre trench, the potentiometer on D0, and the six color-coded LEDs on D1, D2, D4, D5, D8, and D9. Now let's zoom in on the XIAO RP2350 itself to verify the physical footprint and pin routing.
-->

---
layout: hardware-callout
title: "Breadboard Reality Check: Electrical Wins vs. Physical Footprint (0.6&quot; XIAO)"
image: /images/breadboard-wiring-crop-ef.svg
imageCaption: "docs/egm/breadboard-wiring.svg — XIAO RP2350 drawn across Cols E & F (0.3&quot; trench, widened visually)"
---

<!-- S25 -->
<div class="space-y-4">
  <div v-click class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/35 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">Verified Electrical &amp; Pinout Wins</div>
    <ul class="text-sm text-gray-200 space-y-1.5 list-disc pl-5 m-0 leading-relaxed">
      <li>Routes LEDs to <strong class="text-white">6 non-ADC PWM pins</strong> (<code class="text-cyan-300">D1, D2, D4, D5, D8, D9</code>), leaves <code class="text-gray-300">5V</code> unconnected, and budgets <code class="text-emerald-300">23.6 mA</code> with <code class="text-gray-300">330 &Omega;</code> resistors.</li>
      <li>Disables digital input buffer <code class="text-emerald-300">IE=0</code> on <code class="text-gray-300">GPIO 26</code> to defeat Erratum <code class="text-amber-300">RP2350-E9</code>.</li>
    </ul>
  </div>

  <div v-click class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/35 space-y-2">
    <div class="text-xs font-mono text-amber-400 uppercase tracking-wide">Physical Footprint Catch (0.3&quot; vs. 0.6&quot; XIAO Width)</div>
    <p class="text-sm text-gray-200 m-0 leading-relaxed">
      Both runs missed the <strong class="text-white"><code class="text-amber-300">0.6&quot;</code> (<code class="text-gray-300">15.24 mm</code>)</strong> XIAO header spacing (<code class="text-gray-300">Cols C &amp; G</code> or <code class="text-gray-300">D &amp; H</code>): Vanilla spanned <strong class="text-red-300">0.7&quot;</strong> (<code class="text-gray-300">Cols C&ndash;H</code>), while EGM seated it in <strong class="text-amber-300">Cols E &amp; F (<code class="text-amber-300">0.3&quot;</code>)</strong>&mdash;masked in the SVG by widening the visual trench.
    </p>
  </div>
</div>

<!--
Zooming in on docs/egm/breadboard-wiring.svg gives us a great engineering honesty check. On the electrical side, EGM nailed it: six non-ADC PWM pins (D1, D2, D4, D5, D8, D9), 330-ohm resistors keeping total draw at 23.6 milliamps, 5V left unconnected, and IE disabled on GPIO 26 to defeat Erratum RP2350-E9. But look at the physical footprint: a XIAO RP2350's headers are 0.6 inches apart—meaning it needs Columns C and G, or D and H. Vanilla stretched it too wide to Columns C and H (0.7 inches), while EGM seated it too narrow in Columns E and F (0.3 inches) and hid the mismatch by drawing a wider centre trench in the SVG. Contracts caught every silicon and RTOS invariant, but mechanical dimensions still deserve a real-world check.
-->

---
layout: default
---

<!-- S26 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Candid Scorecard Callback: <code class="text-red-300">5 / 10</code> vs. <code class="text-emerald-300">10 / 10</code> (And Engineering Honesty)</h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Experiment A vs. Experiment B &mdash; The Bench Scorecard:
</p>

<div class="grid grid-cols-2 gap-6">
  <div v-click class="p-5 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">Primary 10-Trap Hardware Scorecard</div>
    <div class="text-2xl font-extrabold text-white">
      Vanilla <code class="text-red-300">5 / 10</code> &rarr; EGM <code class="text-emerald-300">10 / 10</code>
    </div>
    <p class="text-sm text-gray-300 m-0 leading-relaxed">
      <code class="text-emerald-300">0</code> hardware or RTOS traps hit on silicon and <code class="text-emerald-300">0</code> firmware build retries.
    </p>
  </div>

  <div v-click class="p-5 rounded-xl bg-amber-950/15 border border-amber-500/30 space-y-2">
    <div class="text-xs font-mono text-amber-400 uppercase">Candid Strict Audit (6 Pass / 4 Partial / 0 Fail)</div>
    <div class="text-lg font-bold text-white">No Unblemished Triumph Narratives</div>
    <p class="text-sm text-gray-300 m-0 leading-relaxed">
      A cold post-build Goldfish audit against the finished C code still flagged <code class="text-amber-300">4</code> secondary polish items (<code class="text-gray-300">SIGTERM</code> cleanup, Makefile test scaffolding)&mdash;proving adversarial review is an ongoing habit, not magic.
    </p>
  </div>
</div>

<!--
Let's look at the scorecard—and keep ourselves honest. Across our ten hardware and RTOS traps, Vanilla Vibe scored five out of ten; the EGM workflow scored ten out of ten with zero build retries. But I promised no unblemished triumph narratives: when I ran a cold post-build Goldfish audit against every line of C, it still flagged four partial polish items like SIGTERM cleanup—proving adversarial review is an ongoing habit, not magic.
-->
