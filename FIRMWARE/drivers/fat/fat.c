/**
 * Author: Michael Crum
 * Date: 1/8/2023
 *
 * FAT file system driver for the STM32g031.
 * Only supports read operations.
 * Only supports SDHC/SDXC cards with 512 bytes per sector.
 * Assumes card formatted as FAT32 with one partion and using Master Boot
 * Register partitioning. (This is the default on Ubuntu 22.04.3)
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 *
 * FAT Spec:
 * https://academy.cba.mit.edu/classes/networking_communications/SD/FAT.pdf
 *
 * Master Boot Record reference:
 * https://en.wikipedia.org/wiki/Master_boot_record
 */

#include <stdint.h>

#include "drivers/fat/fat.h"
#include "drivers/sd/sd.h"

#define reverse_bytes_32(num)                                                  \
	(((num & 0xFF000000) >> 24) | ((num & 0x00FF0000) >> 8) |                  \
	 ((num & 0x0000FF00) << 8) | ((num & 0x000000FF) << 24))

// Master Boot Record
union FAT_block mbr;
// Bios Parameter Block
union FAT_block bs;
union FAT_block root_dir_block;

uint32_t first_data_sector;
uint32_t BS_addr;

uint32_t sector_from_cluster(uint32_t clus_num)
{
	return ((clus_num - 2) * bs.bs.BPB_SecPerClus[0]) + first_data_sector;
}

void init_fat()
{
	// Read MBR (always at 0x00000000)
	sd_read_block(0x00000000, mbr.raw_bytes);

	// Calculate the start address of the first partition
	BS_addr = *(uint32_t *)mbr.mbr.MBR_PartitionEntries[0].PE_LBA_addr;

	// Read BS for FAT partition
	sd_read_block(reverse_bytes_32(BS_addr), bs.raw_bytes);

	// Calculate the the first sector of the data region
	first_data_sector = *(uint16_t *)bs.bs.BPB_RsvdSecCnt +
						bs.bs.BPB_NumFATs[0] * (*(uint32_t *)bs.bs.BPB_FATSz32);

	uint32_t root_dir_sector =
	  sector_from_cluster(*(uint32_t *)bs.bs.BPB_RootClus);

	sd_read_block(reverse_bytes_32(BS_addr + root_dir_sector),
				  root_dir_block.raw_bytes);

	while (1) {
	}
}

void read_file(char *file_name, char *file_ext) {}
