---
layout: default
---

<!-- S024 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Experiment A Baseline: <code class="text-red-300">nuttxspace-vanilla</code></h2>
</div>

<div class="grid grid-cols-3 gap-5 mt-8">
  <div class="p-6 rounded-xl bg-red-950/15 border border-red-500/35 space-y-2">
    <div class="text-xs font-mono text-red-400 uppercase">Documentation</div>
    <div class="text-4xl font-black font-mono text-white">13</div>
    <div class="text-xs text-gray-300">Upstream <code class="text-red-300">README.md</code> files across <code class="text-gray-200">nuttx/</code> + <code class="text-gray-200">apps/</code></div>
  </div>

  <div class="p-6 rounded-xl bg-gray-900/80 border border-gray-800 space-y-2">
    <div class="text-xs font-mono text-gray-400 uppercase">Execution Model</div>
    <div class="text-4xl font-black font-mono text-white">1</div>
    <div class="text-xs text-gray-300">Single parent session (<code class="text-cyan-300">60c052d5</code>)</div>
  </div>

  <div class="p-6 rounded-xl bg-gray-900/80 border border-gray-800 space-y-2">
    <div class="text-xs font-mono text-gray-400 uppercase">Reviewers &amp; Skills</div>
    <div class="text-4xl font-black font-mono text-white">0</div>
    <div class="text-xs text-gray-300">Subagents &middot; zero custom workflow skills</div>
  </div>
</div>

<div class="mt-6 text-xs font-mono text-gray-500">
  Source: docs/reports/vanilla-vs-egm-comparison.md#L13, L33-L38
</div>

<!--
Experiment A starts on upstream master with thirteen README.md files, a single chat session, and zero custom workflow skills.
-->

---
layout: default
---

<!-- S025 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Rensin&rsquo;s &ldquo;Weeds Syndrome&rdquo;</h2>
</div>

<div class="mt-8 p-8 rounded-2xl bg-red-950/15 border border-red-500/35 space-y-5">
  <div class="text-xs font-mono text-red-400 uppercase tracking-wider">Failure Mode &middot; Premature Implementation</div>
  <div class="text-3xl font-extrabold text-white">
    Racing straight from prompt to C before mapping subsystem boundaries
  </div>
  <div class="flex items-center gap-3 text-sm font-mono text-gray-300 pt-2">
    <span class="px-3 py-1.5 rounded bg-gray-900 border border-gray-700">Raw Prompt</span>
    <span class="text-red-400">&rarr;</span>
    <span class="px-3 py-1.5 rounded bg-gray-900 border border-gray-700">Grep Symbols</span>
    <span class="text-red-400">&rarr;</span>
    <span class="px-3 py-1.5 rounded bg-red-950/40 border border-red-500/40 text-red-200">Edit Nearby C Immediately</span>
  </div>
</div>

<!--
Watch for what Dave Rensin calls "Weeds Syndrome"—premature implementation, diving straight into writing C files before understanding any subsystem invariants.
-->

---
layout: cast-full
title: "Experiment A: Vanilla Vibe Coding (nuttxspace-vanilla)"
cast: /casts/vanilla-vibe.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
initialStopPoint: 2
showChapterBar: true
---

<!-- S026 -->
<!--
Here is the raw prompt from EXPERIMENT_RUNBOOK.md going straight into an unprimed thread. Look at the effect right away: it asks one quick question about pins and LED polarity, I answer "pick any free pins", and that is the entire design conversation over. Now at fifty-two times speed—compressing nearly twenty-six minutes of single-thread grep-and-code—it pulls sixty-six files out of the repo without reading a single nuttx/ or apps/ README.md contract. At step 180 olddefconfig quietly drops CONFIG_ADC=y, it patches Kconfig, and twenty-six minutes later it hands us a green nuttx.uf2 build.
-->

---
layout: default
---

<!-- S027 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Vanilla Telemetry Snapshot</h2>
</div>

