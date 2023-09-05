// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_serial.h"
#include "emva_malloc.h"
#include "emva_kfifo.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <stdio.h>
#include "dv_character_device.h"
#include "emva_config.h"

#if EMVA_FM_SERIAL_ENABLED == 1
#define DT_BROKEN_FRAME_TIME 15

static int32 g_fd;
static EMVA_BOOL g_isInit = FALSE;
struct serial_info
{
    uint8 portName;
    uint32 baudRate;
    SerialParityModeU parity;
    SerialDataBitsModeU dataBits;
    SerialStopBitsModeU stopBits;
};

static void serialDataCall(void *arg)
{
    EmvaSerial *this = (EmvaSerial *)arg;
    if (this->dt_event)
    {
        this->dt_event(DataCallBackEvent);
    }
    if (this->dt_call)
    {
        this->dt_call(this->dtBuffer, this->dtLength);
        this->time->stop(this->time);
    }
    this->dtLength = 0;
}

static void event_callback(void *v, const void *d, uint32 len)
{
    //Add a piece of data to FIFO and generate a callback event
    EmvaSerial *e = (EmvaSerial *)v;
		if(e->dtBuffer)
		{
			e->dtBuffer[e->dtLength++] = *(byte *)d;
			e->time->start(e->time, v, serialDataCall, DT_BROKEN_FRAME_TIME);
		}
}

static void serialOpen(EmvaSerial *this)
{
    struct serial_info serial_i;
    serial_i.portName = this->portName;
    serial_i.baudRate = this->baudRate;
    serial_i.parity = this->parity;
    serial_i.dataBits = this->dataBits;
    serial_i.stopBits = this->stopBits;
    if (this->time == NULL)
    {
        this->time = EmvaTimeClass.new();
    }
    character_dev_ioctl(g_fd, this, NULL, (uint32)&serial_i);
}
static void serialClose(EmvaSerial *this)
{
    emva_free(this->dtBuffer);
    emva_free(this);
}
static void serialWrite(EmvaSerial *this, byte *dt, uint32 length)
{
    character_dev_write(g_fd, this, dt, length);
}
static void serialRead(EmvaSerial *this, byte *dt, int32 nread, int32 *nrecv)
{
    do
    {
        *nrecv = character_dev_read(g_fd, this, dt, nread);
        emva_sleep(2);
    } while (*nrecv == 0);
}
static void serialEvent(EmvaSerial *this, void (*fuc)(SerialEventU Sevent))
{
    this->dt_event = fuc;
}
static void serialCallBack(EmvaSerial *this, void (*fuc)(byte *dt, uint32 length))
{
    if (this->dtBuffer == NULL)
    {
        this->dt_call = fuc;
        this->dtBuffer = (byte *)emva_malloc(sizeof(byte) * this->dtLength);
        this->dtLength = 0;
        character_dev_event(g_fd, this, event_callback);
    }
}

EmvaSerial *newSerial(void)
{
    EmvaSerial *pSerial = (EmvaSerial *)emva_malloc(sizeof(EmvaSerial));
    if (g_isInit == FALSE || pSerial == NULL)
    {
        return NULL;
    }
    pSerial->open = serialOpen;
    pSerial->close = serialClose;
    pSerial->write = serialWrite;
    pSerial->read = serialRead;
    pSerial->set_event_call = serialEvent;
    pSerial->set_callBack = serialCallBack;
    pSerial->time = NULL;
    pSerial->dtBuffer = NULL;
    return pSerial;
}

struct EmvaSerialApi EmvaSerialClass = {
    .new = newSerial,
};

EMVA_BOOL EmvaSerialInitialize(void)
{
    g_fd = character_dev_open(EMVA_SERIAL_DRIVER_NAME, NULL);
    g_isInit = (g_fd == -1) ? FALSE : TRUE;
    return g_isInit;
}
#endif
