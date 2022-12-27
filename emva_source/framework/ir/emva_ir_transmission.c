// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org
#include "emva_framework.h"
#include "emva_usb_hid.h"
#include "emva_malloc.h"
#include "dv_character_device.h"
#include "emva_ir_transmission.h"
#include "string.h"
#if EMVA_FM_IR_ENABLED == 1
void (*IrDataCallback)(const void *data, uint16 l);
static void IrEventCallback(void *event, const void *d, uint32 l)
{
    if (IrDataCallback)
    {
        IrDataCallback(d, l);
    }
}
static EMVA_BOOL IrLoadDriverByName(EmvaIrTranmission *this, const char *name)
{
    this->g_fd = character_dev_open(name, NULL);
    if (this->g_fd == -1)
    {
        return FALSE;
    }
    return TRUE;
}
static EMVA_BOOL IrWrite(EmvaIrTranmission *this, const void *data, uint16 len)
{
    return character_dev_write(this->g_fd, NULL, (void *)data, len) == -1 ? FALSE : TRUE;
}
static EMVA_BOOL IrSetCallback(EmvaIrTranmission *this, void (*ptr)(const void *data, uint16 l))
{
    return character_dev_event(this->g_fd, NULL, IrEventCallback) == -1 ? FALSE : TRUE;
}
static EmvaIrTranmission *EmvaIrApiNew(void)
{
    EmvaIrTranmission *eip = (EmvaIrTranmission *)emva_malloc(sizeof(EmvaIrTranmission));
    eip->loadDriver = IrLoadDriverByName;
    eip->write = IrWrite;
    eip->callback = IrSetCallback;
    return eip;
}

struct EmvaIrTransmitApi EmvaIrTransmitClass = {
    .new = EmvaIrApiNew,
};
#endif
