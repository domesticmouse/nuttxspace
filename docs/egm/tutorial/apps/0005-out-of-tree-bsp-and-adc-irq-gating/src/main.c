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
