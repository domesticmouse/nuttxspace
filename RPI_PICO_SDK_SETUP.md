# Raw Raspberry Pi Pico SDK Setup Guide (Fedora 44)

Because this Fedora 44 machine is already configured for building Apache NuttX on the Seeed Studio XIAO RP2350, **almost all required host and cross-compilation packages are already installed**.

---

## 1. Already Installed Prerequisites

Verified on this host:

- **ARM Bare-Metal Cross-Compiler Toolchain**:
  - `arm-none-eabi-gcc` & `arm-none-eabi-g++` (`15.2.0-4.fc44`)
  - `arm-none-eabi-binutils-cs` (`2.45-5.fc44`)
  - `arm-none-eabi-newlib` (`4.5.0.20241231-5.fc44`)
  - Includes `libstdc++.a`, `libc.a`, `libnosys.a`, and multilib directories for both **RP2040** (`cortex-m0plus` / `thumb/v6-m/nofp`) and **RP2350** (`cortex-m33` / `thumb/v8-m.main/nofp`).
- **Host Build Tools**:
  - `cmake` (`4.3.0`), `make`, `gcc` / `g++` (used by the SDK to compile its host-side `pioasm` PIO assembler), `python3`, and `git`.
- **Flashing & Serial Utilities**:
  - `picotool` (`v2.3.1` at `/usr/local/bin/picotool`)
  - `picocom`

> [!NOTE]
> If setting up a fresh Fedora machine from scratch, install these prerequisites with:
> ```bash
> sudo dnf install gcc gcc-c++ make cmake python3 git picocom libusb1-devel \
>   arm-none-eabi-gcc-cs arm-none-eabi-gcc-cs-c++ arm-none-eabi-binutils-cs arm-none-eabi-newlib
> ```

---

## 2. What You Still Need to Install

### A. The Raspberry Pi `pico-sdk` Repository (Required)

Clone `pico-sdk` along with its required submodules (`tinyusb`, `btstack`, `cyw43-driver`, and `mbedtls`):

```bash
git clone --branch master --recurse-submodules https://github.com/raspberrypi/pico-sdk.git ~/pico-sdk
```

Export `PICO_SDK_PATH` in your shell profile (`~/.bashrc` or `~/.zshrc`):

```bash
export PICO_SDK_PATH="$HOME/pico-sdk"
```

*(Alternatively, you can skip cloning `pico-sdk` globally and pass `-DPICO_SDK_FETCH_FROM_GIT=on` to CMake per project, though a single local checkout is faster across multiple projects.)*

### B. Optional Fedora Packages (Recommended)

`make` works out of the box, but for faster incremental builds with Ninja or hardware SWD debugging:

```bash
sudo dnf install ninja-build gdb openocd
```

*(Fedora's standard `gdb` package includes multi-architecture support for `arm-none-eabi` targets, replacing `arm-none-eabi-gdb`.)*

---

## 3. Minimal Raw Pico SDK Project Template

To create a standalone raw Pico SDK C/C++ project from scratch, create a project directory with three files:

### Step 1: Copy the SDK Bootstrap Helper

```bash
mkdir -p my_pico_app && cd my_pico_app
cp "$PICO_SDK_PATH/external/pico_sdk_import.cmake" .
```

### Step 2: Create `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.13)

# Set target board before pico_sdk_import:
# Common values: "pico" (RP2040), "pico2" (RP2350), "seeed_xiao_rp2350" (XIAO RP2350)
set(PICO_BOARD seeed_xiao_rp2350 CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(my_pico_app C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(my_pico_app
    main.c
)

# Link standard libraries (add hardware_adc, hardware_pio, etc. as needed)
target_link_libraries(my_pico_app
    pico_stdlib
)

# Enable USB CDC serial stdio, disable UART stdio
pico_enable_stdio_usb(my_pico_app 1)
pico_enable_stdio_uart(my_pico_app 0)

# Generate .uf2, .bin, .hex, .dis outputs
pico_add_extra_outputs(my_pico_app)
```

### Step 3: Create `main.c`

```c
#include <stdio.h>
#include "pico/stdlib.h"

int main(void) {
    stdio_init_all();
    while (true) {
        printf("Hello from raw Pico SDK!\n");
        sleep_ms(1000);
    }
}
```

---

## 4. Build, Flash, and Monitor

1. **Configure and compile:**
   ```bash
   cmake -B build
   cmake --build build -j$(nproc)
   ```
   *(Or pass `-G Ninja` to `cmake -B build` if `ninja-build` is installed.)*

2. **Flash the `.uf2` image (with board in `BOOTSEL` mode):**
   ```bash
   picotool load -f build/my_pico_app.uf2 -x
   ```

3. **Monitor USB CDC serial output:**
   ```bash
   picocom -b 115200 /dev/ttyACM0
   ```
