// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dev_init.h"
#include "dv_character_device.h"
#include "dv_kfifo.h"
#include "dv_malloc.h"
#include "string.h"
#include "emva_config.h"

#define EMVA_SERIAL_BUFSIZE 128

typedef struct
{
    struct dv_kfifo *serial_kffio;
    void *Eventobj;
} SerialObj_t;

typedef enum
{
    Parity_No = 0,
    Parity_Even,
    Parity_Odd,
} SerialParityModeU;
typedef enum
{
    StopBits_1 = 0,
    StopBits_0_5,
    StopBits_2,
    StopBits_1_5,
} SerialStopBitsModeU;
typedef enum
{
    WordLength_8b = 0,
    WordLength_9b,
} SerialDataBitsModeU;

struct serial_info
{
    uint8 portName;
    uint32 baudRate;
    SerialParityModeU parity;
    SerialDataBitsModeU dataBits;
    SerialStopBitsModeU stopBits;
};

void (*g_eventCallbackSet)(void *event, const void *d, uint32 size);
SerialObj_t serialObj;
static void serialCallback(const void *obj, byte d)
{
    SerialObj_t *thisSerial = (SerialObj_t *)(obj);
    if (thisSerial == NULL)
        return;
    dv_kfifo_put(thisSerial->serial_kffio, &d, 1);
    if (g_eventCallbackSet)
    {
        g_eventCallbackSet(thisSerial->Eventobj, &d, 1);
    }
}
static int32 serial_open_module(void *obj) { return 0; }
static int32 serial_close_module(void *obj) { return 0; }
static int32 serial_read(void *obj, void *d, uint32 size)
{
    uint8 portName = *(uint8 *)obj;
    void *read = hal.usart->Read(portName);
    if (read == NULL)
    {
        return -1;
    }
    SerialObj_t *thisSerial = (SerialObj_t *)(read);
    return dv_kfifo_get(thisSerial->serial_kffio, d, size);
}
static int32 serial_write(void *obj, const void *d, const uint32 size)
{
    uint8 portName = *(uint8 *)obj;
    byte *dt = (byte *)d;
    for (uint32 i = 0; i < size; i++)
        hal.usart->Write(portName, *dt++);
    return 0;
}
static int32 serial_ioct(void *obj, uint32 cmd, uint32 arg)
{
    struct serial_info serial_i;
    serial_i = *(struct serial_info *)(arg);
    hal.usart->Initialize(serial_i.portName, serial_i.baudRate, (uint8)serial_i.dataBits, (uint8)serial_i.parity, (uint8)serial_i.stopBits);
    serialObj.serial_kffio = (struct dv_kfifo *)dv_malloc(sizeof(struct dv_kfifo));
    serialObj.Eventobj = obj;
    byte *this_kbuf = (byte *)dv_malloc(sizeof(byte) * EMVA_SERIAL_BUFSIZE);
    dv_kfifo_init(serialObj.serial_kffio, this_kbuf, EMVA_SERIAL_BUFSIZE);
    hal.usart->CallBack(serial_i.portName, (void *)&serialObj, serialCallback);
    return 0;
}
static int32 serial_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size))
{
    g_eventCallbackSet = fuc;
    return 0;
}

characterModuleInterface_t dv_serial_interface = {
    .open_module = serial_open_module,
    .close_module = serial_close_module,
    .read = serial_read,
    .write = serial_write,
    .ioctl = serial_ioct,
    .events = serial_events,
};

character_driver_t serial_driver = {
    .DeviceName = EMVA_SERIAL_DRIVER_NAME,
    .Version = "1.0.0",
    .Date = "2021.10.12",
    .Owner = EMVA_DRIVER_OWNER,
    .ops = &dv_serial_interface,
};

void dv_serial_init(void)
{
    character_dev_device_registered(&serial_driver);
}
#ifndef EMVA_CREATE_LIB
DV_MODULE_REG(dv_serial_init);
#endif
