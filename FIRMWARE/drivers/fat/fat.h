/**
 * Author: Michael Crum
 * Date: 1/8/2023
 *
 * FAT file system driver for the STM32g031.
 * Only supports read operations.
 * Only supports cards with 512 bytes per sector.
 *
 * MCU reference manual:
 * https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 *
 * FAT Spec:
 * https://academy.cba.mit.edu/classes/networking_communications/SD/FAT.pdf
 */

#pragma once

#include <stdint.h>

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
} BS;

void init_fat();

void read_block();
