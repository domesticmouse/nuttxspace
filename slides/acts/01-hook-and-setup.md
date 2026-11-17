---
layout: center
class: text-center
---

<!-- S01 -->
<div class="h-full flex flex-col items-center justify-center">
  <h1 class="text-5xl font-extrabold tracking-tight text-white mb-4 leading-tight">
    Roll Your Own Docs: Custom AI Skills for Rapid Tech Ramp-Up
  </h1>
  <p class="text-2xl text-gray-300 font-light max-w-3xl">
    Taming Apache NuttX &amp; RP2350 Silicon with the Elephant-Goldfish Model and <code class="text-cyan-300">/teach-code</code>
  </p>

  <div class="mt-10 text-gray-400 text-base">
    <span class="font-semibold text-gray-200">Brett Morgan</span> &middot; Developer Relations Engineer, Google Cloud
  </div>

  <div class="mt-10 text-xs text-gray-500">
    Press <kbd>Space</kbd> or <kbd>&rarr;</kbd> to advance
  </div>
</div>

<!--
G'day everyone, I'm Brett Morgan from Google Cloud. Tonight's talk is about what happens when you drop a coding agent into a massive, unfamiliar C codebase—and how rolling two kinds of living documentation lets both the AI and the human sitting at the keyboard actually understand what ships to silicon.
-->

---
layout: default
---

<!-- S01b -->
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
Before we touch any hardware, let's call out the two main players in tonight's experiment. On the left is Apache NuttX, a mature, POSIX-compliant real-time operating system you can explore at nuttx.apache.org. On the right is Google Antigravity, the agentic development platform I used to tame it, available at antigravity.google.
-->

---
layout: default
---

<!-- S01c -->
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

  <div class="p-5 rounded-xl bg-gray-900/80 border border-gray-800 space-y-2">
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
      Configured via <code class="text-amber-300">1,569</code> <code class="text-gray-300">Kconfig</code> files spanning <code class="text-amber-300">3,241</code> directories&mdash;scaling from 8-bit AVR/Z80 microcontrollers up to multi-core 64-bit ARM, RISC-V, and x86_64 SoCs.
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
To appreciate what we're dropping our coding agent into, let's look at the scale of Apache NuttX. Founded by Gregory Nutt back in February 2007, the project is nearly twenty years old, with more than seventy-five thousand commits from over twelve hundred contributors across the kernel and apps repositories. Under the hood, it spans more than twelve thousand C source files and nearly nine thousand header files—around seven-point-six million lines of C and header code in total. It supports nineteen CPU architectures and four hundred and seventy-three target boards, wired together by over fifteen hundred Kconfig files across three thousand two hundred and forty-one directories. And unlike a flat bare-metal SDK, NuttX is a strict POSIX operating system where peripherals live behind upper-half VFS character drivers and lower-half silicon HALs—yet the entire tree ships with just thirteen README.md files.
-->

---
layout: default
---

<!-- S02a -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">How a Solderless Breadboard Works</h2>
</div>

<div class="w-full flex items-center justify-center">
  <img src="/images/breadboard-scopes.svg" alt="Solderless Breadboard Internal Connections" class="w-full max-h-[430px] object-contain rounded-xl border border-gray-800 shadow-lg" />
</div>

<!--
Before we wire up our hardware, let's take a quick X-ray look at a solderless breadboard—because its hidden metal spring clips behave a lot like variable scopes in code. Along the top and bottom, the horizontal power rails act like global scope: every hole in the blue minus rail or red plus rail is wired together. In the middle, each 5-hole vertical column—rows a through e above the trench, and f through j below it—is an independent local node. And that centre plastic trench is a hard namespace barrier: you seat a dual-row module like the XIAO RP2350 straddling e and f so its left and right pins never short together.
-->

---
layout: center
---

<!-- S02b -->
<div class="w-full h-full flex items-center justify-center">
  <img src="/images/assets-breadboard-wiring.svg" alt="Physical Breadboard Wiring: XIAO RP2350 6-LED Bar Graph" class="w-full max-h-[500px] object-contain rounded-xl border border-gray-800 shadow-lg" />
