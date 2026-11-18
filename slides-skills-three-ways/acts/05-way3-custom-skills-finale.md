---
layout: skill-breakdown
way: 3
title: "Way 3 (Story A): Packaging Talk-Production Tooling into <code class='text-emerald-300'>domesticmouse/skills</code>"
---

<!-- S33 -->
<div class="space-y-3.5">
  <div class="text-xs text-gray-300">
    Using <code class="text-cyan-300">/agy-customizations</code> + <code class="text-emerald-300">/writing-for-agents</code>, our <code class="text-amber-300">/wayfinder</code> runbooks graduated into repeatable skills in <code class="text-emerald-300">domesticmouse/skills</code>:
  </div>

  <div class="grid grid-cols-2 gap-4 text-xs">
    <div class="p-4 rounded-xl bg-gray-900/80 border border-emerald-500/30 space-y-2">
      <div class="font-mono text-emerald-300 font-bold uppercase"><code class="text-emerald-200">/asciinema</code> &amp; <code class="text-emerald-200">/cast-player-html</code></div>
      <ul class="text-gray-200 space-y-1.5 list-disc pl-4 m-0 leading-relaxed">
        <li><code class="text-gray-300">sanitize-cast.py</code> strips Linux <code class="text-gray-300">OSC 3008</code>/<code class="text-gray-300">7</code> in-place so event indices never shift.</li>
        <li><code class="text-gray-300">trim-cast.py</code> &amp; <code class="text-gray-300">mark-cast.py</code> simulate 2D terminal frames with <strong class="text-white"><code class="text-cyan-300">pyte</code> via <code class="text-cyan-300">uv</code></strong> (<code class="text-gray-300">--verify-strip</code>, zsh <code class="text-amber-300">[REV]</code>/<code class="text-emerald-300">[UNREV]</code> splits, <code class="text-gray-300">--tighten-pastes</code>).</li>
        <li>Syncs <code class="text-gray-300">EMBEDDED_CAST_GZ_B64</code> into 16:9 HTML test-drive players without CORS issues.</li>
      </ul>
    </div>
    <div v-click class="p-4 rounded-xl bg-gray-900/80 border border-cyan-500/30 space-y-2">
      <div class="font-mono text-cyan-300 font-bold uppercase"><code class="text-cyan-200">/bjm-voice</code>, <code class="text-cyan-200">/slidev-deck</code> &amp; <code class="text-cyan-200">/crop-image</code></div>
      <ul class="text-gray-200 space-y-1.5 list-disc pl-4 m-0 leading-relaxed">
        <li><strong class="text-white"><code class="text-cyan-300">/bjm-voice</code>:</strong> Australian English spelling, curious-peer tone, zero AI slop, candid false starts.</li>
        <li><strong class="text-white"><code class="text-cyan-300">/slidev-deck</code>:</strong> Modular acts, <code class="text-gray-300">AsciinemaPlayer.vue</code> adaptive speed compiler (<code class="text-gray-300">&ge; 2x</code>, <code class="text-gray-300">&le; 30s</code>), and <code class="text-gray-300">BASE_URL</code> asset safety.</li>
        <li><strong class="text-white"><code class="text-cyan-300">/crop-image</code> &amp; <code class="text-cyan-300">/agy-brain-explorer</code>:</strong> Zero-dep PNG crops &amp; Markdown session trace exports.</li>
      </ul>
    </div>
  </div>
</div>

<!--
That brings us to Way 3: authoring and evolving your own modular skills using /agy-customizations and /writing-for-agents. Here is Story A—packaging our talk-production tooling into domesticmouse/skills. Instead of letting an LLM guess ANSI escape offsets by eye, /asciinema and /cast-player-html bundle Python scripts like sanitize-cast.py, trim-cast.py, and mark-cast.py—running real 2D terminal screen simulation with pyte via uv before stripping a single frame. Beside them sit /bjm-voice for my Australian spelling and candid voice, /slidev-deck for the exact dark-mode Vue layouts and adaptive player you're watching right now, and /crop-image and /agy-brain-explorer. Every skill owns one crisp job.
-->

---
layout: default
---

<!-- S34 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">Way 3 (Story B): The Final Boss &mdash; <span class="text-amber-300">Comprehension Debt</span></h2>
</div>

