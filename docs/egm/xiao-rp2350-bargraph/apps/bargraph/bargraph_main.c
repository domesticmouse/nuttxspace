/****************************************************************************
 * xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <nuttx/analog/adc.h>
#include <nuttx/analog/ioctl.h>
#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/sched.h>
#include <nuttx/timers/pwm.h>

#include "bargraph_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define BARGRAPH_ADC_DEVPATH "/dev/adc0"

/* Slice indices inside pwm_fds[] / g_pwm_devpaths[]:
 *   0 -> /dev/pwm0 (Ch A = LED6 / D6 / GPIO0)
 *   1 -> /dev/pwm2 (Ch B = LED3 / D3 / GPIO5)
 *   2 -> /dev/pwm3 (Ch A = LED4 / D4 / GPIO6, Ch B = LED5 / D5 / GPIO7)
 *   3 -> /dev/pwm5 (Ch B = LED1 / D1 / GPIO27)
 *   4 -> /dev/pwm6 (Ch A = LED2 / D2 / GPIO28)
 */

#define PWM_IDX_SLICE0       0
#define PWM_IDX_SLICE2       1
#define PWM_IDX_SLICE3       2
#define PWM_IDX_SLICE5       3
#define PWM_IDX_SLICE6       4

/****************************************************************************
 * Private Data
 ****************************************************************************/

static FAR const char * const g_pwm_devpaths[BARGRAPH_NUM_PWM_SLICES] =
{
  "/dev/pwm0",
  "/dev/pwm2",
  "/dev/pwm3",
  "/dev/pwm5",
  "/dev/pwm6"
};

static struct bargraph_state_s g_bargraph =
{
  .lock           = PTHREAD_MUTEX_INITIALIZER,
  .wake_cond      = PTHREAD_COND_INITIALIZER,
  .done_cond      = PTHREAD_COND_INITIALIZER,
  .mode           = BARGRAPH_MODE_STOPPED,
  .prev_mode      = BARGRAPH_MODE_SENSING,
  .running        = false,
  .stop_requested = false,
  .test_requested = false,
  .init_err       = 0,
  .manual_pct     = 0,
  .raw_adc        = 0,
  .filtered_adc   = 0,
  .millivolts     = 0,
  .permille       = 0,
  .duty           = { 0, 0, 0, 0, 0, 0 },
  .daemon_pid     = -1
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: bargraph_mode_name
 ****************************************************************************/

static FAR const char *bargraph_mode_name(enum bargraph_mode_e mode)
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

/****************************************************************************
 * Name: bargraph_update_slice
 ****************************************************************************/

static int bargraph_update_slice(int fd, FAR bool *started,
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

  ret = ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)&info);
  if (ret < 0)
    {
      return -errno;
    }

  if (!(*started))
    {
      ret = ioctl(fd, PWMIOC_START, 0);
      if (ret < 0)
        {
          return -errno;
        }

      *started = true;
    }

  return 0;
}

/****************************************************************************
 * Name: bargraph_apply_pwm
 ****************************************************************************/

static int bargraph_apply_pwm(FAR const int pwm_fds[BARGRAPH_NUM_PWM_SLICES],
                              FAR bool pwm_started[BARGRAPH_NUM_PWM_SLICES],
                              FAR const uint16_t duties[BARGRAPH_NUM_LEDS])
{
  int ret;

  /* LED1 -> /dev/pwm5 Ch B (index 3) */

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE5],
                              &pwm_started[PWM_IDX_SLICE5],
                              0, duties[0]);
  if (ret < 0)
    {
      return ret;
    }

  /* LED2 -> /dev/pwm6 Ch A (index 4) */

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE6],
                              &pwm_started[PWM_IDX_SLICE6],
                              duties[1], 0);
  if (ret < 0)
    {
      return ret;
    }

  /* LED3 -> /dev/pwm2 Ch B (index 1) */

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE2],
                              &pwm_started[PWM_IDX_SLICE2],
                              0, duties[2]);
  if (ret < 0)
    {
      return ret;
    }

  /* LED4 -> /dev/pwm3 Ch A, LED5 -> /dev/pwm3 Ch B (index 2) */

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE3],
                              &pwm_started[PWM_IDX_SLICE3],
                              duties[3], duties[4]);
  if (ret < 0)
    {
      return ret;
    }

  /* LED6 -> /dev/pwm0 Ch A (index 0) */

  ret = bargraph_update_slice(pwm_fds[PWM_IDX_SLICE0],
                              &pwm_started[PWM_IDX_SLICE0],
                              duties[5], 0);
  return ret;
}

