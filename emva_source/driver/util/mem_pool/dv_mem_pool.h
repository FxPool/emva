// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#ifndef __DV_MEM_POOL_H
#define __DV_MEM_POOL_H

#ifdef __cplusplus
extern "C"
{
#endif

    extern void dv_mem_pool_init(void);
    extern void *dv_mem_pool_malloc(uint32 size);
    extern uint8 dv_mem_pool_free(void *mem_pointer);

#ifdef __cplusplus
}
#endif

#endif
