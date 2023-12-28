#pragma once

#include <stdint.h>

#include "drivers/clock/clock.h"

void delay(unsigned ms)
{
	uint32_t until = s_ticks + ms;
	while (s_ticks < until)
		(void)0;
}
