#include "app_tasks.h"
#include "terminal.h"
#include <task.h>

QueueHandle_t appCommandQueue;
QueueHandle_t appResultQueue;
QueueHandle_t appMeasurementQueue;

static StaticQueue_t commandQueueStorage, resultQueueStorage, measurementQueueStorage;
static uint8_t commandStorage[sizeof(SetpointCommand)];
static uint8_t resultStorage[sizeof(SetpointResult)];
static uint8_t measurementStorage[sizeof(Measurement)];
static StaticTask_t controlTaskStorage, communicationsTaskStorage;
/* FreeRTOS stack depths are in StackType_t words (4 bytes on this MCU). */
static StackType_t controlStack[256];
static StackType_t communicationsStack[512];
/* Accessed exclusively by the communications task. */
static bool requestPending;

bool AppTasks_init(void)
{
    appCommandQueue = xQueueCreateStatic(1, sizeof(SetpointCommand),
        commandStorage, &commandQueueStorage);
    appResultQueue = xQueueCreateStatic(1, sizeof(SetpointResult),
        resultStorage, &resultQueueStorage);
    appMeasurementQueue = xQueueCreateStatic(1, sizeof(Measurement),
        measurementStorage, &measurementQueueStorage);
    if (!appCommandQueue || !appResultQueue || !appMeasurementQueue) return false;

    TaskHandle_t control = xTaskCreateStatic(mainThread, "control", 256, NULL,
        tskIDLE_PRIORITY + 2, controlStack, &controlTaskStorage);
    TaskHandle_t communications = xTaskCreateStatic(Terminal_task, "console", 512,
        NULL, tskIDLE_PRIORITY + 1, communicationsStack, &communicationsTaskStorage);
    return control != NULL && communications != NULL;
}

bool App_submitSetpoint(uint8_t channel, uint32_t millivolts)
{
    if (requestPending || channel != 0 || millivolts > DAC_COMMAND_MAX_MV) return false;
    SetpointCommand command = {.channel = channel, .millivolts = millivolts};
    requestPending = true;
    if (xQueueSend(appCommandQueue, &command, 0) != pdTRUE) {
        requestPending = false;
        return false;
    }
    return true;
}

bool App_takeResult(SetpointResult *result)
{
    if (xQueueReceive(appResultQueue, result, 0) != pdTRUE) return false;
    requestPending = false;
    return true;
}
