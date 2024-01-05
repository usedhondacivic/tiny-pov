/**
 * Author: Michael Crum
 * Date: 12/28/2023
 *
 * APA102 driver for the STM32g031
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 */

#pragma once

#include "stm32g031xx.h"

#include <stdbool.h>
#include <stdint.h>

void init_apa102(SPI_TypeDef *chan);

void apa102_write_strip(uint8_t *data);
