/**
 * Author: Michael Crum
 * Date: 1/8/2023
 *
 * A limited FAT file system driver for the STM32g031.
 *
 * To make the code smaller, simpler, and faster, I make the following
 * consessions:
 *
 * Only supports read operations.
 * Only supports a single level file hierarchy
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

// Master Boot Record
union FAT_block mbr;
// Bios Parameter Block
union FAT_block bs;

uint32_t first_data_block;
uint32_t BS_addr;

uint32_t block_num_from_cluster(uint32_t clus_num)
{
	return ((clus_num - 2) * bs.bs.BPB_SecPerClus[0]) + first_data_block;
}

uint32_t fat_entry_for_cluster(uint32_t clus_num)
{
	uint32_t FATOffset = clus_num * 4;
	uint32_t FATSecNum = (FATOffset / *(uint16_t *)bs.bs.BPB_BytsPerSec);
	uint32_t FATEntOffset = FATOffset % *(uint16_t *)bs.bs.BPB_BytsPerSec;
	union FAT_block fat_sec;
	sd_read_block(BS_addr + *(uint16_t *)bs.bs.BPB_RsvdSecCnt + FATSecNum,
				  fat_sec.raw_bytes);
	return fat_sec.FAT[FATEntOffset];
}

void init_fat()
{
	// Read MBR (always at 0x00000000)
	sd_read_block(0x00000000, mbr.raw_bytes);

	// Calculate the start address of the first partition
	BS_addr = *(uint32_t *)mbr.mbr.MBR_PartitionEntries[0].PE_LBA_addr;

	// Read BS for FAT partition
	sd_read_block(BS_addr, bs.raw_bytes);

	// Calculate the the first sector of the data region
	first_data_block = BS_addr + *(uint16_t *)bs.bs.BPB_RsvdSecCnt +
					   bs.bs.BPB_NumFATs[0] * (*(uint32_t *)bs.bs.BPB_FATSz32);
}

bool file_name_comp(char *a, char *b)
{
	for (int i = 0; i < 7; i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}

uint32_t find_file_start_cluster(char *file_name)
{
	uint32_t curr_root_dir_cluster = *(uint32_t *)bs.bs.BPB_RootClus;
	do {
		union FAT_block root_dir;
		sd_read_block(block_num_from_cluster(curr_root_dir_cluster),
					  root_dir.raw_bytes);
		curr_root_dir_cluster = fat_entry_for_cluster(curr_root_dir_cluster);
		for (int i = 0; i < 512 / 32; i++) {
			DIR_entry file = root_dir.dir_entries[i];
			if (file_name_comp(file_name, (char *)&(file.DIR_Name))) {
				// ew
				return *(uint32_t *)((uint8_t[]){ file.DIR_FstClusLO[0],
												  file.DIR_FstClusLO[1],
												  file.DIR_FstClusHI[0],
												  file.DIR_FstClusHI[1] });
			}
		}
	} while (curr_root_dir_cluster < 0xFFFFFF6);
	return 0;
}

/*
 * file_name must be an 11 character string following the FAT spec's "short"
 * file name format (6.1).
 */

void fat_read_file(char *file_name)
{
	uint32_t start_cluster = find_file_start_cluster(file_name);

	volatile union FAT_block root_dir;
	sd_read_block(block_num_from_cluster(start_cluster), root_dir.raw_bytes);
	while (1)
		;
}
