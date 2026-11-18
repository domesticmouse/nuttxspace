---
layout: center
class: text-center
---

<!-- S01 -->
<div class="h-full flex flex-col items-center justify-center">
  <h1 class="text-5xl font-extrabold tracking-tight text-white mb-4 leading-tight">
    Skills Three Ways
  </h1>
  <p class="text-2xl text-gray-300 font-light max-w-3xl">
    Adopting, Orchestrating, and Authoring Modular AI Workflows on Real Silicon
  </p>

  <div class="mt-8 flex items-center gap-3 text-xs font-mono">
    <span class="px-3 py-1 rounded-full badge-way1">1. Adopt Plugins</span>
    <span class="text-gray-500">&rarr;</span>
    <span class="px-3 py-1 rounded-full badge-way2">2. Orchestrate Community Skills</span>
    <span class="text-gray-500">&rarr;</span>
    <span class="px-3 py-1 rounded-full badge-way3">3. Author Custom Skills</span>
  </div>

  <div class="mt-10 text-gray-400 text-base">
    <span class="font-semibold text-gray-200">Brett Morgan</span> &middot; Developer Relations Engineer, Google Cloud
  </div>

  <div class="mt-10 text-xs text-gray-500">
    Press <kbd>Space</kbd> or <kbd>&rarr;</kbd> to advance
  </div>
</div>

<!--
G'day everyone, I'm Brett Morgan from Google Cloud. Today we're talking about Skills Three Ways—how modular AI skills let us move from blind prompt-and-pray coding to repeatable engineering workflows on real hardware. To stress-test this properly, we're not building another toy todo app: we're dropping a coding agent into twenty years of unfamiliar Apache NuttX C code targeting an AU$10 microcontroller sitting on a breadboard, and watching three distinct ways to use skills—plus the skills that built this very presentation.
-->

---
layout: default
---

<!-- S02 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Stack: Apache NuttX &amp; Google Antigravity</h2>
</div>

<div class="grid grid-cols-2 gap-8 items-stretch mt-4">
  <div class="p-6 rounded-xl bg-gray-900/80 border border-gray-800 flex flex-col items-center justify-between text-center gap-5">
    <div class="w-full h-52 rounded-lg bg-white/95 flex items-center justify-center p-5">
      <img src="/images/nuttx-logo.png" alt="Apache NuttX RTOS Logo" class="max-h-full max-w-full object-contain" />
    </div>
    <div class="space-y-1.5">
      <div class="text-sm text-gray-300">Real-Time Operating System</div>
      <a href="https://nuttx.apache.org/" target="_blank" rel="noopener noreferrer" class="inline-block text-base font-mono text-cyan-300 hover:text-cyan-200 underline underline-offset-4">
        https://nuttx.apache.org/
      </a>
    </div>
  </div>

  <div class="p-6 rounded-xl bg-gray-900/80 border border-gray-800 flex flex-col items-center justify-between text-center gap-5">
    <div class="w-full h-52 rounded-lg bg-white/95 flex items-center justify-center p-5">
      <img src="/images/antigravity-icon__full-color.png" alt="Google Antigravity Logo" class="max-h-full max-w-full object-contain" />
    </div>
    <div class="space-y-1.5">
      <div class="text-sm text-gray-300">Agentic Development Platform</div>
      <a href="https://antigravity.google/" target="_blank" rel="noopener noreferrer" class="inline-block text-base font-mono text-cyan-300 hover:text-cyan-200 underline underline-offset-4">
        https://antigravity.google/
      </a>
    </div>
  </div>
</div>

<!--
Before we touch any wires, here are the two systems at the centre of our experiment. On the left is Apache NuttX, a mature, POSIX-compliant real-time operating system you can explore at nuttx.apache.org. On the right is Google Antigravity, the agentic development platform we'll drive—from zero skills all the way through bundled plugins, community skills, and custom workflows.
-->

---
layout: default
---

<!-- S03 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Apache NuttX at a Glance: Project Scale &amp; Lineage</h2>
</div>

