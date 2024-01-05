/**
 * Author: Michael Crum
 * Date: 12/25/2023
 *
 * GPIO utils for the STM32g030
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x0-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * Excerpts from:
 * https://github.com/cpq/bare-metal-programming-guide
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

/*
 * Although both the datasheet and ST Employees claim NSRT can't be rebound on
 * the STM32g0x0, writing to the relevant bits still works.
 * https://community.st.com/t5/stm32-mcus-products/can-the-stm32g030-nrst-pin-disabled-or-be-used-as-gpio/td-p/22026h6
 */
// #define FLASH_OPTR_nRST_MODE_Pos 27
// #define FLASH_OPTR_nRST_MODE_Msk (0b11u << FLASH_OPTR_nRST_MODE_Pos)

#define GPIO(bank) ((GPIO_TypeDef *)(GPIOA_BASE + 0x400U * (bank)))
enum
{
	GPIO_MODE_INPUT,
	GPIO_MODE_OUTPUT,
	GPIO_MODE_AF,
	GPIO_MODE_ANALOG
};

void gpio_set_mode(uint16_t pin, uint8_t mode);

void gpio_set_af(uint16_t pin, uint8_t af_num);

void gpio_write(uint16_t pin, bool val);

void gpio_set_highspeed(uint16_t pin);
