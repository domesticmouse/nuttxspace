---
layout: center
class: text-center
---

<!-- S133 -->
<div class="space-y-4 max-w-3xl mx-auto">
  <div class="text-xs font-mono text-emerald-400 uppercase tracking-widest">Layer 2 &middot; The /teach-code Codelab</div>
  <h1 class="text-4xl font-extrabold text-white leading-tight">
    Rolling Docs for the Human
  </h1>
  <p class="text-xl font-mono text-emerald-300">
    Paying Down Comprehension Debt
  </p>
</div>

<!--
Part 2 of "Roll Your Own Docs" flips the camera straight away from the coding agents and back onto us engineers.
-->

---
layout: default
---

<!-- S134 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Rensin&rsquo;s Third Role: &ldquo;Humans Own the Outcomes&rdquo;</h2>
  <span class="text-xs font-mono text-gray-400">egm/docs/index.md#L61-L75</span>
</div>

<div class="grid grid-cols-3 gap-4 mt-4 text-center">
  <div class="p-4 rounded-xl bg-gray-900/70 border border-cyan-500/30">
    <div class="text-xs font-mono text-cyan-400 uppercase">Role 1</div>
    <div class="text-lg font-bold text-white mt-1">The Elephant</div>
    <div class="text-xs text-gray-400 mt-1">Loads &amp; drafts architecture</div>
  </div>
  <div class="p-4 rounded-xl bg-gray-900/70 border border-amber-500/30">
    <div class="text-xs font-mono text-amber-400 uppercase">Role 2</div>
    <div class="text-lg font-bold text-white mt-1">The Goldfish</div>
    <div class="text-xs text-gray-400 mt-1">Cold stateless verification</div>
  </div>
  <div class="p-4 rounded-xl bg-emerald-950/25 border-2 border-emerald-400/60">
    <div class="text-xs font-mono text-emerald-300 uppercase">Role 3</div>
    <div class="text-lg font-bold text-white mt-1">The Humans</div>
    <div class="text-xs text-emerald-200 mt-1 font-semibold">Own the outcomes</div>
  </div>
</div>

