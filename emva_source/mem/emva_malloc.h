// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_MALLOC_H
#define __EMVA_MALLOC_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "emva_malloc.h"
#include "emva_arch.h"

	extern inline void emva_mallocInit(void)
	{
		emva_mem_pool_init();
	}

	extern inline void *emva_malloc(uint32 size)
	{
		return emva_mem_pool_malloc(size);
	}

	extern inline uint8 emva_free(void *mem_pointer)
	{
		return emva_mem_pool_free(mem_pointer);
	}
#ifdef __cplusplus
}
#endif

#endif
