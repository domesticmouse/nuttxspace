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
