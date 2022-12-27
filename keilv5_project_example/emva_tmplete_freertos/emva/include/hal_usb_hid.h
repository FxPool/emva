// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __HAL_USB_HID_H
#define __HAL_USB_HID_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_type.h"

    struct hal_usb_hid
    {
        void (*Initialize)(void);
        void (*Write)(const void *d, uint16 l);
        void (*CallBack)(void (*ptr)(const void *d, uint16 l));
    };

#ifdef __cplusplus
}
#endif

#endif
