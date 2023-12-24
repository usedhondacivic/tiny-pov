#include "hal.h"
#include <stdbool.h>
#include <stdint.h>

int main(void)
{
	uint16_t led = PIN('B', 6);
	gpio_set_mode(led, GPIO_MODE_OUTPUT);
	gpio_write(led, true);

	// Blink the LED.
	while (1) {
		spin(16000000);
		gpio_write(led, false);
		spin(16000000);
		gpio_write(led, true);
	}
	return 0;
}
