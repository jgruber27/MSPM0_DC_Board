#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>
#include <stdint.h>

/* Single task owns these functions; UART RX is buffered by UART0_IRQHandler.
 * Initialize once after programming DAC0 to zero. Owns UART0, PA10 and PA11.
 */
void Terminal_init(bool dacReady);

/* Process a bounded batch of received bytes; valid commands update DAC0.
 * Returns true if the prompt needs redrawing. Call regularly from the owner.
 */
bool Terminal_processInput(void);

/* Cache the latest ADC result for display. Failure displays an error. */
void Terminal_setADC(bool success, uint16_t raw, uint32_t millivolts);
void Terminal_render(void);

#endif
