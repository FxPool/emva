// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva_framework.h"
#include "emva_malloc.h"
#include "emva_mem_pool.h"

inline void emva_mallocInit(void)
{
    emva_mem_pool_init();
}

inline void *emva_malloc(uint32 size)
{
    return emva_mem_pool_malloc(size);
}

inline uint8 emva_free(void *mem_pointer)
{
    return emva_mem_pool_free(mem_pointer);
}
