/**
 * Author: Michael Crum
 * Date: 1/5/2023
 *
 * SD card driver for the STM32g031
 * Operates an SD card in SPI Mode
 * Only supports SD v2.00 or later
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 *
 * SD Spec:
 * https://www.sdcard.org/downloads/pls/pdf/?p=Part1_Physical_Layer_Simplified_Specification_Ver9.10.jpg&f=Part1PhysicalLayerSimplifiedSpecificationVer9.10Fin_20231201.pdf&e=EN_SS9_1
 */

#include <stdbool.h>
#include <stdint.h>

#include "drivers/clock/clock.h"
#include "drivers/gpio/gpio.h"
#include "drivers/spi/spi.h"
#include "sd.h"
#include "stm32g031xx.h"

#define reverse_bytes_32(num)                                                  \
	(((num & 0xFF000000) >> 24) | ((num & 0x00FF0000) >> 8) |                  \
	 ((num & 0x0000FF00) << 8) | ((num & 0x000000FF) << 24))

static volatile SPI_TypeDef *channel;

uint8_t get_cmd_byte(uint8_t num)
{
	return (uint8_t)((0b01 << 6) | (num & 0b00111111));
}

void get_cmd(uint8_t *buff,
			 const uint8_t cmd_num,
			 const uint8_t *args,
			 const uint8_t crc)
{
	buff[0] = get_cmd_byte(cmd_num);
	for (uint8_t i = 0; i < 4; i++) {
		buff[i + 1] = args[i];
	}
	buff[5] = (uint8_t)(crc << 1) | 0b1;
}

void send_sd_cmd(const uint8_t cmd_num,
				 const uint8_t *args,
				 const uint8_t crc,
				 uint8_t *resp_buff,
				 uint8_t resp_length)
{
	// Send command string
	uint8_t cmd_buff[6];
	get_cmd(cmd_buff, cmd_num, args, crc);
	spi_write_sequence(channel, cmd_buff, 6);

	// SD card might need clock cycles to work, during which it will respond
	// 0xFF. Keep the clock going until a non-0xFF byte is recieved, then start
	// recording the response.
	uint8_t i = 0;
	bool start_cond = false;
	while (i < resp_length) {
		uint8_t read;
		spi_read_sequence(channel, &read, 1);
		if (read != 0xFF) {
			start_cond = true;
		}
		if (start_cond) {
			if (resp_buff != 0)
				resp_buff[i] = read;
			i++;
		}
	}

	// The card might need some cycles after the response to clean up. Keep the
	// clock going for eight cycles.
	spi_write_sequence(channel, (uint8_t[]){ 0xFF }, 1);
}

void send_read_sd_cmd(const uint8_t cmd_num,
					  const uint8_t *args,
					  const uint8_t crc,
					  uint8_t *resp_buff,
					  uint16_t resp_length)
{
	uint8_t R1_resp;
	send_sd_cmd(cmd_num, args, crc, &R1_resp, 1);

	uint16_t i = 0;
	bool start_cond = false;
	if (R1_resp == 0x00) {
		while (i < resp_length) {
			uint8_t read;
			spi_read_sequence(channel, &read, 1);
			if (read != 0xFF && !start_cond) {
				start_cond = true;
				continue;
			}
			if (start_cond) {
				if (resp_buff != 0)
					resp_buff[i] = read;
				i++;
			}
		}
	}
	// Read out CRC and an extra byte to keep the clock running
	spi_write_sequence(channel, (uint8_t[]){ 0xFF, 0xFF, 0xFF }, 3);
}

void sd_read_block(uint32_t loc, uint8_t *block_buff)
{
	uint32_t loc_flipped = reverse_bytes_32(loc);
	uint8_t *loc_arr = (uint8_t *)&loc_flipped;
	send_read_sd_cmd(17, loc_arr, 0x00, block_buff, (uint16_t)512);
}

/*
 * REFERENCE:
 * STM32g031x: 27.5.7 Configuration of SPI
 * SD Simplifed Spec: 7.2.1 Mode Selection and Initialization
 *
 * sck_pin, mosi_pin, and miso_pin must be preconfigured to the appropriate AF
 */

bool init_sd(SPI_TypeDef *chan)
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
	delay(300);
	uint8_t attempts = 0;

	// gpio_write(PIN('B', 7), 1);

	// Min 74 cycles with MOSI high to init SPI mode
	// Note: According to the spec, this should be accompanied by a high/low
	// transition on CS. I've found this isn't required, and I'm omiting it
	// because there aren't enough pins on the SOT8 package for CS.
	uint8_t sd_spi_init[10];
	for (int i = 0; i < 10; i++) {
		sd_spi_init[i] = 0xFF;
	}
	spi_write_sequence(channel, sd_spi_init, 10);

	// CMD0 -> CMD8 -> CMD58 (optional) -> repeat ACMD41 until idle bit cleared
	uint8_t R1_resp;
	do {
		send_sd_cmd(
		  0, (const uint8_t[]){ 0x00, 0x00, 0x00, 0x00 }, 0x4A, &R1_resp, 1);
		attempts++;
	} while (R1_resp != 0x01 && attempts < 50);

	uint8_t CMD8_resp[5];
	send_sd_cmd(8, (uint8_t[]){ 0x00, 0x00, 0x01, 0xAA }, 0x0F, CMD8_resp, 5);

	uint8_t CMD58_resp[5];
	send_sd_cmd(58, (uint8_t[]){ 0x00, 0x00, 0x00, 0x00 }, 0x75, CMD58_resp, 5);

	attempts = 0;
	do {
		send_sd_cmd(55, (uint8_t[]){ 0x00, 0x00, 0x00, 0x00 }, 0x00, 0, 1);
		send_sd_cmd(
		  41, (uint8_t[]){ 0x40, 0x00, 0x00, 0x00 }, 0x00, &R1_resp, 1);
		attempts++;
	} while (R1_resp != 0x00 && attempts < 50);

	// Set block size to 512
	send_sd_cmd(16, (uint8_t[]){ 0x00, 0x00, 0x02, 0x00 }, 0x00, &R1_resp, 1);

	if (R1_resp != 0x00)
		return false;
	// Card successfully initialized

	// Increase clock speed
	while (!(channel->SR & SPI_SR_TXE_Msk) || channel->SR & SPI_SR_BSY_Msk) {
	};
	chan->CR1 &= ~(SPI_CR1_BR_Msk);
	// Limit speed for debugging on 24 MHz logic analyzer
	chan->CR1 |= 0b011 << SPI_CR1_BR_Pos;

	return true;
}
