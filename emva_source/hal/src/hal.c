// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "hal.h"
#include "emva_bsp.h"

struct _hal hal = {
    .gpio = &bsp_gpio,
    .time = &bsp_time,
    .usart = &bsp_usart,
    .arch = &bsp_arch,
    .usb_hid = &bsp_usb_hid,
    .irq = &bsp_irq,
};

struct _hal *hal_new(void)
{
    return &hal;
}
