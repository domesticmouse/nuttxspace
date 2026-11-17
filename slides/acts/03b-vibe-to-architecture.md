---
layout: default
---

<!-- S074 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Rensin Step 2: The &ldquo;No Code Yet&rdquo; Rule</h2>
</div>

<div class="mt-8 p-8 rounded-2xl bg-cyan-950/20 border-2 border-cyan-500/40 space-y-5 text-center">
  <div class="text-xs font-mono text-cyan-300 uppercase tracking-widest">Phase 2 &middot; /egm-brainstorm</div>
  <div class="text-4xl font-extrabold text-white">
    0 lines of C permitted during brainstorm
  </div>
  <p class="text-base text-gray-300 max-w-2xl mx-auto m-0">
    Strictly prohibit generating implementation files while exploring hardware constraints, RTOS invariants, and architectural trade-offs.
  </p>
</div>

<!--
Phase 2 enforces Dave Rensin's "No Code Yet" rule—banning implementation so the Elephant has to ask hard questions before solving anything.
-->

---
layout: default
---

<!-- S075 -->
<div class="flex items-center justify-between mb-2">
  <h2 class="text-xl font-bold m-0">Freeze-Frame: <code class="text-cyan-300">02-brainstorm</code></h2>
  <span class="text-xs font-mono text-amber-300">Stop Point 16: Q7: Readiness Checkpoint (02:53 warped &middot; 19 pills)</span>
</div>

<div class="h-[310px] w-full">
  <AsciinemaPlayer
    src="/casts/02-brainstorm.cast"
    :autoPlay="false"
    :speed="2.0"
    :idleTimeLimit="2.0"
    fit="both"
    :showChapterBar="true"
    :initialStopPoint="16"
  />
</div>

<div class="mt-3 grid grid-cols-2 gap-3 text-xs">

<!-- S076 -->
  <div v-click class="p-3 rounded-lg bg-gray-900 border border-amber-500/35">
    <span class="font-mono text-amber-300 font-bold">Interview Depth:</span>
    <span class="text-gray-200 ml-1">Vanilla: <strong>1</strong> pin question vs. EGM: <strong>19 Socratic checkpoints</strong> (<code class="text-amber-200">41m 32s</code> raw)</span>
  </div>

<!-- S077 -->
  <div v-click class="p-3 rounded-lg bg-gray-900 border border-cyan-500/35">
    <span class="font-mono text-cyan-300 font-bold">Contract Reads:</span>
    <span class="text-gray-200 ml-1"><strong>49</strong> <code class="text-cyan-300">README.md</code> contracts (<code class="text-cyan-200">216 read_file calls</code>) before locking choices</span>
  </div>

</div>

<!--
Running /egm-brainstorm drives forty-one minutes of structured Q&A across nineteen checkpoints—notice that packed row of chapter pills along the top. Compare Vanilla's single "pick any free pins" chat against nineteen checkpoints grilling pin budgets, autostart timing, and PWM slice sharing. Before a single requirement settles, the Elephant reads forty-nine directory contracts and two hundred and sixteen files across nuttx/ and apps/.
-->

---
layout: default
---

<!-- S078 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Rensin Step 3: The Sycophant Challenge</h2>
</div>

<p class="text-sm text-gray-300 mb-6">
  7-lens Goldfish shootdown swarm attacking our own brainstorm assumptions before any spec is written:
</p>

<div class="grid grid-cols-4 gap-3 text-center">
  <div class="p-4 rounded-xl bg-red-950/20 border border-red-500/35">
    <div class="text-sm font-bold text-red-300">Contrarian</div>
    <div class="text-xs text-gray-400 mt-1">Challenges core premise</div>
  </div>
  <div class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/35">
    <div class="text-sm font-bold text-amber-300">Economics</div>
    <div class="text-xs text-gray-400 mt-1">Latency &amp; CPU budget</div>
  </div>
  <div class="p-4 rounded-xl bg-cyan-950/20 border border-cyan-500/35">
    <div class="text-sm font-bold text-cyan-300">Ergonomics</div>
    <div class="text-xs text-gray-400 mt-1">CLI &amp; bench usability</div>
  </div>
  <div class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/35">
    <div class="text-sm font-bold text-emerald-300">Migration</div>
    <div class="text-xs text-gray-400 mt-1">Submodule boundaries</div>
  </div>
  <div class="p-4 rounded-xl bg-purple-950/20 border border-purple-500/35">
    <div class="text-sm font-bold text-purple-300">SRE</div>
    <div class="text-xs text-gray-400 mt-1">Panics &amp; fault states</div>
  </div>
  <div class="p-4 rounded-xl bg-gray-900 border border-gray-700">
    <div class="text-sm font-bold text-gray-200">Tech Debt</div>
    <div class="text-xs text-gray-400 mt-1">Maintenance coupling</div>
  </div>
  <div class="col-span-2 p-4 rounded-xl bg-cyan-950/25 border border-cyan-400/40">
    <div class="text-sm font-bold text-cyan-200">Inventive Catalyst</div>
    <div class="text-xs text-gray-300 mt-1">Eliminates hardware/RTOS trade-offs without compromise</div>
  </div>
