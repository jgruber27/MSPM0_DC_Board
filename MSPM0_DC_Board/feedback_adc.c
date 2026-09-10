/*
 * Copyright (c) 2018-2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "feedback_adc.h"
#include "ti_msp_dl_config.h"

/* ADC uses the MCU supply as reference. Set to the measured supply in mV.
 * PA27 and PA26 must stay between GND and the MCU supply (3.3 V).
 */
#define ADC_POLL_LIMIT   32000U

void FeedbackADC_init(void)
{
    const DL_ADC12_ClockConfig clock = {
        .clockSel = DL_ADC12_CLOCK_ULPCLK,
        .freqRange = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
        .divideRatio = DL_ADC12_CLOCK_DIVIDE_8
    };

    /* PA27 / PINCM60 = ADC0.0; PA26 / PINCM59 = ADC0.1. */
    DL_GPIO_disableOutput(GPIOA, DL_GPIO_PIN_27 | DL_GPIO_PIN_26);
    DL_GPIO_initPeripheralAnalogFunction(IOMUX_PINCM60);
    DL_GPIO_initPeripheralAnalogFunction(IOMUX_PINCM59);
    DL_ADC12_reset(ADC0);
    DL_ADC12_enablePower(ADC0);
    delay_cycles(POWER_STARTUP_DELAY);
    DL_ADC12_setClockConfig(ADC0, &clock);
    DL_ADC12_initSeqSample(ADC0, DL_ADC12_REPEAT_MODE_DISABLED,
        DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_SOFTWARE,
        DL_ADC12_SEQ_START_ADDR_00, DL_ADC12_SEQ_END_ADDR_01,
        DL_ADC12_SAMP_CONV_RES_12_BIT, DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);
    DL_ADC12_configConversionMem(ADC0, DL_ADC12_MEM_IDX_0,
        DL_ADC12_INPUT_CHAN_0, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
    DL_ADC12_configConversionMem(ADC0, DL_ADC12_MEM_IDX_1,
        DL_ADC12_INPUT_CHAN_1, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
    /* 500 * 8 / 32 MHz = 125 us acquisition time. */
    DL_ADC12_setSampleTime0(ADC0, 500);
    DL_ADC12_setPowerDownMode(ADC0, DL_ADC12_POWER_DOWN_MODE_MANUAL);
    DL_ADC12_enableConversions(ADC0);
    delay_cycles(320); /* Allow the analog circuitry to wake up. */
}

/* Publish neither result until the entire two-input sequence completes. */
bool FeedbackADC_read(uint16_t raw[FEEDBACK_ADC_COUNT],
    uint32_t millivolts[FEEDBACK_ADC_COUNT])
{
    uint32_t remaining = ADC_POLL_LIMIT;
    DL_ADC12_clearInterruptStatus(ADC0,
        DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED | DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED);
    DL_ADC12_enableConversions(ADC0);
    DL_ADC12_startConversion(ADC0);
    while (DL_ADC12_getRawInterruptStatus(
               ADC0, DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED) == 0) {
        if (--remaining == 0) {
            return false;
        }
    }
    raw[FEEDBACK_ADC_PA27] = (uint16_t)DL_ADC12_getMemResult(ADC0, DL_ADC12_MEM_IDX_0);
    raw[FEEDBACK_ADC_PA26] = (uint16_t)DL_ADC12_getMemResult(ADC0, DL_ADC12_MEM_IDX_1);
    /* 12-bit ADC: one LSB = VREF / 4096. Round to nearest millivolt. */
    for (unsigned int i = 0; i < FEEDBACK_ADC_COUNT; ++i) {
        millivolts[i] = ((uint32_t)raw[i] * ADC_REFERENCE_MV + 2048U) / 4096U;
    }
    return true;
}
