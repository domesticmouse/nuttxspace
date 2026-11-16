/****************************************************************************
 * xiao-rp2350-bargraph/board/src/rp23xx_bringup.c
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

#include <sys/types.h>
#include <syslog.h>

#include <arch/board/board.h>

#include "arm_internal.h"
#include "hardware/rp23xx_pads_bank0.h"
#include "rp23xx_pico.h"

#ifdef CONFIG_ARCH_BOARD_COMMON
#  include "rp23xx_common_bringup.h"
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rp23xx_bringup
 ****************************************************************************/

int rp23xx_bringup(void)
{
  int ret = OK;

#ifdef CONFIG_ARCH_BOARD_COMMON
  ret = rp23xx_common_bringup();
  if (ret < 0)
    {
      return ret;
    }
#endif

  /* Mitigate Erratum RP2350-E9 digital pad leakage on D0/A0 (GPIO26):
   * Disable digital input buffer (IE=0), pull-up (PUE=0), pull-down (PDE=0),
   * and pad isolation (ISO=0), and set output disable (OD=1).
   */

  modreg32(RP23XX_PADS_BANK0_GPIO_OD,
           RP23XX_PADS_BANK0_GPIO_IE |
           RP23XX_PADS_BANK0_GPIO_PUE |
           RP23XX_PADS_BANK0_GPIO_PDE |
           RP23XX_PADS_BANK0_GPIO_ISO,
           RP23XX_PADS_BANK0_GPIO(BARGRAPH_ADC_GPIO));

#ifdef CONFIG_BARGRAPH
  ret = bargraph_autostart();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: bargraph_autostart() failed: %d\n", ret);
    }
#endif

  return ret;
}
