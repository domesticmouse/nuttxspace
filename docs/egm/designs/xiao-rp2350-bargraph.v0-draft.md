# Out-of-Tree Seeed Studio XIAO RP2350 6-Segment PWM LED Bar Graph (`xiao-rp2350-bargraph/`)

*   **Author:** Brett Morgan
*   **Status:** Draft
*   **Date:** 2026-11-16

--------------------------------------------------------------------------------

## Overview

This design defines an out-of-tree Apache NuttX board support package (`xiao-rp2350-bargraph/board/`) and built-in user-space application (`xiao-rp2350-bargraph/apps/bargraph/`) for a Seeed Studio XIAO RP2350 wired on a 400-tie-point solderless breadboard with one linear potentiometer and six discrete active-high LEDs.

At power-on, the firmware initializes the RP2350 analog-to-digital converter (`/dev/adc0`) and five hardware pulse-width modulation slices (`/dev/pwm0`, `/dev/pwm2`, `/dev/pwm3`, `/dev/pwm5`, and `/dev/pwm6`), and launches a background daemon (`bargraphd`) before any USB-C host serial connection is opened. The daemon samples the potentiometer on `D0/A0` (`GPIO26`) at 50 Hz and drives the six LEDs on `D1` through `D6` (`GPIO27`, `GPIO28`, `GPIO5`, `GPIO6`, `GPIO7`, and `GPIO0`) as a continuous bar graph: lower segments below the reading illuminate at 100% duty cycle, the active segment modulates its brightness proportionally to the position within that sixth of the range, and upper segments remain off (`0%` duty cycle). When a host computer attaches over USB-C CDC/ACM (`/dev/ttyACM0`), NuttShell (`nsh`) provides an interactive console where the `bargraph` command can query telemetry or control the daemon (`start`, `stop`, `status`, `test`, `set <0..100>`, and `resume`). Both the `nuttx/` and `apps/` git submodules remain unmodified.

## The Problem

Building a self-contained analog sensor and multi-channel PWM display on the Seeed Studio XIAO RP2350 under Apache NuttX exposes seven concrete hardware, driver, and boot-sequencing obstacles when keeping `nuttx/` and `apps/` clean:

1.  **Unregistered Peripheral Devices in Default Board Configuration**: The upstream `xiao-rp2350:usbnsh` configuration omits `CONFIG_BOARD_LATE_INITIALIZE=y`, which compiles out `board_late_initialize()` in `rp23xx_boardinitialize.c` and prevents `rp23xx_bringup()` from registering `/dev/adc0` and `/dev/pwm*`.
2.  **USB Console Blocking Before Application Startup**: When NSH runs over USB CDC/ACM (`nsh_usbconsole.c`), `nsh_consolemain()` blocks inside `nsh_waitusbready()` until a host computer opens `/dev/ttyACM0` and sends three newline or carriage-return characters. Starting the bar graph from an NSH startup script or foreground entrypoint leaves the LEDs dark when the board is powered from a standalone USB power supply without a host PC.
3.  **Multi-Channel PWM Indexing and Dual Kconfig Guards**: The RP2350 shares PWM slice 3 between `D4` (`GPIO6`, `PWM3A`) and `D5` (`GPIO7`, `PWM3B`), while `D1` (`GPIO27`, `PWM5B`) and `D3` (`GPIO5`, `PWM2B`) reside on hardware channel B of their respective slices. In `nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c`, `setup_pulse()` ignores `info->channels[i].channel` and maps `channels[0].duty` to channel A (`CC[15:0]`) and `channels[1].duty` to channel B (`CC[31:16]`), whereas `nuttx/drivers/timers/pwm.c` stops reading `channels[]` if `channel == 0`. Furthermore, the RP23xx C sources check `CONFIG_PWM_NCHANNELS == 2` while `arch/arm/src/rp23xx/Kconfig` defines `CONFIG_RP23XX_PWM_NCHANNELS`.
4.  **16-Bit Duty Truncation (`b16ONE` Overflow)**: `struct rp23xx_pwm_lowerhalf_s` stores duty cycle as `uint16_t` rather than `ub16_t` (`uint32_t`). Passing the standard NuttX 100% fixed-point value `b16ONE` (`0x00010000` = `65536`) truncates to `0x0000`, turning a segment off at 100% brightness.
5.  **Frozen GPIO State on `PWMIOC_STOP`**: `pwm_stop()` in `rp23xx_pwm.c` clears `RP23XX_PWM_CSR_EN` without driving the output low, freezing each LED pad at whichever level the counter held when stopped.
6.  **ADC Pin-Mux Reset on `D1`/`D2` and RP2350-E9 Pad Leakage on `D0`**: Enabling `ADC_CHANNEL1` (`GPIO27`) or `ADC_CHANNEL2` (`GPIO28`) causes `rp23xx_adc.c` (`my_reset()`) to overwrite `D1` and `D2` PWM pin muxing with `RP23XX_GPIO_FUNC_NULL`. Additionally, leaving the digital input buffer enabled (`IE=1`) on `GPIO26` introduces RP2350-E9 pad leakage across mid-scale potentiometer voltages.
7.  **500 kS/s Free-Running ADC Interrupt Storm, FIFO Staleness, and 19-Bit Left-Shift**: `rp23xx_adc.c` runs the ADC in continuous free-running mode (`ADC_CS_START_MANY`) at 500,000 conversions per second and shifts each 12-bit sample left by 19 bits (`value <<= 19`) in the interrupt handler. Without hardware clock division and FIFO flushing prior to reads, the CPU services a high-rate interrupt storm whenever FIFO slots open and reads stale samples from the head of the software queue.

This design solves these problems entirely within an out-of-tree directory (`xiao-rp2350-bargraph/`) by combining a custom board bringup layer, targeted peripheral register configuration during `rp23xx_bringup()`, an autonomous background daemon (`bargraphd`), and a thread-safe NSH control utility (`bargraph`).

## Goals

*   **Zero Submodule Modifications**: Keep `git status` and `git diff` completely clean in both `nuttx/` and `apps/` by housing all board, driver bringup, and application files under `xiao-rp2350-bargraph/`.
*   **Autonomous Power-On Operation**: Start `bargraphd` during `board_late_initialize()` -> `rp23xx_bringup()` so the 6-LED bar graph tracks the potentiometer within milliseconds of power-on without requiring a USB-C host connection or host `genromfs`/`xxd` binaries.
*   **Stable 50 Hz Potentiometer Sensing**: Sample `D0/A0` (`GPIO26`, `ADC0`) every 20 ms using a 4-sample burst average after `ANIOC_RESET_FIFO`, an integer exponential moving average filter (`alpha = 1/4`), endpoint deadbands (`<= 32` clamped to `0`, `>= 4063` clamped to `4095`), RP2350-E9 digital pad isolation (`IE=0`, `PUE=0`, `PDE=0`, `ISO=0`, `OD=1`), and hardware ADC clock pacing (`RP23XX_ADC_DIV = 0xFFFF00`, `~732 Hz`).
*   **Proportional 6-Segment Hardware PWM Display**: Drive `D1`–`D6` (`PWM5B`, `PWM6A`, `PWM2B`, `PWM3A`, `PWM3B`, `PWM0A`) at `1000 Hz` with `uint16_t`-safe duty cycles (`0..65535`), keeping lower segments at `65535` (100%), linearly interpolating the active segment across `0..65535`, and setting upper segments to `0` (0%).
*   **Interactive USB-C NSH Coexistence and CLI Control**: Provide the `bargraph` built-in command over the USB-C CDC/ACM NSH console supporting `start`, `stop`, `status`, `test`, `set <0..100>`, and `resume` with deterministic synchronization against `bargraphd`.
*   **Complete Breadboard & EGM Documentation**: Provide a reproducible 400-tie-point breadboard layout, bill of materials, current budget, and EGM `README.md` contracts for every directory in `xiao-rp2350-bargraph/`.

## Non-Goals

*   Modifying any tracked source file, header, Kconfig, or Makefile inside `nuttx/` or `apps/`.
*   Supporting a hardware UART console on `D6`/`D7` (`GPIO0` is dedicated to `LED6` via `PWM0A`).
*   Persisting manual override values to flash, remapping GPIO pins at runtime, or streaming continuous telemetry to the terminal without user polling.

## Open Questions

*   None. All hardware pin assignments, driver workarounds, and CLI subcommands were resolved during the concepts phase.

## Critical User Journeys (CUJs)

### CUJ 1: Standalone Power-On and Live Potentiometer Tracking

