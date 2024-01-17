/**
 * Author: Michael Crum
 * Date: 12/28/2023
 *
 * APA102 driver for the STM32g031
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 *
 * APA102 data sheet:
 * https://www.mouser.com/datasheet/2/737/APA102_2020_SMD_LED-2487271.pdf
 */

#include "drivers/gpio/gpio.h"
#include "stm32g031xx.h"

#include "apa102.h"
#include <stdbool.h>
#include <stdint.h>

#define NUM_LEDS 64

static volatile SPI_TypeDef *channel;

static const uint8_t end_frame[] = { 0xFF, 0xFF, 0xFF, 0xFF };
static const uint8_t start_frame[] = { 0x00, 0x00, 0x00, 0x00 };
static const uint8_t brightness_byte = 0b11100001;

/*
 * REFERENCE:
 * 27.5.7 Configuration of SPI
 *
 * clk_pin and data_pin must be preconfigured to the appropriate AF
 */

void init_apa102(volatile SPI_TypeDef *chan)
{
	if (chan == SPI1)
		RCC->APBENR2 |= RCC_APBENR2_SPI1EN_Msk;
	if (chan == SPI2)
		RCC->APBENR1 |= RCC_APBENR1_SPI2EN_Msk;

	// 1. Configure GPIO (SPI1_SCK, SPI1_MOSI)
	// preconditioned

	// 2. Write to the SPI_CR1 register
	// a) Configure the serial clock baud rate (f_{pclk} / 2)
	chan->CR1 &= ~(SPI_CR1_BR_Msk);
	// b) Configure the CPOL and CPHA bits (leave default)
	// c) Select simplex or half-duplex mode (half-duplex, tx-only)
	chan->CR1 |= SPI_CR1_BIDIMODE_Msk;
	chan->CR1 |= SPI_CR1_BIDIOE_Msk;
	// d) Configure the LSBFIRST bit to define the frame format (MSB first)
	chan->CR1 &= ~(SPI_CR1_LSBFIRST_Msk);
	// e) Configure the CRCL and CRCEN bits if CRC is needed (Defaults disabled)
	// f) Configure SSM and SSI (Defaults disabled)
	// g) Configure the MSTR bit (Master)
	chan->CR1 |= SPI_CR1_MSTR_Msk;

	// 3. Write to SPI_CR2 register
	// a) Configure the DS[3:0] bits to select the data length for the transfer
	// (8-bit default)
	// b) Configure SSOE (SS enabled, needed for tx-only)
	chan->CR2 |= SPI_CR2_SSOE_Msk;
	// c) Set the FRF bit if the TI protocol is required (Defaults disabled)
	// d) Set the NSSP bit if the NSS pulse mode between two data units is
	// required (Defaults disabled)
	// e) Configure the FRXTH bit (Not needed until DMA)
	// f) Initialize LDMA_TX and LDMA_RX bits if DMA is used in packed mode (Not
	// needed until DMA)
	// 4) Enable SPI
	//
	chan->CR1 |= SPI_CR1_SPE_Msk;
	channel = chan;
}

void write_sequence(const uint8_t *data, uint16_t len)
{
	for (int i = 0; i < len; i++) {
		while (!(channel->SR & SPI_SR_TXE_Msk)) {
		};
		*((volatile uint8_t *)&(channel->DR)) = data[i];
	}
}

void write_led_frames(const uint8_t *data, uint16_t len)
{
	for (int i = 0; i < len; i++) {
		// write_sequence(&brightness_byte, 1);
		write_sequence(data + i * 4, 4);
	}
}

void apa102_write_strip(uint8_t *data)
{
	write_sequence(start_frame, 4);
	write_led_frames(data, NUM_LEDS);
	write_sequence(end_frame, 4);
}
