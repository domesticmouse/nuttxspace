/****************************************************************************
 * xiao-rp2350-bargraph/board/include/board.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Custom board definitions for the Seeed Studio XIAO RP2350 breadboard
 * potentiometer + 6-LED bar-graph setup.
 ****************************************************************************/

#ifndef __XIAO_RP2350_BARGRAPH_BOARD_INCLUDE_BOARD_H
#define __XIAO_RP2350_BARGRAPH_BOARD_INCLUDE_BOARD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include "rp23xx_i2cdev.h"
#include "rp23xx_spidev.h"
#include "rp23xx_i2sdev.h"
#include "rp23xx_spisd.h"

#ifndef __ASSEMBLY__
#  include <stdint.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Clocking *****************************************************************/

#define MHZ                     1000000

#define BOARD_XOSC_FREQ         (12 * MHZ)
#define BOARD_XOSC_STARTUPDELAY 1
#define BOARD_PLL_SYS_FREQ      (150 * MHZ)
#define BOARD_PLL_USB_FREQ      (48 * MHZ)

#define BOARD_REF_FREQ          (12 * MHZ)
#define BOARD_SYS_FREQ          (150 * MHZ)
#define BOARD_PERI_FREQ         (150 * MHZ)
#define BOARD_USB_FREQ          (48 * MHZ)
#define BOARD_ADC_FREQ          (48 * MHZ)
#define BOARD_HSTX_FREQ         (150 * MHZ)

#define BOARD_UART_BASEFREQ     BOARD_PERI_FREQ

#define BOARD_TICK_CLOCK        (1 * MHZ)

/* Potentiometer ADC definition *********************************************/

/* Potentiometer wiper connected to XIAO RP2350 pin D0/A0 (GPIO26, ADC0) */

#define BOARD_POT_ADC_CHANNEL   0
#define BOARD_POT_ADC_GPIO      26

/* Onboard yellow status LED (GPIO25, active-low) ***************************/

#define BOARD_GPIO_LED_PIN      25

/* Breadboard 6-LED bar graph (active-high, lowest to highest) **************
 *
 *   Bar LED 1 (lowest)  -> XIAO D3 (GPIO5)
 *   Bar LED 2           -> XIAO D4 (GPIO6)
 *   Bar LED 3           -> XIAO D5 (GPIO7)
 *   Bar LED 4           -> XIAO D6 (GPIO0)
 *   Bar LED 5           -> XIAO D7 (GPIO1)
 *   Bar LED 6 (highest) -> XIAO D8 (GPIO2)
 */

#define GPIO_LED1               5   /* XIAO D3 - Bar LED 1 (lowest)  */
#define GPIO_LED2               6   /* XIAO D4 - Bar LED 2           */
#define GPIO_LED3               7   /* XIAO D5 - Bar LED 3           */
#define GPIO_LED4               0   /* XIAO D6 - Bar LED 4           */
#define GPIO_LED5               1   /* XIAO D7 - Bar LED 5           */
#define GPIO_LED6               2   /* XIAO D8 - Bar LED 6 (highest) */

/* GPIO driver definitions **************************************************/

#define BOARD_NGPIOOUT          6
#define BOARD_NGPIOIN           0
#define BOARD_NGPIOINT          0

#define GPIO_OUT1               GPIO_LED1
#define GPIO_OUT2               GPIO_LED2
#define GPIO_OUT3               GPIO_LED3
#define GPIO_OUT4               GPIO_LED4
#define GPIO_OUT5               GPIO_LED5
#define GPIO_OUT6               GPIO_LED6

/* UserLED definitions ******************************************************/

#define BOARD_LED1              0
#define BOARD_LED2              1
#define BOARD_LED3              2
#define BOARD_LED4              3
#define BOARD_LED5              4
#define BOARD_LED6              5
#define BOARD_NLEDS             6

#define BOARD_LED_YELLOW        BOARD_LED1

#define BOARD_LED1_BIT          (1 << BOARD_LED1)
#define BOARD_LED2_BIT          (1 << BOARD_LED2)
#define BOARD_LED3_BIT          (1 << BOARD_LED3)
#define BOARD_LED4_BIT          (1 << BOARD_LED4)
#define BOARD_LED5_BIT          (1 << BOARD_LED5)
#define BOARD_LED6_BIT          (1 << BOARD_LED6)

/* AutoLED definitions (used only if CONFIG_ARCH_LEDS is enabled) ***********/

#define LED_STARTED             0
#define LED_HEAPALLOCATE        0
#define LED_IRQSENABLED         0
#define LED_STACKCREATED        1
#define LED_INIRQ               2
#define LED_SIGNAL              2
#define LED_ASSERTION           2
#define LED_PANIC               3
#undef  LED_IDLE

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__
#if defined(__cplusplus)
extern "C"
{
#endif

void rp23xx_boardearlyinitialize(void);
void rp23xx_boardinitialize(void);

#if defined(__cplusplus)
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __XIAO_RP2350_BARGRAPH_BOARD_INCLUDE_BOARD_H */
