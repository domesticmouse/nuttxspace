# Seeed Studio XIAO RP2350 & Apache NuttX Bar Graph Resources

## Knowledge

- [Raspberry Pi RP2350 Datasheet](https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf)
  Official silicon reference for the dual Cortex-M33 RP2350A microcontroller. Use for: GPIO function multiplexing, 8-slice / 2-channel hardware PWM counter-compare (`CC`) registers, 12-bit SAR ADC conversion timing (`96` clock cycles at `48 MHz`), `PADS_BANK0` register bitfields, and Erratum `RP2350-E9` digital input buffer leakage on analog pads.
- [Seeed Studio XIAO RP2350 Wiki & Pinout Specification](https://wiki.seeedstudio.com/getting_started_with_xiao_rp2350/)
  Official board pinout, power rail topology (`3V3` LDO output vs `5V` VBUS), and BOOTSEL UF2 mass-storage flashing guide. Use for: mapping XIAO header pins `D0`–`D6` (`Rows 5E–11E`) and power pins (`Rows 5F–7F`) to RP2350 GPIO numbers (`GPIO26`, `GPIO27`, `GPIO28`, `GPIO5`, `GPIO6`, `GPIO7`, `GPIO0`).
- [Apache NuttX RTOS Official Documentation](https://nuttx.apache.org/docs/latest/)
  Authoritative guide to the Apache NuttX upper-half/lower-half device driver architecture, Kconfig build system, `task_create()` background daemons, and NuttShell (`nsh`) built-in applications (`PROGNAME`, `PRIORITY`, `STACKSIZE`). Use for: POSIX file descriptor `ioctl()` contracts on `/dev/adc0` (`<nuttx/analog/adc.h>`) and `/dev/pwm*` (`<nuttx/timers/pwm.h>`).
- [Workspace Hardware & Breadboard Wiring Guide (`xiao-rp2350-bargraph/BREADBOARD_WIRING.md`)](https://github.com/apache/nuttx)
  Coordinate-by-coordinate 400-tie-point breadboard assembly guide, Bill of Materials, and electrical current budget (`3.94 mA` per LED, `23.6 mA` total) in this repository. Use for: verifying physical jumper coordinates and resistor/LED polarity.
- [Workspace Architecture & Driver Trap Specification (`docs/designs/xiao-rp2350-bargraph.md`)](https://github.com/apache/nuttx)
  Complete engineering design specification documenting the seven upstream RP23xx NuttX driver behaviors (`rp23xx_pwm.c` and `rp23xx_adc.c`) and their out-of-tree mitigations. Use for: cross-checking register bitmasks, Kconfig flags, and state-machine invariants.

## Wisdom (Communities)

- [Apache NuttX GitHub Discussions & Mailing List](https://github.com/apache/nuttx/discussions)
  Official forum monitored by Apache NuttX PMC members and kernel maintainers. Use for: discussing upper/lower-half driver contracts, out-of-tree board bringup patterns (`CONFIG_ARCH_BOARD_CUSTOM`), and RP23xx architecture support.
- [Raspberry Pi Microcontroller Forums (RP2040 / RP2350)](https://forums.raspberrypi.com/viewforum.php?f=143)
  High-signal hardware and bare-metal engineering forum with Raspberry Pi silicon engineers. Use for: deep-dives into RP2350 ADC front-end impedance, Erratum RP2350-E9 mitigation, and PWM slice counter timing.