*   **Given**: The Seeed Studio XIAO RP2350 is flashed with the `xiao-rp2350-bargraph:bargraph` firmware, wired on the breadboard with the `10 kΩ` potentiometer on `D0/A0` and six LEDs (`D1`–`D6`), and connected to a 5V USB power source with no host terminal open.
*   **When**: The board powers on and the user rotates the potentiometer wiper smoothly from `0.0 V` (`GND`) to `3.3 V` (`3V3`).
*   **Then**: `bargraphd` starts automatically during `rp23xx_bringup()`. At `0.0 V`, all six LEDs are off (`duty = 0`). As the wiper sweeps upward, `D1` ramps smoothly in brightness from `0%` to `100%` (`0.0%`–`16.7%` rotation), stays locked at `100%` while `D2` ramps (`16.7%`–`33.3%`), and continues sequentially through `D3`, `D4`, `D5`, and `D6` until all six LEDs are lit at `100%` duty (`65535`) at the top of travel.

### CUJ 2: USB-C NSH Console Telemetry Inspection (`bargraph status`)

*   **Given**: The board is running `bargraphd` in `SENSING` mode and a user connects a host computer over USB-C, opens `/dev/ttyACM0`, and presses Enter three times to reach the `nsh>` prompt.
*   **When**: The user runs `bargraph status` at the `nsh>` prompt.
*   **Then**: The CLI reads a consistent snapshot of the daemon state and prints the current daemon state (`SENSING`), raw 12-bit ADC count (`0..4095`), filtered ADC count (`0..4095`), estimated voltage in millivolts (`0..3300 mV`), normalized percentage (`0.0%..100.0%`), and the per-segment PWM duty cycle (`0..65535` and `0%..100%`) for `LED1` through `LED6`, without interrupting 50 Hz LED updates.

### CUJ 3: Manual Percentage Override, Hardware Sweep Test, and Safe Stop

*   **Given**: The user is connected to the `nsh>` prompt over USB-C CDC/ACM.
*   **When**: The user executes `bargraph set 50`, then `bargraph test`, then `bargraph resume`, and finally `bargraph stop`.
*   **Then**:
    1.  On `bargraph set 50`, the daemon transitions to `MANUAL` mode, ignores potentiometer movement, lights `D1`, `D2`, and `D3` at `100%` (`65535`), and sets `D4`, `D5`, and `D6` to `0%` (`0`).
    2.  On `bargraph test`, the daemon executes a smooth `0% -> 100% -> 0%` sweep across `D1`–`D6` over ~2 seconds so the user can visually verify every LED and PWM channel, then restores the previous active mode.
    3.  On `bargraph resume` (or `bargraph start`), the daemon returns to `SENSING` mode and immediately tracks the live potentiometer position.
    4.  On `bargraph stop`, the daemon writes `duty = 0` to all six PWM channels before issuing `PWMIOC_STOP` so every LED turns cleanly off, closes the PWM file descriptors, and exits cleanly.

--------------------------------------------------------------------------------

## Technical Plan (Architecture)

### 1. System Architecture & Out-of-Tree Build Topology

All custom board configuration, peripheral bringup hooks, and user-space application code live inside `xiao-rp2350-bargraph/` at the repository root so that `nuttx/` and `apps/` remain clean git submodules:

```text
nuttxspace-egm/
├── nuttx/                                  # Unmodified upstream NuttX submodule
├── apps/                                   # Unmodified upstream nuttx-apps submodule
│   └── external -> ../xiao-rp2350-bargraph/apps   # Git-ignored symlink created at build time
└── xiao-rp2350-bargraph/
    ├── README.md
    ├── board/                              # Out-of-tree custom board (CONFIG_ARCH_BOARD_CUSTOM)
    │   ├── README.md
    │   ├── Kconfig
    │   ├── configs/
    │   │   ├── README.md
    │   │   └── bargraph/
    │   │       ├── README.md
    │   │       └── defconfig
    │   ├── include/
    │   │   ├── README.md
    │   │   └── board.h
    │   ├── scripts/
    │   │   ├── README.md
    │   │   ├── Make.defs
    │   │   └── memmap_default.ld
    │   └── src/
    │       ├── README.md
    │       ├── Make.defs
    │       ├── rp23xx_pico.h
    │       ├── rp23xx_boardinitialize.c
    │       ├── rp23xx_bringup.c
    │       ├── rp23xx_autoleds.c
    │       ├── rp23xx_userleds.c
    │       └── rp23xx_gpio.c
    └── apps/                               # Out-of-tree application root (linked as apps/external)
        ├── README.md
        ├── Kconfig
        ├── Makefile
        ├── Make.defs
        └── bargraph/
            ├── README.md
            ├── Kconfig
            ├── Makefile
            ├── Make.defs
            ├── bargraph_internal.h
            └── bargraph_main.c
```

#### Out-of-Tree Board Discovery (`CONFIG_ARCH_BOARD_CUSTOM`)

`xiao-rp2350-bargraph/board/configs/bargraph/defconfig` sets:
*   `CONFIG_ARCH_BOARD_CUSTOM=y`
*   `CONFIG_ARCH_BOARD_CUSTOM_NAME="xiao-rp2350-bargraph"`
*   `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`
*   `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y`
*   `CONFIG_ARCH_BOARD_COMMON=y`

When `tools/configure.sh ../xiao-rp2350-bargraph/board/configs/bargraph` runs inside `nuttx/`:
1.  `nuttx/tools/Config.mk` resolves `BOARD_DIR` to `$(TOPDIR)/../xiao-rp2350-bargraph/board` and `BOARD_COMMON_DIR` to `$(TOPDIR)/boards/arm/rp23xx/common`.
2.  `nuttx/tools/Unix.mk` links `include/arch/board` to `xiao-rp2350-bargraph/board/include`, links `arch/arm/src/board` to `nuttx/boards/arm/rp23xx/common` (ignored by `nuttx/boards/.gitignore` and `nuttx/arch/arm/src/.gitignore`), and links `arch/arm/src/board/board` to `xiao-rp2350-bargraph/board/src`.
3.  `nuttx/boards/arm/rp23xx/common/Makefile` includes both `src/Make.defs` (compiling `rp23xx_common_bringup.c`, `rp23xx_common_initialize.c`, and `rp23xx_pwmdev.c`) and `board/Make.defs` (compiling our custom `xiao-rp2350-bargraph/board/src/*.c`).

#### Out-of-Tree Application Discovery (`apps/external` Symlink)

`apps/.gitignore` explicitly ignores `/external` and `/Kconfig`. In `apps/Make.defs`, `-include $(TOPDIR)/Make.defs` executes at line 24 before `BUILDIRS := $(dir $(wildcard $(APPDIR)/*/Make.defs))` at line 32. By placing a shell hook in `xiao-rp2350-bargraph/board/scripts/Make.defs`:

```makefile
$(shell if [ ! -e "$(TOPDIR)/../apps/external" ]; then \
  ln -s ../xiao-rp2350-bargraph/apps "$(TOPDIR)/../apps/external"; \
fi)
```

the symlink `apps/external -> ../xiao-rp2350-bargraph/apps` is created automatically before `apps` scans its subdirectories. Furthermore, `xiao-rp2350-bargraph/apps/Kconfig` is tracked in git with relative `source "$APPSDIR/external/bargraph/Kconfig"` and `xiao-rp2350-bargraph/apps/Makefile` omits `MENUDESC` before including `$(APPDIR)/Directory.mk`, preventing `make distclean` from deleting `xiao-rp2350-bargraph/apps/Kconfig`.

---

### 2. Hardware Pinout, Breadboard Layout & Electrical Budget

#### Pin Assignment & Peripheral Mapping

| Signal Role | Bar Segment & Range | XIAO Pin | RP2350 GPIO | Peripheral Device | Hardware Slice & Channel | `pwm_info_s` Index |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Potentiometer Wiper | `0.0% – 100.0%` (`0–3.3V`) | Pin 1 (`D0/A0`) | `GPIO26` | `/dev/adc0` | `ADC` Channel 0 | N/A |
| `LED1` (Lowest) | Segment 1 (`0.0% – 16.7%`) | Pin 2 (`D1/A1`) | `GPIO27` | `/dev/pwm5` | Slice 5, Channel B (`PWM5B`) | `channels[1]` (`channel = 2`) |
| `LED2` | Segment 2 (`16.7% – 33.3%`) | Pin 3 (`D2/A2`) | `GPIO28` | `/dev/pwm6` | Slice 6, Channel A (`PWM6A`) | `channels[0]` (`channel = 1`) |
| `LED3` | Segment 3 (`33.3% – 50.0%`) | Pin 4 (`D3`) | `GPIO5` | `/dev/pwm2` | Slice 2, Channel B (`PWM2B`) | `channels[1]` (`channel = 2`) |
| `LED4` | Segment 4 (`50.0% – 66.7%`) | Pin 5 (`D4/SDA`) | `GPIO6` | `/dev/pwm3` | Slice 3, Channel A (`PWM3A`) | `channels[0]` (`channel = 1`) |
| `LED5` | Segment 5 (`66.7% – 83.3%`) | Pin 6 (`D5/SCL`) | `GPIO7` | `/dev/pwm3` | Slice 3, Channel B (`PWM3B`) | `channels[1]` (`channel = 2`) |
| `LED6` (Highest) | Segment 6 (`83.3% – 100.0%`) | Pin 7 (`D6/TX`) | `GPIO0` | `/dev/pwm0` | Slice 0, Channel A (`PWM0A`) | `channels[0]` (`channel = 1`) |
| `3.3V` Rail Supply | Analog & logic reference | Pin 12 (`3V3`) | `3V3_OUT` | On-board LDO | `3.3 V` nominal | N/A |
| Ground Reference | Common return | Pin 13 (`GND`) | `GND` | Ground plane | `0.0 V` | N/A |

