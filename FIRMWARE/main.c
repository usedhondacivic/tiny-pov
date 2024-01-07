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
#include "drivers/sd/sd.h"
#include "stm32g031xx.h"

static uint16_t apa102_clk_pin = PIN('A', 1);
static uint16_t apa102_mosi_pin = PIN('A', 12);
static uint16_t apa102_miso_pin = PIN('A', 11);

int main(void)
{
	// Configure GPIO
	gpio_set_mode(apa102_clk_pin, GPIO_MODE_AF);
	gpio_set_af(apa102_clk_pin, 0);
	gpio_set_highspeed(apa102_clk_pin);
	gpio_set_mode(apa102_mosi_pin, GPIO_MODE_AF);
	gpio_set_af(apa102_mosi_pin, 0);
	gpio_set_highspeed(apa102_mosi_pin);
	gpio_set_mode(apa102_miso_pin, GPIO_MODE_AF);
	gpio_set_af(apa102_miso_pin, 0);
	gpio_set_highspeed(apa102_miso_pin);

	gpio_set_mode(PIN('B', 7), GPIO_MODE_OUTPUT);

	// Init peripherals

	// init_apa102(SPI1);
	init_sd(SPI1);

	while (1) {
	}

	return 0;
}
