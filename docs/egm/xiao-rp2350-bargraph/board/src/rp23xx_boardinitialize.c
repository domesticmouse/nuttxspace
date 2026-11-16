/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_boardinitialize.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/debug.h>
#include <nuttx/board.h>
#include <arch/board/board.h>

#include "rp23xx_gpio.h"

#ifdef CONFIG_RP23XX_PSRAM
#  include "rp23xx_psram.h"
#endif

#ifdef CONFIG_ARCH_BOARD_COMMON
#  include "rp23xx_common_initialize.h"
#endif

#include "rp23xx_pico.h"

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

  /* Set on-board status LED pin (GPIO25) */

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
  rp23xx_bringup();
}
#endif
