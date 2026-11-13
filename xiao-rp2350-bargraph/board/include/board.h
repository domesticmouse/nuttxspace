/****************************************************************************
 * xiao-rp2350-bargraph/board/include/board.h
 ****************************************************************************/

#ifndef __XIAO_RP2350_BARGRAPH_BOARD_INCLUDE_BOARD_H
#define __XIAO_RP2350_BARGRAPH_BOARD_INCLUDE_BOARD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

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

/* GPIO / ADC definitions ***************************************************/

#define BOARD_GPIO_LED_PIN      25
#define GPIO_ADC_BASE           26
#define GPIO_ADC_COUNT          4

/* LED definitions **********************************************************/

#define BOARD_NLEDS             6

#ifndef __ASSEMBLY__

#if defined(__cplusplus)
extern "C"
{
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

void rp23xx_boardearlyinitialize(void);
void rp23xx_boardinitialize(void);
int rp23xx_bringup(void);

#if defined(__cplusplus)
}
#endif
#endif /* __ASSEMBLY__ */
#endif /* __XIAO_RP2350_BARGRAPH_BOARD_INCLUDE_BOARD_H */
