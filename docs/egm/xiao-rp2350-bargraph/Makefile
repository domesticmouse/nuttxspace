############################################################################
# xiao-rp2350-bargraph/Makefile
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.  The
# ASF licenses this file to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations
# under the License.
#
############################################################################

NUTTX_DIR := $(abspath $(CURDIR)/../nuttx)
CONFIG_PATH := ../xiao-rp2350-bargraph/board/configs/bargraph
NJOBS ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
HOSTCC ?= cc

.PHONY: all configure build test clean distclean

all: build

configure:
	@if [ ! -f "$(NUTTX_DIR)/.config" ] || ! grep -q 'CONFIG_ARCH_BOARD_CUSTOM_NAME="xiao-rp2350-bargraph"' "$(NUTTX_DIR)/.config" 2>/dev/null; then \
		cd "$(NUTTX_DIR)" && ./tools/configure.sh $(CONFIG_PATH); \
	fi

build: configure
	$(MAKE) -C "$(NUTTX_DIR)" -j$(NJOBS)

test:
	@TMP_DIR=$$(mktemp -d /tmp/bargraph_test.XXXXXX); \
	TMP_SRC="$$TMP_DIR/bargraph_test.c"; \
	TMP_BIN="$$TMP_DIR/bargraph_test"; \
	printf '%s\n' \
		'#include <assert.h>' \
		'#include <stdbool.h>' \
		'#include <stdint.h>' \
		'#include <stdio.h>' \
		'#include <string.h>' \
		'#define FAR' \
		'#include "bargraph_internal.h"' \
		'void bargraph_compute_duties(uint32_t total_units, FAR uint16_t duties[BARGRAPH_NUM_LEDS])' \
		'{' \
		'  unsigned int i;' \
		'  if (total_units > BARGRAPH_TOTAL_UNITS_MAX) total_units = BARGRAPH_TOTAL_UNITS_MAX;' \
		'  for (i = 0; i < BARGRAPH_NUM_LEDS; i++)' \
		'    {' \
		'      uint32_t seg_start = (uint32_t)i * BARGRAPH_DUTY_MAX;' \
		'      uint32_t seg_end   = seg_start + BARGRAPH_DUTY_MAX;' \
		'      if (total_units >= seg_end) duties[i] = (uint16_t)BARGRAPH_DUTY_MAX;' \
		'      else if (total_units > seg_start) duties[i] = (uint16_t)(total_units - seg_start);' \
		'      else duties[i] = 0;' \
		'    }' \
		'}' \
		'uint16_t bargraph_filter_adc(uint16_t raw_avg, FAR uint32_t *ema_q4, FAR bool *ema_initialized)' \
		'{' \
		'  uint16_t filtered;' \
		'  if (raw_avg > BARGRAPH_ADC_MAX) raw_avg = BARGRAPH_ADC_MAX;' \
		'  if (!(*ema_initialized)) { *ema_q4 = (uint32_t)raw_avg << 4; *ema_initialized = true; }' \
		'  else { *ema_q4 = *ema_q4 - (*ema_q4 >> 2) + ((uint32_t)raw_avg << 2); }' \
		'  filtered = (uint16_t)((*ema_q4 + 8u) >> 4);' \
		'  if (filtered <= BARGRAPH_ADC_DEADBAND_LOW) filtered = 0;' \
		'  else if (filtered >= BARGRAPH_ADC_DEADBAND_HIGH) filtered = BARGRAPH_ADC_MAX;' \
		'  return filtered;' \
		'}' \
		'int main(void)' \
		'{' \
		'  uint16_t duties[BARGRAPH_NUM_LEDS];' \
		'  uint32_t ema_q4 = 0;' \
		'  bool ema_init = false;' \
		'  unsigned int i;' \
		'  bargraph_compute_duties(0, duties);' \
		'  for (i = 0; i < BARGRAPH_NUM_LEDS; i++) assert(duties[i] == 0);' \
		'  for (i = 1; i <= BARGRAPH_NUM_LEDS; i++)' \
		'    {' \
		'      unsigned int k;' \
		'      bargraph_compute_duties(i * BARGRAPH_DUTY_MAX, duties);' \
		'      for (k = 0; k < BARGRAPH_NUM_LEDS; k++)' \
		'        {' \
		'          assert(duties[k] <= BARGRAPH_DUTY_MAX);' \
		'          if (k < i) assert(duties[k] == BARGRAPH_DUTY_MAX);' \
		'          else assert(duties[k] == 0);' \
		'        }' \
		'    }' \
		'  bargraph_compute_duties(2 * BARGRAPH_DUTY_MAX + 12345u, duties);' \
		'  assert(duties[0] == 65535u && duties[1] == 65535u && duties[2] == 12345u);' \
		'  assert(duties[3] == 0 && duties[4] == 0 && duties[5] == 0);' \
		'  assert((((int32_t)(4095 << BARGRAPH_ADC_SHIFT) >> BARGRAPH_ADC_SHIFT) & 0x0fff) == 4095);' \
		'  assert((((int32_t)(2048 << BARGRAPH_ADC_SHIFT) >> BARGRAPH_ADC_SHIFT) & 0x0fff) == 2048);' \
		'  ema_init = false; assert(bargraph_filter_adc(0, &ema_q4, &ema_init) == 0);' \
		'  ema_init = false; assert(bargraph_filter_adc(32, &ema_q4, &ema_init) == 0);' \
		'  ema_init = false; assert(bargraph_filter_adc(33, &ema_q4, &ema_init) == 33);' \
		'  ema_init = false; assert(bargraph_filter_adc(2048, &ema_q4, &ema_init) == 2048);' \
		'  ema_init = false; assert(bargraph_filter_adc(4062, &ema_q4, &ema_init) == 4062);' \
		'  ema_init = false; assert(bargraph_filter_adc(4063, &ema_q4, &ema_init) == 4095);' \
		'  ema_init = false; assert(bargraph_filter_adc(4095, &ema_q4, &ema_init) == 4095);' \
		'  printf("bargraph unit tests passed\n");' \
		'  return 0;' \
		'}' > "$$TMP_SRC"; \
	$(HOSTCC) -Wall -Wextra -Werror -I"$(CURDIR)/apps/bargraph" "$$TMP_SRC" -o "$$TMP_BIN" && \
	"$$TMP_BIN"; \
	STATUS=$$?; \
	rm -rf "$$TMP_DIR"; \
	exit $$STATUS

clean:
	@if [ -f "$(NUTTX_DIR)/.config" ]; then \
		$(MAKE) -C "$(NUTTX_DIR)" clean; \
	fi

distclean:
	@if [ -f "$(NUTTX_DIR)/.config" ]; then \
		$(MAKE) -C "$(NUTTX_DIR)" distclean; \
	fi
	@rm -f "$(CURDIR)/../apps/external"
