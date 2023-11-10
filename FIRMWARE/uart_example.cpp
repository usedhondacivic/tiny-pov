#include <avr/io.h>
// F_CPU frequency is defined in the makefile
#include <util/delay.h>

#include "protocol_drivers/uart/uart.h"

// LED is on pin 0 of PORTC
#define LED 0
#define DELAY_MS 500

int
main(void)
{
	// Disable clock prescaler
	CCP = CCP_IOREG_gc;
	CLKCTRL_MCLKCTRLB &= ~(1);

	USART0_init();

	PORTC_DIR |= PIN0_bm;
	PORTC_OUT &= ~PIN0_bm;

	uint8_t count = 0;
	while (1) {
		PORTC_OUT ^= ~PIN0_bm;
		printf("Counter value is: %d\r\n", count++);
		_delay_ms(500);
	}
}