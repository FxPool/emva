// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dev_init.h"
#include "dv_character_device.h"
#include "dv_malloc.h"
#include "string.h"
#include "dv_virtual_time.h"
#include "emva_config.h"

void (*eventCallbackSet)(void *event, const void *d, uint32 size);
static void timeCallback(void)
{
    if (eventCallbackSet)
        eventCallbackSet(NULL, NULL, 0);
}

static int32 time_open_module(void *obj)
{
    hal.time->Initialize(0, 1000);
    return 0;
}
static int32 time_close_module(void *obj) { return -1; }
static int32 time_read(void *obj, void *d, uint32 size) { return -1; }
static int32 time_write(void *obj, const void *d, const uint32 size) { return -1; }
static int32 time_ioct(void *obj, uint32 cmd, uint32 arg)
{
    hal.time->Control(0, (EMVA_BOOL)cmd);
    return 0;
}
static int32 time_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size))
{
    eventCallbackSet = fuc;
    hal.time->CallBack(0, timeCallback);
    return 0;
}

characterModuleInterface_t dv_time_interface = {
    .open_module = time_open_module,
    .close_module = time_close_module,
    .read = time_read,
    .write = time_write,
    .ioctl = time_ioct,
    .events = time_events,
};

character_driver_t time_driver = {
    .DeviceName = EMVA_TIME_DRIVER_NAME,
    .Version = "1.0.0",
    .Date = "2021.10.12",
    .Owner = EMVA_DRIVER_OWNER,
    .ops = &dv_time_interface,
};

void dv_time_init(void)
{
    character_dev_device_registered(&time_driver);
}
#ifndef EMVA_CREATE_LIB
DV_MODULE_REG(dv_time_init);
#endif
