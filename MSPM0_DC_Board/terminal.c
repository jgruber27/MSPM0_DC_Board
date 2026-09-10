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

#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "app_tasks.h"
#include "voltage_input.h"
#include "current_sense.h"
#include "terminal.h"
#include "ti_msp_dl_config.h"

#define RX_BUFFER_SIZE       128U
#define COMMAND_SIZE         24U

static volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
static volatile unsigned int rxHead, rxTail;
static volatile bool rxFault;
static char command[COMMAND_SIZE];
static unsigned int commandLength;
static bool discardCommand, previousWasCR, adcValid;
static uint32_t dacSetMillivolts;
static bool setpointPending;

/* No FreeRTOS calls in this ISR. Receive while ADC/terminal work is running. */
void UART0_IRQHandler(void)
{
    DL_UART_Main_clearInterruptStatus(UART0, DL_UART_MAIN_INTERRUPT_RX);
    while (!DL_UART_Main_isRXFIFOEmpty(UART0)) {
        /* Read once: RXDATA contains both the byte and its error flags. */
        uint32_t data = UART0->RXDATA;
        unsigned int next = (rxHead + 1U) % RX_BUFFER_SIZE;
        if ((data & (DL_UART_MAIN_ERROR_OVERRUN | DL_UART_MAIN_ERROR_BREAK |
                     DL_UART_MAIN_ERROR_PARITY | DL_UART_MAIN_ERROR_FRAMING)) ||
            next == rxTail) {
            rxFault = true;
        } else {
            rxBuffer[rxHead] = (uint8_t)data;
            rxHead = next;
        }
    }
}

/* -1: empty, -2: data lost (discard the pending line). */
static int terminalRead(void)
{
    int value = -1;
    taskENTER_CRITICAL();
    if (rxFault) {
        rxFault = false;
        rxTail = rxHead;
        value = -2;
    } else if (rxTail != rxHead) {
        value = rxBuffer[rxTail];
        rxTail = (rxTail + 1U) % RX_BUFFER_SIZE;
    }
    taskEXIT_CRITICAL();
    return value;
}

/* Inspect in the debugger if SPI initialization or a transfer fails. */
volatile bool dacError = false;
volatile bool adcError = false;
volatile uint16_t adcRaw[FEEDBACK_ADC_COUNT] = {0};
volatile uint32_t adcMillivolts[FEEDBACK_ADC_COUNT] = {0};

static void terminalInit(void)
{
    const DL_UART_Main_ClockConfig clock = {
        .clockSel = DL_UART_MAIN_CLOCK_BUSCLK,
        .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
    };
    const DL_UART_Main_Config uart = {
        .mode = DL_UART_MAIN_MODE_NORMAL,
        .direction = DL_UART_MAIN_DIRECTION_TX_RX,
        .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
        .parity = DL_UART_MAIN_PARITY_NONE,
        .wordLength = DL_UART_MAIN_WORD_LENGTH_8_BITS,
        .stopBits = DL_UART_MAIN_STOP_BITS_ONE
    };

    DL_UART_Main_reset(UART0);
    DL_UART_Main_enablePower(UART0);
    delay_cycles(POWER_STARTUP_DELAY);
    DL_UART_Main_setClockConfig(UART0, &clock);
    DL_UART_Main_init(UART0, &uart);
    DL_UART_Main_setOversampling(UART0, DL_UART_MAIN_OVERSAMPLING_RATE_16X);
    /* 32 MHz / (16 * 115200) = 17 + 23/64 (rounded). */
    DL_UART_Main_setBaudRateDivisor(UART0, 17, 23);
    DL_GPIO_initPeripheralOutputFunction(
        IOMUX_PINCM21, IOMUX_PINCM21_PF_UART0_TX); /* PA10 -> XDS110 UART */
    DL_GPIO_initPeripheralInputFunction(
        IOMUX_PINCM22, IOMUX_PINCM22_PF_UART0_RX); /* PA11 <- XDS110 UART */
    DL_UART_Main_enableFIFOs(UART0);
    DL_UART_Main_setRXFIFOThreshold(UART0, DL_UART_MAIN_RX_FIFO_LEVEL_ONE_ENTRY);
    DL_UART_Main_clearInterruptStatus(UART0, DL_UART_MAIN_INTERRUPT_RX);
    DL_UART_Main_enableInterrupt(UART0, DL_UART_MAIN_INTERRUPT_RX);
    NVIC_ClearPendingIRQ(UART0_INT_IRQn);
    NVIC_EnableIRQ(UART0_INT_IRQn);
    DL_UART_Main_enable(UART0);
}

