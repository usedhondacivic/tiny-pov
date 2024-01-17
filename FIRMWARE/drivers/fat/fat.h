/**
 * Author: Michael Crum
 * Date: 1/8/2023
 *
 * FAT file system driver for the STM32g031.
 * Only supports read operations.
 * Only supports cards with 512 bytes per sector.
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

#pragma once

#include <stdint.h>

/*
 * REFERENCE:
 * Sector Layout
 * Partion table entries
 */
typedef struct PartitionEntry
{
	uint8_t PE_status[1];
	uint8_t PE_CHS_start_addr[3];
	uint8_t PE_type[1];
	uint8_t PE_CHS_end_addr[3];
	uint8_t PE_LBA_addr[4];
	uint8_t PE_num_bytes[4];
} __attribute__((packed)) PartitionEntry;

typedef struct MBR
{
	uint8_t MBR_BootStrap[446];
	PartitionEntry MBR_PartitionEntries[4];
	uint8_t MBR_BootSig[2];
} __attribute__((packed)) MBR;

/*
 * REFERENCE:
 * Section 3: Boot Sector and BPB
 */

typedef struct BS
{
	uint8_t BS_jmpBoot[3];
	uint8_t BS_OEMName[8];
	uint8_t BPB_BytsPerSec[2];
	uint8_t BPB_SecPerClus[1];
	uint8_t BPB_RsvdSecCnt[2];
	uint8_t BPB_NumFATs[1];
	uint8_t BPB_RootEntCnt[2];
	uint8_t BPB_TotSec16[2];
	uint8_t BPB_Media[1];
	uint8_t BPB_FATSz16[2];
	uint8_t BPB_SecPerTrk[2];
	uint8_t BPB_NumHeads[2];
	uint8_t BPB_HiddSec[4];
	uint8_t BPB_TotSec32[4];
	uint8_t BPB_FATSz32[4];
	uint8_t BPB_ExtFlags[2];
	uint8_t BPB_FSVer[2];
	uint8_t BPB_RootClus[4];
	uint8_t BPB_FSInfo[2];
	uint8_t BPB_BkBootSec[2];
	uint8_t BPB_Reserved[12];
	uint8_t BS_DrvNum[1];
	uint8_t BS_Reserved1[1];
	uint8_t BS_BootSig[1];
	uint8_t BS_VolID[4];
	uint8_t BS_VolLab[11];
	uint8_t BS_FilSysType[8];
	uint8_t blank[420];
	uint8_t Signature_word[2];
} __attribute__((packed)) BS;

typedef struct DIR_entry
{
	uint8_t DIR_Name[11];
	uint8_t DIR_Attr[1];
	uint8_t DIR_NTRes[1];
	uint8_t DIR_CrtTimeTenth[1];
	uint8_t DIR_CrtTime[2];
	uint8_t DIR_CrtDate[2];
	uint8_t DIR_LstAccDate[2];
	uint8_t DIR_FstClusHI[2];
	uint8_t DIR_WrtTime[2];
	uint8_t DIR_WrtDate[2];
	uint8_t DIR_FstClusLO[2];
	uint8_t DIR_FileSize[4];
} __attribute__((packed)) DIR_entry;

union FAT_block
{
	BS bs;
	MBR mbr;
	DIR_entry dir_entries[16];
	uint8_t raw_bytes[512];
	uint32_t FAT[16];
};

void init_fat();

void fat_open_file(char *file_name);

uint32_t fat_read_file_block(uint8_t *const data);
