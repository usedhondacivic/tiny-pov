/**
 * Author: Michael Crum
 * Date: 12/25/2023
 *
 * Hardware abstraction layer for the STM32G030 MCU
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * Excerpts from:
 * https://github.com/cpq/bare-metal-programming-guide
 */

#include <stdbool.h>
#include <stm32g031xx.h>

#include "gpio.h"

void gpio_set_mode(uint16_t pin, uint8_t mode)
{
	GPIO_TypeDef *gpio = GPIO(PINBANK(pin)); // GPIO bank
	int n = PINNO(pin);						 // Pin number
	RCC->IOPENR |= BIT(PINBANK(pin));		 // Enable GPIO clock
	gpio->MODER &= ~(3U << (n * 2));		 // Clear existing setting
	gpio->MODER |= (mode & 3U) << (n * 2);	 // Set new mode
}

void gpio_set_af(uint16_t pin, uint8_t af_num)
{
	GPIO_TypeDef *gpio = GPIO(PINBANK(pin)); // GPIO bank
	int n = PINNO(pin);						 // Pin number
	gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
	gpio->AFR[n >> 3] |= ((uint32_t)af_num) << ((n & 7) * 4);
}

void gpio_write(uint16_t pin, bool val)
{
	GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
	gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

void gpio_set_highspeed(uint16_t pin)
{
	GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
	int n = PINNO(pin);					 // Pin number
	gpio->OSPEEDR &= ~(3U << (n * 2));	 // Clear existing setting
	gpio->OSPEEDR |= (0b11u) << (n * 2); // Set new mode
}

// void gpio_nrst()
// {
// 	// // Flash options persist through reboot, only write them if they haven't
// 	// // been set.
// 	// if ((((FLASH->OPTR & FLASH_OPTR_nRST_MODE_Pos) >>
// 	// 	  FLASH_OPTR_nRST_MODE_Pos) &
// 	// 	 0b11u) == 0b10u) {
// 	// 	return;
// 	// }
// 	// 3.3.6 Unlocking the Flash Memory
// 	while ((FLASH->SR & FLASH_SR_BSY1) != 0)
// 		;
// 	FLASH->KEYR = 0x45670123;
// 	FLASH->KEYR = 0xCDEF89AB;
//
// 	// 3.4.2 FLASH option byte programming
// 	FLASH->OPTKEYR = 0x08192A3B;
// 	FLASH->OPTKEYR = 0x4C5D6E7F;
//
// 	// Set NRST to be general GPIO
// 	FLASH->OPTR &= ~(FLASH_OPTR_NRST_MODE_Msk);
// 	FLASH->OPTR |= 0b10u << FLASH_OPTR_NRST_MODE_Pos;
// 	// FLASH->OPTR = 0xF7FFFEAA;
//
// 	// Check that no Flash op is ongoing
// 	while ((FLASH->SR & FLASH_SR_BSY1) != 0)
// 		;
//
// 	FLASH->CR |= FLASH_CR_OPTSTRT;
//
// 	while ((FLASH->SR & FLASH_SR_BSY1) != 0)
// 		;
//
// 	FLASH->CR |= FLASH_CR_OBL_LAUNCH_Msk;
//
// 	while ((FLASH->CR & FLASH_CR_OBL_LAUNCH_Msk) != 0)
// 		;
//
// 	// Lock flash
// 	FLASH->CR |= FLASH_CR_LOCK_Msk;
// }
