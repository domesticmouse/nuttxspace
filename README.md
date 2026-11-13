# NuttX Workspace (`nuttxspace`)

## Overview
Top-level NuttX workspace repository coordinating the Apache NuttX RTOS kernel (`nuttx`) and the companion user-space applications repository (`apps`) as Git submodules. Configures out-of-tree/workspace builds where `nuttx` boots the kernel, scheduler, VFS, drivers, and networking stack, and `apps` provides the NuttShell (`nshlib`), system utilities, examples, and test suites linked into the NuttX image.

## Subdirectories & Boundaries
- `apps`: [./apps/README.md](./apps/README.md) - NuttX user-space application repository containing NuttShell (`nshlib`), built-in system utilities (`system`), networking/graphics/filesystem services, and example/testing applications.
- `nuttx`: [./nuttx/README.md](./nuttx/README.md) - Apache NuttX RTOS kernel, architecture HALs (`arch`), board support packages (`boards`), drivers (`drivers`), filesystems (`fs`), C/C++/math libraries (`libs`), memory manager (`mm`), and networking stack (`net`).

## Global Invariants
- `nuttx` and `apps` submodule checkouts in `.gitmodules` must stay compatible at matching upstream/EGM branch revisions (`nuttx/Makefile` and `apps/Makefile` resolve `APPDIR=../apps` relative to `nuttx`).
- Portable kernel interfaces under `nuttx/include` and application user-space interfaces under `apps/include` must remain cleanly separated across kernel/user boundaries.

## Global Side Effects
- Running `tools/configure.sh` or `make` inside `nuttx` generates build configuration state (`.config`, `Make.defs`, `include/nuttx/config.h`) and compiles artifacts across both `nuttx` and `apps`.

## System Verification
- Run: `make -C nuttx test`