#### 400-Tie-Point Breadboard Layout

```text
       +3.3V Rail (Red +)                         GND Rail (Blue -)
         |                                          |
  Row 1: +---[ Potentiometer Lug 1 (CW / 3.3V) ]    |
  Row 2: |   [ Potentiometer Lug 2 (Wiper) ]--------+---> Jumper to Row 5A (D0/A0, GPIO26)
  Row 3: |   [ Potentiometer Lug 3 (CCW / GND) ]----+     (Optional 100 nF cap: Row 5B to GND)
         |                                          |
         |         Cols A..E     Center    Cols F..J|
  Row 5: |   [D0/A0  GPIO26 (5E)]  ||  [(5F) 5V    ] (Unconnected)
  Row 6: |   [D1/A1  GPIO27 (6E)]  ||  [(6F) GND   ]+---> Jumper Row 6J to Blue (-) GND Rail
  Row 7: +---[D2/A2  GPIO28 (7E)]  ||  [(7F) 3V3   ]      (Jumper Row 7J to Red (+) 3.3V Rail)
  Row 8:     [D3     GPIO5  (8E)]  ||  [(8F) GPIO2 ]
  Row 9:     [D4     GPIO6  (9E)]  ||  [(9F) GPIO4 ]
  Row 10:    [D5     GPIO7 (10E)]  ||  [(10F) GPIO3]
  Row 11:    [D6     GPIO0 (11E)]  ||  [(11F) GPIO1]
                                                    |
  Row 14: Jumper from 6A  (D1) -> [330 Ohm] -> [LED1 +  -] ---> Blue (-) GND Rail
  Row 15: Jumper from 7A  (D2) -> [330 Ohm] -> [LED2 +  -] ---> Blue (-) GND Rail
  Row 16: Jumper from 8A  (D3) -> [330 Ohm] -> [LED3 +  -] ---> Blue (-) GND Rail
  Row 17: Jumper from 9A  (D4) -> [330 Ohm] -> [LED4 +  -] ---> Blue (-) GND Rail
  Row 18: Jumper from 10A (D5) -> [330 Ohm] -> [LED5 +  -] ---> Blue (-) GND Rail
  Row 19: Jumper from 11A (D6) -> [330 Ohm] -> [LED6 +  -] ---> Blue (-) GND Rail
```

#### Electrical & Current Budget

*   **LED Current (per segment at 100% duty)**: With $V_{DD} = 3.3\text{ V}$, $V_F \approx 2.0\text{ V}$ (standard red/yellow/green diffused LED), and $R = 330\ \Omega$:
    $$I_{\text{LED}} = \frac{3.3\text{ V} - 2.0\text{ V}}{330\ \Omega} \approx 3.94\text{ mA}$$
    This is below the RP2350 default 4 mA / 8 mA pad drive limit (12 mA max per GPIO pin).
*   **Total 6-LED Current (all segments at 100% duty)**: $6 \times 3.94\text{ mA} \approx 23.6\text{ mA}$, well within the RP2350 200 mA total `IOVDD` current limit and the XIAO 3.3V regulator budget.
*   **Potentiometer Current (`10 kΩ` linear `B10K`)**: $3.3\text{ V} / 10\text{ k}\Omega = 0.33\text{ mA}$ continuous draw. Maximum Thevenin source impedance at mid-scale ($50\%$) is $R_S = 10\text{ k}\Omega / 4 = 2.5\text{ k}\Omega$.

---

### 3. Boot Sequence & Autonomous Startup Coexistence with USB-C NSH

In upstream NuttX, `nsh_usbconsole.c` waits inside `nsh_waitusbready()` until a USB host opens `/dev/ttyACM0` and transmits three `\r` or `\n` bytes. To ensure the LED bar graph operates immediately at power-on (even on a standalone 5V USB power brick), `bargraphd` is spawned during `board_late_initialize()` before `nx_start_application()` starts `nsh_main`:

```mermaid
sequenceDiagram
    participant Boot as RP2350 Reset / nx_start()
    participant LateInit as board_late_initialize() / rp23xx_bringup()
    participant Common as rp23xx_common_bringup()
    participant Daemon as bargraphd (Kernel/Flat Task)
    participant NSH as nsh_main (USB CDC/ACM)

    Boot->>LateInit: Calls board_late_initialize() -> rp23xx_bringup()
    LateInit->>Common: Calls rp23xx_common_bringup()
    Common-->>LateInit: Registers /dev/pwm0,2,3,5,6 and /dev/adc0
    LateInit->>LateInit: Configure PADS_BANK0_GPIO(26) (IE=0, OD=1) & ADC_DIV=0xFFFF00
    LateInit->>Daemon: Calls bargraph_autostart() -> task_create("bargraphd")
    Daemon->>Daemon: Opens /dev/adc0 & /dev/pwm*, enters 50 Hz SENSING loop
    LateInit-->>Boot: Returns OK
    Boot->>NSH: Spawns INIT_ENTRYPOINT ("nsh_main")
    NSH->>NSH: Blocks in nsh_waitusbready() until USB host connects
    Note over Daemon,NSH: LEDs actively track potentiometer while NSH waits for USB
    NSH->>Daemon: User connects & runs "bargraph status|set|test|resume|stop"
```

---

### 4. RP2350 Peripheral & Driver Mitigation Design

#### 4.1 ADC Subsystem Mitigations (`GPIO26` / `ADC0`)

1.  **Preventing `D1`/`D2` PWM Pin-Mux Corruption**:
    In `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`, `my_reset()` iterates over all enabled ADC channels when `/dev/adc0` is registered and calls `rp23xx_gpio_set_function(pin, RP23XX_GPIO_FUNC_NULL)`. Because `rp23xx_common_bringup()` initializes PWM before ADC, enabling `ADC_CHANNEL1` (`GPIO27`) or `ADC_CHANNEL2` (`GPIO28`) would wipe out the `RP23XX_GPIO_FUNC_PWM` function on `D1` and `D2`. The board `defconfig` therefore enables **only** `CONFIG_RP23XX_ADC_CHANNEL0=y` and leaves `ADC_CHANNEL1`, `ADC_CHANNEL2`, `ADC_CHANNEL3`, and `ADC_TEMPERATURE` disabled.
2.  **Erratum RP2350-E9 Digital Pad Isolation on `GPIO26`**:
    `rp23xx_adc.c` sets `GPIO26` function to `NULL` (`0x1f`) and disables pulls, but leaves `RP23XX_PADS_BANK0_GPIO_IE` (`bit 6`, Input Enable) set. On RP2350, an enabled digital input buffer causes mid-scale analog pad leakage (RP2350-E9). Immediately after `rp23xx_common_bringup()` returns in `xiao-rp2350-bargraph/board/src/rp23xx_bringup.c`, the board bringup modifies `RP23XX_PADS_BANK0_GPIO(26)`:
    *   Clears `RP23XX_PADS_BANK0_GPIO_IE` (`bit 6`), `RP23XX_PADS_BANK0_GPIO_PUE` (`bit 3`), `RP23XX_PADS_BANK0_GPIO_PDE` (`bit 2`), and `RP23XX_PADS_BANK0_GPIO_ISO` (`bit 8`).
    *   Sets `RP23XX_PADS_BANK0_GPIO_OD` (`bit 7`, Output Disable).
3.  **Hardware Conversion Rate Pacing (`RP23XX_ADC_DIV`)**:
    `rp23xx_adc.c` runs the ADC in continuous free-running mode (`ADC_CS_START_MANY`). At the default 48 MHz ADC clock (`DIV = 0`, 96 cycles/sample), the ADC fires 500,000 interrupts/second whenever the software FIFO has free slots. In `rp23xx_bringup.c`, writing `putreg32(0x00ffff00, RP23XX_ADC_DIV)` sets the 16-bit integer divider to `65535`, pacing conversions to:
    $$f_{\text{sample}} = \frac{48,000,000\text{ Hz}}{65536} \approx 732.42\text{ Hz}\quad (\text{1.365 ms per sample})$$
    This reduces ADC interrupt overhead by 99.85% while providing 4 fresh samples in `~5.5 ms` on each `20 ms` (`50 Hz`) daemon cycle.
