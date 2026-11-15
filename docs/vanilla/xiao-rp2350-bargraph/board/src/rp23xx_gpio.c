/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_gpio.c
 *
 * SPDX-License-Identifier: Apache-2.0
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <syslog.h>
#include <nuttx/irq.h>
#include <arch/irq.h>
#include <assert.h>
#include <nuttx/debug.h>

#include <nuttx/ioexpander/gpio.h>
#include <arch/board/board.h>

#include "arm_internal.h"
#include "chip.h"
#include "rp23xx_gpio.h"

#if defined(CONFIG_DEV_GPIO) && !defined(CONFIG_GPIO_LOWER_HALF)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct rp23xxgpio_dev_s
{
  struct gpio_dev_s gpio;
  uint8_t id;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#if BOARD_NGPIOOUT > 0
static int gpout_read(struct gpio_dev_s *dev, bool *value);
static int gpout_write(struct gpio_dev_s *dev, bool value);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#if BOARD_NGPIOOUT > 0
static const struct gpio_operations_s gpout_ops =
{
  .go_read   = gpout_read,
  .go_write  = gpout_write,
  .go_attach = NULL,
  .go_enable = NULL,
};

static const uint32_t g_gpiooutputs[BOARD_NGPIOOUT] =
{
  GPIO_OUT1,
  GPIO_OUT2,
  GPIO_OUT3,
  GPIO_OUT4,
  GPIO_OUT5,
  GPIO_OUT6
};

static struct rp23xxgpio_dev_s g_gpout[BOARD_NGPIOOUT];
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if BOARD_NGPIOOUT > 0
static int gpout_read(struct gpio_dev_s *dev, bool *value)
{
  struct rp23xxgpio_dev_s *rp23xxgpio = (struct rp23xxgpio_dev_s *)dev;

  DEBUGASSERT(rp23xxgpio != NULL && value != NULL);
  DEBUGASSERT(rp23xxgpio->id < BOARD_NGPIOOUT);

  *value = rp23xx_gpio_get(g_gpiooutputs[rp23xxgpio->id]);
  return OK;
}

static int gpout_write(struct gpio_dev_s *dev, bool value)
{
  struct rp23xxgpio_dev_s *rp23xxgpio = (struct rp23xxgpio_dev_s *)dev;

  DEBUGASSERT(rp23xxgpio != NULL);
  DEBUGASSERT(rp23xxgpio->id < BOARD_NGPIOOUT);

  rp23xx_gpio_put(g_gpiooutputs[rp23xxgpio->id], value);
  return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int rp23xx_dev_gpio_init(void)
{
#if BOARD_NGPIOOUT > 0
  int i;

  for (i = 0; i < BOARD_NGPIOOUT; i++)
    {
      g_gpout[i].gpio.gp_pintype = GPIO_OUTPUT_PIN;
      g_gpout[i].gpio.gp_ops     = &gpout_ops;
      g_gpout[i].id              = i;
      gpio_pin_register(&g_gpout[i].gpio, g_gpiooutputs[i]);

      rp23xx_gpio_init(g_gpiooutputs[i]);
      rp23xx_gpio_setdir(g_gpiooutputs[i], true);
      rp23xx_gpio_put(g_gpiooutputs[i], false);
    }
#endif

  return 0;
}

#endif /* CONFIG_DEV_GPIO && !CONFIG_GPIO_LOWER_HALF */
