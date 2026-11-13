/****************************************************************************
 * xiao-rp2350-bargraph/external/bargraph/bargraph_main.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include <nuttx/analog/adc.h>
#include <nuttx/irq.h>
#include <nuttx/leds/userled.h>

#include "bargraph_logic.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

static pid_t g_bargraph_pid = -1;
static volatile sig_atomic_t g_bargraph_stop = 0;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void sigterm_handler(int signo, FAR siginfo_t *info, FAR void *arg)
{
  (void)info;
  (void)arg;

  if (signo == SIGTERM)
    {
      g_bargraph_stop = 1;
    }
}

static int redirect_stdio_to_null(void)
{
  int fd = open("/dev/null", O_RDWR);
  if (fd < 0)
    {
      syslog(LOG_ERR, "bargraph: failed to open /dev/null: %d\n", errno);
      return -1;
    }

  for (int target = 0; target <= 2; target++)
    {
      if (fd != target && dup2(fd, target) < 0)
        {
          syslog(LOG_ERR, "bargraph: dup2(/dev/null, %d) failed: %d\n",
                 target, errno);
          if (fd > 2)
            {
              close(fd);
            }

          return -1;
        }
    }

  if (fd > 2)
    {
      close(fd);
    }

  return 0;
}

static void close_adc_critical(int fd)
{
  /* adc_close() calls rp23xx_adc remove_device() without disabling IRQs;
   * guard close() so g_first_device and g_active_count update atomically
   * against the RP23XX ADC FIFO ISR.
   */

  irqstate_t flags = enter_critical_section();
  close(fd);
  leave_critical_section(flags);
}

static int read_adc_sample(FAR const char *adc_path, FAR uint16_t *raw_out)
{
  int fd = open(adc_path, O_RDONLY | O_NONBLOCK);
  if (fd < 0)
    {
      int err = errno;
      return (err == EINTR) ? -EINTR : -err;
    }

  struct pollfd pfd;
  pfd.fd      = fd;
  pfd.events  = POLLIN;
  pfd.revents = 0;

  int poll_ret = poll(&pfd, 1, BARGRAPH_POLL_TIMEOUT_MS);
  if (poll_ret == 0)
    {
      close_adc_critical(fd);
      return -EAGAIN;
    }

  if (poll_ret < 0 || (pfd.revents & POLLIN) == 0)
    {
      int err = errno;
      close_adc_critical(fd);
      if (poll_ret < 0 && err == EINTR)
        {
          return -EINTR;
        }

      return (poll_ret < 0) ? -err : -EIO;
    }

  struct adc_msg_s sample;
  ssize_t nbytes = read(fd, &sample, sizeof(sample));
  int err = errno;
  close_adc_critical(fd);

  if (nbytes < 0)
    {
      return (err == EINTR) ? -EINTR : -err;
    }

  if (nbytes != (ssize_t)sizeof(struct adc_msg_s))
    {
      return -EIO;
    }

  return decode_adc_sample(sample.am_data, raw_out);
}

static int fault_blink_loop(int led_fd)
{
  unsigned long pattern = 0x15UL;

  while (!g_bargraph_stop)
    {
      if (ioctl(led_fd, ULEDIOC_SETALL, pattern) < 0)
        {
          syslog(LOG_ERR,
                 "bargraph: fault ioctl(ULEDIOC_SETALL) failed: %d\n",
                 errno);
          return -1;
        }

      pattern = (pattern == 0x15UL) ? 0x2aUL : 0x15UL;
      usleep(BARGRAPH_FAULT_BLINK_US);
    }

  return 0;
}

