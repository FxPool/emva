// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __HAL_USART_H
#define __HAL_USART_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "emva_bsp_type.h"

    struct hal_usart
    {
        EMVA_BOOL (*Initialize)(const uint8 portName, uint32 baudRate, uint8 dataBits, uint8 parity, uint8 stopBits);
        void (*Write)(const uint8 portName, byte dt);
        void *(*Read)(const uint8 portName);
        void (*CallBack)(const uint8 portName, const void *obj, void (*fun)(const void *obj, byte d));
    };

#ifdef __cplusplus
}
#endif

#endif
