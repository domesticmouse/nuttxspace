---
title: "Fixed-Point Q4 EMA Filtering, Deadbands & ADC Sample Extraction"
page_title: "Lesson 0003: Fixed-Point Q4 EMA Filtering, Deadbands & ADC Sample Extraction — XIAO RP2350 Bar Graph"
nav_id: "lesson-0003"
badge: "Lesson 0003 • ADC Shift Recovery & Q4 EMA Filter"
subtitle: "Unpack rp23xx_adc.c's 19-bit left shift, discard the stale first sample in each 5-sample burst, and smooth potentiometer jitter with a multiplication-free Q4 Exponential Moving Average."
card:
  title: "**Fixed-Point Q4 EMA Filtering, Deadbands & ADC Sample Extraction**"
  summary: "Recover 12-bit ADC counts from rp23xx_adc.c's << 19 format, discard stale burst samples, and implement bargraph_filter_adc() with Q4 EMA smoothing and endpoint deadbands."
---

:::callout title="Recommended Primary Sources"
- **Companion project**: `apps/0003-q4-ema-filtering-and-adc-extraction/` (`make -C apps/0003-q4-ema-filtering-and-adc-extraction test`)
- **Primary driver docs**: [Apache NuttX Analog (ADC) Driver Guide](https://nuttx.apache.org/docs/latest/components/drivers/character/analog.html) and `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c`
- **Course references**: [Glossary](../reference/GLOSSARY.md), [7 NuttX & RP2350 Driver Traps](../reference/nuttx-rp2350-driver-traps.md), [Lesson 0002](0002-proportional-bargraph-math-and-pwm-scaling.md), and [Lesson 0004: Multi-Channel RP2350 PWM Routing](0004-multichannel-pwm-routing-and-safe-shutdown.md)
:::

## 1. Two ADC Driver Quirks in `rp23xx_adc.c`: `<< 19` Shift & Stale `msgs[0]` {#adc-quirks}

When user space reads `struct adc_msg_s` records from `/dev/adc0` on the RP2350, two lower-half driver behaviors in `nuttx/arch/arm/src/rp23xx/rp23xx_adc.c` must be handled before computing LED duty cycles:

1. **19-Bit Left-Shift in `interrupt_handler()`**:
   The RP2350 SAR ADC produces a 12-bit unsigned result (`0..4095`). Inside `rp23xx_adc.c`'s ISR, the driver normalizes the 12-bit sample toward the top of the signed 32-bit `int32_t am_data` field by executing `value <<= 19` (placing the 12-bit conversion in bits `30:19` so bit `31` remains `0` for positive readings, or negative if the hardware FIFO `ERR` bit is set).{{sidenote:Because bit 31 is the sign bit of `int32_t am_data`, checking `msgs[i].am_data >= 0` verifies that the RP2350 ADC conversion completed without a hardware FIFO error flag before we shift right by 19 bits.}}
2. **Stale First Sample (`msgs[0]`) at the Start of Every Burst**:
   At the end of every ADC FIFO interrupt, `interrupt_handler()` immediately calls `get_next_channel()` and writes `RP23XX_ADC_CS_START_ONCE`. When we mask the ADC IRQ in the NVIC between 50 Hz ticks (which we will wire up in Lesson 0005), one last conversion completes `2 µs` after the previous read and sits latched in the RP2350 hardware `ADC_FIFO` register across the entire `20 ms` sleep interval. Even after flushing the *software* ring buffer via `ioctl(adc_fd, ANIOC_RESET_FIFO, 0)`, the first sample delivered on unmasking the IRQ (`msgs[0]`) is that `20 ms`-old hardware-latched reading!

:::callout-warn title="Trap #7 Part A: Read 5 Samples, Discard msgs[0], and Shift >> 19"
By reading `BARGRAPH_ADC_READ_SAMPLES = 5` (`BARGRAPH_ADC_BURST_SAMPLES + 1`), discarding `msgs[0]`, and averaging `msgs[1..4]` after shifting `(am_data >> 19) & 0x0fff`, every 50 Hz tick measures four guaranteed-fresh conversions (`8 µs` total conversion window).
:::

```mermaid caption="Figure 3.1: 5-Sample Burst Extraction, Q4 Exponential Moving Average (alpha = 1/4), and Endpoint Deadband Pipeline"
flowchart LR
  FIFO["5x struct adc_msg_s\nmsgs[0..4]"] -->|"Discard stale msgs[0]\n(am_data >> 19) & 0x0fff"| AVG["4-Sample Rounded Avg\nraw_avg (0..4095)"]
  AVG -->|"ema_q4 = ema_q4 - (ema_q4 >> 2)\n+ (raw_avg << 2)"| ROUND["Nearest Integer\n(ema_q4 + 8) >> 4"]
  ROUND -->|"<= 32 -> 0\n>= 4063 -> 4095"| OUT["filtered_adc\n0..4095"]
```

## 2. Interactive Q4 EMA Filter & Deadband Simulator {#ema-simulator}

Use the simulator below to test how the Q4 fixed-point Exponential Moving Average (`alpha = 1/4`) responds to step changes and how the endpoint deadbands (`<= 32` and `>= 4063`) lock the ends of potentiometer travel to `0` and `4095`:

{{widget:adc-ema-simulator}}

## 3. Code Walkthrough: Burst Extraction & `bargraph_filter_adc()` {#code-walkthrough}

Let's walk through the three new blocks added in `apps/0003-q4-ema-filtering-and-adc-extraction/src/main.c`.

### Block 1: 12-Bit Sample Extraction & 5-Sample Burst Averaging

```c caption="Extracting 12-bit samples (>> 19) and averaging fresh burst samples msgs[1..4]"
uint16_t bargraph_extract_adc12(int32_t am_data)
{
  return (uint16_t)((am_data >> BARGRAPH_ADC_SHIFT) & 0x0fff);
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
```

- **Loop starts at `i = 1u`**: Index `0` (`msgs[0]`) is intentionally skipped because it was latched in the hardware `ADC_FIFO` at the end of the previous 20 ms tick.
- **Error Guard (`msgs[i].am_data >= 0`)**: Skips any sample flagged negative by `rp23xx_adc.c` on hardware conversion error.
- **Rounded Mean**: Adding `valid_count / 2u` (`2` when `valid_count == 4`) before dividing by `valid_count` rounds the 4-sample average to the nearest integer.

### Block 2: Multiplication-Free Q4 Fixed-Point EMA Filter & Deadbands

```c caption="Q4 fixed-point Exponential Moving Average (alpha = 1/4) with endpoint deadbands"
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
```

- **Why Q4 Fixed-Point (`* 16`)?**: An integer EMA of the form `y = (3*y + x) / 4` suffers from truncation limit cycles where `y` gets stuck up to 3 counts away from `x`. Keeping the accumulator `ema_q4` scaled by `2^4 = 16` preserves 4 fractional bits across ticks.{{sidenote:With `alpha = 1/4`, `16 * y_new = 16 * y_old - 4 * y_old + 4 * raw_avg`, which simplifies to `(ema_q4 - (ema_q4 >> 2)) + (raw_avg << 2)`—requiring only bit shifts and additions!}}
- **Cold-Start Seeding (`!*ema_initialized`)**: On the very first tick after boot, `*ema_q4` is seeded directly to `raw_avg << 4` so the bar graph immediately snaps to the true potentiometer position instead of ramping up from `0`.
- **Symmetric Endpoint Deadbands (`<= 32` and `>= 4063`)**: Mechanical `10 kΩ` potentiometers have small end-stop contact resistance (`≈ 25 mV`, or `≈ 32` ADC counts). Clamping `filtered <= 32` to `0` and `filtered >= 4063` (`4095 - 32`) to `4095` guarantees that full counter-clockwise travel always reads `0.0%` (all LEDs off) and full clockwise travel always reads `100.0%` (all 6 LEDs at `65535`).

### Complete src/main.c Listing

```c caption="apps/0003-q4-ema-filtering-and-adc-extraction/src/main.c"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BARGRAPH_NUM_LEDS          6
#define BARGRAPH_NUM_PWM_SLICES    5
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

static const struct bargraph_led_map_s g_led_map[BARGRAPH_NUM_LEDS] =
{
  { "D1", 27, "/dev/pwm5", PWM_IDX_SLICE5, 2, 14 },
  { "D2", 28, "/dev/pwm6", PWM_IDX_SLICE6, 1, 15 },
  { "D3",  5, "/dev/pwm2", PWM_IDX_SLICE2, 2, 16 },
  { "D4",  6, "/dev/pwm3", PWM_IDX_SLICE3, 1, 17 },
  { "D5",  7, "/dev/pwm3", PWM_IDX_SLICE3, 2, 18 },
  { "D6",  0, "/dev/pwm0", PWM_IDX_SLICE0, 1, 19 }
};

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

int main(void)
{
  struct adc_msg_s burst[BARGRAPH_ADC_READ_SAMPLES];
  uint32_t ema_q4 = 0u;
  bool ema_init = false;
  uint16_t raw_avg = 0u;
  uint16_t step1;
  uint16_t step2;

  assert(g_led_map[5].gpio == 0u);
  assert(bargraph_total_led_current_ua(BARGRAPH_NUM_LEDS) == 23634u);

  assert(bargraph_extract_adc12((int32_t)(4095 << BARGRAPH_ADC_SHIFT)) == 4095u);
  assert(bargraph_extract_adc12((int32_t)(2048 << BARGRAPH_ADC_SHIFT)) == 2048u);

  burst[0].am_channel = 0u;
  burst[0].am_data    = (int32_t)(100 << BARGRAPH_ADC_SHIFT);
  burst[1].am_channel = 0u;
  burst[1].am_data    = (int32_t)(2000 << BARGRAPH_ADC_SHIFT);
  burst[2].am_channel = 0u;
  burst[2].am_data    = (int32_t)(2002 << BARGRAPH_ADC_SHIFT);
  burst[3].am_channel = 0u;
  burst[3].am_data    = (int32_t)(2004 << BARGRAPH_ADC_SHIFT);
  burst[4].am_channel = 0u;
  burst[4].am_data    = (int32_t)(2006 << BARGRAPH_ADC_SHIFT);

  assert(bargraph_average_burst(burst, &raw_avg));
  assert(raw_avg == 2003u);

  ema_init = false;
  assert(bargraph_filter_adc(0u, &ema_q4, &ema_init) == 0u);
  ema_init = false;
  assert(bargraph_filter_adc(32u, &ema_q4, &ema_init) == 0u);
  ema_init = false;
  assert(bargraph_filter_adc(33u, &ema_q4, &ema_init) == 33u);
  ema_init = false;
  assert(bargraph_filter_adc(2048u, &ema_q4, &ema_init) == 2048u);
  ema_init = false;
  assert(bargraph_filter_adc(4062u, &ema_q4, &ema_init) == 4062u);
  ema_init = false;
  assert(bargraph_filter_adc(4063u, &ema_q4, &ema_init) == 4095u);
  ema_init = false;
  assert(bargraph_filter_adc(4095u, &ema_q4, &ema_init) == 4095u);

  ema_init = false;
  assert(bargraph_filter_adc(1000u, &ema_q4, &ema_init) == 1000u);
  step1 = bargraph_filter_adc(2000u, &ema_q4, &ema_init);
  step2 = bargraph_filter_adc(2000u, &ema_q4, &ema_init);
  assert(step1 == 1250u);
  assert(step2 == 1438u);

  printf("Lesson 0003 verification passed: burst_avg=%u, step1=%u, step2=%u\n",
         (unsigned int)raw_avg, (unsigned int)step1, (unsigned int)step2);
  return 0;
}
```

## 4. Interactive Practice & Verification {#practice-and-verification}

:::exercise id="ex-0003" title="Interactive Lab: 19-Bit Shift Recovery & Q4 EMA Update" badge="Skill Practice • bargraph_filter_adc"
Complete the 12-bit sample extraction and the Q4 fixed-point EMA update and rounding expressions:

```c
uint16_t sample12 = (uint16_t)((msgs[i].am_data >> {{blank:BARGRAPH_ADC_SHIFT||19|Right-shift bit count in rp23xx_adc.c|??|18}}) & 0x0fff);
*ema_q4 = *ema_q4 - (*ema_q4 >> {{blank:2|Shift for 1/4 decay term|?|4}}) + ((uint32_t)raw_avg << 2);
filtered = (uint16_t)((*ema_q4 + {{blank:8u||8|Half of 16 for Q4 nearest-integer rounding|??|4}}) >> 4);
```

:::prompts
hint: "rp23xx_adc.c shifts samples left by BARGRAPH_ADC_SHIFT (19). In Q4 fixed-point, alpha=1/4 subtracts (*ema_q4 >> 2) and rounds via (*ema_q4 + 8u) >> 4."
success: "Verified! Your 19-bit shift extraction and Q4 EMA filter match bargraph_main.c."
:::
:::

:::quiz id="quiz-0003" title="Retrieval Practice: ADC Shift Recovery, Burst Averaging & Q4 EMA" badge="3 Questions • Equal-Length Options"
- question: "How does bargraph_sample_adc recover a 12-bit sample from am_data?"
  wrong_hint: "Check how interrupt_handler() in rp23xx_adc.c shifts each 12-bit reading."
  options:
    - letter: "A"
      text: "Shift right by twelve bits and mask with constant 0xffff"
      correct: false
      explain: "rp23xx_adc.c shifts left by 19 bits, not 12 bits, and the sample is 12 bits wide (0x0fff)."
    - letter: "B"
      text: "Shift right by nineteen bits and mask with constant 0x0fff"
      correct: true
      explain: "Shifting right by 19 bits (BARGRAPH_ADC_SHIFT) and masking with 0x0fff recovers 0..4095."
    - letter: "C"
      text: "Shift left by nineteen bits and mask with constant 0x0fff"
      correct: false
      explain: "The driver already shifted left by 19 bits; user space must shift right by 19 bits."
    - letter: "D"
      text: "Shift right by sixteen bits and divide by constant 4095"
      correct: false
      explain: "Shifting right by 16 leaves the value multiplied by 8, and dividing by 4095 zeroes it."
- question: "Why does the burst loop start at index 1 instead of 0?"
  wrong_hint: "Consider what sits inside the hardware ADC_FIFO register while the IRQ is masked."
  options:
    - letter: "A"
      text: "Sample zero holds the stale conversion from the previous burst"
      correct: true
      explain: "START_ONCE triggers one final conversion at the end of the prior burst that stays latched in hardware."
    - letter: "B"
      text: "Sample zero always contains the channel identifier instead of data"
      correct: false
      explain: "Every struct adc_msg_s has both am_channel and am_data fields."
    - letter: "C"
      text: "Sample zero overflows the thirty-two-bit signed integer ADC message field"
      correct: false
      explain: "All valid 12-bit samples shifted left by 19 bits fit within bits 30:19 of int32_t."
    - letter: "D"
      text: "Sample zero triggers a hardware reset inside the ADC peripheral"
      correct: false
      explain: "Reading msgs[0] from the buffer does not reset the ADC peripheral."
- question: "How do the endpoint deadbands clamp filtered 12-bit ADC values?"
  wrong_hint: "Check BARGRAPH_ADC_DEADBAND_LOW (32) and BARGRAPH_ADC_DEADBAND_HIGH (4063)."
  options:
    - letter: "A"
      text: "Values at or below 16 clamp zero; 4080 clamps 4095"
      correct: false
      explain: "A 16-count deadband is too narrow for typical mechanical potentiometer end-stop resistance."
    - letter: "B"
      text: "Values at or below 64 clamp zero; 4031 clamps 4095"
      correct: false
      explain: "Our deadband threshold is 32 counts at each end, not 64 counts."
    - letter: "C"
      text: "Values at or below 10 clamp zero; 4085 clamps 4095"
      correct: false
      explain: "BARGRAPH_ADC_DEADBAND_LOW is 32u and BARGRAPH_ADC_DEADBAND_HIGH is 4063u."
    - letter: "D"
      text: "Values at or below 32 clamp zero; 4063 clamps 4095"
      correct: true
      explain: "Filtered counts <= 32 clamp to 0 and counts >= 4063 clamp to 4095 for clean 0% and 100% stops."
:::

:::verify id="verify-0003" title="Verification Lab: Compile & Run Lesson 0003 Companion App" badge="Terminal Gate"
Run the Lesson 0003 verification suite from `tutorial/`:

```bash caption="Compile and run Lesson 0003 verification gate"
make -C apps/0003-q4-ema-filtering-and-adc-extraction test
```

Expected terminal output:

```text caption="Verified output from apps/0003-q4-ema-filtering-and-adc-extraction"
cc -std=c99 -Wall -Wextra -Werror -pedantic -O2 src/main.c -o build/bargraph_lesson
./build/bargraph_lesson
Lesson 0003 verification passed: burst_avg=2003, step1=1250, step2=1438
```

- [ ] Verify that `burst[0]` (`100`) is discarded and `burst[1..4]` (`2000, 2002, 2004, 2006`) average to `2003`.
- [ ] Run `make -C apps/0003-q4-ema-filtering-and-adc-extraction test` and confirm `step1=1250, step2=1438`.
:::

:::teacher-prompt title="Extend This Lesson with Your Agent"
Ask your AI teacher: *"What is the 90% settling time in milliseconds of our Q4 EMA filter (`alpha = 1/4`) running at 50 Hz (`20 ms` per tick), and how would changing `alpha` to `1/8` affect responsiveness?"*
:::