</div>

<!--
Here is the full physical wiring layout on a 28-column breadboard. The XIAO RP2350 straddles the centre trench, leaving open tie-points on both sides of every pin. On the left, a ten-kilohm potentiometer feeds D0/A0 with an optional hundred-nanofarad decoupling capacitor, strictly at 3.3 volts. Across the right, six discrete LEDs—two green, two yellow, two red—each get their own 330-ohm current-limiting resistor on D3 through D5 and D8 through D10, intentionally skipping D1/D2 and D6/D7.
-->

---
layout: default
---

<!-- S02c -->
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
      Smooth <strong class="text-white">16-bit hardware PWM</strong> brightness interpolation across the active segment as you turn the knob—not crude on/off steps.
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
So to recap the target we're building: an AU$10 Seeed Studio XIAO RP2350 sitting on a 400-tie-point solderless breadboard beside my laptop, wired to a ten-kilohm potentiometer on D0/A0 and six through-hole LEDs—two green, two yellow, two red. Instead of crude on/off steps, I wanted smooth 16-bit hardware PWM interpolation across all six channels so the active segment ramps proportionally in brightness as you twist the pot. And it needed to run as an autonomous fifty-hertz background daemon called bargraphd, while keeping the single USB-C serial console free for interactive shell commands like status, test, and set.
-->

---
layout: default
---

<!-- S03 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The RTOS Scale Shock: Why Apache NuttX Breaks &ldquo;Vibe Coding&rdquo;</h2>
</div>

<div class="grid grid-cols-2 gap-6 mt-4">
  <div class="p-5 rounded-xl bg-gray-900/80 border border-gray-800 space-y-2">
    <div class="text-xs font-mono text-gray-400 uppercase">Flat Bare-Metal Sketch</div>
    <div class="text-lg font-bold text-white">Arduino <code class="text-gray-300">loop()</code></div>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      Single file, direct pin calls (<code class="text-gray-300">analogRead</code> / <code class="text-gray-300">analogWrite</code>), abundant in LLM training weights.
    </p>
  </div>

  <div class="p-5 rounded-xl bg-cyan-950/15 border border-cyan-500/30 space-y-2">
    <div class="text-xs font-mono text-cyan-400 uppercase">Industrial POSIX RTOS</div>
    <div class="text-lg font-bold text-white">Apache NuttX (<code class="text-cyan-300">nuttx/</code> + <code class="text-cyan-300">apps/</code>)</div>
    <p class="text-xs text-gray-300 m-0 leading-relaxed">
      Nearly 20 years of strict POSIX character drivers (<code class="text-cyan-300">/dev/adc0</code>, <code class="text-cyan-300">/dev/pwmX</code>), <code class="text-gray-300">Kconfig</code> trees, and out-of-tree board overlays.
    </p>
  </div>
</div>

<div v-click class="mt-5 p-5 rounded-xl bg-amber-950/15 border border-amber-500/30 flex items-center justify-between">
  <div>
    <div class="text-xs font-mono text-amber-400 uppercase">The Documentation Gap</div>
    <div class="text-base font-bold text-white mt-1">
      <code class="text-amber-300">3,241</code> directories across <code class="text-gray-300">nuttx/</code> (<code class="text-gray-300">2,607</code>) and <code class="text-gray-300">apps/</code> (<code class="text-gray-300">634</code>) &mdash; only <code class="text-red-300">13</code> upstream <code class="text-gray-300">README.md</code> files
    </div>
  </div>
  <div class="text-3xl font-black font-mono text-amber-300">0.4%</div>
</div>

<div v-click class="mt-4 p-4 rounded-xl bg-red-950/15 border border-red-500/30">
  <div class="text-sm text-gray-200 leading-relaxed">
    <strong class="text-red-300">The Consequence:</strong> Without a navigable architectural map, coding agents fall straight back to blind <code class="text-gray-300">grep</code> and training-weight guesswork.
  </div>
</div>

