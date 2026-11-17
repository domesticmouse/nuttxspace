---
class: text-center
---

<!-- S001 -->
<div class="h-full flex flex-col items-center justify-center">
  <h1 class="text-5xl font-extrabold tracking-tight text-white mb-4 leading-tight">
    Roll Your Own Docs
  </h1>
  <p class="text-2xl text-cyan-300 font-light max-w-2xl">
    Custom AI Skills for Rapid Tech Ramp-Up
  </p>

  <div class="mt-10 text-gray-300 text-lg">
    <span class="font-semibold text-white">Brett Morgan</span>
  </div>

  <div class="mt-10 text-xs text-gray-500">
    Press <kbd>Space</kbd> or <kbd>&rarr;</kbd> to advance
  </div>
</div>

<!--
G'day everyone. Tonight is about dropping a coding agent into an unfamiliar legacy codebase and helping you both understand what actually ships.
-->

---
layout: hardware-callout
title: "Weekend Silicon: Seeed Studio XIAO RP2350"
image: /images/breadboard-wiring.svg
imageCaption: "docs/egm/breadboard-wiring.svg — 10 kΩ potentiometer (Rows 1–3) & 6 PWM LEDs (Rows 14–19)"
---

<!-- S002 -->
<div class="space-y-4">
  <div class="p-4 rounded-xl bg-gray-900/70 border border-gray-800">
    <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide mb-1">The Bench Target</div>
    <p class="text-sm text-gray-200 m-0 leading-relaxed">
      A tiny Seeed Studio <strong class="text-white">XIAO RP2350</strong> mounted on a 400-tie-point solderless breadboard beside my laptop.
    </p>
  </div>

<!-- S003 -->
  <div v-click class="p-4 rounded-xl bg-cyan-950/20 border border-cyan-500/30 space-y-3">
    <div class="text-xs font-mono text-cyan-300 uppercase tracking-wide">Hardware Spec Callout</div>
    <ul class="text-sm text-gray-200 space-y-2 list-disc pl-5 m-0">
      <li><strong class="text-white">Silicon:</strong> Dual-core Cortex-M33 (<code class="text-cyan-300">RP2350</code>)</li>
      <li><strong class="text-white">Input:</strong> <code class="text-amber-300">D0/A0</code> (<code class="text-gray-200">GPIO 26</code>, <code class="text-gray-200">0–3.3 V</code>) potentiometer</li>
      <li><strong class="text-white">Output:</strong> <code class="text-emerald-300">6</code> hardware PWM LEDs (<code class="text-gray-200">330 Ω</code>, <code class="text-gray-200">~3.94 mA</code> each)</li>
    </ul>
  </div>
</div>

<!--
It started with weekend hardware curiosity: a tiny AU$ Seeed Studio XIAO RP2350 board on a solderless breadboard beside my laptop. Nothing exotic on the bench—just one potentiometer wired into D0/A0 driving a smooth six-LED bar graph via hardware PWM.
-->

---
layout: default
---

<!-- S004 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The RTOS Scale Shock: Apache NuttX</h2>
</div>

<div class="grid grid-cols-2 gap-6 mt-8">
  <div class="p-6 rounded-xl bg-gray-900/80 border border-cyan-500/30 space-y-3">
    <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Kernel Submodule</div>
    <div class="text-3xl font-extrabold font-mono text-white">nuttx/</div>
    <p class="text-sm text-gray-300 leading-relaxed m-0">
      Real-time POSIX microkernel, board support packages, Kconfig build system, and <code class="text-cyan-300">rp23xx</code> silicon drivers.
    </p>
  </div>

  <div class="p-6 rounded-xl bg-gray-900/80 border border-amber-500/30 space-y-3">
    <div class="text-xs font-mono text-amber-400 uppercase tracking-wide">User-Space Submodule</div>
    <div class="text-3xl font-extrabold font-mono text-white">apps/</div>
    <p class="text-sm text-gray-300 leading-relaxed m-0">
      NuttShell (<code class="text-amber-300">nsh&gt;</code>), system utilities, external application hooks, and flat-build user daemons.
    </p>
  </div>
</div>

<div class="mt-6 p-4 rounded-xl bg-gray-900/50 border border-gray-800 text-center text-base text-gray-200">
  <strong class="text-white">30 years of legacy C</strong> across two deep upstream submodules.
