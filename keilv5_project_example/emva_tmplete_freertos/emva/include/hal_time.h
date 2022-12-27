// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __HAL_TIME_H
#define __HAL_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_type.h"

    struct hal_time
    {
        void (*Initialize)(uint8 Timex, uint32 timePeriod);
        void (*Control)(uint8 Timex, bool c);
        void (*CallBack)(uint8 Timex, void (*fuc)(void));
    };

#ifdef __cplusplus
}
#endif

#endif
