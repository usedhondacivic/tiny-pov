/**
 * Author: Michael Crum
 * Date: 12/28/2023
 *
 * Entry point for Tiny POV
 *
 * https://github.com/usedhondacivic/tiny-pov
 * michael-crum.com/tiny-pov
 */

#include <stdbool.h>
#include <stdint.h>

#include "drivers/apa102/apa102.h"
#include "drivers/gpio/gpio.h"
#include "util.h"

int main(void)
{
	init_apa102(64);
	uint8_t all_white[64 * 3];
	for (int i = 0; i < 64 * 3; i++) {
		all_white[i] = 0xFF;
	}
	write_strip(all_white);

	uint16_t led = PIN('A', 8);
	gpio_set_mode(led, GPIO_MODE_OUTPUT);
	gpio_write(led, 1);

	// Blink the LED.
	while (1) {
		delay(1000);
		gpio_write(led, 0);
		delay(1000);
		gpio_write(led, 1);
	}
	return 0;
}
