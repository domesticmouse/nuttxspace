# `xiao-rp2350-bargraph/board/configs/bargraph` Configuration (`board/configs/bargraph`)

## Overview

Leaf NuttX `defconfig` profile for the Seeed Studio XIAO RP2350 6-segment hardware PWM LED bar graph (`xiao-rp2350-bargraph:bargraph`). Configures the RP2350 kernel, USB CDC/ACM console, single-channel `ADC0` potentiometer input, five 2-channel hardware PWM slices (`PWM0`, `PWM2`, `PWM3`, `PWM5`, `PWM6`), UF2 firmware packaging, and the `bargraph` built-in application.

## Interface

- `[defconfig:CONFIG_ARCH_BOARD_CUSTOM]` Selects out-of-tree custom board routing (`CONFIG_ARCH_BOARD_CUSTOM_NAME="xiao-rp2350-bargraph"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y`, `CONFIG_ARCH_BOARD_COMMON=y`).
- `[defconfig:CONFIG_BOARD_LATE_INITIALIZE]` Enables `board_late_initialize()` so `rp23xx_bringup()` runs before `nsh_main`.
- `[defconfig:CONFIG_CDCACM_CONSOLE]` Configures NuttShell (`nsh_main`) over USB-C CDC/ACM (`/dev/ttyACM0`).
- `[defconfig:CONFIG_PWM_NCHANNELS]` Sets `CONFIG_PWM=y` and `CONFIG_PWM_NCHANNELS=2` alongside `[defconfig:CONFIG_RP23XX_PWM_MULTICHAN]` (`CONFIG_RP23XX_PWM_NCHANNELS=2`) to satisfy both upper-half and RP23xx C preprocessor guards.
- `[defconfig:CONFIG_RP23XX_ADC_CHANNEL0]` Enables `/dev/adc0` on `ADC0` (`GPIO26`) only.
- `[defconfig:CONFIG_BARGRAPH]` Enables the `bargraph` built-in application (`CONFIG_BARGRAPH_PRIORITY=100`, `CONFIG_BARGRAPH_STACKSIZE=2048`).

## Invariants

- `[defconfig:CONFIG_RP23XX_ADC_CHANNEL0]` Only `CONFIG_RP23XX_ADC_CHANNEL0=y` is enabled (`ADC_CHANNEL1..3` and `ADC_TEMPERATURE` remain unset) so `get_next_channel()` in `rp23xx_adc.c` never multiplexes non-potentiometer channels into `/dev/adc0`.
- `[defconfig:CONFIG_PWM_NCHANNELS]` Unused sibling PWM pins (`PWM0B_GPIO`, `PWM2A_GPIO`, `PWM5A_GPIO`, `PWM6B_GPIO`) are set to `-1` so `pwm_setup()` in `rp23xx_pwm.c` only muxes the six active LED GPIOs (`GPIO27`, `GPIO28`, `GPIO5`, `GPIO6`, `GPIO7`, `GPIO0`).
- `[defconfig:CONFIG_NFILE_DESCRIPTORS_PER_BLOCK]` Sets `CONFIG_NFILE_DESCRIPTORS_PER_BLOCK=12` so `bargraphd` can hold 9 open file descriptors (`0..2`, `/dev/adc0`, and 5 `/dev/pwm*` slices) in a single descriptor block.

## Side Effects

- `[defconfig:CONFIG_RP23XX_UF2_BINARY]` Enables `CONFIG_RAW_BINARY=y` and `CONFIG_RP23XX_UF2_BINARY=y` so building `nuttx` produces `nuttx/nuttx.bin` and `nuttx/nuttx.uf2` via `picotool`.

## Verification

- Run: `make -C xiao-rp2350-bargraph`
