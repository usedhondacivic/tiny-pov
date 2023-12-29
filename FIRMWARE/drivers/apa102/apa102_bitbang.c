/**
 * Author: Michael Crum
 * Date: 12/28/2023
 *
 * SPI driver for the STM32g030
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 */

// #include <stdint.h>
//
// #include "drivers/gpio/gpio.h"
//
// uint8_t num_leds;
// // uint8_t clk_pin = PIN('A', 13);
// uint16_t clk_pin = PIN('B', 7);
// uint16_t data_pin = PIN('A', 12);
//
// const uint8_t start_frame[] = { 0x00, 0x00, 0x00, 0x00 };
// const uint8_t end_frame[] = { 0xFF, 0xFF, 0xFF, 0xFF };
// const uint8_t brightness_byte = 0xE1;
//
// void write_word(uint8_t *data);
// void write_byte(uint8_t data);
//
// void init_apa102(uint8_t n_leds)
// {
// 	num_leds = n_leds;
// 	gpio_set_mode(data_pin, GPIO_MODE_OUTPUT);
// 	gpio_set_highspeed(data_pin);
// 	gpio_set_mode(clk_pin, GPIO_MODE_OUTPUT);
// 	gpio_set_highspeed(clk_pin);
// 	gpio_write(clk_pin, 0);
// }
//
// void write_strip(uint8_t *data)
// {
// 	write_byte(0x00);
// 	write_word((uint8_t *)start_frame);
// 	for (int i = 0; i < num_leds; i++) {
// 		write_byte(brightness_byte);
// 		write_word(&data[i * 3]);
// 	}
// 	write_byte(0xFF);
// 	write_word((uint8_t *)end_frame);
// }
//
// void write_word(uint8_t *data)
// {
// 	for (int i = 0; i < 3; i++) {
// 		write_byte(data[i]);
// 	}
// }
//
// void write_byte(const uint8_t data)
// {
// 	for (int i = 0; i < 8; i++) {
// 		gpio_write(clk_pin, 0);
// 		uint8_t bit = (data >> (7 - i)) & 0b1u;
// 		gpio_write(data_pin, bit);
// 		gpio_write(clk_pin, 1);
// 	}
// }
