#include <avr/io.h>
// F_CPU frequency is defined in the makefile
#include <util/delay.h>

#include "protocol_drivers/spi/spi.h"
#include "protocol_drivers/uart/uart.h"

int main(void)
{
	// Disable clock division
	CCP = CCP_IOREG_gc;
	CLKCTRL_MCLKCTRLB &= ~(1);

	uint8_t num_leds = 32;
	uint8_t start[] = { 0x00, 0x00, 0x00, 0x00 };
	uint8_t stop[] = { 0xFF, 0xFF, 0xFF, 0xFF };
	SPI0_init();

	SPIDevice apa102Disp = SPIDevice(
	  (pin_t){ .port = (volatile uint8_t*)(&PORTA), .pin_mask = PIN0_bm });

	unsigned int n = 0;

	while (1) {
		apa102Disp.write_data(start, 4);
		for (uint8_t i = 0; i < num_leds; i++) {
			apa102Disp.write_byte(0b11100001);
			for (int j = 0; j < 3; j++) {
				// uint8_t val = (255.0 / 2.0) * (1 + cos(n / 30.0 + j + (i
				// / 32.0) + (i % 8) / 3.0));
				if (n == 0)
					apa102Disp.write_byte(0xFF);
				else
					apa102Disp.write_byte(0x00);
				//}
			}
		}
		apa102Disp.write_data(stop, 4);
		n += 1;
		n = n % 3000;
	}
}