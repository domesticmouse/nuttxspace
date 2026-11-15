/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_bringup.c
 *
 * SPDX-License-Identifier: Apache-2.0
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <nuttx/debug.h>
#include <stddef.h>
#include <syslog.h>

#include <nuttx/fs/fs.h>
#include <arch/board/board.h>

#include "rp23xx_pico.h"

#ifdef CONFIG_ARCH_BOARD_COMMON
#  include "rp23xx_common_bringup.h"
#endif

#ifdef CONFIG_USERLED
#  include <nuttx/leds/userled.h>
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rp23xx_bringup
 ****************************************************************************/

int rp23xx_bringup(void)
{
  int ret = OK;

#ifdef CONFIG_ARCH_BOARD_COMMON
  ret = rp23xx_common_bringup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: rp23xx_common_bringup() failed: %d\n", ret);
    }
#endif

#ifdef CONFIG_USERLED
  /* Register the 6-LED bar-graph userled driver at /dev/userleds */

  ret = userled_lower_initialize("/dev/userleds");
  if (ret < 0)
    {
      syslog(LOG_ERR,
             "ERROR: userled_lower_initialize() failed: %d\n", ret);
    }
#endif

#ifdef CONFIG_BARGRAPH_APP
  /* Auto-start the potentiometer bar-graph daemon immediately at boot so
   * that the 6 LEDs track the potentiometer even before USB serial attaches.
   */

  ret = bargraph_autostart();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: bargraph_autostart() failed: %d\n", ret);
    }
#endif

  return OK;
}
