#include <stdbool.h>
#include <stdint.h>

#include "spi.h"
#include "stm32g031xx.h"

void spi_write_sequence(volatile SPI_TypeDef *channel,
						uint8_t *const data,
						uint16_t len)
{
	for (int i = 0; i < len; i++) {
		while (!(channel->SR & SPI_SR_TXE_Msk)) {
		};
		*((volatile uint8_t *)&(channel->DR)) = data[i];
	}
	while (channel->SR & SPI_SR_BSY_Msk) {
	};
}

void spi_read_sequence(volatile SPI_TypeDef *channel,
					   uint8_t *const buff,
					   uint16_t len)
{
	uint8_t i = 0;
	while (i < len) {
		while (!(channel->SR & SPI_SR_TXE_Msk)) {
		};
		*((volatile uint8_t *)&(channel->DR)) = 0xFF;
		while (!(channel->SR & SPI_SR_RXNE_Msk) ||
			   channel->SR & SPI_SR_BSY_Msk) {
		};
		uint8_t read = 0xFF;
		while (channel->SR & SPI_SR_RXNE_Msk) {
			read = *(((volatile uint8_t *)&(channel->DR)));
		};
		if (buff != 0)
			buff[i] = read;
		i++;
	}
	while (channel->SR & SPI_SR_BSY_Msk) {
	};
}
