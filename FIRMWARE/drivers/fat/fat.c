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

#include <stdint.h>

#include "drivers/fat/fat.h"
#include "drivers/sd/sd.h"

union FAT_block BPB;

union FAT_block root_dir_block;

uint32_t big_to_little_endian(uint32_t big_endian)
{

	return ((big_endian >> 24) & 0xff) |	  // move byte 3 to byte 0
		   ((big_endian << 8) & 0xff0000) |	  // move byte 1 to byte 2
		   ((big_endian >> 8) & 0xff00) |	  // move byte 2 to byte 1
		   ((big_endian << 24) & 0xff000000); // byte 0 to byte 3
}

void init_fat()
{
	sd_read_block(0x00000000, BPB.raw_bytes);
	// uint32_t root_dir = 2;
	sd_read_block(0x00000000, root_dir_block.raw_bytes);
	// sd_read_block(root_dir * 512, (sd_read_data_packet *)root_dir_block);
}

void read_file(char *file_name, char *file_ext) {}
