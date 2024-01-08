/**
 * Author: Michael Crum
 * Date: 1/5/2023
 *
 * SD card driver for the STM32g031
 * Operates an SD card in SPI Mode
 * Only supports SD v2.00 or later
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 *
 * SD Spec:
 * https://www.sdcard.org/downloads/pls/pdf/?p=Part1_Physical_Layer_Simplified_Specification_Ver9.10.jpg&f=Part1PhysicalLayerSimplifiedSpecificationVer9.10Fin_20231201.pdf&e=EN_SS9_1
 */

#pragma once

#include "stm32g031xx.h"
#include <stdbool.h>

typedef struct sd_read_data_packet
{
	uint8_t data_token[1];
	uint8_t data_block[512];
	uint8_t crc[2];
} sd_read_data_packet;

bool init_sd(SPI_TypeDef *chan);

void sd_read_block(uint32_t loc, sd_read_data_packet *packet);
