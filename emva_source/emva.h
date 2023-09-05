// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_H
#define __EMVA_H

#ifdef __cplusplus
extern "C"
{
#endif
#define EMVA_WIFI_DRIVER_NAME "EMVA_WIFI"
#define EMVA_NET_DRIVER_NAME "EMVA_NET"
#define EMVA_SERIAL_DRIVER_NAME "EMVA_SERIAL"
#define EMVA_TIME_DRIVER_NAME "EMVA_TIME"
#define EMVA_BUTTON_DRIVER_NAME "EMVA_BUTTON"
#define EMVA_USB_HID_DRIVER_NAME "EMVA_USBHID"
#define EMVA_POWER_DRIVER_NAME "EMVA_PMIC"
#define EMVA_IR_DRIVER_NAME "EMVA_IR"
#include "emva_config.h"
#include "emva_arch.h"
#include "emva_time.h"
#include "emva_serial.h"
#include "emva_button.h"
#include "emva_observer.h"
#include "emva_net.h"
#include "emva_wifi.h"
#include "emva_pmic.h"
#include "emva_iterator.h"
#include "emva_serialization.h"
#include "emva_fmt.h"
#include "emva_malloc.h"
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
                EMVA_BOOL (*LoadWifiDriver)(const char *name);
        };
        struct emva_api
        {
                struct EmvaDriver Driver;
#if EMVA_FM_SERIAL_ENABLED == 1
                struct EmvaSerialApi SerialClass;
#endif
#if EMVA_FM_BUTTON_ENABLED == 1
                struct EmvaButtonApi ButtonClass;
#endif
#if EMVA_PMIC_PMIC_ENABLED == 1
                struct EmvaPmicApi PmicClass;
#endif
#if EMVA_FM_TIME_ENABLED == 1
                struct EmvaTimeApi TimeClass;
#endif
#if EMVA_FM_WIFI_ENABLED == 1
                struct EmvaWifiApi WifiClass;
#endif
                struct EmvaIteratorApi IteratorClass;
#if EMVA_FM_OBSERVER_ENABLED == 1
                struct EmvaObserverApi ObserverClass;
#endif
                struct EmvaSerializationApi SerializationClass;
#if EMVA_FM_SERIAL_ENABLED == 1
                struct EmvaFmtApi FmtClass;
#endif
#if EMVA_FM_NET_ENABLED == 1
                Dialer Net;
#endif
        };
        extern struct emva_api emva;
        extern void emvaIniaialize(void);
        extern void emvaDeviceInit(void (*call)(uint16 code, const char *info));
				extern void emvaDriverIteratorInit(void);
#ifdef __cplusplus
}
#endif

#endif
