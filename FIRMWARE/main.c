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
// #include "stm32g030xx.h"
#include "stm32g031xx.h"
#include "util.h"
#include <math.h>

static uint16_t apa102_clk_pin = PIN('A', 1);
static uint16_t apa102_data_pin = PIN('A', 12);

int main(void)
{
	// Configure GPIO

	gpio_set_mode(apa102_clk_pin, GPIO_MODE_AF);
	gpio_set_af(apa102_clk_pin, 0);
	gpio_set_highspeed(apa102_clk_pin);
	gpio_set_mode(apa102_data_pin, GPIO_MODE_AF);
	gpio_set_af(apa102_data_pin, 0);
	gpio_set_highspeed(apa102_data_pin);
	// Init peripherals

	init_apa102(SPI1);

	// fun visual
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
		apa102_write_strip(led_data);
		n += 5;
	}

	return 0;
}