<div class="grid grid-cols-4 gap-4 mt-8">
  <div class="p-5 rounded-xl bg-gray-900/80 border border-gray-800">
    <div class="text-xs font-mono text-gray-400 uppercase">Raw Duration</div>
    <div class="text-3xl font-black font-mono text-white mt-2">46m 40s</div>
    <div class="text-xs text-gray-400 mt-1">Single recorded run</div>
  </div>

  <div class="p-5 rounded-xl bg-gray-900/80 border border-gray-800">
    <div class="text-xs font-mono text-gray-400 uppercase">Agent Steps</div>
    <div class="text-3xl font-black font-mono text-white mt-2">225</div>
    <div class="text-xs text-gray-400 mt-1">66 <code class="text-gray-300">read_file</code> calls</div>
  </div>

  <div class="p-5 rounded-xl bg-gray-900/80 border border-gray-800">
    <div class="text-xs font-mono text-gray-400 uppercase">Subagents</div>
    <div class="text-3xl font-black font-mono text-white mt-2">0</div>
    <div class="text-xs text-gray-400 mt-1">No independent audit</div>
  </div>

  <div class="p-5 rounded-xl bg-red-950/25 border border-red-500/45">
    <div class="text-xs font-mono text-red-400 uppercase">Contract Reads</div>
    <div class="text-3xl font-black font-mono text-red-300 mt-2">0</div>
    <div class="text-xs text-red-200 mt-1"><code class="text-red-300">nuttx/</code> + <code class="text-red-300">apps/</code> READMEs</div>
  </div>
</div>

<!-- S028 -->
<div v-click class="mt-8 p-5 rounded-xl bg-gray-900/70 border border-amber-500/35 flex items-center justify-between">
  <div>
    <div class="text-sm font-bold text-white">The Vibe-Coding Illusion</div>
    <div class="text-xs text-gray-300 mt-1">66 blind file greps &middot; 1 quick question &middot; 1 green <code class="text-emerald-300">.uf2</code> &mdash; so what broke beneath the surface?</div>
  </div>
  <span class="px-3 py-1.5 rounded-lg bg-amber-950/40 border border-amber-500/40 font-mono text-xs text-amber-300">
    docs/vanilla/CODE_REVIEW.md
  </span>
</div>

<!--
Forty-six minutes forty seconds raw, two hundred and twenty-five steps, zero subagents, and zero architectural contract reads across nuttx/ or apps/. On paper that speed looks brilliant. Let's open docs/vanilla/CODE_REVIEW.md and inspect what sixty-six blind greps missed underneath.
-->

---
layout: default
---

<!-- S029 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Supporting Trap #1: Silent Kconfig Drop</h2>
</div>

<div class="p-5 rounded-xl bg-gray-900/80 border border-gray-800 space-y-2 mt-4">
  <div class="flex items-center justify-between text-xs font-mono">
    <span class="text-cyan-300">Trace Step 180 &middot; docs/vanilla/CODE_REVIEW.md</span>
    <span class="text-gray-400">board/configs/bargraph/defconfig#L15-L16</span>
  </div>
  <p class="text-base text-gray-200 m-0">
    Vanilla enables <code class="text-cyan-300">CONFIG_ADC=y</code> in <code class="text-gray-200">defconfig</code>, assuming the ADC driver will compile.
  </p>
</div>

<!-- S030 -->
<div v-click class="mt-5 p-5 rounded-xl bg-red-950/25 border border-red-500/45 space-y-3">
  <div class="text-xs font-mono text-red-400 uppercase tracking-wider">Silent olddefconfig Drop + Pin Collision</div>
  <ul class="text-sm text-gray-200 space-y-2 list-disc pl-5 m-0">
    <li>Missing <code class="text-amber-300">CONFIG_ANALOG=y</code> causes <code class="text-red-300">olddefconfig</code> to quietly strip <code class="text-red-300">CONFIG_ADC=y</code> out of <code class="text-gray-200">.config</code>.</li>
    <li>Meanwhile, <code class="text-red-300">CONFIG_DEV_GPIO=y</code> remained enabled, colliding on all six LED pins (<code class="text-amber-300">GPIO 5, 6, 7, 0, 1, 2</code>).</li>
  </ul>
</div>

<!--
First warm-up trap hits NuttX's Kconfig tree: Vanilla dropped CONFIG_ADC=y into defconfig assuming the ADC driver would compile. Without CONFIG_ANALOG=y, olddefconfig quietly stripped CONFIG_ADC=y out—and Vanilla still left CONFIG_DEV_GPIO=y colliding on all six LED pins.
-->

---
layout: default
---

