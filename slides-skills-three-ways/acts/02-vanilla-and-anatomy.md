---
layout: default
---

<!-- S09 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Baseline Foil: Unconstrained Antigravity (&ldquo;Weeds Syndrome&rdquo;)</h2>
</div>

<div class="p-5 rounded-xl bg-red-950/15 border border-red-500/30 mb-5">
  <div class="text-xs font-mono text-red-400 uppercase tracking-wide">Dave Rensin &middot; &ldquo;Weeds Syndrome&rdquo;</div>
  <div class="text-lg font-bold text-white mt-1">
    Diving head-first into writing implementation code before understanding system invariants.
  </div>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">The Zero-Skill Input</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Paste the complete functional bar-graph prompt straight into Antigravity with <code class="text-red-300">0</code> workflow skills, <code class="text-red-300">0</code> directory contracts, and <code class="text-red-300">0</code> review gates.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">What to Watch in <code class="text-cyan-300">vanilla-vibe.cast</code> (<code class="text-gray-300">1m 15s</code> warped)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Asks <strong class="text-white">1 quick pin question</strong> (<code class="text-gray-300">Q1</code>/<code class="text-gray-300">A1</code>), then charges into a <strong class="text-red-300">26-minute monolithic code-gen sprint</strong> (<code class="text-red-300">0</code> <code class="text-gray-300">README.md</code> reads) and generates a breadboard wiring SVG.</li>
    </ul>
  </li>
</ol>

<!--
Let's watch our baseline foil first—zero skills. Keep an eye out for what Dave Rensin calls "Weeds Syndrome": diving head-first into writing implementation C before checking a single subsystem invariant. We drop the complete prompt straight into Antigravity. Watch it ask one quick pin question, sprint through a twenty-six-minute build without reading a single README.md, and spit out a breadboard wiring SVG.
-->

---
layout: cast-full
title: "Baseline Foil: Vanilla Vibe Coding (vanilla-vibe.cast)"
cast: /casts/vanilla-vibe.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S10 -->
<!--
Here we are on the Antigravity start screen. Let's drop in the bar-graph prompt—potentiometer on D0/A0, six PWM LEDs, background daemon, and NSH CLI. Notice it probes the top-level directory, sees nuttx/ and apps/, and decides it already knows enough to ask one clarifying question: which six pins we want for the LEDs and whether they're active-high. I give it plenty of rope: "Pick any free breadboard pins you like, active-high, and generate the wiring guide." Off it goes. We're watching twenty-six raw minutes compressed at fifty-two times speed. Zero README.md reads, straight into creating Kconfig files, writing a monolithic C loop, running cmake, patching headers on the fly, and running seven host unit tests until nuttx.uf2 links cleanly. Green checkmarks everywhere—nuttx.uf2 built and seven out of seven unit tests passed! Now let's ask it to render breadboard_circuit.svg for our physical build. It writes a Python script and produces a dark-mode breadboard SVG. On screen, this looks like an unblemished triumph—until you pick up the actual hardware.
-->

---
layout: default
---

<!-- S11 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">Vanilla Vibe&rsquo;s Generated Breadboard Wiring</h2>
</div>

<div class="w-full flex items-center justify-center">
  <img src="/images/breadboard_circuit.svg" alt="Vanilla Vibe Generated Breadboard Wiring SVG" class="w-full max-h-[430px] object-contain rounded-xl border border-gray-800 shadow-lg" />
</div>

<!--
Here is the full breadboard wiring SVG that Vanilla Vibe just generated. At a glance, it looks super tidy—dark-mode styling, colour-coded jumper wires, labelled resistors. Now let's zoom straight in on the XIAO RP2350 itself to see what happens when you try to push it into real plastic.
-->

---
layout: hardware-callout
title: "Debrief: The Lie of &ldquo;It Compiled!&rdquo; (<code class='text-red-300'>5 / 10</code> Scorecard)"
image: /images/breadboard-vanilla-crop-ch.svg
imageCaption: "docs/vanilla/breadboard_circuit.svg — XIAO RP2350 stretched across 6 columns (Cols C to H)"
---

