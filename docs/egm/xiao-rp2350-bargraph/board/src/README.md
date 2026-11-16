# `xiao-rp2350-bargraph/board/src` Board Source Implementation (`board/src`)

## Overview

Board-level C implementation (`src`) for `xiao-rp2350-bargraph` (`arm/rp23xx`). Provides early GPIO initialization (`rp23xx_boardearlyinitialize()`), standard board initialization (`rp23xx_boardinitialize()`), late board bringup (`board_late_initialize()` -> `rp23xx_bringup()`), RP2350-E9 analog pad isolation on `GPIO26`, and autonomous startup of `bargraphd` (`bargraph_autostart()`).

## Interface

- `[Make.defs:CSRCS]` Registers `rp23xx_boardinitialize.c` and `rp23xx_bringup.c` into `libboard.a`.
- `[rp23xx_pico.h:GPIO_LED1]` On-board status LED GPIO pin macro (`25`).
- `[rp23xx_pico.h:rp23xx_bringup]` Board peripheral bringup declaration `int rp23xx_bringup(void)`.
- `[rp23xx_pico.h:bargraph_autostart]` Built-in bargraph daemon autostart declaration `int bargraph_autostart(void)`.
- `[rp23xx_boardinitialize.c:rp23xx_boardearlyinitialize]` Early board initialization `void rp23xx_boardearlyinitialize(void)`.
- `[rp23xx_boardinitialize.c:rp23xx_boardinitialize]` Standard board initialization `void rp23xx_boardinitialize(void)`.
- `[rp23xx_boardinitialize.c:board_late_initialize]` Late board initialization hook `void board_late_initialize(void)` (enabled by `CONFIG_BOARD_LATE_INITIALIZE=y`).
- `[rp23xx_bringup.c:rp23xx_bringup]` Peripheral registration, `GPIO26` pad isolation, and daemon launch `int rp23xx_bringup(void)`.

## Invariants

- `[rp23xx_bringup.c:rp23xx_bringup]` `rp23xx_common_bringup()` executes before modifying `RP23XX_PADS_BANK0_GPIO(BARGRAPH_ADC_GPIO)` and before calling `bargraph_autostart()`, ensuring `/dev/adc0` and `/dev/pwm0,2,3,5,6` are registered before `bargraphd` opens them.
- `[rp23xx_bringup.c:rp23xx_bringup]` `modreg32()` clears `RP23XX_PADS_BANK0_GPIO_IE`, `PUE`, `PDE`, and `ISO` and sets `RP23XX_PADS_BANK0_GPIO_OD` on `GPIO26` to eliminate RP2350-E9 digital input buffer leakage on the potentiometer wiper input.

## Side Effects

- `[rp23xx_boardinitialize.c:rp23xx_boardearlyinitialize]` Initializes `BOARD_GPIO_LED_PIN` (`GPIO25`) as an output driven high.
- `[rp23xx_bringup.c:rp23xx_bringup]` Registers `/dev/adc0` and `/dev/pwm0,2,3,5,6` via `rp23xx_common_bringup()`, updates `RP23XX_PADS_BANK0_GPIO(26)`, and spawns `bargraphd` via `bargraph_autostart()`.

## Verification

- Run: `make -C xiao-rp2350-bargraph`
