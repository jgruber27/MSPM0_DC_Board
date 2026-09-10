#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>
#include <stdint.h>
#include "feedback_adc.h"

/* Only the communications task owns these functions; UART RX is buffered by UART0_IRQHandler.
 * Initialize once after receiving the control task startup report. Owns UART0, PA10 and PA11.
 */
void Terminal_init(bool dacReady);
void Terminal_task(void *arg);

/* Process a bounded batch of received bytes; valid commands enqueue a DAC0 setpoint.
 * Returns true if the prompt needs redrawing. Call regularly from the owner.
 */
bool Terminal_processInput(void);

/* Cache the latest ADC result for display. Failure displays an error. */
void Terminal_setADC(bool success, const uint16_t raw[FEEDBACK_ADC_COUNT],
    const uint32_t millivolts[FEEDBACK_ADC_COUNT]);
void Terminal_render(void);

#endif
