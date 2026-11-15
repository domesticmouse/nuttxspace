/****************************************************************************
 * xiao-rp2350-bargraph/tests/test_bargraph.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Host unit test suite for the XIAO RP2350 bar-graph logic module.
 ****************************************************************************/

#include "bargraph_logic.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static void test_decode_rp23xx_sample(void)
{
  uint16_t raw = 0xffffU;
  uint32_t code;

  /* Reject NULL output pointer */

  assert(bargraph_decode_rp23xx_sample(0, NULL) == -EINVAL);

  /* Reject negative samples (RP23XX_ADC_FIFO_ERR negated in rp23xx_adc.c) */

  assert(bargraph_decode_rp23xx_sample(-1, &raw) == -EIO);
  assert(bargraph_decode_rp23xx_sample(-(int32_t)(2048U << 19), &raw) == -EIO);

  /* Verify exact decoding of all 4096 shifted 12-bit codes (value << 19) */

  for (code = 0; code <= BARGRAPH_ADC_MAX; code++)
    {
      int32_t shifted = (int32_t)(code << BARGRAPH_RP23XX_ADC_SHIFT);
      assert(bargraph_decode_rp23xx_sample(shifted, &raw) == 0);
      assert(raw == (uint16_t)code);
    }

  /* Verify unshifted 12-bit values in [0, 4095] are also accepted */

  assert(bargraph_decode_rp23xx_sample(0, &raw) == 0 && raw == 0U);
  assert(bargraph_decode_rp23xx_sample(1234, &raw) == 0 && raw == 1234U);
  assert(bargraph_decode_rp23xx_sample(4095, &raw) == 0 && raw == 4095U);
}

static void test_conversions(void)
{
  /* Millivolts at 3300 mV reference */

  assert(bargraph_raw12_to_millivolts(0U, 3300U) == 0U);
  assert(bargraph_raw12_to_millivolts(4095U, 3300U) == 3300U);
  assert(bargraph_raw12_to_millivolts(2048U, 3300U) == 1650U);
  assert(bargraph_raw12_to_millivolts(9999U, 3300U) == 3300U);

  /* Percentage (0..100%) */

  assert(bargraph_raw12_to_percent(0U) == 0U);
  assert(bargraph_raw12_to_percent(2048U) == 50U);
  assert(bargraph_raw12_to_percent(4095U) == 100U);
  assert(bargraph_raw12_to_percent(8000U) == 100U);
}

static void test_levels_and_thresholds(void)
{
  uint16_t prev_level = 0U;
  uint16_t code;
  uint8_t  lvl;

  /* Verify monotonic non-decreasing mapping across entire 12-bit range */

  for (code = 0; code <= BARGRAPH_ADC_MAX; code++)
    {
      uint8_t cur = bargraph_raw12_to_level(code);
      assert(cur >= prev_level);
      assert(cur <= BARGRAPH_NUM_LEDS);
      prev_level = cur;
    }

  assert(bargraph_raw12_to_level(0U) == 0U);
  assert(bargraph_raw12_to_level(4095U) == 6U);
  assert(bargraph_raw12_to_level(65535U) == 6U);

  /* Verify threshold consistency with bargraph_raw12_to_level */

  for (lvl = 1U; lvl <= BARGRAPH_NUM_LEDS; lvl++)
    {
      uint16_t t = bargraph_threshold_for_level(lvl);
      assert(t > 0U && t <= BARGRAPH_ADC_MAX);
      assert(bargraph_raw12_to_level((uint16_t)(t - 1U)) == (uint8_t)(lvl - 1U));
      assert(bargraph_raw12_to_level(t) == lvl);
    }

  /* Verify LED bitmask generation for levels 0..6 */

  assert(bargraph_level_to_mask(0U) == 0x00U);
  assert(bargraph_level_to_mask(1U) == 0x01U);
  assert(bargraph_level_to_mask(2U) == 0x03U);
  assert(bargraph_level_to_mask(3U) == 0x07U);
  assert(bargraph_level_to_mask(4U) == 0x0fU);
  assert(bargraph_level_to_mask(5U) == 0x1fU);
  assert(bargraph_level_to_mask(6U) == 0x3fU);
  assert(bargraph_level_to_mask(99U) == 0x3fU);
}

