---
title: "Out-of-Tree NuttX Board Bringup, RP2350-E9 Pad Isolation & NVIC IRQ Gating"
page_title: "Lesson 0005: Out-of-Tree NuttX Board Bringup, RP2350-E9 Pad Isolation & NVIC IRQ Gating — XIAO RP2350 Bar Graph"
nav_id: "lesson-0005"
badge: "Lesson 0005 • Out-of-Tree BSP & NVIC Gating"
subtitle: "Wire the out-of-tree NuttX board and apps/external symlink, isolate GPIO26 against Erratum RP2350-E9 digital leakage, and defeat the 500 kIRQ/s ADC interrupt storm."
card:
  title: "**Out-of-Tree NuttX Board Bringup, RP2350-E9 Pad Isolation & NVIC IRQ Gating**"
  summary: "Configure CONFIG_ARCH_BOARD_CUSTOM and apps/external without modifying nuttx/ or apps/, isolate GPIO26 (Erratum RP2350-E9), and gate RP23XX_ADC_IRQ_FIFO around 5-sample burst reads."
---

:::callout title="Recommended Primary Sources"
- **Companion project**: `apps/0005-out-of-tree-bsp-and-adc-irq-gating/` (`make -C apps/0005-out-of-tree-bsp-and-adc-irq-gating test`)
- **Primary hardware & RTOS docs**: [RP2350 Datasheet (Erratum RP2350-E9 & Section 9.4: PADS_BANK0)](https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf), `xiao-rp2350-bargraph/board/src/rp23xx_bringup.c`, and `xiao-rp2350-bargraph/board/scripts/Make.defs`
- **Course references**: [Glossary](../reference/GLOSSARY.md), [7 NuttX & RP2350 Driver Traps](../reference/nuttx-rp2350-driver-traps.md), [Lesson 0004](0004-multichannel-pwm-routing-and-safe-shutdown.md), and [Lesson 0006: The 50 Hz bargraphd Daemon & NSH CLI](0006-bargraphd-daemon-and-nsh-cli.md)
:::

## 1. Out-of-Tree Board (`CONFIG_ARCH_BOARD_CUSTOM`) & Application (`apps/external`) Topology {#out-of-tree-topology}

A core invariant of our workspace is **Zero Submodule Modifications**: `git -C nuttx status --porcelain` and `git -C apps status --porcelain` must remain completely empty. We achieve this through two built-in Apache NuttX extension mechanisms:

