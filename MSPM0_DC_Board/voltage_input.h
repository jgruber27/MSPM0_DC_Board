#ifndef VOLTAGE_INPUT_H
#define VOLTAGE_INPUT_H

#include <stdbool.h>
#include <stdint.h>

/* Decimal volts only, up to three fractional digits; no signs/exponents.
 * Reject the entire command on error; leave the output argument unchanged.
 */
static inline bool voltageParse(const char *text, uint32_t limitMv, uint32_t *mv)
{
    uint32_t whole = 0, fraction = 0, scale = 100;
    bool digitSeen = false;
    while (*text == ' ' || *text == '\t') ++text;
    while (*text >= '0' && *text <= '9') {
        digitSeen = true;
        if (whole > limitMv / 1000U) return false;
        whole = whole * 10U + (uint32_t)(*text++ - '0');
    }
    if (*text == '.') {
        ++text;
        while (*text >= '0' && *text <= '9') {
            digitSeen = true;
            if (scale == 0) return false;
            fraction += (uint32_t)(*text++ - '0') * scale;
            scale /= 10U;
        }
    }
    while (*text == ' ' || *text == '\t') ++text;
    uint32_t value = whole * 1000U + fraction;
    if (!digitSeen || *text != '\0' || value > limitMv) return false;
    *mv = value;
    return true;
}

static inline uint16_t voltageToCode(uint32_t mv, uint32_t fullScaleMv)
{
    uint32_t code = (mv * 4096U + fullScaleMv / 2U) / fullScaleMv;
    return (uint16_t)(code > 4095U ? 4095U : code);
}

#endif