<!-- S031 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">Supporting Trap #2: <code class="text-amber-300">my_reset()</code> in Silicon Init</h2>
</div>

<div class="text-xs font-mono text-cyan-300 mb-3">
  nuttx/arch/arm/src/rp23xx/rp23xx_adc.c#L466-L483 (my_reset())
</div>

<div class="grid grid-cols-2 gap-4 mt-3">

<!-- S032 -->
  <div v-click class="p-4 rounded-xl bg-gray-900/70 border border-gray-800 space-y-2">
    <div class="text-xs font-mono text-amber-300">1. Clobbers all 4 ADC pins (GPIO 26&ndash;29)</div>

```c {3-6}
static void my_reset(struct adc_dev_s *dev)
{
  for (int i = 0; i < ADC_CHANNEL_COUNT; ++i)
    {
      a_gpio = g_gpio_map[i]; /* { 26, 27, 28, 29, -1 } */
      if (a_gpio >= 0)
        {
          rp23xx_gpio_set_function(a_gpio, RP23XX_GPIO_FUNC_NULL);
```

  </div>

<!-- S033 -->
  <div v-click class="p-4 rounded-xl bg-gray-900/70 border border-red-500/35 space-y-2">
    <div class="text-xs font-mono text-red-300">2. Sets RP23XX_PADS_BANK0_GPIO_IE (IE=1)</div>

```c {4-6}
      if (a_gpio >= 0)
        {
          rp23xx_gpio_setdir(a_gpio, false);
          rp23xx_gpio_set_function(a_gpio, RP23XX_GPIO_FUNC_NULL);
          rp23xx_gpio_set_pulls(a_gpio, false, false);
        } /* set_function() sets RP23XX_PADS_BANK0_GPIO_IE (IE=1) */
```

  </div>
</div>

<!--
Trap two hides during kernel peripheral bringup in nuttx/arch/arm/src/rp23xx/rp23xx_adc.c—a quiet initialisation helper called my_reset() with global pin side effects. Even when only Channel 0 is configured, my_reset() unconditionally loops across all four pins, GPIO 26 through 29, wiping muxing. Worse, calling rp23xx_gpio_set_function() sets RP23XX_PADS_BANK0_GPIO_IE, quietly switching the digital input buffer back on right across GPIO 26 through 29.
-->

---
layout: default
---

<!-- S034 -->
<div class="flex items-center gap-3 mb-3">
  <h2 class="text-2xl font-bold m-0">Silicon Erratum <code class="text-red-300">RP2350-E9</code></h2>
</div>

<div class="p-3.5 rounded-xl bg-red-950/20 border border-red-500/40 text-sm text-gray-200 mb-3">
  Leaving <code class="text-red-300">IE=1</code> injects digital Schmitt input-buffer leakage current into <code class="text-amber-300">GPIO 26</code> (<code class="text-gray-200">D0/A0</code>), distorting mid-scale potentiometer readings (<code class="text-gray-200">~1.65 V</code>).
</div>

