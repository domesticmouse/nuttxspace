/****************************************************************************
 * xiao-rp2350-bargraph/external/bargraph/bargraph_logic.h
 ****************************************************************************/

#ifndef __XIAO_RP2350_BARGRAPH_EXTERNAL_BARGRAPH_BARGRAPH_LOGIC_H
#define __XIAO_RP2350_BARGRAPH_EXTERNAL_BARGRAPH_BARGRAPH_LOGIC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define BARGRAPH_INTERVAL_US      50000
#define BARGRAPH_POLL_TIMEOUT_MS  10
#define BARGRAPH_FAULT_TICKS      5
#define BARGRAPH_FAULT_BLINK_US   250000
#define BARGRAPH_HYSTERESIS       24
#define BARGRAPH_NLEVELS          6

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

/* rp23xx_adc shifts valid 12-bit samples left by 19 bits into am_data and
 * negates them when RP23XX_ADC_FIFO_ERR is set.
 */

static inline int decode_adc_sample(int32_t am_data, uint16_t *raw_out)
{
  if (raw_out == NULL)
    {
      return -EINVAL;
    }

  if (am_data < 0)
    {
      return -EAGAIN;
    }

  *raw_out = (uint16_t)(am_data >> 19);
  return 0;
}

static inline int quantize_level(uint16_t raw, int current_level, int hyst)
{
  static const int k_thresholds[BARGRAPH_NLEVELS] =
  {
    585, 1170, 1755, 2340, 2925, 3510
  };

  int val = (int)raw;

  if (current_level < 0)
    {
      current_level = 0;
    }
  else if (current_level > BARGRAPH_NLEVELS)
    {
      current_level = BARGRAPH_NLEVELS;
    }

  if (hyst < 0)
    {
      hyst = 0;
    }

  while (current_level < BARGRAPH_NLEVELS &&
         val >= k_thresholds[current_level] + hyst)
    {
      current_level++;
    }

  while (current_level > 0 &&
         val < k_thresholds[current_level - 1] - hyst)
    {
      current_level--;
    }

  return current_level;
}

static inline uint32_t level_to_led_mask(int level)
{
  if (level <= 0)
    {
      return 0U;
    }

  if (level > BARGRAPH_NLEVELS)
    {
      level = BARGRAPH_NLEVELS;
    }

  return (1U << level) - 1U;
}

#endif /* __XIAO_RP2350_BARGRAPH_EXTERNAL_BARGRAPH_BARGRAPH_LOGIC_H */