static void test_filter_and_hysteresis(void)
{
  struct bargraph_state_s st;
  uint16_t t1;
  int i;

  /* Parameter validation */

  assert(bargraph_state_init(NULL, 1U, 4U, 20U) == -EINVAL);
  assert(bargraph_state_init(&st, 0U, 4U, 20U) == -EINVAL);
  assert(bargraph_state_init(&st, 5U, 4U, 20U) == -EINVAL);
  assert(bargraph_state_update(NULL, 100U) == -EINVAL);

  /* Direct pass-through (EMA 1/1) with 25-count hysteresis around t1 (586) */

  assert(bargraph_state_init(&st, 1U, 1U, 25U) == 0);
  t1 = bargraph_threshold_for_level(1U);
  assert(t1 == 586U);

  /* Start just below t1 -> level 0 */

  assert(bargraph_state_update(&st, (uint16_t)(t1 - 5U)) == 0);
  assert(st.level == 0U);
  assert(st.led_mask == 0x00U);

  /* Small noise above t1 but inside hysteresis (< t1 + 25) stays at level 0 */

  assert(bargraph_state_update(&st, (uint16_t)(t1 + 10U)) == 0);
  assert(st.level == 0U);

  /* Crossing upper hysteresis threshold (t1 + 25) transitions to level 1 */

  assert(bargraph_state_update(&st, (uint16_t)(t1 + 25U)) == 0);
  assert(st.level == 1U);
  assert(st.led_mask == 0x01U);

  /* Dropping slightly below t1 but above (t1 - 25) stays at level 1 */

  assert(bargraph_state_update(&st, (uint16_t)(t1 - 10U)) == 0);
  assert(st.level == 1U);

  /* Dropping below lower hysteresis threshold (< t1 - 25) returns to level 0 */

  assert(bargraph_state_update(&st, (uint16_t)(t1 - 26U)) == 0);
  assert(st.level == 0U);
  assert(st.led_mask == 0x00U);

  /* EMA smoothing (1/4) converges from 0 to full scale 4095 */

  assert(bargraph_state_init(&st,
                             BARGRAPH_DEFAULT_EMA_NUM,
                             BARGRAPH_DEFAULT_EMA_DEN,
                             BARGRAPH_DEFAULT_HYST_COUNTS) == 0);
  assert(bargraph_state_update(&st, 0U) == 0);
  assert(st.level == 0U);

  for (i = 0; i < 40; i++)
    {
      assert(bargraph_state_update(&st, 4095U) == 0);
    }

  assert(st.level == 6U);
  assert(st.led_mask == 0x3fU);
}

static void test_format_ascii(void)
{
  char buf[16];

  assert(bargraph_format_ascii(0U, NULL, sizeof(buf)) == -EINVAL);
  assert(bargraph_format_ascii(0U, buf, 8U) == -EINVAL);

  assert(bargraph_format_ascii(0U, buf, sizeof(buf)) == 0);
  assert(strcmp(buf, "[......]") == 0);

  assert(bargraph_format_ascii(3U, buf, sizeof(buf)) == 0);
  assert(strcmp(buf, "[###...]") == 0);

  assert(bargraph_format_ascii(6U, buf, sizeof(buf)) == 0);
  assert(strcmp(buf, "[######]") == 0);
}

int main(void)
{
  test_decode_rp23xx_sample();
  test_conversions();
  test_levels_and_thresholds();
  test_filter_and_hysteresis();
  test_format_ascii();

  printf("All xiao-rp2350-bargraph unit tests passed!\n");
  return 0;
}
