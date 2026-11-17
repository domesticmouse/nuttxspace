---
title: "Glossary of XIAO RP2350 & Apache NuttX Terms"
page_title: "Glossary — XIAO RP2350 6-Segment PWM LED Bar Graph"
nav_id: "glossary"
nav_label: "Glossary"
badge: "Mission: Out-of-Tree RP2350 Bar Graph"
subtitle: "Canonical definitions used across all lessons and reference sheets. Designed for quick screen lookup and clean printing."
card:
  title: "**Glossary of XIAO RP2350 & NuttX Terms**"
  summary: "Canonical definitions for RP2350 ADC/PWM hardware slices, Erratum RP2350-E9 pad isolation, Q4 fixed-point filtering, and Apache NuttX out-of-tree driver boundaries."
---

## 1. Hardware & Breadboard Circuitry

**Seeed Studio XIAO RP2350**:
A compact 14-pin dual-inline microcontroller module powered by the Raspberry Pi RP2350A dual Cortex-M33 (`150 MHz`) processor, exposing analog/digital pins `D0`–`D6` on the left header (`Rows 5E–11E`) and power/GPIO pins `5V`, `GND`, `3V3`, and `D10`–`D7` on the right header (`Rows 5F–11F`).
_Avoid_: Pico 2 board, generic RP2040 module

**Linear Rotary Potentiometer (`VR1`)**:
A three-terminal `10 kΩ` linear-taper (`B10K`) resistive voltage divider wired between `+3.3 V` (`Lug 1`), `D0/A0` (`Lug 2` wiper, `GPIO26`), and `GND` (`Lug 3`) that converts shaft angle into a continuous `0.0 V – 3.3 V` analog input with maximum Thevenin source impedance `R_S,max = 2.5 kΩ` at mid-scale.
_Avoid_: audio/logarithmic pot, rotary encoder

**Active-High LED Segment (`LED1`–`LED6`)**:
One of six discrete through-hole light-emitting diodes driven from an RP2350 GPIO pin through a `330 Ω` series current-limiting resistor to the LED anode (`+`) with the cathode (`-`) tied to `GND`, drawing `≈ 3.94 mA` at `100%` PWM duty (`3.3 V` supply, `V_F ≈ 2.0 V`).
_Avoid_: open-drain active-low sink, Charlieplexed matrix

**Erratum RP2350-E9 Pad Isolation**:
The hardware register configuration on `RP23XX_PADS_BANK0_GPIO(26)` that clears Input Enable (`IE=0`), Pull-Up Enable (`PUE=0`), Pull-Down Enable (`PDE=0`), and Pad Isolation (`ISO=0`) while setting Output Disable (`OD=1`) so the RP2350 digital input buffer cannot leak current into the analog potentiometer node.
_Avoid_: default GPIO pull configuration

## 2. RP2350 Peripherals & Signal Processing

**PWM Slice**:
One of eight independent 16-bit hardware timer-counter blocks (`PWM0`–`PWM7`) in the RP2350, each capable of driving two output compare channels (`Channel A` and `Channel B`) sharing a common period (`TOP`) and clock divider (`1000 Hz`).
_Avoid_: independent PWM timer per pin

**16-Bit Safe PWM Duty (`BARGRAPH_DUTY_MAX`)**:
The maximum 16-bit unsigned integer duty cycle `65535` (`0xFFFF`) passed to `rp23xx_pwm.c`, preventing the silent `uint16_t` truncation wraparound to `0` (`0%`) that occurs when passing the standard NuttX fixed-point constant `b16ONE` (`65536` / `0x00010000`).
_Avoid_: `b16ONE` (`65536`) full-scale duty

**Total Bar Graph Units (`BARGRAPH_TOTAL_UNITS_MAX`)**:
The unified 6-segment brightness scale `393210` (`6 * 65535`) across which `bargraph_compute_duties()` partitions the full input range so each LED segment occupies a contiguous `65535`-unit window.
_Avoid_: floating-point percentage buckets

**Q4 Exponential Moving Average (`ema_q4`)**:
A multiplication-free fixed-point first-order low-pass filter (`alpha = 1/4`) that stores its accumulator scaled by `2^4 = 16`, updating each tick via `ema_q4 = ema_q4 - (ema_q4 >> 2) + (raw_avg << 2)` and rounding back to 12-bit integers via `(ema_q4 + 8) >> 4`.
_Avoid_: floating-point IIR filter, unrounded bit-shift truncation

**Endpoint Deadband (`BARGRAPH_ADC_DEADBAND_LOW` / `HIGH`)**:
Symmetric saturation thresholds at the bottom (`<= 32` clamped to `0`) and top (`>= 4063` clamped to `4095`) of the 12-bit ADC range that absorb mechanical potentiometer end-stop resistance and contact bounce.
_Avoid_: raw unclamped ADC endpoints

## 3. Apache NuttX Out-of-Tree Architecture & Drivers

**Out-of-Tree Custom Board (`CONFIG_ARCH_BOARD_CUSTOM`)**:
An Apache NuttX board support package residing outside the `nuttx/` repository (`xiao-rp2350-bargraph/board/`) that reuses `nuttx/boards/arm/rp23xx/common` while keeping `git -C nuttx status --porcelain` completely empty.
_Avoid_: in-tree `nuttx/boards/arm/rp23xx/` fork

**External Application Symlink (`apps/external`)**:
The git-ignored symbolic link `apps/external -> ../xiao-rp2350-bargraph/apps` created automatically by `board/scripts/Make.defs` during configuration and build so `apps/Makefile` and `mkkconfig.sh` discover out-of-tree built-in applications without modifying `apps/`.
_Avoid_: editing `apps/examples/` in-tree

**Demand-Driven NVIC IRQ Gating**:
The interrupt-control technique where `bargraphd` keeps `RP23XX_ADC_IRQ_FIFO` masked via `up_disable_irq()` while sleeping and unmasks it via `up_enable_irq()` only during a 5-sample burst `read()` after `ANIOC_RESET_FIFO`, defeating the `500,000 IRQ/s` self-retriggering `START_ONCE` interrupt storm in `rp23xx_adc.c`.
_Avoid_: leaving `/dev/adc0` IRQ unmasked continuously

**`bargraphd` Background Daemon**:
The autonomous kernel-spawned user task launched from `rp23xx_bringup()` during `board_late_initialize()` (before `nsh_usbconsole` waits for a USB-C host) that runs the 50 Hz (`20 ms`) `SENSING`, `MANUAL`, and `TEST` control loop.
_Avoid_: foreground NSH startup script
