#include <avr/io.h>
#include <avr/power.h>
// F_CPU frequency is defined in the makefile
#include <util/delay.h>

#include "protocol_drivers/sd/sd.h"
#include "protocol_drivers/spi/spi.h"
#include "protocol_drivers/uart/uart.h"

int main(void)
{
	// Disable clock division
	CCP = CCP_IOREG_gc;
	CLKCTRL_MCLKCTRLB &= ~(1);

	SPI0_init();

	SPIDevice apa102Disp = SPIDevice(
	  (pin_t){ .port = (volatile uint8_t*)(&PORTA), .pin_mask = PIN0_bm });

	while (1) {
	}
}