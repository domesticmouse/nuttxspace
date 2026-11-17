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

int main(void)
{
  uint32_t single_ua = bargraph_led_current_ua(BARGRAPH_VDD_MV,
                                               BARGRAPH_LED_VF_MV,
                                               BARGRAPH_LED_RES_OHM);
  uint32_t total_ua  = bargraph_total_led_current_ua(BARGRAPH_NUM_LEDS);
  unsigned int i;

  assert(single_ua == 3939u);
  assert(single_ua < BARGRAPH_PAD_LIMIT_UA);
  assert(total_ua == 23634u);
  assert(total_ua < BARGRAPH_IOVDD_LIMIT_UA);

  assert(bargraph_adc_to_mv(0) == 0u);
  assert(bargraph_adc_to_mv(2048) == 1650u);
  assert(bargraph_adc_to_mv(4095) == 3300u);

  assert(bargraph_adc_to_permille(0) == 0u);
  assert(bargraph_adc_to_permille(2048) == 500u);
  assert(bargraph_adc_to_permille(4095) == 1000u);

  for (i = 0; i < BARGRAPH_NUM_LEDS; i++)
    {
      assert(g_led_map[i].slice_idx < BARGRAPH_NUM_PWM_SLICES);
      assert(g_led_map[i].channel == 1u || g_led_map[i].channel == 2u);
      assert(g_led_map[i].breadboard_row == 14u + i);
    }

  printf("Lesson 0001 verification passed: single=%u uA, total=%u uA\n",
         (unsigned int)single_ua, (unsigned int)total_ua);
  return 0;
}
