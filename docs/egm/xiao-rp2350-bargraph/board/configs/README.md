# `xiao-rp2350-bargraph/board/configs` Board Configurations Index

## Overview

Contains the NuttX build configuration profiles for the out-of-tree `xiao-rp2350-bargraph` board support package.

## Subdirectories & Boundaries

- `bargraph`: [./bargraph/README.md](./bargraph/README.md) - Default configuration (`defconfig`) enabling USB CDC/ACM NuttShell (`usbnsh`), `ADC0` on `GPIO26`, 5 dual-channel PWM slices on `D1`–`D6`, UF2 binary generation, and the `bargraph` built-in application.

## Invariants

- `[bargraph/defconfig:CONFIG_ARCH_BOARD_CUSTOM]` Every configuration under `configs/` must set `CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`, and `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y` instead of `CONFIG_ARCH_BOARD_XIAO_RP2350=y`.

## Side Effects

- Passing `../xiao-rp2350-bargraph/board/configs/bargraph` to `nuttx/tools/configure.sh` populates `nuttx/.config` and `nuttx/Make.defs`.

## Verification

- Run: `cd nuttx && ./tools/configure.sh ../xiao-rp2350-bargraph/board/configs/bargraph`
