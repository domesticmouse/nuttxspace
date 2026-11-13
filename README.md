# NuttX Workspace (`nuttxspace`)

## Overview
Top-level NuttX workspace repository coordinating the Apache NuttX RTOS kernel (`nuttx`), the companion user-space applications repository (`apps`), out-of-tree target firmware (`xiao-rp2350-bargraph`), and design documentation (`docs`). Configures workspace builds where `nuttx` boots the kernel, scheduler, VFS, drivers, and networking stack, `apps` provides the NuttShell (`nshlib`), system utilities, examples, and test suites, and `xiao-rp2350-bargraph` supplies an out-of-tree custom board and 20 Hz potentiometer LED bar-graph daemon for the Seeed Studio XIAO RP2350.

## Subdirectories & Boundaries
- `apps`: [./apps/README.md](./apps/README.md) - NuttX user-space application repository containing NuttShell (`nshlib`), built-in system utilities (`system`), networking/graphics/filesystem services, and example/testing applications.
- `docs`: [./docs/designs/xiao-rp2350-bargraph.md](./docs/designs/xiao-rp2350-bargraph.md) - System architecture and implementation design documents (`docs/designs/xiao-rp2350-bargraph.md`).
- `nuttx`: [./nuttx/README.md](./nuttx/README.md) - Apache NuttX RTOS kernel, architecture HALs (`arch`), board support packages (`boards`), drivers (`drivers`), filesystems (`fs`), C/C++/math libraries (`libs`), memory manager (`mm`), and networking stack (`net`).
- `xiao-rp2350-bargraph`: [./xiao-rp2350-bargraph/Makefile](./xiao-rp2350-bargraph/Makefile) - Out-of-tree XIAO RP2350 potentiometer-to-6-LED bar-graph project containing custom board support (`board/`), the `bargraph` NSH daemon (`external/bargraph/`), and host unit tests (`tests/test_bargraph_logic.c`).

## Global Invariants
- `nuttx` and `apps` submodule checkouts in `.gitmodules` must stay compatible at matching upstream/EGM branch revisions (`nuttx/Makefile` and `apps/Makefile` resolve `APPDIR=../apps` relative to `nuttx`).
- Portable kernel interfaces under `nuttx/include` and application user-space interfaces under `apps/include` must remain cleanly separated across kernel/user boundaries.
- Tracked files inside `nuttx/` and `apps/` remain unmodified by out-of-tree projects; `xiao-rp2350-bargraph` integrates via `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"` and the gitignored `apps/external` symlink (`../xiao-rp2350-bargraph/external`).
- `xiao-rp2350-bargraph/common/` must not exist so `CONFIG_ARCH_BOARD_COMMON=y` resolves `BOARD_COMMON_DIR` to `nuttx/boards/arm/rp23xx/common`.

## Global Side Effects
- Running `tools/configure.sh` or `make` inside `nuttx` generates build configuration state (`.config`, `Make.defs`, `include/nuttx/config.h`) and compiles artifacts across both `nuttx` and `apps`.
- Running `make -C xiao-rp2350-bargraph setup`, `configure`, or `all` creates the gitignored symlink `apps/external -> ../xiao-rp2350-bargraph/external`, generates composite `Kconfig` files (`xiao-rp2350-bargraph/external/Kconfig`, `apps/Kconfig`), and stages ROMFS `etctmp*` files in `nuttx/boards/arm/rp23xx/common/`; running `make -C xiao-rp2350-bargraph distclean` removes these generated artifacts.

## System Verification
- Host unit tests (`xiao-rp2350-bargraph` quantizer, decoder, and LED bitmask logic): `make -C xiao-rp2350-bargraph test && make -C xiao-rp2350-bargraph clean`
- Full `xiao-rp2350-bargraph` firmware build (requires `arm-none-eabi-gcc`, `kconfig-frontends`, `genromfs`, `xxd`, and `picotool` in `$PATH`): `make -C xiao-rp2350-bargraph`
- Clean out-of-tree build and symlink state: `make -C xiao-rp2350-bargraph distclean`
