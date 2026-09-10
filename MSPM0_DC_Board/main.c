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

#include <FreeRTOS.h>
#include <task.h>
#include "app_tasks.h"
#include "ad5672r.h"
#include "feedback_adc.h"
#include "voltage_input.h"

/* Higher-priority control task: sole owner of SPI/DAC and ADC hardware. */
void mainThread(void *arg0)
{
    (void)arg0;
    Measurement measurement = {0};
    measurement.dacReady = AD5672R_init();
    if (measurement.dacReady) measurement.dacReady = AD5672R_write(0, 0);
    FeedbackADC_init();
    /* Releases the console only after zero-output initialization completes. */
    xQueueOverwrite(appMeasurementQueue, &measurement);

    const TickType_t period = pdMS_TO_TICKS(1000);
    TickType_t lastADC = xTaskGetTickCount();
    for (;;) {
        TickType_t now = xTaskGetTickCount();
        if ((TickType_t)(now - lastADC) >= period) {
            /* Skip missed periods rather than burst-sampling after a long halt. */
            lastADC += ((TickType_t)(now - lastADC) / period) * period;
            measurement.adcValid = FeedbackADC_read(measurement.raw, measurement.millivolts);
            measurement.adcError = !measurement.adcValid;
            if (!measurement.adcValid) {
                FeedbackADC_init();
            }
            /* Never wait for a slow terminal; it receives the latest sample. */
            xQueueOverwrite(appMeasurementQueue, &measurement);
        }

        now = xTaskGetTickCount();
        TickType_t elapsed = now - lastADC;
        TickType_t wait = elapsed < period ? period - elapsed : 0;
        SetpointCommand command;
        if (xQueueReceive(appCommandQueue, &command, wait) == pdTRUE) {
            SetpointResult result = {.command = command, .code = 0, .success = false};
            /* Validate again at the hardware boundary. Only channel 0 exists now. */
            if (measurement.dacReady && command.channel == 0 &&
                command.millivolts <= DAC_COMMAND_MAX_MV) {
                result.code = voltageToCode(command.millivolts, DAC_FULL_SCALE_MV);
                result.success = AD5672R_write(command.channel, result.code);
                if (!result.success) measurement.dacReady = false;
            }
            /* App_submitSetpoint permits one outstanding request. The reply
             * slot is therefore free; publishing never blocks ADC scheduling.
             */
            xQueueOverwrite(appResultQueue, &result);
        }
    }
}
