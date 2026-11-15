/****************************************************************************
 * xiao-rp2350-bargraph/src/bargraph_logic.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Pure, hardware-agnostic potentiometer ADC decoding, filtering, hysteresis,
 * and 6-LED bar-graph mapping logic shared between the NuttX firmware and
 * host-side unit tests.
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "bargraph_logic.h"

#include <errno.h>
#include <string.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static uint16_t clamp_raw12(uint16_t raw12)
{
  if (raw12 > BARGRAPH_ADC_MAX)
    {
      return BARGRAPH_ADC_MAX;
    }

  return raw12;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int bargraph_decode_rp23xx_sample(int32_t sample_data, uint16_t *raw12_out)
{
  uint32_t raw;

  if (raw12_out == NULL)
    {
      return -EINVAL;
    }

  /* rp23xx_adc.c negates the sample when RP23XX_ADC_FIFO_ERR is set */

  if (sample_data < 0)
    {
      return -EIO;
    }

  /* rp23xx_adc.c shifts the 12-bit unsigned FIFO value left by 19 bits.
   * Any non-zero shifted sample is >= (1 << 19) = 524288 > 4095.
   * If sample_data is in [1, 4095], accept it as an already-unshifted
   * 12-bit value for convenience in direct tests/simulations.
   */

  if ((uint32_t)sample_data > BARGRAPH_ADC_MAX)
    {
      raw = ((uint32_t)sample_data >> BARGRAPH_RP23XX_ADC_SHIFT);
      if (raw > BARGRAPH_ADC_MAX)
        {
          raw = BARGRAPH_ADC_MAX;
        }
    }
  else
    {
      raw = (uint32_t)sample_data;
    }

  *raw12_out = (uint16_t)raw;
  return 0;
}

uint32_t bargraph_raw12_to_millivolts(uint16_t raw12, uint32_t vref_mv)
{
  uint16_t clamped = clamp_raw12(raw12);

  return ((uint32_t)clamped * vref_mv + (BARGRAPH_ADC_MAX / 2U)) /
         BARGRAPH_ADC_MAX;
}

uint8_t bargraph_raw12_to_percent(uint16_t raw12)
{
  uint16_t clamped = clamp_raw12(raw12);

  return (uint8_t)(((uint32_t)clamped * 100U + (BARGRAPH_ADC_MAX / 2U)) /
                   BARGRAPH_ADC_MAX);
}

uint16_t bargraph_threshold_for_level(uint8_t level)
{
  if (level == 0U)
    {
      return 0U;
    }

  if (level > BARGRAPH_NUM_LEDS)
    {
      return BARGRAPH_ADC_MAX;
    }

  /* Smallest raw12 where ((raw12 * 7) / 4096) >= level:
   * ceil((level * 4096) / 7) = (level * 4096 + 6) / 7
   */

  return (uint16_t)(((uint32_t)level * BARGRAPH_ADC_SPAN +
                     BARGRAPH_NUM_LEDS) /
                    (BARGRAPH_NUM_LEDS + 1U));
}

uint8_t bargraph_raw12_to_level(uint16_t raw12)
{
  uint16_t clamped = clamp_raw12(raw12);
  uint32_t level;

  level = ((uint32_t)clamped * (BARGRAPH_NUM_LEDS + 1U)) / BARGRAPH_ADC_SPAN;
  if (level > BARGRAPH_NUM_LEDS)
    {
      level = BARGRAPH_NUM_LEDS;
    }

  return (uint8_t)level;
}

uint32_t bargraph_level_to_mask(uint8_t level)
{
  if (level == 0U)
    {
      return 0U;
    }

  if (level >= BARGRAPH_NUM_LEDS)
    {
      return BARGRAPH_ALL_LEDS_MASK;
    }

  return (1U << level) - 1U;
}

int bargraph_state_init(struct bargraph_state_s *state,
                        uint8_t ema_num,
                        uint8_t ema_den,
                        uint16_t hyst_counts)
{
  if (state == NULL || ema_num == 0U || ema_den == 0U || ema_num > ema_den)
    {
      return -EINVAL;
    }

  memset(state, 0, sizeof(*state));
  state->ema_num     = ema_num;
  state->ema_den     = ema_den;
  state->hyst_counts = hyst_counts;
  return 0;
}

int bargraph_state_update(struct bargraph_state_s *state, uint16_t raw12)
{
  uint16_t clamped;
  uint8_t  lvl;

  if (state == NULL || state->ema_den == 0U || state->ema_num == 0U)
    {
      return -EINVAL;
    }

  clamped = clamp_raw12(raw12);
  state->raw_last = clamped;

  if (!state->initialized)
    {
      state->raw_filtered = clamped;
      state->level        = bargraph_raw12_to_level(clamped);
      state->led_mask     = bargraph_level_to_mask(state->level);
      state->initialized  = true;
      state->sample_count = 1U;
      return 0;
    }

  /* Integer Exponential Moving Average (EMA) with rounding:
   * filtered = (ema_num * new + (ema_den - ema_num) * old + ema_den/2) / ema_den
   */

  state->raw_filtered = (uint16_t)(
    ((uint32_t)state->ema_num * (uint32_t)clamped +
     (uint32_t)(state->ema_den - state->ema_num) *
       (uint32_t)state->raw_filtered +
     ((uint32_t)state->ema_den / 2U)) /
    (uint32_t)state->ema_den);

  lvl = state->level;

  /* Step upward if filtered reading exceeds upper threshold + hysteresis */

  while (lvl < BARGRAPH_NUM_LEDS)
    {
      uint32_t up_thresh = (uint32_t)bargraph_threshold_for_level(lvl + 1U) +
                           (uint32_t)state->hyst_counts;
      if (up_thresh > BARGRAPH_ADC_MAX)
        {
          up_thresh = BARGRAPH_ADC_MAX;
        }

      if (state->raw_filtered >= up_thresh)
        {
          lvl++;
        }
      else
        {
          break;
        }
    }

  /* Step downward if filtered reading falls below lower threshold - hysteresis */

  while (lvl > 0U)
    {
      uint16_t nominal = bargraph_threshold_for_level(lvl);
      uint16_t down_thresh = (nominal > state->hyst_counts) ?
                             (uint16_t)(nominal - state->hyst_counts) : 0U;

      if (state->raw_filtered < down_thresh)
        {
          lvl--;
        }
      else
        {
          break;
        }
    }

  state->level    = lvl;
  state->led_mask = bargraph_level_to_mask(lvl);
  state->sample_count++;
  return 0;
}

int bargraph_format_ascii(uint8_t level, char *buf, size_t buflen)
{
  uint8_t i;

  if (buf == NULL || buflen < (BARGRAPH_NUM_LEDS + 3U))
    {
      return -EINVAL;
    }

  if (level > BARGRAPH_NUM_LEDS)
    {
      level = BARGRAPH_NUM_LEDS;
    }

  buf[0] = '[';
  for (i = 0U; i < BARGRAPH_NUM_LEDS; i++)
    {
      buf[i + 1U] = (i < level) ? '#' : '.';
    }

  buf[BARGRAPH_NUM_LEDS + 1U] = ']';
  buf[BARGRAPH_NUM_LEDS + 2U] = '\0';
  return 0;
}