<div class="p-4 rounded-xl bg-gray-900/80 border border-gray-800 mb-5">
  <div class="text-sm font-semibold text-white">
    Way 1 scored <code class="text-emerald-300">10/10</code> and shipped <code class="text-cyan-300">872</code> lines of <code class="text-gray-300">bargraph_main.c</code> &mdash; so why isn&rsquo;t the job finished?
  </div>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Dave Rensin&rsquo;s Third Rule &mdash; <em>&ldquo;Humans Own the Outcomes&rdquo;</em></strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>If a jumper wire pops loose on the bench in six months, <strong class="text-white">you</strong> have to debug that RP2350 firmware&mdash;not the session window that closed months ago.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Fluency Strength vs. Storage Strength</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Skimming a glowing PR summary creates an illusion of fluency; building durable mental models requires <strong class="text-emerald-300">retrieval practice</strong>, <strong class="text-emerald-300">interactive simulation</strong>, and <strong class="text-emerald-300">verifiable code labs</strong>.</li>
    </ul>
  </li>
</ol>

<!--
Now for Story B of Way 3—the problem that scares me most in AI-assisted software engineering: comprehension debt. Way 1 gave us 10 out of 10 on silicon and 872 lines of bargraph_main.c. Brilliant—except Dave Rensin's third rule says "Humans Own the Outcomes." When a wire pops loose on your workbench in six months, you have to understand what ships. Skimming a clean diff gives fluency strength, not storage strength. So I took Matt Pocock's open-source /teach skill and evolved it into /teach-code.
-->

---
layout: skill-breakdown
way: 3
title: "Way 3 (Story B): Evolving Matt Pocock&rsquo;s <code class='text-amber-300'>/teach</code> into <code class='text-emerald-300'>/teach-code</code>"
---

