/**
 * Author: Michael Crum
 * Date: 1/5/2023
 *
 * SD card driver for the STM32g031
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 */

#pragma once

#include "stm32g031xx.h"

void init_sd(SPI_TypeDef *chan);

void sd_read(uint32_t len);
