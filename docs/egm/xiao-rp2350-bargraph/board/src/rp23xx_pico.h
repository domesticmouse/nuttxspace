/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_pico.h
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

#ifndef __XIAO_RP2350_BARGRAPH_BOARD_SRC_RP23XX_PICO_H
#define __XIAO_RP2350_BARGRAPH_BOARD_SRC_RP23XX_PICO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define GPIO_LED1       25 /* The board's LED is connected to this pin */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

int rp23xx_bringup(void);

#ifdef CONFIG_BARGRAPH
int bargraph_autostart(void);
#endif

#endif /* __XIAO_RP2350_BARGRAPH_BOARD_SRC_RP23XX_PICO_H */