</div>

<!--
The catch is running it inside Apache NuttX, thirty years of deep POSIX RTOS C that I have definitely not memorised.
-->

---
layout: default
---

<!-- S005 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Two Starting Documentation Baselines</h2>
</div>

<div class="grid grid-cols-2 gap-8 mt-8 items-stretch">
  <div class="p-8 rounded-2xl bg-red-950/15 border border-red-500/35 flex flex-col justify-between">
    <div>
      <div class="text-xs font-mono text-red-400 uppercase tracking-wider">Upstream nuttx + apps master</div>
      <div class="text-7xl font-black text-red-400 mt-4 mb-2 font-mono">13</div>
      <div class="text-xl font-bold text-white"><code class="text-red-300">README.md</code> files total</div>
    </div>
    <p class="text-xs text-gray-400 mt-6 m-0 font-mono">
      docs/reports/vanilla-vs-egm-comparison.md#L33
    </p>
  </div>

<!-- S006 -->
  <div v-click class="p-8 rounded-2xl bg-emerald-950/15 border border-emerald-500/35 flex flex-col justify-between">
    <div>
      <div class="text-xs font-mono text-emerald-400 uppercase tracking-wider">Contract-Mapped egm Branch</div>
      <div class="text-7xl font-black text-emerald-400 mt-4 mb-2 font-mono">3,241</div>
      <div class="text-xl font-bold text-white"><code class="text-emerald-300">README.md</code> contracts</div>
    </div>
    <div class="text-sm text-gray-300 mt-6 space-y-1 font-mono">
      <div><strong class="text-white">2,607</strong> in <code class="text-emerald-300">nuttx/</code> + <strong class="text-white">634</strong> in <code class="text-emerald-300">apps/</code></div>
    </div>
  </div>
</div>

<!--
Look at upstream master across those two submodules: thirteen README.md files total. Drop an agent there cold and it flies blind. Now compare that against our contract-mapped branch: three thousand, two hundred and forty-one directory README.md contracts waiting to be navigated.
-->

---
layout: default
---

<!-- S007 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Hook &amp; Controlled Experiment</h2>
</div>

<div class="p-8 rounded-2xl bg-gray-900/70 border border-gray-800 space-y-5 text-center mt-4">
  <p class="text-2xl font-semibold text-emerald-300 m-0 leading-snug">
    &ldquo;The joy is new silicon on your desk;
  </p>
  <p class="text-2xl font-semibold text-red-300 m-0 leading-snug">
    the pain is your coding agent waking up blind.&rdquo;
  </p>
</div>

<!-- S008 -->
<div v-click class="grid grid-cols-2 gap-6 mt-8">
  <div class="p-5 rounded-xl bg-red-950/15 border border-red-500/30">
    <div class="text-xs font-mono text-red-400 uppercase tracking-wide">Experiment A</div>
    <div class="text-lg font-bold text-white font-mono mt-1">nuttxspace-vanilla</div>
    <div class="text-sm text-gray-300 mt-1">Baseline: <strong class="text-white">13</strong> upstream <code class="text-red-300">README.md</code> docs</div>
  </div>

  <div class="p-5 rounded-xl bg-emerald-950/15 border border-emerald-500/30">
    <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">Experiment B</div>
    <div class="text-lg font-bold text-white font-mono mt-1">nuttxspace-egm</div>
    <div class="text-sm text-gray-300 mt-1">Baseline: <strong class="text-white">3,241</strong> directory <code class="text-emerald-300">README.md</code> contracts</div>
  </div>
</div>

<div class="mt-4 text-xs font-mono text-gray-500 text-center">
  Same hardware &middot; Same prompt &middot; Recorded end-to-end (<span class="text-gray-300">EXPERIMENT_RUNBOOK.md</span>)
</div>

<!--
The joy is fresh silicon on your desk; the pain is watching your AI accomplice wake up blind inside legacy C. So I ran a controlled experiment: exact same RP2350 hardware, exact same prompt, recorded end-to-end across two separate workspaces.
-->

---
layout: default
---

<!-- S009 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">The Shared Functional Prompt</h2>
</div>

<div class="p-4 rounded-xl bg-gray-900/80 border border-cyan-500/30 mb-5">
  <div class="flex items-center justify-between text-xs font-mono text-cyan-300">
    <span>EXPERIMENT_RUNBOOK.md &middot; Verbatim Brief</span>
    <span class="text-gray-400">Handed identically to both workspaces</span>
  </div>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">

