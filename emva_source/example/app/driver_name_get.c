// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "emva_time.h"
#include "emva_serial.h"
#include "limits.h"
#include "string.h"

EmvaSerial *se;
void fmtout(void *dt)
{
    se->write(se, dt, strlen((char *)dt));
}
// Device init info
static void emvaDeviceInitInfo(uint16 code, const char *info)
{
    if (code != 0)
    {
    }
}
int main(void)
{
    emvaIniaialize();
    emvaDeviceInit(emvaDeviceInitInfo);
    se = emva.SerialClass.new();
    se->portName = 1;
    se->baudRate = 115200;
    se->parity = Parity_No;
    se->dataBits = WordLength_8b;
    se->stopBits = StopBits_1;
    se->open(se);
    emvaFmt *fmt = emva.FmtClass.new(128);
    fmt->io = fmtout;
    while (emva.Driver.IteratorCdName->hasNext(emva.Driver.IteratorCdName))
    {
        struct EmvaDriverInfo *driverInfo = (struct EmvaDriverInfo *)emva.Driver.IteratorCdName->next(emva.Driver.IteratorCdName)->data;
        fmt->println(fmt, "name:%s,dev_num:%d,version:%s,date:%s,owner:%s", driverInfo->DeviceName, driverInfo->DeviceNum, driverInfo->Version, driverInfo->Date, driverInfo->Owner);
    }
    for (;;)
    {
        os_sleep(100);
    }
}
