/****************************************************************************
 * xiao-rp2350-bargraph/apps/bargraph/bargraph_internal.h
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

#ifndef __XIAO_RP2350_BARGRAPH_APPS_BARGRAPH_BARGRAPH_INTERNAL_H
#define __XIAO_RP2350_BARGRAPH_APPS_BARGRAPH_BARGRAPH_INTERNAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#ifndef FAR
#  define FAR
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define BARGRAPH_NUM_LEDS          6
#define BARGRAPH_NUM_PWM_SLICES    5
#define BARGRAPH_PWM_FREQ_HZ       1000
#define BARGRAPH_DUTY_MAX          65535u
#define BARGRAPH_TOTAL_UNITS_MAX   (BARGRAPH_NUM_LEDS * BARGRAPH_DUTY_MAX)
#define BARGRAPH_ADC_MAX           4095u
#define BARGRAPH_ADC_DEADBAND_LOW  32u
#define BARGRAPH_ADC_DEADBAND_HIGH 4063u
#define BARGRAPH_ADC_BURST_SAMPLES 4
#define BARGRAPH_ADC_READ_SAMPLES  (BARGRAPH_ADC_BURST_SAMPLES + 1)
#define BARGRAPH_ADC_SHIFT         19
#define BARGRAPH_TICK_USEC         20000
#define BARGRAPH_TICK_NSEC         20000000L
#define BARGRAPH_TEST_HALF_STEPS   50u

/****************************************************************************
 * Public Types
 ****************************************************************************/

enum bargraph_mode_e
{
  BARGRAPH_MODE_STOPPED = 0,
  BARGRAPH_MODE_SENSING,
  BARGRAPH_MODE_MANUAL,
  BARGRAPH_MODE_TEST
};

struct bargraph_state_s
{
  pthread_mutex_t      lock;
  pthread_cond_t       wake_cond;
  pthread_cond_t       done_cond;
  enum bargraph_mode_e mode;
  enum bargraph_mode_e prev_mode;
  bool                 running;
  bool                 stop_requested;
  bool                 test_requested;
  int                  init_err;        /* 0 on success, -errno on startup failure */
  uint8_t              manual_pct;      /* 0..100 */
  uint16_t             raw_adc;         /* 0..4095 latest burst average */
  uint16_t             filtered_adc;    /* 0..4095 Q4 EMA + deadband clamped */
  uint16_t             millivolts;      /* 0..3300 mV */
  uint16_t             permille;        /* 0..1000 (0.0%..100.0%) */
  uint16_t             duty[BARGRAPH_NUM_LEDS]; /* 0..65535 per LED1..LED6 */
  pid_t                daemon_pid;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

int bargraph_autostart(void);

void bargraph_compute_duties(uint32_t total_units,
                             FAR uint16_t duties[BARGRAPH_NUM_LEDS]);

uint16_t bargraph_filter_adc(uint16_t raw_avg,
                             FAR uint32_t *ema_q4,
                             FAR bool *ema_initialized);

#ifdef __cplusplus
}
#endif

#endif /* __XIAO_RP2350_BARGRAPH_APPS_BARGRAPH_BARGRAPH_INTERNAL_H */