<!-- S010 -->
  <div v-click>
    <li>
      <strong class="text-white">Constraint 1 — Submodule Hygiene</strong>
      <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
        <li>Out-of-tree <code class="text-cyan-300">xiao-rp2350-bargraph/</code> custom board &amp; app</li>
        <li><code class="text-emerald-300">git -C nuttx status</code> and <code class="text-emerald-300">git -C apps status</code> remain spotless</li>
      </ul>
    </li>
  </div>

<!-- S011 -->
  <div v-click>
    <li>
      <strong class="text-white">Constraint 2 — Firmware Behaviour</strong>
      <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
        <li><code class="text-amber-300">D0/A0</code> potentiometer driving 6 PWM LEDs with autonomous power-on boot</li>
        <li>Interactive USB-C <code class="text-cyan-300">nsh&gt;</code> CLI (<code class="text-gray-200">start</code>, <code class="text-gray-200">stop</code>, <code class="text-gray-200">status</code>, <code class="text-gray-200">test</code>, <code class="text-gray-200">set</code>, <code class="text-gray-200">resume</code>)</li>
      </ul>
    </li>
  </div>

<!-- S012 -->
  <div v-click>
    <li>
      <strong class="text-white">Constraint 3 — Deliverables</strong>
      <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
        <li>Host <code class="text-emerald-300">make test</code> + flashable <code class="text-emerald-300">nuttx/nuttx.uf2</code></li>
        <li>Markdown &amp; SVG breadboard wiring guide</li>
      </ul>
    </li>
  </div>

</ol>

<!--
We did not rig the prompt. Both workspaces received the identical functional specification straight out of EXPERIMENT_RUNBOOK.md. Constraint one: everything lives out-of-tree in xiao-rp2350-bargraph/, leaving both upstream nuttx/ and apps/ git submodules completely untouched. Constraint two: power on cleanly without a host computer, track the potentiometer smoothly, and expose an interactive USB serial command line. Constraint three: ship host unit tests, a flashable nuttx.uf2 image, plus a Markdown and SVG breadboard wiring diagram.
-->

---
layout: default
---

<!-- S013 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Comforting Illusion</h2>
</div>

<div class="mt-8 p-8 rounded-2xl bg-emerald-950/20 border-2 border-emerald-500/50 text-center space-y-3">
  <div class="text-xs font-mono text-emerald-400 uppercase tracking-widest">Experiment A &amp; Experiment B</div>
  <div class="text-4xl font-black font-mono text-emerald-300">
    BUILD SUCCEEDED: nuttx/nuttx.uf2
  </div>
</div>

<!-- S014 -->
<div v-click class="mt-8 p-6 rounded-xl bg-gray-900/80 border border-gray-800 space-y-4">
  <div class="flex items-center justify-between text-sm text-gray-200">
    <strong class="text-white">It Blinks!</strong>
    <span class="font-mono text-xs text-amber-300">Potentiometer 0% &rarr; 100%</span>
  </div>
  <div class="grid grid-cols-6 gap-3">
    <div class="h-10 rounded-lg bg-emerald-500/80 border border-emerald-300 flex items-center justify-center font-mono text-xs font-bold text-gray-950">LED 1</div>
    <div class="h-10 rounded-lg bg-emerald-500/80 border border-emerald-300 flex items-center justify-center font-mono text-xs font-bold text-gray-950">LED 2</div>
    <div class="h-10 rounded-lg bg-emerald-500/80 border border-emerald-300 flex items-center justify-center font-mono text-xs font-bold text-gray-950">LED 3</div>
    <div class="h-10 rounded-lg bg-amber-400/85 border border-amber-200 flex items-center justify-center font-mono text-xs font-bold text-gray-950">LED 4</div>
    <div class="h-10 rounded-lg bg-amber-400/85 border border-amber-200 flex items-center justify-center font-mono text-xs font-bold text-gray-950">LED 5</div>
    <div class="h-10 rounded-lg bg-red-500/85 border border-red-300 flex items-center justify-center font-mono text-xs font-bold text-gray-950">LED 6</div>
  </div>
</div>