</div>

<!--
Then comes Rensin's Sycophant Challenge: seven hostile lenses attacking our own hypothesis so the agent stops cheering me on politely.
-->

---
layout: default
---

<!-- S079 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">Concepts Brief: 7 Upstream Traps Upfront</h2>
  <span class="text-xs font-mono text-cyan-300">docs/egm/designs/concepts_brief.md#L16-L35</span>
</div>

<div class="grid grid-cols-2 gap-3 mt-3 text-xs">

<!-- S080 -->
  <div v-click class="p-3.5 rounded-xl bg-red-950/20 border border-red-500/35 space-y-1">
    <div class="font-mono font-bold text-red-300">Obstacles #1 &amp; #2 &middot; Silicon &amp; ADC Pins (#L29-L31)</div>
    <p class="text-gray-200 m-0"><code class="text-red-200">my_reset()</code> pin clobber (<code class="text-gray-300">GPIO 26&ndash;29</code>) + <code class="text-red-200">RP2350-E9</code> pad isolation (<code class="text-gray-300">IE | PUE | PDE | ISO</code> + <code class="text-gray-300">OD=1</code>).</p>
  </div>

<!-- S081 -->
  <div v-click class="p-3.5 rounded-xl bg-amber-950/20 border border-amber-500/35 space-y-1">
    <div class="font-mono font-bold text-amber-300">Obstacle #3 &middot; 5-Slice PWM Layout (#L21-L27)</div>
    <p class="text-gray-200 m-0"><code class="text-amber-200">D1</code>/<code class="text-amber-200">D2</code> re-muxed via <code class="text-gray-300">/dev/pwm5,6</code> after <code class="text-gray-300">my_reset()</code>; <code class="text-gray-300">uint16_t 0..65535</code> dodges <code class="text-gray-300">b16ONE</code> wrap &amp; <code class="text-gray-300">PWMIOC_STOP</code> freeze.</p>
  </div>

<!-- S082 -->
  <div v-click class="p-3.5 rounded-xl bg-red-950/25 border border-red-500/45 space-y-1">
    <div class="font-mono font-bold text-red-300">Obstacle #4 &middot; 500 kHz ADC Storm (#L32-L34)</div>
    <p class="text-gray-200 m-0">Flags <code class="text-red-200">500,000 IRQ/s</code> <code class="text-gray-300">START_ONCE</code> storm &amp; proposes <code class="text-amber-300">RP23XX_ADC_DIV = 0xFFFF00</code> (our honest blunder next!).</p>
  </div>

<!-- S083 -->
  <div v-click class="p-3.5 rounded-xl bg-emerald-950/20 border border-emerald-500/35 space-y-1">
    <div class="font-mono font-bold text-emerald-300">Obstacles #5&ndash;#7 &middot; Boot &amp; Isolation (#L18-L20, L38-L44)</div>
    <p class="text-gray-200 m-0"><code class="text-emerald-200">CONFIG_BOARD_LATE_INITIALIZE=y</code>, <code class="text-gray-300">nsh_waitusbready()</code> USB trap, <code class="text-gray-300">apps/external</code> symlink &amp; host test split.</p>
  </div>

</div>

<!--
Every bit of that reading lands in concepts_brief.md, listing seven real upstream hardware and driver traps before drafting even starts. Right there on lines 29 to 31, it catches Vanilla's exact my_reset() pin clobber and RP2350-E9 pad leakage beforehand. It also uncovers NuttX's hardcoded pwm_info_s indexing, b16ONE wraparound turning 100% LEDs dark, and PWMIOC_STOP freezing output pins high. And at trap seven, it flags our primary spine—the half-megahertz ADC interrupt storm—which sets up our big honest blunder next. Plus CONFIG_BOARD_LATE_INITIALIZE=y, why USB serial blocks init scripts until a laptop connects, and clean apps/external submodule isolation.
-->

