# Build Environment TODO (`xiao-rp2350-bargraph`)

## Installed & Verified Prerequisites

- [x] **Host compiler & build tools**: `gcc`, `cpp`, `make`, `bison`, `flex`, `gperf`, `gettext`, `texinfo`, `ncurses-devel`
- [x] **Kconfig frontends**: `/usr/local/bin/kconfig-conf`, `kconfig-tweak`, and `kconfig-mconf` (linked against `/usr/local/lib/libkconfig-parser-4.11.0.so`)
- [x] **ROMFS generation tools**: `genromfs`, `xxd`
- [x] **Serial terminal**: `picocom`

## Remaining Tools to Install (Fedora 44)

- [x] **ARM Bare-Metal Cross-Compiler Toolchain (`arm-none-eabi-*`)** (`arm-none-eabi-gcc`, `arm-none-eabi-ld`, `arm-none-eabi-ar`, `arm-none-eabi-objcopy`), required by `nuttx/arch/arm/src/armv8-m/Toolchain.defs` and `nuttx/arch/arm/src/common/Toolchain.defs`:

  ```bash
  sudo dnf install arm-none-eabi-gcc-cs arm-none-eabi-gcc-cs-c++ arm-none-eabi-binutils-cs arm-none-eabi-newlib
  ```

  *(Alternatively, download the official Arm GNU Toolchain `arm-gnu-toolchain-*-x86_64-arm-none-eabi` tarball and add its `bin/` directory to `PATH`.)*

- [x] **`picotool` (or `$PICO_SDK_PATH` + `cmake`)**, required by `nuttx/tools/rp23xx/Config.mk` (`CONFIG_RP23XX_UF2_BINARY=y` in `nuttx/boards/arm/rp23xx/common/Kconfig`) to convert the ELF image into `nuttx.uf2`:

  ```bash
  sudo dnf install cmake libusb1-devel
  git clone https://github.com/raspberrypi/pico-sdk.git /tmp/pico-sdk
  git clone https://github.com/raspberrypi/picotool.git /tmp/picotool
  cmake -S /tmp/picotool -B /tmp/picotool/build -DPICO_SDK_PATH=/tmp/pico-sdk
  cmake --build /tmp/picotool/build
  sudo cmake --install /tmp/picotool/build
  ```

## Build & Verification Commands Once Installed

- [x] Run host unit tests: `make -C xiao-rp2350-bargraph test`
- [x] Configure and compile `nuttx.uf2`: `make -C xiao-rp2350-bargraph`
