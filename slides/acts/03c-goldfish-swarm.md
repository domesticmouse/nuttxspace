---
layout: default
---

<!-- S096 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Meet the Goldfish: Zero-Context Reviewers</h2>
</div>

<div class="mt-6 p-8 rounded-2xl bg-amber-950/15 border border-amber-500/40 space-y-5">
  <div class="flex items-center justify-between">
    <span class="text-xs font-mono text-amber-400 uppercase tracking-wider">Dave Rensin&rsquo;s Second Persona</span>
    <span class="text-xs font-mono text-gray-400">egm/docs/index.md#L45-L60</span>
  </div>
  <div class="text-3xl font-extrabold text-white">
    Fresh, stateless subagents with a &ldquo;three-second memory&rdquo;
  </div>
  <div class="grid grid-cols-3 gap-4 pt-2 text-sm">
    <div class="p-3 rounded-lg bg-gray-900/80 border border-gray-800 text-gray-200">
      <strong class="text-amber-300 block">Zero Brainstorm History</strong>
      Never saw your false starts
    </div>
    <div class="p-3 rounded-lg bg-gray-900/80 border border-gray-800 text-gray-200">
      <strong class="text-amber-300 block">Zero Loyalty to <code class="text-amber-200">v0-draft</code></strong>
      Reads every claim cold
    </div>
    <div class="p-3 rounded-lg bg-gray-900/80 border border-gray-800 text-gray-200">
      <strong class="text-amber-300 block">Reads Primary Source</strong>
      Verifies specs against C &amp; contracts
    </div>
  </div>
</div>

<!--
A Goldfish is a fresh subagent with zero memory of our brainstorm, zero chat history, and zero emotional loyalty to v0-draft.
-->

---
layout: default
---

<!-- S097 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The 5-Stage <code class="text-cyan-300">/egm-review</code> Pipeline (Steps 5&ndash;9)</h2>
</div>

<div class="grid grid-cols-5 gap-3 mt-6 text-center text-xs">
  <div class="p-4 rounded-xl bg-gray-900/80 border border-cyan-500/35 space-y-1.5">
    <div class="font-mono text-cyan-400 font-bold">Stage 5</div>
    <div class="text-sm font-bold text-white">Zoom Out</div>
    <div class="text-gray-400">High-altitude YAGNI &amp; reuse</div>
  </div>

  <div class="p-4 rounded-xl bg-gray-900/80 border border-emerald-500/35 space-y-1.5">
    <div class="font-mono text-emerald-400 font-bold">Stage 6</div>
    <div class="text-sm font-bold text-white">Comprehension</div>
    <div class="text-gray-400">Naive reader gaps</div>
  </div>

  <div class="p-4 rounded-xl bg-gray-900/80 border border-red-500/35 space-y-1.5">
    <div class="font-mono text-red-400 font-bold">Stage 7</div>
    <div class="text-sm font-bold text-white">Design Critic</div>
    <div class="text-gray-400">Hostile failure modes</div>
  </div>

  <div class="p-4 rounded-xl bg-gray-900/80 border border-purple-500/35 space-y-1.5">
    <div class="font-mono text-purple-400 font-bold">Stage 8</div>
    <div class="text-sm font-bold text-white">Intermediate Zoom</div>
    <div class="text-gray-400">Synthesize &amp; deduplicate</div>
  </div>

  <div class="p-4 rounded-xl bg-gray-900/80 border border-amber-500/40 space-y-1.5">
    <div class="font-mono text-amber-400 font-bold">Stage 9</div>
    <div class="text-sm font-bold text-white">Readiness + Details</div>
    <div class="text-gray-400">Line-by-line C audit</div>
  </div>
</div>

<!-- S098 -->
<div v-click class="mt-8 grid grid-cols-3 gap-4 text-center">
  <div class="p-5 rounded-xl bg-amber-950/20 border border-amber-500/40">
    <div class="text-3xl font-black font-mono text-amber-300">16</div>
    <div class="text-xs text-gray-300 mt-1">Parallel Goldfish subagents (<code class="text-amber-200">04-review</code>)</div>
  </div>
  <div class="p-5 rounded-xl bg-cyan-950/20 border border-cyan-500/40">
    <div class="text-3xl font-black font-mono text-cyan-300">146</div>
    <div class="text-xs text-gray-300 mt-1"><code class="text-cyan-200">README.md</code> contract reads</div>
  </div>
  <div class="p-5 rounded-xl bg-gray-900/80 border border-gray-700">
    <div class="text-3xl font-black font-mono text-white">653</div>
    <div class="text-xs text-gray-300 mt-1">Total <code class="text-gray-200">read_file</code> verifications</div>
  </div>
