/**
 * Author: Michael Crum
 * Date: 11/7/2023
 * UART Driver for ATtiny1616, based on:
 * https://ww1.microchip.com/downloads/en/AppNotes/TB3216-Getting-Started-with-USART-90003216A.pdf
 */

#define USART0_BAUD_RATE(BAUD_RATE)                                            \
	((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "uart.h"

int USART0_printChar(char character, FILE* stream);
FILE USART_stream =
  FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

void USART0_init(void)
{
	PORTB.DIR &= ~PIN3_bm;
	PORTB.DIR |= PIN2_bm;

	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(115200);
	USART0.CTRLB |= USART_TXEN_bm;

	stdout = &USART_stream;
}

int USART0_printChar(char character, FILE* stream)
{
	while (!(USART0.STATUS & USART_DREIF_bm)) {
		;
	}
	USART0.TXDATAL = character;
	return 0;
}
