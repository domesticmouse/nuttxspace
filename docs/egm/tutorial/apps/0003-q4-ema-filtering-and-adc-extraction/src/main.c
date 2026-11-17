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
