# `xiao-rp2350-bargraph/board` Out-of-Tree Board Support Package

## Overview

Out-of-tree custom board support package (`CONFIG_ARCH_BOARD_CUSTOM=y`) for the Seeed Studio XIAO RP2350 6-segment PWM LED bar graph. Integrates with `nuttx/boards/arm/rp23xx/common` (`CONFIG_ARCH_BOARD_COMMON=y`) to register `/dev/adc0` and `/dev/pwm0,2,3,5,6`, apply RP2350-E9 analog pad isolation on `GPIO26`, and autostart `bargraphd` during `board_late_initialize()`.

## Subdirectories & Boundaries

- `configs`: [./configs/README.md](./configs/README.md) - Board build configuration directory containing the `bargraph` target `defconfig`.
- `include`: [./include/README.md](./include/README.md) - Public board header (`board.h`) linked to `nuttx/include/arch/board` during configuration.
- `scripts`: [./scripts/README.md](./scripts/README.md) - Top-level board `Make.defs` (including the `apps/external` symlink hook) and RP2350 QSPI flash/SRAM linker script (`memmap_default.ld`).
- `src`: [./src/README.md](./src/README.md) - Board-specific initialization (`rp23xx_boardinitialize.c`) and peripheral bringup (`rp23xx_bringup.c`) compiled into `libboard.a`.

## Invariants

- `[Kconfig:ARCH_BOARD_XIAO_RP2350_BARGRAPH]` Defines `ARCH_BOARD_XIAO_RP2350_BARGRAPH` (`default y`) under `if ARCH_BOARD_CUSTOM` so `CUSTOM_BOARD_KPATH = $(BOARD_DIR)/Kconfig` in `nuttx/tools/Config.mk` resolves cleanly without falling back to `dummy_kconfig`.
- `[configs/bargraph/defconfig:CONFIG_ARCH_BOARD_COMMON]` Requires `CONFIG_ARCH_BOARD_COMMON=y` so `nuttx/boards/arm/rp23xx/common` provides `rp23xx_common_earlyinitialize()`, `rp23xx_common_initialize()`, `rp23xx_common_bringup()`, and `rp23xx_pwmdev_initialize()`.

## Side Effects

- `[scripts/Make.defs:ARCHSCRIPT]` Configures the ARMv8-M toolchain flags, links `apps/external -> ../xiao-rp2350-bargraph/apps` during non-clean targets, and supplies `memmap_default.ld` to the NuttX linker.
- `[src/rp23xx_bringup.c:rp23xx_bringup]` Registers `/dev/adc0` and `/dev/pwm0,2,3,5,6`, clears `IE`/`PUE`/`PDE`/`ISO` and sets `OD` on `RP23XX_PADS_BANK0_GPIO(26)`, and launches `bargraphd` via `bargraph_autostart()`.

## Verification

- Run: `make -C xiao-rp2350-bargraph`
