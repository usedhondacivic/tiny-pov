/**
 * Author: Michael Crum
 * Date: 12/25/2023
 *
 * Hardware abstraction layer for the STM32G031 MCU
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * Excerpts from:
 * https://github.com/cpq/bare-metal-programming-guide
 */

#pragma once

#include <stdbool.h>

#include <stm32g031xx.h>

#define FREQ 64000000 // CPU frequency, 64 Mhz
#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

static inline void spin(volatile uint32_t count)
{
	while (count--)
		asm("nop");
}

#define GPIO(bank) ((GPIO_TypeDef *)(GPIOA_BASE + 0x400U * (bank)))
enum
{
	GPIO_MODE_INPUT,
	GPIO_MODE_OUTPUT,
	GPIO_MODE_AF,
	GPIO_MODE_ANALOG
};

static inline void gpio_set_mode(uint16_t pin, uint8_t mode)
{
	GPIO_TypeDef *gpio = GPIO(PINBANK(pin)); // GPIO bank
	int n = PINNO(pin);						 // Pin number
	RCC->IOPENR |= BIT(PINBANK(pin));		 // Enable GPIO clock
	gpio->MODER &= ~(3U << (n * 2));		 // Clear existing setting
	gpio->MODER |= (mode & 3U) << (n * 2);	 // Set new mode
}

static inline void gpio_set_af(uint16_t pin, uint8_t af_num)
{
	GPIO_TypeDef *gpio = GPIO(PINBANK(pin)); // GPIO bank
	int n = PINNO(pin);						 // Pin number
	gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
	gpio->AFR[n >> 3] |= ((uint32_t)af_num) << ((n & 7) * 4);
}

static inline void gpio_write(uint16_t pin, bool val)
{
	GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
	gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

/*
 * REFERENCE
 * 5.2.3: PLL
 * 5.2.6: SYSCLK Selection
 * */
static inline void init_clock()
{
	RCC->CR &= ~(RCC_CR_PLLON_Msk); //
}
