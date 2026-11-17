---
layout: center
class: text-center
---

<!-- S177 -->
<div class="space-y-4 max-w-3xl mx-auto">
  <div class="text-xs font-mono text-amber-400 uppercase tracking-widest">Act V &middot; docs/reports/vanilla-vs-egm-comparison.md</div>
  <h1 class="text-4xl font-extrabold text-white leading-tight">
    The Honest Retro
  </h1>
  <p class="text-xl font-mono text-cyan-300">
    Telemetry, Blind Spots &amp; Skill Evolution
  </p>
</div>

<!--
Now let's put both runs side by side—telemetry, strict post-audit catches, and the physical blunder neither workflow spotted.
-->

---
layout: default
---

<!-- S178 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">Head-to-Head Telemetry</h2>
  <span class="text-xs font-mono text-gray-400">docs/reports/vanilla-vs-egm-comparison.md#L34-L39</span>
</div>

<div class="grid grid-cols-12 gap-3 px-4 py-1.5 text-xs font-mono text-gray-400 uppercase">
  <div class="col-span-4">Metric</div>
  <div class="col-span-4 text-red-300">Experiment A (Vanilla)</div>
  <div class="col-span-4 text-emerald-300">Experiment B (EGM)</div>
</div>

<div class="space-y-2.5 text-sm">
  <div class="grid grid-cols-12 gap-3 p-3.5 rounded-xl bg-gray-900/80 border border-gray-800 items-center">
    <div class="col-span-4 font-bold text-white">Raw Duration</div>
    <div class="col-span-4 font-mono text-gray-200">46m 40.0s</div>
    <div class="col-span-4 font-mono text-cyan-200">154m 36.6s <span class="text-xs text-gray-400">(Ph 1&ndash;5) / 196m 37s (Ph 1&ndash;6)</span></div>
  </div>

<!-- S179 -->
  <div v-click class="grid grid-cols-12 gap-3 p-3.5 rounded-xl bg-gray-900/80 border border-gray-800 items-center">
    <div class="col-span-4 font-bold text-white">Sessions &amp; Subagents</div>
    <div class="col-span-4 font-mono text-gray-200">1 parent + 0 subagents</div>
    <div class="col-span-4 font-mono text-emerald-200">8 parents + 17 Goldfish <span class="text-xs text-gray-400">(25 traces)</span></div>
  </div>

<!-- S180 -->
  <div v-click class="grid grid-cols-12 gap-3 p-3.5 rounded-xl bg-gray-900/80 border border-gray-800 items-center">
    <div class="col-span-4 font-bold text-white"><code class="text-cyan-300">README.md</code> Reads</div>
    <div class="col-span-4 font-mono text-red-300">0 <span class="text-xs text-gray-400">(66 read_file)</span></div>
    <div class="col-span-4 font-mono text-emerald-300">209 <span class="text-xs text-gray-400">(1,005 read_file)</span></div>
  </div>

<!-- S181 -->
  <div v-click class="grid grid-cols-12 gap-3 p-3.5 rounded-xl bg-gray-900/80 border border-gray-800 items-center">
    <div class="col-span-4 font-bold text-white">Build Errors Before <code class="text-gray-300">.uf2</code></div>
    <div class="col-span-4 font-mono text-red-300 text-xs">1 Kconfig drop + 1 xmllint error</div>
    <div class="col-span-4 font-mono text-emerald-300 text-xs">0 firmware errors <span class="text-gray-400">(1 BSD mktemp fix)</span></div>
  </div>
</div>

<!--
Let's be completely upfront about wall-clock cost: Vanilla took forty-six minutes forty seconds; EGM firmware took one hundred fifty-four minutes. That reflects the architecture: one monolithic vibe thread versus eight parent sessions orchestrating seventeen zero-context Goldfish subagents across twenty-five traces. Vanilla read zero nuttx/ or apps/ contracts across sixty-six peeks; EGM checked two hundred and nine directory contracts deliberately. And upfront verification paid off at compile time: zero firmware build errors and a clean 347K .uf2 on attempt one.
-->

---
layout: default
---

<!-- S182 -->
<div class="flex items-center justify-between mb-6">
  <h2 class="text-2xl font-bold m-0">Dual-Tier Trap Scorecard</h2>
  <span class="text-xs font-mono text-gray-400">docs/reports/vanilla-vs-egm-comparison.md#L22-L23</span>
