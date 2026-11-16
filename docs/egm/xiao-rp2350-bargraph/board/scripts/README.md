# `xiao-rp2350-bargraph/board/scripts` Build & Linker Scripts (`board/scripts`)

## Overview

Provides the top-level board `Make.defs` fragment (copied to `nuttx/Make.defs` during configuration) and the RP2350 2 MB QSPI flash / 520 KB SRAM linker script (`memmap_default.ld`).

## Interface

- `[Make.defs:LDSCRIPT]` Selects `memmap_default.ld` and appends `$(BOARD_DIR)/scripts/$(LDSCRIPT)` to `ARCHSCRIPT`.
- `[Make.defs:apps/external]` `distclean`-guarded `$(shell ...)` hook that creates `apps/external -> ../xiao-rp2350-bargraph/apps` before `apps/Make.defs` discovers application subdirectories.
- `[memmap_default.ld:MEMORY]` Defines `FLASH` (`0x10000000`, `2048k`), `RAM` (`0x20000000`, `512k`), `SCRATCH_X` (`0x20080000`, `4k`), and `SCRATCH_Y` (`0x20081000`, `4k`), plus the RP2350 bootrom Picobin `IMAGE_DEF` block.

## Invariants

- `[Make.defs:apps/external]` The symlink creation hook is guarded by `ifeq ($(filter distclean clean_context apps_distclean,$(MAKECMDGOALS)),)` so running `make -C nuttx distclean` removes `apps/external` without recreating it during `clean_context`.

## Side Effects

- `[Make.defs:apps/external]` Creates the git-ignored symlink `$(TOPDIR)/../apps/external -> ../xiao-rp2350-bargraph/apps` when `Make.defs` is evaluated by non-clean Make goals.

## Verification

- Run: `make -C xiao-rp2350-bargraph`
