// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_USB_HID_H
#define __EMVA_USB_HID_H

#ifdef __cplusplus
extern "C"
{
#endif
    struct EmvaUsbHidApi
    {
        void (*init)(void);
        void (*tranmission)(const void *d, uint16 l);
        void (*setCall)(void (*ptr)(const void *d, uint16 l));
    };
    extern struct EmvaUsbHidApi EmvaUsbHid;
    extern void EmvaUsbHidInitialize(void);
#ifdef __cplusplus
}
#endif

#endif