</div>

<div class="p-6 rounded-2xl bg-gray-900/80 border border-cyan-500/35 grid grid-cols-12 gap-4 items-center mt-4">
  <div class="col-span-4">
    <div class="text-xs font-mono text-cyan-400 uppercase">Tier 1</div>
    <div class="text-xl font-bold text-white">Primary 10-Point Scorecard</div>
  </div>
  <div class="col-span-4 text-center p-4 rounded-xl bg-amber-950/25 border border-amber-500/40">
    <div class="text-xs font-mono text-amber-300 uppercase">Vanilla</div>
    <div class="text-4xl font-black font-mono text-amber-300 mt-1">5 / 10</div>
  </div>
  <div class="col-span-4 text-center p-4 rounded-xl bg-emerald-950/25 border border-emerald-500/45">
    <div class="text-xs font-mono text-emerald-300 uppercase">EGM</div>
    <div class="text-4xl font-black font-mono text-emerald-300 mt-1">10 / 10</div>
  </div>
</div>

<!-- S183 -->
<div v-click class="mt-5 p-6 rounded-2xl bg-gray-900/80 border border-purple-500/35 grid grid-cols-12 gap-4 items-center">
  <div class="col-span-4">
    <div class="text-xs font-mono text-purple-400 uppercase">Tier 2</div>
    <div class="text-xl font-bold text-white">Strict Post-Audit</div>
  </div>
  <div class="col-span-4 text-center p-4 rounded-xl bg-red-950/25 border border-red-500/45">
    <div class="text-xs font-mono text-red-300 uppercase">Vanilla</div>
    <div class="text-lg font-black font-mono text-red-200 mt-1">4 Pass / 4 Partial / 2 Fail</div>
  </div>
  <div class="col-span-4 text-center p-4 rounded-xl bg-amber-950/20 border border-amber-500/40">
    <div class="text-xs font-mono text-amber-300 uppercase">EGM</div>
    <div class="text-lg font-black font-mono text-amber-200 mt-1">6 Pass / 4 Partial / 0 Fail</div>
  </div>
</div>

<!--
Across the ten hardware and RTOS traps we tested, Vanilla scored five out of ten while EGM swept ten out of ten. Then we turned fresh read-only Goldfish loose after Phase 5—and under strict grading, EGM scored six Passes, four Partials, zero Fails.
-->

---
layout: default
---

<!-- S184 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">The Time Tradeoff &amp; What 108 Minutes Bought</h2>
</div>

<div class="p-6 rounded-2xl bg-gray-900/80 border border-cyan-500/35 flex items-center justify-between mt-3">
  <div>
    <div class="text-xs font-mono text-cyan-400 uppercase">Firmware Wall-Clock Ratio</div>
    <div class="text-xl font-bold text-white mt-1"><code class="text-cyan-300">154m 37s</code> vs. <code class="text-gray-300">46m 40s</code> (<code class="text-amber-300">+108 min</code> upfront)</div>
  </div>
  <div class="text-5xl font-black font-mono text-cyan-300">~3.3x</div>
</div>

<!-- S185 -->
<div v-click class="mt-4 grid grid-cols-3 gap-4 text-center text-xs font-mono">
  <div class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/40 text-emerald-200">
    <strong>0 High-Severity</strong> Silicon / ISR Fails
  </div>
  <div class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/40 text-emerald-200">
    <strong>1st-Attempt</strong> <code class="text-emerald-300">.uf2</code> Binary
  </div>
  <div class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/40 text-emerald-200">
    <strong>6-Lesson</strong> Interactive Codelab
  </div>
</div>

<!-- S186 -->
<div v-click class="mt-5 p-4 rounded-xl bg-amber-950/25 border border-amber-500/45 text-center text-sm text-amber-200">
  <strong>Now about EGM&rsquo;s 4 Strict Partials:</strong> <code class="text-amber-300">#6, #7, #8, #10</code> (<code class="text-xs text-gray-300">vanilla-vs-egm-comparison.md#L23, L68-L72</code>)
</div>

<!--
Would I spin up sixteen Goldfish for a one-line CSS tweak? Definitely not. Three-point-three times overhead belongs on unfamiliar legacy complexity. When thirty years of RTOS C meets real silicon errata, spending two calm hours upfront saves weekends chasing ISR panics. Now about those four strict partial marks on our own ten-out-of-ten firmware—let's inspect every single one of them candidly.
-->

