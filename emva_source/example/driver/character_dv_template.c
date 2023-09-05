// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "dev_init.h"
#include "dv_character_device.h"
#include "emva_kfifo.h"
#include "emva_malloc.h"
#include "string.h"
#include "emva_map.h"
#include "util.h"

static int32 template_open_module(void *obj) {return 0;}
static int32 template_close_module(void *obj) { return 0; }
static int32 template_read(void *obj, void *d, uint32 size) { return 0; }
static int32 template_write(void *obj, const void *d, const uint32 size) { return 0; }
static int32 template_ioct(void *obj, uint32 cmd, uint32 arg) { return 0; }
static int32 template_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size)) { return 0; }

characterModuleInterface_t dv_template_interface = {
    .open_module = template_open_module,
    .close_module = template_close_module,
    .read = template_read,
    .write = template_write,
    .ioctl = template_ioct,
    .events = template_events,
};

character_driver_t template_driver = {
    .DeviceName = "dv_template",
		.Owner = "xxx",
		.Version = "1.0.0",
		.Date = "2023.07.06",
    .ops = &dv_template_interface,
};

void dv_template_init(void)
{
    character_dev_device_registered(&template_driver);
}

DV_MODULE_REG(dv_template_init);

