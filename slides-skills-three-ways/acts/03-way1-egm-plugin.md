---
layout: default
---

<!-- S15 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Way 1: Multi-Agent Workflow Plugins (<code class="text-cyan-300">/plugin</code> &rarr; <code class="text-white">egm</code>)</h2>
</div>

<div class="p-4 rounded-xl bg-cyan-950/15 border border-cyan-500/30 mb-5">
  <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Dave Rensin (@drensin) &middot; The Elephant-Goldfish Model (egm)</div>
  <div class="text-base font-semibold text-white mt-1 italic">
    &ldquo;Design is the new code: design with an Elephant, review with a Goldfish.&rdquo;
  </div>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">The Elephant (Context Memory via Directory Contracts)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li><code class="text-cyan-300">3,241</code> directory-level <code class="text-gray-300">README.md</code> contracts give the agent a top-down architectural map (<code class="text-emerald-300">209</code> contract reads vs. <code class="text-red-300">0</code> in Vanilla).</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">The Human (Intent &amp; Ownership)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Makes product and hardware-boundary calls one question at a time; owns the final outcome.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">The Goldfish (Stateless Adversarial Subagents)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Fresh-context review subagents with 3-second memory&mdash;zero emotional loyalty or sycophancy toward the draft.</li>
    </ul>
  </li>
</ol>

<!--
Way 1 is adopting a full multi-agent workflow plugin straight from the built-in marketplace. For our NuttX firmware build, I installed Dave Rensin's Elephant-Goldfish Model—egm. Dave puts the whole philosophy in one sentence: "Design is the new code: design with an Elephant, review with a Goldfish." The Elephant gives the agent durable architectural memory across 3,241 directory README.md contracts—meaning it reads 209 contracts instead of zero. The Human steers hardware boundaries one question at a time. And the Goldfish is a swarm of stateless subagents with three-second memory who owe zero politeness to our draft.
-->

---
layout: skill-breakdown
way: 1
title: "Way 1 Architecture: Directory Contracts + Bundled Subagent Personas"
---

<!-- S16 -->
<div class="space-y-3.5">
  <div class="p-3.5 rounded-xl bg-gray-900/80 border border-gray-800">
    <div class="text-xs font-mono text-cyan-400 uppercase mb-1">Beat 1 &middot; Top-Down Tree Map (&ldquo;The Elephant&rdquo;)</div>
    <p class="text-sm text-gray-200 m-0">
      <code class="text-cyan-300">2,607</code> <code class="text-gray-300">README.md</code> contracts in <code class="text-gray-300">nuttx/</code> + <code class="text-cyan-300">634</code> in <code class="text-gray-300">apps/</code> navigated hierarchically from the workspace root.
    </p>
  </div>

  <div v-click class="p-3.5 rounded-xl bg-cyan-950/20 border border-cyan-500/35 space-y-1.5">
    <div class="text-xs font-mono text-cyan-300 uppercase">Beat 2 &middot; Stylized Contract Card (nuttx/arch/arm/src/rp23xx/README.md)</div>
    <div class="text-xs text-gray-200 space-y-1 font-mono">
      <div><span class="text-gray-400">Public Interfaces:</span> <span class="text-white">rp23xx_adc.c, rp23xx_pwm.c</span></div>
      <div><span class="text-amber-300">Hardware Invariant:</span> <span class="text-gray-100">&ldquo;ADC init resets all 4 pins GPIO 26&ndash;29 and leaves digital input buffer IE=1 enabled.&rdquo;</span></div>
    </div>
  </div>

  <div v-click class="p-3.5 rounded-xl bg-gray-900/80 border border-gray-800 space-y-1">
    <div class="text-xs font-mono text-emerald-400 uppercase">Beat 3 &middot; One <code class="text-emerald-300">/plugin</code> Install = Commands + 10 Subagent Personas</div>
    <p class="text-xs text-gray-200 m-0 leading-relaxed">
      Bundles slash workflows (<code class="text-cyan-300">/egm-bootstrap</code>, <code class="text-cyan-300">/egm-brainstorm</code>, <code class="text-cyan-300">/egm-draft</code>, <code class="text-cyan-300">/egm-review</code>) with adversarial reviewers (<code class="text-gray-300">egm-design</code>, <code class="text-gray-300">egm-codebase-details</code>, <code class="text-gray-300">egm-comprehension</code>, <code class="text-gray-300">egm-zoom-out</code>, <code class="text-gray-300">egm-implementation-readiness</code>, <code class="text-gray-300">egm-synthesizer</code>).
    </p>
  </div>