4.  **Persistent Descriptor, FIFO Flush, and 19-Bit Shift Recovery**:
    *   `rp23xx_adc.c` has an unguarded `remove_device(priv)` in `adc_interrupt()` when the last reader closes `/dev/adc0` while an interrupt is in flight. `bargraphd` therefore opens `/dev/adc0` once on startup and keeps the descriptor open until `bargraph stop` terminates the daemon.
    *   Before each 50 Hz sample burst, `bargraphd` calls `ioctl(adc_fd, ANIOC_RESET_FIFO, 0)` to discard samples accumulated during the 20 ms sleep window, then reads `4` `struct adc_msg_s` structs (`CONFIG_ADC_FIFOSIZE=16`).
    *   Because `rp23xx_adc.c` shifts the 12-bit result left by 19 bits (`value <<= 19`) and sets `am_data = -value` on conversion errors, `bargraphd` checks `msg.am_data >= 0` and extracts the 12-bit unsigned value as:
        $$\text{raw}_{12} = (\text{uint16\_t})((\text{msg.am\_data} \gg 19) \ \& \ \text{0x0FFF})$$
5.  **Filtering & Endpoint Deadband Clamping**:
    *   The 4 burst samples are averaged into `raw_avg` (`0..4095`).
    *   An integer exponential moving average (EMA) with $\alpha = 1/4$ smooths mechanical wiper jitter using fixed-point Q4 state (`ema_q4`):
        $$\text{ema\_q4} \leftarrow \text{ema\_q4} - (\text{ema\_q4} \gg 2) + (\text{raw\_avg} \ll 2), \quad \text{filtered} = \text{ema\_q4} \gg 2$$
    *   Endpoint deadbands clamp `filtered <= 32` (`~0.8%`) to `0` and `filtered >= 4063` (`~99.2%`) to `4095` so the user can reliably reach complete darkness (`0%` on all 6 LEDs) and full saturation (`100%` on all 6 LEDs) despite potentiometer mechanical end-stop resistance.

#### 4.2 Hardware PWM Subsystem Mitigations (`D1`–`D6`)

1.  **Dual Kconfig Multi-Channel Guards & Unused Sibling Pin Masking**:
    `nuttx/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c`, `rp23xx_pwmdev.c`, and `rp23xx_pwm.c` check `#if defined(CONFIG_PWM_NCHANNELS) && CONFIG_PWM_NCHANNELS == 2`, whereas `arch/arm/src/rp23xx/Kconfig` defines `CONFIG_RP23XX_PWM_MULTICHAN` and `CONFIG_RP23XX_PWM_NCHANNELS`. The board `defconfig` enables both pairs (`CONFIG_PWM_MULTICHAN=y`, `CONFIG_PWM_NCHANNELS=2`, `CONFIG_RP23XX_PWM_MULTICHAN=y`, `CONFIG_RP23XX_PWM_NCHANNELS=2`) and sets unused sibling pins to `-1`:
    *   Slice 0 (`/dev/pwm0`): `PWM0A_GPIO=0` (`D6`), `PWM0B_GPIO=-1`
    *   Slice 2 (`/dev/pwm2`): `PWM2A_GPIO=-1`, `PWM2B_GPIO=5` (`D3`)
    *   Slice 3 (`/dev/pwm3`): `PWM3A_GPIO=6` (`D4`), `PWM3B_GPIO=7` (`D5`)
    *   Slice 5 (`/dev/pwm5`): `PWM5A_GPIO=-1`, `PWM5B_GPIO=27` (`D1`)
    *   Slice 6 (`/dev/pwm6`): `PWM6A_GPIO=28` (`D2`), `PWM6B_GPIO=-1`
    In `rp23xx_pwm.c`, pins `< 0` (`>= 30` unsigned) are skipped during GPIO mux setup, so setting `-1` leaves the sibling GPIOs untouched.
2.  **Two-Channel `struct pwm_info_s` Population**:
    Because the upper-half driver (`nuttx/drivers/timers/pwm.c`) stops reading `info.channels[]` if `channel == 0` while the lower-half driver (`rp23xx_pwm.c`) maps `channels[0].duty` to hardware channel A (`CC[15:0]`) and `channels[1].duty` to hardware channel B (`CC[31:16]`), `bargraphd` always populates every slice's `struct pwm_info_s` with:
    *   `info.frequency = 1000` (`1 kHz`, yielding `DIV = 16`, `TOP = 9374` at `150 MHz` peripheral clock for flicker-free dimming)
    *   `info.channels[0].channel = 1`, `info.channels[0].duty = duty_a`
    *   `info.channels[1].channel = 2`, `info.channels[1].duty = duty_b`
    On `/dev/pwm3`, `duty_a` is `LED4` (`D4`) and `duty_b` is `LED5` (`D5`), updated atomically in one `ioctl(fd, PWMIOC_SETCHARACTERISTICS, &info)` + `ioctl(fd, PWMIOC_START, 0)` sequence.
3.  **16-Bit Duty Scaling (`0..65535`) and 6-Segment Transfer Function**:
    To avoid the `uint16_t` truncation bug in `rp23xx_pwm.c` when `duty == b16ONE` (`65536` -> `0`), maximum duty is defined as `BARGRAPH_DUTY_MAX = 65535` (`0xFFFF`). Given a normalized input level $L \in [0, 4095]$ (or in `MANUAL`/`TEST` mode, $L \in [0, 6 \times 65535]$):
    *   Scale input to total bar units $U \in [0, 6 \times 65535]$ ($[0, 393210]$):
        $$U = \left\lfloor \frac{L \times 393210 + 2047}{4095} \right\rfloor$$
    *   For each segment $k \in \{0, 1, 2, 3, 4, 5\}$ corresponding to `LED1`..`LED6` with segment threshold $T_k = k \times 65535$:
        $$\text{duty}_k = \begin{cases} 65535 & \text{if } U \ge T_k + 65535 \\ U - T_k & \text{if } T_k < U < T_k + 65535 \\ 0 & \text{if } U \le T_k \end{cases}$$
4.  **Zero-Duty Before `PWMIOC_STOP`**:
    Because `pwm_stop()` in `rp23xx_pwm.c` only clears `RP23XX_PWM_CSR_EN` and leaves the GPIO pad frozen at its instantaneous output level, `bargraphd` always writes `duty_a = 0` and `duty_b = 0` via `PWMIOC_SETCHARACTERISTICS` + `PWMIOC_START`, waits `2 ms` (two 1 kHz PWM periods) for the counter compare registers to latch low, and only then issues `PWMIOC_STOP` and `close()`.

---

### 5. Daemon State Machine & IPC Synchronization

Because NuttX on RP2350 runs in flat build mode (`CONFIG_BUILD_FLAT=y`), `bargraphd` and transient `bargraph` NSH CLI processes share a single address space. They coordinate through a static global state structure protected by a POSIX mutex and condition variable:

```mermaid
stateDiagram-v2
    [*] --> STOPPED
    STOPPED --> SENSING : rp23xx_bringup() -> bargraph_autostart()\nor "bargraph start"
    SENSING --> MANUAL : "bargraph set <0..100>"
    MANUAL --> MANUAL : "bargraph set <0..100>"
    MANUAL --> SENSING : "bargraph resume" or "bargraph start"
    SENSING --> TEST : "bargraph test"
    MANUAL --> TEST : "bargraph test"
    TEST --> SENSING : Sweep complete (prior mode == SENSING)
    TEST --> MANUAL : Sweep complete (prior mode == MANUAL)
    SENSING --> STOPPED : "bargraph stop" (zero duty -> PWMIOC_STOP)
    MANUAL --> STOPPED : "bargraph stop" (zero duty -> PWMIOC_STOP)
```

#### Shared State Structure (`bargraph_internal.h`)

```c
enum bargraph_mode_e
{
  BARGRAPH_MODE_STOPPED = 0,
  BARGRAPH_MODE_SENSING,
  BARGRAPH_MODE_MANUAL,
  BARGRAPH_MODE_TEST
};

struct bargraph_state_s
{
  pthread_mutex_t      lock;
  pthread_cond_t        wake_cond;
  pthread_cond_t       done_cond;
  enum bargraph_mode_e mode;
  enum bargraph_mode_e prev_mode;
  bool                 running;
  bool                 stop_requested;
  bool                 test_requested;
  uint8_t              manual_pct;      /* 0..100 */
  uint16_t             raw_adc;         /* 0..4095 latest burst average */
  uint16_t             filtered_adc;    /* 0..4095 EMA + deadband clamped */
  uint16_t             millivolts;      /* 0..3300 mV */
  uint16_t             permille;        /* 0..1000 (0.0%..100.0%) */
  uint16_t             duty[6];         /* 0..65535 per-segment duty for LED1..LED6 */
  pid_t                daemon_pid;
};
```

#### CLI Subcommand Behavior (`bargraph <subcommand>`)

