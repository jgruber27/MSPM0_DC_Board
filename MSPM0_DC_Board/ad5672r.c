#include "ad5672r.h"
#include "ti_msp_dl_config.h"

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#define DAC_SYNC_PIN  DL_GPIO_PIN_13
#define DAC_RESET_PIN DL_GPIO_PIN_7
#define SPI_POLL_LIMIT 32000U

static StaticSemaphore_t mutexStorage;
static SemaphoreHandle_t dacMutex;
static bool initialized;

/* Mutex must be held. Three bytes, MSB first, with no /SYNC gap. */
static bool writeFrame(uint8_t command, uint8_t address, uint16_t data)
{
    const uint8_t bytes[3] = {
        (uint8_t)((command << 4) | address),
        (uint8_t)(data >> 8), (uint8_t)data
    };
    bool success = true;

    DL_GPIO_clearPins(GPIOA, DAC_SYNC_PIN);
    delay_cycles(32); /* >= 1 us at the configured 32 MHz MCLK. */
    for (unsigned int i = 0; i < 3; ++i) {
        uint32_t remaining = SPI_POLL_LIMIT;
        DL_SPI_transmitData8(SPI1, bytes[i]);
        while (DL_SPI_isRXFIFOEmpty(SPI1)) {
            if (--remaining == 0) {
                success = false;
                goto finish;
            }
        }
        /* Drain receive data even for writes; SDO is unused by this example. */
        (void)DL_SPI_receiveData8(SPI1);
    }
    {
        uint32_t remaining = SPI_POLL_LIMIT;
        while (DL_SPI_isBusy(SPI1)) {
            if (--remaining == 0) {
                success = false;
                break;
            }
        }
    }
finish:
    if (!success) {
        DL_SPI_disable(SPI1);
        initialized = false; /* Reinitialize before retrying a failed bus. */
    }
    delay_cycles(32);
    DL_GPIO_setPins(GPIOA, DAC_SYNC_PIN);
    delay_cycles(32);
    return success;
}

bool AD5672R_init(void)
{
    const DL_SPI_ClockConfig clock = {
        .clockSel = DL_SPI_CLOCK_BUSCLK,
        .divideRatio = DL_SPI_CLOCK_DIVIDE_RATIO_1
    };
    const DL_SPI_Config spi = {
        .mode = DL_SPI_MODE_CONTROLLER,
        /* SPI mode 1: idle low, DAC captures data on falling SCLK edges.
         * MOTO3 leaves chip select under GPIO control.
         */
        .frameFormat = DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA1,
        .parity = DL_SPI_PARITY_NONE,
        .dataSize = DL_SPI_DATA_SIZE_8,
        .bitOrder = DL_SPI_BIT_ORDER_MSB_FIRST,
        .chipSelectPin = DL_SPI_CHIP_SELECT_NONE
    };

    /* First initialization must finish before other tasks use this driver. */
    if (dacMutex == NULL) {
        dacMutex = xSemaphoreCreateMutexStatic(&mutexStorage);
    }
    if (dacMutex == NULL || xSemaphoreTake(dacMutex, portMAX_DELAY) != pdTRUE) {
        return false;
    }
    initialized = false;
    /* GPIO banks were powered by SYSCFG_DL_init(); do not reset them here. */
    DL_GPIO_setPins(GPIOA, DAC_SYNC_PIN | DAC_RESET_PIN);
    DL_GPIO_initDigitalOutput(IOMUX_PINCM35); /* PA13: /SYNC */
    DL_GPIO_initDigitalOutput(IOMUX_PINCM14); /* PA7: /RESET */
    DL_GPIO_enableOutput(GPIOA, DAC_SYNC_PIN | DAC_RESET_PIN);

    DL_SPI_reset(SPI1);
    DL_SPI_enablePower(SPI1);
    delay_cycles(POWER_STARTUP_DELAY);
    DL_SPI_setClockConfig(SPI1, &clock);
    DL_SPI_init(SPI1, &spi);
    /* BUSCLK = 32 MHz: SCLK = BUSCLK / (2 * (15 + 1)) = 1 MHz. */
    DL_SPI_setBitRateSerialClockDivider(SPI1, 15);
    DL_GPIO_initPeripheralOutputFunction(
        IOMUX_PINCM26, IOMUX_PINCM26_PF_SPI1_SCLK); /* PB9 */
    DL_GPIO_initPeripheralOutputFunction(
        IOMUX_PINCM40, IOMUX_PINCM40_PF_SPI1_PICO); /* PA18 -> SDI */
    DL_GPIO_initPeripheralInputFunction(
        IOMUX_PINCM24, IOMUX_PINCM24_PF_SPI1_POCI); /* PB7 <- SDO */
    DL_SPI_enable(SPI1);

    /* Allow power-on initialization, then pulse /RESET for 10 us. */
    vTaskDelay(pdMS_TO_TICKS(10) + 1);
    DL_GPIO_clearPins(GPIOA, DAC_RESET_PIN);
    delay_cycles(320);
    DL_GPIO_setPins(GPIOA, DAC_RESET_PIN);
    vTaskDelay(pdMS_TO_TICKS(10) + 1);

    /* Command 7, DB0=0 enables the internal reference (not DB0=1). */
    initialized = writeFrame(0x7, 0, 0x0000);
    vTaskDelay(pdMS_TO_TICKS(10) + 1);
    bool success = initialized;
    (void)xSemaphoreGive(dacMutex);
    return success;
}

bool AD5672R_write(uint8_t channel, uint16_t code)
{
    if (channel > 7 || code > 4095 || dacMutex == NULL) {
        return false;
    }
    if (xSemaphoreTake(dacMutex, portMAX_DELAY) != pdTRUE) {
        return false;
    }
    /* Command 3: write and update. AD5672R data occupy DB15:DB4. */
    bool success = initialized && writeFrame(0x3, channel, (uint16_t)(code << 4));
    (void)xSemaphoreGive(dacMutex);
    return success;
}
