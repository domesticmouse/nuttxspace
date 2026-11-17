---
title: "Multi-Channel RP2350 PWM Routing & Safe Hardware Shutdown"
page_title: "Lesson 0004: Multi-Channel RP2350 PWM Routing & Safe Hardware Shutdown — XIAO RP2350 Bar Graph"
nav_id: "lesson-0004"
badge: "Lesson 0004 • 5 PWM Slices & Zero-Before-Stop"
subtitle: "Route six LEDs across five multi-channel RP2350 PWM slices, satisfy both upper-half and lower-half channel indexing contracts, and prevent frozen GPIO pads on PWMIOC_STOP."
card:
  title: "**Multi-Channel RP2350 PWM Routing & Safe Hardware Shutdown**"
  summary: "Map LED1–LED6 across /dev/pwm0, /dev/pwm2, /dev/pwm3, /dev/pwm5, and /dev/pwm6, populate dual-channel pwm_info_s structures, and enforce zero-before-stop shutdown ordering."
---

:::callout title="Recommended Primary Sources"
- **Companion project**: `apps/0004-multichannel-pwm-routing-and-safe-shutdown/` (`make -C apps/0004-multichannel-pwm-routing-and-safe-shutdown test`)
- **Primary driver docs**: `nuttx/drivers/timers/pwm.c` and `nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c`
- **Course references**: [Glossary](../reference/GLOSSARY.md), [Hardware Pinout & Breadboard Sheet](../reference/hardware-pinout-and-wiring.md), [7 NuttX & RP2350 Driver Traps](../reference/nuttx-rp2350-driver-traps.md), [Lesson 0003](0003-q4-ema-filtering-and-adc-extraction.md), and [Lesson 0005: Out-of-Tree Board Bringup & NVIC IRQ Gating](0005-out-of-tree-bsp-and-adc-irq-gating.md)
:::

## 1. Two Multi-Channel PWM Driver Traps in `pwm.c` and `rp23xx_pwm.c` {#pwm-driver-traps}

The Seeed Studio XIAO RP2350 routes its six left-side digital pins (`D1`–`D6`) across **five** hardware PWM slices, using a mix of hardware Channel A (`GPIO = 2n`) and Channel B (`GPIO = 2n + 1`):

Table: Mapping Six Bar Graph LEDs Across Five RP2350 Hardware PWM Slices

| Segment | XIAO Pin | RP2350 GPIO | Device Node | Slice Array Index | Hardware Slice & Channel | `pwm_info_s` Duty Slot |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `LED1` (`0.0–16.7%`) | `D1` | `GPIO27` | `/dev/pwm5` | `PWM_IDX_SLICE5` (`3`) | Slice 5, Channel B (`PWM5B`) | `channels[1].duty` (`duty_b`) |
| `LED2` (`16.7–33.3%`) | `D2` | `GPIO28` | `/dev/pwm6` | `PWM_IDX_SLICE6` (`4`) | Slice 6, Channel A (`PWM6A`) | `channels[0].duty` (`duty_a`) |
| `LED3` (`33.3–50.0%`) | `D3` | `GPIO5` | `/dev/pwm2` | `PWM_IDX_SLICE2` (`1`) | Slice 2, Channel B (`PWM2B`) | `channels[1].duty` (`duty_b`) |
| `LED4` (`50.0–66.7%`) | `D4` | `GPIO6` | `/dev/pwm3` | `PWM_IDX_SLICE3` (`2`) | Slice 3, Channel A (`PWM3A`) | `channels[0].duty` (`duty_a`) |
| `LED5` (`66.7–83.3%`) | `D5` | `GPIO7` | `/dev/pwm3` | `PWM_IDX_SLICE3` (`2`) | Slice 3, Channel B (`PWM3B`) | `channels[1].duty` (`duty_b`) |
| `LED6` (`83.3–100.0%`) | `D6` | `GPIO0` | `/dev/pwm0` | `PWM_IDX_SLICE0` (`0`) | Slice 0, Channel A (`PWM0A`) | `channels[0].duty` (`duty_a`) |

