
// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __HAL_ARCH_H
#define __HAL_ARCH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "emva_bsp_type.h"

    struct hal_arch
    {
        void (*Initialize)(void);
    };

#ifdef __cplusplus
}
#endif

#endif
