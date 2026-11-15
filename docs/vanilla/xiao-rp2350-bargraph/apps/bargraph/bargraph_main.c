/****************************************************************************
 * xiao-rp2350-bargraph/apps/bargraph/bargraph_main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Potentiometer ADC to 6-LED Bar-Graph application and background daemon
 * for the Seeed Studio XIAO RP2350 on Apache NuttX.
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sched.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <syslog.h>
#include <unistd.h>

#include <nuttx/analog/adc.h>
#include <nuttx/analog/ioctl.h>
#include <nuttx/leds/userled.h>

#include "bargraph_logic.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_BARGRAPH_ADC_DEVPATH
#  define CONFIG_BARGRAPH_ADC_DEVPATH "/dev/adc0"
#endif

#ifndef CONFIG_BARGRAPH_LED_DEVPATH
#  define CONFIG_BARGRAPH_LED_DEVPATH "/dev/userleds"
#endif

#ifndef CONFIG_BARGRAPH_POLL_INTERVAL_MS
#  define CONFIG_BARGRAPH_POLL_INTERVAL_MS 25
#endif

#ifndef CONFIG_BARGRAPH_APP_PRIORITY
#  define CONFIG_BARGRAPH_APP_PRIORITY 100
#endif

#ifndef CONFIG_BARGRAPH_APP_STACKSIZE
#  define CONFIG_BARGRAPH_APP_STACKSIZE 2048
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static volatile bool g_daemon_running = false;
static volatile bool g_daemon_stop    = false;
static volatile pid_t g_daemon_pid    = -1;
static struct bargraph_state_s g_bargraph_state;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: bargraph_read_adc_once
 *
 * Description:
 *   Open /dev/adc0, read one ADC sample message, and close /dev/adc0 so
 *   that rp23xx_adc.c disables its continuous conversion IRQ between reads.
 ****************************************************************************/

static int bargraph_read_adc_once(uint16_t *raw12_out)
{
  struct adc_msg_s msg;
  ssize_t nbytes;
  int fd;
  int ret;

  fd = open(CONFIG_BARGRAPH_ADC_DEVPATH, O_RDONLY);
  if (fd < 0)
    {
      return -errno;
    }

  nbytes = read(fd, &msg, sizeof(msg));
  close(fd);

  if (nbytes != (ssize_t)sizeof(msg))
    {
      return (nbytes < 0) ? -errno : -EIO;
    }

  ret = bargraph_decode_rp23xx_sample(msg.am_data, raw12_out);
  return ret;
}

/****************************************************************************
 * Name: bargraph_write_leds
 *
 * Description:
 *   Write the 6-bit LED bar-graph bitmask to /dev/userleds.
 ****************************************************************************/

static int bargraph_write_leds(int led_fd, uint32_t mask)
{
  userled_set_t ledset = (userled_set_t)(mask & BARGRAPH_ALL_LEDS_MASK);
  int ret;

  ret = ioctl(led_fd, ULEDIOC_SETALL, (unsigned long)ledset);
  if (ret < 0)
    {
      return -errno;
    }

  return 0;
}

/****************************************************************************
 * Name: bargraph_print_status
 ****************************************************************************/

static void bargraph_print_status(const struct bargraph_state_s *st,
                                  bool running,
                                  pid_t pid)
{
  char ascii[BARGRAPH_NUM_LEDS + 4U];
  uint32_t mv  = bargraph_raw12_to_millivolts(st->raw_filtered,
                                              BARGRAPH_DEFAULT_VREF_MV);
  uint8_t  pct = bargraph_raw12_to_percent(st->raw_filtered);

  bargraph_format_ascii(st->level, ascii, sizeof(ascii));

  printf("bargraph: daemon=%s (pid=%d) samples=%" PRIu32 "\n",
         running ? "running" : "stopped",
         (int)pid,
         st->sample_count);
  printf("  adc: raw=%u filtered=%u (%u mV, %u%%)\n",
         (unsigned int)st->raw_last,
         (unsigned int)st->raw_filtered,
         (unsigned int)mv,
         (unsigned int)pct);
  printf("  leds: %s (%u/%u, mask=0x%02" PRIx32 ")\n",
         ascii,
         (unsigned int)st->level,
         (unsigned int)BARGRAPH_NUM_LEDS,
         st->led_mask);
}

/****************************************************************************
 * Name: bargraph_daemon
 ****************************************************************************/