</div>

<!--
/egm-review runs five adversarial stages: high-altitude Zoom Out, naive Comprehension, hostile Design Critic, Synthesizer, and line-by-line Codebase Details. Across Pass 1 and Pass 2, sixteen fresh Goldfish fanned out in parallel, checking every claim against one hundred and forty-six contracts.
-->

---
layout: cast-full
title: "Pass 1 Goldfish Swarm (/egm-review)"
cast: /casts/04-review.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
initialStopPoint: 4
showChapterBar: true
---

<!-- S099 -->
<!--
I invoke /egm-review on xiao-rp2350-bargraph.md and choose consolidated mode. Look at the effect: instead of one chat thread nodding along with itself, parallel Goldfish subagents spin up cold across five stages, cross-checking every register address, Kconfig guard, and fixed-point equation in our spec directly against NuttX's driver files and README.md contracts. Compressing twenty-one and a half minutes at thirty-nine times speed, those stateless reviewers pile into pass1/goldfish_review_report.md—and our clean draft is about to look very bruised.
-->

---
layout: default
---

<!-- S100 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Pass 1 Harvest: Bruised in Markdown</h2>
  <span class="text-xs font-mono text-amber-300">docs/egm/reports/pass1/goldfish_review_report.md</span>
</div>

<div class="grid grid-cols-3 gap-4 mt-4 text-center">
  <div class="p-6 rounded-xl bg-red-950/25 border border-red-500/50">
    <div class="text-5xl font-black font-mono text-red-400">4</div>
    <div class="text-sm font-bold text-white mt-2">Blockers (<code class="text-red-300">B-1..B-4</code>)</div>
  </div>
  <div class="p-6 rounded-xl bg-amber-950/20 border border-amber-500/40">
    <div class="text-5xl font-black font-mono text-amber-300">5</div>
    <div class="text-sm font-bold text-white mt-2">Majors (<code class="text-amber-200">M-1..M-5</code>)</div>
  </div>
  <div class="p-6 rounded-xl bg-gray-900/80 border border-gray-700">
    <div class="text-5xl font-black font-mono text-gray-300">3</div>
    <div class="text-sm font-bold text-white mt-2">Nits</div>
  </div>
</div>

<!-- S101 -->
<div v-click class="mt-5 grid grid-cols-4 gap-3 text-xs font-mono">
  <div class="p-3 rounded-lg bg-red-950/30 border border-red-500/45 text-red-200"><strong class="text-red-300">B-1:</strong> ADC_DIV no-op</div>
  <div class="p-3 rounded-lg bg-red-950/30 border border-red-500/45 text-red-200"><strong class="text-red-300">B-2:</strong> 4x EMA gain</div>
  <div class="p-3 rounded-lg bg-red-950/30 border border-red-500/45 text-red-200"><strong class="text-red-300">B-3:</strong> Kconfig drop/abort</div>
  <div class="p-3 rounded-lg bg-red-950/30 border border-red-500/45 text-red-200"><strong class="text-red-300">B-4:</strong> 6 driver details</div>
</div>

<!-- S102 -->
<div v-click class="mt-5 p-4 rounded-xl bg-cyan-950/25 border border-cyan-500/40 text-center font-mono text-sm text-cyan-200">
  /egm-resolve &rarr; interactive &rarr; Item 1/12 (<strong class="text-white">B-1</strong>)
</div>

<!--
The verdict lands: four Blockers, five Majors, and three Nits—twelve real discrepancies surfaced in markdown before a single line of C existed. Every one of those four Blockers would have burned half a weekend with a multimeter and JTAG debugger if we had shipped first. Next, /egm-resolve steps through each finding with me in interactive mode—starting right at Item 1, Blocker B-1.
-->

---
layout: default
---

<!-- S103 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Primary Spine Payoff: Checking <code class="text-amber-300">RP23XX_ADC_DIV</code></h2>
</div>