/* Only the communications task writes the terminal. No semihosting/debugger required. */
static void terminalWrite(const char *text)
{
    while (*text != '\0') {
        DL_UART_Main_transmitDataBlocking(UART0, (uint8_t)*text++);
    }
    /* Finish transmission before the task sleeps. */
    while (DL_UART_Main_isBusy(UART0)) {
    }
}

/* ANSI erase-and-redraw preserves the partially typed command during updates. */
void Terminal_render(void)
{
    char line[192];
    char adcText[96];
    if (adcError) {
        (void)snprintf(adcText, sizeof(adcText), "PA27/PA26: ADC timeout");
    } else if (!adcValid) {
        (void)snprintf(adcText, sizeof(adcText), "PA27:-- PA26:--mV I:--mA");
    } else {
        const uint16_t senseRaw = adcRaw[FEEDBACK_ADC_PA26];
        const uint32_t microvolts = FeedbackADC_rawToMicrovolts(senseRaw);
        const uint32_t microamps = CurrentSense_rawToMicroamps(senseRaw);
        (void)snprintf(adcText, sizeof(adcText),
            "PA27:%lu.%03luV%s PA26:%lu.%03lumV%s I:%lu.%03lumA%s",
            (unsigned long)(adcMillivolts[FEEDBACK_ADC_PA27] / 1000U),
            (unsigned long)(adcMillivolts[FEEDBACK_ADC_PA27] % 1000U),
            adcRaw[FEEDBACK_ADC_PA27] == 4095U ? " FULL" : "",
            (unsigned long)(microvolts / 1000U),
            (unsigned long)(microvolts % 1000U),
            senseRaw == 4095U ? " FULL" : "",
            (unsigned long)(microamps / 1000U),
            (unsigned long)(microamps % 1000U),
            senseRaw == 4095U ? " SAT" : "");
    }
    (void)snprintf(line, sizeof(line),
        "\r\033[2K%s | DAC:%lu.%03luV%s | volts> %s%s",
        adcText, (unsigned long)(dacSetMillivolts / 1000U),
        (unsigned long)(dacSetMillivolts % 1000U), dacError ? " ERROR" : (setpointPending ? " pending" : ""),
        command, discardCommand ? " [invalid; press Enter]" : "");
    terminalWrite(line);
}

static void terminalMessage(const char *message)
{
    terminalWrite("\r\033[2K");
    terminalWrite(message);
    terminalWrite("\r\n");
}

static void submitCommand(void)
{
    uint32_t mv;
    char message[96];
    if (discardCommand || !voltageParse(command, DAC_COMMAND_MAX_MV, &mv)) {
        (void)snprintf(message, sizeof(message),
            "Enter 0 to %lu.%03lu volts, with at most 3 decimal places. Output unchanged.",
            (unsigned long)(DAC_COMMAND_MAX_MV / 1000U),
            (unsigned long)(DAC_COMMAND_MAX_MV % 1000U));
        terminalMessage(message);
    } else if (dacError) {
        terminalMessage("DAC unavailable: restart to reinitialize. Command not applied.");
    } else if (!App_submitSetpoint(0, mv)) {
        terminalMessage("Previous command pending. Wait for its result, then re-enter the voltage.");
    } else {
        setpointPending = true;
    }
    commandLength = 0;
    command[0] = '\0';
    discardCommand = false;
}

