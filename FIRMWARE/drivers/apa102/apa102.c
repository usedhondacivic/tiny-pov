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

static uint16_t clk_pin = PIN('A', 1);
static uint16_t data_pin = PIN('A', 12);

/*
 * REFERENCE:
 * 27.5.7 Configuration of SPI
 */

void init_apa102(SPI_TypeDef *chan)
{
	// 1. Configure GPIO (SPI1_SCK, SPI1_MOSI)
	gpio_set_mode(clk_pin, GPIO_MODE_AF);
	gpio_set_af(clk_pin, 0);
	gpio_set_mode(data_pin, GPIO_MODE_AF);
	gpio_set_af(data_pin, 0);

	// 2. Write to the SPI_CR1 register
	// a) Configure the serial clock baud rate (f_{pclk} / 2)
	chan->CR1 &= ~(SPI_CR1_BR_Msk);
	// b) Configure the CPOL and CPHA bits (leave default)
	// c) Select simplex or half-duplex mode (half-duplex, tx-only)
	chan->CR1 |= SPI_CR1_BIDIMODE_Msk;
	chan->CR1 |= SPI_CR1_BIDIOE_Msk;
	// d) Configure the LSBFIRST bit to define the frame format (LSB first)
	chan->CR1 |= SPI_CR1_LSBFIRST_Msk;
	// e) Configure the CRCL and CRCEN bits if CRC is needed (Defaults disabled)
	// f) Configure SSM and SSI (Defaults disabled)
	// g) Configure the MSTR bit (Master)
	chan->CR1 |= SPI_CR1_MSTR_Msk;

	// 3. Write to SPI_CR2 register
	// a) Configure the DS[3:0] bits to select the data length for the transfer
	// (8-bit default)
	// b) Configure SSOE (Defaults disabled)
	// c) Set the FRF bit if the TI protocol is required (Defaults disabled)
	// d) Set the NSSP bit if the NSS pulse mode between two data units is
	// required (Defaults disabled)
	// e) Configure the FRXTH bit (Not needed until DMA)
	// f) Initialize LDMA_TX and LDMA_RX bits if DMA is used in packed mode (Not
	// needed until DMA)
	// 4) Enable SPI
	if (chan == SPI1)
		RCC->APBENR2 |= RCC_APBENR2_SPI1EN_Msk;
	if (chan == SPI2)
		RCC->APBENR1 |= RCC_APBENR1_SPI2EN_Msk;
}

void apa102_write_strip(uint8_t *data) {}
