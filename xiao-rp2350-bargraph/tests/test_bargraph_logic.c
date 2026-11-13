/****************************************************************************
 * xiao-rp2350-bargraph/tests/test_bargraph_logic.c
 ****************************************************************************/

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

#include "bargraph_logic.h"

static void test_constants(void)
{
  assert(BARGRAPH_INTERVAL_US == 50000);
  assert(BARGRAPH_POLL_TIMEOUT_MS == 10);
  assert(BARGRAPH_FAULT_TICKS == 5);
  assert(BARGRAPH_FAULT_BLINK_US == 250000);
  assert(BARGRAPH_HYSTERESIS == 24);
  assert(BARGRAPH_NLEVELS == 6);
}

static void test_decode_adc_sample(void)
{
  uint16_t raw = 999;

  assert(decode_adc_sample(0, NULL) == -EINVAL);

  assert(decode_adc_sample(-1, &raw) == -EAGAIN);
  assert(raw == 999);
  assert(decode_adc_sample(-(1 << 19), &raw) == -EAGAIN);
  assert(raw == 999);
  assert(decode_adc_sample(-(4095 << 19), &raw) == -EAGAIN);
  assert(raw == 999);

  assert(decode_adc_sample(0, &raw) == 0);
  assert(raw == 0);

  assert(decode_adc_sample(1 << 19, &raw) == 0);
  assert(raw == 1);

  assert(decode_adc_sample(2048 << 19, &raw) == 0);
  assert(raw == 2048);

  assert(decode_adc_sample(4095 << 19, &raw) == 0);
  assert(raw == 4095);
}

static void test_quantize_level_thresholds_and_hysteresis(void)
{
  static const int thresholds[BARGRAPH_NLEVELS] =
  {
    585, 1170, 1755, 2340, 2925, 3510
  };

  /* Endpoints */

  assert(quantize_level(0, 0, BARGRAPH_HYSTERESIS) == 0);
  assert(quantize_level(4095, 6, BARGRAPH_HYSTERESIS) == 6);

  /* Single-step ascending and descending hysteresis bands */

  for (int i = 0; i < BARGRAPH_NLEVELS; i++)
    {
      uint16_t t = (uint16_t)thresholds[i];

      /* Below threshold + hyst stays at level i */

      assert(quantize_level((uint16_t)(t + BARGRAPH_HYSTERESIS - 1),
                            i, BARGRAPH_HYSTERESIS) == i);

      /* At threshold + hyst steps to level i + 1 */

      assert(quantize_level((uint16_t)(t + BARGRAPH_HYSTERESIS),
                            i, BARGRAPH_HYSTERESIS) == i + 1);

      /* Stationary jitter inside +-hyst around threshold does not drop */

      assert(quantize_level(t, i + 1, BARGRAPH_HYSTERESIS) == i + 1);
      assert(quantize_level((uint16_t)(t - BARGRAPH_HYSTERESIS),
                            i + 1, BARGRAPH_HYSTERESIS) == i + 1);

      /* Below threshold - hyst drops from i + 1 to i */

      assert(quantize_level((uint16_t)(t - BARGRAPH_HYSTERESIS - 1),
                            i + 1, BARGRAPH_HYSTERESIS) == i);
    }

  /* Multi-step single-tick jumps and out-of-range level clamping */

  assert(quantize_level(4095, 0, BARGRAPH_HYSTERESIS) == 6);
  assert(quantize_level(0, 6, BARGRAPH_HYSTERESIS) == 0);
  assert(quantize_level(2000, 0, BARGRAPH_HYSTERESIS) == 3);
  assert(quantize_level(1000, 6, BARGRAPH_HYSTERESIS) == 1);
  assert(quantize_level(0, -1, BARGRAPH_HYSTERESIS) == 0);
  assert(quantize_level(4095, 99, BARGRAPH_HYSTERESIS) == 6);
  assert(quantize_level(100, 1, 600) == 1);
}

static void test_level_to_led_mask(void)
{
  assert(level_to_led_mask(-1) == 0x00U);
  assert(level_to_led_mask(0) == 0x00U);
  assert(level_to_led_mask(1) == 0x01U);
  assert(level_to_led_mask(2) == 0x03U);
  assert(level_to_led_mask(3) == 0x07U);
  assert(level_to_led_mask(4) == 0x0fU);
  assert(level_to_led_mask(5) == 0x1fU);
  assert(level_to_led_mask(6) == 0x3fU);
  assert(level_to_led_mask(7) == 0x3fU);
}

int main(void)
{
  test_constants();
  test_decode_adc_sample();
  test_quantize_level_thresholds_and_hysteresis();
  test_level_to_led_mask();

  printf("PASS: test_bargraph_logic (all assertions succeeded)\n");
  return 0;
}
