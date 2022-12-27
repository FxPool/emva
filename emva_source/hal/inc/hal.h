// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#ifndef __HAL_H
#define __HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "emva_bsp_type.h"
#include "hal_gpio.h"
#include "hal_irq.h"
#include "hal_time.h"
#include "hal_pwr.h"
#include "hal_usart.h"
#include "hal_usb_hid.h"
#include "hal_arch.h"

    typedef struct _hal hal_t;

    struct _hal
    {
        struct hal_gpio *gpio;
        struct hal_time *time;
        struct hal_usart *usart;
        struct hal_arch *arch;
        struct hal_irq *irq;
        struct hal_pwr *pwr;
        struct hal_usb_hid *usb_hid;
    };

    extern hal_t hal;

#ifdef __cplusplus
}
#endif

#endif
