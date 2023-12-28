#pragma once

#include <stdint.h>

void spin(volatile uint32_t count)
{
	while (count--)
		asm("nop");
}
