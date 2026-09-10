#ifndef APP_TASKS_H
#define APP_TASKS_H

#include <stdbool.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <queue.h>
#include "feedback_adc.h"

/* Match the current DAC: GAIN grounded, 2.5 V internal reference. */
#define DAC_FULL_SCALE_MV 2500U
#define DAC_COMMAND_MAX_MV 2500U

typedef struct {
    uint8_t channel;
    uint32_t millivolts;
} SetpointCommand;

typedef struct {
    SetpointCommand command;
    uint16_t code;
    bool success;
} SetpointResult;

typedef struct {
    bool dacReady;
    bool adcValid;
    bool adcError;
    uint16_t raw[FEEDBACK_ADC_COUNT];
    uint32_t millivolts[FEEDBACK_ADC_COUNT];
} Measurement;

/* Created before scheduling. Control task receives commands, sends results,
 * and overwrites the one-item measurement mailbox with the latest sample.
 * These queue handles never change after AppTasks_init().
 */
extern QueueHandle_t appCommandQueue;
extern QueueHandle_t appResultQueue;
extern QueueHandle_t appMeasurementQueue;
bool AppTasks_init(void);

/* Communications-task-only API. One command may be outstanding at a time.
 * A successful submit means queued, not applied. Taking its result releases
 * the slot. This guarantees room for the control task's nonblocking reply.
 */
bool App_submitSetpoint(uint8_t channel, uint32_t millivolts);
bool App_takeResult(SetpointResult *result);

void mainThread(void *arg);

#endif
