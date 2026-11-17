---
layout: default
---

<!-- S06 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Act II Setup: Watching Unconstrained Antigravity (&ldquo;Weeds Syndrome&rdquo;)</h2>
</div>

<div class="p-5 rounded-xl bg-red-950/15 border border-red-500/30 mb-5">
  <div class="text-xs font-mono text-red-400 uppercase tracking-wide">Dave Rensin &middot; &ldquo;Weeds Syndrome&rdquo;</div>
  <div class="text-lg font-bold text-white mt-1">
    Diving head-first into writing implementation code before understanding system invariants.
  </div>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">The Input</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Paste the complete functional bar-graph prompt straight into Antigravity with zero directory contracts or review gates.</li>
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
Let's watch Experiment A first. Keep an eye out for what Google's Dave Rensin calls "Weeds Syndrome"—diving head-first into writing C code before knowing a single subsystem invariant. We paste the functional prompt straight into Antigravity with zero directory contracts. Watch it ask one quick pin question, sprint through a twenty-six-minute monolithic build without reading a single README.md, and spit out a breadboard wiring SVG.
-->

---
layout: cast-full
title: "Experiment A: Vanilla Vibe Coding (vanilla-vibe.cast)"
cast: /casts/vanilla-vibe.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S07 -->
<!--
Here we are on the Antigravity start screen. Let's drop in the bar graph prompt. There's the full prompt—potentiometer on D0/A0, six PWM LEDs, background daemon, and NSH CLI. Let's send it. Notice it immediately probes the top-level directory, sees nuttx/ and apps/, and decides it already knows enough to ask one clarifying question. It asks which six pins we want for the LEDs and whether they're active-high. I give it rope: "Pick any free breadboard pins you like, active-high, and generate the wiring guide." Off it goes. We're watching twenty-six raw minutes compressed at fifty-two times speed. This is textbook Weeds Syndrome: zero README.md reads, straight into creating Kconfig files, writing a monolithic C loop, running cmake, hitting compiler errors, patching headers on the fly, and running seven host unit tests until nuttx.uf2 links cleanly. Green checkmarks everywhere—nuttx.uf2 built and seven out of seven unit tests passed! Now let's ask for the breadboard wiring SVG. Asking it to render breadboard_circuit.svg for our physical build. It writes a Python generator and renders a gorgeous-looking dark-mode breadboard SVG. Everything looks like an unblemished triumph—until you pick up the actual hardware. Done. Let's inspect what it actually built.
-->

---
layout: hardware-callout
title: "Debrief: The Lie of “It Compiled!” (Cols C–H 0.7&quot; Straddle)"
image: /images/breadboard-vanilla-crop-ch.svg
imageCaption: "docs/vanilla/breadboard_circuit.svg — XIAO RP2350 stretched across 6 columns (Cols C to H)"
---

<!-- S08 -->
<div class="space-y-4">
  <div v-click class="p-4 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-1.5">
    <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">Fair Credit on Host</div>
    <p class="text-sm text-gray-200 m-0 leading-relaxed">
      Clean C fixed-point math, <strong class="text-white">7/7 host unit tests passing</strong>, and a compiled <code class="text-emerald-300">nuttx.uf2</code> binary on the first session.
    </p>
  </div>

  <div v-click class="p-4 rounded-xl bg-red-950/20 border border-red-500/35 space-y-1.5">
    <div class="text-xs font-mono text-red-400 uppercase tracking-wide">Physical Reality Check (0.7&quot; Straddle Across Cols C&ndash;H)</div>
    <p class="text-sm text-gray-200 m-0 leading-relaxed">
      Stretched a <code class="text-amber-300">0.3&quot;</code> wide module across <strong class="text-red-300">6 breadboard columns (Cols C to H)</strong>&mdash;burying almost every tie-point underneath the fibreglass PCB!
    </p>
  </div>
</div>

<!--
Here is the SVG Vanilla Vibe just generated. To be completely fair to Vanilla Vibe, the host C math and unit test scaffolding were genuinely tidy, and nuttx.uf2 compiled on the first session. Except look closely at where it seated the XIAO module on the breadboard: it stretched a zero-point-three-inch module across six columns—Columns C to H! On a physical breadboard, that buries almost every tie-point underneath the fibreglass PCB.
-->

---
layout: default
---

<!-- S09 -->
<div class="flex items-center justify-between mb-6">
  <h2 class="text-2xl font-bold m-0">Vanilla Vibe Audit Scorecard: <code class="text-red-300">5 / 10</code> Against the Hardware Traps</h2>
  <span class="px-3 py-1 rounded-lg bg-red-950/30 border border-red-500/40 text-sm font-mono text-red-300 font-bold">0 README.md Reads</span>
</div>

<div class="grid grid-cols-2 gap-6 mt-4">
  <div v-click class="p-5 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-3">
    <div class="text-xs font-mono text-emerald-400 uppercase">Where Vanilla Shone (5 Passes &middot; Pre-Training Strengths)</div>
    <h3 class="text-lg font-bold text-white m-0">Generic C &amp; Host Logic</h3>
    <ul class="text-xs text-gray-300 space-y-2 list-disc pl-4 m-0 leading-relaxed">
      <li>Fixed-point Q4 exponential moving average (EMA) filter</li>
      <li>Proportional 6-segment duty-cycle slicing math</li>
      <li>Clean host unit test structure (<code class="text-emerald-300">7/7</code> passing)</li>
      <li>Basic POSIX CLI argument parsing</li>
    </ul>
  </div>

  <div v-click class="p-5 rounded-xl bg-red-950/15 border border-red-500/30 space-y-3">
    <div class="text-xs font-mono text-red-400 uppercase">Where Vanilla Failed on Silicon (5 Failures &middot; Unread Invariants)</div>
    <h3 class="text-lg font-bold text-white m-0">RP2350 &amp; NuttX Traps</h3>
    <ul class="text-xs text-gray-300 space-y-2 list-disc pl-4 m-0 leading-relaxed">
      <li>Tripped Erratum <code class="text-red-300">RP2350-E9</code> pad leakage (<code class="text-gray-300">IE=1</code> left enabled on <code class="text-gray-300">GPIO 26</code>)</li>
      <li>Looped <code class="text-red-300">open()</code>/<code class="text-red-300">close()</code> at <code class="text-gray-300">40 Hz</code> into the unguarded ISR teardown race</li>
      <li>Straddled breadboard <strong class="text-white">Columns C&ndash;H</strong> &amp; silently dropped Kconfig symbols</li>
    </ul>
  </div>
</div>

<!--
When we audited Vanilla Vibe against ten real hardware and RTOS invariants, it scored five out of ten. Every pass was generic C knowledge baked into LLM pre-training: fixed-point Q4 EMA filtering, proportional segment slicing, and host unit tests. Every failure was an RP2350 or NuttX invariant it never read: RP2350-E9 pad leakage, looping open() and close() at forty hertz into the ISR teardown race, and dropped Kconfig symbols.
-->