---
layout: center
class: text-center
---

<!-- S187 -->
<div class="space-y-5 max-w-3xl mx-auto">
  <div class="text-xs font-mono text-amber-400 uppercase tracking-widest">docs/reports/vanilla-vs-egm-comparison.md#L3-L5</div>
  <h1 class="text-3xl font-extrabold text-white leading-snug">
    Post-Experiment Hostile Audit
  </h1>
  <div class="p-6 rounded-2xl bg-gray-900/80 border border-amber-500/40 text-base text-gray-200">
    Independent read-only Goldfish (<code class="text-cyan-300">egm-codebase-details</code> + <code class="text-amber-300">egm-design</code>) graded the archived Phase 5 C, <code class="text-gray-200">Makefile</code>, and <code class="text-gray-200">defconfig</code> cold.
  </div>
</div>

<!--
We did not cherry-pick a kind reviewer; we launched fresh read-only Goldfish auditors against the archived Phase 5 C files cold.
-->

---
layout: default
---

<!-- S188 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">Self-Correction #3: Invariant #6 Exit Ordering</h2>
  <span class="px-2.5 py-1 rounded bg-amber-950/40 border border-amber-500/45 text-xs font-mono text-amber-300">Strict: Partial</span>
</div>

<div class="text-xs font-mono text-gray-400 mb-3">
  docs/egm/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L388-L390, L527-L530
</div>

<div class="grid grid-cols-2 gap-4 mt-3">

<!-- S189 -->
  <div v-click class="p-4 rounded-xl bg-gray-900/70 border border-amber-500/40 space-y-2">
    <div class="text-xs font-mono text-amber-300">Unmasked 1 line BEFORE close(adc_fd)!</div>

```c {2,5}
/* Startup failure rollback (#L388-L390): */
up_enable_irq(RP23XX_ADC_IRQ_FIFO); /* Unmasks 1 line BEFORE close! */
(void)close(adc_fd);
/* Daemon shutdown on bargraph stop (#L527-L530): */
up_enable_irq(RP23XX_ADC_IRQ_FIFO); /* Starts 2 us conversion early! */
(void)close(adc_fd);
```

  </div>

