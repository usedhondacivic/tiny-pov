/**
 * Author: Michael Crum
 * Date: 12/28/2023
 *
 * SPI driver for the STM32g031
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 */

#pragma once

#include <stdint.h>

void init_bb_apa102(uint8_t num_leds);

void bb_write_strip(uint8_t *data);
