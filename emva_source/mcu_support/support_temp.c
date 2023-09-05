// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include <stdio.h>

// pool init
void emva_mem_pool_init(void)
{
}

// mem malloc
void *emva_mem_pool_malloc(unsigned int size)
{
	return 0x00000000L;
}

// mem free
uint8 emva_mem_pool_free(void *mem_pointer)
{
	return 1;
}

// sleep ms
inline void os_sleep(uint32 nms)
{
	
}

// sleep us
inline void os_sleepus(uint32 nus)
{
	
}

// arch initializes
inline void emva_arch_init(void)
{
	
}


