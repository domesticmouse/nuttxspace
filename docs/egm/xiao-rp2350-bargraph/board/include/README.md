# `xiao-rp2350-bargraph/board/include` Public Board Header (`board/include`)

## Overview

Public board header directory linked to `nuttx/include/arch/board` during configuration. Exports RP2350 system/peripheral clock frequencies (`150 MHz` system/peripheral, `48 MHz` USB/ADC), the on-board `GPIO25` LED pin, the `BARGRAPH_ADC_GPIO` (`26`) pad isolation constant, and board initialization prototypes.

## Interface

- `[board.h:BOARD_SYS_FREQ]` System and peripheral clock frequency (`150 MHz`).
- `[board.h:BOARD_ADC_FREQ]` ADC clock frequency (`48 MHz`).
- `[board.h:BOARD_GPIO_LED_PIN]` On-board status LED pin (`GPIO25`).
- `[board.h:BARGRAPH_ADC_GPIO]` Potentiometer wiper GPIO pin (`26`, `D0/A0` / `ADC0`).
- `[board.h:rp23xx_boardearlyinitialize]` Early board initialization prototype `void rp23xx_boardearlyinitialize(void)`.
- `[board.h:rp23xx_boardinitialize]` Standard board initialization prototype `void rp23xx_boardinitialize(void)`.

## Invariants

- `[board.h:BOARD_NGPIOOUT]` Sets `BOARD_NGPIOOUT 0`, `BOARD_NGPIOIN 0`, and `BOARD_NGPIOINT 0` so `GPIO26`–`GPIO28` are dedicated to `ADC0`, `PWM5B`, and `PWM6A` rather than generic `/dev/gpio*` nodes.
- `[board.h:BARGRAPH_ADC_GPIO]` Defines `BARGRAPH_ADC_GPIO 26` for the `RP23XX_PADS_BANK0_GPIO(BARGRAPH_ADC_GPIO)` RP2350-E9 pad isolation fixup in `rp23xx_bringup.c`, while keeping PWM and ADC channel routing in `defconfig` as the single source of truth.

## Side Effects

- None (header-only declarations and preprocessor constants).

## Verification

- Run: `make -C xiao-rp2350-bargraph`