| Subcommand | Behavior |
| :--- | :--- |
| `bargraph start` | If `bargraphd` is not running (`STOPPED`), spawns `bargraphd` via `task_create()` and waits on `done_cond` until initialization succeeds. If already running in `MANUAL` mode, transitions to `SENSING` mode and wakes the daemon. |
| `bargraph stop` | If running, sets `stop_requested = true`, signals `wake_cond`, and waits on `done_cond` until `bargraphd` zeros all 6 PWM channels, calls `PWMIOC_STOP`, closes `/dev/pwm*` and `/dev/adc0`, and sets `running = false`. |
| `bargraph status` | Locks `g_bargraph.lock`, copies a consistent snapshot of `mode`, `raw_adc`, `filtered_adc`, `millivolts`, `permille`, and `duty[0..5]`, unlocks, and prints formatted telemetry to `stdout`. |
| `bargraph set <0..100>` | Validates integer percentage `0..100` using `strtol()`. If `bargraphd` is stopped, starts it first. Sets `manual_pct`, sets `mode = BARGRAPH_MODE_MANUAL`, signals `wake_cond`, and prints confirmation. |
| `bargraph resume` | Transitions `mode` from `BARGRAPH_MODE_MANUAL` back to `BARGRAPH_MODE_SENSING` and signals `wake_cond`. Returns an informational message if already in `SENSING` mode or an error if `STOPPED`. |
| `bargraph test` | If running, sets `test_requested = true`, signals `wake_cond`, and waits on `done_cond` while `bargraphd` sweeps total bar brightness `0% -> 100% -> 0%` in 100 steps (`20 ms`/step, `2.0 s` total), then restores `prev_mode` (`SENSING` or `MANUAL`). |

--------------------------------------------------------------------------------

## Alternatives Considered

### Alternative A: Discrete On/Off GPIO Bar Graph (`/dev/userleds` or `/dev/gpioN`)

*   **Pros**: Avoids RP23xx multi-channel PWM Kconfig guards (`CONFIG_PWM_NCHANNELS` vs. `CONFIG_RP23XX_PWM_NCHANNELS`), `struct pwm_info_s` channel indexing quirks, and 16-bit `uint16_t` duty truncation at `b16ONE`.
*   **Cons**: Limits the 6-LED display to 7 coarse discrete steps (`0` through `6` LEDs lit) with abrupt jumps at each `16.7%` threshold, hiding sub-segment potentiometer movement.
*   **Reason for Rejection**: Rejected by author: hardware PWM on `D1`–`D6` (`PWM5B`, `PWM6A`, `PWM2B`, `PWM3A`, `PWM3B`, `PWM0A`) provides continuous proportional brightness modulation on the active segment while locking lower segments at 100% duty and upper segments at 0% duty.

### Alternative B: Foreground Application Entrypoint (`CONFIG_INIT_ENTRYPOINT="bargraph_main"`)

*   **Pros**: Starts the bar graph loop directly as the initial user task without needing a separate `bargraph_autostart()` hook in `rp23xx_bringup()`.
*   **Cons**: Occupying the foreground init task either prevents NuttShell (`nsh_main`) from running on the USB-C CDC/ACM console or forces `bargraph_main` to duplicate NSH initialization and session management.
*   **Reason for Rejection**: Rejected by author: keeping `CONFIG_INIT_ENTRYPOINT="nsh_main"` while spawning `bargraphd` as a background daemon preserves standard interactive USB-C NSH behavior alongside continuous 50 Hz LED updates.

### Alternative C: Launching `bargraphd` Exclusively from a ROMFS Startup Script (`/etc/init.d/rcS`)

*   **Pros**: Uses the standard NSH shell script mechanism (`bargraph start`) without calling an application startup function from `rp23xx_bringup()`.
*   **Cons**: In `apps/nshlib/nsh_usbconsole.c`, `nsh_consolemain()` blocks inside `nsh_waitusbready()` until a host computer opens `/dev/ttyACM0` and sends three `\r` or `\n` characters before executing `nsh_initscript()`. If the board is powered from a standalone 5V USB wall adapter without a host terminal, `/etc/init.d/rcS` never runs and the LEDs remain dark. In addition, `nuttx/boards/Board.mk` requires host `genromfs` and `xxd` binaries to build `etctmp.c`.
*   **Reason for Rejection**: Rejected by author: spawning `bargraphd` at the end of `rp23xx_bringup()` during `board_late_initialize()` guarantees immediate power-on operation before USB enumeration and removes external host build tool dependencies.

### Alternative D: Reusing `bargraph start` Alone Without a Dedicated `bargraph resume` Subcommand

*   **Pros**: Reduces the CLI surface by one subcommand name.
*   **Cons**: When the daemon is already running in `MANUAL` override mode after `bargraph set <0..100>`, typing `bargraph start` is less explicit to an interactive user than `bargraph resume`.
*   **Reason for Rejection**: Rejected by author: supporting both `bargraph resume` and `bargraph start` to exit `MANUAL` mode back to `SENSING` mode makes interactive CLI usage explicit while remaining forgiving.

--------------------------------------------------------------------------------

## Non-Functional Requirements

*   **Performance & CPU Utilization**:
    *   Setting `RP23XX_ADC_DIV = 0x00FFFF00` reduces the free-running ADC hardware conversion interrupt rate from `500,000 IRQ/s` to `~732.42 IRQ/s` (`99.85%` reduction).
    *   `bargraphd` runs at `50 Hz` (`20 ms` cycle period), reading a 4-sample burst (`~5.5 ms` acquisition window) and issuing 5 `PWMIOC_SETCHARACTERISTICS` ioctls per frame (`< 1%` Cortex-M33 CPU utilization at `150 MHz`).
*   **Electrical Safety & Pad Integrity**:
    *   Series `330 Ω` resistors limit each LED to `~3.94 mA` at `100%` duty (`~23.6 mA` total across all 6 LEDs), staying below the RP2350 per-pin (`12 mA`) and total `IOVDD` (`200 mA`) limits.
    *   Disabling `IE`, `PUE`, `PDE`, and `ISO` and enabling `OD` on `RP23XX_PADS_BANK0_GPIO(26)` eliminates RP2350-E9 digital input buffer leakage on the analog potentiometer input.
*   **Repository Hygiene & Reproducibility**:
    *   `git status --porcelain` and `git diff` inside `nuttx/` and `apps/` remain empty after configuration, build, and `distclean`.

--------------------------------------------------------------------------------

### Additional Resources

*   `nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c` — RP2350 PWM lower-half driver (`setup_pulse()`, `pwm_stop()`, `uint16_t` duty calculation).
*   `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` — RP2350 ADC lower-half driver (`my_reset()`, `adc_interrupt()`, `<< 19` sample shift).
*   `nuttx/boards/arm/rp23xx/common/src/rp23xx_common_bringup.c` — Common RP23xx peripheral bringup (`rp23xx_pwmdev_initialize()`, `rp23xx_adc_setup()`).
*   `apps/nshlib/nsh_usbconsole.c` — USB CDC/ACM console startup sequence (`nsh_waitusbready()`).
*   Raspberry Pi RP2350 Datasheet — Section 12.4 (ADC & Erratum RP2350-E9 Pad Leakage) and Section 12.5 (PWM Slices & Counter Compare Registers).

### Future Improvements

*   **Perceptual Gamma Correction Table**: Add an optional CLI toggle (`bargraph gamma on|off`) applying a CIE 1931 lightness curve ($\gamma \approx 2.2$) to the active segment's `0..65535` PWM duty so human-perceived brightness scales linearly with wiper angle.
*   **Peak-Hold Indicator Mode**: Allow the highest reached LED segment (`D6` down to `D1`) to hold briefly at `25%` duty cycle for `1.0 s` after rapid downward potentiometer sweeps.

--------------------------------------------------------------------------------

## Detailed Implementation Plan

### Target Files to Create/Modify

#### 1. `README.md` (Modify)

*   **Type / Scope**: Top-Level Workspace EGM Blueprint Documentation
*   **Rationale**: Registers the new `xiao-rp2350-bargraph` out-of-tree project directory in the workspace root blueprint alongside `apps` and `nuttx`.
*   **Proposed Changes**:
    *   Add `- xiao-rp2350-bargraph: [./xiao-rp2350-bargraph/README.md](./xiao-rp2350-bargraph/README.md) - Out-of-tree Seeed Studio XIAO RP2350 custom board support package (board) and 6-segment hardware PWM LED bar graph application (apps/bargraph).` under `## Subdirectories & System Boundaries`.

#### 2. `xiao-rp2350-bargraph/README.md` (Create)