1. **Out-of-Tree Custom Board (`xiao-rp2350-bargraph/board/`)**:
   In `xiao-rp2350-bargraph/board/configs/bargraph/defconfig`, we set:
   - `CONFIG_ARCH_BOARD_CUSTOM=y`
   - `CONFIG_ARCH_BOARD_CUSTOM_NAME="xiao-rp2350-bargraph"`
   - `CONFIG_ARCH_BOARD_CUSTOM_DIR="../xiao-rp2350-bargraph/board"`
   - `CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y`
   - `CONFIG_ARCH_BOARD_COMMON=y`
   - `CONFIG_BOARD_LATE_INITIALIZE=y` (solving **Trap #1**, which compiled out `board_late_initialize()` in upstream `xiao-rp2350:usbnsh`)
2. **Out-of-Tree Built-In Application (`apps/external` Symlink Hook)**:
   Because `apps/.gitignore` explicitly ignores `/external` and `/Kconfig`, placing a `distclean`-guarded symlink hook in `xiao-rp2350-bargraph/board/scripts/Make.defs` creates `apps/external -> ../xiao-rp2350-bargraph/apps` before `apps/Make.defs` scans subdirectories:

```makefile caption="xiao-rp2350-bargraph/board/scripts/Make.defs symlink hook"
ifeq ($(filter distclean clean_context apps_distclean,$(MAKECMDGOALS)),)
$(shell if [ ! -e "$(TOPDIR)/../apps/external" ]; then \
  ln -s ../xiao-rp2350-bargraph/apps "$(TOPDIR)/../apps/external"; \
fi)
endif
```

```mermaid caption="Figure 5.1: Boot Sequence from nx_start() Through board_late_initialize() and rp23xx_bringup() to Autonomous bargraphd Startup"
sequenceDiagram
  participant Kernel as nx_start() (NuttX Kernel)
  participant Board as board_late_initialize() / rp23xx_bringup()
  participant Daemon as bargraphd (50 Hz Task)
  participant NSH as nsh_main (USB CDC/ACM)

  Kernel->>Board: OSINIT_HARDWARE -> board_late_initialize()
  Board->>Board: rp23xx_common_bringup() registers /dev/adc0 & /dev/pwm0,2,3,5,6
  Board->>Board: modreg32(OD, IE|PUE|PDE|ISO, PADS_BANK0_GPIO(26))
  Board->>Daemon: bargraph_autostart() -> task_create("bargraphd")
  Daemon->>Daemon: open("/dev/adc0") + up_disable_irq(RP23XX_ADC_IRQ_FIFO)
  Daemon->>Daemon: open("/dev/pwm0,2,3,5,6") + signal done_cond
  Daemon-->>Board: init_err = 0 (running in SENSING mode)
  Kernel->>NSH: Spawn nsh_main (blocks in nsh_waitusbready until USB-C host connects)
```

## 2. Conquering Traps #2, #6, and #7 in `rp23xx_bringup.c` and `bargraph_sample_adc()` {#traps-2-6-7}

Let's examine how `rp23xx_bringup.c` and `bargraph_sample_adc()` solve the remaining hardware and driver traps:

- **Trap #2 — Autonomous Power-On Startup Before USB Console Wait**:
  When NSH runs over USB CDC/ACM (`CONFIG_NSH_USBCONSOLE=y`), `nsh_consolemain()` blocks inside `nsh_waitusbready()` until a host PC opens `/dev/ttyACM0` and sends three newline characters. By calling `bargraph_autostart()` directly at the end of `rp23xx_bringup()` (which runs during `board_late_initialize()` before `nsh_main` is spawned), `bargraphd` starts tracking the potentiometer within milliseconds of power-on—even on a standalone 5V USB wall adapter!
- **Trap #6 — ADC Pin-Mux Conflict & Erratum `RP2350-E9` Digital Pad Leakage**:
  In `rp23xx_adc.c`, `my_reset()` resets the GPIO function of every enabled ADC channel (`GPIO26`–`GPIO29`) to `RP23XX_GPIO_FUNC_NULL`. Enabling only `CONFIG_RP23XX_ADC_CHANNEL0=y` in `defconfig` prevents `/dev/adc0` from wiping out the PWM pin muxing on `D1` (`GPIO27`) and `D2` (`GPIO28`). Furthermore, to prevent **Erratum RP2350-E9** digital input buffer leakage on `GPIO26` (`D0/A0`), `rp23xx_bringup()` clears `IE` (Input Enable), `PUE` (Pull-Up), `PDE` (Pull-Down), and `ISO` (Pad Isolation) and sets `OD` (Output Disable) in `RP23XX_PADS_BANK0_GPIO(26)`.{{sidenote:On RP2350 A2 silicon, leaving `IE=1` on an analog GPIO allows the Schmitt trigger input buffer to source/sink microamperes around mid-scale voltages (~1.65 V), distorting potentiometer linearity.}}
- **Trap #7 — The 500,000 IRQ/s Self-Retriggering `START_ONCE` Interrupt Storm**:
  In `rp23xx_adc.c`, `interrupt_handler()` calls `get_next_channel()` at the end of every FIFO IRQ and writes `RP23XX_ADC_CS_START_ONCE`. Because the RP2350 `ADC_DIV` register only paces free-running `START_MANY` conversions and has **zero effect on single-shot `START_ONCE` writes**, every `START_ONCE` write completes in `96` ADC clock cycles (`2 µs` at `48 MHz`), firing **500,000 interrupts per second** (`500 kIRQ/s`) continuously!
  - **Why we keep `/dev/adc0` open and gate `RP23XX_ADC_IRQ_FIFO` in the NVIC**: Closing `/dev/adc0` after every read is unsafe because `remove_device()` in `rp23xx_adc.c` mutates the driver's linked list without re-initializing node pointers. Instead, `bargraphd` opens `/dev/adc0` once at startup and immediately calls `up_disable_irq(RP23XX_ADC_IRQ_FIFO)`. On each 20 ms tick, `bargraph_sample_adc()` flushes stale software FIFO entries via `ioctl(adc_fd, ANIOC_RESET_FIFO, 0)`, unmasks `RP23XX_ADC_IRQ_FIFO` (`up_enable_irq`) just long enough to read 5 samples (`~10 µs`), and masks `RP23XX_ADC_IRQ_FIFO` (`up_disable_irq`) before returning!

## 3. Code Walkthrough: Pad Isolation & Demand-Gated ADC Sampling {#code-walkthrough}

Let's inspect the two new blocks added to `apps/0005-out-of-tree-bsp-and-adc-irq-gating/src/main.c`.

### Block 1: Erratum RP2350-E9 Pad Isolation (`modreg32` on `PADS_BANK0_GPIO(26)`)

```c caption="Modifying PADS_BANK0_GPIO(26) to isolate analog input D0/A0"
#define RP23XX_PADS_BANK0_GPIO_ISO (1u << 8)
#define RP23XX_PADS_BANK0_GPIO_OD  (1u << 7)
#define RP23XX_PADS_BANK0_GPIO_IE  (1u << 6)
#define RP23XX_PADS_BANK0_GPIO_PUE (1u << 3)
#define RP23XX_PADS_BANK0_GPIO_PDE (1u << 2)

static void modreg32(uint32_t setbits, uint32_t clearbits, uint32_t *reg)
{
  *reg = (*reg & ~clearbits) | setbits;
}

void bargraph_isolate_adc_pad(uint32_t *pad_reg)
{
  modreg32(RP23XX_PADS_BANK0_GPIO_OD,
           RP23XX_PADS_BANK0_GPIO_IE |
           RP23XX_PADS_BANK0_GPIO_PUE |
           RP23XX_PADS_BANK0_GPIO_PDE |
           RP23XX_PADS_BANK0_GPIO_ISO,
           pad_reg);
}
```

- **`modreg32(setbits, clearbits, addr)`**: The standard NuttX ARM register helper that clears `clearbits` (`IE | PUE | PDE | ISO`) and sets `setbits` (`OD`) in a single read-modify-write sequence.

### Block 2: Demand-Driven NVIC IRQ Gating (`bargraph_sample_adc`)

```c caption="Flushing the FIFO and gating RP23XX_ADC_IRQ_FIFO around a 5-sample burst read"
static int bargraph_sample_adc(int adc_fd,
                               uint32_t *ema_q4,
                               bool *ema_initialized,
                               uint16_t *raw_out,
                               uint16_t *filtered_out)
{
  struct adc_msg_s msgs[BARGRAPH_ADC_READ_SAMPLES];
  uint8_t *buf = (uint8_t *)msgs;
  size_t bytes_needed = sizeof(msgs);
  size_t total_read = 0u;
  uint32_t sum = 0u;
  unsigned int valid_count = 0u;
  unsigned int i;

  (void)adc_ioctl(adc_fd, ANIOC_RESET_FIFO, 0);

  up_enable_irq(RP23XX_ADC_IRQ_FIFO);

  while (total_read < bytes_needed)
    {
      ssize_t nread = adc_read(adc_fd, buf + total_read,
                               bytes_needed - total_read);
      if (nread < 0)
        {
          if (errno == EINTR)
            {
              continue;
            }

          up_disable_irq(RP23XX_ADC_IRQ_FIFO);
          return -errno;
        }

      if (nread == 0)
        {
          break;
        }

      total_read += (size_t)nread;
    }

  up_disable_irq(RP23XX_ADC_IRQ_FIFO);
```

- **Guaranteed IRQ Remasking**: Whether the `while (total_read < bytes_needed)` loop finishes normally or exits early on a non-`EINTR` read error, `up_disable_irq(RP23XX_ADC_IRQ_FIFO)` is always called before returning so the 500 kIRQ/s storm can never stay unmasked while the daemon sleeps.

### Complete src/main.c Listing

```c caption="apps/0005-out-of-tree-bsp-and-adc-irq-gating/src/main.c"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define BARGRAPH_NUM_LEDS          6
#define BARGRAPH_NUM_PWM_SLICES    5
#define BARGRAPH_PWM_FREQ_HZ       1000
#define BARGRAPH_ADC_MAX           4095u
#define BARGRAPH_VDD_MV            3300u
#define BARGRAPH_LED_VF_MV         2000u
#define BARGRAPH_LED_RES_OHM       330u
#define BARGRAPH_PAD_LIMIT_UA      12000u
#define BARGRAPH_IOVDD_LIMIT_UA    200000u
#define BARGRAPH_ADC_GPIO          26u

#define BARGRAPH_DUTY_MAX          65535u
#define BARGRAPH_TOTAL_UNITS_MAX   (BARGRAPH_NUM_LEDS * BARGRAPH_DUTY_MAX)

#define BARGRAPH_ADC_DEADBAND_LOW  32u
#define BARGRAPH_ADC_DEADBAND_HIGH 4063u
#define BARGRAPH_ADC_BURST_SAMPLES 4
#define BARGRAPH_ADC_READ_SAMPLES  (BARGRAPH_ADC_BURST_SAMPLES + 1)
#define BARGRAPH_ADC_SHIFT         19

#define PWM_IDX_SLICE0             0
#define PWM_IDX_SLICE2             1
#define PWM_IDX_SLICE3             2
#define PWM_IDX_SLICE5             3
#define PWM_IDX_SLICE6             4

#define PWMIOC_SETCHARACTERISTICS  1
#define PWMIOC_START               2
#define PWMIOC_STOP                3
#define ANIOC_RESET_FIFO           4

#define RP23XX_ADC_IRQ_FIFO        22

#define RP23XX_PADS_BANK0_GPIO_ISO (1u << 8)
#define RP23XX_PADS_BANK0_GPIO_OD  (1u << 7)
#define RP23XX_PADS_BANK0_GPIO_IE  (1u << 6)
#define RP23XX_PADS_BANK0_GPIO_PUE (1u << 3)
#define RP23XX_PADS_BANK0_GPIO_PDE (1u << 2)

typedef uint32_t ub16_t;

struct pwm_chan_s
{
  ub16_t duty;
  int8_t channel;
};

struct pwm_info_s
{
  uint32_t          frequency;
  struct pwm_chan_s channels[2];
};

struct bargraph_led_map_s
{
  const char *xiao_pin;
  uint8_t     gpio;
  const char *pwm_devpath;
  uint8_t     slice_idx;
  uint8_t     channel;
  uint8_t     breadboard_row;
};

struct adc_msg_s
{
  uint8_t am_channel;
  int32_t am_data;
};

struct mock_pwm_slice_s
{
  bool     open;
  bool     running;
  bool     frozen_high;
  uint32_t frequency;
  uint16_t duty_a;
  uint16_t duty_b;
  uint32_t start_count;
  uint32_t stop_count;
};

struct mock_adc_dev_s
{
  bool             irq_enabled;
  uint32_t         fifo_resets;
  uint32_t         irq_enable_count;
  uint32_t         irq_disable_count;
  bool             inject_eintr_once;
  size_t           cursor;
  struct adc_msg_s samples[BARGRAPH_ADC_READ_SAMPLES];
};

static struct mock_pwm_slice_s g_mock_slices[BARGRAPH_NUM_PWM_SLICES];
static struct mock_adc_dev_s   g_mock_adc;

static const struct bargraph_led_map_s g_led_map[BARGRAPH_NUM_LEDS] =
{
  { "D1", 27, "/dev/pwm5", PWM_IDX_SLICE5, 2, 14 },
  { "D2", 28, "/dev/pwm6", PWM_IDX_SLICE6, 1, 15 },
  { "D3",  5, "/dev/pwm2", PWM_IDX_SLICE2, 2, 16 },
  { "D4",  6, "/dev/pwm3", PWM_IDX_SLICE3, 1, 17 },
  { "D5",  7, "/dev/pwm3", PWM_IDX_SLICE3, 2, 18 },
  { "D6",  0, "/dev/pwm0", PWM_IDX_SLICE0, 1, 19 }
};

static void modreg32(uint32_t setbits, uint32_t clearbits, uint32_t *reg)
{
  *reg = (*reg & ~clearbits) | setbits;
}

void bargraph_isolate_adc_pad(uint32_t *pad_reg)
{
  modreg32(RP23XX_PADS_BANK0_GPIO_OD,
           RP23XX_PADS_BANK0_GPIO_IE |
           RP23XX_PADS_BANK0_GPIO_PUE |
           RP23XX_PADS_BANK0_GPIO_PDE |
           RP23XX_PADS_BANK0_GPIO_ISO,
           pad_reg);
}

static void up_enable_irq(int irq)
{
  if (irq == RP23XX_ADC_IRQ_FIFO)
    {
      g_mock_adc.irq_enabled = true;
      g_mock_adc.irq_enable_count++;
    }
}

static void up_disable_irq(int irq)
{
  if (irq == RP23XX_ADC_IRQ_FIFO)
    {
      g_mock_adc.irq_enabled = false;
      g_mock_adc.irq_disable_count++;
    }
}

static int adc_ioctl(int fd, int cmd, unsigned long arg)
{
  (void)arg;
  if (fd != 0)
    {
      errno = EBADF;
      return -1;
    }

  if (cmd == ANIOC_RESET_FIFO)
    {
      g_mock_adc.fifo_resets++;
      g_mock_adc.cursor = 0u;
      return 0;
    }

  errno = ENOTTY;
  return -1;
}

static ssize_t adc_read(int fd, void *buf, size_t count)
{
  const uint8_t *src = (const uint8_t *)g_mock_adc.samples;
  size_t total_bytes = sizeof(g_mock_adc.samples);
  size_t avail;
  size_t chunk;

  if (fd != 0 || !g_mock_adc.irq_enabled)
    {
      errno = EIO;
      return -1;
    }

  if (g_mock_adc.inject_eintr_once)
    {
      g_mock_adc.inject_eintr_once = false;
      errno = EINTR;
      return -1;
    }

  if (g_mock_adc.cursor >= total_bytes)
    {
      return 0;
    }

  avail = total_bytes - g_mock_adc.cursor;
  chunk = count < avail ? count : avail;
  if (chunk > sizeof(struct adc_msg_s) * 2u)
    {
      chunk = sizeof(struct adc_msg_s) * 2u;
    }

  memcpy(buf, src + g_mock_adc.cursor, chunk);
  g_mock_adc.cursor += chunk;
  return (ssize_t)chunk;
}

static int pwm_ioctl(int fd, int cmd, unsigned long arg)
{
  struct mock_pwm_slice_s *s;

  if (fd < 0 || fd >= BARGRAPH_NUM_PWM_SLICES || !g_mock_slices[fd].open)
    {
      errno = EBADF;
      return -1;
    }

  s = &g_mock_slices[fd];
  if (cmd == PWMIOC_SETCHARACTERISTICS)
    {
      const struct pwm_info_s *info = (const struct pwm_info_s *)arg;
      if (info->channels[0].channel <= 0 || info->channels[1].channel <= 0)
        {
          errno = EINVAL;
          return -1;
        }

      s->frequency = info->frequency;
      s->duty_a    = (uint16_t)info->channels[0].duty;
      s->duty_b    = (uint16_t)info->channels[1].duty;
      return 0;
    }
  else if (cmd == PWMIOC_START)
    {
      s->running = true;
      s->start_count++;
      return 0;
    }
  else if (cmd == PWMIOC_STOP)
    {
      s->running     = false;
      s->frozen_high = (s->duty_a != 0u || s->duty_b != 0u);
      s->stop_count++;
      return 0;
    }

  errno = ENOTTY;
  return -1;
}

static int pwm_close(int fd)
{
  if (fd < 0 || fd >= BARGRAPH_NUM_PWM_SLICES || !g_mock_slices[fd].open)
    {
      errno = EBADF;
      return -1;
    }

  g_mock_slices[fd].open = false;
  return 0;
}

uint32_t bargraph_led_current_ua(uint32_t vdd_mv,
                                 uint32_t vf_mv,
                                 uint32_t res_ohm)
{
  if (vdd_mv <= vf_mv || res_ohm == 0u)
    {
      return 0u;
    }

  return ((vdd_mv - vf_mv) * 1000u) / res_ohm;
}

uint32_t bargraph_total_led_current_ua(unsigned int num_leds)
{
  uint32_t per_led_ua = bargraph_led_current_ua(BARGRAPH_VDD_MV,
                                                BARGRAPH_LED_VF_MV,
                                                BARGRAPH_LED_RES_OHM);
  return per_led_ua * (uint32_t)num_leds;
}

uint16_t bargraph_adc_to_mv(uint16_t adc_count)
{
  if (adc_count > BARGRAPH_ADC_MAX)
    {
      adc_count = BARGRAPH_ADC_MAX;
    }

  return (uint16_t)(((uint32_t)adc_count * BARGRAPH_VDD_MV + 2047u) /
                    BARGRAPH_ADC_MAX);
}

uint16_t bargraph_adc_to_permille(uint16_t adc_count)
{
  if (adc_count > BARGRAPH_ADC_MAX)
    {
      adc_count = BARGRAPH_ADC_MAX;
    }

  return (uint16_t)(((uint32_t)adc_count * 1000u + 2047u) /
                    BARGRAPH_ADC_MAX);
}

uint32_t bargraph_units_from_adc(uint16_t adc_count)
{
  if (adc_count > BARGRAPH_ADC_MAX)
    {
      adc_count = BARGRAPH_ADC_MAX;
    }

  return ((uint32_t)adc_count * BARGRAPH_TOTAL_UNITS_MAX + 2047u) /
         BARGRAPH_ADC_MAX;
}

void bargraph_compute_duties(uint32_t total_units,
                             uint16_t duties[BARGRAPH_NUM_LEDS])
{
  unsigned int i;

  if (total_units > BARGRAPH_TOTAL_UNITS_MAX)
    {
      total_units = BARGRAPH_TOTAL_UNITS_MAX;
    }

  for (i = 0; i < BARGRAPH_NUM_LEDS; i++)
    {
      uint32_t seg_start = (uint32_t)i * BARGRAPH_DUTY_MAX;
      uint32_t seg_end   = seg_start + BARGRAPH_DUTY_MAX;

      if (total_units >= seg_end)
        {
          duties[i] = (uint16_t)BARGRAPH_DUTY_MAX;
        }
      else if (total_units > seg_start)
        {
          duties[i] = (uint16_t)(total_units - seg_start);
        }
      else
        {
          duties[i] = 0u;
        }
    }
}

uint16_t bargraph_extract_adc12(int32_t am_data)
{
  return (uint16_t)((am_data >> BARGRAPH_ADC_SHIFT) & 0x0fff);
}

uint16_t bargraph_filter_adc(uint16_t raw_avg,
                             uint32_t *ema_q4,
                             bool *ema_initialized)
{
  uint16_t filtered;

  if (raw_avg > BARGRAPH_ADC_MAX)
    {
      raw_avg = BARGRAPH_ADC_MAX;
    }

  if (!(*ema_initialized))
    {
      *ema_q4          = (uint32_t)raw_avg << 4;
      *ema_initialized = true;
    }
  else
    {
      *ema_q4 = *ema_q4 - (*ema_q4 >> 2) + ((uint32_t)raw_avg << 2);
    }

  filtered = (uint16_t)((*ema_q4 + 8u) >> 4);

  if (filtered <= BARGRAPH_ADC_DEADBAND_LOW)
    {
      filtered = 0u;
    }
  else if (filtered >= BARGRAPH_ADC_DEADBAND_HIGH)
    {
      filtered = BARGRAPH_ADC_MAX;
    }

  return filtered;
}

static int bargraph_sample_adc(int adc_fd,
                               uint32_t *ema_q4,
                               bool *ema_initialized,
                               uint16_t *raw_out,
                               uint16_t *filtered_out)
{
  struct adc_msg_s msgs[BARGRAPH_ADC_READ_SAMPLES];
  uint8_t *buf = (uint8_t *)msgs;
  size_t bytes_needed = sizeof(msgs);
  size_t total_read = 0u;
  uint32_t sum = 0u;
  unsigned int valid_count = 0u;
  unsigned int i;

  (void)adc_ioctl(adc_fd, ANIOC_RESET_FIFO, 0);

  up_enable_irq(RP23XX_ADC_IRQ_FIFO);

  while (total_read < bytes_needed)
    {
      ssize_t nread = adc_read(adc_fd, buf + total_read,
                               bytes_needed - total_read);
      if (nread < 0)
        {
          if (errno == EINTR)
            {
              continue;
            }

          up_disable_irq(RP23XX_ADC_IRQ_FIFO);
          return -errno;
        }

      if (nread == 0)
        {
          break;
        }

      total_read += (size_t)nread;
    }

  up_disable_irq(RP23XX_ADC_IRQ_FIFO);

  if (total_read < sizeof(msgs))
    {
      return -EIO;
    }

  for (i = 1u; i < BARGRAPH_ADC_READ_SAMPLES; i++)
    {
      if (msgs[i].am_data >= 0)
        {
          uint16_t sample12 =
            (uint16_t)((msgs[i].am_data >> BARGRAPH_ADC_SHIFT) & 0x0fff);
          sum += sample12;
          valid_count++;
        }
    }

  if (valid_count > 0u)
    {
      uint16_t raw_avg = (uint16_t)((sum + (valid_count / 2u)) / valid_count);
      *raw_out      = raw_avg;
      *filtered_out = bargraph_filter_adc(raw_avg, ema_q4, ema_initialized);
    }

  return 0;
}

static int bargraph_update_slice(int fd, bool *started,
                                 uint16_t duty_a, uint16_t duty_b)
{
  struct pwm_info_s info;
  int ret;

  memset(&info, 0, sizeof(info));
  info.frequency           = BARGRAPH_PWM_FREQ_HZ;
  info.channels[0].channel = 1;
  info.channels[0].duty    = (ub16_t)duty_a;
  info.channels[1].channel = 2;
  info.channels[1].duty    = (ub16_t)duty_b;

  ret = pwm_ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)&info);
  if (ret < 0)
    {
      return -errno;
    }

  if (!(*started))
    {
      ret = pwm_ioctl(fd, PWMIOC_START, 0);
      if (ret < 0)
        {
          return -errno;
        }

      *started = true;
    }

  return 0;
}

static int bargraph_apply_pwm(const int pwm_fds[BARGRAPH_NUM_PWM_SLICES],
                              bool pwm_started[BARGRAPH_NUM_PWM_SLICES],
                              const uint16_t duties[BARGRAPH_NUM_LEDS])
{
  int ret;

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE5],
                              &pwm_started[PWM_IDX_SLICE5],
                              0u, duties[0]);
  if (ret < 0)
    {
      return ret;
    }

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE6],
                              &pwm_started[PWM_IDX_SLICE6],
                              duties[1], 0u);
  if (ret < 0)
    {
      return ret;
    }

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE2],
                              &pwm_started[PWM_IDX_SLICE2],
                              0u, duties[2]);
  if (ret < 0)
    {
      return ret;
    }

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE3],
                              &pwm_started[PWM_IDX_SLICE3],
                              duties[3], duties[4]);
  if (ret < 0)
    {
      return ret;
    }

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE0],
                              &pwm_started[PWM_IDX_SLICE0],
                              duties[5], 0u);
  return ret;
}

static void bargraph_shutdown_pwm(int pwm_fds[BARGRAPH_NUM_PWM_SLICES],
                                  bool pwm_started[BARGRAPH_NUM_PWM_SLICES])
{
  int i;

  for (i = 0; i < BARGRAPH_NUM_PWM_SLICES; i++)
    {
      if (pwm_fds[i] >= 0)
        {
          (void)bargraph_update_slice(pwm_fds[i], &pwm_started[i], 0u, 0u);
          (void)pwm_ioctl(pwm_fds[i], PWMIOC_STOP, 0);
          (void)pwm_close(pwm_fds[i]);
          pwm_fds[i]     = -1;
          pwm_started[i] = false;
        }
    }
}

int main(void)
{
  uint32_t pad_reg = RP23XX_PADS_BANK0_GPIO_ISO |
                     RP23XX_PADS_BANK0_GPIO_IE |
                     RP23XX_PADS_BANK0_GPIO_PDE |
                     0x00000010u;
  uint32_t ema_q4 = 0u;
  bool ema_init = false;
  uint16_t raw_adc = 0u;
  uint16_t filtered_adc = 0u;
  int pwm_fds[BARGRAPH_NUM_PWM_SLICES] = { -1, -1, -1, -1, -1 };
  bool pwm_started[BARGRAPH_NUM_PWM_SLICES] = { false, false, false, false, false };
  uint16_t duties[BARGRAPH_NUM_LEDS];

  assert(g_led_map[0].gpio == 27u);
  assert(bargraph_total_led_current_ua(BARGRAPH_NUM_LEDS) == 23634u);

  bargraph_isolate_adc_pad(&pad_reg);
  assert((pad_reg & RP23XX_PADS_BANK0_GPIO_OD) != 0u);
  assert((pad_reg & RP23XX_PADS_BANK0_GPIO_IE) == 0u);
  assert((pad_reg & RP23XX_PADS_BANK0_GPIO_PUE) == 0u);
  assert((pad_reg & RP23XX_PADS_BANK0_GPIO_PDE) == 0u);
  assert((pad_reg & RP23XX_PADS_BANK0_GPIO_ISO) == 0u);
  assert((pad_reg & 0x00000010u) != 0u);

  memset(&g_mock_adc, 0, sizeof(g_mock_adc));
  g_mock_adc.inject_eintr_once = true;
  g_mock_adc.samples[0].am_data = (int32_t)(50 << BARGRAPH_ADC_SHIFT);
  g_mock_adc.samples[1].am_data = (int32_t)(3000 << BARGRAPH_ADC_SHIFT);
  g_mock_adc.samples[2].am_data = (int32_t)(3004 << BARGRAPH_ADC_SHIFT);
  g_mock_adc.samples[3].am_data = (int32_t)(3008 << BARGRAPH_ADC_SHIFT);
  g_mock_adc.samples[4].am_data = (int32_t)(3012 << BARGRAPH_ADC_SHIFT);

  assert(bargraph_sample_adc(0, &ema_q4, &ema_init,
                             &raw_adc, &filtered_adc) == 0);
  assert(!g_mock_adc.irq_enabled);
  assert(g_mock_adc.fifo_resets == 1u);
  assert(g_mock_adc.irq_enable_count == 1u);
  assert(g_mock_adc.irq_disable_count == 1u);
  assert(raw_adc == 3006u);
  assert(filtered_adc == 3006u);

  bargraph_compute_duties(bargraph_units_from_adc(filtered_adc), duties);
  (void)bargraph_apply_pwm(pwm_fds, pwm_started, duties);
  bargraph_shutdown_pwm(pwm_fds, pwm_started);

  printf("Lesson 0005 verification passed: pad=0x%02x, raw_adc=%u, irq_en=%u\n",
         (unsigned int)pad_reg,
         (unsigned int)raw_adc,
         (unsigned int)g_mock_adc.irq_enabled);
  return 0;
}
```

## 4. Interactive Practice & Verification {#practice-and-verification}

:::exercise id="ex-0005" title="Interactive Lab: RP2350-E9 Pad Isolation & NVIC ADC IRQ Gating" badge="Skill Practice • rp23xx_bringup & bargraph_sample_adc"
Complete the `modreg32()` call that isolates `GPIO26` (`D0/A0`) against Erratum RP2350-E9 and the FIFO reset + NVIC IRQ gating sequence in `bargraph_sample_adc()`:

```c
modreg32({{blank:RP23XX_PADS_BANK0_GPIO_OD|Bit set to disable digital output driver|?????????????????????????|25}},
         RP23XX_PADS_BANK0_GPIO_IE |
         RP23XX_PADS_BANK0_GPIO_PUE |
         RP23XX_PADS_BANK0_GPIO_PDE |
         RP23XX_PADS_BANK0_GPIO_ISO,
         pad_reg);

(void)adc_ioctl(adc_fd, {{blank:ANIOC_RESET_FIFO|Ioctl command to flush software ADC FIFO|????????????????|16}}, 0);
{{blank:up_enable_irq|NVIC function to unmask ADC FIFO interrupt|?????????????|13}}(RP23XX_ADC_IRQ_FIFO);
```

:::prompts
hint: "Set RP23XX_PADS_BANK0_GPIO_OD in modreg32(), flush the software FIFO with ANIOC_RESET_FIFO, and unmask the IRQ with up_enable_irq(RP23XX_ADC_IRQ_FIFO)."
success: "Verified! Your RP2350-E9 pad isolation and demand-driven NVIC IRQ gating match the firmware."
:::
:::

:::quiz id="quiz-0005" title="Retrieval Practice: Out-of-Tree Bringup, RP2350-E9 & NVIC Gating" badge="3 Questions • Equal-Length Options"
- question: "How does rp23xx_bringup mitigate Erratum RP2350-E9 on analog pin GPIO26?"
  wrong_hint: "Check which bit is set vs cleared in RP23XX_PADS_BANK0_GPIO(26)."
  options:
    - letter: "A"
      text: "Clear IE PUE PDE ISO and set OD on GPIO26"
      correct: true
      explain: "Disabling the input buffer (IE=0), pulls (PUE=0, PDE=0), and isolation (ISO=0) while setting OD=1 isolates the analog pad."
    - letter: "B"
      text: "Set IE PUE PDE ISO and clear OD on GPIO26"
      correct: false
      explain: "Setting IE=1 and pulls enables digital leakage and voltage division on the potentiometer wiper."
    - letter: "C"
      text: "Enable internal pull-up resistor PUE on GPIO26 and disable OD"
      correct: false
      explain: "Enabling PUE pulls the wiper toward 3.3 V and distorts the linear voltage divider."
    - letter: "D"
      text: "Configure GPIO26 as push-pull digital output driven high at boot"
      correct: false
      explain: "Driving GPIO26 as a digital output fights the potentiometer wiper and risks shorting 3.3 V to GND."
- question: "How does bargraphd prevent the 500,000 IRQ/s START_ONCE interrupt storm?"
  wrong_hint: "Remember that RP23XX_ADC_DIV only paces START_MANY, not START_ONCE."
  options:
    - letter: "A"
      text: "Mask RP23XX_ADC_IRQ_FIFO while idle and unmask only during burst reads"
      correct: true
      explain: "Gating RP23XX_ADC_IRQ_FIFO in the NVIC stops the ISR from firing during the 20 ms sleep window."
    - letter: "B"
      text: "Close /dev/adc0 after every single sample and reopen next tick"
      correct: false
      explain: "Repeated close() calls corrupt the driver's device list in remove_device() inside rp23xx_adc.c."
    - letter: "C"
      text: "Write 0xFFFF00 into RP23XX_ADC_DIV to slow down START_ONCE single conversions"
      correct: false
      explain: "The hardware ADC_DIV register only paces free-running START_MANY mode and ignores START_ONCE."
    - letter: "D"
      text: "Increase CONFIG_ADC_FIFOSIZE to 65535 entries inside the board defconfig file"
      correct: false
      explain: "Even when the software FIFO fills up, interrupt_handler() still re-triggers START_ONCE at 500 kIRQ/s."
- question: "How does the firmware start bargraphd before USB console wait?"
  wrong_hint: "Check where nsh_waitusbready() blocks relative to board_late_initialize()."
  options:
    - letter: "A"
      text: "Spawn bargraphd from /etc/init.d/rcS after the USB console handshake completes"
      correct: false
      explain: "rcS does not run until after nsh_waitusbready() receives a USB host connection and 3 newlines."
    - letter: "B"
      text: "Replace CONFIG_INIT_ENTRYPOINT with bargraph_main instead of running the NuttShell console"
      correct: false
      explain: "Replacing nsh_main would lose the interactive USB-C NSH console."
    - letter: "C"
      text: "Call bargraph_autostart inside rp23xx_bringup during kernel board_late_initialize before nsh_main starts"
      correct: true
      explain: "rp23xx_bringup() runs during board_late_initialize() before nsh_main blocks waiting for USB-C."
    - letter: "D"
      text: "Wait for the user to type bargraph start over /dev/ttyACM0"
      correct: false
      explain: "Waiting for manual user input leaves the bar graph dark on standalone USB power."
:::

:::verify id="verify-0005" title="Verification Lab: Compile & Run Lesson 0005 Companion App" badge="Terminal Gate"
Run the Lesson 0005 verification gate from `tutorial/`:

```bash caption="Compile and run Lesson 0005 verification gate"
make -C apps/0005-out-of-tree-bsp-and-adc-irq-gating test
```

Expected terminal output:

```text caption="Verified output from apps/0005-out-of-tree-bsp-and-adc-irq-gating"
cc -std=c99 -Wall -Wextra -Werror -pedantic -O2 src/main.c -o build/bargraph_lesson
./build/bargraph_lesson
Lesson 0005 verification passed: pad=0x90, raw_adc=3006, irq_en=0
```

- [ ] Confirm `pad=0x90` (`OD=1` bit 7 + preserved drive strength `0x10`, with `IE`, `PUE`, `PDE`, and `ISO` cleared).
- [ ] Confirm `bargraph_sample_adc()` recovers from `EINTR`, averages `3000, 3004, 3008, 3012` to `3006`, and leaves `irq_en=0`.
:::

:::teacher-prompt title="Extend This Lesson with Your Agent"
Ask your AI teacher: *"Why does `xiao-rp2350-bargraph/apps/Makefile` omit `MENUDESC` before including `$(APPDIR)/Directory.mk`, and what would `make distclean` delete if `MENUDESC` were defined?"*
:::
