/****************************************************************************
 * xiao-rp2350-bargraph/board/include/board.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
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

/* GPIO definitions *********************************************************/

#define BOARD_GPIO_LED_PIN      25
#define BOARD_NGPIOOUT          0
#define BOARD_NGPIOIN           0
#define BOARD_NGPIOINT          0

/* Potentiometer ADC0 input pad (D0/A0 -> GPIO26) */

#define BARGRAPH_ADC_GPIO       26

/* LED definitions **********************************************************/

#define BOARD_LED1              0
#define BOARD_NLEDS             1

#define BOARD_LED_YELLOW        BOARD_LED1

#define BOARD_LED1_BIT          (1 << BOARD_LED1)

#define LED_STARTED             0  /* NuttX has been started  OFF    */
#define LED_HEAPALLOCATE        0  /* Heap has been allocated OFF    */
#define LED_IRQSENABLED         0  /* Interrupts enabled      OFF    */
#define LED_STACKCREATED        1  /* Idle stack created      ON     */
#define LED_INIRQ               2  /* In an interrupt         N/C    */
#define LED_SIGNAL              2  /* In a signal handler     N/C    */
#define LED_ASSERTION           2  /* An assertion failed     N/C    */
#define LED_PANIC               3  /* The system has crashed  FLASH  */
#undef  LED_IDLE                   /* Not used                       */

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#if defined(__cplusplus)
extern "C"
{
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: rp23xx_boardearlyinitialize
 ****************************************************************************/

void rp23xx_boardearlyinitialize(void);

/****************************************************************************
 * Name: rp23xx_boardinitialize
 ****************************************************************************/

void rp23xx_boardinitialize(void);

#if defined(__cplusplus)
}
#endif
#endif /* __ASSEMBLY__ */
#endif /* __XIAO_RP2350_BARGRAPH_BOARD_INCLUDE_BOARD_H */
