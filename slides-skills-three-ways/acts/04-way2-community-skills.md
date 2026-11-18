---
layout: default
---

<!-- S29 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">Way 2: Curated Community Skills (<code class="text-amber-300">npx skills add mattpocock/skills</code>)</h2>
  <span class="px-3 py-1 rounded-lg bg-amber-950/30 border border-amber-500/40 text-xs font-mono text-amber-300 font-bold">27 Skills &middot; SHA-256 Pinned</span>
</div>

<div class="space-y-4">
  <div class="grid grid-cols-2 gap-4 text-xs">
    <div class="p-4 rounded-xl bg-amber-950/15 border border-amber-500/30 space-y-1.5">
      <div class="font-mono text-amber-400 uppercase">Supply-Chain Hygiene (<code class="text-gray-200">skills-lock.json</code>)</div>
      <p class="text-gray-200 m-0 leading-relaxed">
        Installing <code class="text-amber-300">27</code> skills from <strong class="text-white">Matt Pocock&rsquo;s</strong> repo writes exact Git commit SHAs (<code class="text-gray-300">5863806dd4e1...</code>) and SHA-256 <code class="text-gray-300">computedHash</code> digests straight into version control.
      </p>
    </div>
    <div v-click class="p-4 rounded-xl bg-gray-900/80 border border-gray-800 space-y-1.5">
      <div class="font-mono text-emerald-400 uppercase">Inspect Before You Trust</div>
      <p class="text-gray-200 m-0 leading-relaxed">
        Every installed skill lands as plain Markdown under <code class="text-emerald-300">.agents/skills/&lt;name&gt;/SKILL.md</code>&mdash;auditable with <code class="text-gray-300">git diff</code> before an agent ever executes a line.
      </p>
    </div>
  </div>

  <div v-click>

```json
// skills-lock.json (excerpt — 27 mattpocock/skills + 7 domesticmouse/skills)
"wayfinder": {
  "source": "mattpocock/skills",
  "commit": "5863806dd4e15c63174bee4e7515a4256792ecfa",
  "skillPath": "skills/engineering/wayfinder/SKILL.md",
  "computedHash": "bd82983c105881f6cb0c15e5d029c0d5fca756939b69eebe94e49003069a276f"
}
```

  </div>
</div>

<!--
Unlike a heavyweight plugin, Way 2 is installing lightweight, single-purpose community skills via npx skills add—here, 27 skills from Matt Pocock's mattpocock/skills repository. Two things make this practical for real engineering teams. First, supply-chain hygiene: skills-lock.json pins the exact upstream commit hash and SHA-256 computedHash digest for every single skill. Second, readability: because skills live as plain Markdown files inside .agents/skills/, you can read and audit every prompt, subagent dispatch, and tool rule with git diff before you ever run it. Let's inspect two of Matt's best patterns under an X-ray.
-->

---
layout: skill-breakdown
way: 2
title: "Way 2 X-Ray: 7-Line Micro-Skills (<code class='text-amber-300'>/grill-me</code>) &amp; Router Skills (<code class='text-amber-300'>/wayfinder</code>)"
---

<!-- S30 -->
<div class="space-y-3">
  <div class="grid grid-cols-12 gap-4 items-start">
    <div class="col-span-5 space-y-2">
      <div class="text-xs font-mono text-amber-300 uppercase">Beat 1 &middot; 7-Line User-Invoked Entrypoint</div>
      <pre class="p-3.5 rounded-lg bg-[#1a1b26] border border-gray-800 text-xs font-mono leading-relaxed text-gray-200 overflow-x-auto m-0"><code><span class="text-gray-500">---</span>