<div class="mt-6 grid grid-cols-2 gap-6 items-center">
  <div class="p-6 rounded-xl bg-amber-950/20 border border-amber-500/40 space-y-2">
    <div class="text-xs font-mono text-amber-300 uppercase">Our Approved Draft Claim</div>
    <div class="text-lg font-bold font-mono text-white">v0-draft.md#L242</div>
    <p class="text-sm text-gray-300 m-0">Claims <code class="text-amber-200">RP23XX_ADC_DIV = 0xFFFF00</code> paces ADC conversions down to ~732 Hz.</p>
  </div>

  <div class="p-6 rounded-xl bg-cyan-950/20 border border-cyan-500/40 space-y-2">
    <div class="text-xs font-mono text-cyan-300 uppercase">Cold Goldfish Verification</div>
    <div class="text-lg font-bold font-mono text-white">rp23xx_adc.c#L169-L320</div>
    <p class="text-sm text-gray-300 m-0"><code class="text-cyan-200">egm-codebase-details</code> wakes up cold and reads NuttX&rsquo;s actual driver code line by line.</p>
  </div>
</div>

<!--
Because egm-codebase-details wakes up cold, it has zero reason to trust our 732 Hz divider arithmetic. It opens rp23xx_adc.c itself.
-->

---
layout: cast-full
title: "Pass 1 Interactive Resolve: Item 1/12 (B-1)"
cast: /casts/04-review.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
initialStopPoint: 8
showChapterBar: true
---

<!-- S104 -->
<!--
Selecting interactive mode forces /egm-resolve to walk me through each blocker one by one instead of auto-applying blind patches. Right on Item 1 of 12, B-1 cites lines 169 through 320 of rp23xx_adc.c and lays out the replacement: NVIC IRQ gating around each fifty-hertz burst. Watching that evidence appear, I type accept—no ego, just cold proof.
-->

---
layout: default
---

<!-- S105 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">Issue <code class="text-red-300">B-1</code> Finding Card</h2>
  <span class="text-xs font-mono text-gray-400">docs/egm/reports/pass1/goldfish_review_report.md#L8-L16</span>
</div>

