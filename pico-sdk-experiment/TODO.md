# Raw Raspberry Pi Pico SDK Experiment — TODO (`pico-sdk-experiment`)

## 1. Already Installed Prerequisites (Verified on Fedora 44)

- [x] **ARM Bare-Metal Cross-Compiler Toolchain**:
  - `arm-none-eabi-gcc` & `arm-none-eabi-g++` (`15.2.0-4.fc44`)
  - `arm-none-eabi-binutils-cs` (`2.45-5.fc44`)
  - `arm-none-eabi-newlib` (`4.5.0.20241231-5.fc44`, includes `libstdc++.a`, `libc.a`, `libnosys.a`, and multilibs for RP2040 `cortex-m0plus` and RP2350 `cortex-m33`)
- [x] **Host Build Tools**: `cmake` (`4.3.0`), `make`, `gcc`, `g++` (used to build the SDK's host-side `pioasm` tool), `python3`, `git`
- [x] **Flashing & Serial Utilities**: `picotool` (`v2.3.1` in `/usr/local/bin/picotool`), `picocom`

---

## 2. Tasks to Install Required & Optional Dependencies

- [x] **Clone `pico-sdk` with submodules** (`/home/brett/pico-sdk` verified with initialized submodules: `btstack`, `cyw43-driver`, `lwip`, `mbedtls`, `tinyusb`):

  ```bash
  git clone --branch master --recurse-submodules https://github.com/raspberrypi/pico-sdk.git ~/pico-sdk
  ```

- [x] **Export `PICO_SDK_PATH`** (`PICO_SDK_PATH=/home/brett/pico-sdk` verified in active environment):

  ```bash
  export PICO_SDK_PATH="$HOME/pico-sdk"
  ```

- [x] **(Optional) Install Ninja, GDB, and OpenOCD** (`ninja-build-1.13.2-2.fc44.x86_64`, `gdb-18.1-1.fc44.x86_64`, `openocd-0.12.0-4.gitcb52502.fc44.x86_64` verified):

  ```bash
  sudo dnf install ninja-build gdb openocd
  ```

---

## 3. Commands to Compile the Firmware

Run from the workspace root (`/home/brett/Documents/GitHub/nuttxspace`):

- [x] **Configure the CMake build directory** (configured `seeed_xiao_rp2350`, `rp2350-arm-s`, `Release`, `pico_arm_cortex_m33_gcc`, `TinyUSB` USB support; pass `-DPICO_BOARD=pico2` or `-DPICO_BOARD=pico` for other boards):

  ```bash
  cmake -S pico-sdk-experiment -B pico-sdk-experiment/build
  ```

- [x] **Compile and verify the firmware** (produced `pico-sdk-experiment/build/pico_exp.uf2` (`57K`) and `pico_exp.elf` (`595K`)):

  ```bash
  cmake --build pico-sdk-experiment/build -j$(nproc)
  ```

  Verified output metrics:
  - `arm-none-eabi-size pico-sdk-experiment/build/pico_exp.elf`: `text = 32528`, `data = 0`, `bss = 2640`, `dec = 35168` (`0x8960`)
  - `picotool info -a pico-sdk-experiment/build/pico_exp.uf2`: family `rp2350-arm-s`, SDK `2.3.1`, board `seeed_xiao_rp2350`, features `USB stdin / stdout`, range `0x10000000`–`0x10006f14`, image type `ARM Secure` (`boot2_w25q080`)

---

## 4. Commands to Install (Flash) & Verify the Firmware

- [ ] **Put the board into `BOOTSEL` mode**:
  - Hold the `B` (BOOT) button on the XIAO RP2350 while plugging in USB-C (or while tapping `R` / RESET), then release `B`.

- [ ] **Flash `pico_exp.uf2` and reboot into the application**:

  ```bash
  picotool load -f pico-sdk-experiment/build/pico_exp.uf2 -x
  ```

  *(Alternatively, copy `pico-sdk-experiment/build/pico_exp.uf2` onto the mounted `RP2350` USB mass-storage volume.)*

- [ ] **Verify hardware & serial output**:
  - Confirm the onboard yellow user LED (`GPIO 25`) toggles every second.
  - Connect to the USB CDC serial port (`Ctrl+A` then `Ctrl+X` to exit `picocom`):

    ```bash
    picocom -b 115200 /dev/ttyACM0
    ```
