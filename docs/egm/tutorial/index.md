---
title: "Building the Seeed Studio XIAO RP2350 6-Segment PWM LED Bar Graph"
page_title: "Course Index — XIAO RP2350 6-Segment PWM LED Bar Graph on Apache NuttX"
nav_id: "index"
badge: "Interactive Hardware & Embedded RTOS Story Arc"
subtitle: "A step-by-step hands-on engineering course teaching you how to wire the 400-tie-point breadboard circuit and build the out-of-tree Apache NuttX board support package, 50 Hz ADC-to-PWM daemon, and NSH CLI from scratch."
---

## The Story Arc: From Breadboard Physics to an Autonomous RTOS Daemon

Building a proportional 6-LED analog bar graph on the **Seeed Studio XIAO RP2350** under **Apache NuttX** without modifying a single file inside the upstream `nuttx/` or `apps/` git submodules is a masterclass in embedded systems engineering. Over six progressive lessons, you will assemble the physical circuit, uncover seven real-world silicon errata and upstream driver traps, and grow a clean, host-verified C99 codebase from hardware constants into a complete multi-threaded RTOS daemon (`bargraphd`) and interactive NuttShell (`nsh`) command (`bargraph`).

```mermaid caption="Figure 0.1: Six-Lesson Story Arc from Breadboard Circuitry to Out-of-Tree NuttX Firmware"
flowchart LR
  L1["0001: Breadboard Circuit\n& 23.6 mA Budget"] --> L2["0002: 6-Segment Math\n& 16-Bit Duty Scaling"]
  L2 --> L3["0003: ADC >> 19 Shift\n& Q4 EMA Filter"]
  L3 --> L4["0004: Multi-Channel PWM\n& Safe Stop Ordering"]
  L4 --> L5["0005: Out-of-Tree BSP,\nRP2350-E9 & IRQ Gating"]
  L5 --> L6["0006: 50 Hz bargraphd\nState Machine & NSH CLI"]
```

## Interactive Lessons

Work through the lessons in order. Each lesson includes an interactive circuit or firmware simulator, block-by-block C99 walkthroughs, a complete runnable companion project in `tutorial/apps/<NNNN-slug>/`, a fill-in-the-blank coding lab, an equal-option retrieval quiz, and an automated terminal verification gate.

{{widget:lesson-cards}}

## Printable Reference Sheets & Engineering Guides

Keep these reference sheets open (or print them on paper via your browser's print dialog) while wiring your breadboard and debugging on hardware:

{{widget:reference-cards}}
