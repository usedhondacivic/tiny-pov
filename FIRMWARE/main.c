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

// static uint16_t sd_clk_pin = PIN('B', 8);
// static uint16_t sd_mosi_pin = PIN('A', 9);
// static uint16_t sd_miso_pin = PIN('B', 6);

// static uint16_t sd_clk_pin = PIN('A', 1);
// static uint16_t sd_mosi_pin = PIN('A', 12);
// static uint16_t sd_miso_pin = PIN('A', 11);

static uint16_t sd_clk_pin = PIN('B', 8);
static uint16_t sd_mosi_pin = PIN('A', 10);
static uint16_t sd_miso_pin = PIN('A', 9);

static uint16_t apa102_clk_pin = PIN('A', 1);
static uint16_t apa102_data_pin = PIN('B', 5);

int main(void)
{
	// Delay to allow reprogramming
	delay(3000);

	/* Configure GPIO */

	// disable SWD
	// gpio_set_mode(PIN('A', 14), GPIO_MODE_ANALOG);
	// gpio_set_mode(PIN('A', 13), GPIO_MODE_ANALOG);
	// gpio_set_PUPDR(PIN('A', 14), GPIO_NO_PULL);
	// gpio_set_PUPDR(PIN('A', 13), GPIO_NO_PULL);

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

	// gpio_set_mode(apa102_clk_pin, GPIO_MODE_AF);
	// gpio_set_af(apa102_clk_pin, 0);
	// gpio_set_highspeed(apa102_clk_pin);
	//
	// gpio_set_mode(apa102_data_pin, GPIO_MODE_AF);
	// gpio_set_af(apa102_data_pin, 0);
	// gpio_set_highspeed(apa102_data_pin);

	/* Init peripherals */

	// init_apa102(SPI1);
	if (init_sd(SPI2)) {
		init_fat();
	}
	fat_read_file("README  TXT");

	// uint8_t all_red[64 * 3];
	//
	// for (int i = 0; i < 64; i++) {
	// 	all_red[i * 3] = 0x00;
	// 	all_red[i * 3 + 1] = 0x00;
	// 	all_red[i * 3 + 2] = 0xFF;
	// }
	//
	// apa102_write_strip(all_red);

	while (1) {
	}

	return 0;
}
