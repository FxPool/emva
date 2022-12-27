// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dv_malloc.h"
#include "dv_mem_pool.h"

inline void dv_mallocInit(void)
{
	dv_mem_pool_init();
}

inline void *dv_malloc(uint32 size)
{
	return dv_mem_pool_malloc(size);
}

inline uint8 dv_free(void *mem_pointer)
{
	return dv_mem_pool_free(mem_pointer);
}
