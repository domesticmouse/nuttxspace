# Out-of-Tree Seeed Studio XIAO RP2350 6-Segment PWM LED Bar Graph (`xiao-rp2350-bargraph`)

## Overview

`xiao-rp2350-bargraph` is a self-contained out-of-tree Apache NuttX board support package (`board/`) and user-space application (`apps/bargraph/`) for a Seeed Studio XIAO RP2350 wired on a 400-tie-point solderless breadboard with one `10 kΩ` linear potentiometer (`D0/A0`, `GPIO26` / `/dev/adc0`) and six active-high LEDs (`D1`–`D6`, `GPIO27`, `GPIO28`, `GPIO5`, `GPIO6`, `GPIO7`, `GPIO0` across `/dev/pwm0`, `/dev/pwm2`, `/dev/pwm3`, `/dev/pwm5`, and `/dev/pwm6`). At power-on, the firmware initializes the ADC and PWM peripherals during `board_late_initialize()` and spawns the `bargraphd` background daemon before `nsh_main` blocks waiting for a USB-C CDC/ACM host console connection. Both `nuttx/` and `apps/` git submodules remain unmodified.

## Subdirectories & Boundaries

- `board`: [./board/README.md](./board/README.md) - Out-of-tree custom board support package (`CONFIG_ARCH_BOARD_CUSTOM=y`) providing the `bargraph` target configuration (`configs/bargraph/defconfig`), public board clock/pin definitions (`include/board.h`), linker script and `apps/external` symlink hook (`scripts/`), and RP2350 peripheral bringup with RP2350-E9 analog pad isolation and daemon autostart (`src/`).
- `apps`: [./apps/README.md](./apps/README.md) - Out-of-tree user-space application root linked at build time as `apps/external`, housing the `bargraph` built-in NSH CLI command and `bargraphd` 50 Hz ADC-to-PWM background daemon (`apps/bargraph/`).

## Hardware Bill of Materials & Breadboard Wiring

See [BREADBOARD_WIRING.md](./BREADBOARD_WIRING.md) and [breadboard-wiring.svg](./breadboard-wiring.svg) for the step-by-step coordinate assembly table and visual SVG diagram.

### Bill of Materials

- 1x Seeed Studio XIAO RP2350 (with 2x 7-pin male header strips soldered)
- 1x 400-tie-point solderless breadboard
- 1x `10 kΩ` linear rotary potentiometer (`B10K`, 3-pin breadboard footprint)
- 6x 3 mm or 5 mm diffused through-hole LEDs (`LED1`–`LED6`, standard red/green/yellow, $V_F \approx 2.0\text{ V}$)
- 6x `330 Ω` $\pm 1\%$ or $\pm 5\%$ through-hole metal/carbon film resistors ($1/4\text{ W}$)
- Optional: 1x `100 nF` ceramic decoupling capacitor between `D0/A0` (`GPIO26`) and `GND`
- Assorted 22 AWG solid-core or M-M breadboard jumper wires and 1x USB-C data/power cable

### Pin Assignment & Peripheral Mapping

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

### 400-Tie-Point Breadboard Layout