<!-- S12 -->
<div class="space-y-3.5">
  <div class="p-3.5 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-1">
    <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">Pre-Training Wins (5 / 10 Passes)</div>
    <p class="text-xs text-gray-200 m-0 leading-relaxed">
      Clean fixed-point Q4 EMA filter, proportional 6-segment duty math, <strong class="text-white">7/7 host unit tests passing</strong>, and a compiled <code class="text-emerald-300">nuttx.uf2</code> on session one.
    </p>
  </div>

  <div v-click class="p-3.5 rounded-xl bg-red-950/20 border border-red-500/35 space-y-1">
    <div class="text-xs font-mono text-red-400 uppercase tracking-wide">Physical &amp; Silicon Failures (5 Failures &middot; 0 README.md Reads)</div>
    <ul class="text-xs text-gray-200 space-y-1 list-disc pl-4 m-0 leading-relaxed">
      <li>Stretched a <code class="text-amber-300">0.3&quot;</code> module across <strong class="text-red-300">6 breadboard columns (Cols C&ndash;H, 0.7&quot;)</strong>, burying tie-points under the PCB!</li>
      <li>Tripped Erratum <code class="text-red-300">RP2350-E9</code> pad leakage (<code class="text-gray-300">IE=1</code> left enabled on <code class="text-gray-300">GPIO 26</code>).</li>
      <li>Looped <code class="text-red-300">open()</code>/<code class="text-red-300">close()</code> at <code class="text-gray-300">40 Hz</code> into the unguarded ISR teardown race &amp; dropped Kconfig symbols.</li>
    </ul>
  </div>
</div>

<!--
To be completely fair to Vanilla Vibe, everything in LLM pre-training weights worked nicely: fixed-point Q4 EMA math, proportional PWM slicing, seven out of seven host unit tests, and a compiled nuttx.uf2 binary. Yet against our ten real hardware and RTOS traps, it scored five out of ten. Look at the zoom on the left: it stretched the XIAO across six breadboard columns—Columns C to H—burying almost every tie-point underneath the PCB. Worse, because it read zero READMEs, it left IE=1 enabled on GPIO 26 (tripping RP2350-E9 leakage), hammered open() and close() at forty hertz straight into the ISR teardown race, and silently dropped Kconfig symbols. So how do we fix this without dumping seven million lines of C into a prompt? With Skills. Let's X-ray what a skill actually is.
-->

---
layout: skill-breakdown
way: mashup
title: "X-Raying a Skill: How <code class='text-purple-300'>skills/&lt;name&gt;/SKILL.md</code> Works"
---

<!-- S13 -->
<div class="space-y-3">
  <div class="text-xs text-gray-300">
    A skill is a folder (<code class="text-purple-300">.agents/skills/&lt;name&gt;/</code>) designed around <strong class="text-white">progressive disclosure</strong>, <strong class="text-white">task framing</strong>, and <strong class="text-white">deterministic tooling</strong>:
  </div>

