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

static int32 ir_open_module(void *obj) { return 0; }
static int32 ir_close_module(void *obj) { return 0; }
static int32 ir_read(void *obj, void *d, uint32 size) { return 0; }
static int32 ir_write(void *obj, const void *d, const uint32 size) { return 0; }
static int32 ir_ioct(void *obj, uint32 cmd, uint32 arg) { return 0; }
static int32 ir_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size)) { return 0; }

characterModuleInterface_t dv_ir_interface = {
    .open_module = ir_open_module,
    .close_module = ir_close_module,
    .read = ir_read,
    .write = ir_write,
    .ioctl = ir_ioct,
    .events = ir_events,
};

character_driver_t ir_driver = {
    .DeviceName = EMVA_IR_DRIVER_NAME,
    .Version = "1.0.0",
    .Date = "2021.10.12",
    .Owner = EMVA_DRIVER_OWNER,
    .ops = &dv_ir_interface,
};

void dv_ir_init(void)
{
    character_dev_device_registered(&ir_driver);
}
#ifndef EMVA_CREATE_LIB
DV_MODULE_REG(dv_ir_init);
#endif
