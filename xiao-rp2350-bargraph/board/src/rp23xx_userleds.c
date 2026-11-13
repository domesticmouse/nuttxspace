/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_userleds.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>

#include <nuttx/board.h>
#include <arch/board/board.h>

#include "rp23xx_gpio.h"

#ifndef CONFIG_ARCH_LEDS

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Ordered mapping for breadboard bar-graph LEDs: D3, D4, D5, D8, D9, D10 */

static const uint32_t g_ledcfg[BOARD_NLEDS] =
{
  5, 6, 7, 2, 4, 3
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_userled_initialize
 ****************************************************************************/

uint32_t board_userled_initialize(void)
{
  for (int i = 0; i < BOARD_NLEDS; i++)
    {
      rp23xx_gpio_init(g_ledcfg[i]);
      rp23xx_gpio_setdir(g_ledcfg[i], true);
      rp23xx_gpio_put(g_ledcfg[i], false);
    }

  return BOARD_NLEDS;
}

/****************************************************************************
 * Name: board_userled
 ****************************************************************************/

void board_userled(int led, bool ledon)
{
  if ((unsigned int)led < BOARD_NLEDS)
    {
      rp23xx_gpio_put(g_ledcfg[led], ledon);
    }
}

/****************************************************************************
 * Name: board_userled_all
 ****************************************************************************/

void board_userled_all(uint32_t ledset)
{
  for (int i = 0; i < BOARD_NLEDS; i++)
    {
      rp23xx_gpio_put(g_ledcfg[i], (ledset & (1UL << i)) != 0);
    }
}

#endif /* !CONFIG_ARCH_LEDS */