When configuring these slices with `ioctl(fd, PWMIOC_SETCHARACTERISTICS, &info)`, two subtle interactions between the generic NuttX upper-half driver (`nuttx/drivers/timers/pwm.c`) and the RP2350 lower-half driver (`nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c`) come into play:

1. **Trap #3 — Hardcoded `channels[0]` / `channels[1]` Indexing vs Upper-Half `channel == 0` Termination**:
   In `rp23xx_pwm.c`, `setup_pulse()` ignores `info->channels[i].channel` and unconditionally writes `info->channels[0].duty` into hardware **Channel A** (`CC[15:0]`) and `info->channels[1].duty` into hardware **Channel B** (`CC[31:16]`). Meanwhile, the upper-half driver (`drivers/timers/pwm.c`) validates `info->channels[]` before calling the lower-half driver and **stops scanning the array as soon as it sees `channel == 0`**!{{sidenote:If you left `info.channels[0].channel = 0` on `/dev/pwm5` because `LED1` only uses Channel B (`PWM5B`), `pwm.c` would treat slot 0 as an unused terminator and never validate slot 1!}}
   - **Solution**: On every slice update (`bargraph_update_slice()`), always set `info.channels[0].channel = 1` with `duty_a` and `info.channels[1].channel = 2` with `duty_b` (passing `0` for unused sibling channels, whose GPIO pins are set to `-1` in `defconfig`).
2. **Trap #5 — `PWMIOC_STOP` Freezes the Output GPIO Pad at Its Instantaneous Counter Level**:
   In `rp23xx_pwm.c`, `pwm_stop()` (`clear_enabled()`) simply clears the slice enable bit (`1 << priv->num`) in `RP23XX_PWM_EN`. If the PWM counter happened to be in the high phase of its 1 kHz pulse when `PWMIOC_STOP` executed, the LED pad stays frozen HIGH until `close()` calls `pwm_shutdown()`.
   - **Solution**: In `bargraph_shutdown_pwm()`, always call `bargraph_update_slice(fd, &started, 0, 0)` to zero both compare registers (`CC = 0`) *before* issuing `PWMIOC_STOP` and `close(fd)`.

```mermaid caption="Figure 4.1: Mapping the 6 Logical LED Duties (duties[0..5]) to the 5 Physical RP2350 PWM Slice Descriptors"
flowchart LR
  subgraph Logical["6 Logical Segment Duties"]
    D0["duties[0] (LED1 / D1)"]
    D1["duties[1] (LED2 / D2)"]
    D2["duties[2] (LED3 / D3)"]
    D3["duties[3] (LED4 / D4)"]
    D4["duties[4] (LED5 / D5)"]
    D5["duties[5] (LED6 / D6)"]
  end
  subgraph Slices["5 Physical RP2350 PWM Slices"]
    S5["/dev/pwm5 (idx 3)\nduty_a=0, duty_b=duties[0]"]
    S6["/dev/pwm6 (idx 4)\nduty_a=duties[1], duty_b=0"]
    S2["/dev/pwm2 (idx 1)\nduty_a=0, duty_b=duties[2]"]
    S3["/dev/pwm3 (idx 2)\nduty_a=duties[3], duty_b=duties[4]"]
    S0["/dev/pwm0 (idx 0)\nduty_a=duties[5], duty_b=0"]
  end
  D0 --> S5
  D1 --> S6
  D2 --> S2
  D3 --> S3
  D4 --> S3
  D5 --> S0
```

## 2. Code Walkthrough: `bargraph_update_slice()`, `bargraph_apply_pwm()`, and `bargraph_shutdown_pwm()` {#code-walkthrough}

Let's examine the three PWM driver control functions added in `apps/0004-multichannel-pwm-routing-and-safe-shutdown/src/main.c`.

### Block 1: Updating a Dual-Channel PWM Slice (`bargraph_update_slice`)

```c caption="Populating struct pwm_info_s for both Channel A and Channel B"
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
```

