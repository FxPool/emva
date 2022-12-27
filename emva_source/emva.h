// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_H
#define __EMVA_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "emva_config.h"
#include "emva_bsp_type.h"
#include "emva_time.h"
#include "emva_serial.h"
#include "emva_button.h"
#include "emva_observer.h"
#include "emva_net.h"
#include "emva_wifi.h"
#include "emva_pmic.h"
#include "iterator.h"
#include "emva_serialization.h"
#include "emva_fmt.h"
#include "emva_usb_hid.h"
#include "emva_ir_transmission.h"
#include "emva_malloc.h"

    void emvaIniaialize(void);
    struct EmvaDriverInfo
    {
        char *DeviceName;
        int32 DeviceNum;
        char *Date;
        char *Version;
        char *Owner;
    };
    struct EmvaDriver
    {
        IteratorInterface *IteratorCdName;
        IteratorInterface *IteratorNdName;
        EMVA_BOOL(*LoadNetDriver)
        (const char *name);
        void (*LoadWifiDriver)(const char *name);
    };
    struct emva_api
    {
        struct EmvaDriver Driver;
        struct EmvaSerialApi SerialClass;
        struct EmvaButtonApi ButtonClass;
        struct EmvaPmicApi PmicClass;
        struct EmvaTimeApi TimeClass;
        struct EmvaWifiApi WifiClass;
        struct EmvaIteratorApi IteratorClass;
        struct EmvaObserverApi ObserverClass;
        struct EmvaSerializationApi SerializationClass;
        struct EmvaFmtApi FmtClass;
        struct EmvaIrTransmitApi IrTransmitClass;
        struct EmvaUsbHidApi UsbHid;
        Dialer Net;
    };
    extern struct emva_api emva;
#ifdef __cplusplus
}
#endif

#endif