static void handleCharacter(int ch)
{
    if (ch == -2) {
        discardCommand = true;
        terminalMessage("Serial data lost. Press Enter, then retype the entire voltage.");
        return;
    }
    if (ch == '\n' && previousWasCR) {
        previousWasCR = false; /* Treat CRLF as one Enter. */
        return;
    }
    previousWasCR = (ch == '\r');
    if (ch == '\r' || ch == '\n') {
        if (commandLength != 0 || discardCommand) submitCommand();
    } else if (ch == 3 || ch == 21) { /* Ctrl-C or Ctrl-U: cancel the line. */
        commandLength = 0;
        command[0] = '\0';
        discardCommand = false;
    } else if (!discardCommand && (ch == '\b' || ch == 127)) {
        if (commandLength != 0) command[--commandLength] = '\0';
    } else if (!discardCommand) {
        if (ch < 32 || ch > 126 || commandLength == COMMAND_SIZE - 1U) {
            discardCommand = true; /* Never execute a truncated command. */
        } else {
            command[commandLength++] = (char)ch;
            command[commandLength] = '\0';
        }
    }
}

/* Communications task starts the UART after the control task initializes. */
void Terminal_init(bool dacReady)
{
    dacError = !dacReady;
    terminalInit();
    terminalWrite("\r\nDAC voltage control. Type volts and press Enter; local echo OFF.\r\n");
    char banner[96];
    (void)snprintf(banner, sizeof(banner),
        "Range: 0-%lu.%03lu V. Backspace edits; Ctrl-U clears. ADC updates every second.\r\n",
        (unsigned long)(DAC_COMMAND_MAX_MV / 1000U),
        (unsigned long)(DAC_COMMAND_MAX_MV % 1000U));
    terminalWrite(banner);
    if (dacError) terminalMessage("DAC initialization failed; output is unconfirmed.");
    Terminal_render();
}

bool Terminal_processInput(void)
{
    bool changed = false;
    /* Bound work so a continuous sender cannot starve the ADC schedule. */
    for (unsigned int i = 0; i < 32U; ++i) {
        int ch = terminalRead();
        if (ch == -1) break;
        handleCharacter(ch);
        changed = true;
    }
    return changed;
}

void Terminal_setADC(bool success, const uint16_t raw[FEEDBACK_ADC_COUNT],
    const uint32_t millivolts[FEEDBACK_ADC_COUNT])
{
    adcError = !success;
    adcValid = success;
    if (success) {
        for (unsigned int i = 0; i < FEEDBACK_ADC_COUNT; ++i) {
            adcRaw[i] = raw[i];
            adcMillivolts[i] = millivolts[i];
        }
    }
}

static bool terminalTakeResult(void)
{
    SetpointResult result;
    if (!App_takeResult(&result)) return false;
    setpointPending = false;
    dacError = !result.success;
    if (!result.success) {
        terminalMessage("SPI transfer failed; DAC output is unconfirmed. Restart to retry.");
    } else {
        char message[96];
        dacSetMillivolts = result.command.millivolts;
        (void)snprintf(message, sizeof(message),
            "DAC0 set to %lu.%03lu V (code %u)%s",
            (unsigned long)(dacSetMillivolts / 1000U),
            (unsigned long)(dacSetMillivolts % 1000U),
            (unsigned int)result.code, result.code == 4095U ? " [maximum code]" : "");
        terminalMessage(message);
    }
    return true;
}

void Terminal_task(void *arg)
{
    (void)arg;
    Measurement measurement;
    /* No spin or UART input before the control task has initialized the DAC. */
    if (xQueueReceive(appMeasurementQueue, &measurement, portMAX_DELAY) != pdTRUE) {
        vTaskDelete(NULL);
        return;
    }
    Terminal_init(measurement.dacReady);
    for (;;) {
        bool redraw = terminalTakeResult();
        redraw = Terminal_processInput() || redraw;
        if (xQueueReceive(appMeasurementQueue, &measurement, 0) == pdTRUE) {
            Terminal_setADC(measurement.adcValid, measurement.raw, measurement.millivolts);
            redraw = true;
        }
        if (redraw) Terminal_render();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
