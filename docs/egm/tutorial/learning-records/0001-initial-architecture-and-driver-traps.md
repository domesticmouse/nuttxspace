# Out-of-Tree RP2350 Hardware & NuttX Driver Constraints Established

The Seeed Studio XIAO RP2350 6-segment PWM LED bar graph requires seven deliberate hardware and driver mitigations to operate reliably without modifying the upstream `nuttx/` or `apps/` git submodules: enabling `CONFIG_BOARD_LATE_INITIALIZE=y`, populating both `channels[0]` (`channel=1`) and `channels[1]` (`channel=2`) in `struct pwm_info_s`, bounding PWM duty cycles to `0..65535` (`uint16_t`) instead of `b16ONE` (`65536`), zeroing PWM duties before `PWMIOC_STOP`, isolating `GPIO26` (`D0/A0`) against Erratum RP2350-E9 digital input buffer leakage (`OD=1`, `IE=0`, `PUE=0`, `PDE=0`, `ISO=0`), gating `RP23XX_ADC_IRQ_FIFO` in the NVIC around 5-sample burst reads (`>> 19` shift, discarding `msgs[0]`), and autostarting `bargraphd` during `rp23xx_bringup()` before `nsh_usbconsole` blocks waiting for a USB-C host connection.

## Implications

- Lessons 0001 through 0006 are sequenced so each lesson isolates and verifies one layer of this stack in a standalone, host-compilable C99 program (`tutorial/apps/0001-...` through `0006-...`) before culminating in the complete multi-threaded daemon, CLI, and NuttX UF2 firmware build.