<!-- S035 -->
<div v-click class="space-y-2">
  <div class="text-xs font-mono text-gray-400">
    Vanilla (<code class="text-red-300">rp23xx_bringup.c#L42-L54</code>) vs. EGM (<code class="text-emerald-300">rp23xx_bringup.c#L67-L72</code>)
  </div>

```c {3-8}
  ret = rp23xx_common_bringup(); /* Vanilla omits pad isolation */
  /* EGM rp23xx_bringup.c#L67-L72: mitigate RP2350-E9 on GPIO26 */
  modreg32(RP23XX_PADS_BANK0_GPIO_OD,
           RP23XX_PADS_BANK0_GPIO_IE |
           RP23XX_PADS_BANK0_GPIO_PUE |
           RP23XX_PADS_BANK0_GPIO_PDE |
           RP23XX_PADS_BANK0_GPIO_ISO,
           RP23XX_PADS_BANK0_GPIO(BARGRAPH_ADC_GPIO));
```

</div>

<!-- S036 -->
<div v-click class="mt-3 p-3 rounded-lg bg-gray-900 border border-gray-700 text-xs font-mono text-center text-gray-200">
  <span class="text-red-300">Grep &rarr; symbol names</span>
  <span class="mx-3 text-gray-500">|</span>
  <span class="text-emerald-300">Contracts &rarr; cross-layer silicon invariants</span>
</div>

<!--
On RP2350 silicon, leaving IE=1 on an analogue input triggers Erratum RP2350-E9, leaking digital buffer current into mid-scale potentiometer readings. Vanilla never clears IE, pulls, or isolation after rp23xx_common_bringup(), shipping RP2350-E9 analogue pad leakage straight onto the user's bench. Grep finds you C symbol names; it never warns you about cross-layer silicon errata. Now meet our primary spine trap.
-->

---
layout: default
---

<!-- S037 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">Primary Spine: The <code class="text-red-300">500 kHz</code> <code class="text-cyan-300">START_ONCE</code> Storm</h2>
</div>

<div class="text-xs font-mono text-cyan-300 mb-3">
  nuttx/arch/arm/src/rp23xx/rp23xx_adc.c
</div>

<div class="grid grid-cols-2 gap-4 mt-3">

<!-- S038 -->
  <div v-click class="p-4 rounded-xl bg-gray-900/70 border border-gray-800 space-y-2">
    <div class="text-xs font-mono text-amber-300">interrupt_handler() (#L232-L237)</div>

```c {3}
  /* Start next channel read */

  get_next_channel();

  return OK;
}
```

  </div>

<!-- S039 -->
  <div v-click class="p-4 rounded-xl bg-gray-900/70 border border-red-500/35 space-y-2">
    <div class="text-xs font-mono text-red-300">get_next_channel() (#L302-L308)</div>

```c {3-5}
              /* Start the conversion */

              value += RP23XX_ADC_CS_START_ONCE;

              putreg32(value, RP23XX_ADC_CS);

              return;
```

  </div>
</div>

<!--
This next quirk inside rp23xx_adc.c threads through both experiments and exposes why both vibe coding and unreviewed specs break. Inside the ADC interrupt handler, every single time a sample finishes, rp23xx_adc.c unconditionally calls get_next_channel() before returning. And get_next_channel() immediately pokes RP23XX_ADC_CS_START_ONCE right back into the control register, chaining another conversion as fast as silicon allows.
-->

---
layout: center
class: text-center
---

<!-- S040 -->
<div class="space-y-6">
  <div class="text-xs font-mono text-red-400 uppercase tracking-widest">Whenever /dev/adc0 Stays Open</div>
  <div class="text-7xl font-black font-mono text-red-400">
    500,000 IRQ/s
  </div>
  <div class="text-lg text-gray-200 font-mono">
    96 ADC clock cycles @ 48 MHz = <strong class="text-white">2 &micro;s / sample</strong> interrupt flood
  </div>
</div>

<!--
At forty-eight megahertz and ninety-six cycles per sample, leaving /dev/adc0 open unleashes five hundred thousand interrupts per second onto the CPU.
-->

---
layout: default
---

<!-- S041 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">Vanilla&rsquo;s Workaround: <code class="text-cyan-300">bargraph_read_adc_once()</code></h2>
</div>

<div class="text-xs font-mono text-gray-400 mb-3">
  docs/vanilla/xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c#L80-L103
</div>

<!-- S042 -->
<div v-click class="space-y-2">
  <div class="text-xs font-mono text-amber-300">open("/dev/adc0") &rarr; blocking read() &rarr; close(fd) at 40 Hz (25 ms ticks)</div>

```c {1,6,7}
  fd = open(CONFIG_BARGRAPH_ADC_DEVPATH, O_RDONLY);
  if (fd < 0)
    {
      return -errno;
    }
  nbytes = read(fd, &msg, sizeof(msg));
  close(fd);
  if (nbytes != (ssize_t)sizeof(msg))
```

</div>

<!-- S043 -->
<div v-click class="mt-4 p-4 rounded-xl bg-emerald-950/25 border border-emerald-500/40 flex items-center justify-between">
  <div>
    <div class="text-sm font-bold text-emerald-300">Why That Looked Clever on the Desk</div>
    <div class="text-xs text-gray-200 mt-0.5">Closing <code class="text-emerald-300">/dev/adc0</code> stops conversions between ticks, dodging the 500 kHz storm.</div>
  </div>
  <span class="px-3 py-1 rounded bg-emerald-500/20 border border-emerald-400/40 text-xs font-mono text-emerald-300">
    CPU load drops + LEDs track
  </span>
</div>

<!--
Vanilla noticed the ADC kept hammering interrupts continuously, so it improvised a neat-looking user-space workaround in bargraph_read_adc_once(). Forty times every second—every twenty-five milliseconds—Vanilla opens /dev/adc0, reads one blocking message, and immediately calls close(fd). On the desk, CPU load drops away and the bar graph tracks cleanly. Looks like a sharp engineering workaround, doesn't it?
-->

---
layout: default
---

<!-- S044 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">Zoom Into Upper-Half Teardown</h2>
</div>

<div class="text-xs font-mono text-cyan-300 mb-3">
  close(fd) &rarr; adc_close() (nuttx/drivers/analog/adc.c#L181-L214) &rarr; remove_device()
</div>

<div class="grid grid-cols-2 gap-4 mt-3">

<!-- S045 -->
  <div v-click class="p-4 rounded-xl bg-gray-900/70 border border-gray-800 space-y-2">
    <div class="text-xs font-mono text-amber-300">adc_close() &mdash; nxmutex only, zero IRQ mask</div>

```c {1,6,7}
  ret = nxmutex_lock(&dev->ad_lock);
  if (ret >= 0)
    {
      ...
      dev->ad_ocount = 0;
      dev->ad_ops->ao_shutdown(dev); /* No enter_critical_section()! */
      nxmutex_unlock(&dev->ad_lock);
    }
```

  </div>

<!-- S046 -->
  <div v-click class="p-4 rounded-xl bg-gray-900/70 border border-red-500/35 space-y-2">
    <div class="text-xs font-mono text-red-300">remove_device() (#L395-L407) &mdash; 7-line race window</div>

```c {3,7}
  if (dev == g_first_device)
    {
      g_first_device = PRIV(g_first_device)->next_device;
      if (g_first_device != NULL)
        PRIV(g_first_device)->prior_device = NULL;

      g_active_count -= 1; /* 7 lines after g_first_device cleared! */
    }
```

  </div>
</div>

<!--
Until you trace close(fd) down into NuttX's upper-half driver in nuttx/drivers/analog/adc.c and see what adc_close() actually locks. adc_close() holds nxmutex_lock(), not enter_critical_section(). A mutex blocks competing threads—it does nothing to stop a two-microsecond hardware interrupt. Inside remove_device(), g_first_device becomes NULL seven lines before g_active_count drops. If the ADC interrupt lands inside that tiny window...
-->

---
layout: default
---

<!-- S047 -->
<div class="flex items-center gap-3 mb-4">
  <h2 class="text-2xl font-bold m-0">The Teardown Race &amp; Read Safety Gaps</h2>
</div>

<div class="p-5 rounded-xl bg-gray-900/80 border border-red-500/40 space-y-3">
  <div class="text-xs font-mono text-red-400 uppercase">Two-Lane ISR Race Timeline (rp23xx_adc.c)</div>
  <div class="grid grid-cols-3 gap-3 text-xs font-mono items-center text-center">
    <div class="p-3 rounded-lg bg-gray-950 border border-gray-700 text-gray-200">
      <div class="text-amber-300 font-bold">#L399</div>
      g_first_device = NULL
    </div>
    <div class="p-3 rounded-lg bg-red-950/40 border border-red-500 text-red-200">
      <div class="text-red-400 font-bold">RP23XX_ADC_IRQ_FIFO fires!</div>
      get_next_channel() &rarr; #L314 ASSERT(next != g_current_channel)
    </div>
    <div class="p-3 rounded-lg bg-gray-950 border border-gray-700 text-gray-400">
      <div class="text-gray-300 font-bold">#L406 (Too Late)</div>
      g_active_count -= 1
    </div>
  </div>
</div>

<!-- S048 -->
<div v-click class="mt-4 p-4 rounded-xl bg-red-950/30 border border-red-500/50 text-center">
  <span class="text-base font-bold font-mono text-red-200">
    40 Hz <code class="text-red-300">close(fd)</code> = 40 kernel-panic race rolls / second
  </span>
</div>

<!-- S049 -->
<div v-click class="mt-4 p-4 rounded-xl bg-amber-950/20 border border-amber-500/35 text-sm text-gray-200">
  <strong class="text-amber-300">Read Safety Nits:</strong> Blocking <code class="text-amber-200">read()</code> has zero <code class="text-gray-200">O_NONBLOCK</code> / <code class="text-gray-200">poll()</code> timeout and <code class="text-gray-200">bargraph_daemon()</code> silently swallows read errors (<code class="text-xs font-mono text-gray-400">bargraph_main.c#L93, L191-L201</code>).
</div>

<!--
...the interrupt handler runs get_next_channel(), finds zero active devices, and crashes straight into ASSERT(next != g_current_channel) at line 314. By closing /dev/adc0 at forty hertz, Vanilla quietly turned a rare shutdown race into forty kernel-panic dice rolls every single second. On top of that, blocking read() has zero timeout and bargraph_daemon() silently swallows read failures instead of reporting or blinking faults.
-->

---
layout: default
---

<!-- S050 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Credit Where Due: What Vanilla Got Right</h2>
</div>

<div class="p-4 rounded-xl bg-gray-900/70 border border-gray-800 text-sm text-gray-300 mb-6">
  No cheap straw-manning &mdash; verified in <code class="text-cyan-300">docs/vanilla/CODE_REVIEW.md</code> &amp; <code class="text-cyan-300">docs/reports/vanilla-vs-egm-comparison.md</code>
</div>

<!-- S051 -->
<div v-click class="grid grid-cols-2 gap-6">
  <div class="p-6 rounded-xl bg-emerald-950/20 border border-emerald-500/40 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">Genuine Win #1</div>
    <div class="text-lg font-bold text-white"><code class="text-emerald-300">D3&ndash;D8</code> LED Pin Assignment</div>
    <p class="text-sm text-gray-300 m-0">
      Safely dodged <code class="text-emerald-300">GPIO 26&ndash;29</code> clobbering inside <code class="text-gray-200">my_reset()</code> without needing runtime re-muxing.
    </p>
  </div>

  <div class="p-6 rounded-xl bg-emerald-950/20 border border-emerald-500/40 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">Genuine Win #2</div>
    <div class="text-lg font-bold text-white">Shared <code class="text-emerald-300">src/bargraph_logic.c</code></div>
    <p class="text-sm text-gray-300 m-0">
      Clean single-source C split tested directly on host&mdash;cleaner than EGM&rsquo;s first Phase 5 Makefile pass.
    </p>
  </div>
</div>

<!--
Now, to be fair—no cheap straw-manning here—let's credit what Vanilla vibe coding genuinely got right on the first try. It parked the LEDs on D3 through D8, dodging my_reset(), and wrote a cleaner shared bargraph_logic.c test split than EGM's first pass.
-->

---
layout: default
---

<!-- S052 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Concurrency, Lifecycle &amp; High-Severity Fails</h2>
</div>

<div class="space-y-4 mt-4">
  <div class="text-xs font-mono text-amber-300 uppercase">Lifecycle &amp; Concurrency Gaps (vanilla-vs-egm-comparison.md#L70-L71)</div>
  <div class="grid grid-cols-3 gap-4">
    <div class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/35 text-sm text-gray-200">
      <strong class="text-amber-300 block mb-1">Dual Autostart</strong>
      Started from both <code class="text-gray-200">rp23xx_bringup</code> and <code class="text-gray-200">rcS</code>
    </div>
    <div class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/35 text-sm text-gray-200">
      <strong class="text-amber-300 block mb-1">Data Races</strong>
      Unguarded <code class="text-gray-200">g_bargraph_state</code> shared state
    </div>
    <div class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/35 text-sm text-gray-200">
      <strong class="text-amber-300 block mb-1">Stop Glitch</strong>
      LEDs left frozen on at <code class="text-gray-200">bargraph stop</code>
    </div>
  </div>
</div>

<!-- S053 -->
<div v-click class="mt-6 space-y-3">
  <div class="text-xs font-mono text-red-400 uppercase">Two High-Severity Hardware &amp; Kernel Fails</div>
  <div class="grid grid-cols-2 gap-4">
    <div class="p-4 rounded-xl bg-red-950/30 border border-red-500/50 flex items-center gap-3">
      <span class="px-2 py-1 rounded bg-red-500/20 text-xs font-mono font-bold text-red-300">FAIL #3</span>
      <span class="text-sm font-semibold text-white"><code class="text-red-300">RP2350-E9</code> IE=1 Analogue Leakage</span>
    </div>
    <div class="p-4 rounded-xl bg-red-950/30 border border-red-500/50 flex items-center gap-3">
      <span class="px-2 py-1 rounded bg-red-500/20 text-xs font-mono font-bold text-red-300">FAIL #6</span>
      <span class="text-sm font-semibold text-white"><code class="text-red-300">40 Hz</code> ISR Teardown Race</span>
    </div>
  </div>
</div>

<!--
Still, Vanilla also shipped dual autostart (rp23xx_bringup plus rcS), unlocked global state races, and left LEDs frozen bright on stop. Combined with RP2350-E9 pad leakage and the forty-hertz close(fd) race, two high-severity defects sit silently inside a blinking demo.
-->

---
layout: default
---

<!-- S054 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Vanilla Audit Scorecard</h2>
</div>

<div class="mt-6 p-8 rounded-2xl bg-amber-950/15 border border-amber-500/40 flex items-center justify-between">
  <div>
    <div class="text-xs font-mono text-amber-400 uppercase tracking-wider">Primary 10-Point Trap Scorecard</div>
    <div class="text-2xl font-bold text-white mt-1"><code class="text-amber-300">nuttxspace-vanilla</code></div>
    <div class="text-xs font-mono text-gray-400 mt-2">docs/reports/vanilla-vs-egm-comparison.md#L22</div>
  </div>
  <div class="text-6xl font-black font-mono text-amber-300">
    5 / 10
  </div>
</div>

<!-- S055 -->
<div v-click class="mt-6 p-6 rounded-2xl bg-red-950/20 border border-red-500/45 flex items-center justify-between">
  <div>
    <div class="text-xs font-mono text-red-400 uppercase tracking-wider">Strict Hostile Goldfish Post-Audit</div>
    <div class="text-sm text-gray-200 mt-1">Downgrades missing <code class="text-red-300">Make.defs</code> <code class="text-gray-200">distclean</code> filter &amp; partial defects</div>
  </div>
  <div class="text-2xl font-black font-mono text-red-300">
    4 Pass / 4 Partial / 2 Fail
  </div>
</div>

<!--
On our ten-point hardware and RTOS trap scorecard, Vanilla scores five out of ten: impressive surface velocity, unsafe underneath. Under a strict hostile audit, that drops to four passes, four partials, and two hard fails across ten invariants.
-->

---
layout: default
---

<!-- S056 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Rensin&rsquo;s Diagnosis: Context Pollution &amp; Self-Validation</h2>
</div>

<div class="p-6 rounded-2xl bg-gray-900/80 border border-gray-800 space-y-5 mt-4">
  <div class="flex items-center justify-between text-xs font-mono text-red-400 uppercase">
    <span>Single-Chat Failure Mode</span>
    <span>Hallucination Cascade &amp; Context Pollution</span>
  </div>
  <div class="grid grid-cols-4 gap-3 text-center font-mono text-sm">
    <div class="p-4 rounded-xl bg-gray-950 border border-gray-700 text-white">1. Prompt</div>
    <div class="p-4 rounded-xl bg-amber-950/30 border border-amber-500/40 text-amber-200">2. Guess</div>
    <div class="p-4 rounded-xl bg-red-950/30 border border-red-500/40 text-red-200">3. Write C</div>
    <div class="p-4 rounded-xl bg-red-950/40 border border-red-500 text-red-300 font-bold">4. Self-Approve</div>
  </div>
</div>

<!-- S057 -->
<div v-click class="mt-8 p-6 rounded-xl bg-cyan-950/25 border border-cyan-500/40 text-center">
  <div class="text-xs font-mono text-cyan-300 uppercase tracking-wider">Next &middot; Act III</div>
  <div class="text-xl font-bold text-white mt-1">
    Experiment B: <code class="text-cyan-300">nuttxspace-egm</code> &mdash; Rolling Architectural Contracts Before Writing a Line of C
  </div>
</div>

<!--
This is exactly the trap Dave Rensin diagnosed: context pollution and self-validation, where one chat session guesses, writes C, and approves itself. Same hardware, same prompt—now let's roll contracts for the Elephant before writing a single line of C.
-->