/****************************************************************************
 * Name: bargraph_shutdown_pwm
 ****************************************************************************/

static void bargraph_shutdown_pwm(FAR int pwm_fds[BARGRAPH_NUM_PWM_SLICES],
                                  FAR bool pwm_started[BARGRAPH_NUM_PWM_SLICES])
{
  uint16_t zero_duties[BARGRAPH_NUM_LEDS];
  int i;

  memset(zero_duties, 0, sizeof(zero_duties));

  for (i = 0; i < BARGRAPH_NUM_PWM_SLICES; i++)
    {
      if (pwm_fds[i] >= 0)
        {
          (void)bargraph_update_slice(pwm_fds[i], &pwm_started[i], 0, 0);
          (void)ioctl(pwm_fds[i], PWMIOC_STOP, 0);
          (void)close(pwm_fds[i]);
          pwm_fds[i]     = -1;
          pwm_started[i] = false;
        }
    }
}

/****************************************************************************
 * Name: bargraph_sample_adc
 ****************************************************************************/

static int bargraph_sample_adc(int adc_fd,
                               FAR uint32_t *ema_q4,
                               FAR bool *ema_initialized,
                               FAR uint16_t *raw_out,
                               FAR uint16_t *filtered_out)
{
  struct adc_msg_s msgs[BARGRAPH_ADC_READ_SAMPLES];
  FAR uint8_t *buf = (FAR uint8_t *)msgs;
  size_t bytes_needed = sizeof(msgs);
  size_t total_read = 0;
  uint32_t sum = 0;
  unsigned int valid_count = 0;
  unsigned int i;

  (void)ioctl(adc_fd, ANIOC_RESET_FIFO, 0);

  up_enable_irq(RP23XX_ADC_IRQ_FIFO);

  while (total_read < bytes_needed)
    {
      ssize_t nread = read(adc_fd, buf + total_read,
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

  /* Discard msgs[0] (latched at the end of the previous burst) and average
   * valid fresh samples in msgs[1..BARGRAPH_ADC_READ_SAMPLES - 1].
   */

  for (i = 1; i < BARGRAPH_ADC_READ_SAMPLES; i++)
    {
      if (msgs[i].am_data >= 0)
        {
          uint16_t sample12 =
            (uint16_t)((msgs[i].am_data >> BARGRAPH_ADC_SHIFT) & 0x0fff);
          sum += sample12;
          valid_count++;
        }
    }

  if (valid_count > 0)
    {
      uint16_t raw_avg = (uint16_t)((sum + (valid_count / 2u)) / valid_count);
      *raw_out      = raw_avg;
      *filtered_out = bargraph_filter_adc(raw_avg, ema_q4, ema_initialized);
    }

  return 0;
}

/****************************************************************************
 * Name: bargraph_add_tick
 ****************************************************************************/

static void bargraph_add_tick(FAR struct timespec *ts)
{
  ts->tv_nsec += BARGRAPH_TICK_NSEC;
  if (ts->tv_nsec >= 1000000000L)
    {
      ts->tv_sec  += 1;
      ts->tv_nsec -= 1000000000L;
    }
}

/****************************************************************************
 * Name: bargraphd_daemon
 ****************************************************************************/

static int bargraphd_daemon(int argc, FAR char *argv[])
{
  int adc_fd = -1;
  int pwm_fds[BARGRAPH_NUM_PWM_SLICES];
  bool pwm_started[BARGRAPH_NUM_PWM_SLICES];
  uint32_t ema_q4 = 0;
  bool ema_initialized = false;
  uint32_t test_step = 0;
  int i;

  (void)argc;
  (void)argv;

  for (i = 0; i < BARGRAPH_NUM_PWM_SLICES; i++)
    {
      pwm_fds[i]     = -1;
      pwm_started[i] = false;
    }

  adc_fd = open(BARGRAPH_ADC_DEVPATH, O_RDONLY);
  if (adc_fd < 0)
    {
      int err = -errno;
      pthread_mutex_lock(&g_bargraph.lock);
      g_bargraph.init_err   = err;
      g_bargraph.running    = false;
      g_bargraph.mode       = BARGRAPH_MODE_STOPPED;
      g_bargraph.daemon_pid = -1;
      pthread_cond_broadcast(&g_bargraph.done_cond);
      pthread_mutex_unlock(&g_bargraph.lock);
      return EXIT_FAILURE;
    }

  /* Immediately mask RP23XX_ADC_IRQ_FIFO so the self-retriggering
   * START_ONCE handler in rp23xx_adc.c does not fire at 500 kIRQ/s.
   */

  up_disable_irq(RP23XX_ADC_IRQ_FIFO);

  for (i = 0; i < BARGRAPH_NUM_PWM_SLICES; i++)
    {
      pwm_fds[i] = open(g_pwm_devpaths[i], O_RDONLY);
      if (pwm_fds[i] < 0)
        {
          int err = -errno;
          bargraph_shutdown_pwm(pwm_fds, pwm_started);
          up_enable_irq(RP23XX_ADC_IRQ_FIFO);
          (void)close(adc_fd);

          pthread_mutex_lock(&g_bargraph.lock);
          g_bargraph.init_err   = err;
          g_bargraph.running    = false;
          g_bargraph.mode       = BARGRAPH_MODE_STOPPED;
          g_bargraph.daemon_pid = -1;
          pthread_cond_broadcast(&g_bargraph.done_cond);
          pthread_mutex_unlock(&g_bargraph.lock);
          return EXIT_FAILURE;
        }
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

  g_bargraph.daemon_pid = getpid();
  pthread_cond_broadcast(&g_bargraph.done_cond);

  while (!g_bargraph.stop_requested)
    {
      struct timespec ts;
      uint32_t total_units = 0;
      uint16_t duties[BARGRAPH_NUM_LEDS];
      uint16_t raw_adc = g_bargraph.raw_adc;
      uint16_t filtered_adc = g_bargraph.filtered_adc;

      if (g_bargraph.test_requested &&
          g_bargraph.mode != BARGRAPH_MODE_TEST)
        {
          g_bargraph.mode = BARGRAPH_MODE_TEST;
          test_step       = 0;
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
        }

      (void)clock_gettime(CLOCK_REALTIME, &ts);
      bargraph_add_tick(&ts);
      (void)pthread_cond_timedwait(&g_bargraph.wake_cond,
                                   &g_bargraph.lock, &ts);
    }

  pthread_mutex_unlock(&g_bargraph.lock);

  bargraph_shutdown_pwm(pwm_fds, pwm_started);

  up_enable_irq(RP23XX_ADC_IRQ_FIFO);
  (void)close(adc_fd);

  pthread_mutex_lock(&g_bargraph.lock);
  memset(g_bargraph.duty, 0, sizeof(g_bargraph.duty));
  g_bargraph.running        = false;
  g_bargraph.stop_requested = false;
  g_bargraph.test_requested = false;
  g_bargraph.mode           = BARGRAPH_MODE_STOPPED;
  g_bargraph.daemon_pid     = -1;
  pthread_cond_broadcast(&g_bargraph.done_cond);
  pthread_mutex_unlock(&g_bargraph.lock);

  return EXIT_SUCCESS;
}

/****************************************************************************
 * Name: bargraph_start_daemon
 ****************************************************************************/

static int bargraph_start_daemon(void)
{
  int pid;
  int err;

  pthread_mutex_lock(&g_bargraph.lock);
  if (g_bargraph.running)
    {
      if (g_bargraph.mode == BARGRAPH_MODE_MANUAL ||
          g_bargraph.mode == BARGRAPH_MODE_TEST)
        {
          g_bargraph.test_requested = false;
          g_bargraph.mode           = BARGRAPH_MODE_SENSING;
          g_bargraph.prev_mode      = BARGRAPH_MODE_SENSING;
          pthread_cond_broadcast(&g_bargraph.done_cond);
          pthread_cond_broadcast(&g_bargraph.wake_cond);
        }

      pthread_mutex_unlock(&g_bargraph.lock);
      return 0;
    }

  g_bargraph.init_err       = 1; /* Pending initialization */
  g_bargraph.stop_requested = false;
  g_bargraph.test_requested = false;

  pid = task_create("bargraphd",
                    CONFIG_BARGRAPH_PRIORITY,
                    CONFIG_BARGRAPH_STACKSIZE,
                    bargraphd_daemon,
                    NULL);
  if (pid < 0)
    {
      err = -errno;
      g_bargraph.init_err = err;
      pthread_mutex_unlock(&g_bargraph.lock);
      return err;
    }

  while (!g_bargraph.running && g_bargraph.init_err > 0)
    {
      pthread_cond_wait(&g_bargraph.done_cond, &g_bargraph.lock);
    }

  err = g_bargraph.init_err;
  pthread_mutex_unlock(&g_bargraph.lock);
  return err;
}

/****************************************************************************
 * Name: bargraph_print_usage
 ****************************************************************************/

static void bargraph_print_usage(FAR const char *progname)
{
  printf("Usage: %s <start|stop|status|test|set <0..100>|resume>\n",
         progname);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: bargraph_compute_duties
 ****************************************************************************/

void bargraph_compute_duties(uint32_t total_units,
                             FAR uint16_t duties[BARGRAPH_NUM_LEDS])
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
          duties[i] = 0;
        }
    }
}

/****************************************************************************
 * Name: bargraph_filter_adc
 ****************************************************************************/

uint16_t bargraph_filter_adc(uint16_t raw_avg,
                             FAR uint32_t *ema_q4,
                             FAR bool *ema_initialized)
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
      filtered = 0;
    }
  else if (filtered >= BARGRAPH_ADC_DEADBAND_HIGH)
    {
      filtered = BARGRAPH_ADC_MAX;
    }

  return filtered;
}

