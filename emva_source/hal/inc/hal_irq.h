// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __HAL_IRQ_H
#define __HAL_IRQ_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "emva_bsp_type.h"
    struct hal_irq_nrf24l01
    {
        void (*request)(void);
        void (*set_callback)(void (*fuc)(void));
    };
    struct hal_irq_button
    {
        void (*request)(uint16 pin);
        void (*set_callback)(void (*fuc)(uint8 key));
        uint8 (*read_pin_state)(uint16 pin);
    };
    struct hal_irq
    {
        struct hal_irq_nrf24l01 nrf24l01;
        struct hal_irq_button button;
    };

#ifdef __cplusplus
}
#endif

#endif