static int bargraph_daemon(int argc, FAR char *argv[])
{
  uint32_t prev_mask = 0xffffffffU;
  int led_fd;

  (void)argc;
  (void)argv;

  led_fd = open(CONFIG_BARGRAPH_LED_DEVPATH, O_WRONLY);
  if (led_fd < 0)
    {
      syslog(LOG_ERR, "bargraph: failed to open %s: %d\n",
             CONFIG_BARGRAPH_LED_DEVPATH, errno);
      g_daemon_running = false;
      g_daemon_pid     = -1;
      return EXIT_FAILURE;
    }

  if (!g_bargraph_state.initialized)
    {
      bargraph_state_init(&g_bargraph_state,
                          BARGRAPH_DEFAULT_EMA_NUM,
                          BARGRAPH_DEFAULT_EMA_DEN,
                          BARGRAPH_DEFAULT_HYST_COUNTS);
    }

  while (!g_daemon_stop)
    {
      uint16_t raw12 = 0U;

      if (bargraph_read_adc_once(&raw12) == 0)
        {
          bargraph_state_update(&g_bargraph_state, raw12);
          if (g_bargraph_state.led_mask != prev_mask)
            {
              if (bargraph_write_leds(led_fd, g_bargraph_state.led_mask) == 0)
                {
                  prev_mask = g_bargraph_state.led_mask;
                }
            }
        }

      usleep((useconds_t)CONFIG_BARGRAPH_POLL_INTERVAL_MS * 1000U);
    }

  close(led_fd);
  g_daemon_running = false;
  g_daemon_stop    = false;
  g_daemon_pid     = -1;
  return EXIT_SUCCESS;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: bargraph_autostart
 *
 * Description:
 *   Idempotently start the background bar-graph daemon task. Can be called
 *   from board bringup or via the NSH startup script / CLI.
 ****************************************************************************/

int bargraph_autostart(void)
{
  int pid;

  sched_lock();
  if (g_daemon_running)
    {
      sched_unlock();
      return OK;
    }

  g_daemon_stop    = false;
  g_daemon_running = true;
  sched_unlock();

  pid = task_create("bargraph_d",
                    CONFIG_BARGRAPH_APP_PRIORITY,
                    CONFIG_BARGRAPH_APP_STACKSIZE,
                    bargraph_daemon,
                    NULL);
  if (pid < 0)
    {
      int errcode = errno;
      g_daemon_running = false;
      return -errcode;
    }

  g_daemon_pid = (pid_t)pid;
  return OK;
}

/****************************************************************************
 * Name: bargraph_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  if (argc <= 1 || strcmp(argv[1], "start") == 0)
    {
      bool already = g_daemon_running;
      int ret      = bargraph_autostart();
      if (ret < 0)
        {
          fprintf(stderr, "bargraph: failed to start daemon: %d\n", -ret);
          return EXIT_FAILURE;
        }

      if (!already)
        {
          printf("bargraph: daemon started (pid=%d)\n", (int)g_daemon_pid);
        }
      else
        {
          printf("bargraph: daemon already running (pid=%d)\n",
                 (int)g_daemon_pid);
        }

      return EXIT_SUCCESS;
    }

  if (strcmp(argv[1], "status") == 0)
    {
      bargraph_print_status(&g_bargraph_state,
                            g_daemon_running,
                            g_daemon_pid);
      return EXIT_SUCCESS;
    }

  if (strcmp(argv[1], "stop") == 0)
    {
      int wait_ticks = 20;

      if (!g_daemon_running)
        {
          printf("bargraph: daemon is not running\n");
          return EXIT_SUCCESS;
        }

      g_daemon_stop = true;
      while (g_daemon_running && wait_ticks-- > 0)
        {
          usleep(10 * 1000U);
        }

      printf("bargraph: daemon stopped\n");
      return EXIT_SUCCESS;
    }

  if (strcmp(argv[1], "once") == 0)
    {
      uint16_t raw12 = 0U;
      int led_fd;
      int ret;

      ret = bargraph_read_adc_once(&raw12);
      if (ret < 0)
        {
          fprintf(stderr, "bargraph: ADC read failed: %d\n", -ret);
          return EXIT_FAILURE;
        }

      if (!g_bargraph_state.initialized)
        {
          bargraph_state_init(&g_bargraph_state,
                              BARGRAPH_DEFAULT_EMA_NUM,
                              BARGRAPH_DEFAULT_EMA_DEN,
                              BARGRAPH_DEFAULT_HYST_COUNTS);
        }

      bargraph_state_update(&g_bargraph_state, raw12);

      led_fd = open(CONFIG_BARGRAPH_LED_DEVPATH, O_WRONLY);
      if (led_fd >= 0)
        {
          bargraph_write_leds(led_fd, g_bargraph_state.led_mask);
          close(led_fd);
        }

      bargraph_print_status(&g_bargraph_state,
                            g_daemon_running,
                            g_daemon_pid);
      return EXIT_SUCCESS;
    }

  printf("Usage: %s [start|status|once|stop]\n", argv[0]);
  return (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
           ? EXIT_SUCCESS
           : EXIT_FAILURE;
}
