// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "bsp_type.h"
#include "bsp_configure.h"
#include "emva_mem_pool.h"
#include "dv_mem_pool.h"

uint8 eg_dv_mem_pool[DV_MEM_SIZE];
#if defined(__CC_ARM) || defined(__CLANG_ARM) /* ARMCC Complier */
__align(8) uint8 g_dv_membase[DV_MEM_SIZE];
#endif
uint16 g_dvmmt[DV_MMTS];
void dv_mem_pool_init(void)
{
	uint32 i = 0;
	for (i = 0; i < DV_MMTS; i++)
	{
		g_dvmmt[i] = 0;
	}
}

void *dv_mem_pool_malloc(uint32 size)
{
	uint32 need_blocks = 0;
	uint32 free_blocks = 0;
	uint32 blocks = 0;
	uint32 i;

	need_blocks = size / DV_MEM_BANK_MIN_SIZE;
	if (size % DV_MEM_BANK_MIN_SIZE)
		need_blocks++;
	for (i = 0; i < DV_MMTS; i++)
	{
		if (g_dvmmt[i] == 0)
			free_blocks++;
		else
			free_blocks = 0;
		if (need_blocks == free_blocks)
		{
			i = (i + 1) - free_blocks;
			for (blocks = i; blocks < (i + free_blocks); blocks++)
			{
				g_dvmmt[blocks] = 1;
			}
			g_dvmmt[i] = need_blocks;
			return (g_dv_membase + i * DV_MEM_BANK_MIN_SIZE);
		}
	}
	return 0x00000000L;
}

uint8 dv_mem_pool_free(void *mem_pointer)
{
	uint32 free;
	uint32 num;
	uint32 i;
	free = (uint32)mem_pointer - (uint32)&g_dv_membase[0];
	free /= DV_MEM_BANK_MIN_SIZE;
	num = g_dvmmt[free];
	for (i = free; i < free + num; i++)
	{
		g_dvmmt[i] = 0;
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////

uint8 eg_emva_mem_pool[EMVA_MEM_SIZE];
#if defined(__CC_ARM) || defined(__CLANG_ARM) /* ARMCC Complier */
__align(8) uint8 g_emva_membase[EMVA_MEM_SIZE];
#endif
uint16 g_emvammt[EMVA_MMTS];
void emva_mem_pool_init(void)
{
	uint32 i = 0;
	for (i = 0; i < EMVA_MMTS; i++)
	{
		g_emvammt[i] = 0;
	}
}

void *emva_mem_pool_malloc(unsigned int size)
{
	unsigned int need_blocks = 0;
	unsigned int free_blocks = 0;
	unsigned int blocks = 0;
	unsigned int i;

	need_blocks = size / EMVA_MEM_BANK_MIN_SIZE;
	if (size % EMVA_MEM_BANK_MIN_SIZE)
		need_blocks++;
	for (i = 0; i < EMVA_MMTS; i++)
	{
		if (g_emvammt[i] == 0)
			free_blocks++;
		else
			free_blocks = 0;
		if (need_blocks == free_blocks)
		{
			i = (i + 1) - free_blocks;
			for (blocks = i; blocks < (i + free_blocks); blocks++)
			{
				g_emvammt[blocks] = 1;
			}
			g_emvammt[i] = need_blocks;
			return (g_emva_membase + i * EMVA_MEM_BANK_MIN_SIZE);
		}
	}
	return 0x00000000L;
}

uint8 emva_mem_pool_free(void *mem_pointer)
{
	uint32 free;
	uint32 num;
	uint32 i;
	free = (uint32)mem_pointer - (uint32)&g_emva_membase[0];
	free /= EMVA_MEM_BANK_MIN_SIZE;
	num = g_emvammt[free];
	for (i = free; i < free + num; i++)
	{
		g_emvammt[i] = 0;
	}
	return 1;
}