static int bargraph_daemon(int argc, FAR char *argv[])
{
  FAR const char *adc_path = (argc >= 2 && argv[1] != NULL) ?
                             argv[1] : "/dev/adc0";
  int led_fd = -1;
  int ret_code = EXIT_SUCCESS;

  if (redirect_stdio_to_null() < 0)
    {
      ret_code = EXIT_FAILURE;
      goto cleanup;
    }

  struct sigaction act;
  memset(&act, 0, sizeof(struct sigaction));
  act.sa_sigaction = sigterm_handler;
  act.sa_flags     = SA_SIGINFO;
  sigemptyset(&act.sa_mask);

  if (sigaction(SIGTERM, &act, NULL) != 0)
    {
      syslog(LOG_ERR, "bargraph: sigaction(SIGTERM) failed: %d\n", errno);
      ret_code = EXIT_FAILURE;
      goto cleanup;
    }

  led_fd = open("/dev/userleds", O_WRONLY);
  if (led_fd < 0)
    {
      syslog(LOG_ERR, "bargraph: failed to open /dev/userleds: %d\n", errno);
      ret_code = EXIT_FAILURE;
      goto cleanup;
    }

  if (ioctl(led_fd, ULEDIOC_SETALL, 0UL) < 0)
    {
      syslog(LOG_ERR, "bargraph: failed to clear /dev/userleds: %d\n",
             errno);
      ret_code = EXIT_FAILURE;
      goto cleanup;
    }

  syslog(LOG_INFO, "bargraph: daemon started (pid=%d, adc=%s)\n",
         (int)getpid(), adc_path);

  int current_level = 0;
  int consecutive_errors = 0;

  while (!g_bargraph_stop)
    {
      uint16_t raw = 0;
      int ret = read_adc_sample(adc_path, &raw);

      if (ret == -EINTR || g_bargraph_stop)
        {
          break;
        }

      if (ret < 0)
        {
          consecutive_errors++;
          if (consecutive_errors == BARGRAPH_FAULT_TICKS)
            {
              syslog(LOG_ERR,
                     "bargraph: ADC fault after %d consecutive errors on %s "
                     "(last=%d)\n",
                     consecutive_errors, adc_path, ret);
              if (fault_blink_loop(led_fd) < 0)
                {
                  ret_code = EXIT_FAILURE;
                }

              break;
            }

          usleep(BARGRAPH_INTERVAL_US);
          continue;
        }

      consecutive_errors = 0;

      int next_level = quantize_level(raw, current_level,
                                      BARGRAPH_HYSTERESIS);
      if (next_level != current_level)
        {
          current_level = next_level;
          unsigned long mask = (unsigned long)level_to_led_mask(current_level);
          if (ioctl(led_fd, ULEDIOC_SETALL, mask) < 0)
            {
              syslog(LOG_ERR, "bargraph: ioctl(ULEDIOC_SETALL) failed: %d\n",
                     errno);
              ret_code = EXIT_FAILURE;
              goto cleanup;
            }
        }

      usleep(BARGRAPH_INTERVAL_US);
    }

cleanup:
  if (led_fd >= 0)
    {
      (void)ioctl(led_fd, ULEDIOC_SETALL, 0UL);
      close(led_fd);
    }

  sched_lock();
  if (g_bargraph_pid == getpid())
    {
      g_bargraph_pid = -1;
    }
  sched_unlock();

  return ret_code;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  FAR const char *adc_path = (argc >= 2 && argv[1] != NULL) ?
                             argv[1] : "/dev/adc0";
  FAR char * const daemon_argv[] =
  {
    (FAR char *)adc_path,
    NULL
  };

  sched_lock();
  if (g_bargraph_pid > 0 && kill(g_bargraph_pid, 0) == 0)
    {
      sched_unlock();
      printf("bargraph: daemon already running\n");
      return EXIT_SUCCESS;
    }

  g_bargraph_stop = 0;

  pid_t pid = task_create("bargraph_daemon",
                          CONFIG_BARGRAPH_PRIORITY,
                          CONFIG_BARGRAPH_STACKSIZE,
                          bargraph_daemon,
                          daemon_argv);
  if (pid > 0)
    {
      g_bargraph_pid = pid;
    }
  sched_unlock();

  if (pid < 0)
    {
      syslog(LOG_ERR, "bargraph: task_create failed: %d\n", errno);
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