```text
skills/my-skill/
├── SKILL.md        # YAML context pointer + ordered workflow steps & completion criteria
├── references/     # On-demand schemas & edge-case guides (disclosed only when branched)
├── scripts/        # Deterministic Python/bash tools instead of fragile LLM one-liners
└── resources/      # Starter templates (package.json, Vue layouts, HTML harnesses)
```

  <div class="grid grid-cols-3 gap-3 text-xs pt-1">
    <div v-click class="p-3 rounded-lg bg-gray-900/80 border border-purple-500/30 space-y-1">
      <div class="font-mono text-purple-300 font-bold">1. Progressive Disclosure</div>
      <p class="text-gray-300 m-0 leading-relaxed">
        Only YAML <code class="text-white">name</code> &amp; <code class="text-white">description</code> stay loaded in context&mdash;acting as a <strong class="text-white">context pointer</strong> with explicit trigger branches (or <code class="text-amber-300">disable-model-invocation: true</code> for zero-load slash commands).
      </p>
    </div>
    <div v-click class="p-3 rounded-lg bg-gray-900/80 border border-cyan-500/30 space-y-1">
      <div class="font-mono text-cyan-300 font-bold">2. Framing &amp; Execution</div>
      <p class="text-gray-300 m-0 leading-relaxed">
        Ordered steps end on checkable <strong class="text-white">completion criteria</strong> to stop premature rushing; branch-specific schemas move down into <code class="text-cyan-300">references/*.md</code>.
      </p>
    </div>
    <div v-click class="p-3 rounded-lg bg-gray-900/80 border border-emerald-500/30 space-y-1">
      <div class="font-mono text-emerald-300 font-bold">3. Deterministic Tool Use</div>
      <p class="text-gray-300 m-0 leading-relaxed">
        Instead of hoping an LLM parses ANSI escape sequences or C headers by eye, <code class="text-emerald-300">scripts/</code> and <code class="text-emerald-300">resources/</code> package exact verification gates.
      </p>
    </div>
  </div>
</div>

<!--
Before we tour the Three Ways, let's put a Skill under an X-ray. Physically, a skill is just a folder containing SKILL.md and optional references/, scripts/, and resources/ directories. First, Progressive Disclosure: only the YAML name and description stay pinned in the agent's context window. That description acts as a context pointer telling the model which branches should pull the full SKILL.md into context—or you set disable-model-invocation: true so it costs zero context tokens until a human types slash-name. Second, Context Framing and Execution: the body lays out ordered steps that end on explicit, checkable completion criteria so the model doesn't rush past verification, while heavy format rules are pushed down into references/. Third, Deterministic Tool Use: instead of asking an LLM to improvise fragile shell one-liners, scripts/ and resources/ give it battle-tested Python tools and templates. Now—here is the best part.
-->

---
layout: center
class: text-center
---

<!-- S14 -->
<div class="h-full flex flex-col items-center justify-center px-6 space-y-6">
  <blockquote class="text-3xl font-bold text-white tracking-tight leading-snug max-w-4xl">
    &ldquo;That schema is just an implementation detail,<br/>
    which the tools already know, thanks to skills.&rdquo;
  </blockquote>

  <p v-click class="text-base text-gray-300 max-w-2xl">
    Don&rsquo;t hand-write YAML frontmatter or directory conventions from memory&mdash;ask the meta-skills built to author skills:
  </p>

  <div v-click class="grid grid-cols-2 gap-6 w-full max-w-4xl text-left">
    <div class="p-5 rounded-xl bg-cyan-950/15 border border-cyan-500/30 space-y-2">
      <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Built-In Platform Guide</div>
      <h3 class="text-lg font-bold text-white font-mono m-0">/agy-customizations</h3>
      <p class="text-xs text-gray-300 m-0 leading-relaxed">
        Antigravity&rsquo;s built-in skill knows discovery roots (<code class="text-gray-200">.agents/skills/</code> vs <code class="text-gray-200">~/.gemini/config/</code>), loading precedence, progressive disclosure mechanics, and <code class="text-cyan-300">plugin.json</code> bundling.
      </p>
    </div>
    <div class="p-5 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-2">
      <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">Community Authoring Craft (mattpocock/skills)</div>
      <h3 class="text-lg font-bold text-white font-mono m-0">/writing-for-agents</h3>
      <p class="text-xs text-gray-300 m-0 leading-relaxed">
        Teaches the agent to sharpen context pointers, enforce checkable completion criteria, anchor behaviour with <strong class="text-white">leading words</strong>, avoid negations, and prune no-op sentences.
      </p>
    </div>
  </div>
</div>

<!--
Here is the secret that saves hours of squinting at docs: that entire folder schema is just an implementation detail, which the tools already know, thanks to skills. You never need to memorise YAML fields or discovery precedence from scratch. In Antigravity, the built-in /agy-customizations skill already knows where workspace skills live, how loading priority works, and how plugins bundle them. Pair that with Matt Pocock's /writing-for-agents skill, and the agent itself knows how to sharpen context pointers, write checkable completion criteria, and prune wasted tokens. Now let's see Skills Three Ways in action—starting with Way 1: adopting a bundled multi-agent plugin to beat our NuttX hardware traps.
-->
