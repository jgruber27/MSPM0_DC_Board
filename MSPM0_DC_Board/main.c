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
#include "ad5672r.h"
#include "feedback_adc.h"
#include "terminal.h"

/* One application task owns commands, DAC updates, ADC scheduling and display.
 * UART reception remains interrupt-driven inside terminal.c.
 */
void *mainThread(void *arg0)
{
    (void)arg0;
    /* RSTSEL is grounded; program DAC0 zero before accepting input. */
    bool dacReady = AD5672R_init();
    if (dacReady) dacReady = AD5672R_write(0, 0);
    FeedbackADC_init();
    Terminal_init(dacReady);

    TickType_t lastADC = xTaskGetTickCount();
    for (;;) {
        bool redraw = Terminal_processInput();
        TickType_t now = xTaskGetTickCount();
        if ((TickType_t)(now - lastADC) >= pdMS_TO_TICKS(1000)) {
            lastADC += pdMS_TO_TICKS(1000);
            uint16_t raw = 0;
            uint32_t mv = 0;
            bool success = FeedbackADC_read(&raw, &mv);
            Terminal_setADC(success, raw, mv);
            if (!success) FeedbackADC_init();
            redraw = true;
        }
        if (redraw) Terminal_render();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