- **Why track `*started` per slice?**: In `rp23xx_pwm.c`, `PWMIOC_SETCHARACTERISTICS` calls `setup_pulse()`, which updates the hardware `CC` register live whenever the slice is already enabled (`CSR_EN = 1`). Calling `PWMIOC_START` only once when `*started == false` avoids redundant clock-divider and GPIO pad re-initialization on every 20 ms tick.

### Block 2: Routing Six LEDs Across Five Slices (`bargraph_apply_pwm`)

```c caption="Routing duties[0..5] to PWM slices 5, 6, 2, 3, and 0"
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
```

- **Atomic Shared-Slice Update on `/dev/pwm3` (`PWM_IDX_SLICE3`)**: Because `LED4` (`GPIO6`, `PWM3A`) and `LED5` (`GPIO7`, `PWM3B`) share Slice 3, passing `duties[3]` as `duty_a` and `duties[4]` as `duty_b` in a single `bargraph_update_slice()` call updates both `CC[15:0]` and `CC[31:16]` together without overwriting one channel while updating the other.

### Block 3: Zero-Before-Stop Hardware Shutdown (`bargraph_shutdown_pwm`)

```c caption="Zeroing compare registers before PWMIOC_STOP and close()"
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
```

- **Three-Step Shutdown Sequence**:
  1. `bargraph_update_slice(..., 0u, 0u)` drives the hardware counter-compare values to zero (`0%` duty).
  2. `PWMIOC_STOP` disables the hardware slice timer (`CSR_EN = 0`).
  3. `close()` invokes `pwm_shutdown()` in `rp23xx_pwm.c`, muxing each GPIO pin back to `RP23XX_GPIO_FUNC_SIO` driven low (`0`).

### Complete src/main.c Listing

```c caption="apps/0004-multichannel-pwm-routing-and-safe-shutdown/src/main.c"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

static struct mock_pwm_slice_s g_mock_slices[BARGRAPH_NUM_PWM_SLICES];

static const struct bargraph_led_map_s g_led_map[BARGRAPH_NUM_LEDS] =
{
  { "D1", 27, "/dev/pwm5", PWM_IDX_SLICE5, 2, 14 },
  { "D2", 28, "/dev/pwm6", PWM_IDX_SLICE6, 1, 15 },
  { "D3",  5, "/dev/pwm2", PWM_IDX_SLICE2, 2, 16 },
  { "D4",  6, "/dev/pwm3", PWM_IDX_SLICE3, 1, 17 },
  { "D5",  7, "/dev/pwm3", PWM_IDX_SLICE3, 2, 18 },
  { "D6",  0, "/dev/pwm0", PWM_IDX_SLICE0, 1, 19 }
};

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

bool bargraph_average_burst(
  const struct adc_msg_s msgs[BARGRAPH_ADC_READ_SAMPLES],
  uint16_t *raw_out)
{
  uint32_t sum = 0u;
  unsigned int valid_count = 0u;
  unsigned int i;

  for (i = 1u; i < BARGRAPH_ADC_READ_SAMPLES; i++)
    {
      if (msgs[i].am_data >= 0)
        {
          uint16_t sample12 = bargraph_extract_adc12(msgs[i].am_data);
          sum += sample12;
          valid_count++;
        }
    }

  if (valid_count == 0u)
    {
      return false;
    }

  *raw_out = (uint16_t)((sum + (valid_count / 2u)) / valid_count);
  return true;
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
  int pwm_fds[BARGRAPH_NUM_PWM_SLICES];
  bool pwm_started[BARGRAPH_NUM_PWM_SLICES];
  const uint16_t test_duties[BARGRAPH_NUM_LEDS] =
    { 1000u, 2000u, 3000u, 4000u, 5000u, 6000u };
  int i;

  assert(g_led_map[3].slice_idx == PWM_IDX_SLICE3);
  assert(bargraph_total_led_current_ua(BARGRAPH_NUM_LEDS) == 23634u);

  memset(g_mock_slices, 0, sizeof(g_mock_slices));
  for (i = 0; i < BARGRAPH_NUM_PWM_SLICES; i++)
    {
      g_mock_slices[i].open = true;
      pwm_fds[i]            = i;
      pwm_started[i]        = false;
    }

  assert(bargraph_apply_pwm(pwm_fds, pwm_started, test_duties) == 0);
  assert(bargraph_apply_pwm(pwm_fds, pwm_started, test_duties) == 0);

  assert(g_mock_slices[PWM_IDX_SLICE5].duty_a == 0u);
  assert(g_mock_slices[PWM_IDX_SLICE5].duty_b == 1000u);
  assert(g_mock_slices[PWM_IDX_SLICE6].duty_a == 2000u);
  assert(g_mock_slices[PWM_IDX_SLICE6].duty_b == 0u);
  assert(g_mock_slices[PWM_IDX_SLICE2].duty_a == 0u);
  assert(g_mock_slices[PWM_IDX_SLICE2].duty_b == 3000u);
  assert(g_mock_slices[PWM_IDX_SLICE3].duty_a == 4000u);
  assert(g_mock_slices[PWM_IDX_SLICE3].duty_b == 5000u);
  assert(g_mock_slices[PWM_IDX_SLICE0].duty_a == 6000u);
  assert(g_mock_slices[PWM_IDX_SLICE0].duty_b == 0u);

  for (i = 0; i < BARGRAPH_NUM_PWM_SLICES; i++)
    {
      assert(g_mock_slices[i].start_count == 1u);
      assert(g_mock_slices[i].running);
    }

  bargraph_shutdown_pwm(pwm_fds, pwm_started);

  for (i = 0; i < BARGRAPH_NUM_PWM_SLICES; i++)
    {
      assert(pwm_fds[i] == -1);
      assert(!pwm_started[i]);
      assert(!g_mock_slices[i].open);
      assert(!g_mock_slices[i].running);
      assert(!g_mock_slices[i].frozen_high);
      assert(g_mock_slices[i].stop_count == 1u);
    }

  printf("Lesson 0004 verification passed: slice3_A=%u, slice3_B=%u\n",
         4000u, 5000u);
  return 0;
}
```

