#pragma once

#include <avr/io.h>

class APA102
{
  public:
	APA102(uint8_t num_leds,
		   volatile uint8_t* port,
		   uint8_t clk_pin_mask,
		   uint8_t* pin_masks,
		   uint8_t num_pins);

	void write_strip(uint8_t* buffer);

  private:
	const volatile uint8_t* port;
	uint8_t clk_pin_mask;
	uint8_t* pin_masks;
	uint8_t num_leds;
	uint8_t num_pins;

	uint8_t all_pins_mask;
};