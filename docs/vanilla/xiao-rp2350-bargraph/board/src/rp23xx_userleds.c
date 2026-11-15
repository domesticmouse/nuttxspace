/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_userleds.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * UserLED lower-half support for the 6 breadboard bar-graph LEDs on the
 * Seeed Studio XIAO RP2350.
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdint.h>
#include <nuttx/debug.h>

#include <nuttx/board.h>
#include <nuttx/leds/userled.h>
#include <arch/board/board.h>

#include "chip.h"
#include "rp23xx_gpio.h"
#include "rp23xx_pico.h"

#ifndef CONFIG_ARCH_LEDS

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Maps bar-graph LED index (0..5) to RP2350 GPIO pin (active-high) */

static const uint32_t g_ledcfg[BOARD_NLEDS] =
{
  GPIO_LED1,  /* Bar LED 1 (lowest)  - XIAO D3 (GPIO5) */
  GPIO_LED2,  /* Bar LED 2           - XIAO D4 (GPIO6) */
  GPIO_LED3,  /* Bar LED 3           - XIAO D5 (GPIO7) */
  GPIO_LED4,  /* Bar LED 4           - XIAO D6 (GPIO0) */
  GPIO_LED5,  /* Bar LED 5           - XIAO D7 (GPIO1) */
  GPIO_LED6   /* Bar LED 6 (highest) - XIAO D8 (GPIO2) */
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_userled_initialize
 ****************************************************************************/

uint32_t board_userled_initialize(void)
{
  int i;

  for (i = 0; i < BOARD_NLEDS; i++)
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
  int i;

  for (i = 0; i < BOARD_NLEDS; i++)
    {
      rp23xx_gpio_put(g_ledcfg[i], (ledset & (1U << i)) != 0U);
    }
}

#ifdef CONFIG_USERLED_LOWER_READSTATE
/****************************************************************************
 * Name: board_userled_getall
 ****************************************************************************/

void board_userled_getall(userled_set_t *ledset)
{
  userled_set_t mask = 0U;
  int i;

  if (ledset == NULL)
    {
      return;
    }

  for (i = 0; i < BOARD_NLEDS; i++)
    {
      if (rp23xx_gpio_get(g_ledcfg[i]))
        {
          mask |= (1U << i);
        }
    }

  *ledset = mask;
}
#endif

#endif /* !CONFIG_ARCH_LEDS */