<span class="text-cyan-300">name:</span> <span class="text-emerald-300">grill-me</span>
<span class="text-cyan-300">description:</span> <span class="text-amber-200">A relentless interview to sharpen a plan or design.</span>
<span class="text-cyan-300">disable-model-invocation:</span> <span class="text-purple-300">true</span>
<span class="text-gray-500">---</span>
<span></span>
Call the Skill tool with <span class="text-amber-200">"grilling"</span>.</code></pre>
      <p class="text-xs text-gray-300 m-0 leading-relaxed">
        <code class="text-amber-300">disable-model-invocation: true</code> costs <strong class="text-white">0</strong> always-on context tokens until typed; <code class="text-gray-200">/grilling</code> works a <strong class="text-white">design tree</strong> frontier in rounds with <code class="text-emerald-300">➡️</code> defaults and subagent lookups.
      </p>
    </div>
    <div v-click class="col-span-7 space-y-2.5">
      <div class="p-3.5 rounded-xl bg-gray-900/80 border border-amber-500/30 space-y-1.5">
        <div class="text-xs font-mono text-amber-400 uppercase">Beat 2 &middot; Router Skill (<code class="text-gray-200">/wayfinder</code>)</div>
        <p class="text-xs text-gray-200 m-0 leading-relaxed">
          When work exceeds one <code class="text-gray-300">100K</code>-token session, <code class="text-amber-300">/wayfinder</code> creates a low-res <strong class="text-white">shared map</strong> (<code class="text-gray-300">wayfinder:map</code>) and routes each decision ticket to the right skill:
          <code class="text-cyan-300">/grilling</code> + <code class="text-cyan-300">/domain-modeling</code> (HITL), <code class="text-purple-300">/prototype</code> (HITL), or <code class="text-emerald-300">/research</code> (AFK subagent).
        </p>
      </div>
      <div v-click class="p-3.5 rounded-xl bg-gray-900/80 border border-gray-800 space-y-1.5">
        <div class="text-xs font-mono text-cyan-400 uppercase">Beat 3 &middot; Fog of War Quarantine</div>
        <p class="text-xs text-gray-200 m-0 leading-relaxed">
          Sharp questions become child tickets; fuzzy downstream unknowns wait in <strong class="text-white">Not yet specified (Fog of war)</strong> until the frontier clears; distractions close into <strong class="text-amber-300">Out of scope</strong>. Max <strong class="text-white">1 ticket resolved per session</strong>.
        </p>
      </div>
    </div>
  </div>
</div>

<!--
Look at how tiny and composable community skills can be. On the left is the entire .agents/skills/grill-me/SKILL.md—literally seven lines! Setting disable-model-invocation: true means it costs zero context tokens until I type /grill-me, at which point it hands off to /grilling to walk a design tree in rounds, recommending an answer for every frontier question while dispatching subagents to look up facts from disk. On the right, /wayfinder acts as a router skill for problems too huge for a single 100K-token session. It keeps a low-resolution map, routes sharp decision tickets to /grilling, /prototype, or /research subagents, quarantines unready unknowns inside a "Fog of war" section, and enforces one ticket per session so context never rots.
-->

---
layout: default
---

<!-- S31 -->
<div class="flex items-center gap-3 mb-5">
  <h2 class="text-2xl font-bold m-0">Meta-Story: Using Skills to Make a Talk About Skills</h2>
</div>

<div class="p-4 rounded-xl bg-amber-950/15 border border-amber-500/30 mb-4">
  <div class="text-xs font-mono text-amber-400 uppercase tracking-wide">The Presentation Fog</div>
  <div class="text-base font-bold text-white mt-1">
    <code class="text-amber-300">7</code> raw casts (<code class="text-amber-300">228m+</code> / <code class="text-amber-300">25 MB</code> of ANSI PTY logs) + <code class="text-cyan-300">47</code> session traces (<code class="text-cyan-300">4,478</code> steps) &rarr; zero chance of fitting in one prompt.
  </div>
</div>

<ol class="space-y-3 text-xs text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white">Charted <code class="text-amber-300">.scratch/egm-nuttx-casts-presentation/map.md</code> via <code class="text-cyan-300">/wayfinder</code></strong>
    <ul class="list-disc pl-5 mt-1 space-y-0.5 text-gray-300">
      <li>Quarantined downstream slide pacing and script voice in the <strong class="text-white">Fog of war</strong> until cast curation settled; locked C syntax and tool internals into <strong class="text-amber-300">Out of scope</strong>.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white">Worked <code class="text-emerald-300">7</code> Single-Session Decision Tickets (<code class="text-gray-300">issues/01..07.md</code>)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li><code class="text-cyan-300">01</code> Research cast stop points &rarr; <code class="text-cyan-300">02</code> Grill hardware staging &rarr; <code class="text-purple-300">03</code> Prototype cast curation (<code class="text-emerald-300">10m 24s</code> warped, <code class="text-emerald-300">53</code> stops) &rarr; <code class="text-cyan-300">04</code> Grill EGM beats.</li>
      <li><code class="text-purple-300">05</code> Prototype slide pacing &rarr; <code class="text-cyan-300">06</code> Script verbatim <code class="text-gray-200">bjm-voice</code> (<code class="text-gray-300">~140 wpm</code>) &rarr; <code class="text-emerald-300">07</code> Assemble &amp; verify <code class="text-gray-200">spec.md</code>.</li>
    </ul>
  </li>
