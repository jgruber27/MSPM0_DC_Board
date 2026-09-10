#ifndef FEEDBACK_ADC_H
#define FEEDBACK_ADC_H

#include <stdbool.h>
#include <stdint.h>

/* Owns ADC0 and PA27. Call after SYSCFG_DL_init(), from the application task. */
void FeedbackADC_init(void);

/* Single-task, bounded blocking conversion. Both output pointers must be valid.
 * Returns false on timeout, leaving outputs unchanged. Units are raw ADC code
 * and pin voltage in millivolts, using ADC_REFERENCE_MV in feedback_adc.c.
 */
bool FeedbackADC_read(uint16_t *raw, uint32_t *millivolts);

#endif