## 3. Interactive Practice & Verification {#practice-and-verification}

:::exercise id="ex-0004" title="Interactive Lab: Dual-Channel PWM Info & Shared Slice 3 Update" badge="Skill Practice • bargraph_apply_pwm"
Complete the dual-channel `pwm_info_s` setup in `bargraph_update_slice()` and the shared Slice 3 (`/dev/pwm3`) update for `LED4` and `LED5`:

```c
info.channels[0].channel = {{blank:1|Hardware Channel A identifier|?|4}};
info.channels[0].duty    = (ub16_t)duty_a;
info.channels[1].channel = {{blank:2|Hardware Channel B identifier|?|4}};
info.channels[1].duty    = (ub16_t)duty_b;

ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE3],
                            &pwm_started[PWM_IDX_SLICE3],
                            {{blank:duties[3]|LED4 (D4/PWM3A) duty cycle|?????????|9}}, {{blank:duties[4]|LED5 (D5/PWM3B) duty cycle|?????????|9}});
```

:::prompts
hint: "Set channels[0].channel = 1 and channels[1].channel = 2. On Slice 3 (/dev/pwm3), LED4 (duties[3]) drives Channel A and LED5 (duties[4]) drives Channel B."
success: "Verified! Your dual-channel PWM configuration and Slice 3 routing match bargraph_main.c."
:::
:::

:::quiz id="quiz-0004" title="Retrieval Practice: Multi-Channel PWM Indexing & Safe Shutdown" badge="3 Questions • Equal-Length Options"
- question: "Why must info.channels[0].channel be 1 even for Channel-B-only LEDs?"
  wrong_hint: "Check how nuttx/drivers/timers/pwm.c validates the channels[] array."
  options:
    - letter: "A"
      text: "Upper-half driver stops scanning channels when channel equals zero value"
      correct: true
      explain: "drivers/timers/pwm.c treats channel == 0 as an end-of-list sentinel during ioctl validation."
    - letter: "B"
      text: "Lower-half driver swaps channel A and channel B duty values"
      correct: false
      explain: "rp23xx_pwm.c always maps index 0 to Channel A and index 1 to Channel B without swapping."
    - letter: "C"
      text: "Hardware timer resets its frequency register when channel equals zero"
      correct: false
      explain: "The channel field is software metadata in struct pwm_chan_s, not a hardware timer bit."
    - letter: "D"
      text: "Kernel closes the file descriptor whenever channel equals zero value"
      correct: false
      explain: "An invalid channel number returns -EINVAL from ioctl rather than closing the descriptor."