*   **Type / Scope**: Project Root EGM Blueprint & Hardware Assembly Guide
*   **Rationale**: Documents the top-level boundaries between `board` (`./xiao-rp2350-bargraph/board/README.md`) and `apps` (`./xiao-rp2350-bargraph/apps/README.md`), the bill of materials, the 400-tie-point breadboard layout, pinout table, current budget, build/flash commands (`nuttx.uf2`), and NSH CLI usage.
*   **Proposed Changes**:
    *   Include `## Overview`, `## Subdirectories & Boundaries` linking to `./board/README.md` and `./apps/README.md`, `## Hardware Bill of Materials & Breadboard Wiring`, `## Invariants`, `## Side Effects`, and `## Verification`.

#### 3. `xiao-rp2350-bargraph/board/README.md` (Create)

*   **Type / Scope**: Board Support Package EGM Blueprint Documentation
*   **Rationale**: Documents the out-of-tree BSP directory hierarchy (`configs`, `include`, `scripts`, `src`) and its integration with `nuttx/boards/arm/rp23xx/common`.
*   **Proposed Changes**:
    *   Link to `./configs/README.md`, `./include/README.md`, `./scripts/README.md`, and `./src/README.md` and document `Kconfig` symbol `ARCH_BOARD_XIAO_RP2350_BARGRAPH`.

#### 4. `xiao-rp2350-bargraph/board/Kconfig` (Create)

*   **Type / Scope**: Board Kconfig Definition
*   **Rationale**: Satisfies `CUSTOM_BOARD_KPATH = $(BOARD_DIR)/Kconfig` in `nuttx/tools/Config.mk` when `CONFIG_ARCH_BOARD_CUSTOM=y`.
*   **Proposed Changes**:
    *   Define `if ARCH_BOARD_CUSTOM` block (or empty Kconfig guard) so `nuttx` Kconfig parsing succeeds without falling back to `dummy_kconfig`.

#### 5. `xiao-rp2350-bargraph/board/configs/README.md` (Create)

*   **Type / Scope**: Board Configurations EGM Index Documentation
*   **Rationale**: Indexes available build configurations under `xiao-rp2350-bargraph/board/configs/`.
*   **Proposed Changes**:
    *   Link to `./bargraph/README.md` and describe the `bargraph` target configuration.

#### 6. `xiao-rp2350-bargraph/board/configs/bargraph/README.md` (Create)

*   **Type / Scope**: Leaf EGM Documentation for `bargraph` Defconfig
*   **Rationale**: Documents the Kconfig invariants and peripheral selections in `xiao-rp2350-bargraph/board/configs/bargraph/defconfig`.
*   **Proposed Changes**:
    *   Document the `CONFIG_BOARD_LATE_INITIALIZE=y`, multi-channel PWM, single-channel ADC0, USB CDC/ACM console, and `CONFIG_BARGRAPH=y` invariants.

#### 7. `xiao-rp2350-bargraph/board/configs/bargraph/defconfig` (Create)

*   **Type / Scope**: NuttX Board Build Configuration (`defconfig`)
*   **Rationale**: Configures the RP2350 kernel, USB CDC/ACM NSH console, ADC0, 5 multi-channel PWM slices, custom board paths, and built-in `bargraph` app without touching `nuttx/boards/arm/rp23xx/xiao-rp2350/configs/usbnsh/defconfig`.
*   **Proposed Changes**:
    *   Base on `xiao-rp2350/configs/usbnsh/defconfig` with the following critical symbols:
        *   Custom board routing: `CONFIG_ARCH_BOARD_CUSTOM=y`, `CONFIG_ARCH_BOARD_CUSTOM_NAME="xiao-rp2350-bargraph"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`, `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y`, `CONFIG_ARCH_BOARD_COMMON=y`.
        *   Late board bringup: `CONFIG_BOARD_LATE_INITIALIZE=y`.
        *   Multi-channel PWM (both upper-half and arch symbols): `CONFIG_PWM=y`, `CONFIG_PWM_MULTICHAN=y`, `CONFIG_PWM_NCHANNELS=2`, `CONFIG_RP23XX_PWM=y`, `CONFIG_RP23XX_PWM_MULTICHAN=y`, `CONFIG_RP23XX_PWM_NCHANNELS=2`.
        *   PWM slices & GPIO pins:
            *   `CONFIG_RP23XX_PWM0=y`, `CONFIG_RP23XX_PWM0A_GPIO=0`, `CONFIG_RP23XX_PWM0B_GPIO=-1` (`D6` / `LED6`)
            *   `CONFIG_RP23XX_PWM2=y`, `CONFIG_RP23XX_PWM2A_GPIO=-1`, `CONFIG_RP23XX_PWM2B_GPIO=5` (`D3` / `LED3`)
            *   `CONFIG_RP23XX_PWM3=y`, `CONFIG_RP23XX_PWM3A_GPIO=6`, `CONFIG_RP23XX_PWM3B_GPIO=7` (`D4` / `LED4` and `D5` / `LED5`)
            *   `CONFIG_RP23XX_PWM5=y`, `CONFIG_RP23XX_PWM5A_GPIO=-1`, `CONFIG_RP23XX_PWM5B_GPIO=27` (`D1` / `LED1`)
            *   `CONFIG_RP23XX_PWM6=y`, `CONFIG_RP23XX_PWM6A_GPIO=28`, `CONFIG_RP23XX_PWM6B_GPIO=-1` (`D2` / `LED2`)
        *   ADC0 only: `CONFIG_ADC=y`, `CONFIG_ADC_FIFOSIZE=16`, `CONFIG_RP23XX_ADC=y`, `CONFIG_RP23XX_ADC_CHANNEL0=y` (with `CHANNEL1`, `CHANNEL2`, `CHANNEL3`, and `TEMPERATURE` unset).
        *   Application enable: `CONFIG_BARGRAPH=y`, `CONFIG_BARGRAPH_PRIORITY=100`, `CONFIG_BARGRAPH_STACKSIZE=2048`.

#### 8. `xiao-rp2350-bargraph/board/include/README.md` (Create)

*   **Type / Scope**: Leaf EGM Documentation for Board Public Headers
*   **Rationale**: Documents the clock, pin, and prototype declarations exported in `board.h`.
*   **Proposed Changes**:
    *   Document `[board.h:BOARD_SYS_FREQ]`, `[board.h:BARGRAPH_ADC_GPIO]`, `[board.h:BARGRAPH_LED1_GPIO]`..`[board.h:BARGRAPH_LED6_GPIO]`, `[board.h:rp23xx_boardearlyinitialize]`, and `[board.h:rp23xx_boardinitialize]`.

#### 9. `xiao-rp2350-bargraph/board/include/board.h` (Create)

*   **Type / Scope**: C Header File (`<arch/board/board.h>`)
*   **Rationale**: Provides clock definitions (`150 MHz` `BOARD_SYS_FREQ`, `48 MHz` `BOARD_ADC_FREQ`) required by `arch/arm/src/rp23xx` and `boards/arm/rp23xx/common`, plus named constants for the bargraph potentiometer and LED GPIOs/devices.
*   **Proposed Changes**:
    *   Retain standard XIAO RP2350 clock macros and on-board `GPIO25` status LED definitions.
    *   Define `BARGRAPH_ADC_DEVPATH "/dev/adc0"`, `BARGRAPH_ADC_GPIO 26`, and LED segment pin/device macros (`BARGRAPH_LED1_GPIO 27` through `BARGRAPH_LED6_GPIO 0`).
    *   Declare `void rp23xx_boardearlyinitialize(void)` and `void rp23xx_boardinitialize(void)`.

#### 10. `xiao-rp2350-bargraph/board/scripts/README.md` (Create)

*   **Type / Scope**: Leaf EGM Documentation for Board Build Scripts & Linker Script
*   **Rationale**: Documents the toolchain flags, `memmap_default.ld` flash layout, and the `apps/external` symlink hook in `Make.defs`.
*   **Proposed Changes**:
    *   Document `[Make.defs:LDSCRIPT]`, `[Make.defs:apps/external]`, and `[memmap_default.ld:MEMORY]`.

#### 11. `xiao-rp2350-bargraph/board/scripts/Make.defs` (Create)

*   **Type / Scope**: Board Top-Level Makefile Definitions
*   **Rationale**: Copied to `nuttx/Make.defs` during configuration; configures ARMv8-M compiler flags, points `ARCHSCRIPT` to `memmap_default.ld`, and creates the git-ignored `apps/external -> ../xiao-rp2350-bargraph/apps` symlink before `apps/Make.defs` scans `$(APPDIR)/*/Make.defs`.
*   **Proposed Changes**:
    *   Include `$(TOPDIR)/.config`, `$(TOPDIR)/tools/Config.mk`, `$(TOPDIR)/tools/rp23xx/Config.mk`, and `$(TOPDIR)/arch/arm/src/armv8-m/Toolchain.defs`.
    *   Ensure `$(TOPDIR)/../apps/external` symlink points to `../xiao-rp2350-bargraph/apps` via `$(shell ...)`.
    *   Append `$(BOARD_DIR)/scripts/$(LDSCRIPT)` to `ARCHSCRIPT`.

