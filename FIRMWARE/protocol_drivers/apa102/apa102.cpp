/**
 * Author: Michael Crum
 * Date: 11/8/2023
 * APA102 LED Driver for ATtiny1616
 * THIS IS NOT A GENERAL PURPOSE DRIVER
 * Writes an APA102 strip split into four even parts
 * The four parts are controlled by pins [4:7] of PORTA
 * Using four pins can allow for faster writes.
 */

#include <avr/io.h>
// F_CPU frequency is defined in the makefile
#include <util/delay.h>

#include "apa102.h"

APA102::APA102(uint8_t num_leds, uint8_t clk_pin_mask)
  : num_leds(num_leds)
  , clk_pin_mask(clk_pin_mask)
{
	// Pins are the top half of PORTA (4:7)
	all_pins_mask |= 0xF0;
	PORTA_DIR |= all_pins_mask;
};

void APA102::write_strip(uint8_t *buffer)
{
	// NOTE: Buffer must be preprocessed for this specific type of parallel
	// transmission. Interlace the signal for each quadrant of the strip.

	write_start();
	for (int i = 0; i < num_leds; i++) {
		write_brightness();
		write_two_bit(buffer[i]);
	}
	write_end();
};

void APA102::write_start()
{
	//  Clear all pins
	PORTA_OUT &= ~(all_pins_mask);
	// Set clock pin high
	PORTA_OUT |= clk_pin_mask;
	for (int i = 0; i < 16; i++) {
		write_two_bit(0x00);
	}
}

void APA102::write_end()
{
	for (int i = 0; i < 16; i++) {
		write_two_bit(0x11);
	}
}

void APA102::write_brightness()
{
	write_two_bit(0xFF);
	write_two_bit(0xF0);
	write_two_bit(0x00);
	write_two_bit(0x00);
	write_two_bit(0x0F);
}

void APA102::write_two_bit(uint8_t data)
{
	PORTA_OUT &= ~(clk_pin_mask);			  // Clock low
	PORTA_OUT |= all_pins_mask & data;		  // High nibble (hehe)
	PORTA_OUT |= clk_pin_mask;				  // Clock high (send)
	PORTA_OUT &= ~(clk_pin_mask);			  // Clock low
	PORTA_OUT |= all_pins_mask & (data << 4); // Low nibble
	PORTA_OUT |= clk_pin_mask;				  // Clock high (send)
};