<div class="p-6 rounded-2xl bg-gray-900/80 border border-red-500/45 space-y-4">
  <div class="flex items-center justify-between">
    <span class="px-2.5 py-1 rounded bg-red-500/20 border border-red-400/40 text-xs font-mono font-bold text-red-300">BLOCKER &middot; B-1</span>
    <span class="text-xs font-mono text-cyan-300">egm-zoom-out / egm-inventive-catalyst / egm-design / egm-codebase-details</span>
  </div>

  <div class="space-y-3 text-sm">
    <div>
      <strong class="text-white">Discrepancy:</strong>
      <span class="text-gray-200 ml-1"><code class="text-amber-300">rp23xx_adc.c</code> never sets <code class="text-gray-200">START_MANY</code>; it writes single-shot <code class="text-cyan-300">START_ONCE</code> (<code class="text-gray-400">#L304</code>), where <code class="text-red-300">RP23XX_ADC_DIV</code> has zero hardware effect.</span>
    </div>
    <div>
      <strong class="text-emerald-300">Fix:</strong>
      <span class="text-gray-200 ml-1">Remove <code class="text-gray-300">RP23XX_ADC_DIV</code>; keep <code class="text-gray-200">/dev/adc0</code> open and gate <code class="text-emerald-300">RP23XX_ADC_IRQ_FIFO</code> via the NVIC.</span>
    </div>
  </div>
</div>

<!-- S106 -->
<div v-click class="mt-6 p-4 rounded-xl bg-red-950/35 border border-red-500/50 text-center font-mono text-sm text-red-200">
  <code class="text-red-300">RP23XX_ADC_DIV = 0xFFFF00</code> &rarr; <strong>0 hardware effect</strong> &rarr; <strong>500,000 IRQ/s CPU starvation</strong>
</div>

<!--
Here is what B-1 actually proved: the RP2350 ADC_DIV register only paces free-running START_MANY conversions—it never runs under START_ONCE. Our clever 0xFFFF00 divider was a complete no-op; the Cortex-M33 would still have been drowned under half a million interrupts per second.
-->

---
layout: default
---

<!-- S107 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">The Goldfish Solution (<code class="text-emerald-300">B-1</code>): NVIC IRQ Gating</h2>
  <span class="text-xs font-mono text-emerald-300">Keep /dev/adc0 open + Mask/Unmask IRQ (250 IRQ/s total)</span>
</div>

<!-- S108 -->
<div v-click class="p-4 rounded-xl bg-gray-900/70 border border-emerald-500/35 space-y-2">
  <div class="text-xs font-mono text-emerald-300">docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L266-L293 (bargraph_sample_adc())</div>

```c {1,2,8}
  (void)ioctl(adc_fd, ANIOC_RESET_FIFO, 0);
  up_enable_irq(RP23XX_ADC_IRQ_FIFO);
  while (total_read < bytes_needed)
    {
      ssize_t nread = read(adc_fd, buf + total_read, bytes_needed - total_read);
      ... /* ~10 us burst for 5 samples (25 bytes) */
    }
  up_disable_irq(RP23XX_ADC_IRQ_FIFO);
```

</div>

<!-- S109 -->
<div v-click class="mt-4 p-3.5 rounded-xl bg-gray-900 border border-gray-700 text-xs font-mono text-center text-gray-200">
  <span class="text-emerald-300">Spec review catch: 10 seconds</span>
  <span class="mx-3 text-gray-500">|</span>
  <span class="text-red-300">Hardware JTAG / logic analyser catch: hours</span>
</div>

<!-- S110 -->
<div v-click class="mt-3 p-3 rounded-xl bg-amber-950/20 border border-amber-500/35 text-xs font-mono text-center text-amber-200">
  Also in Pass 1: <strong>B-2</strong> (Fixed-Point Maths) &amp; <strong>B-3</strong> (Kconfig Verification)
</div>

<!--
Instead of closing /dev/adc0 like Vanilla did, the Goldfish proposed keeping it persistently open and gating the interrupt line directly in the NVIC. Flush the FIFO, unmask RP23XX_ADC_IRQ_FIFO for ten microseconds to pull five samples, then mask it—dropping 500,000 interrupts down to 250. Ten seconds to type accept in markdown versus hours staring at a stalled Cortex-M33 under a logic analyser. And B-1 was only the headline catch. Let's look at B-2 and B-3 sitting right underneath it in the same report.
-->

---
layout: default
---

<!-- S111 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">Supporting Trap #3 (<code class="text-red-300">B-2</code>): 4x Q4 EMA Gain Bug</h2>
  <span class="text-xs font-mono text-gray-400">pass1/goldfish_review_report.md#L20-L28</span>
</div>

<div class="p-3 rounded-xl bg-gray-900/80 border border-red-500/35 text-xs space-y-1 mb-3">
  <div><strong class="text-cyan-300">Reviewers:</strong> <code class="text-gray-300">egm-codebase-details / egm-implementation-readiness</code> (<span class="text-red-300 font-bold">Blocker</span>)</div>
  <div><strong class="text-white">Discrepancy:</strong> <code class="text-red-300">ema_q4 &gt;&gt; 2</code> outputs <code class="text-red-200">4R [0, 16380]</code> instead of <code class="text-gray-200">[0, 4095]</code>; missing first-sample seed &rarr; <strong class="text-emerald-300">Fix:</strong> seed <code class="text-emerald-300">raw_avg &lt;&lt; 4</code>, round <code class="text-emerald-300">(ema_q4 + 8) &gt;&gt; 4</code>.</div>
</div>

<div class="grid grid-cols-2 gap-4">

<!-- S112 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/70 border border-red-500/35 space-y-1.5">
    <div class="text-xs font-mono text-red-300">Buggy v0-draft.md#L253-L254</div>

```c {4}
/* v0-draft.md#L254: Q4 state (alpha = 1/4) */
ema_q4 = ema_q4 - (ema_q4 >> 2) + (raw_avg << 2);
/* Steady state: ema_q4 = 16 * raw_avg (= raw_avg << 4) */
filtered = ema_q4 >> 2; /* BUG: outputs 4 * raw_avg [0..16380]! */
```

  </div>

<!-- S113 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/70 border border-emerald-500/35 space-y-1.5">
    <div class="text-xs font-mono text-emerald-300">Fixed bargraph_main.c#L661-L671</div>

```c {3,8}
  if (!(*ema_initialized))
    {
      *ema_q4          = (uint32_t)raw_avg << 4;
      *ema_initialized = true;
    }
  else
    *ema_q4 = *ema_q4 - (*ema_q4 >> 2) + ((uint32_t)raw_avg << 2);
  filtered = (uint16_t)((*ema_q4 + 8u) >> 4);
```

  </div>

</div>

<!--
Goldfish do not only read C files—in Issue B-2, one verified our fixed-point exponential moving average algebra by hand. In v0-draft, we wrote ema_q4 >> 2—leaving a four-times gain error that pegged all six LEDs at 100% by quarter-turn pot travel. B-2 fixed both: seeding raw_avg << 4 on the first sample so startup never ramps from zero, then rounding (*ema_q4 + 8u) >> 4.
-->

---
layout: default
---

<!-- S114 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">Build-System Guard (<code class="text-red-300">B-3</code>)</h2>
  <span class="text-xs font-mono text-gray-400">pass1/goldfish_review_report.md#L32-L42</span>
</div>

<div class="p-5 rounded-2xl bg-gray-900/80 border border-red-500/40 space-y-3 text-sm">
  <div class="flex items-center justify-between">
    <span class="px-2.5 py-1 rounded bg-red-500/20 text-xs font-mono font-bold text-red-300">BLOCKER &middot; B-3</span>
    <span class="text-xs font-mono text-cyan-300">egm-design / egm-codebase-details / egm-implementation-readiness</span>
  </div>
  <p class="text-gray-200 m-0">
    <strong class="text-white">Discrepancy:</strong> Missing <code class="text-amber-300">CONFIG_ANALOG=y</code> silently drops <code class="text-amber-300">CONFIG_ADC=y</code>; <code class="text-red-300">CONFIG_PWM_MULTICHAN=y</code> does not exist in <code class="text-gray-300">timers/Kconfig</code>.
  </p>
</div>

<!-- S115 -->
<div v-click class="mt-4 grid grid-cols-2 gap-4 text-xs font-mono">
  <div class="p-4 rounded-xl bg-emerald-950/25 border border-emerald-500/40 text-emerald-200">
    <strong>+ CONFIG_ANALOG=y</strong><br/>
    <span class="text-gray-300">Prevents Vanilla&rsquo;s Step 180 silent <code class="text-emerald-300">CONFIG_ADC</code> drop</span>
  </div>
  <div class="p-4 rounded-xl bg-red-950/25 border border-red-500/40 text-red-200">
    <strong>- CONFIG_PWM_MULTICHAN=y</strong><br/>
    <span class="text-gray-300">Prevents hallucinated symbol <code class="text-red-300">olddefconfig exit 1</code></span>
  </div>
</div>

<!-- S116 -->
<div v-click class="mt-5 p-4 rounded-xl bg-amber-950/20 border border-amber-500/35 text-center text-sm text-amber-200">
  <strong>Multi-Angle Defence:</strong> One Goldfish checks silicon errata, another checks fixed-point shifts, another checks Kconfig &mdash; <em>Next: Why ONE pass isn&rsquo;t enough.</em>
</div>

<!--
Meanwhile, Issue B-3 audited every single Kconfig symbol in our draft directly against nuttx/drivers/timers/Kconfig and nuttx/drivers/analog/Kconfig. It caught both Vanilla's missing CONFIG_ANALOG=y drop and a hallucinated CONFIG_PWM_MULTICHAN=y symbol before configure.sh ever ran. One pass caught four huge Blockers. Now watch why stopping after a single review pass is still a rookie mistake.
-->

---
layout: default
---

<!-- S117 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Second-Order Patch Trap</h2>
</div>

<div class="mt-6 p-7 rounded-2xl bg-amber-950/15 border border-amber-500/40 space-y-4 text-center">
  <div class="text-xs font-mono text-amber-300 uppercase tracking-wider">Recursive Risk</div>
  <div class="text-2xl font-bold font-mono text-white">
    Fixing <code class="text-emerald-300">B-1</code> &rarr; Added 5-sample (25 B) <code class="text-cyan-300">read()</code> burst &rarr; <span class="text-red-300">Unreviewed new prose!</span>
  </div>
  <p class="text-sm text-gray-300 m-0">
    Every patch written during review introduces fresh assumptions that no Goldfish has examined yet.
  </p>
</div>

<!-- S118 -->
<div v-click class="mt-8 p-6 rounded-2xl bg-cyan-950/25 border-2 border-cyan-400/50 text-center space-y-2">
  <div class="text-4xl font-black font-mono text-cyan-300">/clear</div>
  <div class="text-sm font-mono text-gray-200">
    Archive Pass 1 &rarr; Wipe Parent Session Memory Completely &rarr; Launch Pass 2 Cold
  </div>
</div>

<!--
Every time you patch a spec to resolve a blocker, you write fresh assumptions that no Goldfish has ever examined. So we archive Pass 1, issue /clear to wipe parent memory completely, and run /egm-review a second time cold.
-->

---
layout: cast-full
title: "Pass 2 Cold Review (/clear + /egm-review)"
cast: /casts/04-review.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
initialStopPoint: 13
showChapterBar: true
---

<!-- S119 -->
<!--
Watch the exact sequence: Pass 1 is archived, we fire /clear so the session that wrote the B-1 patch cannot grade its own homework, and we launch Pass 2 /egm-review in consolidated mode. Immediately at twenty-six times speed, a fresh wave of stateless Goldfish reads our brand-new NVIC IRQ gating section alongside NuttX's upper-half analogue driver adc.c—and catches a subtle second-order bug inside our own fix.
-->

---
layout: default
---

<!-- S120 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">Pass 2 Catch <code class="text-amber-300">CA-01</code>: The 5-Byte Short Read</h2>
  <span class="text-xs font-mono text-gray-400">pass2/goldfish_review_report.md#L9-L17 (0 Blockers!)</span>
</div>

<div class="p-3 rounded-xl bg-gray-900/80 border border-amber-500/40 text-xs space-y-1 mb-3">
  <div><strong class="text-cyan-300">Reviewers:</strong> <code class="text-gray-300">egm-design / egm-codebase-details / egm-implementation-readiness</code> (<span class="text-amber-300 font-bold">Major</span>)</div>
  <div><strong class="text-white">Discrepancy:</strong> <code class="text-amber-200">adc_read()</code> wakes on <code class="text-gray-200">af_sem</code> and returns <strong class="text-red-300">5 bytes</strong> as soon as 1 message arrives &rarr; <strong class="text-emerald-300">Fix:</strong> loop <code class="text-emerald-200">read()</code> to 25 bytes before masking IRQ.</div>
</div>

<div class="grid grid-cols-2 gap-3">

<!-- S121 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/70 border border-red-500/35 space-y-1">
    <div class="text-xs font-mono text-red-300">nuttx/drivers/analog/adc.c#L270-L305</div>

```c {1,3,5-7}
  while (fifo->af_head == fifo->af_tail)
    {
      ret = nxsem_wait(&fifo->af_sem); /* Wakes on 1st sample! */
    }
  /* The FIFO is not empty. Copy all buffered data that will fit
   * in the user buffer. (Returns 5 bytes immediately, not 25!)
   */
```

  </div>

<!-- S122 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/70 border border-emerald-500/35 space-y-1">
    <div class="text-xs font-mono text-emerald-300">bargraph_main.c#L270-L306</div>

```c {1-3,8}
  while (total_read < bytes_needed) /* 25 bytes = 5 x 5-byte msgs */
    {
      ssize_t nread = read(adc_fd, buf + total_read, bytes_needed - total_read);
      if (nread < 0 && errno == EINTR) continue;
      total_read += (size_t)nread;
    }
  up_disable_irq(RP23XX_ADC_IRQ_FIFO);
  for (i = 1; i < BARGRAPH_ADC_READ_SAMPLES; i++) /* discard msgs[0] */
```

  </div>
</div>

<!-- S123 -->
<div v-click class="mt-3 p-2.5 rounded-lg bg-emerald-950/25 border border-emerald-500/40 text-center text-xs font-mono text-emerald-200">
  <strong>Pass 1 (B-1):</strong> Killed <code class="text-gray-200">ADC_DIV</code> &nbsp;|&nbsp; <strong>/clear + Pass 2 (CA-01):</strong> Killed 5-byte stack-garbage read
</div>

<!--
Pass 2 returned 0 Blockers and caught CA-01—a sneaky short-read behaviour inside our brand-new B-1 burst specification. Asking POSIX read() for twenty-five bytes does not wait for twenty-five bytes; adc_read() wakes after one five-byte sample, leaving msgs[1..4] as uninitialised stack garbage. So the Pass 2 spec locks in the exact EINTR-safe twenty-five-byte accumulation loop, masks the IRQ after, and discards stale msgs[0]. Pass 1 killed the fake clock divider; /clear plus Pass 2 killed the stack-garbage read. Now—finally—we write some C.
-->
