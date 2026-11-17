---
layout: default
---

<!-- S124 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Phase 5 (<code class="text-cyan-300">05-implement</code>): Calm &amp; Boring Coding</h2>
</div>

<div class="mt-8 grid grid-cols-3 gap-4 items-center text-center">
  <div class="p-6 rounded-xl bg-emerald-950/20 border border-emerald-500/40 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">Pass-2 Hardened Spec</div>
    <div class="text-base font-bold font-mono text-white">xiao-rp2350-bargraph.md</div>
    <div class="text-xs text-emerald-300">705 lines &middot; 0 Blockers</div>
  </div>

  <div class="p-6 rounded-xl bg-gray-900/80 border border-gray-700 space-y-2">
    <div class="text-xs font-mono text-gray-400 uppercase">Handoff</div>
    <div class="text-2xl font-black font-mono text-cyan-300">&rarr;</div>
    <div class="text-xs text-gray-300">Zero architectural guesses left</div>
  </div>

  <div class="p-6 rounded-xl bg-cyan-950/20 border border-cyan-500/40 space-y-2">
    <div class="text-xs font-mono text-cyan-300 uppercase">Executor</div>
    <div class="text-lg font-bold text-white">Phase 5 Implementer</div>
    <div class="text-xs text-gray-300">Writes board BSP, app, tests &amp; 9 contracts</div>
  </div>
</div>

<!--
With four Blockers and seven Majors killed inside markdown, Phase 5 hands xiao-rp2350-bargraph.md to an implementer so coding stays calm and boring.
-->

---
layout: cast-full
title: "Phase 5: First-Attempt Implementation (05-implement)"
cast: /casts/05-implement.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
initialStopPoint: 3
showChapterBar: true
---

<!-- S125 -->
<!--
Here is the prompt handing that Pass-2-hardened spec straight to implementation. Notice the behavioural effect compared to Vanilla: there are zero architectural guesses left to improvise mid-flight. At twenty-seven times speed across thirteen raw minutes, the session authors the custom board files, rp23xx_bringup.c, bargraph_main.c, and nine new directory contracts, fixes one macOS BSD mktemp flag in the host test harness, and runs configure.sh plus the NuttX ARM build—delivering a clean UF2 firmware binary on the very first compile attempt.
-->

---
layout: default
---

<!-- S126 -->
<div class="flex items-center justify-between mb-6">
  <h2 class="text-2xl font-bold m-0">First-Attempt Build Verdict</h2>
  <span class="text-xs font-mono text-gray-400">docs/reports/vanilla-vs-egm-comparison.md#L39</span>
</div>

<div class="mt-4 p-8 rounded-2xl bg-emerald-950/20 border-2 border-emerald-500/50 text-center space-y-3">
  <div class="text-xs font-mono text-emerald-400 uppercase tracking-widest">Attempt #1 Build</div>
  <div class="text-4xl font-black font-mono text-emerald-300">
    nuttx/nuttx.uf2 (347K)
  </div>
  <div class="text-base font-mono text-gray-200">
    177,232 B FLASH &nbsp;&middot;&nbsp; 13,712 B RAM &nbsp;&middot;&nbsp; Built cleanly on Attempt #1
  </div>
</div>

<!-- S127 -->
<div v-click class="mt-6 grid grid-cols-3 gap-4 text-center font-mono text-xs">
  <div class="p-4 rounded-xl bg-emerald-950/25 border border-emerald-500/40 text-emerald-200">
    <strong class="text-2xl block text-white mb-1">0</strong>
    Firmware build retries
  </div>
  <div class="p-4 rounded-xl bg-gray-900/80 border border-gray-700 text-gray-200">
    <strong class="text-2xl block text-amber-300 mb-1">1</strong>
    macOS BSD <code class="text-amber-200">mktemp</code> fix at Step 74
  </div>
  <div class="p-4 rounded-xl bg-emerald-950/25 border border-emerald-500/40 text-emerald-200">
    <strong class="text-2xl block text-white mb-1">0</strong>
    <code class="text-emerald-300">xmllint</code> SVG errors
  </div>
</div>

<!--
nuttx/nuttx.uf2, three hundred forty-seven kilobytes, compiled, linked, and packed cleanly on the very first attempt—zero firmware build failures. Our lone build hiccup was a macOS BSD mktemp flag inside the host test target—zero NuttX Kconfig drops or compiler errors.
-->

---
layout: default
---

<!-- S128 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Rolling the Map Forward</h2>
  <span class="text-xs font-mono text-cyan-300">docs/egm/xiao-rp2350-bargraph/README.md#L72-L76</span>