<!--
Here is the trap: both the unprimed vibe-coding run and the full contract run reached this comforting green build badge. You drag the .uf2 onto the RP2350, twist the pot, and all six LEDs light up across the breadboard.
-->

---
layout: default
---

<!-- S015 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">The Lie of &ldquo;It Compiled!&rdquo;</h2>
</div>

<div class="mt-6 p-8 rounded-2xl bg-gray-900/70 border-2 border-red-500/40 text-center">
  <blockquote class="text-2xl font-semibold text-white tracking-tight leading-snug max-w-3xl mx-auto m-0">
    &ldquo;In embedded systems&mdash;and any deep legacy codebase&mdash;&lsquo;it compiles and blinks&rsquo; is the most dangerous lie an AI accomplice can tell you.&rdquo;
  </blockquote>
</div>

<!-- S016 -->
<div v-click class="grid grid-cols-2 gap-6 mt-6">
  <div class="p-4 rounded-xl bg-red-950/30 border border-red-500/50 flex items-center gap-3">
    <span class="px-2.5 py-1 rounded bg-red-500/20 border border-red-400/40 text-xs font-mono font-bold text-red-300">TIMEBOMB 1</span>
    <span class="text-sm font-semibold text-white"><code class="text-red-300">RP2350-E9</code> IE=1 Pad Leakage</span>
  </div>

  <div class="p-4 rounded-xl bg-red-950/30 border border-red-500/50 flex items-center gap-3">
    <span class="px-2.5 py-1 rounded bg-red-500/20 border border-red-400/40 text-xs font-mono font-bold text-red-300">TIMEBOMB 2</span>
    <span class="text-sm font-semibold text-white"><code class="text-red-300">40 Hz</code> ISR Teardown Race</span>
  </div>
</div>

<!--
In embedded systems—and any deep legacy codebase—"it compiles and blinks" is the most dangerous lie an AI accomplice can tell you. Underneath that happy blinking demo hid a silicon erratum corruption bug and a forty-hertz interrupt teardown panic waiting to fire.
-->

---
layout: default
---

<!-- S017 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Foundational Lineage: The Elephant-Goldfish Model</h2>
</div>

<div class="p-5 rounded-xl bg-gray-900/80 border border-gray-800 space-y-2">
  <div class="flex items-center justify-between">
    <span class="text-lg font-bold text-white">Dave Rensin (<code class="text-cyan-300">@drensin</code>)</span>
    <span class="text-xs font-mono text-cyan-400">github.com/google/elephant-goldfish-model</span>
  </div>
  <p class="text-sm text-gray-300 m-0">
    <a href="https://drensin.medium.com/elephants-goldfish-and-the-new-golden-age-of-software-engineering-c33641a48874" target="_blank" class="text-cyan-300 underline">
      &ldquo;Elephants, Goldfish, and the New Golden Age of Software Engineering&rdquo;
    </a>
  </p>
</div>

<!-- S018 -->
<div v-click class="space-y-4 mt-6">
  <div class="p-5 rounded-xl bg-cyan-950/20 border border-cyan-500/35">
    <div class="text-xs font-mono text-cyan-400 uppercase tracking-wider mb-1">Mantra #1</div>
    <div class="text-2xl font-bold text-white">&ldquo;Design is the new code.&rdquo;</div>
  </div>

  <div class="p-5 rounded-xl bg-amber-950/20 border border-amber-500/35">
    <div class="text-xs font-mono text-amber-400 uppercase tracking-wider mb-1">Mantra #2</div>
    <div class="text-2xl font-bold text-white">&ldquo;Design with an elephant. Review with a goldfish.&rdquo;</div>
  </div>
</div>

<!--
To stop shipping hidden timebombs, I turned to Dave Rensin's Elephant-Goldfish Model from Google's open-source elephant-goldfish-model plugin. Dave captures the mindset in two lines: design is the new code; design with an elephant, review with a goldfish.
-->

---
layout: default
---

<!-- S019 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Three Layers of &ldquo;Roll Your Own Docs&rdquo;</h2>
</div>

<div class="space-y-4 mt-4">
  <div class="p-4 rounded-xl bg-cyan-950/20 border border-cyan-500/35">
    <div class="text-xs font-mono text-cyan-300 uppercase">Layer 1 &mdash; Docs for the Elephant &amp; Goldfish</div>
    <div class="text-base font-bold text-white mt-1">3,241 bottom-up <code class="text-cyan-300">README.md</code> contracts + EGM specs</div>
    <p class="text-xs text-gray-300 mt-1 m-0">Drift report, brainstorm brief, design drafts, and two-pass Goldfish reviews in <code class="text-cyan-300">docs/egm/</code>.</p>
  </div>