</ol>

<!--
Here is the fun meta-story: I didn't just use skills to build the firmware—I used Way 2 skills to build this entire talk! After the hardware experiment wrapped, I was sitting on seven raw terminal recordings spanning over 228 minutes and 25 megabytes of ANSI logs, plus 47 Antigravity traces. Dumping 25 megabytes into one prompt fails instantly. So I ran /wayfinder. It charted map.md, parked slide timings in the Fog of war while we audited the recordings, and marched us through seven single-session decision tickets—from research ticket 01 all the way to verified spec 07.
-->

---
layout: default
---

<!-- S32 -->
<div class="flex items-center justify-between mb-5">
  <h2 class="text-2xl font-bold m-0">High-Fidelity Decisions with <code class="text-purple-300">/prototype</code> (Interactive Planners)</h2>
  <div class="flex items-center gap-2 text-xs font-mono">
    <a href="./prototypes/cast-curation-prototype.html" target="_blank" class="px-2.5 py-1 rounded bg-purple-950/40 border border-purple-400/50 text-purple-200 hover:bg-purple-900/50">Cast Curation Matrix &nearr;</a>
    <a href="./prototypes/slide-structure-prototype.html" target="_blank" class="px-2.5 py-1 rounded bg-amber-950/40 border border-amber-400/50 text-amber-200 hover:bg-amber-900/50">Slide Timing Planner &nearr;</a>
  </div>
</div>

<p class="text-xs text-gray-400 m-0 mb-4">
  <code class="text-purple-300">.agents/skills/prototype/SKILL.md</code>: &ldquo;A prototype is <strong>throwaway code that answers a question</strong>&rdquo; &mdash; rendered as single shareable HTML files during <code class="text-gray-300">issues/03</code> &amp; <code class="text-gray-300">issues/05</code>:
</p>

<ol class="space-y-4 text-sm text-gray-200 list-decimal pl-6 leading-relaxed">
  <li v-click>
    <strong class="text-white"><code class="text-purple-300">cast-curation-prototype.html</code> (<code class="text-gray-300">issues/03</code>)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Interactive segment matrix contrasting <code class="text-gray-300">228m+</code> raw vs. <code class="text-emerald-300">10m 24s</code> adaptive-speed playback (<code class="text-gray-300">&ge; 2x</code>, <code class="text-gray-300">&le; 30s</code> cap), paired Before/After stop points (<code class="text-emerald-300">53</code> stops), and middle-turn trims.</li>
    </ul>
  </li>
  <li v-click>
    <strong class="text-white"><code class="text-amber-300">slide-structure-prototype.html</code> (<code class="text-gray-300">issues/05</code>)</strong>
    <ul class="list-disc pl-5 mt-1 space-y-1 text-gray-300">
      <li>Interactive pacing dashboard balancing <code class="text-gray-300">v-click</code> reveals (<code class="text-gray-300">~2.1</code> beats/slide), live cast durations, and spoken word budgets (<code class="text-gray-300">~140 wpm</code>) before writing a single Vue slide.</li>
    </ul>
  </li>
</ol>

<!--
Two of those Wayfinder tickets hit questions that static Markdown couldn't answer: "Which of 53 terminal stop points feel crisp on stage?" and "Does our pacing actually fit?" That is where Matt's /prototype skill shines—throwaway single-file HTML widgets built purely to sharpen human judgement. You can pop both live prototypes open right from those badges in the corner: cast-curation-prototype.html lets us compare raw versus adaptive-speed segment cuts across all seven casts, and slide-structure-prototype.html budgets every v-click beat and word count before touching Slidev. Now—once a community workflow proves itself, how do you turn those lessons into your own reusable skills? Welcome to Way 3.
-->