</div>

<div class="p-6 rounded-xl bg-cyan-950/15 border border-cyan-500/35 space-y-3 mt-4">
  <div class="flex items-center justify-between">
    <span class="text-lg font-bold text-white">9 new 5-section <code class="text-cyan-300">README.md</code> contracts</span>
    <span class="px-2.5 py-1 rounded bg-cyan-500/20 text-xs font-mono text-cyan-300">Map Updated Alongside Code</span>
  </div>
  <div class="grid grid-cols-3 gap-2 text-xs font-mono text-gray-300 pt-1">
    <div class="p-2 rounded bg-gray-900/80 border border-gray-800">xiao-rp2350-bargraph/</div>
    <div class="p-2 rounded bg-gray-900/80 border border-gray-800">apps/bargraph/</div>
    <div class="p-2 rounded bg-gray-900/80 border border-gray-800">board/configs/bargraph/</div>
  </div>
</div>

<!-- S129 -->
<div v-click class="mt-5 p-6 rounded-xl bg-amber-950/20 border border-amber-500/40 space-y-2">
  <div class="flex items-center justify-between">
    <span class="text-xs font-mono text-amber-300 uppercase">Honest Contract Boundary (Pass 2 CA-02 &middot; #L75)</span>
    <span class="text-xs font-mono text-gray-400">Intentional CONFIG_BUILD_FLAT=y Boundary Exceptions</span>
  </div>
  <p class="text-sm text-gray-200 m-0">
    Explicitly documents why flat-build <code class="text-amber-300">bargraphd</code> in <code class="text-gray-200">apps/</code> calls kernel <code class="text-amber-300">up_enable_irq()</code> / <code class="text-amber-300">up_disable_irq()</code> without modifying upstream submodules.
  </p>
</div>

<!--
Better yet, Phase 5 rolls the map forward—adding nine new five-section README.md contracts covering every directory we introduced. Including Pass 2 CA-02: explicitly documenting why flat-build bargraphd calls kernel up_enable_irq directly without modifying upstream submodules.
-->

---
layout: hardware-callout
title: "Generated Wiring Guide &amp; SVG"
image: /images/breadboard-wiring.svg
imageCaption: "docs/egm/breadboard-wiring.svg — 330 Ω resistors (~3.94 mA/LED) + 100 nF bypass cap C1"
---

<!-- S130 -->
<div class="space-y-4">
  <div class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/35 space-y-2">
    <div class="text-xs font-mono text-emerald-300 uppercase">Consistent Hardware Docs</div>
    <div class="text-sm text-gray-200">
      <code class="text-emerald-300">BREADBOARD_WIRING.md</code> + <code class="text-emerald-300">breadboard-wiring.svg</code>:
    </div>
    <ul class="text-xs text-gray-300 space-y-1 list-disc pl-5 m-0">
      <li>Six <code class="text-white">330 Ω</code> resistors (<code class="text-gray-200">~3.94 mA</code>/LED, <code class="text-gray-200">23.6 mA</code> total)</li>
      <li><code class="text-white">100 nF</code> wiper bypass capacitor <code class="text-gray-200">C1</code></li>
    </ul>
  </div>

<!-- S131 -->
  <div v-click class="p-4 rounded-xl bg-amber-950/25 border border-amber-500/45 space-y-1">
    <div class="text-xs font-mono text-amber-300 uppercase">Foreshadowing Seed &middot; Keep a Mental Bookmark</div>
    <p class="text-xs text-gray-200 m-0">
      Look closely at the XIAO RP2350 seated in <strong class="text-amber-200">Cols E &amp; F</strong> (<code class="text-gray-300">x=340 / x=460</code>) on the left &mdash; we will measure that with a ruler in Act V.
    </p>
  </div>

<!-- S132 -->
  <div v-click class="p-4 rounded-xl bg-purple-950/25 border border-purple-500/45 space-y-1">
    <div class="text-xs font-mono text-purple-300 uppercase">Next &middot; Act IV</div>
    <p class="text-sm font-bold text-white m-0">
      Rolling Docs for the Human: <code class="text-purple-300">/teach-code</code> &mdash; Do you understand the C you&rsquo;re about to ship?
    </p>
  </div>
</div>

<!--
And it generates a consistent wiring guide and SVG diagram: 330-ohm resistors, 23.6 milliamps total, plus a 100-nanofarad wiper capacitor. Keep one eye on the XIAO seated in columns E and F right there—we will measure that with a ruler in Act V. The agent nailed the firmware; now comes the harder question—do you actually understand the eight hundred lines of C you're about to ship?
-->
