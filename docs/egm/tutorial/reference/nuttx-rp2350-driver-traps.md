---
title: "7 Upstream NuttX & RP2350 Hardware/Driver Traps"
page_title: "NuttX & RP2350 Driver Traps — XIAO RP2350 Bar Graph"
nav_id: "ref-traps"
nav_label: "Driver Traps"
badge: "Reference Sheet • Architecture & Errata"
subtitle: "Summary of the seven concrete RP2350 silicon and Apache NuttX driver traps uncovered in nuttx/arch/arm/src/rp23xx and their exact out-of-tree mitigations."
card:
  title: "**7 NuttX & RP2350 Hardware/Driver Traps**"
  summary: "Quick-lookup table of all 7 upstream RP23xx ADC/PWM/USB driver traps and how our out-of-tree BSP and daemon mitigate them without touching nuttx/ or apps/."
---

## 1. The 7 Hardware & Driver Traps at a Glance

Table: Upstream RP23xx Driver Traps and Out-of-Tree Mitigations

| # | Subsystem & File | Symptom if Unmitigated | Root Cause in Hardware / Upstream Driver | Out-of-Tree Mitigation in `xiao-rp2350-bargraph/` |
| :--- | :--- | :--- | :--- | :--- |
| **1** | Board Bringup (`xiao-rp2350:usbnsh`) | `/dev/adc0` and `/dev/pwm*` do not exist in `/dev` | Upstream `usbnsh/defconfig` omits `CONFIG_BOARD_LATE_INITIALIZE=y`, compiling out `board_late_initialize()` | Enable `CONFIG_BOARD_LATE_INITIALIZE=y` in `board/configs/bargraph/defconfig` and call `rp23xx_bringup()` |
| **2** | USB Console (`nsh_usbconsole.c`) | LEDs remain dark on standalone USB power until a PC opens `/dev/ttyACM0` and sends 3 newlines | `nsh_consolemain()` blocks inside `nsh_waitusbready()` before running NSH startup scripts | Spawn `bargraphd` via `bargraph_autostart()` at the end of `rp23xx_bringup()` before `nsh_main` starts |
| **3** | Multi-Channel PWM (`rp23xx_pwm.c` & `pwm.c`) | Channel B LEDs (`D1/PWM5B`, `D3/PWM2B`, `D5/PWM3B`) never light or fail `ioctl` | `rp23xx_pwm.c` reads `channels[0]` as Ch A and `channels[1]` as Ch B, while `drivers/timers/pwm.c` stops scanning if `channel == 0` | Always set `channels[0].channel = 1` (`duty_a`) and `channels[1].channel = 2` (`duty_b`) on every slice update |
| **4** | PWM Duty Scaling (`rp23xx_pwm.c`) | LEDs turn completely OFF (`0%`) the instant they reach `100%` brightness | `struct rp23xx_pwm_lowerhalf_s` stores `duty` as `uint16_t`; passing `b16ONE` (`65536` / `0x10000`) truncates to `0x0000` | Clamp and scale segment duties to `BARGRAPH_DUTY_MAX = 65535u` (`0xFFFF`) |
| **5** | PWM Stop (`rp23xx_pwm.c`) | Running `bargraph stop` leaves random LEDs frozen ON | `pwm_stop()` clears `RP23XX_PWM_CSR_EN` without driving the GPIO pad low while the descriptor is open | Write `duty_a = 0, duty_b = 0` via `PWMIOC_SETCHARACTERISTICS` before calling `PWMIOC_STOP` and `close()` |
| **6** | ADC Pad Mux & Erratum `RP2350-E9` (`rp23xx_adc.c`) | Mid-scale potentiometer voltage sags/sticks; `D1`/`D2` PWM mux wiped if `ADC_CHANNEL1..2` enabled | `my_reset()` resets all enabled ADC GPIOs, and `GPIO26` leaves digital input buffer `IE=1` enabled (Erratum `RP2350-E9`) | Enable only `CONFIG_RP23XX_ADC_CHANNEL0=y` and run `modreg32(OD, IE|PUE|PDE|ISO, RP23XX_PADS_BANK0_GPIO(26))` |
| **7** | ADC `START_ONCE` IRQ Storm & Shift (`rp23xx_adc.c`) | `500,000 IRQ/s` starves the CPU, FIFO returns stale samples, and raw values are shifted left by 19 bits | `interrupt_handler()` re-triggers `START_ONCE` (`2 µs`) unconditionally and executes `value <<= 19` | Mask `RP23XX_ADC_IRQ_FIFO` via `up_disable_irq()`, flush with `ANIOC_RESET_FIFO`, unmask for a 5-sample burst (discarding `msgs[0]`), and shift `(am_data >> 19) & 0x0fff` |

## 2. Key Register & Data-Structure Contracts

### Erratum RP2350-E9 Pad Isolation (`board/src/rp23xx_bringup.c`)

```c caption="Isolating GPIO26 (D0/A0) digital pad buffer in rp23xx_bringup()"
modreg32(RP23XX_PADS_BANK0_GPIO_OD,
         RP23XX_PADS_BANK0_GPIO_IE |
         RP23XX_PADS_BANK0_GPIO_PUE |
         RP23XX_PADS_BANK0_GPIO_PDE |
         RP23XX_PADS_BANK0_GPIO_ISO,
         RP23XX_PADS_BANK0_GPIO(BARGRAPH_ADC_GPIO));
```

### Dual-Channel `pwm_info_s` Population (`apps/bargraph/bargraph_main.c`)

```c caption="Populating both Channel A (channels[0]) and Channel B (channels[1])"
memset(&info, 0, sizeof(info));
info.frequency           = BARGRAPH_PWM_FREQ_HZ;
info.channels[0].channel = 1;
info.channels[0].duty    = (ub16_t)duty_a;
info.channels[1].channel = 2;
info.channels[1].duty    = (ub16_t)duty_b;
```