<!-- S135 -->
<div v-click class="mt-6 p-6 rounded-2xl bg-gray-900/80 border border-red-500/40">
  <div class="text-xs font-mono text-red-400 uppercase mb-2">Verbatim Warning (egm/docs/index.md#L63-L65)</div>
  <blockquote class="text-xl font-semibold text-white leading-snug m-0">
    &ldquo;&lsquo;Sorry! My AI tool just pushed broken code to production...&rsquo; isn&rsquo;t an excuse &mdash; it was you that pushed it.&rdquo;
  </blockquote>
</div>

<!--
Dave Rensin pairs the Elephant and Goldfish with a blunt third rule for the person holding the keyboard: humans own the outcomes. As Dave writes, "Sorry, my AI tool just pushed broken code to production" will not fly—it was still you who pushed it.
-->

---
layout: default
---

<!-- S136 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The Comprehension Gap</h2>
</div>

<div class="grid grid-cols-2 gap-6 mt-6">
  <div class="p-6 rounded-xl bg-emerald-950/20 border border-emerald-500/40 space-y-2">
    <div class="text-xs font-mono text-emerald-400 uppercase">Agent &amp; Repo Docs</div>
    <div class="text-4xl font-black font-mono text-emerald-300">10 / 10</div>
    <div class="text-sm text-gray-200">NuttX <code class="text-emerald-200">rp23xx_adc.c</code>, <code class="text-emerald-200">RP2350-E9</code>, &amp; NVIC IRQ gating mastery</div>
  </div>

  <div class="p-6 rounded-xl bg-gray-900/80 border border-amber-500/40 space-y-2">
    <div class="text-xs font-mono text-amber-400 uppercase">Human Mental Model</div>
    <div class="text-4xl font-black font-mono text-amber-300">?</div>
    <div class="text-sm text-gray-200">Can you debug this bench circuit six months from now?</div>
  </div>
</div>

<!-- S137 -->
<div v-click class="mt-6 p-5 rounded-xl bg-red-950/25 border border-red-500/45 text-center">
  <div class="text-xs font-mono text-red-400 uppercase">The Human Confession</div>
  <div class="text-lg font-bold text-white mt-1">
    Comprehension Debt = shipping <code class="text-red-300">872</code> lines of <code class="text-red-300">bargraph_main.c</code> you didn&rsquo;t write by hand
  </div>
</div>

<!--
Right now, our repo contracts and Goldfish reports understand rp23xx_adc.c, RP2350-E9 leakage, and NVIC gating cold—scoring ten out of ten. Yet I did not write those eight hundred lines of C myself. Come back in six months and that becomes silent comprehension debt.
-->

---
layout: default
---

<!-- S138 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">Foundational Lineage: Matt Pocock&rsquo;s <code class="text-emerald-300">/teach</code></h2>
  <span class="text-xs font-mono text-gray-400">.agents/skills/teach/SKILL.md#L12-L53, L110</span>
</div>

<div class="p-4 rounded-xl bg-emerald-950/15 border border-emerald-500/35 text-sm text-gray-200">
  Open-source interactive pedagogy skill grounded in cognitive science instead of walls of generic AI prose.
</div>

<!-- S139 -->
<div v-click class="grid grid-cols-3 gap-4 mt-4 text-xs">
  <div class="p-4 rounded-xl bg-gray-900/80 border border-cyan-500/30 space-y-1">
    <div class="font-mono font-bold text-cyan-300">Zone of Proximal Development</div>
    <p class="text-gray-300 m-0"><code class="text-cyan-200">learning-records/</code> diagnose the learner&rsquo;s exact starting edge.</p>
  </div>
  <div class="p-4 rounded-xl bg-gray-900/80 border border-emerald-500/30 space-y-1">
    <div class="font-mono font-bold text-emerald-300">Edward Tufte Ergonomics</div>
    <p class="text-gray-300 m-0">Wide prose column with right-margin sidenotes keeping context adjacent.</p>
  </div>
  <div class="p-4 rounded-xl bg-gray-900/80 border border-purple-500/30 space-y-1">
    <div class="font-mono font-bold text-purple-300">Fluency vs. Storage Strength</div>
    <p class="text-gray-300 m-0">Equal-word-count retrieval quizzes beat passive skimming.</p>
  </div>
</div>

<!-- S140 -->
<div v-click class="mt-4 p-4 rounded-xl bg-red-950/20 border border-red-500/40 text-sm text-gray-200 text-center">
  <strong class="text-red-300">Why Static Prose Breaks for Firmware:</strong>
  Hand-pasted tutorial snippets &harr; Compiling C99 code = <strong class="text-white">Silent Divergence</strong>
</div>

<!--
To pay down that debt deliberately, I started from Matt Pocock's brilliant open-source /teach skill instead of inventing pedagogy from scratch. /teach builds durable storage strength over surface fluency using Zone of Proximal Development records, Edward Tufte layout, and active retrieval quizzes. One catch with teaching embedded C, though: pasted code blocks inside static HTML tutorials quickly drift away from code that actually compiles.
-->

---
layout: default
---

<!-- S141 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">Evolving <code class="text-gray-300">/teach</code> &rarr; <code class="text-emerald-300">/teach-code</code> via <code class="text-purple-300">/agy-customizations</code></h2>
  <span class="text-xs font-mono text-gray-400">SKILL.md#L17-L32, L56-L110</span>
</div>

<div class="p-4 rounded-xl bg-purple-950/15 border border-purple-500/35 text-xs font-mono text-gray-200 flex items-center justify-between">
  <span>.agents/skills/teach/SKILL.md</span>
  <span class="text-purple-300">&rarr; modified via /agy-customizations &rarr;</span>
  <span class="text-emerald-300">.agents/skills/teach-code/SKILL.md</span>
</div>

<!-- S142 -->
<div v-click class="mt-5 p-5 rounded-xl bg-emerald-950/15 border border-emerald-500/35 space-y-2">
  <div class="text-xs font-mono text-emerald-400 uppercase">Kept Intact &middot; Pedagogical Structure</div>
  <div class="flex flex-wrap gap-3 text-xs font-mono text-gray-200">
    <span class="px-3 py-1 rounded bg-gray-900 border border-emerald-500/30">&check; MISSION.md</span>
    <span class="px-3 py-1 rounded bg-gray-900 border border-emerald-500/30">&check; RESOURCES.md</span>
    <span class="px-3 py-1 rounded bg-gray-900 border border-emerald-500/30">&check; learning-records/*.md (ZPD ADRs)</span>
  </div>
</div>

<!-- S143 -->
<div v-click class="mt-4 p-5 rounded-xl bg-emerald-950/15 border border-emerald-500/35 space-y-2">
  <div class="text-xs font-mono text-emerald-400 uppercase">Kept Intact &middot; Tufte &amp; Retrieval Ergonomics</div>
  <div class="flex flex-wrap gap-3 text-xs font-mono text-gray-200">
    <span class="px-3 py-1 rounded bg-gray-900 border border-emerald-500/30">&check; Printable reference/*.md sheets</span>
    <span class="px-3 py-1 rounded bg-gray-900 border border-emerald-500/30">&check; Tufte right-margin sidenotes</span>
    <span class="px-3 py-1 rounded bg-gray-900 border border-emerald-500/30">&check; Equal-word-count :::quiz rules</span>
  </div>
</div>

<!--
So over repeated real projects I gently shaped Matt's /teach skill into /teach-code using Antigravity's built-in /agy-customizations skill. We kept the soul of /teach untouched: MISSION.md, verified RESOURCES.md, and ADR-style learning records tracking the learner's starting edge. Same printable reference sheets, same distraction-free Tufte margin notes, and Matt's rule forcing every quiz option to share identical word counts.
-->

---
layout: default
---

<!-- S144 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">What <code class="text-emerald-300">/teach-code</code> Adds: 3 Compiler-Grade Gates</h2>
</div>

<div class="space-y-4 mt-4">

<!-- S145 -->
  <div v-click class="p-4 rounded-xl bg-cyan-950/20 border border-cyan-500/40">
    <div class="flex items-center justify-between">
      <span class="font-mono font-bold text-cyan-300">Gate #1 &middot; build.py --check</span>
      <span class="text-xs font-mono text-gray-400">docs/egm/tutorial/build.py</span>
    </div>
    <p class="text-sm text-gray-200 mt-1 m-0">Validates Markdown directives, internal <code class="text-cyan-300">.md</code> links, Mermaid syntax, Prism blocks, and quiz option word counts.</p>
  </div>

<!-- S146 -->
  <div v-click class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/40">
    <div class="flex items-center justify-between">
      <span class="font-mono font-bold text-emerald-300">Gate #2 &middot; Runnable C99 Apps (0001&ndash;0006)</span>
      <span class="text-xs font-mono text-gray-400">docs/egm/tutorial/apps/0001..0006/</span>
    </div>
    <p class="text-sm text-gray-200 mt-1 m-0">Every lesson compiles a companion C99 project under <code class="text-emerald-300">-std=c99 -Wall -Wextra -Werror -pedantic</code> + <code class="text-emerald-300">make test</code>.</p>
  </div>

<!-- S147 -->
  <div v-click class="p-4 rounded-xl bg-purple-950/20 border border-purple-500/40">
    <div class="flex items-center justify-between">
      <span class="font-mono font-bold text-purple-300">Gate #3 &middot; sync_listing.py --check</span>
      <span class="text-xs font-mono text-gray-400">.agents/skills/teach-code/scripts/sync_listing.py</span>
    </div>
    <p class="text-sm text-gray-200 mt-1 m-0">Guarantees byte-for-byte parity between lesson code listings and compiling <code class="text-purple-300">src/main.c</code> files.</p>
  </div>

</div>

<!--
What /teach-code adds—inspired by pragmatic engineering hygiene—is three deterministic compiler-like verification gates wrapped around every lesson. Gate one: lessons are authored in clean Markdown and compiled by build.py --check, checking links, quiz lengths, and HTML structure. Gate two: every lesson backs its prose with a runnable C99 project compiled with -Wall -Wextra -Werror -pedantic and make test. Gate three: sync_listing.py --check guarantees every full code listing inside a lesson matches its compiling .c file byte for byte.
-->

---
layout: center
class: text-center
---

<!-- S148 -->
<div class="space-y-6 max-w-3xl mx-auto">
  <div class="text-xs font-mono text-emerald-400 uppercase tracking-widest">Skill Evolution Formula</div>
  <div class="p-8 rounded-2xl bg-gray-900/80 border border-emerald-500/40 text-3xl font-extrabold text-white leading-snug">
    <span class="text-cyan-300">/teach</span> <span class="text-gray-400 font-normal text-xl">(Pedagogical Soul)</span>
    <span class="mx-2 text-gray-500">+</span>
    <span class="text-purple-300">Strict Compiler Gates</span>
    <span class="mx-2 text-gray-500">=</span>
    <span class="text-emerald-300">/teach-code</span>
  </div>
</div>

<!--
Keep the cognitive science, enforce compiler hygiene. Now let's run /teach-code against our verified firmware and walk through the course.
-->

---
layout: default
---

<!-- S149 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Phase 6 Prompt: Weaving the 6-Lesson Story Arc</h2>
</div>

<div class="mt-8 grid grid-cols-3 gap-4 items-center text-center">
  <div class="p-6 rounded-xl bg-cyan-950/20 border border-cyan-500/35 space-y-2">
    <div class="text-xs font-mono text-cyan-300 uppercase">94 EGM Artefacts</div>
    <div class="text-sm font-mono text-white">Contracts + Goldfish Reviews + Verified C</div>
  </div>

  <div class="p-6 rounded-xl bg-purple-950/25 border border-purple-500/40 space-y-2">
    <div class="text-xs font-mono text-purple-300 uppercase">Prompt</div>
    <div class="text-xl font-black font-mono text-white">/teach-code</div>
    <div class="text-xs text-gray-300">Story arc in <code class="text-purple-200">./tutorial/</code></div>
  </div>

  <div class="p-6 rounded-xl bg-emerald-950/20 border border-emerald-500/40 space-y-2">
    <div class="text-xs font-mono text-emerald-300 uppercase">Output Course</div>
    <div class="text-sm font-mono text-white"><code class="text-emerald-300">docs/egm/tutorial/</code></div>
    <div class="text-xs text-gray-300">6 lessons, 3 simulators, 4 bench sheets</div>
  </div>
</div>

<!--
In Phase 6, I point /teach-code at our generated EGM contracts, Goldfish reviews, and firmware to build a six-lesson story arc.
-->

---
layout: cast-full
title: "Phase 6: /teach-code Course Authoring (06-teach-code)"
cast: /casts/06-teach-code.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
initialStopPoint: 7
showChapterBar: true
---

<!-- S150 -->
<!--
Here is the prompt asking /teach-code to weave our entire hardware and RTOS story arc inside ./tutorial/. Watch the pipeline effect at seventy-nine times speed across thirty-nine raw minutes: it calculates the Zone of Proximal Development first, builds six progressive C99 companion apps from Lesson 0001 through 0006—running -Wall -Wextra -Werror -pedantic and fixing test failures on each chapter—writes six matching Tufte lessons, crafts three interactive JavaScript simulators plus four reference sheets, and runs sync_listing.py --check and build.py --check before we switch straight over to Chrome.
-->

---
layout: default
---

<!-- S151 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">Browser Tour Stop 1: Course Landing Page</h2>
  <a href="./tutorial/index.html" target="_blank" class="px-3 py-1 rounded-lg bg-emerald-950/40 border border-emerald-500/50 text-xs font-mono text-emerald-300 hover:underline">
    Open docs/egm/tutorial/index.html &nearr;
  </a>
</div>

<div class="p-5 rounded-xl bg-gray-900/80 border border-cyan-500/35 space-y-3 mt-4">
  <div class="flex items-center justify-between text-xs font-mono text-cyan-300">
    <span>Figure 0.1 &middot; 6-Lesson Mermaid Story Arc</span>
    <span>docs/egm/tutorial/index.md#L13-L20</span>
  </div>
  <div class="grid grid-cols-6 gap-2 text-center text-xs font-mono pt-1">
    <div class="p-2.5 rounded bg-gray-950 border border-gray-700 text-gray-200">0001<br/>Breadboard</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-700 text-gray-200">0002<br/>PWM Math</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-700 text-gray-200">0003<br/>Q4 EMA</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-700 text-gray-200">0004<br/>PWM Slice</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-700 text-gray-200">0005<br/>IRQ &amp; E9</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-700 text-gray-200">0006<br/>bargraphd</div>
  </div>
</div>

<!-- S152 -->
<div v-click class="mt-5 grid grid-cols-2 gap-4 text-sm">
  <div class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/35">
    <div v-pre class="text-xs font-mono text-emerald-300">{{widget:lesson-cards}}</div>
    <div class="font-bold text-white mt-1">6 Cumulative Chapters (<code class="text-emerald-200">0001&ndash;0006</code>)</div>
  </div>
  <div class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/35">
    <div v-pre class="text-xs font-mono text-amber-300">{{widget:reference-cards}}</div>
    <div class="font-bold text-white mt-1">4 Printable Bench Reference Sheets</div>
  </div>
</div>

<!--
Flip straight into Chrome on docs/egm/tutorial/index.html—compiled from index.md—opening on a six-lesson Mermaid story-arc map from breadboard to daemon. Underneath sit auto-generated cards for all six hands-on chapters and four printable bench sheets ready for lookup beside real hardware.
-->

---
layout: default
---

<!-- S153 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Course Mission: Anchoring Scope &amp; Constraints</h2>
  <span class="text-xs font-mono text-cyan-300">docs/egm/tutorial/MISSION.md#L3-L22</span>
</div>

<div class="grid grid-cols-2 gap-4 mt-4 text-sm">
  <div class="p-5 rounded-xl bg-gray-900/80 border border-cyan-500/30 space-y-1.5">
    <div class="text-xs font-mono text-cyan-400 uppercase">Why &amp; Success Looks Like</div>
    <p class="text-gray-200 m-0">Build authentic mental ownership of the <code class="text-cyan-300">xiao-rp2350-bargraph</code> firmware from Ohm&rsquo;s Law to POSIX daemons.</p>
  </div>
  <div class="p-5 rounded-xl bg-gray-900/80 border border-emerald-500/30 space-y-1.5">
    <div class="text-xs font-mono text-emerald-400 uppercase">Constraints</div>
    <p class="text-gray-200 m-0">Zero submodule edits, strict C99 host testing (<code class="text-emerald-300">-Werror -pedantic</code>), and safe 3.3 V current budgets.</p>
  </div>
  <div class="col-span-2 p-4 rounded-xl bg-gray-900/60 border border-gray-800 text-xs font-mono text-gray-300">
    <strong class="text-amber-300">Out of scope:</strong> Upstream NuttX PR patches, kernel-mode MMU builds, or external bench oscilloscopes.
  </div>
</div>

<!--
MISSION.md grounds every page: zero submodule edits, strict C99 host testing, and safe three-point-three-volt current budgets before touching silicon.
-->

---
layout: default
---

<!-- S154 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">ZPD Learning Record (ADR-Style)</h2>
  <span class="text-xs font-mono text-gray-400">learning-records/0001-initial-architecture-and-driver-traps.md#L1-L7</span>
</div>

<div class="p-5 rounded-xl bg-gray-900/80 border border-cyan-500/35 font-mono text-xs text-gray-200 space-y-2 mt-4">
  <div class="text-cyan-300 font-bold"># 0001 &mdash; Initial Architecture &amp; Driver Traps</div>
  <div><strong class="text-white">Known:</strong> Basic C pointers, terminal CLI, voltage divider intuition</div>
  <div><strong class="text-amber-300">Target Edge (ZPD):</strong> Fixed-point Q4 EMA, RP2350-E9 pad isolation, NVIC IRQ gating, NuttX upper/lower split</div>
  <div><strong class="text-emerald-300">Scaffold:</strong> One layer per chapter with host C99 verification before touching hardware</div>
</div>

<!-- S155 -->
<div v-click class="mt-6 p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/40 text-center">
  <div class="text-xs font-mono text-emerald-400 uppercase mb-1">Stop 1 Payoff &middot; Cumulative Ramp</div>
  <div class="font-mono text-sm text-white">
    0001 Physics &rarr; 0002 Scaling &rarr; 0003 Q4 EMA &rarr; 0004 PWM &rarr; 0005 IRQ/E9 &rarr; 0006 bargraphd
  </div>
</div>

<!--
Just like an engineering ADR, this learning record models the learner's Zone of Proximal Development so concepts stack one clean layer per chapter. Instead of dumping bargraph_main.c on page one, Lesson 0001 starts with Ohm's Law. Now let's jump to Stop 2 inside Lesson 0003.
-->

---
layout: default
---

<!-- S156 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">Browser Tour Stop 2: Lesson 0003 &amp; Tufte Sidenotes</h2>
  <span class="text-xs font-mono text-cyan-300">0003-q4-ema-filtering-and-adc-extraction.md#L18-L29</span>
</div>

<div class="grid grid-cols-12 gap-5 mt-5 items-stretch">
  <div class="col-span-8 p-6 rounded-xl bg-gray-900/80 border border-gray-800 space-y-3">
    <div class="text-xs font-mono text-emerald-400 uppercase">Section 1 &middot; Main Tufte Prose Column</div>
    <div class="text-lg font-bold text-white">19-Bit Left-Shift &amp; Stale First Sample (<code class="text-amber-300">msgs[0]</code>)</div>
    <p class="text-sm text-gray-300 m-0 leading-relaxed">
      Turns Pass 1&rsquo;s <code class="text-red-300">4x</code> Q4 EMA bug (<code class="text-amber-300">B-2</code>) and Pass 2&rsquo;s stale head sample (<code class="text-amber-300">CA-01</code>) directly into guided interactive exploration.
    </p>
  </div>

<!-- S157 -->
  <div v-click class="col-span-4 p-5 rounded-xl bg-amber-950/20 border border-amber-500/40 space-y-2">
    <div class="text-xs font-mono text-amber-300 uppercase">Tufte Sidenote #1 (#L23)</div>
    <p class="text-xs text-gray-200 m-0 leading-relaxed">
      Sits right beside the C listing explaining why checking <code class="text-amber-200">int32_t am_data &gt;= 0</code> (bit 31 sign bit) catches hardware FIFO errors without tab hopping.
    </p>
  </div>
</div>

<!--
Remember Pass 1's 4x Q4 EMA gain bug (B-2) and Pass 2's stale sample (CA-01)? Lesson 0003 turns both straight into tactile understanding. Tufte right-margin sidenotes sit directly beside the prose—explaining why checking am_data >= 0 catches hardware FIFO errors without tab hopping.
-->

---
layout: default
---

<!-- S158 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">Interactive Simulator: <code v-pre class="text-cyan-300">{{widget:adc-ema-simulator}}</code></h2>
  <span class="text-xs font-mono text-gray-400">docs/egm/tutorial/assets/course.js</span>
</div>

<div class="p-5 rounded-xl bg-gray-900/80 border border-cyan-500/35 space-y-2">
  <div class="text-xs font-mono text-cyan-300 uppercase">Live JavaScript Sandbox Inside Lesson 0003 (#L42)</div>
  <p class="text-sm text-gray-200 m-0">
    Explore real potentiometer step edges with interactive toggles wired straight to the C fixed-point recurrence.
  </p>
</div>

<div class="grid grid-cols-2 gap-5 mt-5">

<!-- S159 -->
  <div v-click class="p-5 rounded-xl bg-amber-950/20 border border-amber-500/40 space-y-2">
    <div class="text-xs font-mono text-amber-300 uppercase">Live Toggle #1 &middot; CA-01</div>
    <div class="text-base font-bold text-white">Discard stale <code class="text-amber-300">msgs[0]</code></div>
    <p class="text-xs text-gray-300 m-0">Flip it off across a step edge and watch the 20 ms-old latched sample visibly drag the waveform behind reality.</p>
  </div>

<!-- S160 -->
  <div v-click class="p-5 rounded-xl bg-red-950/20 border border-red-500/40 space-y-2">
    <div class="text-xs font-mono text-red-300 uppercase">Live Toggle #2 &middot; B-2</div>
    <div class="text-base font-bold text-white"><code class="text-red-300">&gt;&gt; 2</code> vs. <code class="text-emerald-300">(ema_q4 + 8) &gt;&gt; 4</code></div>
    <p class="text-xs text-gray-300 m-0">Toggle back to buggy <code class="text-red-300">&gt;&gt; 2</code> and the curve pegs to <code class="text-white">4095</code> at quarter-turn pot travel.</p>
  </div>

</div>

<!--
Rather than passively reading fixed-point bit-shift equations, course.js embeds a live adc-ema-simulator widget right inside the lesson. Flip off "Discard msgs[0]" on a step change, and you watch that twenty-millisecond-old hardware sample visibly drag the waveform behind reality. Switch B-2 back to >> 2, and the curve pegs to 4095 at a quarter turn—showing why Goldfish B-2 mattered immediately.
-->

---
layout: default
---

<!-- S161 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">Inline <code class="text-emerald-300">:::exercise</code> With Prism <code class="text-cyan-300">keep-markup</code></h2>
  <span class="text-xs font-mono text-gray-400">ex-0003 &middot; 0003-q4-ema-filtering-and-adc-extraction.md#L399-L412</span>
</div>

<div class="mt-6 p-6 rounded-2xl bg-gray-900/80 border border-emerald-500/35 space-y-4 font-mono text-sm">
  <div class="text-xs text-emerald-400 uppercase">Fill-in-the-Blank C99 Exercise</div>
  <div class="p-4 rounded-xl bg-gray-950 border border-gray-800 text-gray-200 leading-loose">
    <div>raw = (uint16_t)(msg-&gt;am_data &gt;&gt; <span class="px-2 py-0.5 rounded bg-cyan-950/60 border border-cyan-400/60 text-cyan-200">[ BARGRAPH_ADC_SHIFT ]</span>);</div>
    <div>*ema_q4 = *ema_q4 - (*ema_q4 &gt;&gt; <span class="px-2 py-0.5 rounded bg-cyan-950/60 border border-cyan-400/60 text-cyan-200">[ 2 ]</span>) + ((uint32_t)raw_avg &lt;&lt; 2);</div>
    <div>return (uint16_t)((*ema_q4 + <span class="px-2 py-0.5 rounded bg-cyan-950/60 border border-cyan-400/60 text-cyan-200">[ 8u ]</span>) &gt;&gt; 4);</div>
  </div>
</div>

<!--
Right underneath sits an inline keep-markup C exercise forcing you to type BARGRAPH_ADC_SHIFT, shift 2, and rounding offset 8u yourself.
-->

---
layout: default
---

<!-- S162 -->
<div class="flex items-center justify-between mb-4">
  <h2 class="text-2xl font-bold m-0">Equal-Word-Count Retrieval Quiz (<code class="text-purple-300">:::quiz</code>)</h2>
  <span class="text-xs font-mono text-gray-400">quiz-0003 Q2 &middot; #L434-L452 (Strictly 10 words per option)</span>
</div>

<div class="p-4 rounded-xl bg-gray-900/80 border border-purple-500/35 space-y-2.5 text-xs">
  <div class="font-bold text-white text-sm">Why does <code class="text-cyan-300">bargraph_sample_adc()</code> discard <code class="text-amber-300">msgs[0]</code>?</div>
  <div class="grid grid-cols-2 gap-2.5 font-mono text-gray-300">
    <div class="p-2.5 rounded bg-gray-950 border border-gray-800">A) (10 words) &mdash; distraction-free equal length</div>
    <div class="p-2.5 rounded bg-emerald-950/30 border border-emerald-500/50 text-emerald-200">B) (10 words) &mdash; latched stale head from previous burst</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-800">C) (10 words) &mdash; zero longest-answer guessing cues</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-800">D) (10 words) &mdash; enforced by <code class="text-purple-300">build.py --check</code></div>
  </div>
</div>

<!-- S163 -->
<div v-click class="mt-5 p-4 rounded-xl bg-purple-950/20 border border-purple-500/40 text-center text-sm text-purple-200">
  <strong>Cognitive Science Payoff:</strong> Active Recall &rarr; Durable <em>Storage Strength</em> (Not Passive Skimming <em>Fluency</em>)
</div>

<!--
Then build.py checks that all four quiz options contain ten words apiece—eliminating longest-answer guessing cues when testing msgs[0] staleness. Answering why sample zero is stale locks that hardware invariant into storage strength instead of letting your eyes glide past prose.
-->

---
layout: default
---

<!-- S164 -->
<div class="flex items-center justify-between mb-6">
  <h2 class="text-2xl font-bold m-0">Simulator Flash #1: Lesson 0001</h2>
  <span class="text-xs font-mono text-cyan-300">0001-breadboard-circuit-and-current-budget.md#L63</span>
</div>

<div class="mt-6 p-7 rounded-2xl bg-cyan-950/20 border border-cyan-500/40 space-y-4">
  <div v-pre class="text-lg font-bold font-mono text-cyan-300">{{widget:breadboard-simulator}}</div>
  <div class="grid grid-cols-3 gap-4 text-sm">
    <div class="p-4 rounded-xl bg-gray-900/80 border border-gray-800">
      <div class="text-xs font-mono text-gray-400">Wiper Sweep</div>
      <div class="text-lg font-bold text-white mt-1">0 .. 3300 mV</div>
    </div>
    <div class="p-4 rounded-xl bg-gray-900/80 border border-gray-800">
      <div class="text-xs font-mono text-gray-400">Source Impedance</div>
      <div class="text-lg font-bold text-white mt-1">Thevenin <code class="text-cyan-300">R_S</code></div>
    </div>
    <div class="p-4 rounded-xl bg-gray-900/80 border border-gray-800">
      <div class="text-xs font-mono text-gray-400">6-LED Budget</div>
      <div class="text-lg font-bold text-emerald-300 mt-1">23.6 mA total</div>
    </div>
  </div>
</div>

<!--
Lesson 0001 includes a live breadboard simulator too—letting you drag the wiper to verify Thevenin source impedance and LED budgets.
-->

---
layout: default
---

<!-- S165 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Simulator Flash #2: Lesson 0002</h2>
  <span class="text-xs font-mono text-emerald-300">0002-proportional-bargraph-math-and-pwm-scaling.md#L47</span>
</div>

<div class="p-6 rounded-2xl bg-emerald-950/20 border border-emerald-500/40 space-y-3">
  <div v-pre class="text-lg font-bold font-mono text-emerald-300">{{widget:pwm-segment-visualizer}}</div>
  <p class="text-sm text-gray-200 m-0">
    Interpolates <code class="text-emerald-200">0..393,210</code> units live across all <strong class="text-white">6</strong> <code class="text-emerald-200">0..65535</code> PWM channels &mdash; making the <code class="text-amber-300">b16ONE</code> <code class="text-red-300">65536</code>-to-zero truncation bug obvious at a glance.
  </p>
</div>

<!-- S166 -->
<div v-click class="mt-6 grid grid-cols-4 gap-3 text-center text-xs font-mono">
  <div class="p-3 rounded-lg bg-gray-900 border border-cyan-500/35 text-cyan-300">breadboard-simulator</div>
  <div class="p-3 rounded-lg bg-gray-900 border border-emerald-500/35 text-emerald-300">pwm-segment-visualizer</div>
  <div class="p-3 rounded-lg bg-gray-900 border border-purple-500/35 text-purple-300">adc-ema-simulator</div>
  <div class="p-3 rounded-lg bg-amber-950/30 border border-amber-500/45 text-amber-200 font-bold">Next &rarr; Lesson 0005</div>
</div>

<!--
And Lesson 0002 models all six 0..65535 PWM channels live—making the b16ONE 65536-to-zero truncation bug obvious at a glance. Three interactive JavaScript simulators turn dry embedded arithmetic into tactile play. Now let's jump forward to Lesson 0005 for the hardware traps.
-->

---
layout: default
---

<!-- S167 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Browser Tour Stop 3: Lesson 0005 Hardware Traps</h2>
  <span class="text-xs font-mono text-cyan-300">0005-out-of-tree-bsp-and-adc-irq-gating.md#L58-L69</span>
</div>

<div class="p-5 rounded-xl bg-gray-900/80 border border-cyan-500/35 space-y-2 mt-4">
  <div class="text-xs font-mono text-cyan-400 uppercase">Section 2 &middot; Conquering Traps #2, #6, and #7</div>
  <div class="text-lg font-bold text-white">Distils Acts II &amp; III directly into the human curriculum</div>
  <p class="text-sm text-gray-300 m-0">Dead ends aren&rsquo;t hidden: the lesson explicitly walks through our <code class="text-amber-300">RP23XX_ADC_DIV</code> false start.</p>
</div>

<!-- S168 -->
<div v-click class="mt-5 grid grid-cols-3 gap-4 text-xs">
  <div class="p-4 rounded-xl bg-red-950/20 border border-red-500/35 space-y-1">
    <div class="font-mono font-bold text-red-300">Why ADC_DIV Fails</div>
    <div class="text-gray-200"><code class="text-red-200">START_ONCE</code> ignores <code class="text-gray-200">RP23XX_ADC_DIV</code> completely (#L64-L68)</div>
  </div>
  <div class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/35 space-y-1">
    <div class="font-mono font-bold text-emerald-300">NVIC IRQ Gating</div>
    <div class="text-gray-200">Keep <code class="text-gray-200">/dev/adc0</code> open &amp; mask <code class="text-emerald-200">RP23XX_ADC_IRQ_FIFO</code></div>
  </div>
  <div class="p-4 rounded-xl bg-amber-950/20 border border-amber-500/35 space-y-1">
    <div class="font-mono font-bold text-amber-300">RP2350-E9 Isolation</div>
    <div class="text-gray-200"><code class="text-amber-200">modreg32</code> clears <code class="text-gray-200">IE | PUE | PDE | ISO</code> on GPIO 26</div>
  </div>
</div>

<!--
Lesson 0005 distils the entire Act II and Act III hardware investigation into one focused chapter—explicitly teaching our RP23XX_ADC_DIV false start. A great tutorial doesn't hide dead ends—it teaches why ADC_DIV ignores START_ONCE, so you never repeat that assumption elsewhere.
-->

---
layout: default
---

<!-- S169 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Printable Bench Reference Sheet</h2>
  <span class="text-xs font-mono text-gray-400">reference/nuttx-rp2350-driver-traps.md#L15-L25</span>
</div>

<div class="p-5 rounded-xl bg-gray-900/80 border border-amber-500/35 space-y-3 mt-4">
  <div class="text-xs font-mono text-amber-300 uppercase">One-Page Lookup Table &middot; All 7 Upstream RP23xx / NuttX Traps</div>
  <div class="grid grid-cols-2 gap-3 text-xs font-mono text-gray-200">
    <div class="p-2.5 rounded bg-gray-950 border border-gray-800">1. <code class="text-amber-300">CONFIG_ANALOG=y</code> silent Kconfig drop</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-800">2. <code class="text-amber-300">my_reset()</code> GPIO 26&ndash;29 clobber</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-800">3. <code class="text-amber-300">RP2350-E9</code> IE=1 analogue leakage</div>
    <div class="p-2.5 rounded bg-gray-950 border border-gray-800">4. <code class="text-amber-300">START_ONCE</code> 500 kHz storm &amp; <code class="text-emerald-300">adc_read</code> short read</div>
  </div>
</div>

<!-- S170 -->
<div v-click class="mt-5 p-5 rounded-xl bg-gray-100 text-gray-900 border-2 border-gray-300 flex items-center justify-between">
  <div>
    <div class="text-xs font-mono font-bold uppercase text-gray-600">@media print Override (assets/style.css#L1031-L1032)</div>
    <div class="text-sm font-bold mt-0.5">Strips dark backgrounds into crisp light-mode A4 ink for your workbench</div>
  </div>
  <span class="px-3 py-1 rounded bg-gray-900 text-white font-mono text-xs">Print Ready</span>
</div>

<!--
Beside the six lessons sit four condensed reference cards, including this one-page lookup table of all seven NuttX and RP2350 traps. Hit print, and @media print strips dark backgrounds into crisp A4 ink—ready to tape right above your soldering iron.
-->

---
layout: default
---

<!-- S171 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Knowledge Loop Completed</h2>
</div>

<div class="mt-6 p-7 rounded-2xl bg-emerald-950/20 border border-emerald-500/40 text-center space-y-3">
  <div class="text-xs font-mono text-emerald-400 uppercase tracking-wider">Machine Truth &rarr; Human Mental Model</div>
  <div class="text-xl font-bold font-mono text-white">
    3,241 Contracts &rarr; 2-Pass Goldfish Audit &rarr; 6-Lesson Codelab + Bench Sheet
  </div>
</div>

<!-- S172 -->
<div v-click class="mt-7 p-6 rounded-2xl bg-amber-950/25 border-2 border-amber-500/50 text-center space-y-1">
  <div class="text-xs font-mono text-amber-300 uppercase tracking-wider">Surprise Bonus Twist</div>
  <div class="text-xl font-bold text-white">
    Forcing Lesson 0006 to compile under strict C99 caught <strong class="text-amber-300">2 real firmware bugs</strong> we missed in Phase 5!
  </div>
</div>

<!--
That closes the knowledge loop between machine and human: what the Goldfish discovered in markdown now lives inside your own head. Better yet, forcing Lesson 0006 to compile real C99 under -Werror -pedantic actually caught two bugs inside our Phase 5 code!
-->

---
layout: default
---

<!-- S173 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Browser Tour Stop 4: Lesson 0006 Host Lab</h2>
  <span class="text-xs font-mono text-emerald-300">tutorial/apps/0006-bargraphd-daemon-and-nsh-cli/src/main.c#L743-L798</span>
</div>

<div class="p-5 rounded-xl bg-gray-900/80 border border-emerald-500/35 space-y-2 mt-4">
  <div class="text-xs font-mono text-emerald-400 uppercase">Strict C99 Host POSIX Threads Harness</div>
  <div class="text-lg font-bold text-white">Compiles the full <code class="text-emerald-300">bargraphd</code> state machine under <code class="text-cyan-300">-std=c99 -Wall -Wextra -Werror -pedantic</code></div>
</div>

<!-- S174 -->
<div v-click class="mt-5 grid grid-cols-2 gap-4 text-xs">
  <div class="p-4 rounded-xl bg-red-950/20 border border-red-500/35 space-y-1">
    <div class="font-mono font-bold text-red-300">Phase 5 Shortcut (Makefile#L40-L75)</div>
    <p class="text-gray-300 m-0">Synthesised <code class="text-red-200">/tmp/bargraph_test.XXXXXX.c</code> via <code class="text-gray-200">printf '%s\n'</code>.</p>
  </div>
  <div class="p-4 rounded-xl bg-emerald-950/20 border border-emerald-500/40 space-y-1">
    <div class="font-mono font-bold text-emerald-300">Self-Correction #1 (Lesson 0006)</div>
    <p class="text-gray-200 m-0">Replaced with genuine single-source <code class="text-emerald-200">src/main.c</code> C99 compilation.</p>
  </div>
</div>

<!--
Lesson 0006 compiles the full bargraphd state machine on macOS pthreads under strict C99—turning teaching material into an uncompromising integration harness. Remember Vanilla's clean bargraph_logic.c? In Phase 5, EGM Makefile synthesised /tmp/bargraph_test.c with printf—and /teach-code forced genuine single-source compilation.
-->

---
layout: default
---

<!-- S175 -->
<div class="flex items-center justify-between mb-3">
  <h2 class="text-2xl font-bold m-0">Self-Correction #2: <code class="text-amber-300">bargraph test</code> <code class="text-red-300">MANUAL</code>-Clobber Bug</h2>
  <span class="text-xs font-mono text-gray-400">Trace 24-0cc3a75f#L61-L65</span>
</div>

<div class="p-3.5 rounded-xl bg-amber-950/20 border border-amber-500/35 text-xs text-gray-200 mb-3">
  Running Lesson 0006&rsquo;s pthreads harness surfaced a tick-sync <code class="text-amber-200">pthread_cond_wait</code> block (Step 133/135) and a <code class="text-red-300">MANUAL</code>-mode clobber bug (Step 144/146).
</div>

<!-- S176 -->
<div v-click class="p-3.5 rounded-xl bg-gray-900/70 border border-cyan-500/35 space-y-2">
  <div class="text-xs font-mono text-cyan-300">Phase 5 bargraph_main.c#L557-L565, L841-L854 &amp; Lesson 0006 main.c#L743-L749</div>

```c {1,2,4,7,8}
/* Phase 5 bargraph_main.c#L843 -> #L561 resets MANUAL before #L853! */
int ret = bargraph_start_daemon(); /* forces mode = BARGRAPH_MODE_SENSING */
if (g_bargraph.mode != BARGRAPH_MODE_TEST)
  g_bargraph.prev_mode = g_bargraph.mode; /* captures SENSING, not MANUAL! */
/* Lesson 0006 main.c#L743-L749 fixes tick-sync wait predicate: */
static void bargraph_wait_next_tick_locked(uint32_t prev_seq)
{ while (g_bargraph.running && g_bargraph.tick_seq == prev_seq)
    pthread_cond_wait(&g_bargraph.done_cond, &g_bargraph.lock); }
```

</div>

<!--
Worse, exercising bargraphd threads inside trace 24-0cc3a75f hit two surprises: a startup tick-sync pthread_cond_wait block and a MANUAL-mode clobber bug. bargraph test called bargraph_start_daemon(), which reset MANUAL to SENSING before saving prev_mode—proving human docs catch bugs too.
-->
