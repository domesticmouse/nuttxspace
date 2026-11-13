/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_boardinitialize.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>

#include <nuttx/board.h>
#include <arch/board/board.h>

#include "rp23xx_gpio.h"

#ifdef CONFIG_RP23XX_PSRAM
#  include "rp23xx_psram.h"
#endif

#ifdef CONFIG_ARCH_BOARD_COMMON
#  include "rp23xx_common_initialize.h"
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rp23xx_boardearlyinitialize
 ****************************************************************************/

void rp23xx_boardearlyinitialize(void)
{
#ifdef CONFIG_ARCH_BOARD_COMMON
  rp23xx_common_earlyinitialize();
#endif

  /* Drive the active-low onboard LED pin high (LED off) at boot */

  rp23xx_gpio_init(BOARD_GPIO_LED_PIN);
  rp23xx_gpio_setdir(BOARD_GPIO_LED_PIN, true);
  rp23xx_gpio_put(BOARD_GPIO_LED_PIN, true);
}

/****************************************************************************
 * Name: rp23xx_boardinitialize
 ****************************************************************************/

void rp23xx_boardinitialize(void)
{
#ifdef CONFIG_ARCH_BOARD_COMMON
  rp23xx_common_initialize();
#endif

#ifdef CONFIG_RP23XX_PSRAM
  rp23xx_psramconfig();
#endif
}

/****************************************************************************
 * Name: board_late_initialize
 ****************************************************************************/

#ifdef CONFIG_BOARD_LATE_INITIALIZE
void board_late_initialize(void)
{
  (void)rp23xx_bringup();
}
#endif
