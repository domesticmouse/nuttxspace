/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_pico.h
 *
 * SPDX-License-Identifier: Apache-2.0
 ****************************************************************************/

#ifndef __XIAO_RP2350_BARGRAPH_BOARD_SRC_RP23XX_PICO_H
#define __XIAO_RP2350_BARGRAPH_BOARD_SRC_RP23XX_PICO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <arch/board/board.h>

int rp23xx_bringup(void);

#ifdef CONFIG_BARGRAPH_APP
int bargraph_autostart(void);
#endif

#if defined(CONFIG_DEV_GPIO) && !defined(CONFIG_ARCH_BOARD_COMMON)
int rp23xx_dev_gpio_init(void);
#endif

#endif /* __XIAO_RP2350_BARGRAPH_BOARD_SRC_RP23XX_PICO_H */
