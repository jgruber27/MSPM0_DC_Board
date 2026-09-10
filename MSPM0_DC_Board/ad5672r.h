#ifndef AD5672R_H
#define AD5672R_H

#include <stdbool.h>
#include <stdint.h>

/* Call once from a task after SYSCFG_DL_init(), before using write().
 * Owns SPI1, PA13 (/SYNC), PA7 (/RESET), PB9 (SCLK), PA18 (SDI), PB7 (SDO).
 * Enables the internal 2.5 V reference. TSSOP gain is selected by GAIN pin.
 */
bool AD5672R_init(void);

/* Task-only, serialized blocking write: channel 0..7, code 0..4095.
 * Updates the output immediately, independently of /LDAC.
 * Returns false for invalid arguments or an SPI peripheral timeout.
 * Successful transmission does not confirm that a DAC is connected.
 */
bool AD5672R_write(uint8_t channel, uint16_t code);

#endif
