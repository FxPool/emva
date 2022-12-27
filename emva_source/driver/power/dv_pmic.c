// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dev_init.h"
#include "dv_character_device.h"
#include "dv_kfifo.h"
#include "dv_malloc.h"
#include "string.h"
#include "util.h"
#include "emva_config.h"

#define PMIC_REBOOT 1
#define PMIC_SHUTDOWN 2
#define PMIC_SLEEP 3
#define PMIC_STANDBY 4
#define PMIC_STOP 5

static int32 pmic_open_module(void *obj) { return 0; }
static int32 pmic_close_module(void *obj) { return 0; }
static int32 pmic_read(void *obj, void *d, uint32 size)
{

    return 0;
}
static int32 pmic_write(void *obj, const void *d, const uint32 size) { return 0; }
static int32 pmic_ioct(void *obj, uint32 cmd, uint32 arg)
{
    switch (cmd)
    {
    case 1:
        hal.pwr->lowpower.reboot();
        break;
    case 2:
        hal.pwr->lowpower.shutdown();
        break;
    case 3:
        hal.pwr->lowpower.sleep();
        break;
    case 4:
        hal.pwr->lowpower.standby();
        break;
    case 5:
        hal.pwr->lowpower.stop();
        break;
    default:
        return -1;
    }
    return 0;
}
static int32 pmic_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size)) { return 0; }

characterModuleInterface_t dv_pmic_interface = {
    .open_module = pmic_open_module,
    .close_module = pmic_close_module,
    .read = pmic_read,
    .write = pmic_write,
    .ioctl = pmic_ioct,
    .events = pmic_events,
};

character_driver_t pmic_driver = {
    .DeviceName = EMVA_POWER_DRIVER_NAME,
    .Version = "1.0.0",
    .Date = "2021.10.12",
    .Owner = EMVA_DRIVER_OWNER,
    .ops = &dv_pmic_interface,
};

void dv_pmic_init(void)
{
    character_dev_device_registered(&pmic_driver);
}
#ifndef EMVA_CREATE_LIB
DV_MODULE_REG(dv_pmic_init);
#endif