```text
       +3.3V Rail (Red +)                         GND Rail (Blue -)
         |                                          |
  Row 1: +---[ Potentiometer Lug 1 (CW / 3.3V) ]    |
  Row 2:     [ Potentiometer Lug 2 (Wiper) ]--------+---> Jumper to Row 5A (D0/A0, GPIO26)
  Row 3:     [ Potentiometer Lug 3 (CCW / GND) ]----+     (Optional 100 nF cap: Row 5B to GND)
                                                    |
                   Cols A..E     Center    Cols F..J|
  Row 5:     [D0/A0  GPIO26 (5E)]  ||  [(5F) 5V    ] (Unconnected)
  Row 6:     [D1/A1  GPIO27 (6E)]  ||  [(6F) GND   ]+---> Jumper Row 6J to Blue (-) GND Rail
  Row 7:     [D2/A2  GPIO28 (7E)]  ||  [(7F) 3V3   ]+---> Jumper Row 7J to Red (+) 3V3 Rail
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

### Electrical Current Budget

- Per-LED current at 100% duty ($V_{DD} = 3.3\text{ V}$, $V_F \approx 2.0\text{ V}$, $R = 330\ \Omega$): $I_{\text{LED}} \approx 3.94\text{ mA}$ (below the RP2350 12 mA per-pin limit).
- Total 6-LED current at 100% duty: $6 \times 3.94\text{ mA} \approx 23.6\text{ mA}$ (well below the RP2350 200 mA total `IOVDD` budget).
- Potentiometer current (`10 kΩ`): $3.3\text{ V} / 10\text{ k}\Omega = 0.33\text{ mA}$ ($R_S \le 2.5\text{ k}\Omega$ Thevenin impedance at mid-scale).

## Invariants

- Zero Submodule Modifications: All custom board configuration, peripheral bringup, and application code reside inside `xiao-rp2350-bargraph/` so `git -C nuttx status --porcelain` and `git -C apps status --porcelain` remain empty across configure, build, and `distclean` `[Makefile:configure]`, `[board/scripts/Make.defs:apps/external]`, `[apps/Makefile:distclean]`.
- Intentional `CONFIG_BUILD_FLAT=y` Boundary Exceptions: To keep `nuttx/` and `apps/` completely unmodified while mitigating the `rp23xx_adc.c` 500 kS/s self-retriggering `START_ONCE` interrupt storm and `nsh_usbconsole.c` host-wait blocking, `bargraphd` calls `up_enable_irq(RP23XX_ADC_IRQ_FIFO)` / `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` directly and `rp23xx_bringup()` invokes `bargraph_autostart()` directly as documented flat-build exceptions to the workspace Kernel/User-Space Separation rule in [`README.md`](../README.md#L16) `[board/src/rp23xx_bringup.c:rp23xx_bringup]`, `[apps/bargraph/bargraph_main.c:bargraph_sample_adc]`.
- 16-Bit Safe PWM Duty Range: All segment duty cycles are bounded to `0..65535` (`BARGRAPH_DUTY_MAX`), avoiding the `uint16_t` truncation of `b16ONE` (`65536` -> `0`) in `nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c` `[apps/bargraph/bargraph_internal.h:BARGRAPH_DUTY_MAX]`, `[apps/bargraph/bargraph_main.c:bargraph_compute_duties]`.

## Side Effects

- Out-of-Tree Symlink & Build Artifacts: Running `make -C xiao-rp2350-bargraph` (or `./tools/configure.sh ../xiao-rp2350-bargraph/board/configs/bargraph` in `nuttx/`) creates the git-ignored `apps/external -> ../xiao-rp2350-bargraph/apps` symlink and emits `nuttx/nuttx`, `nuttx/nuttx.bin`, and `nuttx/nuttx.uf2` `[Makefile:build]`, `[board/scripts/Make.defs:apps/external]`.
- Hardware Peripheral Initialization at Boot: At power-on, `board_late_initialize()` registers `/dev/adc0` and `/dev/pwm0,2,3,5,6`, isolates the digital pad on `GPIO26` (`IE=0`, `PUE=0`, `PDE=0`, `ISO=0`, `OD=1`), and spawns `bargraphd` to sample `ADC0` at 50 Hz and drive `D1`–`D6` via hardware PWM `[board/src/rp23xx_bringup.c:rp23xx_bringup]`, `[apps/bargraph/bargraph_main.c:bargraph_autostart]`.

## Verification

- Run host unit tests: `make -C xiao-rp2350-bargraph test`
- Run full firmware build: `make -C xiao-rp2350-bargraph`
- Verify submodule cleanliness: `git -C nuttx status -s && git -C apps status -s`
- Flash `nuttx/nuttx.uf2` in RP2350 BOOTSEL mode and test NSH commands over `/dev/ttyACM0`: `bargraph status`, `bargraph set 50`, `bargraph test`, `bargraph resume`, `bargraph stop`, `bargraph start`
