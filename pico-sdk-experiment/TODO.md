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

- [ ] **Clone `pico-sdk` with submodules** (`tinyusb` is required for USB CDC serial `stdio_usb`):

  ```bash
  git clone --branch master --recurse-submodules https://github.com/raspberrypi/pico-sdk.git ~/pico-sdk
  ```

  *(Note: `/tmp/pico-sdk` currently exists on this machine without submodules initialized; if you prefer to reuse it temporarily before reboot, run `git -C /tmp/pico-sdk submodule update --init`.)*

- [ ] **Export `PICO_SDK_PATH`** in your current shell (and add to `~/.bashrc` or `~/.zshrc`):

  ```bash
  export PICO_SDK_PATH="$HOME/pico-sdk"
  ```

- [ ] **(Optional) Install Ninja, GDB, and OpenOCD** for faster incremental builds and SWD hardware debugging:

  ```bash
  sudo dnf install ninja-build gdb openocd
  ```

---

## 3. Commands to Compile the Firmware

Run from the workspace root (`/home/brett/Documents/GitHub/nuttxspace`):

- [ ] **Configure the CMake build directory** (targets `seeed_xiao_rp2350` by default; pass `-DPICO_BOARD=pico2` or `-DPICO_BOARD=pico` for other boards):

  ```bash
  cmake -S pico-sdk-experiment -B pico-sdk-experiment/build
  ```

- [ ] **Compile the firmware** (produces `pico-sdk-experiment/build/pico_exp.uf2` and `pico_exp.elf`):

  ```bash
  cmake --build pico-sdk-experiment/build -j$(nproc)
  ```

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
