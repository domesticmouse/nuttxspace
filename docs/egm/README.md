# nuttxspace-egm Blueprint

## System Overview

`nuttxspace-egm` is the top-level Apache NuttX workspace superproject coordinating the side-by-side git submodules defined in `[.gitmodules]`: the real-time operating system kernel (`nuttx`) and the user-space application repository (`apps`). Placing `nuttx/` and `apps/` as sibling directories at the workspace root satisfies the default cross-repository build discovery contract (`CONFIG_APPS_DIR="../apps"` in Make/Kconfig and `NUTTX_APPS_DIR="${NUTTX_DIR}/../apps"` in CMake) without requiring out-of-tree path overrides.

## Subdirectories & System Boundaries

- `apps`: [./apps/README.md](../../apps/README.md) - User-space application repository providing built-in application registration (`builtin`), the NuttShell (`nshlib`) command interpreter, system utilities (`system`), networking daemons and libraries (`netutils`), filesystem utilities (`fsutils`), graphics toolkits (`graphics`), industrial control stacks (`industry`), scripting interpreters and WebAssembly runtimes (`interpreters`), cryptographic libraries (`crypto`), audio/video utilities (`audioutils`, `videoutils`), wireless/LTE stacks (`wireless`, `lte`), machine learning (`mlearning`), SDR libraries (`sdr`), bootloaders (`boot`), benchmarks (`benchmarks`), and reference examples (`examples`).
- `nuttx`: [./nuttx/README.md](../../nuttx/README.md) - Apache NuttX Real-Time Operating System (RTOS) kernel, standard C/C++/math/DSP/graphics libraries (`libs`), public POSIX and kernel headers (`include`), processor hardware abstraction layers (`arch`), board support packages (`boards`), task scheduler and OS core (`sched`), memory management (`mm`), virtual and storage filesystems (`fs`), kernel networking stack (`net`), upper/lower-half device drivers (`drivers`), system call tables (`syscall`), binary format loaders (`binfmt`), cryptographic subsystem (`crypto`), audio/video/graphics/wireless frameworks (`audio`, `video`, `graphics`, `wireless`), Sphinx documentation (`Documentation`), and host configuration/build tools (`tools`).

## Global Invariants

- Sibling Submodule Layout: `[.gitmodules]` maps the `nuttx` and `apps` submodules as direct sibling directories at the workspace root so `[nuttx/Kconfig:APPS_DIR]`, `[nuttx/tools/Unix.mk:CONFIG_APPS_DIR]`, and `[nuttx/CMakeLists.txt:NUTTX_APPS_DIR]` resolve `../apps` automatically from `nuttx/`.
- Cross-Repository Build Contract: When `APPDIR` resolves to `../apps`, `nuttx` delegates `preconfig`, `context`, `depend`, `all` (`libapps.a`), `clean`, and `distclean` targets to `[apps/Makefile]` with `TOPDIR` set to the `nuttx` root `[nuttx/Kconfig:APPS_DIR]`, `[nuttx/tools/Unix.mk:APPDIR]`, `[nuttx/tools/LibTargets.mk:libapps]`.
- Kernel/User-Space Separation: Portable kernel subsystems in `nuttx/` (`sched`, `mm`, `fs`, `net`, `drivers`) and architecture/board layers (`arch`, `boards`) enforce the boundary rules in `[nuttx/INVIOLABLES.md]`, while user-space applications in `apps/` interact with the OS through public headers in `[nuttx/include/README.md]`, `[nuttx/syscall/syscall.csv:g_stublookup]`, `[nuttx/boards/boardctl.c:boardctl]`, and the built-in application table `[apps/builtin/builtin_list.c:g_builtins]`.
- Monotonic Kernel Bootstrap: Kernel initialization in `[nuttx/sched/init/nx_start.c:nx_start]` advances `g_nx_initstate` monotonically through `OSINIT_BOOT`, `OSINIT_TASKLISTS`, `OSINIT_MEMORY`, `OSINIT_HARDWARE`, `OSINIT_OSREADY`, and `OSINIT_IDLELOOP` before spawning the initial user-space entry task (such as `nsh_main` in `apps/system/nsh`).

## Global Side Effects

- Board Configuration Generation: Running `nuttx/tools/configure.sh <board>:<config>` (or CMake configuration) generates `.config`, `Make.defs`, and `include/nuttx/config.h` inside `nuttx/`, and invokes `[apps/tools/mkkconfig.sh]` via `preconfig` to synthesize `apps/Kconfig` across all `apps/` subdirectories `[nuttx/tools/Unix.mk:preconfig]`, `[apps/Makefile:Kconfig]`.
- Cross-Submodule Archive Staging & Linking: Building `nuttx` compiles configured `apps/` directories into `apps/libapps.a`, populates `apps/builtin/registry/` `.bdat`/`.pdat` descriptors during the `context`/`register` phase, and installs `libapps.a` into `nuttx/staging/libapps.a` for flat (`NUTTXLIBS`) or protected (`USERLIBS`) linking `[nuttx/tools/LibTargets.mk:staging/libapps.a]`, `[nuttx/tools/FlatLibs.mk:NUTTXLIBS]`, `[nuttx/tools/ProtectedLibs.mk:USERLIBS]`.
- Export Bundling: Running `make export` in `nuttx/` packages kernel headers, startup objects, libraries, and `apps/` exported includes and built-in registry descriptors (`EXPORTDIR/include`, `EXPORTDIR/registry`) into a redistributable export archive `[nuttx/tools/Unix.mk:MKEXPORT_ARGS]`, `[apps/Makefile:export]`.

## System Verification

- Run: `make -C nuttx test`
