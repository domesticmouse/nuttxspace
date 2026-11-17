(function () {
  "use strict";

  function initMermaidAndPrism() {
    if (window.mermaid && typeof window.mermaid.initialize === "function") {
      window.mermaid.initialize({
        startOnLoad: true,
        theme: "dark",
        securityLevel: "loose"
      });
    }
    if (window.Prism && typeof window.Prism.highlightAll === "function") {
      window.Prism.highlightAll();
    }
  }

  function initScrollablePreFocus() {
    var pres = document.querySelectorAll(".code-figure pre");
    pres.forEach(function (pre) {
      if (!pre.hasAttribute("tabindex")) {
        pre.setAttribute("tabindex", "0");
      }
    });
  }

  function initSidenotes() {
    var triggers = document.querySelectorAll(".sidenote-number");
    triggers.forEach(function (btn) {
      function toggle() {
        var wrapper = btn.closest(".sidenote-wrapper");
        if (!wrapper) return;
        var note = wrapper.querySelector(".sidenote");
        if (!note) return;
        var open = note.classList.toggle("is-open");
        btn.setAttribute("aria-expanded", open ? "true" : "false");
      }
      btn.addEventListener("click", toggle);
      btn.addEventListener("keydown", function (e) {
        if (e.key === "Enter") {
          toggle();
        } else if (e.key === " ") {
          e.preventDefault();
        }
      });
      btn.addEventListener("keyup", function (e) {
        if (e.key === " ") {
          toggle();
        }
      });
    });
  }

  function normalizeCodeToken(str) {
    return String(str || "")
      .trim()
      .replace(/\s+/g, " ");
  }

  function initExercises() {
    var cards = document.querySelectorAll(".exercise-card");
    cards.forEach(function (card) {
      var exId = card.id;
      var feedback = document.getElementById(exId + "-feedback");
      var checkBtn = card.querySelector(".exercise-check-btn");
      var hintBtn = card.querySelector(".exercise-hint-btn");
      var resetBtn = card.querySelector(".exercise-reset-btn");

      function checkAll() {
        var inputs = card.querySelectorAll(".code-blank");
        var allCorrect = true;
        var firstErrorHint = "";

        inputs.forEach(function (inp) {
          var rawExpected = inp.getAttribute("data-answer") || "";
          var alternatives = rawExpected.split("||").map(normalizeCodeToken);
          var userVal = normalizeCodeToken(inp.value);
          var matched = alternatives.indexOf(userVal) !== -1;

          inp.classList.remove("is-correct", "is-wrong");
          if (matched) {
            inp.classList.add("is-correct");
          } else {
            inp.classList.add("is-wrong");
            allCorrect = false;
            if (!firstErrorHint) {
              firstErrorHint = inp.getAttribute("data-hint") || "Check this blank.";
            }
          }
        });

        if (feedback) {
          feedback.hidden = false;
          feedback.classList.remove("is-success", "is-error");
          if (allCorrect) {
            feedback.classList.add("is-success");
            feedback.textContent = card.getAttribute("data-success") || "All blanks verified!";
          } else {
            feedback.classList.add("is-error");
            feedback.textContent = "Not quite: " + firstErrorHint;
          }
        }
      }

      if (checkBtn) {
        checkBtn.addEventListener("click", checkAll);
      }
      if (hintBtn) {
        hintBtn.addEventListener("click", function () {
          if (!feedback) return;
          feedback.hidden = false;
          feedback.classList.remove("is-success", "is-error");
          feedback.textContent = "Hint: " + (card.getAttribute("data-hint") || "");
        });
      }
      if (resetBtn) {
        resetBtn.addEventListener("click", function () {
          var inputs = card.querySelectorAll(".code-blank");
          inputs.forEach(function (inp) {
            inp.value = "";
            inp.classList.remove("is-correct", "is-wrong");
          });
          if (feedback) {
            feedback.hidden = true;
            feedback.textContent = "";
          }
        });
      }
    });
  }

  function initQuizzes() {
    var questions = document.querySelectorAll(".quiz-question");
    questions.forEach(function (qEl) {
      var feedback = document.getElementById(qEl.id + "-feedback");
      var wrongHint = qEl.getAttribute("data-wrong-hint") || "";
      var buttons = qEl.querySelectorAll(".quiz-option");

      buttons.forEach(function (btn) {
        btn.addEventListener("click", function () {
          buttons.forEach(function (b) {
            b.classList.remove("is-correct", "is-wrong");
          });
          var isCorrect = btn.getAttribute("data-correct") === "true";
          var explain = btn.getAttribute("data-explain") || "";
          btn.classList.add(isCorrect ? "is-correct" : "is-wrong");

          if (feedback) {
            feedback.hidden = false;
            feedback.classList.remove("is-success", "is-error");
            if (isCorrect) {
              feedback.classList.add("is-success");
              feedback.textContent = "Correct! " + explain;
            } else {
              feedback.classList.add("is-error");
              feedback.textContent = explain + (wrongHint ? " (" + wrongHint + ")" : "");
            }
          }
        });
      });
    });
  }

  function initVerifyChecklists() {
    var boxes = document.querySelectorAll(".verify-checkbox");
    boxes.forEach(function (box) {
      var key = "nuttx_bargraph_verify_" + (box.getAttribute("data-verify-id") || box.id);
      try {
        if (window.localStorage.getItem(key) === "1") {
          box.checked = true;
        }
      } catch (e) {}
      box.addEventListener("change", function () {
        try {
          window.localStorage.setItem(key, box.checked ? "1" : "0");
        } catch (e) {}
      });
    });
  }

  function computeDuties(totalUnits) {
    var DUTY_MAX = 65535;
    var TOTAL_MAX = 6 * DUTY_MAX;
    if (totalUnits > TOTAL_MAX) totalUnits = TOTAL_MAX;
    if (totalUnits < 0) totalUnits = 0;
    var duties = [];
    for (var i = 0; i < 6; i++) {
      var start = i * DUTY_MAX;
      var end = start + DUTY_MAX;
      if (totalUnits >= end) {
        duties.push(DUTY_MAX);
      } else if (totalUnits > start) {
        duties.push(totalUnits - start);
      } else {
        duties.push(0);
      }
    }
    return duties;
  }

  function renderBreadboardSimulator(container) {
    container.innerHTML =
      '<div class="sim-header">' +
      '<h4 class="sim-title">Interactive Breadboard &amp; Electrical Current Budget Simulator</h4>' +
      '<span class="card-badge">D0/A0 (GPIO26) + D1..D6 (330 Ω Active-High LEDs)</span>' +
      "</div>" +
      '<div class="sim-controls">' +
      '<label for="sim-bb-adc">Potentiometer Wiper (12-bit ADC 0..4095): ' +
      '<input type="range" id="sim-bb-adc" min="0" max="4095" value="2048">' +
      "</label>" +
      '<span id="sim-bb-adc-readout" class="sim-metric-val">2048</span>' +
      "</div>" +
      '<div class="sim-metrics-grid">' +
      '<div class="sim-metric-box"><span class="sim-metric-label">Wiper Voltage</span><span class="sim-metric-val" id="sim-bb-mv">1650 mV</span></div>' +
      '<div class="sim-metric-box"><span class="sim-metric-label">Normalized Level</span><span class="sim-metric-val" id="sim-bb-pct">50.0%</span></div>' +
      '<div class="sim-metric-box"><span class="sim-metric-label">Thevenin R_S (D0)</span><span class="sim-metric-val" id="sim-bb-rs">2.50 kΩ</span></div>' +
      '<div class="sim-metric-box"><span class="sim-metric-label">Total 6-LED Current</span><span class="sim-metric-val" id="sim-bb-curr">11.82 mA</span></div>' +
      "</div>" +
      '<div class="sim-led-strip" id="sim-bb-leds"></div>';

    var slider = container.querySelector("#sim-bb-adc");
    var readout = container.querySelector("#sim-bb-adc-readout");
    var mvEl = container.querySelector("#sim-bb-mv");
    var pctEl = container.querySelector("#sim-bb-pct");
    var rsEl = container.querySelector("#sim-bb-rs");
    var currEl = container.querySelector("#sim-bb-curr");
    var ledsContainer = container.querySelector("#sim-bb-leds");

    var ledMeta = [
      { name: "LED1", pin: "D1 / GPIO27", slice: "PWM5B", color: "#34d399" },
      { name: "LED2", pin: "D2 / GPIO28", slice: "PWM6A", color: "#34d399" },
      { name: "LED3", pin: "D3 / GPIO5", slice: "PWM2B", color: "#fbbf24" },
      { name: "LED4", pin: "D4 / GPIO6", slice: "PWM3A", color: "#fbbf24" },
      { name: "LED5", pin: "D5 / GPIO7", slice: "PWM3B", color: "#f87171" },
      { name: "LED6", pin: "D6 / GPIO0", slice: "PWM0A", color: "#f87171" }
    ];

    function update() {
      var adc = parseInt(slider.value, 10) || 0;
      readout.textContent = String(adc);
      var mv = Math.floor((adc * 3300 + 2047) / 4095);
      var permille = Math.floor((adc * 1000 + 2047) / 4095);
      var frac = adc / 4095;
      var rsKohm = 10.0 * frac * (1.0 - frac);
      var totalUnits = Math.floor((adc * 393210 + 2047) / 4095);
      var duties = computeDuties(totalUnits);

      var totalCurrentMa = 0;
      var cellsHtml = "";
      for (var i = 0; i < 6; i++) {
        var duty = duties[i];
        var dutyRatio = duty / 65535;
        var ledMa = 3.939393 * dutyRatio;
        totalCurrentMa += ledMa;
        var pct = Math.floor((duty * 100 + 32767) / 65535);
        var glow = duty > 0 ? "0 0 " + Math.round(6 + 14 * dutyRatio) + "px " + ledMeta[i].color : "none";
        var bg = duty > 0 ? ledMeta[i].color : "#1e293b";
        cellsHtml +=
          '<div class="sim-led-cell">' +
          '<div class="sim-led-bulb" style="background:' + bg + ";opacity:" + (0.25 + 0.75 * dutyRatio).toFixed(2) + ";box-shadow:" + glow + ';"></div>' +
          "<strong>" + ledMeta[i].name + "</strong>" +
          '<div class="sim-led-meta">' + ledMeta[i].pin + "</div>" +
          '<div class="sim-led-meta">' + ledMeta[i].slice + "</div>" +
          '<div class="sim-led-meta">' + pct + "% (" + ledMa.toFixed(2) + " mA)</div>" +
          "</div>";
      }

      mvEl.textContent = mv + " mV";
      pctEl.textContent = Math.floor(permille / 10) + "." + (permille % 10) + "%";
      rsEl.textContent = rsKohm.toFixed(2) + " kΩ";
      currEl.textContent = totalCurrentMa.toFixed(2) + " mA";
      ledsContainer.innerHTML = cellsHtml;
    }

    slider.addEventListener("input", update);
    update();
  }

  function renderPwmSegmentVisualizer(container) {
    container.innerHTML =
      '<div class="sim-header">' +
      '<h4 class="sim-title">Interactive 6-Segment PWM Duty &amp; uint16_t Overflow Explorer</h4>' +
      '<span class="card-badge">BARGRAPH_DUTY_MAX = 65535 vs b16ONE = 65536</span>' +
      "</div>" +
      '<div class="sim-controls">' +
      '<label for="sim-pwm-units">Total Bar Graph Units (0..393210): ' +
      '<input type="range" id="sim-pwm-units" min="0" max="393210" step="255" value="208955">' +
      "</label>" +
      '<span id="sim-pwm-units-val" class="sim-metric-val">208955</span>' +
      "</div>" +
      '<div class="sim-led-strip" id="sim-pwm-cells"></div>';

    var slider = container.querySelector("#sim-pwm-units");
    var valEl = container.querySelector("#sim-pwm-units-val");
    var cellsEl = container.querySelector("#sim-pwm-cells");

    function update() {
      var units = parseInt(slider.value, 10) || 0;
      valEl.textContent = units + " / 393210";
      var duties = computeDuties(units);
      var htmlStr = "";
      for (var i = 0; i < 6; i++) {
        var d = duties[i];
        var pct = Math.floor((d * 100 + 32767) / 65535);
        var b16Bug = d === 65535 ? "0 (WRAP BUG!)" : d + " (OK)";
        htmlStr +=
          '<div class="sim-led-cell">' +
          "<strong>Segment " + (i + 1) + "</strong>" +
          '<div class="sim-led-meta">Range: ' + i * 65535 + ".." + (i + 1) * 65535 + "</div>" +
          '<div class="sim-led-meta" style="color:#38bdf8;font-weight:700;">Safe: ' + d + " (" + pct + "%)</div>" +
          '<div class="sim-led-meta">If 65536: ' + b16Bug + "</div>" +
          "</div>";
      }
      cellsEl.innerHTML = htmlStr;
    }

    slider.addEventListener("input", update);
    update();
  }

  function renderAdcEmaSimulator(container) {
    container.innerHTML =
      '<div class="sim-header">' +
      '<h4 class="sim-title">Interactive Q4 EMA Filter &amp; Endpoint Deadband Simulator</h4>' +
      '<span class="card-badge">alpha = 1/4 • Deadbands &lt;= 32 and &gt;= 4063</span>' +
      "</div>" +
      '<div class="sim-controls">' +
      '<label for="sim-ema-raw">Target Raw ADC Input (0..4095): ' +
      '<input type="range" id="sim-ema-raw" min="0" max="4095" value="25">' +
      "</label>" +
      '<button type="button" class="btn-primary" id="sim-ema-step-btn">Step 1 Tick (20 ms)</button>' +
      '<button type="button" class="btn-secondary" id="sim-ema-reset-btn">Reset Cold Start</button>' +
      "</div>" +
      '<div class="sim-metrics-grid">' +
      '<div class="sim-metric-box"><span class="sim-metric-label">Raw Burst Avg</span><span class="sim-metric-val" id="sim-ema-raw-val">25</span></div>' +
      '<div class="sim-metric-box"><span class="sim-metric-label">Accumulator (ema_q4)</span><span class="sim-metric-val" id="sim-ema-acc-val">400</span></div>' +
      '<div class="sim-metric-box"><span class="sim-metric-label">Unclamped Rounded</span><span class="sim-metric-val" id="sim-ema-unclamped-val">25</span></div>' +
      '<div class="sim-metric-box"><span class="sim-metric-label">Filtered Output</span><span class="sim-metric-val" id="sim-ema-out-val">0 (Deadband Low)</span></div>' +
      "</div>";

    var slider = container.querySelector("#sim-ema-raw");
    var stepBtn = container.querySelector("#sim-ema-step-btn");
    var resetBtn = container.querySelector("#sim-ema-reset-btn");
    var rawValEl = container.querySelector("#sim-ema-raw-val");
    var accValEl = container.querySelector("#sim-ema-acc-val");
    var unclampedEl = container.querySelector("#sim-ema-unclamped-val");
    var outValEl = container.querySelector("#sim-ema-out-val");

    var emaQ4 = 0;
    var initialized = false;

    function stepFilter() {
      var raw = parseInt(slider.value, 10) || 0;
      if (raw > 4095) raw = 4095;
      if (!initialized) {
        emaQ4 = raw << 4;
        initialized = true;
      } else {
        emaQ4 = emaQ4 - (emaQ4 >> 2) + (raw << 2);
      }
      var rounded = (emaQ4 + 8) >> 4;
      var clamped = rounded;
      var note = "";
      if (clamped <= 32) {
        clamped = 0;
        note = " (Clamped <= 32)";
      } else if (clamped >= 4063) {
        clamped = 4095;
        note = " (Clamped >= 4063)";
      }
      rawValEl.textContent = String(raw);
      accValEl.textContent = String(emaQ4);
      unclampedEl.textContent = String(rounded);
      outValEl.textContent = String(clamped) + note;
    }

    slider.addEventListener("input", function () {
      rawValEl.textContent = slider.value;
    });
    stepBtn.addEventListener("click", stepFilter);
    resetBtn.addEventListener("click", function () {
      initialized = false;
      emaQ4 = 0;
      stepFilter();
    });
    stepFilter();
  }

  function initSimulators() {
    var sims = document.querySelectorAll(".interactive-simulator");
    sims.forEach(function (el) {
      var kind = el.getAttribute("data-simulator");
      if (kind === "breadboard-simulator") {
        renderBreadboardSimulator(el);
      } else if (kind === "pwm-segment-visualizer") {
        renderPwmSegmentVisualizer(el);
      } else if (kind === "adc-ema-simulator") {
        renderAdcEmaSimulator(el);
      }
    });
  }

  document.addEventListener("DOMContentLoaded", function () {
    initMermaidAndPrism();
    initScrollablePreFocus();
    initSidenotes();
    initExercises();
    initQuizzes();
    initVerifyChecklists();
    initSimulators();
  });
})();