<!-- S35 -->
<div class="space-y-3">
  <div class="text-xs text-gray-300">
    Inspected <code class="text-amber-300">.agents/skills/teach/SKILL.md</code> (<code class="text-gray-200">140</code> lines) and evolved <code class="text-emerald-300">.agents/skills/teach-code/SKILL.md</code> (<code class="text-gray-200">188</code> lines) via <code class="text-cyan-300">/agy-customizations</code> + <code class="text-cyan-300">/writing-for-agents</code>:
  </div>

  <div class="grid grid-cols-3 gap-3 text-xs">
    <div class="p-3.5 rounded-xl bg-gray-900/80 border border-amber-500/30 space-y-1.5">
      <div class="font-mono text-amber-300 font-bold">1. Preserved Pedagogical Core</div>
      <p class="text-gray-300 m-0 leading-relaxed">
        Kept Matt&rsquo;s stateful <code class="text-gray-200">MISSION.md</code>, <code class="text-gray-200">RESOURCES.md</code>, <code class="text-gray-200">learning-records/0001..N.md</code> (ZPD), <strong class="text-white">Edward Tufte</strong> layout, and equal-word-count quizzes (<code class="text-emerald-300">10 words/option</code>).
      </p>
    </div>
    <div v-click class="p-3.5 rounded-xl bg-gray-900/80 border border-cyan-500/30 space-y-1.5">
      <div class="font-mono text-cyan-300 font-bold">2. Disclosed Formats &amp; Compiler</div>
      <p class="text-gray-300 m-0 leading-relaxed">
        Moved schemas to <code class="text-cyan-300">references/*-FORMAT.md</code>; authored clean Markdown (<code class="text-gray-200">:::exercise</code>, <code class="text-gray-200">:::quiz</code>, <code class="text-gray-200">&#123;&#123;sidenote:...&#125;&#125;</code>) compiled by <code class="text-cyan-300">build.py --check</code> with dark + <code class="text-gray-200">@media print</code> CSS.
      </p>
    </div>
    <div v-click class="p-3.5 rounded-xl bg-gray-900/80 border border-emerald-500/30 space-y-1.5">
      <div class="font-mono text-emerald-300 font-bold">3. Zero-Divergence C99 Labs</div>
      <p class="text-gray-300 m-0 leading-relaxed">
        Every lesson backs prose with runnable code in <code class="text-emerald-300">apps/0001..0006/</code> compiled under <code class="text-gray-200">-std=c99 -Wall -Wextra -Werror -pedantic</code> and synced via <code class="text-emerald-300">scripts/sync_listing.py</code>.
      </p>
    </div>
  </div>
</div>

<!--
Here is exactly how /teach evolved into /teach-code. We kept Matt's brilliant pedagogical core intact—MISSION.md, learning records for Zone of Proximal Development, Tufte layout, and equal-word-count quizzes so you can't guess the longest option. Then, applying /writing-for-agents, we pushed detailed Markdown schemas down into references/*-FORMAT.md, added a deterministic static builder build.py --check that enforces links, HTML tags, and quiz word counts, and paired every lesson with a strict C99 companion app in apps/0001 through 0006 verified byte-for-byte by scripts/sync_listing.py. Let's watch /teach-code install dynamically and build our course.
-->

---
layout: cast-full
title: "Way 3 in Action: Installing & Running /teach-code (06-teach-code.cast)"
cast: /casts/06-teach-code.cast
autoPlay: false
speed: 2.0
idleTimeLimit: 2.0
fit: both
---

<!-- S36 -->
<!--
This recording starts right in the shell so you see community installation live. Running npx skills add domesticmouse/skills. We pick teach-code from the repo and target Antigravity in our project workspace. Skill installed. Now we launch Antigravity. Inside Antigravity, we invoke /teach-code and ask it to turn our design specs, directory contracts, and Goldfish reports into a six-lesson interactive HTML course with runnable C99 companion apps. Watching thirty-nine raw minutes at seventy-nine times speed: it maps the learner's Zone of Proximal Development, builds six runnable C99 projects in tutorial/apps/0001 through 0006 under -Wall -Wextra -Werror -pedantic, crafts three live JavaScript simulators and four printable A4 bench sheets, and compiles the Tufte HTML pages. Now let's run sync_listing.py --check and build.py --check—confirming every C snippet in the Markdown is byte-for-byte identical to the compiled test binaries. Zero drift. Let's tour the live course!
-->

---
layout: default
---

<!-- S37 -->
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
Here is the live course landing page built right into this slide deck—click that top-right badge anytime to explore it. Clean Edward Tufte typography and a Figure 0.1 Mermaid arc taking a human engineer from Lesson 0001 (breadboard current budgets) through PWM math, Q4 EMA filtering, and out-of-tree NuttX BSP bring-up all the way to the 50 Hz bargraphd daemon in Lesson 0006—backed by deterministic C99 verification so tutorials never rot out of sync with code.
-->

---
layout: default
---

<!-- S38 -->
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
Inside the lessons, you play directly with both the physics and the exact Goldfish catches. In Lesson 0001 you drag the 0 to 3300 millivolt pot wiper and watch current draw update live; in Lesson 0002 you scrub 16-bit PWM channels. Best of all, in Lesson 0003's adc-ema-simulator you can toggle Pass 2's stale msgs[0] discard or flip the bit-shift bug and watch the bar graph peg at a quarter-turn! Pair that with Tufte sidenotes, interactive fill-in-the-blank C99 labs, and Matt's equal-word-count quizzes.
-->

---
layout: default
---

<!-- S39 -->
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
Two final surprises in the generated course. First, Lesson 0005 doesn't sweep our RP23XX_ADC_DIV false start under the rug—it teaches why START_ONCE ignores it, backed by four @media print A4 sheets you can tape beside your soldering iron. Second, the best twist of all: when /teach-code built Lesson 0006's strict C99 pthreads harness to teach the daemon state machine, that tutorial harness caught two real firmware bugs we missed in Phase 5—including bargraph test forgetting to enter MANUAL mode so the background loop clobbered the sweep twenty milliseconds later!
-->

---
layout: default
---

<!-- S40 -->
<div class="flex items-center gap-3 mb-6">
  <h2 class="text-2xl font-bold m-0">The &ldquo;Skills Three Ways&rdquo; Flywheel: Monday Morning Habits</h2>
</div>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 mt-4 leading-relaxed">
  <li v-click>
    <strong class="text-cyan-300">Way 1 &mdash; Adopt Multi-Agent Workflow Plugins (<code class="text-gray-200">/plugin</code> &rarr; <code class="text-white">egm</code>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Feed the Elephant directory <code class="text-gray-300">README.md</code> contracts (<code class="text-cyan-300">/egm-bootstrap</code>, <code class="text-cyan-300">/egm-drift-report</code>) and unleash stateless Goldfish reviewers (<code class="text-cyan-300">/egm-review</code> + <code class="text-cyan-300">/clear</code>) before writing code.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-amber-300">Way 2 &mdash; Orchestrate Curated Community Skills (<code class="text-gray-200">npx skills add mattpocock/skills</code>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Pin hashes in <code class="text-gray-300">skills-lock.json</code>, audit <code class="text-gray-300">.agents/skills/*/SKILL.md</code>, grill plans with <code class="text-amber-300">/grill-me</code>, and tame multi-session fog with <code class="text-amber-300">/wayfinder</code> + <code class="text-purple-300">/prototype</code>.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-emerald-300">Way 3 &mdash; Author &amp; Evolve Custom Modular Skills (<code class="text-gray-200">/agy-customizations</code> &rarr; <code class="text-white">domesticmouse/skills</code>)</strong>
    <ul class="list-disc pl-5 mt-1 text-gray-300">
      <li>Let <code class="text-cyan-300">/agy-customizations</code> + <code class="text-cyan-300">/writing-for-agents</code> structure your context pointers, completion criteria, and deterministic <code class="text-gray-300">scripts/</code>&mdash;and run <code class="text-emerald-300">/teach-code</code> so humans own the outcomes.</li>
    </ul>
  </li>
