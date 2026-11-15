/****************************************************************************
 * xiao-rp2350-bargraph/include/bargraph_logic.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Pure, hardware-agnostic potentiometer ADC decoding, filtering, hysteresis,
 * and 6-LED bar-graph mapping logic shared between the NuttX firmware and
 * host-side unit tests.
 ****************************************************************************/

#ifndef __XIAO_RP2350_BARGRAPH_INCLUDE_BARGRAPH_LOGIC_H
#define __XIAO_RP2350_BARGRAPH_INCLUDE_BARGRAPH_LOGIC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Number of discrete LEDs in the breadboard bar graph */

#define BARGRAPH_NUM_LEDS             6U

/* Full-scale 12-bit ADC resolution on RP2350 (0..4095) */

#define BARGRAPH_ADC_BITS             12U
#define BARGRAPH_ADC_MAX              4095U
#define BARGRAPH_ADC_SPAN             4096U

/* Shift applied by nuttx/arch/arm/src/rp23xx/rp23xx_adc.c to 12-bit FIFO
 * readings so that the MSB aligns with bit 30 of signed int32_t.
 */

#define BARGRAPH_RP23XX_ADC_SHIFT     19U

/* Default analog reference voltage in millivolts (XIAO RP2350 3V3 rail) */

#define BARGRAPH_DEFAULT_VREF_MV      3300U

/* Default EMA filter smoothing weight (alpha = 1/4 = 0.25) and hysteresis
 * deadband in 12-bit ADC counts around each LED threshold boundary.
 */

#define BARGRAPH_DEFAULT_EMA_NUM      1U
#define BARGRAPH_DEFAULT_EMA_DEN      4U
#define BARGRAPH_DEFAULT_HYST_COUNTS  20U

/* Mask covering all 6 bar-graph LEDs (bits 0..5 = 0x3f) */

#define BARGRAPH_ALL_LEDS_MASK        ((1U << BARGRAPH_NUM_LEDS) - 1U)

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct bargraph_state_s
{
  bool     initialized;       /* True after first sample is processed */
  uint8_t  ema_num;           /* EMA numerator (1..ema_den) */
  uint8_t  ema_den;           /* EMA denominator (>= 1) */
  uint16_t hyst_counts;       /* Hysteresis deadband in 12-bit ADC counts */
  uint16_t raw_last;          /* Most recent unfiltered 12-bit sample */
  uint16_t raw_filtered;      /* EMA-filtered 12-bit ADC value */
  uint8_t  level;             /* Current bar-graph level (0..6 LEDs lit) */
  uint32_t led_mask;          /* Current LED bitmask (0x00..0x3f) */
  uint32_t sample_count;      /* Total valid samples processed */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************
 * Name: bargraph_decode_rp23xx_sample
 *
 * Description:
 *   Extract a 12-bit unsigned ADC value (0..4095) from the signed 32-bit
 *   sample word returned by the NuttX RP23xx ADC driver (/dev/adc0).
 *
 *   In NuttX's rp23xx_adc.c, the 12-bit hardware FIFO value is shifted left
 *   by 19 bits, and negated if the hardware FIFO conversion error flag is
 *   asserted. Unshifted values in [0, 4095] are also accepted for direct
 *   injection.
 *
 * Returned Value:
 *   0 on success; negated errno (-EINVAL or -EIO) on error.
 ****************************************************************************/

int bargraph_decode_rp23xx_sample(int32_t sample_data, uint16_t *raw12_out);

/****************************************************************************
 * Name: bargraph_raw12_to_millivolts
 *
 * Description:
 *   Convert a 12-bit ADC reading (0..4095) into millivolts (0..vref_mv)
 *   using nearest-integer rounding.
 ****************************************************************************/

uint32_t bargraph_raw12_to_millivolts(uint16_t raw12, uint32_t vref_mv);

/****************************************************************************
 * Name: bargraph_raw12_to_percent
 *
 * Description:
 *   Convert a 12-bit ADC reading (0..4095) into a percentage (0..100)
 *   using nearest-integer rounding.
 ****************************************************************************/

uint8_t bargraph_raw12_to_percent(uint16_t raw12);

/****************************************************************************
 * Name: bargraph_threshold_for_level
 *
 * Description:
 *   Return the nominal 12-bit ADC threshold at which the bar graph
 *   transitions from (level - 1) to (level), for level in 1..6.
 ****************************************************************************/

uint16_t bargraph_threshold_for_level(uint8_t level);

/****************************************************************************
 * Name: bargraph_raw12_to_level
 *
 * Description:
 *   Map a 12-bit ADC reading (0..4095) directly (without hysteresis) to a
 *   bar-graph level in [0, BARGRAPH_NUM_LEDS] (0..6).
 ****************************************************************************/

uint8_t bargraph_raw12_to_level(uint16_t raw12);

/****************************************************************************
 * Name: bargraph_level_to_mask
 *
 * Description:
 *   Convert a bar-graph level (0..6) to a contiguous bitmask of lit LEDs
 *   (0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f).
 ****************************************************************************/

uint32_t bargraph_level_to_mask(uint8_t level);

/****************************************************************************
 * Name: bargraph_state_init
 *
 * Description:
 *   Initialize a bar-graph filter & hysteresis state structure.
 *
 * Returned Value:
 *   0 on success; -EINVAL if arguments are invalid.
 ****************************************************************************/

int bargraph_state_init(struct bargraph_state_s *state,
                        uint8_t ema_num,
                        uint8_t ema_den,
                        uint16_t hyst_counts);

/****************************************************************************
 * Name: bargraph_state_update
 *
 * Description:
 *   Update the bar-graph filter & hysteresis state machine with a new
 *   12-bit ADC sample (0..4095).
 *
 * Returned Value:
 *   0 on success; -EINVAL if state is NULL.
 ****************************************************************************/

int bargraph_state_update(struct bargraph_state_s *state, uint16_t raw12);

/****************************************************************************
 * Name: bargraph_format_ascii
 *
 * Description:
 *   Render an ASCII representation of the bar graph, e.g. "[####..]", into
 *   buf (requires buflen >= BARGRAPH_NUM_LEDS + 3).
 *
 * Returned Value:
 *   0 on success; -EINVAL if buf is NULL or buflen is too small.
 ****************************************************************************/

int bargraph_format_ascii(uint8_t level, char *buf, size_t buflen);

#ifdef __cplusplus
}
#endif

#endif /* __XIAO_RP2350_BARGRAPH_INCLUDE_BARGRAPH_LOGIC_H */
