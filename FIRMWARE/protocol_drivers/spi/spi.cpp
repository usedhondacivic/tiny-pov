/**
 * Author: Michael Crum
 * Date: 11/8/2023
 * SPI Driver for ATtiny1616 based on:
 * https://ww1.microchip.com/downloads/en/Appnotes/TB3215-Getting-Started-with-SPI-90003215A.pdf
 */

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>

#include "spi.h"

void SPI0_init(void)
{
	PORTA.DIR |= PIN1_bm;		  /* Set MOSI pin direction to output */
	PORTA.DIR &= ~PIN2_bm;		  /* Set MISO pin direction to input */
	PORTA.DIR |= PIN3_bm;		  /* Set SCK pin direction to output */
	PORTA.DIR |= PIN4_bm;		  /* Set SS pin direction to output */
	SPI0.CTRLA = SPI_CLK2X_bm	  /* Enable double-speed */
				 | SPI_ENABLE_bm  /* Enable module */
				 | SPI_MASTER_bm; /* SPI module in Master mode */
}

pin_t SPIDevice::current_device = { .port = nullptr, .pin_mask = 0 };

SPIDevice::SPIDevice(pin_t cs)
  : cs(cs)
{
	// Set CS pin as output
	*(cs.port + 0) |= cs.pin_mask; // pin DIR is offset 0
}

void SPIDevice::write_data(const uint8_t* data, uint8_t data_len)
{
	enable(cs);
	for (uint8_t i = 0; i < data_len; i++) {
		write_byte(data[i]);
	}
}

void SPIDevice::read_data(uint8_t* buffer, uint8_t data_len)
{
	enable(cs);
}

void SPIDevice::enable(pin_t dev)
{
	// Disable current device (note cs is active low)
	if (current_device.port != nullptr)
		*(current_device.port + 4) |=
		  current_device.pin_mask; // pin OUT is offset 0x4
	// Enable new device
	*(dev.port + 4) &= ~dev.pin_mask; // pin OUT is offset 0x4
	// Update current device
	current_device = dev;
}

uint8_t SPIDevice::write_byte(uint8_t data)
{
	SPI0.DATA = data;
	while (!(SPI0.INTFLAGS & SPI_IF_bm)) /* waits until data is exchanged*/
	{
		;
	}
	return SPI0.DATA;
}

uint8_t SPIDevice::read_byte(uint8_t data)
{
	return write_byte(0xFF);
};