<div class="grid grid-cols-2 gap-5 mt-2">
  <div class="p-5 rounded-xl bg-cyan-950/15 border border-cyan-500/30 space-y-2">
    <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Age &amp; Commit Velocity</div>
    <div class="text-2xl font-extrabold text-white font-mono">~20 Years &middot; 75,800+ Commits</div>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      Founded in <strong class="text-white">February 2007</strong> by Gregory Nutt (<code class="text-cyan-300">66,144</code> kernel commits + <code class="text-cyan-300">9,674</code> <code class="text-gray-300">apps/</code> commits from <code class="text-cyan-300">1,239+</code> contributors; Apache Top-Level Project).
    </p>
  </div>

  <div v-click class="p-5 rounded-xl bg-gray-900/80 border border-gray-800 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">C Source &amp; Header Footprint</div>
    <div class="text-2xl font-extrabold text-white font-mono">12,139 <code class="text-emerald-300">.c</code> &amp; 8,801 <code class="text-emerald-300">.h</code> Files</div>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      <strong class="text-white">~7.65M lines</strong> across <code class="text-gray-300">20,940</code> C/header files (<code class="text-emerald-300">10,322</code> <code class="text-gray-300">.c</code> files / <code class="text-gray-300">4.85M</code> LOC in <code class="text-gray-300">nuttx/</code> + <code class="text-emerald-300">1,817</code> <code class="text-gray-300">.c</code> files / <code class="text-gray-300">670K</code> LOC in <code class="text-gray-300">apps/</code>).
    </p>
  </div>

  <div v-click class="p-5 rounded-xl bg-gray-900/80 border border-gray-800 space-y-2">
    <div class="text-xs font-mono text-amber-400 uppercase tracking-wide">Hardware &amp; Build Surface</div>
    <div class="text-2xl font-extrabold text-white font-mono">19 CPU Archs &middot; 473 Boards</div>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      Configured via <code class="text-amber-300">1,569</code> <code class="text-gray-300">Kconfig</code> files spanning <code class="text-amber-300">3,241</code> directories&mdash;scaling from 8-bit microcontrollers up to multi-core 64-bit ARM, RISC-V, and x86_64 SoCs.
    </p>
  </div>

  <div v-click class="p-5 rounded-xl bg-amber-950/15 border border-amber-500/30 space-y-2">
    <div class="text-xs font-mono text-amber-300 uppercase tracking-wide">Strict POSIX Architecture</div>
    <div class="text-2xl font-extrabold text-white font-mono">Everything Is a File (<code class="text-cyan-300">/dev/*</code>)</div>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      Split upper-half VFS character drivers + lower-half silicon HALs (<code class="text-gray-300">open</code>, <code class="text-gray-300">read</code>, <code class="text-gray-300">ioctl</code>, <code class="text-gray-300">poll</code>), interactive <code class="text-cyan-300">nsh&gt;</code> shell, and only <strong class="text-red-300">13 upstream <code class="text-gray-300">README.md</code> files</strong>.
    </p>
  </div>
</div>

<!--
To see why unguided vibe coding struggles here, look at the scale of Apache NuttX. Founded by Gregory Nutt back in February 2007, it holds more than seventy-five thousand commits across twelve thousand C source files and nearly nine thousand headers—roughly seven-point-six million lines of code across nineteen CPU architectures and four hundred and seventy-three boards. All of that sits across three thousand two hundred and forty-one directories, wired by fifteen hundred Kconfig files—yet upstream ships with just thirteen README.md files. That is zero-point-four percent directory coverage.
-->

---
layout: default
---

<!-- S04 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">How a Solderless Breadboard Works</h2>
</div>

<div class="w-full flex items-center justify-center">
  <img src="/images/breadboard-scopes.svg" alt="Solderless Breadboard Internal Connections" class="w-full max-h-[430px] object-contain rounded-xl border border-gray-800 shadow-lg" />
</div>

<!--
And on the hardware side, we are wiring everything on a solderless breadboard beside the laptop. If you haven't touched one in a while, its hidden metal spring clips behave just like variable scopes in code. Top and bottom horizontal rails act as global scope, carrying ground and 3.3 volts along the length of the board. Inside, each five-hole vertical column—rows a to e above the trench, and f to j below it—is an independent local node. That centre plastic trench is a hard namespace boundary: you seat a dual-row board straddling e and f so opposite pins never short together.
-->

---
layout: center
---

<!-- S05 -->
<div class="w-full h-full flex items-center justify-center">
  <img src="/images/assets-breadboard-wiring.svg" alt="Physical Breadboard Wiring: XIAO RP2350 6-LED Bar Graph" class="w-full max-h-[500px] object-contain rounded-xl border border-gray-800 shadow-lg" />
</div>

<!--
Here is the physical wiring layout on a 28-column breadboard. Our XIAO RP2350 straddles the centre trench, leaving open tie-points on both sides of every pin. On the left, a ten-kilohm potentiometer feeds D0/A0 at 3.3 volts. Across the right, six discrete LEDs—two green, two yellow, two red—each get their own 330-ohm current-limiting resistor on pins D3 through D5 and D8 through D10. Notice we deliberately skip D1 and D2, and you're about to see why.
-->

---
layout: default
---

<!-- S06 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Hardware Target: XIAO RP2350 &amp; 6-LED Bar Graph</h2>
</div>

<div class="space-y-4">
  <div class="p-4 rounded-xl bg-gray-900/80 border border-gray-800 space-y-1.5">
    <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Bench Hardware</div>
    <p class="text-sm text-gray-200 m-0 leading-relaxed">
      <strong class="text-white">AU$10 Seeed Studio XIAO RP2350</strong> + <code class="text-amber-300">10 k&Omega;</code> linear potentiometer on <code class="text-amber-300">D0/A0</code> (<code class="text-gray-300">GPIO 26 / ADC0</code>) + <strong class="text-white">6 through-hole LEDs</strong> (2 Green, 2 Yellow, 2 Red with <code class="text-gray-300">330 &Omega;</code> resistors) on a 400-tie-point solderless breadboard.
    </p>
  </div>

  <div v-click class="p-4 rounded-xl bg-cyan-950/20 border border-cyan-500/30 space-y-1.5">
    <div class="text-xs font-mono text-cyan-300 uppercase tracking-wide">Proportional 6-Segment PWM Display</div>
    <p class="text-sm text-gray-200 m-0 leading-relaxed">
      Smooth <strong class="text-white">16-bit hardware PWM</strong> brightness interpolation across the active segment as you turn the knob&mdash;not crude on/off steps.
    </p>
  </div>

  <div v-click class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/30 space-y-1.5">
    <div class="text-xs font-mono text-emerald-300 uppercase tracking-wide">Dual Runtime Requirement</div>
    <p class="text-sm text-gray-200 m-0 leading-relaxed">
      Autonomous <strong class="text-white">50 Hz background daemon</strong> (<code class="text-emerald-300">bargraphd</code>) while keeping the single USB-C <code class="text-cyan-300">nsh&gt;</code> serial console interactive for live CLI commands (<code class="text-gray-300">start</code>, <code class="text-gray-300">stop</code>, <code class="text-gray-300">status</code>, <code class="text-gray-300">test</code>, <code class="text-gray-300">set</code>).
    </p>
  </div>
</div>

<!--
So that is our target: an AU$10 Seeed Studio XIAO RP2350 driving six through-hole LEDs from a ten-kilohm potentiometer on D0/A0. Instead of simple digital on/off steps, I wanted smooth 16-bit hardware PWM brightness interpolation on the active segment as you turn the knob, driven by a fifty-hertz background daemon called bargraphd—while still keeping the single USB-C serial console alive for interactive commands.
-->

---
layout: default
---

<!-- S07 -->
<div class="flex items-center justify-between mb-6">
  <h2 class="text-2xl font-bold m-0">Three Hardware &amp; RTOS Traps Waiting on the Bench</h2>
  <span class="px-3 py-1 rounded-lg bg-amber-950/30 border border-amber-500/40 text-xs font-mono text-amber-300 font-bold">3,241 Dirs &middot; 13 READMEs (0.4%)</span>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Real RP2350 silicon &amp; Apache NuttX driver hazards lurking inside those undocumented directories:
</p>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Trap 1 &mdash; USB-C Console Boot Hang</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>The XIAO RP2350 has only a single USB-C port (<code class="text-cyan-300">CDC/ACM</code> console).</li>
      <li>Blocking startup hangs <code class="text-cyan-300">nsh&gt;</code> enumeration; wiring dual startup hooks triggers a daemon boot race.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Trap 2 &mdash; ADC Pin Clobber &amp; Silicon Erratum <code class="text-amber-300">RP2350-E9</code> Pad Leakage</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>NuttX&rsquo;s RP2350 ADC initialiser unconditionally resets all 4 ADC pins (<code class="text-amber-300">GPIO 26&ndash;29</code>) and leaves digital input buffer <code class="text-red-300">IE=1</code> enabled.</li>
      <li>Clobbers LEDs wired to <code class="text-gray-300">D1</code>/<code class="text-gray-300">D2</code> and injects pad leakage into the potentiometer on <code class="text-amber-300">D0/A0</code>.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Trap 3 &mdash; The <code class="text-red-300">500k IRQ/s</code> Interrupt Storm</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Opening <code class="text-cyan-300">/dev/adc0</code> triggers continuous <code class="text-red-300">500 kHz</code> (<code class="text-gray-300">2 &micro;s</code>) free-running interrupts that starve the CPU cold.</li>
      <li>Dodging it by calling <code class="text-gray-300">open()</code>/<code class="text-gray-300">close()</code> at <code class="text-gray-300">40 Hz</code> hits an unguarded ISR teardown race.</li>
    </ul>
  </li>
</ol>

<!--
Now here is why this makes a brilliant litmus test for AI skills. Inside those 3,241 directories sit three nasty hardware and RTOS traps. Trap one: the XIAO has one USB-C port. Block on startup and you brick the shell; fire two init hooks and the daemon races itself. Trap two: NuttX's RP2350 ADC initialiser unconditionally resets all four ADC pins—clobbering LEDs on D1 or D2—and leaves the digital input buffer IE=1 enabled, tripping Silicon Erratum RP2350-E9 pad leakage on our potentiometer. Trap three: opening /dev/adc0 fires five hundred thousand interrupts per second, starving the CPU—and trying to dodge that by opening and closing the device forty times a second runs straight into an unguarded ISR teardown race.
-->

---
layout: default
---

<!-- S08 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">The Controlled Experiment: <code class="text-red-300">0</code> Skills vs. Skills Three Ways</h2>
</div>

<div class="grid grid-cols-2 gap-5 mt-2">
  <div class="p-5 rounded-xl bg-red-950/15 border border-red-500/30 space-y-2.5">
    <div class="text-xs font-mono text-red-400 uppercase tracking-wide">Baseline Foil &middot; 0 Skills</div>
    <h3 class="text-lg font-bold text-white m-0">Vanilla Vibe Coding</h3>
    <ul class="text-xs text-gray-300 space-y-1.5 list-disc pl-4 m-0 leading-relaxed">
      <li>Unconstrained Antigravity against raw upstream NuttX (<code class="text-red-300">13</code> <code class="text-gray-300">README.md</code> files).</li>
      <li><code class="text-red-300">0</code> workflow skills, <code class="text-red-300">0</code> directory contracts, <code class="text-red-300">0</code> review gates.</li>
      <li>Relies purely on blind <code class="text-gray-300">grep</code> and LLM training-weight guesswork.</li>
    </ul>
  </div>

  <div v-click class="p-5 rounded-xl bg-cyan-950/15 border border-cyan-500/30 space-y-2.5">
    <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Way 1 &middot; Adopt Bundled Plugins</div>
    <h3 class="text-lg font-bold text-white m-0"><code class="text-cyan-300">/plugin</code> &rarr; <code class="text-white">egm</code></h3>
    <ul class="text-xs text-gray-300 space-y-1.5 list-disc pl-4 m-0 leading-relaxed">
      <li>Install Dave Rensin&rsquo;s <code class="text-cyan-300">egm</code> plugin from the Antigravity marketplace.</li>
      <li>Pairs <code class="text-cyan-300">3,241</code> directory contracts (&ldquo;The Elephant&rdquo;) with stateless review swarms (&ldquo;The Goldfish&rdquo;).</li>
    </ul>
  </div>

  <div v-click class="p-5 rounded-xl bg-amber-950/15 border border-amber-500/30 space-y-2.5">
    <div class="text-xs font-mono text-amber-400 uppercase tracking-wide">Way 2 &middot; Orchestrate Community Skills</div>
    <h3 class="text-lg font-bold text-white m-0"><code class="text-amber-300">mattpocock/skills</code> + <code class="text-white">/wayfinder</code></h3>
    <ul class="text-xs text-gray-300 space-y-1.5 list-disc pl-4 m-0 leading-relaxed">
      <li>Install via <code class="text-amber-300">npx skills add</code> with SHA-256 <code class="text-gray-300">skills-lock.json</code> pinning.</li>
      <li>Compose <code class="text-gray-300">/grill-me</code>, <code class="text-gray-300">/wayfinder</code>, and <code class="text-gray-300">/prototype</code> to build this talk from <code class="text-gray-300">25 MB</code> of raw logs.</li>
    </ul>
  </div>

  <div v-click class="p-5 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-2.5">
    <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">Way 3 &middot; Author &amp; Evolve Custom Skills</div>
    <h3 class="text-lg font-bold text-white m-0"><code class="text-emerald-300">/agy-customizations</code> &rarr; <code class="text-white">domesticmouse/skills</code></h3>
    <ul class="text-xs text-gray-300 space-y-1.5 list-disc pl-4 m-0 leading-relaxed">
      <li>Package presentation tooling (<code class="text-gray-300">/asciinema</code>, <code class="text-gray-300">/slidev-deck</code>, <code class="text-gray-300">/bjm-voice</code>).</li>
      <li>Evolve Matt&rsquo;s <code class="text-gray-300">/teach</code> into <code class="text-emerald-300">/teach-code</code> to eliminate comprehension debt.</li>
    </ul>
  </div>
</div>

<!--
So I ran a controlled experiment with Google Antigravity on that exact bench target. First, our zero-skill baseline foil: Vanilla Vibe coding straight into raw NuttX. Then, Skills Three Ways: Way 1, adopting a bundled multi-agent plugin from the marketplace—Dave Rensin's egm—to ship firmware that actually respects the silicon. Way 2, orchestrating curated community skills from mattpocock/skills—using /wayfinder, /grill-me, and /prototype to tame 25 megabytes of raw recordings into a conference talk. And Way 3, using meta-skills like /agy-customizations and /writing-for-agents to author and evolve our own modular skills in domesticmouse/skills, culminating in /teach-code so a human engineer can actually own the resulting C firmware.
-->
