---
layout: center
class: text-center
---

<!-- S058 -->
<div class="space-y-5 max-w-3xl mx-auto">
  <div class="text-xs font-mono text-cyan-400 uppercase tracking-widest">Experiment B &middot; nuttxspace-egm</div>
  <h1 class="text-4xl font-extrabold text-white leading-tight">
    Rolling Docs for the Agent
  </h1>
  <p class="text-xl font-mono text-cyan-300">
    The Elephant-Goldfish Model
  </p>

<!-- S059 -->
  <div v-click class="mt-6 p-4 rounded-xl bg-cyan-950/25 border border-cyan-500/40 text-sm text-gray-200">
    <div class="text-xs font-mono text-cyan-300 uppercase mb-1">Rensin Step 1 &middot; Feeding the Elephant via /egm-bootstrap</div>
    <div class="font-mono text-white font-bold">
      2,607 <code class="text-cyan-300">nuttx/</code> + 634 <code class="text-cyan-300">apps/</code> = <span class="text-emerald-300">3,241 README.md contracts</span>
    </div>
  </div>
</div>

<!--
Welcome to Experiment B, nuttxspace-egm—running the identical prompt inside the Elephant-Goldfish Model across our contract-mapped submodules. Step one feeds the Elephant cheaply: 3,241 bottom-up README.md files summarising every directory across nuttx/ and apps/ before anyone starts coding.
-->

---
layout: default
---

<!-- S060 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">5-Section Contract Anatomy</h2>
  <span class="text-xs font-mono text-cyan-300">nuttx/arch/arm/src/rp23xx/README.md (30 headers, 40 source files)</span>
</div>

<div class="space-y-2.5 mt-3 text-xs">

