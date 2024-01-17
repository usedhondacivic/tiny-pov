#pragma once

#include <stdint.h>

#include "stm32g031xx.h"

void spi_write_sequence(volatile SPI_TypeDef *channel,
						uint8_t *const data,
						uint16_t len);

void spi_read_sequence(volatile SPI_TypeDef *channel,
					   uint8_t *const buff,
					   uint16_t len);
