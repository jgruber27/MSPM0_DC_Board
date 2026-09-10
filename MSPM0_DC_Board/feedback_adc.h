#ifndef FEEDBACK_ADC_H
#define FEEDBACK_ADC_H

#include <stdbool.h>
#include <stdint.h>

#define FEEDBACK_ADC_COUNT 2U
#define FEEDBACK_ADC_PA27  0U
#define FEEDBACK_ADC_PA26  1U

/* MCU supply used as ADC reference; set to the measured supply in mV. */
#define ADC_REFERENCE_MV 3300U

/* Preserve sub-millivolt information for display (not additional ADC bits). */
static inline uint32_t FeedbackADC_rawToMicrovolts(uint16_t raw)
{
    return (uint32_t)(((uint64_t)raw * ADC_REFERENCE_MV * 1000U + 2048U) / 4096U);
}

/* Owns ADC0, PA27 (channel 0) and PA26 (channel 1). Control-task only. */
void FeedbackADC_init(void);

/* One bounded sequence: PA27 then PA26, with 125 us acquisition per input.
 * Output arrays must each hold FEEDBACK_ADC_COUNT elements in that order.
 * Returns false on timeout, leaving both arrays unchanged. Units are raw ADC code
 * and pin voltage in millivolts, using ADC_REFERENCE_MV above.
 */
bool FeedbackADC_read(uint16_t raw[FEEDBACK_ADC_COUNT],
    uint32_t millivolts[FEEDBACK_ADC_COUNT]);

#endif
