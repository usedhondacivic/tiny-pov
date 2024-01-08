/**
 * Author: Michael Crum
 * Date: 1/8/2023
 *
 * FAT file system driver for the STM32g031.
 * Only read operations supported.
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 *
 * FAT Spec:
 * https://academy.cba.mit.edu/classes/networking_communications/SD/FAT.pdf
 */

#include "drivers/fat/fat.h"
#include "drivers/sd/sd.h"

sd_read_data_packet BPB;

void init_fat()
{
	sd_read_block(0x00000000, &BPB);
}

void read_block() {}
