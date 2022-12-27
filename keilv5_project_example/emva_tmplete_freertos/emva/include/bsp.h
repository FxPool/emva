// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "string.h"

    extern struct hal_gpio bsp_gpio;
    extern struct hal_time bsp_time;
    extern struct hal_usart bsp_usart;
    extern struct hal_arch bsp_arch;
    extern struct hal_usb_hid bsp_usb_hid;

#ifdef __cplusplus
}
#endif

#endif
