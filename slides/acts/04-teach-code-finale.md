---
layout: default
---

<!-- S27 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Act IV Finale: From &ldquo;Docs for the AI&rdquo; to &ldquo;Docs for the Human&rdquo;</h2>
</div>

<div class="p-4 rounded-xl bg-gray-900/80 border border-gray-800 mb-5">
  <div class="text-sm font-semibold text-white">The Final Boss of AI Engineering: <span class="text-amber-300">Comprehension Debt</span></div>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Dave Rensin&rsquo;s Third Rule &mdash; <em>&ldquo;Humans Own the Outcomes&rdquo;</em></strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>The AI scored <code class="text-emerald-300">10/10</code> and shipped <code class="text-gray-300">872</code> lines of <code class="text-gray-300">bargraph_main.c</code>&mdash;but if a wire pops loose in six months, <em>you</em> have to debug it on the bench.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Rolling Docs for the Human (<code class="text-cyan-300">/teach-code</code>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Adapted from <strong class="text-white">Matt Pocock&rsquo;s</strong> open-source <code class="text-cyan-300">/teach</code> skill (using <code class="text-gray-300">/agy-customizations</code>) and installed via <code class="text-emerald-300">npx skills add domesticmouse/skills</code>&mdash;turning our <code class="text-gray-300">README.md</code> contracts, design spec, and Goldfish reports into a 6-lesson <strong class="text-white">Edward Tufte</strong> interactive HTML course.</li>
    </ul>
  </li>
</ol>

<!--
That brings us to Act IV—and the biggest trap in AI-assisted engineering. The AI just scored 10/10 and shipped 872 lines of bargraph_main.c. Brilliant—except Dave Rensin's third rule says "Humans Own the Outcomes." If a wire pops loose on the bench in six months, you have to debug that firmware. To pay down comprehension debt, I adapted Matt Pocock's brilliant /teach skill (using /agy-customizations) into /teach-code—turning our README.md contracts, design spec, and Goldfish reports into a six-lesson interactive Edward Tufte HTML course.
-->

---
layout: cast-full
title: "Act IV: Installing & Running /teach-code (06-teach-code.cast)"
cast: /casts/06-teach-code.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S28 -->
<!--
This recording starts in the terminal so you can see how teach-code installs dynamically. Running npx skills add domesticmouse/skills. We select teach-code from the repository. Target Antigravity in the project workspace. Skill installed. Now let's launch Antigravity. Now we're inside Antigravity. Let's ask /teach-code to build the full six-lesson course. Prompting /teach-code to turn our specs, contracts, and Goldfish catches into six self-contained HTML lessons with runnable C99 companion apps. We're watching thirty-nine raw minutes at seventy-nine times speed. It maps the learner's Zone of Proximal Development, builds six runnable C99 companion apps in tutorial/apps/0001 through 0006 under -Wall -Wextra -Werror -pedantic, crafts three interactive JavaScript simulators and four printable reference sheets, and compiles the Tufte HTML lessons. All six lessons generated. Now let's run the zero-divergence verification check. Running sync_listing.py --check and build.py --check. It verifies that every C snippet inside the Markdown lessons is byte-for-byte identical to the compiled C99 test binaries. Zero drift. Now let's tour the compiled interactive course!
-->

---
layout: default
---

<!-- S29 -->
<div class="flex items-center justify-between mb-6">
  <h2 class="text-2xl font-bold m-0">Finale Showcase 1: Course Hub &amp; 6-Lesson Story Arc</h2>
  <a href="./tutorial/index.html" target="_blank" class="px-3.5 py-1.5 rounded-lg bg-cyan-950/40 border border-cyan-400/50 text-sm font-mono text-cyan-300 hover:bg-cyan-900/50">Open ./tutorial/index.html &nearr;</a>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Edward Tufte Typography &amp; Figure 0.1 Mermaid Story Arc (<code class="text-cyan-300">Lesson 0001</code> &rarr; <code class="text-cyan-300">Lesson 0006</code>)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li><code class="text-cyan-300">0001</code> Breadboard Geometry &amp; <code class="text-gray-300">23.6 mA</code> Current Budget &rarr; <code class="text-cyan-300">0002</code> 16-Bit Proportional PWM Scaling &rarr; <code class="text-cyan-300">0003</code> <code class="text-gray-300">&gt;&gt; 19</code> Bit-Shift &amp; Q4 Fixed-Point EMA</li>
      <li><code class="text-cyan-300">0004</code> Multi-Channel PWM State &rarr; <code class="text-cyan-300">0005</code> Out-of-Tree NuttX BSP &amp; NVIC IRQ Gating &rarr; <code class="text-cyan-300">0006</code> <code class="text-gray-300">50 Hz</code> <code class="text-emerald-300">bargraphd</code> Daemon &amp; NSH CLI</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Zero-Divergence C99 Verification Gate</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Runnable companion apps in <code class="text-gray-300">tutorial/apps/0001..0006/</code> compiled under <code class="text-emerald-300">-std=c99 -Wall -Wextra -Werror -pedantic</code> and verified byte-for-byte via <code class="text-cyan-300">sync_listing.py --check</code> + <code class="text-cyan-300">build.py --check</code>.</li>
    </ul>
  </li>
</ol>

<!--
Here is the compiled course landing page at docs/egm/tutorial/index.html—and we can pop it open directly from the slide badge. Notice the clean Edward Tufte typography and the Figure 0.1 Mermaid story arc: it walks a human engineer from Lesson 0001 (breadboard geometry and the 23.6 mA current budget) through PWM scaling, Q4 EMA filtering, and out-of-tree NuttX BSP bring-up, all the way to Lesson 0006 (bargraphd and the NSH CLI). Crucially, tutorials usually rot the moment code changes. Here, sync_listing.py --check guarantees every lesson snippet compiles cleanly under strict C99 (-Wall -Wextra -Werror -pedantic).
-->

---
layout: default
---

<!-- S30 -->
<div class="flex items-center justify-between mb-6">
  <h2 class="text-2xl font-bold m-0">Finale Showcase 2: Interactive JS Simulators, C99 Exercises &amp; Quizzes</h2>
  <div class="flex items-center gap-2 text-xs font-mono">
    <a href="./tutorial/lessons/0001-breadboard-circuit-and-current-budget.html" target="_blank" class="px-2.5 py-1 rounded bg-gray-900 border border-gray-700 text-cyan-300 hover:underline">0001 &nearr;</a>
    <a href="./tutorial/lessons/0002-proportional-bargraph-math-and-pwm-scaling.html" target="_blank" class="px-2.5 py-1 rounded bg-gray-900 border border-gray-700 text-cyan-300 hover:underline">0002 &nearr;</a>
    <a href="./tutorial/lessons/0003-q4-ema-filtering-and-adc-extraction.html" target="_blank" class="px-2.5 py-1 rounded bg-gray-900 border border-gray-700 text-cyan-300 hover:underline">0003 &nearr;</a>
  </div>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Live Hardware &amp; PWM Simulators (<code class="text-cyan-300">Lesson 0001</code> &amp; <code class="text-cyan-300">0002</code>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Drag the <code class="text-amber-300">0..3300 mV</code> wiper in <code class="text-cyan-300">breadboard-simulator</code> (watching per-pin mA and <code class="text-gray-300">Cols E &amp; F</code> geometry) and scrub 6-channel <code class="text-gray-300">0..65535</code> duty cycles in <code class="text-cyan-300">pwm-segment-visualizer</code>.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Turning Goldfish Catches into Interactive Play (<code class="text-cyan-300">Lesson 0003</code> <code class="text-emerald-300">adc-ema-simulator</code>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Toggle <em>&ldquo;Discard stale <code class="text-amber-300">msgs[0]</code>&rdquo;</em> (<code class="text-amber-300">CA-01</code>) and flip <code class="text-red-300">&gt;&gt; 2</code> vs. <code class="text-emerald-300">(ema_q4 + 8) &gt;&gt; 4</code> (<code class="text-red-300">B-2</code>) to watch the bar graph peg at a quarter-turn of the knob!</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Tufte Sidenotes, Fill-in-the-Blank C99 &amp; Equal-Length Quizzes</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Right-margin sidenotes, Prism <code class="text-gray-300">keep-markup</code> C99 exercises, and Matt Pocock&rsquo;s retrieval quizzes where every option has the exact same word count (<code class="text-emerald-300">10 words/option</code>).</li>
    </ul>
  </li>
</ol>

<!--
Inside the lessons, you don't just passively skim prose—you play with the hardware physics and the exact bugs the Goldfish caught. In Lesson 0001's breadboard-simulator, you drag the 0 to 3300 mV potentiometer wiper and watch per-pin milliamps update live; in Lesson 0002's pwm-segment-visualizer, you scrub across all six 16-bit (0..65535) PWM duty cycles. Better yet, look at Lesson 0003's adc-ema-simulator: you can toggle a checkbox for Pass 2's CA-01 stale msgs[0] sample or flip Pass 1's B-2 >> 2 vs >> 4 shift bug and watch the LED bar graph peg at a quarter-turn of the knob! Right alongside sit Tufte margin sidenotes, fill-in-the-blank C99 exercises, and Matt Pocock's retrieval quizzes where every multiple-choice option is forced to the exact same word count (10 words) so you can't guess the longest answer.
-->

---
layout: default
---

<!-- S31 -->
<div class="flex items-center justify-between mb-6">
  <h2 class="text-2xl font-bold m-0">Finale Showcase 3: Teaching Dead Ends, Printable Bench Sheets &amp; Bug Catch!</h2>
  <div class="flex items-center gap-2 text-xs font-mono">
    <a href="./tutorial/lessons/0005-out-of-tree-bsp-and-adc-irq-gating.html" target="_blank" class="px-2.5 py-1 rounded bg-gray-900 border border-gray-700 text-cyan-300 hover:underline">Lesson 0005 &nearr;</a>
    <a href="./tutorial/reference/nuttx-rp2350-driver-traps.html" target="_blank" class="px-2.5 py-1 rounded bg-gray-900 border border-gray-700 text-amber-300 hover:underline">A4 Traps Sheet &nearr;</a>
  </div>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Dead Ends as Curriculum + Printable <code class="text-cyan-300">@media print</code> A4 Bench Sheets</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li><code class="text-cyan-300">Lesson 0005</code> explicitly teaches <em>why</em> <code class="text-amber-300">CONFIG_RP23XX_ADC_DIV</code> failed under <code class="text-red-300">START_ONCE</code>, paired with <strong class="text-white">4 single-page A4 reference sheets</strong> (<code class="text-gray-300">nuttx-rp2350-driver-traps.md</code>) formatted for the workbench.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-emerald-300">The Bonus Twist &mdash; Docs for Humans Catch Firmware Bugs Too!</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Compiling <code class="text-cyan-300">Lesson 0006</code>&rsquo;s strict C99 <code class="text-gray-300">pthreads</code> harness caught <strong class="text-white">2 real bugs</strong> missed in Phase 5&mdash;including <code class="text-amber-300">bargraph test</code> forgetting to set <code class="text-emerald-300">MANUAL</code> mode so the <code class="text-gray-300">50 Hz</code> loop clobbered the LED sweep after <code class="text-gray-300">20 ms</code>!</li>
    </ul>
  </li>
</ol>

<!--
Two final touches in the course really surprised me. First, Lesson 0005 doesn't hide our RP23XX_ADC_DIV false start—it teaches why START_ONCE ignores the hardware divider, paired with four @media print A4 workbench reference sheets (nuttx-rp2350-driver-traps.md) you can print out and tape beside your oscilloscope. Second, the bonus twist: when /teach-code compiled Lesson 0006's strict C99 pthreads harness to teach the daemon state machine, the tutorial compiler caught two real bugs we missed in Phase 5—including bargraph test forgetting to set MANUAL mode!
-->

---
layout: default
---

<!-- S32 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The &ldquo;Roll Your Own Docs&rdquo; Flywheel: Three Habits for Monday Morning</h2>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 mt-5 leading-relaxed">
  <li v-click>
    <strong class="text-white">Feed the Elephant First (<span class="text-cyan-300">Docs for the AI</span>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Maintain directory-level <code class="text-gray-300">README.md</code> contracts (<code class="text-cyan-300">/egm-bootstrap</code> + <code class="text-cyan-300">/egm-drift-report</code>) so agents navigate architecture instead of grepping blind.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Unleash Stateless Goldfish Before Coding</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Audit specs with fresh-context subagents (<code class="text-cyan-300">/egm-review</code> + <code class="text-cyan-300">/clear</code>) to catch second-order hardware traps in Markdown.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Pay Down Comprehension Debt Immediately (<span class="text-emerald-300">Docs for the Human</span>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Run <code class="text-emerald-300">/teach-code</code> so a <code class="text-gray-300">10/10</code> AI build becomes a <code class="text-gray-300">10/10</code> mental model in your own head.</li>
    </ul>
  </li>
</ol>

<!--
That's the Roll Your Own Docs flywheel—three habits you can steal on Monday morning: One: Feed the Elephant first. Maintain directory-level README.md contracts (/egm-bootstrap and /egm-drift-report) so your agent navigates architecture instead of grepping blind. Two: Unleash stateless Goldfish before coding. Use /egm-review and /clear to catch second-order hardware traps in Markdown. Three: Pay down comprehension debt immediately. Run /teach-code so a 10/10 AI build becomes a 10/10 mental model in your own head.
-->

---
layout: default
---

<!-- S33 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Links, Open-Source Skills &amp; Interactive Course</h2>
</div>

<div class="grid grid-cols-2 gap-4 mt-4">
  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800">
    <div class="flex items-center justify-between">
      <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Repository &amp; Slidev Deck</div>
      <span class="text-xs font-mono text-gray-500">github.com</span>
    </div>
    <div class="text-base font-bold font-mono mt-1">
      <a href="https://github.com/domesticmouse/nuttxspace" target="_blank" class="text-cyan-300 hover:underline">github.com/domesticmouse/nuttxspace</a>
    </div>
    <p class="text-xs text-gray-400 mt-1.5 m-0">
      Complete experiment repo, 7 terminal casts, specs, Goldfish reports &amp; Slidev deck
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800">
    <div class="flex items-center justify-between">
      <div class="text-xs font-mono text-amber-400 uppercase tracking-wide">Elephant-Goldfish Model (egm)</div>
      <span class="text-xs font-mono text-gray-500">@drensin</span>
    </div>
    <div class="text-base font-bold font-mono mt-1 text-white">
      /plugin &rarr; Discover &rarr; egm
    </div>
    <p class="text-xs text-gray-400 mt-1.5 m-0">
      Credit: <strong class="text-gray-200">Dave Rensin</strong> &middot; Built-in Antigravity plugin marketplace
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800">
    <div class="flex items-center justify-between">
      <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">/teach-code Skill</div>
      <span class="text-xs font-mono text-gray-500">skills CLI</span>
    </div>
    <div class="text-base font-bold font-mono mt-1 text-emerald-300">
      npx skills add domesticmouse/skills
    </div>
    <p class="text-xs text-gray-400 mt-1.5 m-0">
      Credit: Adapted from <strong class="text-gray-200">Matt Pocock&rsquo;s</strong> <code class="text-gray-300">/teach</code> skill via <code class="text-gray-300">/agy-customizations</code>
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800">
    <div class="flex items-center justify-between">
      <div class="text-xs font-mono text-purple-400 uppercase tracking-wide">Live 6-Lesson Interactive Course</div>
      <span class="text-xs font-mono text-gray-500">Edward Tufte HTML</span>
    </div>
    <div class="text-base font-bold font-mono mt-1">
      <a href="./tutorial/index.html" target="_blank" class="text-purple-300 hover:underline">/nuttxspace/tutorial/index.html &nearr;</a>
    </div>
    <p class="text-xs text-gray-400 mt-1.5 m-0">
      Interactive JS hardware simulators, strict C99 exercises &amp; printable A4 bench sheets
    </p>
  </div>
</div>

<!--
Everything you saw tonight—the nuttxspace repo, Dave Rensin's egm plugin, the /teach-code skill (npx skills add domesticmouse/skills), all seven terminal casts, and the live six-lesson interactive course—is open source at these links. Thanks heaps for listening, and I'd love to take your questions!
-->
