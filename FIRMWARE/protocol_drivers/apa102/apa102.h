#pragma once

#include <avr/io.h>

class APA102
{
  public:
	APA102(uint8_t num_leds, uint8_t clk_pin_mask);

	void write_strip(uint8_t *buffer);

  private:
	uint8_t num_leds;
	uint8_t clk_pin_mask;

	uint8_t all_pins_mask;
	uint8_t pin_offset;

	void write_start();

	void write_end();

	void write_two_bit(uint8_t data);

	void APA102::write_brightness();
};