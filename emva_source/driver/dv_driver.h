// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __DV_DRIVER_H
#define __DV_DRIVER_H

#include "hal.h"
#include "dv_malloc.h"
#include "dv_mem_pool.h"
#include "emva_os.h"
#ifdef __cplusplus
extern "C"
{
#endif
    extern inline void dv_sleep(uint32 nms)
    {
        os_sleep(nms);
    }
    extern inline void dv_sleepus(uint32 nus)
    {
        os_sleepus(nus);
    }
#ifdef __cplusplus
}
#endif

#endif