#### 12. `xiao-rp2350-bargraph/board/scripts/memmap_default.ld` (Create)

*   **Type / Scope**: GNU LD Linker Script
*   **Rationale**: Defines the RP2350 2 MB QSPI XIP flash (`0x10000000`) and 520 KB SRAM (`0x20000000` + `SCRATCH_X`/`SCRATCH_Y`) memory layout and Picobin `IMAGE_DEF` block required by the RP2350 bootrom.
*   **Proposed Changes**:
    *   Provide the standard XIAO RP2350 flash/SRAM linker script matching `nuttx/boards/arm/rp23xx/xiao-rp2350/scripts/memmap_default.ld`.

#### 13. `xiao-rp2350-bargraph/board/src/README.md` (Create)

*   **Type / Scope**: Leaf EGM Documentation for Board Source Implementation
*   **Rationale**: Documents the board initialization functions, RP2350-E9 pad register fixup, ADC clock divider setup, and `bargraph_autostart()` invocation in `rp23xx_bringup.c`.
*   **Proposed Changes**:
    *   Document interfaces, invariants, and side effects for `Make.defs`, `rp23xx_pico.h`, `rp23xx_boardinitialize.c`, `rp23xx_bringup.c`, `rp23xx_autoleds.c`, `rp23xx_userleds.c`, and `rp23xx_gpio.c`.

#### 14. `xiao-rp2350-bargraph/board/src/Make.defs` (Create)

*   **Type / Scope**: Board Source Makefile Fragment
*   **Rationale**: Included as `board/Make.defs` by `nuttx/boards/arm/rp23xx/common/Makefile` to compile custom board C files into `libboard.a`.
*   **Proposed Changes**:
    *   Add `CSRCS = rp23xx_boardinitialize.c rp23xx_bringup.c` plus conditional `rp23xx_gpio.c`, `rp23xx_autoleds.c`, and `rp23xx_userleds.c`.
    *   Set `DEPPATH += --dep-path board`, `VPATH += :board`, and `CFLAGS += ${INCDIR_PREFIX}$(TOPDIR)/arch/$(CONFIG_ARCH)/src/board/board`.

#### 15. `xiao-rp2350-bargraph/board/src/rp23xx_pico.h` (Create)

*   **Type / Scope**: Internal Board Header
*   **Rationale**: Declares `GPIO_LED1` (`25`), `int rp23xx_bringup(void)`, and `int bargraph_autostart(void)` for board bringup.
*   **Proposed Changes**:
    *   Declare `rp23xx_bringup(void)` and weak/conditional `bargraph_autostart(void)` under `#ifdef CONFIG_BARGRAPH`.

#### 16. `xiao-rp2350-bargraph/board/src/rp23xx_boardinitialize.c` (Create)

*   **Type / Scope**: Board Early and Late Initialization C Source
*   **Rationale**: Implements `rp23xx_boardearlyinitialize()`, `rp23xx_boardinitialize()`, and `board_late_initialize()` (which invokes `rp23xx_bringup()` when `CONFIG_BOARD_LATE_INITIALIZE=y`).
*   **Proposed Changes**:
    *   Call `rp23xx_common_earlyinitialize()`, initialize onboard LED `GPIO25`, call `rp23xx_common_initialize()`, and call `rp23xx_bringup()` inside `board_late_initialize()`.

#### 17. `xiao-rp2350-bargraph/board/src/rp23xx_bringup.c` (Create)

*   **Type / Scope**: Board Peripheral Bringup & Daemon Autostart C Source
*   **Rationale**: Executes after kernel hardware init and before `nsh_main`. Registers `/dev/pwm*` and `/dev/adc0` via `rp23xx_common_bringup()`, applies the RP2350-E9 analog pad isolation on `GPIO26` and the `~732 Hz` ADC clock divider (`RP23XX_ADC_DIV = 0x00FFFF00`), and spawns `bargraphd` via `bargraph_autostart()`.
*   **Proposed Changes**:
    *   Call `rp23xx_common_bringup()`.
    *   Modify `RP23XX_PADS_BANK0_GPIO(26)` via `modreg32(RP23XX_PADS_BANK0_GPIO_OD, RP23XX_PADS_BANK0_GPIO_IE | RP23XX_PADS_BANK0_GPIO_PUE | RP23XX_PADS_BANK0_GPIO_PDE | RP23XX_PADS_BANK0_GPIO_ISO, RP23XX_PADS_BANK0_GPIO(26))`.
    *   Write `putreg32(0x00ffff00, RP23XX_ADC_DIV)`.
    *   If `CONFIG_BARGRAPH` is defined, call `bargraph_autostart()` and log any error via `syslog(LOG_ERR, ...)`.

#### 18. `xiao-rp2350-bargraph/board/src/rp23xx_autoleds.c` (Create)

*   **Type / Scope**: Board OS Status LED Driver (`CONFIG_ARCH_LEDS`)
*   **Rationale**: Provides `board_autoled_initialize()`, `board_autoled_on()`, and `board_autoled_off()` on onboard `GPIO25` when `CONFIG_ARCH_LEDS=y`.
*   **Proposed Changes**:
    *   Implement standard XIAO RP2350 active-low status LED control on `GPIO25`.

#### 19. `xiao-rp2350-bargraph/board/src/rp23xx_userleds.c` (Create)

*   **Type / Scope**: Board User LED Driver (`!CONFIG_ARCH_LEDS`)
*   **Rationale**: Required by `xiao-rp2350-bargraph/board/src/Make.defs` when `CONFIG_ARCH_LEDS` is not set so `board_userled_*` symbols resolve cleanly.
*   **Proposed Changes**:
    *   Implement `board_userled_initialize()`, `board_userled()`, and `board_userled_all()` for onboard `GPIO25`.

#### 20. `xiao-rp2350-bargraph/board/src/rp23xx_gpio.c` (Create)

*   **Type / Scope**: Optional Board GPIO Driver (`CONFIG_DEV_GPIO`)
*   **Rationale**: Provides `rp23xx_dev_gpio_init()` if `CONFIG_DEV_GPIO` is enabled.
*   **Proposed Changes**:
    *   Guard with `#if defined(CONFIG_DEV_GPIO) && !defined(CONFIG_GPIO_LOWER_HALF)`.

#### 21. `xiao-rp2350-bargraph/apps/README.md` (Create)

*   **Type / Scope**: External Applications Root EGM Blueprint Documentation
*   **Rationale**: Documents the out-of-tree application container linked at `apps/external` and links to `./bargraph/README.md`.
*   **Proposed Changes**:
    *   Describe the `apps/external` symlink discovery mechanism, `Kconfig` inclusion, and `bargraph` (`./xiao-rp2350-bargraph/apps/bargraph/README.md`) subcomponent.

#### 22. `xiao-rp2350-bargraph/apps/Kconfig` (Create)

*   **Type / Scope**: External Applications Kconfig Aggregator
*   **Rationale**: Sourced by `apps/Kconfig` (via `apps/tools/mkkconfig.sh`) when linked as `apps/external/Kconfig`.
*   **Proposed Changes**:
    *   Contain `menu "External Applications"` and `source "$APPSDIR/external/bargraph/Kconfig"` followed by `endmenu`.

#### 23. `xiao-rp2350-bargraph/apps/Makefile` (Create)

*   **Type / Scope**: External Applications Directory Makefile
*   **Rationale**: Delegates `all`, `clean`, and `distclean` across subdirectories of `xiao-rp2350-bargraph/apps/` via `$(APPDIR)/Directory.mk`.
*   **Proposed Changes**:
    *   Leave `MENUDESC` unset (so `Directory.mk` `distclean` does not delete the tracked `xiao-rp2350-bargraph/apps/Kconfig`) and `include $(APPDIR)/Directory.mk`.

#### 24. `xiao-rp2350-bargraph/apps/Make.defs` (Create)

*   **Type / Scope**: External Applications Make Discovery Fragment
*   **Rationale**: Included by `apps/Make.defs` via `$(wildcard $(APPDIR)/*/Make.defs)` to include each child application's `Make.defs`.
*   **Proposed Changes**:
    *   Include `$(wildcard $(APPDIR)/external/*/Make.defs)`.

#### 25. `xiao-rp2350-bargraph/apps/bargraph/README.md` (Create)

*   **Type / Scope**: Leaf EGM Documentation for `bargraph` Application & Daemon
*   **Rationale**: Documents the public entrypoints (`bargraph_main`, `bargraph_autostart`), state machine invariants, ADC/PWM ioctl contracts, and verification commands.
*   **Proposed Changes**:
    *   Document `## Overview`, `## Interface`, `## Invariants`, `## Side Effects`, and `## Verification`.

#### 26. `xiao-rp2350-bargraph/apps/bargraph/Kconfig` (Create)