</div>

<!--
Under the hood, Way 1 plugins package skills, rules, and dedicated subagent personas into a single installable unit. Each directory in nuttx/ and apps/ carries a crisp README.md contract—2,607 in the kernel, 634 in user space—stating public interfaces and plain-English hardware invariants like that GPIO 26 to 29 ADC reset trap. Meanwhile, one /plugin install registers both the /egm-* workflow skills and ten specialist subagent personas ready to cross-examine our specs in isolated context windows.
-->

---
layout: default
---

<!-- S17 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Keeping the Elephant Honest: <code class="text-cyan-300">/plugin</code> &rarr; <code class="text-cyan-300">/egm-bootstrap</code> &rarr; <code class="text-cyan-300">/egm-drift-report</code></h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Phase 1 Setup &mdash; Installing the Plugin &amp; Verifying the Elephant (<code class="text-cyan-300">01-bootstrap.cast</code>):
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
Before designing a single circuit or line of C, we wire up the plugin in three quick commands. First, /plugin installs egm live inside Antigravity. Second, /egm-bootstrap synthesises our root workspace README.md map. Third, /egm-drift-report verifies all 3,241 directory contracts against git HEAD so stale docs never poison the design.
-->

---
layout: cast-full
title: "Way 1 Phase 1: Bootstrap & Drift Report (01-bootstrap.cast)"
cast: /casts/01-bootstrap.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S18 -->
<!--
Back on the Antigravity start screen—let's install egm. /plugin opens the built-in marketplace. We pick egm from Discover and install it. Immediately all the /egm-* slash commands are live. Let's run /egm-bootstrap to synthesise our root README.md linking nuttx/ and apps/. Notice how the Elephant reads the top-level submodule contracts first—no blind grepping—and writes the workspace map. Root blueprint done, and it prompts whether to run /egm-drift-report across the tree. Yes please. In twenty-five warped seconds it verifies all 3,241 directories against git history: zero drifted contracts. The Elephant's map is verified. Now it's ready to brainstorm.
-->

---
layout: default
---

<!-- S19 -->
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
Next comes /egm-brainstorm. Notice the difference from Vanilla Vibe's blind sprint: before asking Question 1, the agent reads 49 directory contracts and catches the GPIO 26 to 29 ADC clobber immediately. Then it asks one Socratic question at a time—pin mapping, USB-C console coexistence, and PWM curves—before dispatching Goldfish subagents to verify everything into concepts_brief.md.
-->

---
layout: cast-full
title: "Way 1 Phase 2: Grounded Brainstorming (02-brainstorm.cast)"
cast: /casts/02-brainstorm.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S20 -->
<!--
Let's kick off /egm-brainstorm with the exact same hardware prompt as Vanilla Vibe. Watch the tool calls flying past: instead of writing C, it reads 49 directory README.md contracts across boards/arm/rp23xx/xiao-rp2350 and arch/arm/src/rp23xx, finding upfront that GPIO 27, 28, and 29 get clobbered by ADC init. Look at Q1: because it read the contract, it warns us to keep our LEDs off the ADC pins! We tell it to pick six non-ADC PWM pins and design a clean breadboard layout. Q2 catches Trap 1: how should bargraphd boot without hanging our single USB-C nsh> port? Spawn bargraphd in the background and keep nsh> interactive. Q3 locks in proportional PWM brightness across the active segment. At the readiness gate, we select "Yes, synthesize and review with Goldfishes". Fresh subagents verify the brief against the contracts and write concepts_brief.md—still zero lines of C written.
-->

---
layout: default
---

