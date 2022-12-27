// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __HAL_PWR_H
#define __HAL_PWR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "emva_bsp_type.h"
 
    struct hal_irq_lowpower
    {
        void (*shutdown)(void);
        void (*reboot)(void);
        void (*sleep)(void);
        void (*standby)(void);
        void (*stop)(void);
    };
    struct hal_pwr
    {
        struct hal_irq_lowpower lowpower;
    };

#ifdef __cplusplus
}
#endif

#endif
