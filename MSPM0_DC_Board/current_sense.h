#ifndef CURRENT_SENSE_H
#define CURRENT_SENSE_H

#include "feedback_adc.h"

#define CURRENT_SENSE_RATIO       5000U
#define CURRENT_SENSE_SHUNT_OHMS  1000U
#define CURRENT_SENSE_RF_OHMS     10000U
#define CURRENT_SENSE_RG_OHMS     890U

/* PA26 measures Vadc = (Iout / 5000) * 1000 * (1 + 10000 / 890).
 * Calculate Iout in microamps directly from the ADC code, rounding once.
 * uint64_t prevents overflow; this assumes ideal gain and zero offset.
 */
static inline uint32_t CurrentSense_rawToMicroamps(uint16_t raw)
{
    const uint64_t numerator = (uint64_t)raw * ADC_REFERENCE_MV * 1000U *
        CURRENT_SENSE_RATIO * CURRENT_SENSE_RG_OHMS;
    const uint64_t denominator = (uint64_t)4096U * CURRENT_SENSE_SHUNT_OHMS *
        (CURRENT_SENSE_RF_OHMS + CURRENT_SENSE_RG_OHMS);
    return (uint32_t)((numerator + denominator / 2U) / denominator);
}

#endif