<!-- S21 -->
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
    <strong class="text-amber-300">Watch Closely at Stop 4 (<code class="text-gray-200">Q3: Technical Plan</code>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Both the Elephant and the Human approve throttling <code class="text-cyan-300">/dev/adc0</code> via <code class="text-amber-300">CONFIG_RP23XX_ADC_DIV</code>. Sounds completely plausible&hellip; and it&rsquo;s a trap!</li>
    </ul>
  </li>
</ol>

<!--
In Phase 3, /egm-draft turns concepts_brief.md into our full technical spec, xiao-rp2350-bargraph.md, walking section by section with human approval gates. Watch Stop 4 closely: both the Elephant and I happily approve throttling /dev/adc0 with CONFIG_RP23XX_ADC_DIV. Sounds totally plausible—and it's a complete trap.
-->

---
layout: cast-full
title: "Way 1 Phase 3: Section-by-Section Spec Drafting (03-draft.cast)"
cast: /casts/03-draft.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S22 -->
<!--
Running /egm-draft against @concepts_brief.md, targeting docs/designs/xiao-rp2350-bargraph.md. The Elephant loads 51 contract and header files so every pin number, PWM slice, and Kconfig symbol in the draft is grounded. Context confirmed—proceed to drafting. Section 1—Overview, Goals, and Non-Goals—looks spot-on. Approved. Now watch the Technical Plan: it spots the 500 kHz interrupt hazard and proposes setting CONFIG_RP23XX_ADC_DIV to slow down hardware pacing. Confession time: I read that on screen, thought "beauty, hardware clock divider, makes sense," and hit Approve! It finishes the remaining sections and promotes v0-draft. Let's look at why that approval was dead wrong—and how the plugin's Goldfish swarm catches us.
-->

---
layout: default
---

<!-- S23 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Author Bias vs. Stateless Goldfish Swarm (<code class="text-cyan-300">/egm-review</code>)</h2>
</div>

<div class="grid grid-cols-2 gap-6 mt-2">
  <div class="p-4 rounded-xl bg-red-950/15 border border-red-500/30 space-y-2">
    <div class="text-xs font-mono text-red-400 uppercase">The Hidden Landmine &amp; Author Bias</div>
    <h3 class="text-base font-bold text-white m-0"><code class="text-red-300">START_ONCE</code> Ignores <code class="text-gray-300">DIV</code></h3>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      NuttX&rsquo;s RP2350 ADC driver triggers single-shot <code class="text-red-300">START_ONCE</code>&mdash;ignoring the <code class="text-gray-300">DIV</code> register and firing <strong class="text-red-300">500k IRQ/s</strong>! Co-authoring in one long session breeds confirmation bias.
    </p>
  </div>

  <div v-click class="p-4 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">The Goldfish Cure (146 Contract Reads)</div>
    <h3 class="text-base font-bold text-white m-0">Stateless Parallel Personas</h3>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      Fresh context windows (<code class="text-gray-200">Zoom-Out</code>, <code class="text-gray-200">Comprehension</code>, <code class="text-gray-200">Critic</code>, <code class="text-gray-200">Readiness</code>, <code class="text-gray-200">Codebase Details</code>) audit <code class="text-gray-300">v0-draft</code> cold and rank Blockers (<code class="text-red-300">B-1..B-3</code>).
    </p>
  </div>
</div>

<div v-click class="mt-5 p-4 rounded-xl bg-gray-900/80 border border-gray-800 space-y-1.5">
  <div class="text-xs font-mono text-cyan-400 uppercase">What to Watch in <code class="text-cyan-300">04-review.cast</code> (<code class="text-gray-300">2m 21s</code> warped)</div>
  <p class="text-xs text-gray-200 m-0 leading-relaxed">
    <strong class="text-white">Pass 1</strong> busts <code class="text-amber-300">RP23XX_ADC_DIV</code> (<code class="text-red-300">B-1</code>) and switches to NVIC IRQ burst gating; then we wipe session memory with <code class="text-cyan-300">/clear</code> so <strong class="text-white">Pass 2</strong> catches <code class="text-amber-300">CA-01</code>&mdash;a brand-new deadlock <em>introduced by Pass 1&rsquo;s fix</em>!
  </p>
</div>

<!--
Why did CONFIG_RP23XX_ADC_DIV fail? Because NuttX triggers conversions with single-shot START_ONCE, which ignores the hardware DIV register completely! Once the agent and I spent half an hour co-authoring v0-draft, shared-context confirmation bias kicked in. That is why /egm-review dispatches five fresh-context Goldfish personas across 146 README.md contracts—zero memory of our brainstorm, zero loyalty to our draft. Watch 04-review.cast closely: Pass 1 flags Blocker B-1 and swaps in NVIC IRQ burst gating. Then we run /clear to wipe session amnesia before Pass 2—which catches a brand-new deadlock introduced by Pass 1's own fix!
-->

---
layout: cast-full
title: "Way 1 Phase 4: Two-Pass Goldfish Swarm & /clear Amnesia (04-review.cast)"
cast: /casts/04-review.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S24 -->
<!--
Running /egm-review @docs/designs/xiao-rp2350-bargraph.md in Consolidated mode. Twenty-one raw minutes compressed at thirty-nine times speed: watch the parallel Goldfish subagents fan out across 146 contracts and silicon registers. Pass 1 uncovers 3 Blockers and 5 Majors. Let's run /egm-resolve. Typing interactive to step through each finding. Look at Item 1/12 (B-1): the Goldfish busted RP23XX_ADC_DIV cold and recommends gating the NVIC IRQ around a 5-sample burst! Typing accept. Now all Pass 1 fixes are patched into the spec. Next move: before Pass 2, we run /clear so the orchestrator forgets every rationale from Pass 1. Clean slate. Now we launch Pass 2 /egm-review cold against the updated spec. Watch Pass 2 audit the 5-sample burst fix we just accepted: 5 samples times 5 bytes is 25 bytes, colliding with NuttX's 8-sample software FIFO! Boom—CA-01 caught in Markdown before a single line of C exists.
-->

---
layout: default
---

<!-- S25 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Second-Order Catch (<code class="text-amber-300">CA-01</code>) &rarr; Boring First-Try Compile</h2>
</div>

<div class="grid grid-cols-2 gap-5 mt-2">
  <div class="p-4 rounded-xl bg-cyan-950/15 border border-cyan-500/30 space-y-1.5">
    <div class="text-xs font-mono text-cyan-400 uppercase">Pass 1 Catch (<code class="text-red-300">B-1</code>)</div>
    <h3 class="text-base font-bold text-white m-0">NVIC IRQ Burst Gating</h3>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      Killed the <code class="text-red-300">500 kHz</code> storm by gating the NVIC interrupt around a 5-sample burst (discard <code class="text-gray-300">1</code> stale sample + average <code class="text-gray-300">4</code>).
    </p>
  </div>

  <div v-click class="p-4 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-1.5">
    <div class="text-xs font-mono text-emerald-400 uppercase">Pass 2 Catch After /clear (<code class="text-amber-300">CA-01</code>)</div>
    <h3 class="text-base font-bold text-white m-0">The <code class="text-amber-300">25-Byte</code> Short-Read Deadlock</h3>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      NuttX&rsquo;s ADC FIFO holds <code class="text-gray-300">8</code> slots (<code class="text-amber-300">7</code> usable). Reading <code class="text-red-300">25 B</code> (5 samples) leaves <code class="text-gray-300">2</code> behind, so the second <code class="text-red-300">read(25)</code> blocks forever at <code class="text-amber-300">7</code>! Fixed to <strong class="text-emerald-300">20 bytes (4 samples)</strong>.
    </p>
  </div>
</div>

<div v-click class="mt-4 p-4 rounded-xl bg-gray-900/80 border border-gray-800 space-y-1.5">
  <div class="text-xs font-mono text-emerald-400 uppercase">Phase 5 Result (<code class="text-cyan-300">05-implement.cast</code> &middot; <code class="text-gray-300">58s</code> warped)</div>
  <p class="text-xs text-gray-200 m-0 leading-relaxed">
    With <code class="text-emerald-300">4</code> Blockers + <code class="text-emerald-300">7</code> Majors dead in Markdown, <code class="text-emerald-300">100%</code> of host unit tests pass, <code class="text-emerald-300">nuttx/nuttx.uf2</code> (<code class="text-gray-300">347K</code>) compiles on the <strong class="text-white">first attempt (<code class="text-emerald-300">0</code> retries)</strong>, and <code class="text-cyan-300">9</code> new <code class="text-gray-300">README.md</code> contracts keep the Elephant synced.
  </p>
</div>

<!--
That two-pass loop is still my favourite moment of the project. NuttX's software ADC FIFO holds eight samples, seven usable before full. Reading five samples—twenty-five bytes—leaves two behind, so the very next read of twenty-five blocks forever at seven! Wiping session context with /clear gave Pass 2 the cold eyes needed to catch Pass 1's side effect in Markdown. Once four Blockers and seven Majors were gone from the spec, Phase 5 implementation became delightfully boring: zero retries to build nuttx.uf2.
-->

---
layout: cast-full
title: "Way 1 Phase 5: First-Try Firmware Build & Breadboard SVG (05-implement.cast)"
cast: /casts/05-implement.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S26 -->
<!--
Handing the hardened v1 spec to Antigravity to implement xiao-rp2350-bargraph.md and build nuttx.uf2. At twenty-seven times speed: it wires the out-of-tree board overlays, writes bargraph_main.c and the pure math core, passes 6/6 host unit test suites first go, compiles nuttx.uf2 (347K) with zero retries, and updates 9 directory README.md contracts so the Elephant stays in sync. First-try firmware build done! Next we ask for docs/egm/breadboard-wiring.svg from our verified pin table. Because the spec already locked the non-ADC pins, it produces a working breadboard wiring diagram in one pass. Let's inspect the hardware result honestly.
-->

---
layout: default
---

<!-- S27 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">EGM&rsquo;s Generated Breadboard Wiring</h2>
</div>

<div class="w-full flex items-center justify-center">
  <img src="/images/breadboard-wiring.svg" alt="EGM Generated Breadboard Wiring SVG" class="w-full max-h-[430px] object-contain rounded-xl border border-gray-800 shadow-lg" />
</div>

<!--
Here is the full breadboard SVG EGM generated in one pass—potentiometer on D0 and six colour-coded LEDs on D1, D2, D4, D5, D8, and D9. Now let's zoom in on the XIAO RP2350 again, because we promised no unblemished triumph narratives.
-->

---
layout: hardware-callout
title: "Way 1 Scorecard: <code class='text-emerald-300'>10 / 10</code> Hardware Wins &amp; Candid Checks"
image: /images/breadboard-wiring-crop-ef.svg
imageCaption: "docs/egm/breadboard-wiring.svg — XIAO RP2350 drawn across Cols E & F (0.3&quot; trench, widened visually)"
---

<!-- S28 -->
<div class="space-y-3">
  <div class="p-3.5 rounded-xl bg-emerald-950/20 border border-emerald-500/35 space-y-1">
    <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">Primary Scorecard: Vanilla <code class="text-red-300">5/10</code> &rarr; EGM <code class="text-emerald-300">10/10</code></div>
    <p class="text-xs text-gray-200 m-0 leading-relaxed">
      <code class="text-emerald-300">0</code> traps hit on silicon (<code class="text-cyan-300">D1, D2, D4, D5, D8, D9</code>, <code class="text-emerald-300">IE=0</code> on <code class="text-gray-300">GPIO 26</code>, <code class="text-emerald-300">20 B</code> FIFO reads, <code class="text-emerald-300">23.6 mA</code> total) &amp; <code class="text-emerald-300">0</code> build retries.
    </p>
  </div>

  <div v-click class="p-3.5 rounded-xl bg-amber-950/20 border border-amber-500/35 space-y-1">
    <div class="text-xs font-mono text-amber-400 uppercase tracking-wide">Mechanical Catch (0.3&quot; vs. 0.6&quot; XIAO Header Spacing)</div>
    <p class="text-xs text-gray-200 m-0 leading-relaxed">
      Both runs missed the <strong class="text-white"><code class="text-amber-300">0.6&quot;</code> (<code class="text-gray-300">15.24 mm</code>)</strong> XIAO spacing (<code class="text-gray-300">Cols C &amp; G</code> or <code class="text-gray-300">D &amp; H</code>): Vanilla spanned <strong class="text-red-300">0.7&quot;</strong> (<code class="text-gray-300">C&ndash;H</code>), while EGM seated <strong class="text-amber-300">Cols E &amp; F (0.3&quot;)</strong> and masked it by widening the visual trench.
    </p>
  </div>

  <div v-click class="p-3.5 rounded-xl bg-gray-900/80 border border-gray-800 space-y-1">
    <div class="text-xs font-mono text-amber-300 uppercase tracking-wide">Candid Strict Audit: 6 Pass / 4 Partial / 0 Fail</div>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      Cold post-build Goldfish audit on the C code still caught <code class="text-amber-300">4</code> secondary polish items (<code class="text-gray-200">SIGTERM</code> cleanup, Makefile test scaffolding)&mdash;adversarial review is a habit, not magic.
    </p>
  </div>
</div>

<!--
Across all ten hardware and RTOS traps, Way 1 jumped from 5 out of 10 to 10 out of 10 with zero build retries. Yet keeping ourselves honest: first, while EGM nailed the electronics, a physical XIAO RP2350 has headers 0.6 inches apart—meaning Columns C and G, or D and H. Vanilla stretched to 0.7 inches, and EGM drew Columns E and F (0.3 inches) and widened the centre trench visually to fit! Second, a cold post-build Goldfish audit of the final C still found 6 Pass, 4 Partial, 0 Fail—reminding us that skills give massive leverage, not magic perfection. Now, Way 1 solved firmware delivery with a big bundled plugin. What happens when we want lightweight, composable community skills for planning and orchestration? That brings us to Way 2.
-->