---
layout: center
class: text-center
---

<!-- S084 -->
<div class="space-y-5 max-w-2xl mx-auto">
  <div class="text-xs font-mono text-emerald-400 uppercase tracking-widest">Phase 2 Payoff</div>
  <div class="text-5xl font-black text-white">
    0 lines of C written
  </div>
  <div class="text-2xl font-bold text-emerald-300">
    7 upstream traps caught in <code class="text-emerald-200">concepts_brief.md</code>
  </div>
</div>

<!--
Zero C written, seven real traps caught upfront. Now watch what happens in Phase 3 when the Elephant drafts the full spec.
-->

---
layout: default
---

<!-- S085 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Phase 3: <code class="text-cyan-300">/egm-draft</code> Section-by-Section Spec</h2>
</div>

<div class="mt-8 grid grid-cols-3 gap-4 items-center text-center">
  <div class="p-6 rounded-xl bg-gray-900/80 border border-gray-700 space-y-2">
    <div class="text-xs font-mono text-gray-400 uppercase">Input Brief</div>
    <div class="text-base font-bold font-mono text-cyan-300">concepts_brief.md</div>
    <div class="text-xs text-gray-400">7 hardware/driver traps</div>
  </div>

  <div class="p-6 rounded-xl bg-cyan-950/25 border border-cyan-500/40 space-y-2">
    <div class="text-xs font-mono text-cyan-300 uppercase">Skill Command</div>
    <div class="text-xl font-black font-mono text-white">/egm-draft</div>
    <div class="text-xs text-gray-300">Human approval per section</div>
  </div>

  <div class="p-6 rounded-xl bg-emerald-950/20 border border-emerald-500/40 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">Draft Spec Output</div>
    <div class="text-sm font-bold font-mono text-emerald-300">xiao-rp2350-bargraph.v0-draft.md</div>
    <div class="text-xs text-gray-300">693 lines of architecture &amp; plan</div>
  </div>
</div>

<!--
Phase 3 runs /egm-draft, expanding that concepts brief section by section into a 693-line architecture and implementation specification.
-->

---
layout: default
---

<!-- S086 -->
<div class="flex items-center justify-between mb-2">
  <h2 class="text-xl font-bold m-0">Freeze-Frame: <code class="text-cyan-300">03-draft</code></h2>
  <span class="text-xs font-mono text-amber-300">Stop Point 6: Q3: Section Approval &mdash; Technical Plan (01:30 warped &middot; 15x)</span>
</div>

<div class="h-[340px] w-full">
  <AsciinemaPlayer
    src="/casts/03-draft.cast"
    :autoPlay="false"
    :speed="2.0"
    :idleTimeLimit="2.0"
    fit="both"
    :showChapterBar="true"
    :initialStopPoint="6"
  />
</div>

