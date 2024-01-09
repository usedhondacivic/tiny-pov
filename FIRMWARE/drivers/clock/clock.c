/**
 * Author: Michael Crum
 * Date: 12/27/2023
 *
 * Clock utils for the STM32g031
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 */

#include "stm32g031xx.h"

#include "clock.h"

#define SYS_FREQUENCY 64000000

/*
 * Configure system clock use the PLL.
 * PLL takes HSI16 (16 MHz by default) and applies a 4x multiplier for a total
 * of 64 MHz. This is the max speed the chip can run.
 *
 * REFERENCE
 * 5.2.3: PLL
 * 5.2.6: SYSCLK Selection
 * 3.3.4: FLASH read access latency
 * */

void init_clock()
{
	// 1. Disable PLL
	RCC->CR &= ~(RCC_CR_PLLON_Msk);

	// 2. Wait until PLLRDY is cleared
	while ((RCC->CR & RCC_CR_PLLRDY_Msk) > 0)
		;

	// 3. Configure PLL
	// Set HSI16 as clock source
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC_Msk;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI << RCC_PLLCFGR_PLLSRC_Pos;

	// (N / M) / R = 4 -> output = 4x input
	// N = 16, M = 2, R = 2
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
	RCC->PLLCFGR |= 16 << RCC_PLLCFGR_PLLN_Pos;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
	RCC->PLLCFGR |= 0b001 << RCC_PLLCFGR_PLLM_Pos;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR_Msk;
	RCC->PLLCFGR |= 0b001 << RCC_PLLCFGR_PLLR_Pos;

	// 4. Enable PLLR
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN_Msk;

	// 5. Enable PLL
	RCC->CR |= RCC_CR_PLLON_Msk;

	// 6. Increate FLASH latency to account for increased clock speed
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_2 | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;

	// 6. Switch system clock to PLLR
	RCC->CFGR |= 0b010 << RCC_CFGR_SW_Pos;
}

/*
 * SystemInit is a special function called by CMSIS during initialization
 */
void SystemInit(void)
{
	init_clock();
	//  Enable clock to SysTick
	RCC->APBENR2 |= RCC_APBENR2_SYSCFGEN;
	// Generate interupt every milisecond
	SysTick_Config(SYS_FREQUENCY / 1000);
}

/*
 * SysTick_Handler is configured as the SysTick interupt handler
 */
volatile uint32_t s_ticks;
void SysTick_Handler(void)
{
	s_ticks++;
}

void delay(unsigned ms)
{
	uint32_t until = s_ticks + ms;
	while (s_ticks < until)
		(void)0;
}
