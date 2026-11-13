/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_bringup.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <nuttx/debug.h>
#include <syslog.h>

#include <arch/board/board.h>

#ifdef CONFIG_ARCH_BOARD_COMMON
#  include "rp23xx_common_bringup.h"
#endif

#if defined(CONFIG_ADC) && defined(CONFIG_RP23XX_ADC)
#  include "rp23xx_gpio.h"
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
  int status;

#ifdef CONFIG_ARCH_BOARD_COMMON
  status = rp23xx_common_bringup();
  if (status < 0)
    {
      syslog(LOG_ERR, "ERROR: rp23xx_common_bringup() failed: %d\n", status);
      ret = status;
    }
#endif

#if defined(CONFIG_ADC) && defined(CONFIG_RP23XX_ADC)
  /* rp23xx_adc my_reset() re-enables digital pad input buffers (IE=1)
   * across GPIO 26..29 during adc_register(). Clear IE on all four ADC
   * pads so analog sampling on GPIO 26 avoids digital input-buffer leakage.
   */

  for (int i = 0; i < GPIO_ADC_COUNT; i++)
    {
      clrbits_reg32(RP23XX_PADS_BANK0_GPIO_IE,
                    RP23XX_PADS_BANK0_GPIO(GPIO_ADC_BASE + i));
    }
#endif

#ifdef CONFIG_USERLED
  /* Register /dev/userleds even if rp23xx_common_bringup() failed so the
   * daemon can still indicate faults on the hardware LEDs.
   */

  status = userled_lower_initialize("/dev/userleds");
  if (status < 0)
    {
      syslog(LOG_ERR,
             "ERROR: userled_lower_initialize() failed: %d\n", status);
      if (ret == OK)
        {
          ret = status;
        }
    }
#endif

  return ret;
}
