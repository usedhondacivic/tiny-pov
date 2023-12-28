#include <stdbool.h>
#include <stdint.h>

#include "drivers/clock/clock.h"
#include "drivers/gpio/gpio.h"
#include "util.h"

int main(void)
{
	init_clock();
	uint16_t led = PIN('A', 8);
	gpio_set_mode(led, GPIO_MODE_OUTPUT);
	gpio_write(led, 1);

	// Blink the LED.
	while (1) {
		spin(16000000);
		gpio_write(led, 0);
		spin(16000000);
		gpio_write(led, 1);
	}
	return 0;
}
