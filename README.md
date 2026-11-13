# NuttX Workspace (`nuttxspace`)

## Overview
Top-level NuttX workspace repository coordinating the Apache NuttX RTOS kernel (`nuttx`), the companion user-space applications repository (`apps`), out-of-tree target firmware (`xiao-rp2350-bargraph`), and design documentation (`docs`). Configures workspace builds where `nuttx` boots the kernel, scheduler, VFS, drivers, and networking stack, `apps` provides the NuttShell (`nshlib`), system utilities, examples, and test suites, and `xiao-rp2350-bargraph` supplies an out-of-tree custom board and 20 Hz potentiometer LED bar-graph daemon for the Seeed Studio XIAO RP2350.

## Subdirectories & Boundaries
- `apps`: [./apps/README.md](./apps/README.md) - NuttX user-space application repository containing NuttShell (`nshlib`), built-in system utilities (`system`), networking/graphics/filesystem services, and example/testing applications.
- `docs`: [./docs/designs/xiao-rp2350-bargraph.md](./docs/designs/xiao-rp2350-bargraph.md) - System architecture and implementation design documents (`docs/designs/xiao-rp2350-bargraph.md`), plus software-engineer tutorials for the firmware ([./docs/tutorials/xiao-rp2350-bargraph.md](./docs/tutorials/xiao-rp2350-bargraph.md)) and replaying the EGM workflow ([./docs/tutorials/egm-replication-tutorial.md](./docs/tutorials/egm-replication-tutorial.md)).
- `nuttx`: [./nuttx/README.md](./nuttx/README.md) - Apache NuttX RTOS kernel, architecture HALs (`arch`), board support packages (`boards`), drivers (`drivers`), filesystems (`fs`), C/C++/math libraries (`libs`), memory manager (`mm`), and networking stack (`net`).
- `xiao-rp2350-bargraph`: [./xiao-rp2350-bargraph/Makefile](./xiao-rp2350-bargraph/Makefile) - Out-of-tree XIAO RP2350 potentiometer-to-6-LED bar-graph project containing custom board support (`board/`), the `bargraph` NSH daemon (`external/bargraph/`), and host unit tests (`tests/test_bargraph_logic.c`).

## Global Invariants
- `nuttx` and `apps` submodule checkouts in `.gitmodules` must stay compatible at matching upstream/EGM branch revisions (`nuttx/Makefile` and `apps/Makefile` resolve `APPDIR=../apps` relative to `nuttx`).
- Portable kernel interfaces under `nuttx/include` and application user-space interfaces under `apps/include` must remain cleanly separated across kernel/user boundaries.
- Tracked files inside `nuttx/` and `apps/` remain unmodified by out-of-tree projects; `xiao-rp2350-bargraph` integrates via `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"` and the gitignored `apps/external` symlink (`../xiao-rp2350-bargraph/external`).
- `xiao-rp2350-bargraph/common/` must not exist so `CONFIG_ARCH_BOARD_COMMON=y` resolves `BOARD_COMMON_DIR` to `nuttx/boards/arm/rp23xx/common`.

## Global Side Effects
- Running `tools/configure.sh` or `make` inside `nuttx` generates build configuration state (`.config`, `Make.defs`, `include/nuttx/config.h`) and compiles artifacts across both `nuttx` and `apps`.
- Running `make -C xiao-rp2350-bargraph setup`, `configure`, or `all` creates the gitignored symlink `apps/external -> ../xiao-rp2350-bargraph/external`, generates composite `Kconfig` files (`xiao-rp2350-bargraph/external/Kconfig`, `apps/Kconfig`), stages ROMFS `etctmp*` files in `nuttx/boards/arm/rp23xx/common/`, and produces `nuttx/nuttx` (ELF) and `nuttx/nuttx.uf2` (RP2350 UF2 image via `picotool`) alongside `nuttx/nuttx.manifest`; running `make -C xiao-rp2350-bargraph distclean` removes these generated artifacts.

## System Verification
- **Host Prerequisites (Fedora 44)** (see [./TODO.md](./TODO.md)):
  - Host build and packaging tools: `gcc`, `make`, `bison`, `flex`, `gperf`, `gettext`, `texinfo`, `ncurses-devel`, `genromfs`, `xxd`, `cmake`, `libusb1-devel`, and `picocom`.
  - Kconfig frontends: `kconfig-conf`, `kconfig-tweak`, and `kconfig-mconf`.
  - ARM bare-metal cross-compiler toolchain: `arm-none-eabi-gcc-cs`, `arm-none-eabi-gcc-cs-c++`, `arm-none-eabi-binutils-cs`, and `arm-none-eabi-newlib` (`arm-none-eabi-gcc` `15.2.0-4.fc44`).
  - RP2350 UF2 packaging and flashing tool: `picotool` (`v2.3.1` in `/usr/local/bin/picotool`).
- **Host Unit Tests**:
  - `make -C xiao-rp2350-bargraph test` compiles and runs `xiao-rp2350-bargraph/tests/test_bargraph_logic`, verifying `decode_adc_sample()`, `quantize_level()` with `±24`-count hysteresis, and `level_to_led_mask()`.
- **Firmware Build**:
  - `make -C xiao-rp2350-bargraph` configures `xiao-rp2350-bargraph/board/configs/bargraph/defconfig`, builds `nuttx/nuttx`, and converts it to `nuttx/nuttx.uf2` (verified footprint: `FLASH: 117320 B / 2 MB (5.59%)`, `RAM: 15096 B / 512 KB (2.88%)`).
- **Hardware Pinout & Flashing (Seeed Studio XIAO RP2350)**:
  - Potentiometer wiper on `D0` (`GPIO 26` / `/dev/adc0`, `0–3.3 V`) and 6 active-high LEDs with `>= 330 Ω` series resistors on `D3–D5` (`GPIO 5, 6, 7`) and `D8–D10` (`GPIO 2, 4, 3`) mapped to `/dev/userleds` bits `0..5` (`0x01..0x20`).
  - Flash in `BOOTSEL` mode via `picotool load -f nuttx/nuttx.uf2 -x` or by copying `nuttx/nuttx.uf2` to the `RP2350` USB mass-storage volume.
- **Interactive USB-C NuttShell (`nsh>`) Verification**:
  - Connect to `/dev/ttyACM0` (`picocom -b 115200 /dev/ttyACM0`) and press Enter three times to open `nsh>`.
  - Inspect boot/daemon logs (`dmesg`), running tasks (`ps`), singleton guard (`bargraph`), graceful shutdown (`kill -15 <pid>`), and headless ADC fault blink (`bargraph /dev/adc_missing` -> alternating `0x15` / `0x2a` at 250 ms).
- **Clean Targets**:
  - Clean compiled objects and test binary: `make -C xiao-rp2350-bargraph clean`
  - Clean out-of-tree build, generated `Kconfig` files, and `apps/external` symlink state: `make -C xiao-rp2350-bargraph distclean`