<!-- S190 -->
  <div v-click class="p-4 rounded-xl bg-gray-900/70 border border-emerald-500/40 space-y-2">
    <div class="text-xs font-mono text-emerald-300">Swap lines (#L529-L530) &rarr; clean shutdown</div>

```diff {3-4}
-  up_enable_irq(RP23XX_ADC_IRQ_FIFO);
   (void)close(adc_fd);
+  up_enable_irq(RP23XX_ADC_IRQ_FIFO); /* Now safe: g_active_count == 0 */
+/* In rp23xx_adc.c#L176-L196: pending IRQ runs clean ADC shutdown */
```

  </div>

</div>

<!--
Look at Self-Correction #3 under Invariant 6: remember how EGM kept /dev/adc0 persistently open to avoid Vanilla's 40 Hz shutdown race? On exit and startup rollback, EGM unmasked up_enable_irq() one line before close(adc_fd)—briefly starting another conversion right before remove_device() runs! Swapping those two lines closes adc_fd while masked, then unmasks the pending IRQ so g_active_count == 0 shuts down cleanly.
-->

---
layout: default
---

<!-- S191 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">The Other Three Strict Partials</h2>
  <span class="text-xs font-mono text-gray-400">docs/reports/vanilla-vs-egm-comparison.md#L69-L72</span>
</div>

<div class="space-y-4 mt-4 text-sm">
  <div class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/40">
    <div class="font-mono font-bold text-amber-300">Strict Partial #2 &middot; #7 Kconfig Hygiene (<code class="text-gray-300">defconfig#L39</code>)</div>
    <p class="text-gray-200 mt-1 m-0">Inherited <code class="text-gray-200">EXAMPLES_HELLO</code>, <code class="text-gray-200">TESTING_GETPRIME</code>, and <code class="text-gray-200">TESTING_OSTEST</code> left enabled + <code class="text-amber-200">CONFIG_RAMLOG_SYSLOG=y</code> omitted.</p>
  </div>

<!-- S192 -->
  <div v-click class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/40">
    <div class="font-mono font-bold text-amber-300">Strict Partial #3 &middot; #8 Daemon Lifecycle (<code class="text-gray-300">#L70, L125-L128</code>)</div>
    <p class="text-gray-200 mt-1 m-0">Phase 5 <code class="text-gray-200">bargraph test</code> <code class="text-amber-200">MANUAL</code> reset (fixed in Lesson 0006), <code class="text-gray-200">CLOCK_REALTIME</code> vs. <code class="text-gray-200">date -s</code>, and missing <code class="text-gray-200">SIGTERM</code>.</p>
  </div>

<!-- S193 -->
  <div v-click class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/40">
    <div class="font-mono font-bold text-amber-300">Strict Partial #4 &middot; #10 Test Duplication (<code class="text-gray-300">Makefile#L40-L114</code>)</div>
    <p class="text-gray-200 mt-1 m-0">Phase 5 <code class="text-gray-200">Makefile</code> <code class="text-amber-200">printf</code> <code class="text-gray-200">/tmp</code> duplication (fixed in Lesson 0006) + blocking <code class="text-gray-200">read()</code> without <code class="text-gray-200">poll()</code>.</p>
  </div>
</div>

<!--
Strict Partial 2 on Kconfig hygiene: inherited EXAMPLES_HELLO and OSTEST stayed compiled into defconfig, and CONFIG_RAMLOG_SYSLOG=y wasn't enabled for dmesg. Strict Partial 3 flagged Phase 5's bargraph test MANUAL-reset bug, CLOCK_REALTIME sleep sensitivity if date -s runs, and no SIGTERM handler. And Strict Partial 4 marked Phase 5's Makefile printf test duplication—the shortcut we resolved directly inside Lesson 0006's C99 harness.
-->

---
layout: default
---

<!-- S194 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Honest Engineering Takeaway</h2>
</div>

<div class="mt-6 p-8 rounded-2xl bg-gray-900/80 border border-amber-500/40 text-center space-y-3">
  <div class="text-xs font-mono text-amber-300 uppercase tracking-wider">No Silver Bullet</div>
  <div class="text-2xl font-bold text-white">
    A cold read-only Goldfish audit still caught a 1-line shutdown ordering bug inside <code class="text-emerald-300">10 / 10</code> primary-pass code.
  </div>
</div>

<!-- S195 -->
<div v-click class="mt-8 p-5 rounded-xl bg-cyan-950/25 border border-cyan-500/40 text-center">
  <div class="text-xs font-mono text-cyan-300 uppercase">Next &middot; Self-Correction #4 (Hold Up a Physical Ruler)</div>
  <div class="text-lg font-bold text-white mt-1">
    <code class="text-red-300">docs/vanilla/breadboard_circuit.svg</code> vs. <code class="text-amber-300">docs/egm/breadboard-wiring.svg</code>
  </div>
</div>

<!--
No workflow is magic. Even after two Goldfish passes, turning auditors onto shipping C surfaced real one-line edge cases immediately. Now for Self-Correction #4—remember that XIAO RP2350 seated in Cols E & F earlier? Let's hold a physical ruler up.
-->

---
layout: default
---

<!-- S196 -->
<div class="flex items-center justify-between mb-6">
  <h2 class="text-2xl font-bold m-0">Self-Correction #4: Physical Hardware Check</h2>
  <span class="text-xs font-mono text-gray-400">docs/reports/vanilla-vs-egm-comparison.md#L147</span>
</div>

<div class="mt-6 p-8 rounded-2xl bg-gray-900/80 border border-cyan-500/40 text-center space-y-3">
  <div class="text-xs font-mono text-cyan-300 uppercase">Mechanical Bench Question</div>
  <div class="text-3xl font-extrabold text-white">
    How wide is a Seeed Studio XIAO RP2350 across its two header rows?
  </div>
</div>

<!-- S197 -->
<div v-click class="mt-6 p-6 rounded-2xl bg-emerald-950/25 border-2 border-emerald-500/50 text-center space-y-2">
  <div class="text-xs font-mono text-emerald-400 uppercase tracking-wider">Physical Ground Truth</div>
  <div class="text-3xl font-black font-mono text-emerald-300">
    0.6&Prime; (15.24 mm, 6 breadboard holes)
  </div>
  <div class="text-sm font-mono text-gray-200">
    Straddling a standard <code class="text-white">0.3&Prime;</code> centre ravine = <strong class="text-white">Cols D &amp; H</strong> or <strong class="text-white">Cols C &amp; G</strong>
  </div>
</div>

<!--
Simple mechanical bench question before anyone pushes pins into plastic: how wide is a Seeed Studio XIAO across its two header rows? Zero-point-six inches—fifteen point two-four millimetres. Across a standard zero-point-three-inch centre ravine, that lands in Columns D & H or C & G.
-->

---
layout: hardware-callout
title: "Vanilla&rsquo;s SVG: Seated Across <code class='text-red-300'>c1..h1</code>"
image: /images/breadboard-vanilla-crop-ch.svg
imageCaption: "docs/vanilla/breadboard_circuit.svg#L67-L74, L225-L250 (viewBox 120 180 320 320)"
---

<!-- S198 -->
<div class="space-y-4">
  <div class="p-4 rounded-xl bg-gray-900/80 border border-gray-700 space-y-1">
    <div class="text-xs font-mono text-cyan-300 uppercase">Horizontal Placement</div>
    <p class="text-sm text-gray-200 m-0">
      North pins on <code class="text-white">c1..c7</code> (<code class="text-gray-300">y=260</code>) and South pins on <code class="text-white">h1..h7</code> (<code class="text-gray-300">y=420</code>).
    </p>
  </div>

<!-- S199 -->
  <div v-click class="p-5 rounded-xl bg-red-950/30 border-2 border-red-500/50 space-y-2">
    <div class="text-xs font-mono text-red-400 uppercase">Vanilla Span Error</div>
    <div class="text-2xl font-black font-mono text-red-300">
      c &rarr; h = 0.7&Prime;
    </div>
    <p class="text-sm text-gray-200 m-0">
      <code class="text-red-200">0.2&Prime; + 0.3&Prime; + 0.2&Prime; = 0.7&Prime;</code> &mdash; 1 row too wide; your pins literally won&rsquo;t drop into the breadboard holes!
    </p>
  </div>
</div>

<!--
First let's check Vanilla's SVG diagram: it seated the XIAO horizontally spanning from row c down across the trench to h. Measure c to h on a real breadboard: zero-point-seven inches. One row too wide—the header pins literally won't enter the holes.
-->

---
layout: hardware-callout
title: "EGM&rsquo;s SVG: Seated in <code class='text-amber-300'>Cols E &amp; F</code>"
image: /images/breadboard-wiring-crop-ef.svg
imageCaption: "docs/egm/breadboard-wiring.svg#L121, L280-L333 (viewBox 150 130 500 500)"
---

<!-- S200 -->
<div class="space-y-4">
  <div class="p-4 rounded-xl bg-gray-900/80 border border-gray-700 space-y-1">
    <div class="text-xs font-mono text-amber-300 uppercase">Vertical Placement</div>
    <p class="text-sm text-gray-200 m-0">
      Seats the XIAO vertically in <code class="text-white">Col E</code> (<code class="text-gray-300">x=340</code>) and <code class="text-white">Col F</code> (<code class="text-gray-300">x=460</code>).
    </p>
  </div>

<!-- S201 -->
  <div v-click class="p-5 rounded-xl bg-red-950/30 border-2 border-red-500/50 space-y-2">
    <div class="text-xs font-mono text-red-400 uppercase">EGM Span Error (Disguised in SVG!)</div>
    <div class="text-2xl font-black font-mono text-red-300">
      Col E..F = 0.3&Prime; (Half-Width!)
    </div>
    <p class="text-sm text-gray-200 m-0">
      Stretched the SVG centre trench to <code class="text-red-200">120px</code> (vs. <code class="text-gray-300">35px</code> hole pitch) so it <em>looked</em> right on screen!
    </p>
  </div>
</div>

<!--
Surely ten-out-of-ten EGM nailed the breadboard geometry? Zoom in on EGM's SVG: it placed the XIAO vertically in Columns E and F. E to F spans zero-point-three inches—half the XIAO's width! Worse, EGM stretched the SVG trench to 120 pixels so it looked right.
-->

---
layout: default
---

<!-- S202 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Law of Contracts &amp; The Blind-Spot Corollary</h2>
</div>

<div class="p-6 rounded-2xl bg-emerald-950/20 border border-emerald-500/45 space-y-2 mt-4">
  <div class="text-xs font-mono text-emerald-400 uppercase tracking-wider">The Law of Contracts</div>
  <div class="text-2xl font-bold text-white">
    Inside Contracts (NuttX + RP2350 Silicon): <span class="text-emerald-300 font-mono">EGM 10/10</span> vs. <span class="text-amber-300 font-mono">Vanilla 5/10</span>
  </div>
  <p class="text-sm text-gray-300 m-0">Where we had grounded <code class="text-emerald-300">README.md</code> contracts and Goldfish checks, EGM dominated.</p>
</div>

<!-- S203 -->
<div v-click class="mt-6 p-6 rounded-2xl bg-red-950/25 border border-red-500/45 space-y-2">
  <div class="text-xs font-mono text-red-400 uppercase tracking-wider">The Blind-Spot Corollary</div>
  <div class="text-2xl font-bold text-white">
    Outside Contracts (Breadboard Pitch): <span class="text-red-300 font-mono">Both Hallucinated (0.7&Prime; / 0.3&Prime; vs 0.6&Prime;)</span>
  </div>
  <p class="text-sm text-gray-300 m-0">Now here is how you turn production warts into deterministic <code class="text-purple-300">SKILL.md</code> gates.</p>
</div>

<!--
And there is the Law of Contracts: inside our NuttX and silicon contract boundary, EGM crushed Vanilla ten to five. Where neither run had a breadboard pitch contract, both hallucinated. Now here is how you turn production warts into SKILL.md gates.
-->

---
layout: default
---

<!-- S204 -->
<div class="flex items-center justify-between mb-2">
  <h2 class="text-xl font-bold m-0">Layer 3 Payoff: Closing the &ldquo;Roll Your Own Skills&rdquo; Loop</h2>
  <span class="text-xs font-mono text-purple-300">Trace 25-98ffad8f (0:06:45, 54 steps)</span>
</div>

<div class="grid grid-cols-2 gap-3 compact-code">

<!-- S205 -->
  <div v-click class="p-2.5 rounded-xl bg-gray-900/70 border border-amber-500/35 space-y-1">
    <div class="text-[11px] font-mono text-amber-300">1. Trace 25-98ffad8f#L21-L22 (Chrome Bug)</div>

```markdown {2-4}
### Turn 1 (Step 0)
> The html in @[tutorial] contains unrendered TeX markup, and
> there is an odd bleed through in the form of a raw xml'ish
> markup in the HTML like `<2 id="breadboard-topology">` that appears in multiple lessons.
```

  </div>

<!-- S206 -->
  <div v-click class="p-2.5 rounded-xl bg-gray-900/70 border border-cyan-500/35 space-y-1">
    <div class="text-[11px] font-mono text-cyan-300">2. build.py#L22-L25, L736-L737 (Root Cause)</div>

```python {4}
HEADING_ID_RE = re.compile(r"(<h([1-6])>)((?:(?!</h\2>).)*?)\s*\{#([a-zA-Z0-9_-]+)\}\s*(</h\2>)")
# Before: HEADING_ID_RE.sub(r'<\2 id="\4">\3\5', rendered) -> emitted <2 id="...">!
# After (#L737):
rendered = HEADING_ID_RE.sub(r'<h\2 id="\4">\3\5', rendered)
# Plus Lesson 0001..0003 raw TeX like $R_S$ -> Unicode / inline code
```

  </div>

<!-- S207 -->
  <div v-click class="p-2.5 rounded-xl bg-gray-900/70 border border-emerald-500/35 space-y-1">
    <div class="text-[11px] font-mono text-emerald-300">3. build.py#L37-L38, L761-L770 (Permanent Gate)</div>

```python {1-2,6-7}
MALFORMED_TAG_RE = re.compile(r"<[0-9]\b|\{#[a-zA-Z0-9_-]+\}")
UNRENDERED_TEX_RE = re.compile(r"\$[^$\n]+\$|\\(?:approx|pm|sim|Omega|text|le|ge|in)\b")
prose_only = STRIP_CODE_AND_PRE_RE.sub("", rendered)
for bad_tag in MALFORMED_TAG_RE.finditer(prose_only):
    errors.append(f"{page.rel_src}: malformed HTML tag ...")
for bad_tex in UNRENDERED_TEX_RE.finditer(prose_only):
    errors.append(f"{page.rel_src}: unrendered TeX markup ...")
```

  </div>

<!-- S208 -->
  <div v-click class="p-2.5 rounded-xl bg-gray-900/70 border border-purple-500/40 space-y-1">
    <div class="text-[11px] font-mono text-purple-300">4. .agents/skills/teach-code/SKILL.md#L93</div>

```markdown {2-5}
- **Heading Anchor Suffixes (`{#id}`) & Clean Math/Symbol Formatting**:
  converting `## Heading {#my-id}` to `<h2 id="my-id">Heading</h2>` — always
  preserve the leading `h` in `<h\2 id="\4">` so malformed `<2 id="...">` tags
  never appear! ... enforce in `build.py --check` that neither raw `{#...}`,
  malformed `<[1-6]`, nor unrendered TeX math (`$...$`) leak into rendered HTML.
```

  </div>

</div>

<!--
Remember earlier I said Layer 3 is rolling your own skills? Even after Phase 6 finished recording, Chrome showed two tiny warts. Opening the compiled tutorial pages in Chrome revealed two things build.py missed: raw dollar-sign TeX markup and <2 id="breadboard-topology"> heading tags. One missing letter h inside the regex replacement emitted <2 id="..."> tags, while $R_S$ slipped through without a math renderer. Step one: never just patch the symptom—turn both catches into MALFORMED_TAG_RE and UNRENDERED_TEX_RE compiler gates inside build.py --check. Step two: use /agy-customizations to fold that exact regex and Unicode invariant upstream into .agents/skills/teach-code/SKILL.md line 93.
-->

---
layout: default
---

<!-- S209 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Skill Flywheel: Never Pay the Same Tuition Twice</h2>
</div>

<div class="mt-8 grid grid-cols-4 gap-4 text-center items-center">
  <div class="p-5 rounded-xl bg-red-950/20 border border-red-500/40 space-y-2">
    <div class="text-xs font-mono text-red-300 uppercase">Step 1</div>
    <div class="text-base font-bold text-white">Real Project Bug</div>
    <div class="text-xs text-gray-300">Production edge case bites you once</div>
  </div>

  <div class="p-5 rounded-xl bg-emerald-950/20 border border-emerald-500/40 space-y-2">
    <div class="text-xs font-mono text-emerald-300 uppercase">Step 2</div>
    <div class="text-base font-bold text-white">Build / Linter Gate</div>
    <div class="text-xs text-gray-300">Encode invariant into <code class="text-emerald-200">--check</code></div>
  </div>

  <div class="p-5 rounded-xl bg-purple-950/20 border border-purple-500/40 space-y-2">
    <div class="text-xs font-mono text-purple-300 uppercase">Step 3</div>
    <div class="text-base font-bold text-white"><code class="text-purple-200">SKILL.md</code> Rule</div>
    <div class="text-xs text-gray-300">Upstream via <code class="text-purple-200">/agy-customizations</code></div>
  </div>

  <div class="p-5 rounded-xl bg-cyan-950/20 border border-cyan-500/40 space-y-2">
    <div class="text-xs font-mono text-cyan-300 uppercase">Step 4</div>
    <div class="text-base font-bold text-white">Cleaner Next Run</div>
    <div class="text-xs text-gray-300">Every future codebase benefits</div>
  </div>
</div>

<div class="mt-8 text-center text-sm text-gray-300">
  Treat your <code class="text-purple-300">SKILL.md</code> files like living engineering craft &mdash; not static downloads.
</div>

<!--
Treat your SKILL.md files like living engineering craft rather than static downloads—you never pay the same tuition twice.
-->

---
layout: default
---

<!-- S210 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Three Habits for Monday Morning</h2>
</div>

<div class="space-y-4 mt-4">
  <div class="p-5 rounded-xl bg-cyan-950/20 border border-cyan-500/40">
    <div class="text-xs font-mono text-cyan-300 uppercase">1. Roll Docs for the Agent</div>
    <div class="text-lg font-bold text-white mt-1">3,241 bottom-up <code class="text-cyan-300">README.md</code> contracts + Elephant &amp; Goldfish before C</div>
    <p class="text-xs text-gray-300 mt-1 m-0">Map the terrain and kill bugs in markdown before writing a single line of implementation.</p>
  </div>

<!-- S211 -->
  <div v-click class="p-5 rounded-xl bg-emerald-950/20 border border-emerald-500/40">
    <div class="text-xs font-mono text-emerald-300 uppercase">2. Roll Docs for the Human</div>
    <div class="text-lg font-bold text-white mt-1">Interactive <code class="text-emerald-300">/teach-code</code> codelab + strict C99 labs</div>
    <p class="text-xs text-gray-300 mt-1 m-0">Because <em>you</em> own the outcome &mdash; pay down comprehension debt deliberately.</p>
  </div>

<!-- S212 -->
  <div v-click class="p-5 rounded-xl bg-purple-950/20 border border-purple-500/40">
    <div class="text-xs font-mono text-purple-300 uppercase">3. Roll Your Own Skills</div>
    <div class="text-lg font-bold text-white mt-1">Matt Pocock&rsquo;s <code class="text-purple-300">/teach</code> + <code class="text-purple-300">/agy-customizations</code> &rarr; <code class="text-purple-300">/teach-code</code></div>
    <p class="text-xs text-gray-300 mt-1 m-0">Start from generous community craft and harden your skills every time a project bites.</p>
  </div>
</div>

<!--
Three habits to try on Monday morning. First: roll directory contracts and run Dave Rensin's Elephant and Goldfish protocol before coding. Second: roll interactive /teach-code codelabs from verified specs so your team—not only the repository—holds the architecture. Third: start from generous community craft like Matt Pocock's /teach and sharpen your own SKILL.md files every time a project bites.
-->

---
layout: center
class: text-center
---

<!-- S213 -->
<div class="space-y-6 max-w-3xl mx-auto">
  <blockquote class="text-4xl font-extrabold text-white tracking-tight leading-snug">
    &ldquo;Design with an elephant.<br/>
    Review with a goldfish.<br/>
    <span class="text-emerald-300">Own the outcome</span>&mdash;and keep sharpening the skills.&rdquo;
  </blockquote>
</div>

<!--
Design with an elephant. Review with a goldfish. Own the outcome—and keep sharpening the skills that guide them both.
-->

---
layout: default
---

<!-- S214 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Links &amp; Resources</h2>
</div>

<div class="grid grid-cols-2 gap-4 mt-4">
  <div class="p-4 rounded-xl bg-gray-900 border border-cyan-500/35 space-y-1">
    <div class="text-xs font-mono text-cyan-400 uppercase">Elephant-Goldfish Model (Dave Rensin)</div>
    <div class="text-sm font-bold font-mono">
      <a href="https://github.com/google/elephant-goldfish-model" target="_blank" class="text-cyan-300 hover:underline">github.com/google/elephant-goldfish-model</a>
    </div>
    <p class="text-xs text-gray-400 m-0">
      <a href="https://drensin.medium.com/elephants-goldfish-and-the-new-golden-age-of-software-engineering-c33641a48874" target="_blank" class="text-gray-300 underline">Medium Essay: Elephants, Goldfish &amp; Software Engineering</a>
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-purple-500/35 space-y-1">
    <div class="text-xs font-mono text-purple-400 uppercase">Community Pedagogy Lineage</div>
    <div class="text-sm font-bold font-mono text-purple-300">Matt Pocock&rsquo;s /teach &rarr; /teach-code</div>
    <p class="text-xs text-gray-400 m-0">
      <code class="text-gray-300">.agents/skills/teach/SKILL.md</code> &amp; <code class="text-gray-300">.agents/skills/teach-code/SKILL.md</code>
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-emerald-500/35 space-y-1">
    <div class="text-xs font-mono text-emerald-400 uppercase">Experiment Repository &amp; Traces</div>
    <div class="text-sm font-bold font-mono">
      <a href="https://github.com/domesticmouse/nuttxspace" target="_blank" class="text-emerald-300 hover:underline">github.com/domesticmouse/nuttxspace</a>
    </div>
    <p class="text-xs text-gray-400 m-0">
      Full <code class="text-gray-300">nuttxspace-vanilla</code> vs. <code class="text-gray-300">nuttxspace-egm</code> artifacts, telemetry &amp; recordings
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-amber-500/35 space-y-1">
    <div class="text-xs font-mono text-amber-400 uppercase">Interactive Codelab</div>
    <div class="text-sm font-bold font-mono">
      <a href="./tutorial/index.html" target="_blank" class="text-amber-300 hover:underline">docs/egm/tutorial/ (Open Compiled Course &nearr;)</a>
    </div>
    <p class="text-xs text-gray-400 m-0">
      6 Tufte lessons, 3 interactive JS simulators, 4 printable sheets &amp; 6 C99 labs
    </p>
  </div>
</div>

<!--
Thanks very much everyone—take an Elephant-Goldfish pass onto your gnarliest legacy codebase, and come grab me afterwards for questions!
-->