<!-- S020 -->
  <div v-click class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/35">
    <div class="text-xs font-mono text-emerald-300 uppercase">Layer 2 &mdash; Docs for the Human (Who Owns the Outcome)</div>
    <div class="text-base font-bold text-white mt-1">6-lesson interactive Edward Tufte codelab via <code class="text-emerald-300">/teach-code</code></div>
    <p class="text-xs text-gray-300 mt-1 m-0"><code class="text-emerald-300">docs/egm/tutorial/index.md</code> compiled to HTML with 3 live simulators, 4 bench sheets, and 6 strict C99 labs.</p>
  </div>

<!-- S021 -->
  <div v-click class="p-4 rounded-xl bg-purple-950/20 border border-purple-500/35">
    <div class="text-xs font-mono text-purple-300 uppercase">Layer 3 &mdash; Docs for the Workflow (Skills That Evolve)</div>
    <div class="text-base font-bold text-white mt-1">Matt Pocock&rsquo;s <code class="text-purple-300">/teach</code> &rarr; <code class="text-purple-300">/teach-code</code> via <code class="text-purple-300">/agy-customizations</code></div>
    <p class="text-xs text-gray-300 mt-1 m-0">Real-world compiler &amp; HTML gates (<code class="text-purple-300">build.py --check</code>, <code class="text-purple-300">sync_listing.py</code>) folded upstream into <code class="text-purple-300">SKILL.md</code>.</p>
  </div>
</div>

<!--
That gives us layer one of "Roll Your Own Docs": build contracts and specs so your coding agents stop guessing. Layer two rolls docs for the human who owns the outcome: a six-lesson interactive codelab so you actually understand the firmware. And layer three rolls docs for your own workflow, evolving Matt Pocock's /teach skill into /teach-code through real-world use.
-->

---
layout: default
---

<!-- S022 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Three-Layer Reinforcement Loop</h2>
</div>

<div class="grid grid-cols-3 gap-4 mt-8 items-center">
  <div class="p-5 rounded-xl bg-cyan-950/20 border border-cyan-500/40 text-center space-y-2">
    <div class="text-xs font-mono text-cyan-300 uppercase">Step 1</div>
    <div class="text-lg font-bold text-white">1. Agent Contracts</div>
    <div class="text-xs text-gray-300">3,241 <code class="text-cyan-300">README.md</code> + EGM Specs</div>
  </div>

  <div class="p-5 rounded-xl bg-emerald-950/20 border border-emerald-500/40 text-center space-y-2">
    <div class="text-xs font-mono text-emerald-300 uppercase">Step 2</div>
    <div class="text-lg font-bold text-white">2. Human Codelab</div>
    <div class="text-xs text-gray-300">Tufte Course + Strict C99 Labs</div>
  </div>

  <div class="p-5 rounded-xl bg-purple-950/20 border border-purple-500/40 text-center space-y-2">
    <div class="text-xs font-mono text-purple-300 uppercase">Step 3</div>
    <div class="text-lg font-bold text-white">3. Living <code class="text-purple-300">SKILL.md</code></div>
    <div class="text-xs text-gray-300">Deterministic Gates Upstream</div>
  </div>
</div>

<div class="mt-4 text-center text-xs font-mono text-gray-400">
  Agent Contracts &rarr; Human Codelab &rarr; Living <code class="text-purple-300">SKILL.md</code> &rarr; Next Codebase
</div>

<!-- S023 -->
<div v-click class="mt-8 p-5 rounded-xl bg-red-950/20 border border-red-500/40 text-center">
  <div class="text-xs font-mono text-red-400 uppercase tracking-wider">Next &middot; Act II</div>
  <div class="text-xl font-bold text-white mt-1">Experiment A: <code class="text-red-300">nuttxspace-vanilla</code> &mdash; What Happens When You Skip All Three</div>
</div>

<!--
Together, those three layers create a tight loop: grounded code, genuine human comprehension, and sharper custom skills for the next repo. Let's roll the tape on what happens when you skip all three layers and vibe code straight into C.
-->