/****************************************************************************
 * Name: bargraph_autostart
 ****************************************************************************/

int bargraph_autostart(void)
{
  return bargraph_start_daemon();
}

/****************************************************************************
 * Name: main (bargraph_main)
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  if (argc < 2)
    {
      bargraph_print_usage(argv[0]);
      return EXIT_FAILURE;
    }

  if (strcmp(argv[1], "start") == 0)
    {
      int ret = bargraph_start_daemon();
      if (ret < 0)
        {
          fprintf(stderr, "bargraph: start failed (%d)\n", ret);
          return EXIT_FAILURE;
        }

      printf("bargraph: running in SENSING mode\n");
      return EXIT_SUCCESS;
    }
  else if (strcmp(argv[1], "stop") == 0)
    {
      pthread_mutex_lock(&g_bargraph.lock);
      if (!g_bargraph.running)
        {
          pthread_mutex_unlock(&g_bargraph.lock);
          printf("bargraph: already stopped\n");
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
      printf("bargraph: stopped\n");
      return EXIT_SUCCESS;
    }
  else if (strcmp(argv[1], "status") == 0)
    {
      struct bargraph_state_s snap;
      unsigned int i;

      pthread_mutex_lock(&g_bargraph.lock);
      snap = g_bargraph;
      pthread_mutex_unlock(&g_bargraph.lock);

      printf("state:        %s\n", bargraph_mode_name(snap.mode));
      printf("raw_adc:      %u\n", (unsigned int)snap.raw_adc);
      printf("filtered_adc: %u\n", (unsigned int)snap.filtered_adc);
      printf("voltage_mv:   %u mV\n", (unsigned int)snap.millivolts);
      printf("level_pct:    %u.%u%%\n",
             (unsigned int)(snap.permille / 10u),
             (unsigned int)(snap.permille % 10u));
      printf("led_duty:    ");
      for (i = 0; i < BARGRAPH_NUM_LEDS; i++)
        {
          unsigned int pct =
            ((unsigned int)snap.duty[i] * 100u + (BARGRAPH_DUTY_MAX / 2u)) /
            BARGRAPH_DUTY_MAX;
          printf(" LED%u=%u (%u%%)",
                 i + 1u, (unsigned int)snap.duty[i], pct);
        }

      printf("\n");
      return EXIT_SUCCESS;
    }
  else if (strcmp(argv[1], "set") == 0)
    {
      FAR char *endptr = NULL;
      long val;
      int ret;

      if (argc < 3)
        {
          bargraph_print_usage(argv[0]);
          return EXIT_FAILURE;
        }

      errno = 0;
      val = strtol(argv[2], &endptr, 10);
      if (errno != 0 || endptr == argv[2] || *endptr != '\0' ||
          val < 0 || val > 100)
        {
          fprintf(stderr,
                  "bargraph: invalid percentage '%s' (expected 0..100)\n",
                  argv[2]);
          return EXIT_FAILURE;
        }

      ret = bargraph_start_daemon();
      if (ret < 0)
        {
          fprintf(stderr, "bargraph: start failed (%d)\n", ret);
          return EXIT_FAILURE;
        }

      pthread_mutex_lock(&g_bargraph.lock);
      g_bargraph.test_requested = false;
      g_bargraph.manual_pct     = (uint8_t)val;
      g_bargraph.mode           = BARGRAPH_MODE_MANUAL;
      g_bargraph.prev_mode      = BARGRAPH_MODE_MANUAL;
      pthread_cond_broadcast(&g_bargraph.done_cond);
      pthread_cond_broadcast(&g_bargraph.wake_cond);
      pthread_mutex_unlock(&g_bargraph.lock);

      printf("bargraph: manual override set to %ld%%\n", val);
      return EXIT_SUCCESS;
    }
  else if (strcmp(argv[1], "resume") == 0)
    {
      pthread_mutex_lock(&g_bargraph.lock);
      if (!g_bargraph.running || g_bargraph.mode == BARGRAPH_MODE_STOPPED)
        {
          pthread_mutex_unlock(&g_bargraph.lock);
          fprintf(stderr,
                  "bargraph: daemon is stopped (use 'bargraph start')\n");
          return EXIT_FAILURE;
        }

      if (g_bargraph.mode == BARGRAPH_MODE_SENSING &&
          !g_bargraph.test_requested)
        {
          pthread_mutex_unlock(&g_bargraph.lock);
          printf("bargraph: already in SENSING mode\n");
          return EXIT_SUCCESS;
        }

      g_bargraph.test_requested = false;
      g_bargraph.mode           = BARGRAPH_MODE_SENSING;
      g_bargraph.prev_mode      = BARGRAPH_MODE_SENSING;
      pthread_cond_broadcast(&g_bargraph.done_cond);
      pthread_cond_broadcast(&g_bargraph.wake_cond);
      pthread_mutex_unlock(&g_bargraph.lock);

      printf("bargraph: resumed SENSING mode\n");
      return EXIT_SUCCESS;
    }
  else if (strcmp(argv[1], "test") == 0)
    {
      int ret = bargraph_start_daemon();
      if (ret < 0)
        {
          fprintf(stderr, "bargraph: start failed (%d)\n", ret);
          return EXIT_FAILURE;
        }

      pthread_mutex_lock(&g_bargraph.lock);
      if (g_bargraph.mode != BARGRAPH_MODE_TEST)
        {
          g_bargraph.prev_mode = g_bargraph.mode;
        }

      g_bargraph.test_requested = true;
      pthread_cond_broadcast(&g_bargraph.wake_cond);

      while (g_bargraph.running && g_bargraph.test_requested &&
             !g_bargraph.stop_requested)
        {
          pthread_cond_wait(&g_bargraph.done_cond, &g_bargraph.lock);
        }

      pthread_mutex_unlock(&g_bargraph.lock);
      printf("bargraph: test sweep complete\n");
      return EXIT_SUCCESS;
    }

  bargraph_print_usage(argv[0]);
  return EXIT_FAILURE;
}
