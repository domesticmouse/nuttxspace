---
title: "Proportional 6-Segment Bar Graph Math & 16-Bit PWM Scaling"
page_title: "Lesson 0002: Proportional 6-Segment Bar Graph Math & 16-Bit PWM Scaling — XIAO RP2350 Bar Graph"
nav_id: "lesson-0002"
badge: "Lesson 0002 • 16-Bit Duty Scaling & Interpolation"
subtitle: "Eliminate the b16ONE (65536 -> 0) uint16_t wraparound bug in rp23xx_pwm.c and implement the continuous 6-segment duty cycle interpolator."
card:
  title: "**Proportional 6-Segment Bar Graph Math & 16-Bit PWM Scaling**"
  summary: "Discover why standard NuttX b16ONE (65536) wraps to 0% in rp23xx_pwm.c, and implement bargraph_compute_duties() across 393,210 total units."
---

:::callout title="Recommended Primary Sources"
- **Companion project**: `apps/0002-proportional-bargraph-math-and-pwm-scaling/` (`make -C apps/0002-proportional-bargraph-math-and-pwm-scaling test`)
- **Primary driver docs**: [Apache NuttX PWM Driver Guide](https://nuttx.apache.org/docs/latest/components/drivers/special/pwm.html) and `nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c`
- **Course references**: [Glossary](../reference/GLOSSARY.md), [7 NuttX & RP2350 Driver Traps](../reference/nuttx-rp2350-driver-traps.md), [Lesson 0001](0001-breadboard-circuit-and-current-budget.md), and [Lesson 0003: Q4 EMA Filtering & ADC Extraction](0003-q4-ema-filtering-and-adc-extraction.md)
:::

## 1. The `b16ONE` (`65536` → `0`) Truncation Trap in `rp23xx_pwm.c` {#b16one-trap}

In Apache NuttX, the upper-half PWM header (`<nuttx/timers/pwm.h>`) defines `pwm_info_s.duty` as an unsigned 16.16 fixed-point number (`ub16_t`, a `uint32_t` typedef) where `100%` duty is `b16ONE = 0x00010000` (`65536`).{{sidenote:A standard NuttX application written for STM32 or ESP32 typically passes `b16ONE` (`65536`) for 100% duty cycle.}}

However, inspecting the RP2350 lower-half PWM driver (`nuttx/arch/arm/src/rp23xx/rp23xx_pwm.c`) reveals a critical type-narrowing bug:

- `struct rp23xx_pwm_lowerhalf_s` stores each channel's duty cycle in a **16-bit unsigned integer (`uint16_t duty[i]`)** rather than `ub16_t` (`uint32_t`).
- Inside `setup_pulse()`, the driver computes the hardware counter-compare register (`CC`) as:
  `((uint64_t)priv->duty[i] * (priv->top + 1)) / 65535`
- If user space passes `b16ONE` (`0x00010000 = 65536`) for `100%` brightness, assigning `65536` into `uint16_t priv->duty[i]` **truncates the lower 16 bits to `0x0000` (`0`)**, turning the LED completely OFF the instant it reaches full brightness!

:::callout-warn title="Trap #4: Bound All PWM Duties Strictly to BARGRAPH_DUTY_MAX (65535)"
To keep `nuttx/` 100% unmodified while achieving true `100%` duty (`(65535 * (top + 1)) / 65535 == top + 1`), our application defines `BARGRAPH_DUTY_MAX = 65535u` (`0xFFFF`) and never passes `65536`.
:::

```mermaid caption="Figure 2.1: Six Contiguous 65,535-Unit Brightness Windows Across BARGRAPH_TOTAL_UNITS_MAX (393,210)"
flowchart LR
  U["total_units\n0 .. 393,210"] --> S0["Seg 1 (LED1 / D1)\n0 .. 65,535"]
  U --> S1["Seg 2 (LED2 / D2)\n65,535 .. 131,070"]
  U --> S2["Seg 3 (LED3 / D3)\n131,070 .. 196,605"]
  U --> S3["Seg 4 (LED4 / D4)\n196,605 .. 262,140"]
  U --> S4["Seg 5 (LED5 / D5)\n262,140 .. 327,675"]
  U --> S5["Seg 6 (LED6 / D6)\n327,675 .. 393,210"]
```

## 2. Interactive 6-Segment PWM Duty & Overflow Explorer {#pwm-visualizer}

Drag the slider below across `0 .. 393,210` total bar graph units (`BARGRAPH_TOTAL_UNITS_MAX`) to see how each segment saturates at `65535` below the active window, interpolates proportionally inside the active window, and stays `0` above it:

{{widget:pwm-segment-visualizer}}

## 3. Code Walkthrough: Continuous 6-Segment Interpolation {#code-walkthrough}

Let's examine the two new blocks added to `apps/0002-proportional-bargraph-math-and-pwm-scaling/src/main.c`.

### Block 1: Scaling 12-Bit ADC Counts to Total Bar Graph Units

```c caption="Mapping 0..4095 ADC counts into 0..393210 total bar graph units"
#define BARGRAPH_DUTY_MAX        65535u
#define BARGRAPH_TOTAL_UNITS_MAX (BARGRAPH_NUM_LEDS * BARGRAPH_DUTY_MAX)

uint32_t bargraph_units_from_adc(uint16_t adc_count)
{
  if (adc_count > BARGRAPH_ADC_MAX)
    {
      adc_count = BARGRAPH_ADC_MAX;
    }

  return ((uint32_t)adc_count * BARGRAPH_TOTAL_UNITS_MAX + 2047u) /
         BARGRAPH_ADC_MAX;
}
```

- **`BARGRAPH_TOTAL_UNITS_MAX` (`393210u`)**: Partitioning the full range into `6 * 65535 = 393,210` integer units means every unit of `total_units` corresponds to exactly one LSB of PWM duty cycle in the active LED segment.
- **32-Bit Overflow Safety**: At full scale (`adc_count = 4095`), the numerator is `4095 * 393210 + 2047 = 1,610,196,997`, which is well below `UINT32_MAX` (`4,294,967,295`).

### Block 2: Computing Per-Segment PWM Duties (`bargraph_compute_duties`)

```c caption="Piecewise-linear 6-segment duty cycle interpolator"
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
```

- **Three Deterministic Regimes per Segment `i` (`0..5`)**:
  1. **Fully Lit (`total_units >= seg_end`)**: The reading has passed the top of segment `i`, so `duties[i]` clamps at `65535u` (`100%`).
  2. **Proportional Active Segment (`seg_start < total_units < seg_end`)**: The reading lies inside segment `i`, so `duties[i] = total_units - seg_start` (`1..65534`).
  3. **Dark (`total_units <= seg_start`)**: The reading has not yet reached segment `i`, so `duties[i] = 0u` (`0%`).

### Complete src/main.c Listing

```c caption="apps/0002-proportional-bargraph-math-and-pwm-scaling/src/main.c"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BARGRAPH_NUM_LEDS        6
#define BARGRAPH_NUM_PWM_SLICES  5
#define BARGRAPH_ADC_MAX         4095u
#define BARGRAPH_VDD_MV          3300u
#define BARGRAPH_LED_VF_MV       2000u
#define BARGRAPH_LED_RES_OHM     330u
#define BARGRAPH_PAD_LIMIT_UA    12000u
#define BARGRAPH_IOVDD_LIMIT_UA  200000u
#define BARGRAPH_ADC_GPIO        26u

#define BARGRAPH_DUTY_MAX        65535u
#define BARGRAPH_TOTAL_UNITS_MAX (BARGRAPH_NUM_LEDS * BARGRAPH_DUTY_MAX)

#define PWM_IDX_SLICE0           0
#define PWM_IDX_SLICE2           1
#define PWM_IDX_SLICE3           2
#define PWM_IDX_SLICE5           3
#define PWM_IDX_SLICE6           4

struct bargraph_led_map_s
{
  const char *xiao_pin;
  uint8_t     gpio;
  const char *pwm_devpath;
  uint8_t     slice_idx;
  uint8_t     channel;
  uint8_t     breadboard_row;
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

int main(void)
{
  uint16_t duties[BARGRAPH_NUM_LEDS];
  uint32_t b16_one_raw = 0x00010000u;
  uint16_t truncated_b16 = (uint16_t)b16_one_raw;
  unsigned int i;

  assert(g_led_map[0].gpio == 27u);
  assert(bargraph_total_led_current_ua(BARGRAPH_NUM_LEDS) == 23634u);
  assert(truncated_b16 == 0u);

  bargraph_compute_duties(0u, duties);
  for (i = 0; i < BARGRAPH_NUM_LEDS; i++)
    {
      assert(duties[i] == 0u);
    }

  for (i = 1; i <= BARGRAPH_NUM_LEDS; i++)
    {
      unsigned int k;
      bargraph_compute_duties(i * BARGRAPH_DUTY_MAX, duties);
      for (k = 0; k < BARGRAPH_NUM_LEDS; k++)
        {
          assert(duties[k] <= BARGRAPH_DUTY_MAX);
          if (k < i)
            {
              assert(duties[k] == BARGRAPH_DUTY_MAX);
            }
          else
            {
              assert(duties[k] == 0u);
            }
        }
    }

  bargraph_compute_duties(2u * BARGRAPH_DUTY_MAX + 12345u, duties);
  assert(duties[0] == 65535u && duties[1] == 65535u && duties[2] == 12345u);
  assert(duties[3] == 0u && duties[4] == 0u && duties[5] == 0u);

  bargraph_compute_duties(bargraph_units_from_adc(4095u), duties);
  for (i = 0; i < BARGRAPH_NUM_LEDS; i++)
    {
      assert(duties[i] == 65535u);
    }

  printf("Lesson 0002 verification passed: max_units=%u, seg3_partial=%u\n",
         (unsigned int)BARGRAPH_TOTAL_UNITS_MAX, 12345u);
  return 0;
}
```

## 4. Interactive Practice & Verification {#practice-and-verification}

:::exercise id="ex-0002" title="Interactive Lab: Piecewise-Linear Segment Duty Interpolation" badge="Skill Practice • bargraph_compute_duties"
Fill in the window bounds and active-segment interpolation inside `bargraph_compute_duties()`:

```c
uint32_t seg_start = (uint32_t)i * {{blank:BARGRAPH_DUTY_MAX||65535u||65535|Maximum 16-bit duty per segment|?????????????????|17}};
uint32_t seg_end   = seg_start + BARGRAPH_DUTY_MAX;

if (total_units >= {{blank:seg_end|Upper boundary of segment i|???????|8}})
  {
    duties[i] = (uint16_t)BARGRAPH_DUTY_MAX;
  }
else if (total_units > seg_start)
  {
    duties[i] = (uint16_t)({{blank:total_units - seg_start|Fractional offset inside segment i|???????????????????????|23}});
  }
```

:::prompts
hint: "Each segment spans BARGRAPH_DUTY_MAX (65535u) units from seg_start to seg_end, and the active segment duty is total_units - seg_start."
success: "Verified! Your piecewise-linear 6-segment duty calculation is 16-bit safe and matches bargraph_main.c."
:::
:::

:::quiz id="quiz-0002" title="Retrieval Practice: 16-Bit PWM Scaling & Segment Interpolation" badge="3 Questions • Equal-Length Options"
- question: "What happens if user space passes b16ONE (65536) to rp23xx_pwm.c?"
  wrong_hint: "Check the integer type of priv->duty[i] in struct rp23xx_pwm_lowerhalf_s."
  options:
    - letter: "A"
      text: "Passing b16ONE overflows uint16_t and truncates full brightness to zero"
      correct: true
      explain: "65536 is 0x00010000, which truncates to 0x0000 when stored in a 16-bit uint16_t field."
    - letter: "B"
      text: "Passing b16ONE triggers a kernel panic inside the PWM driver"
      correct: false
      explain: "C unsigned integer conversion silently wraps modulo 65536 without raising an exception."
    - letter: "C"
      text: "Passing b16ONE doubles the hardware PWM carrier frequency to 2kHz"
      correct: false
      explain: "The duty cycle field controls the counter-compare register CC, not the TOP period register."
    - letter: "D"
      text: "Passing b16ONE locks every other PWM slice in reset state"
      correct: false
      explain: "Each PWM slice has independent CC registers; overflow only zeroes that channel's duty."
- question: "What is the value of BARGRAPH_TOTAL_UNITS_MAX across all six segments?"
  wrong_hint: "Multiply 6 segments by 65,535 maximum duty units per segment."
  options:
    - letter: "A"
      text: "Three hundred ninety-three thousand two hundred sixteen total bargraph units"
      correct: false
      explain: "393,216 is 6 * 65,536 (using b16ONE), which is 6 units too high."
    - letter: "B"
      text: "Sixty-five thousand five hundred thirty-five total bargraph units across segments"
      correct: false
      explain: "65,535 is the maximum duty for a single LED segment, not all six combined."
    - letter: "C"
      text: "Three hundred ninety-three thousand two hundred ten total bargraph units"
      correct: true
      explain: "6 * 65,535 equals 393,210 total units across the six bar graph segments."
    - letter: "D"
      text: "Four thousand ninety-five total bargraph units across all six segments"
      correct: false
      explain: "4,095 is BARGRAPH_ADC_MAX (the 12-bit raw ADC ceiling), not the scaled PWM unit ceiling."
- question: "When total_units equals 2 * 65535 + 12345, which LEDs illuminate?"
  wrong_hint: "Count how many full 65535-unit windows fit inside 2 * 65535 + 12345."
  options:
    - letter: "A"
      text: "Segments one through three lock full while segment four dims"
      correct: false
      explain: "Only two full 65,535-unit windows have been completed, not three."
    - letter: "B"
      text: "Segments one and two lock full while segment three dims"
      correct: true
      explain: "duties[0] and duties[1] saturate at 65535, duties[2] is 12345, and duties[3..5] are 0."
    - letter: "C"
      text: "Segment one locks full while segments two through six dim"
      correct: false
      explain: "Only one segment interpolates at a time; upper segments above seg_end remain at 0."
    - letter: "D"
      text: "All six segments illuminate together at identical thirty-percent duty cycle"
      correct: false
      explain: "A bar graph fills segments sequentially rather than dimming all LEDs uniformly."
:::

:::verify id="verify-0002" title="Verification Lab: Compile & Run Lesson 0002 Companion App" badge="Terminal Gate"
Execute the Lesson 0002 test gate from `tutorial/`:

```bash caption="Compile and run Lesson 0002 verification gate"
make -C apps/0002-proportional-bargraph-math-and-pwm-scaling test
```

Expected terminal output:

```text caption="Verified output from apps/0002-proportional-bargraph-math-and-pwm-scaling"
cc -std=c99 -Wall -Wextra -Werror -pedantic -O2 src/main.c -o build/bargraph_lesson
./build/bargraph_lesson
Lesson 0002 verification passed: max_units=393210, seg3_partial=12345
```

- [ ] Confirm `truncated_b16 == 0u` demonstrates why `b16ONE` (`0x00010000`) wraps to zero in a `uint16_t`.
- [ ] Run `make -C apps/0002-proportional-bargraph-math-and-pwm-scaling test` and verify `max_units=393210, seg3_partial=12345`.
:::

:::teacher-prompt title="Extend This Lesson with Your Agent"
Ask your AI teacher: *"How would we modify `bargraph_compute_duties()` if we wanted a 'dot mode' display where only the single active segment lights up while lower segments turn off?"*
:::
