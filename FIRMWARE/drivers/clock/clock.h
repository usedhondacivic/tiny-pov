/**
 * Author: Michael Crum
 * Date: 12/27/2023
 *
 * Clock utils for the STM32g031
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 */

#pragma once

#include <stdint.h>

/*
 * REFERENCE
 * 5.2.3: PLL
 * 5.2.6: SYSCLK Selection
 * */
void init_clock();

extern volatile uint32_t s_ticks;
