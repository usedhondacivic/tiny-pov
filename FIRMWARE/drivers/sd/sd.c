/**
 * Author: Michael Crum
 * Date: 1/5/2023
 *
 * SD card driver for the STM32g031
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 */

#include <stdbool.h>
#include <stdint.h>

#include "drivers/gpio/gpio.h"
#include "sd.h"
#include "stm32g031xx.h"
#include "util.h"

static volatile SPI_TypeDef *channel;

uint8_t get_cmd_byte(uint8_t num)
{
	return (0b01 << 6) | (num & 0b00111111);
}

void get_cmd(uint8_t *buff, uint8_t cmd_num, uint8_t *args, uint8_t crc)
{
	buff[0] = get_cmd_byte(cmd_num);
	for (uint8_t i = 0; i < 4; i++) {
		buff[i + 1] = args[i];
	}
	buff[5] = (uint8_t)(crc << 1) | 0b1;
}

void spi_write_sequence(const uint8_t *data, uint16_t len)
{
	for (int i = 0; i < len; i++) {
		while (!(channel->SR & SPI_SR_TXE_Msk)) {
		};
		*((volatile uint8_t *)&(channel->DR)) = data[i];
	}
}

void spi_read_sequence(uint8_t *buff, uint16_t len)
{
	bool start_cond = false;
	uint8_t i = 0;
	while (i < len) {
		while (!(channel->SR & SPI_SR_TXE_Msk)) {
		};
		*((volatile uint8_t *)&(channel->DR)) = 0xFF;
		while (!(channel->SR & SPI_SR_RXNE_Msk) ||
			   channel->SR & SPI_SR_BSY_Msk) {
		};
		uint8_t read;
		while (channel->SR & SPI_SR_RXNE_Msk) {
			read = *(((volatile uint8_t *)&(channel->DR)));
		};
		if (read != 0xFF)
			start_cond = true;
		if (start_cond) {
			if (buff != 0)
				buff[i] = read;
			i++;
		}
	}
	while (!(channel->SR & SPI_SR_TXE_Msk)) {
	};
	*((volatile uint8_t *)&(channel->DR)) = 0xFF;
}

/*
 * REFERENCE:
 * 27.5.7 Configuration of SPI
 *
 * sck_pin, mosi_pin, and miso_pin must be preconfigured to the appropriate AF
 */

void init_sd(SPI_TypeDef *chan)
{
	if (chan == SPI1)
		RCC->APBENR2 |= RCC_APBENR2_SPI1EN_Msk;
	if (chan == SPI2)
		RCC->APBENR1 |= RCC_APBENR1_SPI2EN_Msk;

	// 1. Configure GPIO (SPI1_SCK, SPI1_MOSI)
	// preconditioned

	// 2. Write to the SPI_CR1 register
	// a) Configure the serial clock baud rate (f_{pclk} / 128)
	chan->CR1 &= ~(SPI_CR1_BR_Msk);
	chan->CR1 |= 0b110 << SPI_CR1_BR_Pos;
	// b) Configure the CPOL and CPHA bits (leave default)
	// c) Select simplex or half-duplex mode (defaults full-duplex)
	// d) Configure the LSBFIRST bit to define the frame format (MSB first)
	chan->CR1 &= ~(SPI_CR1_LSBFIRST_Msk);
	// e) Configure the CRCL and CRCEN bits if CRC is needed (Defaults disabled)
	// f) Configure SSM and SSI (Software control)
	chan->CR1 |= SPI_CR1_SSM_Msk;
	// g) Configure the MSTR bit (Master)
	chan->CR1 |= SPI_CR1_MSTR_Msk;

	// 3. Write to SPI_CR2 register
	// a) Configure the DS[3:0] bits to select the data length for the transfer
	// (8-bit default)
	// b) Configure SSOE (SS enabled)
	chan->CR2 |= SPI_CR2_SSOE_Msk;
	// c) Set the FRF bit if the TI protocol is required (Defaults disabled)
	// d) Set the NSSP bit if the NSS pulse mode between two data units is
	// required (Defaults disabled)
	// e) Configure the FRXTH bit (Not needed until DMA)
	chan->CR2 |= SPI_CR2_FRXTH_Msk;
	// f) Initialize LDMA_TX and LDMA_RX bits if DMA is used in packed mode (Not
	// needed until DMA)
	// 4) Enable SPI
	chan->CR1 |= SPI_CR1_SPE_Msk;

	channel = chan;

	delay(10);

	gpio_write(PIN('B', 7), 1);

	uint8_t sd_spi_init[10];
	for (int i = 0; i < 10; i++) {
		sd_spi_init[i] = 0xFF;
	}

	spi_write_sequence(sd_spi_init, 10);
	// uint8_t CMD0[] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x95 };
	uint8_t CMD0[6];
	get_cmd(CMD0, 0, (uint8_t[]){ 0x00, 0x00, 0x00, 0x00 }, 0x4A);
	spi_write_sequence(CMD0, 6);
	spi_read_sequence(0, 1);

	// 0x48000001AA0F
	uint8_t CMD8[6];
	get_cmd(CMD8, 8, (uint8_t[]){ 0x00, 0x00, 0x01, 0xAA }, 0x0F);
	spi_write_sequence(CMD8, 6);
	uint8_t CMD8_resp[5];
	spi_read_sequence(CMD8_resp, 5);

	// 0x7A0000000075
	// uint8_t CMD58[] = { 0x7A, 0x00, 0x00, 0x00, 0x00, 0x75 };
	uint8_t CMD58[6];
	get_cmd(CMD58, 58, (uint8_t[]){ 0x00, 0x00, 0x00, 0x00 }, 0x75);
	spi_write_sequence(CMD58, 6);
	uint8_t CMD58_resp[5];
	spi_read_sequence(CMD58_resp, 5);

	uint8_t CMD55[6];
	uint8_t ACMD41[6];
	get_cmd(CMD55, 55, (uint8_t[]){ 0x00, 0x00, 0x00, 0x00 }, 0x00);
	get_cmd(ACMD41, 41, (uint8_t[]){ 0x40, 0x00, 0x00, 0x00 }, 0x00);
	uint8_t attempts = 0;
	uint8_t resp[1] = { 0x01 };
	while ((*resp) != 0x00 && attempts < 50) {
		spi_write_sequence(CMD55, 6);
		spi_read_sequence(0, 1);
		spi_write_sequence(ACMD41, 6);
		spi_read_sequence(resp, 1);
		delay(5);
	}

	// uint8_t CMD1[] = { 0x41, 0x00, 0x00, 0x00, 0x00, 0x95 };
	// // get_cmd(CMD1, 1, (uint8_t[]){ 0x00, 0x00, 0x00, 0x00 }, 0x95);
	// uint8_t resp[1] = { 0x01 };
	// uint8_t attempts = 0;
	// while ((*resp) == 0x01 && attempts < 15) {
	// 	spi_write_sequence(CMD1, 6);
	// 	spi_read_sequence(resp, 1);
	// 	attempts++;
	// 	delay(500);
	// }
	// spi_write_sequence(CMD1, 6);
	// spi_read_sequence(0, 1);
	// delay(1);
}

void sd_read(uint32_t len) {}