- question: "How are LED4 (GPIO6) and LED5 (GPIO7) routed to PWM?"
  wrong_hint: "Look at which hardware PWM slice GPIO6 and GPIO7 share on the RP2350."
  options:
    - letter: "A"
      text: "LED4 drives slice two channel A and LED5 slice three"
      correct: false
      explain: "GPIO6 is PWM3A (Slice 3 Channel A), whereas Slice 2 drives LED3 (GPIO5 / PWM2B)."
    - letter: "B"
      text: "LED4 drives slice three channel A and LED5 channel B"
      correct: true
      explain: "GPIO6 (PWM3A) and GPIO7 (PWM3B) share /dev/pwm3 as Channel A and Channel B."
    - letter: "C"
      text: "LED4 drives slice five channel B and LED5 channel A"
      correct: false
      explain: "Slice 5 (/dev/pwm5) drives LED1 on GPIO27 (PWM5B)."
    - letter: "D"
      text: "LED4 drives slice six channel A and LED5 slice zero"
      correct: false
      explain: "Slice 6 drives LED2 (GPIO28) and Slice 0 drives LED6 (GPIO0)."
- question: "How does bargraph_shutdown_pwm prevent LEDs from freezing ON when stopped?"
  wrong_hint: "Recall what pwm_stop() in rp23xx_pwm.c does to the GPIO output state."
  options:
    - letter: "A"
      text: "Zero both channel duties before issuing PWMIOC_STOP and closing descriptors"
      correct: true
      explain: "Setting duty_a=0 and duty_b=0 before PWMIOC_STOP and close() guarantees all pads go low."
    - letter: "B"
      text: "Issue PWMIOC_STOP immediately before setting frequency to zero hertz value"
      correct: false
      explain: "Issuing PWMIOC_STOP while duty > 0 freezes the pad at whatever state the counter held."
    - letter: "C"
      text: "Close the file descriptor without calling any PWM ioctl command"
      correct: false
      explain: "Explicitly zeroing duties and stopping the slice ensures deterministic hardware state."
    - letter: "D"
      text: "Set both channel duties to 65535 before issuing PWMIOC_STOP command"
      correct: false
      explain: "Setting duties to 65535 drives the output high (100% ON) right before freezing the counter."
:::

:::verify id="verify-0004" title="Verification Lab: Compile & Run Lesson 0004 Companion App" badge="Terminal Gate"
Execute the Lesson 0004 verification suite from `tutorial/`:

```bash caption="Compile and run Lesson 0004 verification gate"
make -C apps/0004-multichannel-pwm-routing-and-safe-shutdown test
```

Expected terminal output:

```text caption="Verified output from apps/0004-multichannel-pwm-routing-and-safe-shutdown"
cc -std=c99 -Wall -Wextra -Werror -pedantic -O2 src/main.c -o build/bargraph_lesson
./build/bargraph_lesson
Lesson 0004 verification passed: slice3_A=4000, slice3_B=5000
```

- [ ] Confirm `g_mock_slices[PWM_IDX_SLICE3]` receives `duty_a == 4000` (`LED4`) and `duty_b == 5000` (`LED5`).
- [ ] Confirm `bargraph_shutdown_pwm()` leaves `frozen_high == false` across all 5 slices.
:::

:::teacher-prompt title="Extend This Lesson with Your Agent"
Ask your AI teacher: *"Why must `CONFIG_RP23XX_PWM0B_GPIO=-1`, `CONFIG_RP23XX_PWM2A_GPIO=-1`, `CONFIG_RP23XX_PWM5A_GPIO=-1`, and `CONFIG_RP23XX_PWM6B_GPIO=-1` be set in `defconfig` when two-channel PWM mode is enabled?"*
:::
