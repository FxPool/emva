// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dev_init.h"
#include "dv_character_device.h"
#include "dv_kfifo.h"
#include "dv_malloc.h"
#include "string.h"
#include "dv_map.h"
#include "util.h"
#include "emva_config.h"

void (*usb_hid_event_call)(void *event, const void *d, uint32 size);
static void usb_hid_call(const void *data, uint16 length)
{
    if (usb_hid_event_call)
    {
        usb_hid_event_call(NULL, data, length);
    }
}

static int32 usb_hid_open_module(void *obj)
{
    return 0;
}
static int32 usb_hid_close_module(void *obj) { return 0; }
static int32 usb_hid_read(void *obj, void *d, uint32 size) { return 0; }
static int32 usb_hid_write(void *obj, const void *d, const uint32 size)
{
    hal.usb_hid->Write(d, size);
    return 0;
}
static int32 usb_hid_ioct(void *obj, uint32 cmd, uint32 arg)
{
    if (cmd == 1)
    {
        hal.usb_hid->Initialize();
    }
    return 0;
}
static int32 usb_hid_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size))
{
    usb_hid_event_call = fuc;
    hal.usb_hid->CallBack(usb_hid_call);
    return 0;
}

characterModuleInterface_t dv_usb_hid_interface = {
    .open_module = usb_hid_open_module,
    .close_module = usb_hid_close_module,
    .read = usb_hid_read,
    .write = usb_hid_write,
    .ioctl = usb_hid_ioct,
    .events = usb_hid_events,
};

character_driver_t usb_hid_driver = {
    .DeviceName = EMVA_USB_HID_DRIVER_NAME,
    .Version = "1.0.0",
    .Date = "2021.10.12",
    .Owner = EMVA_DRIVER_OWNER,
    .ops = &dv_usb_hid_interface,
};

void dv_usb_hid_init(void)
{
    character_dev_device_registered(&usb_hid_driver);
}
#ifndef EMVA_CREATE_LIB
DV_MODULE_REG(dv_usb_hid_init);
#endif