<!--
In Arduino, that's a thirty-line loop() sketch. The catch? I wanted to build it as a proper out-of-tree application inside Apache NuttX—nearly twenty years of industrial POSIX real-time operating system C that I have definitely not memorised. When I checked the upstream submodules—nuttx/ and apps/—they spanned 3,241 directories, shipping with just 13 README.md files. That is zero-point-four percent directory coverage. Drop an unguided coding agent into three thousand undocumented directories, and it falls straight back to blind grep and training-weight guesswork.
-->

---
layout: default
---

<!-- S04 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Three Hardware &amp; RTOS Booby Traps Waiting on the Bench</h2>
</div>

<p class="text-sm text-gray-400 m-0 mb-5">
  Three real-world RP2350 silicon &amp; NuttX driver hazards &mdash; zero C syntax required to understand why naive code fails:
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
      <li>Clobbers any LEDs wired to <code class="text-gray-300">D1</code>/<code class="text-gray-300">D2</code> and injects pad leakage into our potentiometer on <code class="text-amber-300">D0/A0</code>.</li>
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
Worse still, this particular combination of RP2350 silicon and NuttX drivers hides three brutal booby traps waiting on the bench. First: the XIAO has only one USB-C port. If your startup script blocks waiting on the daemon, USB enumeration hangs and bricks your console; if you wire two startup hooks, the daemon races itself on boot. Second: NuttX's RP2350 ADC initialiser unconditionally resets all four ADC-capable pins (GPIO 26 to 29) and leaves the digital input buffer IE=1 enabled—clobbering any LEDs wired to D1 or D2 and tripping Silicon Erratum RP2350-E9 pad leakage on our potentiometer. Third: opening /dev/adc0 fires continuous five-hundred-kilohertz interrupts that starve the CPU cold—and if you try to dodge that by opening and closing the device forty times a second, you hit an unguarded ISR teardown race.
-->

---
layout: default
---

<!-- S05 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Controlled Experiment: Same Hardware, Same Prompt, Two Workflows</h2>
</div>

<p class="text-sm text-gray-300 m-0 mb-6">
  Same bench hardware, same functional bar-graph goal, and the exact same coding agent (<strong class="text-white">Google Antigravity</strong>):
</p>

<div class="grid grid-cols-2 gap-6">
  <div v-click class="p-6 rounded-xl bg-red-950/15 border border-red-500/30 space-y-3">
    <div class="text-xs font-mono text-red-400 uppercase">Experiment A &middot; The Foil</div>
    <h3 class="text-xl font-bold text-white m-0">Vanilla Vibe Coding</h3>
    <ul class="text-sm text-gray-300 space-y-2 list-disc pl-5 m-0">
      <li>Unconstrained Antigravity against raw upstream NuttX</li>
      <li><code class="text-red-300">13</code> upstream <code class="text-gray-300">README.md</code> files (<code class="text-gray-300">0.4%</code> coverage)</li>
      <li><code class="text-red-300">0</code> custom workflow skills or review gates</li>
    </ul>
  </div>

  <div v-click class="p-6 rounded-xl bg-emerald-950/15 border border-emerald-500/30 space-y-3">
    <div class="text-xs font-mono text-emerald-400 uppercase">Experiment B &middot; Roll Your Own Docs</div>
    <h3 class="text-xl font-bold text-white m-0">Docs for the AI + Docs for the Human</h3>
    <ul class="text-sm text-gray-300 space-y-2 list-disc pl-5 m-0">
      <li><strong class="text-white">Docs for the AI (<code class="text-emerald-300">egm</code>):</strong> <code class="text-emerald-300">3,241</code> directory contracts + stateless Goldfish review swarm</li>
      <li><strong class="text-white">Docs for the Human (<code class="text-cyan-300">/teach-code</code>):</strong> 6-lesson interactive HTML course &amp; strict C99 verification</li>
    </ul>
  </div>
</div>

<!--
So I ran a controlled experiment with Google Antigravity on the exact same hardware and functional goal. In Experiment A, we run unconstrained Vanilla Vibe coding against raw upstream NuttX—thirteen READMEs, zero custom skills. In Experiment B, we roll our own docs: first Docs for the AI using the Elephant-Goldfish Model, and then Docs for the Human using /teach-code.
-->
