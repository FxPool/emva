// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __DV_MALLOC_H
#define __DV_MALLOC_H

#include "hal.h"
#include "dv_malloc.h"
#include "dv_mem_pool.h"
#ifdef __cplusplus
extern "C"
{
#endif
    extern inline void dv_mallocInit(void)
    {
        dv_mem_pool_init();
    }
    extern inline void *dv_malloc(uint32 size)
    {
        return dv_mem_pool_malloc(size);
    }
    extern inline uint8 dv_free(void *mem_pointer)
    {
        return dv_mem_pool_free(mem_pointer);
    }

#ifdef __cplusplus
}
#endif

#endif