<!-- S061 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/80 border border-cyan-500/30">
    <div class="font-mono font-bold text-cyan-300">1&ndash;2. Overview + Subdirectories &amp; Boundaries (<code class="text-gray-300">#L3-L8</code>)</div>
    <div class="text-gray-300 mt-0.5">Defines ownership boundaries and child links (<code class="text-gray-200">./hardware/README.md</code>) before reading 40 C files.</div>
  </div>

<!-- S062 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/80 border border-emerald-500/30">
    <div class="font-mono font-bold text-emerald-300">3. Interface (<code class="text-gray-300">#L9-L40</code>)</div>
    <div class="text-gray-300 mt-0.5">Public entry points with <code class="text-gray-200">[file:symbol]</code> citations: <code class="text-emerald-200">[rp23xx_adc.h:rp23xx_adc_setup]</code>, <code class="text-emerald-200">[rp23xx_gpio.h:rp23xx_gpio_set_function]</code>.</div>
  </div>

<!-- S063 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/80 border border-amber-500/35">
    <div class="font-mono font-bold text-amber-300">4. Invariants (<code class="text-gray-300">#L42-L55</code>)</div>
    <div class="text-gray-300 mt-0.5">Cross-layer rules grep misses: <code class="text-amber-200">my_reset()</code>, <code class="text-amber-200">START_ONCE</code>, <code class="text-amber-200">RP2350-E9</code>, <code class="text-amber-200">[rp23xx_adc.c:ASSERT]</code>, <code class="text-amber-200">enter_critical_section</code>.</div>
  </div>

<!-- S064 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/80 border border-purple-500/35">
    <div class="font-mono font-bold text-purple-300">5. Side Effects &amp; Verification (<code class="text-gray-300">#L56-L71</code>)</div>
    <div class="text-gray-300 mt-0.5">MMIO registers touched and concrete tests or assertions proving contract truth still holds.</div>
  </div>

</div>

<!--
Open nuttx/arch/arm/src/rp23xx/README.md and you see the exact five-section contract format shared across all three thousand directories. Sections one and two state what lives here and link child boundaries—so the agent navigates hierarchy without slurping forty files. Section three lists the public interface via [file:symbol] citations—giving the agent exact entry points instead of blind string matches. Section four documents invariants—assert preconditions, critical-section rules, and silicon constraints that grep never surfaces by itself. And sections on side effects and verification point directly at MMIO registers touched and tests proving those invariants still hold.
-->

---
layout: default
---

<!-- S065 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Bottom-Up Synthesis: Compressing 30 Years of C</h2>
</div>

<div class="mt-8 grid grid-cols-4 gap-4 items-center text-center">
  <div class="p-5 rounded-xl bg-gray-900/80 border border-gray-700 space-y-2">
    <div class="text-xs font-mono text-gray-400 uppercase">Tier 1 &middot; Leaves</div>
    <div class="text-sm font-bold font-mono text-white">Leaf <code class="text-cyan-300">README.md</code></div>
    <div class="text-xs text-gray-400">Registers &amp; file symbols</div>
  </div>

  <div class="p-5 rounded-xl bg-cyan-950/20 border border-cyan-500/35 space-y-2">
    <div class="text-xs font-mono text-cyan-400 uppercase">Tier 2 &middot; Drivers</div>
    <div class="text-sm font-bold font-mono text-white"><code class="text-cyan-300">rp23xx/README.md</code></div>
    <div class="text-xs text-gray-300">Chip errata &amp; invariants</div>
  </div>

  <div class="p-5 rounded-xl bg-emerald-950/20 border border-emerald-500/35 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">Tier 3 &middot; Submodule</div>
    <div class="text-sm font-bold font-mono text-white"><code class="text-emerald-300">nuttx/README.md</code></div>
    <div class="text-xs text-gray-300">Boot &amp; build contracts</div>
  </div>

  <div class="p-5 rounded-xl bg-purple-950/20 border border-purple-500/40 space-y-2">
    <div class="text-xs font-mono text-purple-400 uppercase">Tier 4 &middot; Workspace</div>
    <div class="text-sm font-bold font-mono text-white">Root <code class="text-purple-300">README.md</code></div>
    <div class="text-xs text-gray-300">Cross-submodule layout</div>
  </div>
</div>

<div class="mt-8 p-4 rounded-xl bg-gray-900/60 border border-gray-800 text-center text-sm text-gray-200">
  The Elephant navigates lightweight architectural contracts in seconds instead of grepping thousands of C files.
</div>

<!--
Leaf modules roll upward into subsystem blueprints, compressing thirty years of NuttX architecture into lightweight markdown your Elephant reads in seconds.
-->

---
layout: default
---

<!-- S066 -->
<div class="flex items-center justify-between mb-2">
  <h2 class="text-xl font-bold m-0">Freeze-Frame: <code class="text-cyan-300">01-bootstrap</code></h2>
  <span class="text-xs font-mono text-amber-300">Stop Point 8: Q1: Run EGM Drift Report? (00:38 warped &middot; 7x)</span>
</div>

<div class="h-[340px] w-full">
  <AsciinemaPlayer
    src="/casts/01-bootstrap.cast"
    :autoPlay="false"
    :speed="2.0"
    :idleTimeLimit="2.0"
    fit="both"
    :showChapterBar="true"
    :initialStopPoint="8"
  />
</div>

<!-- S067 -->
<div v-click class="mt-3 grid grid-cols-4 gap-3 text-center text-xs font-mono">
  <div class="p-2.5 rounded-lg bg-gray-900 border border-gray-800 text-white"><strong>72</strong> agent steps</div>
  <div class="p-2.5 rounded-lg bg-gray-900 border border-gray-800 text-white"><strong>40</strong> tool calls (17 reads)</div>
  <div class="p-2.5 rounded-lg bg-cyan-950/30 border border-cyan-500/40 text-cyan-300"><strong>6</strong> README.md reads</div>
  <div class="p-2.5 rounded-lg bg-emerald-950/30 border border-emerald-500/40 text-emerald-300"><strong>0</strong> C files touched</div>
</div>

<!--
Invoking /egm-bootstrap anchors the workspace root in eight raw minutes, reading nuttx/ and apps/ contracts before pausing here on drift detection. Seventy-two steps, six contract reads, zero lines of C—and the Elephant already holds the exact cross-submodule build boundary in memory.
-->

---
layout: default
---

<!-- S068 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Bootstrapped Root Contract</h2>
  <span class="text-xs font-mono text-cyan-300">docs/egm/README.md#L7-L18 (28 lines)</span>
</div>

<div class="grid grid-cols-3 gap-4 mt-6">
  <div class="p-5 rounded-xl bg-gray-900/80 border border-cyan-500/35 space-y-2">
    <div class="text-xs font-mono text-cyan-400 uppercase">Sibling 1</div>
    <div class="text-xl font-bold font-mono text-white">apps/</div>
    <div class="text-xs text-gray-300">Upstream NuttX apps submodule (read-only)</div>
  </div>

  <div class="p-5 rounded-xl bg-gray-900/80 border border-cyan-500/35 space-y-2">
    <div class="text-xs font-mono text-cyan-400 uppercase">Sibling 2</div>
    <div class="text-xl font-bold font-mono text-white">nuttx/</div>
    <div class="text-xs text-gray-300">Upstream RTOS kernel submodule (read-only)</div>
  </div>

  <div class="p-5 rounded-xl bg-emerald-950/20 border border-emerald-500/40 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">Sibling 3</div>
    <div class="text-xl font-bold font-mono text-white">xiao-rp2350-bargraph/</div>
    <div class="text-xs text-gray-300">Out-of-tree custom board &amp; app workspace</div>
  </div>
</div>

<div class="mt-6 p-4 rounded-xl bg-gray-900/70 border border-gray-800 space-y-1 text-sm text-gray-200">
  <div class="text-xs font-mono text-amber-300 uppercase">Global Invariants Locked at Root</div>
  <p class="m-0">Flat kernel/user-space boundary (<code class="text-cyan-300">CONFIG_BUILD_FLAT=y</code>) &middot; Monotonic kernel boot chain &middot; Strict submodule isolation</p>
</div>

<!--
Out pops a crisp twenty-eight-line root blueprint locking sibling layout, flat kernel/user-space separation, and the monotonic kernel boot chain.
-->

---
layout: default
---

<!-- S069 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">Trust-But-Verify Hook: <code class="text-cyan-300">/egm-drift-report</code></h2>
  <span class="text-xs font-mono text-gray-400">Markdown Contract &harr; Live C Source Audit</span>
</div>

<div class="space-y-3 mt-2">

<!-- S070 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/70 border border-amber-500/35 space-y-1.5">
    <div class="text-xs font-mono text-amber-300">docs/egm/reports/2026-11-16_DRIFT_REPORT.md#L9-L11 (11 lines)</div>

```markdown {3}
## nuttx/README.md

- Incomplete State Progression in `Invariants`: Line 79 states that `g_nx_initstate` progresses monotonically through `OSINIT_BOOT`, `OSINIT_TASKLISTS`, `OSINIT_MEMORY`, `OSINIT_OSREADY`, and `OSINIT_IDLELOOP` during `nx_start()`, omitting the intermediate `OSINIT_HARDWARE` state assigned at `nuttx/sched/init/nx_start.c:660` (and checked by `OSINIT_HW_READY()` in `nuttx/include/nuttx/init.h:104`).
```

  </div>

<!-- S071 -->
  <div v-click class="p-3 rounded-xl bg-gray-900/70 border border-cyan-500/35 space-y-1.5">
    <div class="text-xs font-mono text-cyan-300">nuttx/sched/init/nx_start.c#L658-L664</div>

```c {3}
  /* Hardware resources are now available */

  g_nx_initstate = OSINIT_HARDWARE;
  sched_trace_mark("HARDWARE");

  /* Setup for Multi-Tasking ************************************************/
```

  </div>

</div>

<!--
Because markdown contracts can go stale over time, Phase 1 immediately follows bootstrap with /egm-drift-report against the live C tree. Eleven lines of drift report immediately spot a missing enum state in nuttx/README.md line 79: OSINIT_HARDWARE was omitted. It cites nx_start.c line 660 verbatim, proving where hardware init finishes and reconciling contract truth before any design begins.
-->

---
layout: default
---

<!-- S072 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Phase 1 Payoff: Grounded Map in 8 Minutes</h2>
</div>

<div class="mt-6 p-7 rounded-2xl bg-emerald-950/20 border border-emerald-500/40 space-y-3">
  <div class="text-xs font-mono text-emerald-400 uppercase tracking-wider">01-bootstrap Complete</div>
  <div class="text-2xl font-bold text-white">
    <code class="text-emerald-300">8m 14s</code> raw &rarr; Grounded root <code class="text-emerald-300">README.md</code> + live drift resolved
  </div>
  <p class="text-sm text-gray-300 m-0">
    Zero architecture decisions guessed; cross-submodule boundaries verified directly against C source.
  </p>
</div>

<!-- S073 -->
<div v-click class="mt-8 p-5 rounded-xl bg-cyan-950/25 border border-cyan-500/40 text-center">
  <div class="text-xs font-mono text-cyan-300 uppercase tracking-wider">Next &middot; Phase 2 (Act III.b)</div>
  <div class="text-xl font-bold text-white mt-1">
    <code class="text-cyan-300">/egm-brainstorm</code> &mdash; Interrogating the Design With Zero Code Allowed
  </div>
</div>

<!--
Eight minutes of setup buys a verified workspace map and catches stale documentation before we make a single architecture call. Now let's invoke /egm-brainstorm and watch the Elephant grow without touching a single line of C.
-->
