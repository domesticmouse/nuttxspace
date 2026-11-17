#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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
#define BARGRAPH_TEST_HALF_STEPS   50u

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

enum bargraph_mode_e
{
  BARGRAPH_MODE_STOPPED = 0,
  BARGRAPH_MODE_SENSING,
  BARGRAPH_MODE_MANUAL,
  BARGRAPH_MODE_TEST
};

struct bargraph_state_s
{
  pthread_mutex_t      lock;
  pthread_cond_t       wake_cond;
  pthread_cond_t       done_cond;
  enum bargraph_mode_e mode;
  enum bargraph_mode_e prev_mode;
  bool                 running;
  bool                 stop_requested;
  bool                 test_requested;
  int                  init_err;
  uint8_t              manual_pct;
  uint16_t             raw_adc;
  uint16_t             filtered_adc;
  uint16_t             millivolts;
  uint16_t             permille;
  uint16_t             duty[BARGRAPH_NUM_LEDS];
  uint32_t             test_peaks_seen;
  uint32_t             tick_seq;
};

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
static pthread_t               g_daemon_thread;
static bool                    g_daemon_joinable = false;

static struct bargraph_state_s g_bargraph =
{
  .lock            = PTHREAD_MUTEX_INITIALIZER,
  .wake_cond       = PTHREAD_COND_INITIALIZER,
  .done_cond       = PTHREAD_COND_INITIALIZER,
  .mode            = BARGRAPH_MODE_STOPPED,
  .prev_mode       = BARGRAPH_MODE_SENSING,
  .running         = false,
  .stop_requested  = false,
  .test_requested  = false,
  .init_err        = 0,
  .manual_pct      = 0u,
  .raw_adc         = 0u,
  .filtered_adc    = 0u,
  .millivolts      = 0u,
  .permille        = 0u,
  .duty            = { 0u, 0u, 0u, 0u, 0u, 0u },
  .test_peaks_seen = 0u,
  .tick_seq        = 0u
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

static const char *bargraph_mode_name(enum bargraph_mode_e mode)
{
  switch (mode)
    {
      case BARGRAPH_MODE_SENSING:
        return "SENSING";
      case BARGRAPH_MODE_MANUAL:
        return "MANUAL";
      case BARGRAPH_MODE_TEST:
        return "TEST";
      case BARGRAPH_MODE_STOPPED:
      default:
        return "STOPPED";
    }
}

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

static void *bargraphd_thread_fn(void *arg)
{
  int adc_fd = 0;
  int pwm_fds[BARGRAPH_NUM_PWM_SLICES];
  bool pwm_started[BARGRAPH_NUM_PWM_SLICES];
  uint32_t ema_q4 = 0u;
  bool ema_initialized = false;
  uint32_t test_step = 0u;
  int i;

  (void)arg;

  up_disable_irq(RP23XX_ADC_IRQ_FIFO);

  for (i = 0; i < BARGRAPH_NUM_PWM_SLICES; i++)
    {
      g_mock_slices[i].open = true;
      pwm_fds[i]            = i;
      pwm_started[i]        = false;
    }

  pthread_mutex_lock(&g_bargraph.lock);
  g_bargraph.init_err       = 0;
  g_bargraph.running        = true;
  g_bargraph.stop_requested = false;
  if (g_bargraph.mode == BARGRAPH_MODE_STOPPED)
    {
      g_bargraph.mode      = BARGRAPH_MODE_SENSING;
      g_bargraph.prev_mode = BARGRAPH_MODE_SENSING;
    }

  pthread_cond_broadcast(&g_bargraph.done_cond);

  while (!g_bargraph.stop_requested)
    {
      uint32_t total_units = 0u;
      uint16_t duties[BARGRAPH_NUM_LEDS];
      uint16_t raw_adc = g_bargraph.raw_adc;
      uint16_t filtered_adc = g_bargraph.filtered_adc;

      if (g_bargraph.test_requested &&
          g_bargraph.mode != BARGRAPH_MODE_TEST)
        {
          g_bargraph.mode = BARGRAPH_MODE_TEST;
          test_step       = 0u;
        }

      if (g_bargraph.mode == BARGRAPH_MODE_SENSING)
        {
          pthread_mutex_unlock(&g_bargraph.lock);
          (void)bargraph_sample_adc(adc_fd, &ema_q4, &ema_initialized,
                                    &raw_adc, &filtered_adc);
          pthread_mutex_lock(&g_bargraph.lock);

          if (g_bargraph.stop_requested)
            {
              break;
            }

          if (g_bargraph.mode == BARGRAPH_MODE_SENSING)
            {
              g_bargraph.raw_adc      = raw_adc;
              g_bargraph.filtered_adc = filtered_adc;
              g_bargraph.millivolts   =
                (uint16_t)(((uint32_t)filtered_adc * 3300u + 2047u) /
                           BARGRAPH_ADC_MAX);
              g_bargraph.permille     =
                (uint16_t)(((uint32_t)filtered_adc * 1000u + 2047u) /
                           BARGRAPH_ADC_MAX);
              total_units             =
                ((uint32_t)filtered_adc * BARGRAPH_TOTAL_UNITS_MAX + 2047u) /
                BARGRAPH_ADC_MAX;
            }
        }

      if (g_bargraph.mode == BARGRAPH_MODE_MANUAL)
        {
          uint32_t pct = g_bargraph.manual_pct;
          if (pct > 100u)
            {
              pct = 100u;
            }

          g_bargraph.millivolts = (uint16_t)((pct * 3300u + 50u) / 100u);
          g_bargraph.permille   = (uint16_t)(pct * 10u);
          total_units           =
            (pct * BARGRAPH_TOTAL_UNITS_MAX + 50u) / 100u;
        }
      else if (g_bargraph.mode == BARGRAPH_MODE_TEST)
        {
          if (!g_bargraph.test_requested)
            {
              g_bargraph.mode = g_bargraph.prev_mode;
              pthread_cond_broadcast(&g_bargraph.done_cond);
              continue;
            }

          uint32_t level_step = (test_step <= BARGRAPH_TEST_HALF_STEPS)
                                ? test_step
                                : (2u * BARGRAPH_TEST_HALF_STEPS - test_step);

          if (level_step == BARGRAPH_TEST_HALF_STEPS)
            {
              g_bargraph.test_peaks_seen++;
            }

          total_units =
            (level_step * BARGRAPH_TOTAL_UNITS_MAX +
             (BARGRAPH_TEST_HALF_STEPS / 2u)) /
            BARGRAPH_TEST_HALF_STEPS;
          g_bargraph.permille   =
            (uint16_t)((level_step * 1000u) / BARGRAPH_TEST_HALF_STEPS);
          g_bargraph.millivolts =
            (uint16_t)((level_step * 3300u) / BARGRAPH_TEST_HALF_STEPS);
        }

      bargraph_compute_duties(total_units, duties);
      memcpy(g_bargraph.duty, duties, sizeof(duties));

      pthread_mutex_unlock(&g_bargraph.lock);
      (void)bargraph_apply_pwm(pwm_fds, pwm_started, duties);
      pthread_mutex_lock(&g_bargraph.lock);

      if (g_bargraph.stop_requested)
        {
          break;
        }

      if (g_bargraph.mode == BARGRAPH_MODE_TEST)
        {
          test_step++;
          if (test_step > 2u * BARGRAPH_TEST_HALF_STEPS)
            {
              g_bargraph.test_requested = false;
              g_bargraph.mode           = g_bargraph.prev_mode;
              pthread_cond_broadcast(&g_bargraph.done_cond);
              continue;
            }

          continue;
        }

      g_bargraph.tick_seq++;
      pthread_cond_broadcast(&g_bargraph.done_cond);
      pthread_cond_wait(&g_bargraph.wake_cond, &g_bargraph.lock);
    }

  pthread_mutex_unlock(&g_bargraph.lock);

  bargraph_shutdown_pwm(pwm_fds, pwm_started);
  up_enable_irq(RP23XX_ADC_IRQ_FIFO);

  pthread_mutex_lock(&g_bargraph.lock);
  memset(g_bargraph.duty, 0, sizeof(g_bargraph.duty));
  g_bargraph.running        = false;
  g_bargraph.stop_requested = false;
  g_bargraph.test_requested = false;
  g_bargraph.mode           = BARGRAPH_MODE_STOPPED;
  pthread_cond_broadcast(&g_bargraph.done_cond);
  pthread_mutex_unlock(&g_bargraph.lock);

  return NULL;
}

static void bargraph_wait_next_tick_locked(uint32_t prev_seq)
{
  while (g_bargraph.running && g_bargraph.tick_seq == prev_seq)
    {
      pthread_cond_wait(&g_bargraph.done_cond, &g_bargraph.lock);
    }
}

static int bargraph_start_daemon(void)
{
  int err;
  uint32_t seq;

  pthread_mutex_lock(&g_bargraph.lock);
  if (g_bargraph.running)
    {
      if (g_bargraph.mode == BARGRAPH_MODE_MANUAL ||
          g_bargraph.mode == BARGRAPH_MODE_TEST)
        {
          seq                       = g_bargraph.tick_seq;
          g_bargraph.test_requested = false;
          g_bargraph.mode           = BARGRAPH_MODE_SENSING;
          g_bargraph.prev_mode      = BARGRAPH_MODE_SENSING;
          pthread_cond_broadcast(&g_bargraph.wake_cond);
          bargraph_wait_next_tick_locked(seq);
        }

      pthread_mutex_unlock(&g_bargraph.lock);
      return 0;
    }

  seq                       = g_bargraph.tick_seq;
  g_bargraph.init_err       = 1;
  g_bargraph.stop_requested = false;
  g_bargraph.test_requested = false;

  if (pthread_create(&g_daemon_thread, NULL, bargraphd_thread_fn, NULL) != 0)
    {
      err = -errno;
      g_bargraph.init_err = err;
      pthread_mutex_unlock(&g_bargraph.lock);
      return err;
    }

  g_daemon_joinable = true;

  while (!g_bargraph.running && g_bargraph.init_err > 0)
    {
      pthread_cond_wait(&g_bargraph.done_cond, &g_bargraph.lock);
    }

  bargraph_wait_next_tick_locked(seq);
  err = g_bargraph.init_err;
  pthread_mutex_unlock(&g_bargraph.lock);
  return err;
}

static int bargraph_cli(int argc, const char * const argv[])
{
  if (argc < 2)
    {
      return EXIT_FAILURE;
    }

  if (strcmp(argv[1], "start") == 0 || strcmp(argv[1], "resume") == 0)
    {
      return bargraph_start_daemon() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }
  else if (strcmp(argv[1], "stop") == 0)
    {
      pthread_mutex_lock(&g_bargraph.lock);
      if (!g_bargraph.running)
        {
          pthread_mutex_unlock(&g_bargraph.lock);
          return EXIT_SUCCESS;
        }

      g_bargraph.stop_requested = true;
      g_bargraph.test_requested = false;
      pthread_cond_broadcast(&g_bargraph.wake_cond);

      while (g_bargraph.running)
        {
          pthread_cond_wait(&g_bargraph.done_cond, &g_bargraph.lock);
        }

      pthread_mutex_unlock(&g_bargraph.lock);
      if (g_daemon_joinable)
        {
          (void)pthread_join(g_daemon_thread, NULL);
          g_daemon_joinable = false;
        }

      return EXIT_SUCCESS;
    }
  else if (strcmp(argv[1], "set") == 0)
    {
      char *endptr = NULL;
      long val;

      if (argc < 3)
        {
          return EXIT_FAILURE;
        }

      errno = 0;
      val = strtol(argv[2], &endptr, 10);
      if (errno != 0 || endptr == argv[2] || *endptr != '\0' ||
          val < 0 || val > 100)
        {
          return EXIT_FAILURE;
        }

      if (bargraph_start_daemon() < 0)
        {
          return EXIT_FAILURE;
        }

      pthread_mutex_lock(&g_bargraph.lock);
      {
        uint32_t seq = g_bargraph.tick_seq;
        g_bargraph.manual_pct     = (uint8_t)val;
        g_bargraph.test_requested = false;
        g_bargraph.mode           = BARGRAPH_MODE_MANUAL;
        g_bargraph.prev_mode      = BARGRAPH_MODE_MANUAL;
        pthread_cond_broadcast(&g_bargraph.wake_cond);
        bargraph_wait_next_tick_locked(seq);
      }
      pthread_mutex_unlock(&g_bargraph.lock);
      return EXIT_SUCCESS;
    }
  else if (strcmp(argv[1], "test") == 0)
    {
      if (bargraph_start_daemon() < 0)
        {
          return EXIT_FAILURE;
        }

      pthread_mutex_lock(&g_bargraph.lock);
      if (g_bargraph.mode != BARGRAPH_MODE_TEST)
        {
          g_bargraph.prev_mode = g_bargraph.mode;
        }

      {
        uint32_t seq = g_bargraph.tick_seq;
        g_bargraph.test_requested = true;
        pthread_cond_broadcast(&g_bargraph.wake_cond);

        while (g_bargraph.running && g_bargraph.test_requested)
          {
            pthread_cond_wait(&g_bargraph.done_cond, &g_bargraph.lock);
          }

        bargraph_wait_next_tick_locked(seq);
      }
      pthread_mutex_unlock(&g_bargraph.lock);
      return EXIT_SUCCESS;
    }

  return EXIT_FAILURE;
}

int main(void)
{
  uint32_t pad_reg = RP23XX_PADS_BANK0_GPIO_IE;
  const char *cmd_start[]  = { "bargraph", "start" };
  const char *cmd_set50[]  = { "bargraph", "set", "50" };
  const char *cmd_bad[]    = { "bargraph", "set", "105" };
  const char *cmd_test[]   = { "bargraph", "test" };
  const char *cmd_resume[] = { "bargraph", "resume" };
  const char *cmd_stop[]   = { "bargraph", "stop" };
  unsigned int i;

  assert(g_led_map[0].gpio == 27u);
  assert(bargraph_total_led_current_ua(BARGRAPH_NUM_LEDS) == 23634u);
  bargraph_isolate_adc_pad(&pad_reg);
  assert((pad_reg & RP23XX_PADS_BANK0_GPIO_OD) != 0u);

  memset(&g_mock_adc, 0, sizeof(g_mock_adc));
  for (i = 0u; i < BARGRAPH_ADC_READ_SAMPLES; i++)
    {
      g_mock_adc.samples[i].am_data = (int32_t)(4095 << BARGRAPH_ADC_SHIFT);
    }

  assert(bargraph_cli(2, cmd_start) == EXIT_SUCCESS);
  assert(strcmp(bargraph_mode_name(g_bargraph.mode), "SENSING") == 0);
  assert(g_bargraph.permille == 1000u);
  for (i = 0u; i < BARGRAPH_NUM_LEDS; i++)
    {
      assert(g_bargraph.duty[i] == 65535u);
    }

  assert(bargraph_cli(3, cmd_bad) == EXIT_FAILURE);
  assert(bargraph_cli(3, cmd_set50) == EXIT_SUCCESS);
  assert(strcmp(bargraph_mode_name(g_bargraph.mode), "MANUAL") == 0);
  assert(g_bargraph.permille == 500u);
  assert(g_bargraph.duty[0] == 65535u && g_bargraph.duty[1] == 65535u);
  assert(g_bargraph.duty[2] == 65535u && g_bargraph.duty[3] == 0u);

  assert(bargraph_cli(2, cmd_resume) == EXIT_SUCCESS);
  assert(strcmp(bargraph_mode_name(g_bargraph.mode), "SENSING") == 0);

  assert(bargraph_cli(2, cmd_test) == EXIT_SUCCESS);
  assert(g_bargraph.test_peaks_seen == 1u);
  assert(strcmp(bargraph_mode_name(g_bargraph.mode), "SENSING") == 0);

  assert(bargraph_cli(2, cmd_stop) == EXIT_SUCCESS);
  assert(strcmp(bargraph_mode_name(g_bargraph.mode), "STOPPED") == 0);
  assert(!g_bargraph.running);

  printf("Lesson 0006 verification passed: state=%s, test_peaks=%u\n",
         bargraph_mode_name(g_bargraph.mode),
         (unsigned int)g_bargraph.test_peaks_seen);
  return 0;
}