</ol>

<!--
That is the Skills Three Ways flywheel you can put to work on Monday morning. Way 1: adopt bundled multi-agent workflow plugins like egm so your agent maps directory contracts and audits specs with zero-loyalty Goldfish before writing code. Way 2: orchestrate curated community skills—pinning hashes in skills-lock.json, grilling decisions with /grill-me, and carving multi-session fog into /wayfinder tickets and /prototype HTML planners. Way 3: let meta-skills like /agy-customizations and /writing-for-agents help you author and evolve your own modular skills with deterministic scripts—and run /teach-code so you own every line your agent ships.
-->

---
layout: default
---

<!-- S41 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Links, Open-Source Skills, Interactive Course &amp; Prototypes</h2>
</div>

<div class="grid grid-cols-3 gap-3.5 mt-3">
  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800 space-y-1.5">
    <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Experiment Repo &amp; Decks</div>
    <div class="text-sm font-bold font-mono">
      <a href="https://github.com/domesticmouse/nuttxspace" target="_blank" class="text-cyan-300 hover:underline">github.com/domesticmouse/nuttxspace</a>
    </div>
    <p class="text-xs text-gray-400 m-0">
      7 casts, specs, Goldfish audits, both Slidev decks &amp; firmware
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800 space-y-1.5">
    <div class="text-xs font-mono text-cyan-400 uppercase tracking-wide">Way 1 &middot; EGM Plugin</div>
    <div class="text-sm font-bold font-mono text-white">
      /plugin &rarr; Discover &rarr; egm
    </div>
    <p class="text-xs text-gray-400 m-0">
      Credit: <strong class="text-gray-200">Dave Rensin</strong> (<code class="text-gray-300">@drensin</code>) &middot; Built-in Antigravity marketplace
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800 space-y-1.5">
    <div class="text-xs font-mono text-amber-400 uppercase tracking-wide">Way 2 &middot; Community Skills</div>
    <div class="text-sm font-bold font-mono text-amber-300">
      npx skills add mattpocock/skills
    </div>
    <p class="text-xs text-gray-400 m-0">
      Credit: <strong class="text-gray-200">Matt Pocock</strong> &middot; <code class="text-gray-300">/grill-me</code>, <code class="text-gray-300">/wayfinder</code>, <code class="text-gray-300">/prototype</code>, <code class="text-gray-300">/writing-for-agents</code>
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800 space-y-1.5">
    <div class="text-xs font-mono text-emerald-400 uppercase tracking-wide">Way 3 &middot; Custom Skills</div>
    <div class="text-sm font-bold font-mono text-emerald-300">
      npx skills add domesticmouse/skills
    </div>
    <p class="text-xs text-gray-400 m-0">
      <code class="text-gray-300">/teach-code</code>, <code class="text-gray-300">/asciinema</code>, <code class="text-gray-300">/cast-player-html</code>, <code class="text-gray-300">/slidev-deck</code>, <code class="text-gray-300">/bjm-voice</code>
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800 space-y-1.5">
    <div class="text-xs font-mono text-purple-400 uppercase tracking-wide">Live 6-Lesson Course</div>
    <div class="text-sm font-bold font-mono">
      <a href="./tutorial/index.html" target="_blank" class="text-purple-300 hover:underline">./tutorial/index.html &nearr;</a>
    </div>
    <p class="text-xs text-gray-400 m-0">
      Edward Tufte HTML, JS simulators, strict C99 labs &amp; A4 bench sheets
    </p>
  </div>

  <div class="p-4 rounded-xl bg-gray-900 border border-gray-800 space-y-1.5">
    <div class="text-xs font-mono text-purple-400 uppercase tracking-wide">Talk Planning Prototypes</div>
    <div class="text-xs font-mono space-y-1">
      <div><a href="./prototypes/cast-curation-prototype.html" target="_blank" class="text-purple-300 hover:underline">cast-curation-prototype.html &nearr;</a></div>
      <div><a href="./prototypes/slide-structure-prototype.html" target="_blank" class="text-amber-300 hover:underline">slide-structure-prototype.html &nearr;</a></div>
    </div>
    <p class="text-xs text-gray-400 m-0">
      Interactive HTML tools built via <code class="text-gray-300">/wayfinder</code> + <code class="text-gray-300">/prototype</code>
    </p>
  </div>
</div>

<!--
Everything you saw today—the nuttxspace repository, Dave Rensin's egm plugin, Matt Pocock's mattpocock/skills pack, my custom skills at domesticmouse/skills, the live six-lesson course, and the interactive prototypes that helped shape this talk—is open source at these links. Thanks heaps for coming along, and I'd love to take your questions!
-->
