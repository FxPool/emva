// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva_framework.h"
#include "emva_usb_hid.h"
#include "emva_malloc.h"
#include "dv_character_device.h"
#include "string.h"
#include "emva_config.h"

#if EMVA_FM_USBHID_ENABLED == 1
static int32 g_fd = -1;
void (*UsbHidDataCall)(const void *d, uint16 l);
static void UsbEventCall(void *event, const void *d, uint32 size)
{
    if (UsbHidDataCall)
    {
        UsbHidDataCall(d, size);
    }
}
static void usbHidInit(void)
{
    character_dev_ioctl(g_fd, NULL, 1, NULL);
}
static void usbHidtranmission(const void *d, uint16 l)
{
    character_dev_write(g_fd, NULL, (void *)d, l);
}
static void usbHidsetCall(void (*ptr)(const void *d, uint16 l))
{
    UsbHidDataCall = ptr;
    character_dev_event(g_fd, NULL, UsbEventCall);
}
struct EmvaUsbHidApi EmvaUsbHid = {
    .init = usbHidInit,
    .tranmission = usbHidtranmission,
    .setCall = usbHidsetCall,
};

void EmvaUsbHidInitialize(void)
{
    g_fd = character_dev_open(EMVA_USB_HID_DRIVER_NAME, NULL);
    if (g_fd == -1)
    {
        return;
    }
}
#endif
