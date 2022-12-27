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

CDvMap *ButtonMap;
void (*gEventCall)(void *event, const void *d, uint32 size);
static int32 button_open_module(void *obj)
{
    if (ButtonMap == NULL)
        ButtonMap = CMapCalss.new();
    return 0;
}
static int32 button_close_module(void *obj) { return 0; }
static int32 button_read(void *obj, void *d, uint32 size)
{
    return hal.irq->button.read_pin_state(*(int16 *)obj);
}
static int32 button_write(void *obj, const void *d, const uint32 size) { return 0; }
static int32 button_ioct(void *obj, uint32 cmd, uint32 arg) { return 0; }
static void buttonCallback(uint8 key)
{
    void *v = (void *)ButtonMap->Get(ButtonMap, (uint8 *)&key, 1);
    if (gEventCall)
    {
        gEventCall(NULL, v, sizeof(v));
    }
}
static int32 button_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size))
{
    int16 KeyNum = *(int16 *)obj;
    CdMap *BMap = NULL;
    BMap = (CdMap *)dv_malloc(sizeof(CdMap));
    BMap->key = (uint8 *)dv_malloc(1);
    if (BMap != NULL && BMap->key != NULL)
    {
        *(uint8 *)BMap->key = KeyNum;
        BMap->value = obj;
        ButtonMap->Add(ButtonMap, BMap);
        hal.irq->button.request(KeyNum);
        hal.irq->button.set_callback(buttonCallback);
        gEventCall = fuc;
        KeyNum++;
    }
    return 0;
}

characterModuleInterface_t dv_button_interface = {
    .open_module = button_open_module,
    .close_module = button_close_module,
    .read = button_read,
    .write = button_write,
    .ioctl = button_ioct,
    .events = button_events,
};

character_driver_t button_driver = {
    .DeviceName = EMVA_BUTTON_DRIVER_NAME,
    .Version = "1.0.0",
    .Date = "2021.10.12",
    .Owner = EMVA_DRIVER_OWNER,
    .ops = &dv_button_interface,
};

void dv_button_init(void)
{
    character_dev_device_registered(&button_driver);
}
#ifndef EMVA_CREATE_LIB
DV_MODULE_REG(dv_button_init);
#endif
