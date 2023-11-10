/**
 * Author: Michael Crum
 * Date: 11/8/2023
 * APA102 LED Driver for ATtiny1616
 * Writes to an APA102 strip using multiple output pins
 * This can allow for faster writes.
 */

#include <avr/io.h>
// F_CPU frequency is defined in the makefile
#include <util/delay.h>

#include "apa102.h"

APA102::APA102(uint8_t num_leds,
			   volatile uint8_t* port,
			   uint8_t clk_pin_mask,
			   uint8_t* pin_masks,
			   uint8_t num_pins)
  : num_leds(num_leds)
  , port(port)
  , clk_pin_mask(clk_pin_mask)
  , pin_masks(pin_masks)
  , num_pins(num_pins)
{
	for (int i = 0; i < num_pins; i++) {
		all_pins_mask |= pin_masks[i];
	}
	*(port + 0) |= all_pins_mask; // DIR is offset 0
};

void APA102::write_strip(uint8_t* buffer){

};