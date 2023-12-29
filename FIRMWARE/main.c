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
#include <math.h>

int main(void)
{
	init_apa102(64);

	unsigned int n = 0;
	uint8_t led_data[64 * 3];
	while (1) {
		for (uint8_t i = 0; i < 64; i++) {
			for (int j = 0; j < 3; j++) {
				uint8_t val =
				  (uint8_t)((255.0 / 2.0) * (1 + cos(n / 30.0 + j + (i / 32.0) +
													 (i % 8) / 3.0)));
				led_data[i * 3 + j] = val;
			}
		}
		write_strip(led_data);
		n += 5;
	}

	return 0;
}
