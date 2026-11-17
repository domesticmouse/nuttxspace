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