*   **Type / Scope**: Application Kconfig Specification
*   **Rationale**: Defines `CONFIG_BARGRAPH`, `CONFIG_BARGRAPH_PROGNAME`, `CONFIG_BARGRAPH_PRIORITY`, and `CONFIG_BARGRAPH_STACKSIZE`.
*   **Proposed Changes**:
    *   Define `config BARGRAPH` (`bool "Seeed XIAO RP2350 6-LED PWM Bar Graph"`, `default n`), `BARGRAPH_PROGNAME` (`default "bargraph"`), `BARGRAPH_PRIORITY` (`default 100`), and `BARGRAPH_STACKSIZE` (`default 2048`).

#### 27. `xiao-rp2350-bargraph/apps/bargraph/Make.defs` (Create)

*   **Type / Scope**: Application Build Registration Fragment
*   **Rationale**: Appends `$(APPDIR)/external/bargraph` to `CONFIGURED_APPS` when `CONFIG_BARGRAPH=y`.
*   **Proposed Changes**:
    *   Guard `CONFIGURED_APPS += $(APPDIR)/external/bargraph` with `ifneq ($(CONFIG_BARGRAPH),)`.

#### 28. `xiao-rp2350-bargraph/apps/bargraph/Makefile` (Create)

*   **Type / Scope**: Built-in Application Makefile
*   **Rationale**: Compiles `bargraph_main.c` and registers the `bargraph` built-in NSH command via `$(APPDIR)/Application.mk`.
*   **Proposed Changes**:
    *   Set `PROGNAME = $(CONFIG_BARGRAPH_PROGNAME)`, `PRIORITY = $(CONFIG_BARGRAPH_PRIORITY)`, `STACKSIZE = $(CONFIG_BARGRAPH_STACKSIZE)`, `MODULE = $(CONFIG_BARGRAPH)`, `MAINSRC = bargraph_main.c`, and `include $(APPDIR)/Application.mk`.

#### 29. `xiao-rp2350-bargraph/apps/bargraph/bargraph_internal.h` (Create)

*   **Type / Scope**: Application Internal Header
*   **Rationale**: Centralizes hardware constants, filter/deadband thresholds, PWM slice mapping structures, shared state `struct bargraph_state_s`, and function prototypes.
*   **Proposed Changes**:
    *   Define constants:
        *   `BARGRAPH_NUM_LEDS 6`
        *   `BARGRAPH_NUM_PWM_SLICES 5`
        *   `BARGRAPH_PWM_FREQ_HZ 1000`
        *   `BARGRAPH_DUTY_MAX 65535u`
        *   `BARGRAPH_ADC_MAX 4095u`
        *   `BARGRAPH_ADC_DEADBAND_LOW 32u`
        *   `BARGRAPH_ADC_DEADBAND_HIGH 4063u`
        *   `BARGRAPH_ADC_BURST_SAMPLES 4`
        *   `BARGRAPH_ADC_SHIFT 19`
        *   `BARGRAPH_TICK_USEC 20000` (`50 Hz`)
    *   Define `enum bargraph_mode_e` and `struct bargraph_state_s`.
    *   Declare `int bargraph_autostart(void)` and `int bargraph_main(int argc, FAR char *argv[])`.

#### 30. `xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c` (Create)

*   **Type / Scope**: Daemon (`bargraphd`) and NSH CLI (`bargraph`) C Implementation
*   **Rationale**: Implements autonomous startup (`bargraph_autostart()`), the 50 Hz ADC sampling and 6-segment PWM update loop (`bargraphd_daemon()`), pure duty-cycle transfer math, and the NSH CLI command parser (`bargraph_main()`).
*   **Proposed Changes**:
    *   **Pure Transfer Function (`bargraph_compute_duties`)**:
        *   Input: `uint32_t total_units` in `0 .. (6 * 65535)` (`0 .. 393210`), output: `uint16_t duties[6]`.
        *   For each segment `i` in `0..5`: compute `seg_base = i * 65535u`; if `total_units <= seg_base`, `duties[i] = 0`; else if `total_units >= seg_base + 65535u`, `duties[i] = 65535u`; else `duties[i] = (uint16_t)(total_units - seg_base)`.
    *   **PWM Slice Update (`bargraph_apply_pwm`)**:
        *   Maintains 5 open file descriptors for `/dev/pwm0`, `/dev/pwm2`, `/dev/pwm3`, `/dev/pwm5`, `/dev/pwm6`.
        *   Populates `struct pwm_info_s` with `frequency = 1000`, `channels[0].channel = 1`, `channels[1].channel = 2`, and maps:
            *   `/dev/pwm5`: `channels[0].duty = 0`, `channels[1].duty = duties[0]` (`LED1` / `D1`)
            *   `/dev/pwm6`: `channels[0].duty = duties[1]` (`LED2` / `D2`), `channels[1].duty = 0`
            *   `/dev/pwm2`: `channels[0].duty = 0`, `channels[1].duty = duties[2]` (`LED3` / `D3`)
            *   `/dev/pwm3`: `channels[0].duty = duties[3]` (`LED4` / `D4`), `channels[1].duty = duties[4]` (`LED5` / `D5`)
            *   `/dev/pwm0`: `channels[0].duty = duties[5]` (`LED6` / `D6`), `channels[1].duty = 0`
        *   Calls `ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)&info)` and (when not yet started or on update) `ioctl(fd, PWMIOC_START, 0)`.
    *   **Safe Shutdown (`bargraph_shutdown_pwm`)**:
        *   Writes all zeros (`duties[0..5] = 0`) via `bargraph_apply_pwm()`, sleeps `2000 us` (`usleep(2000)`), then issues `ioctl(fd, PWMIOC_STOP, 0)` and `close(fd)` on all 5 PWM descriptors.
    *   **ADC Burst Read & Filter (`bargraph_sample_adc`)**:
        *   Calls `ioctl(adc_fd, ANIOC_RESET_FIFO, 0)`.
        *   Reads `4` `struct adc_msg_s` entries from `adc_fd`, validates `msg.am_data >= 0`, extracts `(uint16_t)((msg.am_data >> 19) & 0x0fff)`, averages the 4 readings, updates the Q4 EMA accumulator, and applies the `<= 32` -> `0` and `>= 4063` -> `4095` deadbands.
    *   **Daemon Main Loop (`bargraphd_daemon`)**:
        *   Opens `/dev/adc0` (`O_RDONLY`) and `/dev/pwm0,2,3,5,6` (`O_RDONLY`), signals `done_cond` that startup succeeded, and loops at 50 Hz using `pthread_cond_timedwait()` (`CLOCK_REALTIME` or `usleep` + mutex lock) to service `SENSING`, `MANUAL`, `TEST`, and `stop_requested` transitions immediately.
    *   **CLI Entrypoint (`bargraph_main`)**:
        *   Parses `start`, `stop`, `status`, `test`, `set <0..100>`, and `resume`; prints usage help on invalid arguments and returns `EXIT_SUCCESS` (`0`) or `EXIT_FAILURE` (`1`).

---

### Verification Plan

*   **Out-of-Tree Configuration & Firmware Build**:
    *   Run from repository root:
        ```bash
        cd nuttx
        ./tools/configure.sh -l ../xiao-rp2350-bargraph/board/configs/bargraph
        make -j$(sysctl -n hw.ncpu)
        ```
    *   Verify `nuttx/nuttx.elf`, `nuttx/nuttx.bin`, and `nuttx/nuttx.uf2` (when `picotool` or UF2 conversion is enabled) are generated with zero compiler warnings in `xiao-rp2350-bargraph/`.
*   **Submodule Cleanliness Check**:
    *   Run `git -C nuttx status --porcelain` and `git -C apps status --porcelain` and verify both produce empty output (zero modified or untracked files in either submodule).
*   **Host-Side Pure Math & Transfer Function Unit Verification**:
    *   Verify the 6-segment duty cycle transfer function (`0%`, `16.7%`, `33.3%`, `50.0%`, `66.7%`, `83.3%`, `100.0%`), `>> 19` ADC sample extraction, Q4 EMA filter convergence, and endpoint deadband clamping across boundary values (`0`, `32`, `33`, `2048`, `4062`, `4063`, `4095`), confirming no duty value ever exceeds `65535` (`0xFFFF`).
*   **Hardware-in-the-Loop Breadboard Verification**:
    *   Flash `nuttx.uf2` to the Seeed Studio XIAO RP2350 in BOOTSEL mode (`RPI-RP2` mass storage volume).
    *   Verify standalone power-on operation (bar graph tracks potentiometer immediately prior to opening `/dev/ttyACM0`).
    *   Connect to `/dev/ttyACM0` (`115200 8N1`), press Enter three times to open `nsh>`, and verify `bargraph status`, `bargraph set 0`, `bargraph set 50`, `bargraph set 100`, `bargraph test`, `bargraph resume`, `bargraph stop`, and `bargraph start`.

--------------------------------------------------------------------------------

<!-- Provenance: Indicates initial structural drafting via EGM. -->

*Drafted with EGM*
