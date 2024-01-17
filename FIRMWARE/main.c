/**
 * Author: Michael Crum
 * Date: 12/28/2023
 *
 * Entry point for Tiny POV
 *
 * https://github.com/usedhondacivic/tiny-pov
 * michael-crum.com/tiny-pov
 */

#include <stdbool.h>
#include <stdint.h>

#include "drivers/apa102/apa102.h"
#include "drivers/clock/clock.h"
#include "drivers/fat/fat.h"
#include "drivers/gpio/gpio.h"
#include "drivers/sd/sd.h"
#include "stm32g031xx.h"

static uint16_t sd_clk_pin = PIN('B', 8);
static uint16_t sd_mosi_pin = PIN('A', 10);
static uint16_t sd_miso_pin = PIN('A', 9);

static uint16_t apa102_clk_pin = PIN('A', 1);
static uint16_t apa102_data_pin = PIN('B', 5);

void apa102_gpio_map()
{
	// disable SWD
	gpio_set_mode(PIN('A', 14), GPIO_MODE_ANALOG);
	gpio_set_mode(PIN('A', 13), GPIO_MODE_ANALOG);
	gpio_set_PUPDR(PIN('A', 14), GPIO_NO_PULL);
	gpio_set_PUPDR(PIN('A', 13), GPIO_NO_PULL);

	gpio_set_mode(apa102_clk_pin, GPIO_MODE_AF);
	gpio_set_af(apa102_clk_pin, 0);
	gpio_set_highspeed(apa102_clk_pin);

	gpio_set_mode(apa102_data_pin, GPIO_MODE_AF);
	gpio_set_af(apa102_data_pin, 0);
	gpio_set_highspeed(apa102_data_pin);
}

int main(void)
{
	// Delay to allow reprogramming
	delay(3000);

	/* Configure GPIO */

	// SD Card SPI Mode
	gpio_set_mode(sd_clk_pin, GPIO_MODE_AF);
	gpio_set_af(sd_clk_pin, 1);
	gpio_set_highspeed(sd_clk_pin);

	SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_RMP_Msk; // Enable A9
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA12_RMP_Msk; // Enable A10
	gpio_set_mode(sd_mosi_pin, GPIO_MODE_AF);
	gpio_set_af(sd_mosi_pin, 0);
	gpio_set_highspeed(sd_mosi_pin);

	gpio_set_mode(sd_miso_pin, GPIO_MODE_AF);
	gpio_set_af(sd_miso_pin, 0b100u);
	gpio_set_highspeed(sd_miso_pin);

	/* Init peripherals */

	if (init_sd(SPI2)) {
		init_fat();
	}

	uint8_t pixel_buff_one[512] = { 0 };
	uint8_t pixel_buff_two[512] = { 0 };

	// tiny_pov.pov
	fat_open_file("TINY_POVPOV");
	// fat_read_file_block((uint8_t *)pixel_buff_one);

	init_apa102(SPI1);

	uint32_t bytes_remaining_total =
	  fat_read_file_block((uint8_t *)pixel_buff_one);
	uint32_t bytes_written_block = 0;
	while (bytes_remaining_total > 0) {
		bytes_remaining_total = fat_read_file_block((uint8_t *)pixel_buff_one);
		while (bytes_written_block <= 512 - 64 * 4 &&
			   bytes_remaining_total > 64 * 4) {
			apa102_gpio_map();
			apa102_write_strip((uint8_t *)pixel_buff_one);
		}
	}

	return 0;
}
