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

#include "drivers/clock/clock.h"
#include "drivers/gpio/gpio.h"

static volatile uint32_t s_ticks;
void SysTick_Handler(void)
{
	s_ticks++;
}

void delay(unsigned ms)
{
	uint32_t until = s_ticks + ms;
	while (s_ticks < until)
		(void)0;
}

int main(void)
{
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