<!-- S087 -->
<div v-click class="mt-3 p-3 rounded-lg bg-gray-900 border border-cyan-500/35 text-xs flex items-center justify-between">
  <span class="font-mono text-cyan-300 font-bold">Semantic Boundaries:</span>
  <span class="text-gray-200"><code class="text-emerald-300">FeatureRange</code> (Goals &amp; Architecture) split cleanly from <code class="text-amber-300">DetailedRange</code> (Target Files #1..#30 Implementation Plan)</span>
</div>

<!--
Calling /egm-draft pauses explicitly at every section boundary—forcing me, the human, to review and approve the Technical Plan before continuing. Splitting high-level architectural choices from file-by-file C instructions makes every single decision easy to read—except when both minds share a blind spot.
-->

---
layout: default
---

<!-- S088 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">Primary Spine (Part 2): The Honest False Start</h2>
  <span class="text-xs font-mono text-amber-300">How did v0-draft fix the 500 kHz ADC storm?</span>
</div>

<div class="space-y-3 mt-2">

<!-- S089 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/70 border border-gray-800 space-y-1.5">
    <div class="text-xs font-mono text-cyan-300">docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L33</div>

```markdown {4}
* **Stable 50 Hz Potentiometer Sensing**: Sample `D0/A0` (`GPIO26`, `ADC0`)
  every 20 ms using a 4-sample burst average after `ANIOC_RESET_FIFO`,
  ... RP2350-E9 digital pad isolation (`IE=0`, `PUE=0`, `PDE=0`, `ISO=0`, `OD=1`),
  and hardware ADC clock pacing (`RP23XX_ADC_DIV = 0xFFFF00`, `~732 Hz`).
```

  </div>

<!-- S090 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/70 border border-amber-500/35 space-y-1.5">
    <div class="text-xs font-mono text-amber-300">docs/egm/designs/xiao-rp2350-bargraph.v0-draft.md#L242-L245</div>

```markdown {2-4}
3. **Hardware Conversion Rate Pacing (`RP23XX_ADC_DIV`)**:
   In `rp23xx_bringup.c`, writing `putreg32(0x00ffff00, RP23XX_ADC_DIV)`
   sets the 16-bit integer divider to `65535`, pacing conversions to:
   $$f_{\text{sample}} = 48,000,000\text{ Hz} / 65536 \approx 732.42\text{ Hz}$$
   This reduces ADC interrupt overhead by 99.85%.
```

  </div>

</div>

<!--
Remember earlier I promised no unblemished triumph story? Here is where our Elephant—and I—walked straight into a classic trap. The Elephant spotted a hardware divider register in the RP2350 headers—RP23XX_ADC_DIV—and proposed pacing the ADC down to seven hundred hertz. Look how convincing that looked: exact hex mask 0x00ffff00, neat LaTeX frequency arithmetic, 99.85% interrupt reduction calculated right down to two decimal places.
-->

---
layout: default
---

<!-- S091 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Mutual Agreement Illusion</h2>
</div>

<div class="relative mt-8 p-8 rounded-2xl bg-gray-900/80 border border-emerald-500/40 space-y-4">
  <div class="flex items-center justify-between">
    <span class="text-xs font-mono text-emerald-400 uppercase tracking-wider">Phases 2&ndash;3 Cumulative Reads</span>
    <span class="px-3 py-1 rounded-full bg-emerald-500/20 border border-emerald-400/40 font-mono text-xs text-emerald-300">
      Human Clicked: Approve Section
    </span>
  </div>
  <div class="text-3xl font-extrabold text-white">
    <code class="text-cyan-300">53</code> <code class="text-gray-200">README.md</code> contracts read &mdash; and <em>both</em> Brett and the Elephant approved <code class="text-amber-300">RP23XX_ADC_DIV = 0xFFFF00</code>!
  </div>

<!-- S092 -->
  <div v-click class="mt-4 flex justify-center">
    <div class="px-8 py-3 rounded-xl border-4 border-red-500 bg-red-950/80 text-red-400 text-5xl font-black font-mono tracking-widest -rotate-3 shadow-2xl">
      DEAD WRONG
    </div>
  </div>
</div>

<!--
Fifty-three README.md reads deep, the prose sounded airtight, the frequency maths checked out, and I nodded along and clicked Approve. And we were both dead wrong. Our tidy divider register would have done literally nothing on real silicon.
-->

---
layout: default
---

<!-- S093 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Rensin&rsquo;s Central Warning</h2>
</div>

<div class="mt-4 p-8 rounded-2xl bg-gray-900/70 border border-amber-500/40 text-center">
  <blockquote class="text-2xl font-semibold text-white tracking-tight leading-snug max-w-3xl mx-auto m-0">
    &ldquo;A costly failure is a plan the agent and operator agreed on too quickly, because both were looking at the same context.&rdquo;
  </blockquote>
  <p class="mt-4 text-sm text-gray-400 m-0">&mdash; Dave Rensin (<code class="text-cyan-300">@drensin</code>)</p>
</div>

<!-- S094 -->
<div v-click class="mt-4 p-4 rounded-xl bg-red-950/20 border border-red-500/35 text-xs font-mono text-center text-gray-200">
  Long context &rarr; plausible assumption &rarr; confirmation bias &rarr; unchallenged C draft
</div>

<!-- S095 -->
<div v-click class="mt-4 p-4 rounded-xl bg-cyan-950/25 border border-cyan-500/40 text-center">
  <span class="text-sm font-bold text-white">Next &middot; Act III.c: </span>
  <code class="text-cyan-300 text-sm">v0-draft.md &rarr; /egm-review (Stateless Goldfish Swarm)</code>
</div>

<!--
Dave Rensin describes this exact failure: a costly mistake is a plan both agent and operator agreed on inside the same context. Once our Elephant locked onto RP23XX_ADC_DIV, confirmation bias kicked in; it never stopped to check whether START_ONCE even reads that divider. Which is precisely why nobody writes C yet. Watch what happens next when zero-context Goldfish review v0-draft cold.
-->
