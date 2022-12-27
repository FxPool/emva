// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "hal.h"
#include "emva.h"
#include "dev_init.h"
#include "emva_time.h"
#include "emva_serial.h"
#include "emva_button.h"
#include "emva_observer.h"
#include "emva_net.h"
#include "emva_wifi.h"
#include "emva_pmic.h"
#include "emva_serialization.h"
#include "emva_fmt.h"
#include "emva_usb_hid.h"
#include "dv_character_device.h"
#include "dv_net_device.h"
#include "iterator.h"
#include "string.h"
#include "emva_config.h"

struct emva_api emva;
void EmvaApiInit(void)
{
#if EMVA_FM_SERIAL_ENABLED == 1
  emva.SerialClass = EmvaSerialClass;
#endif
#if EMVA_FM_BUTTON_ENABLED == 1
  emva.ButtonClass = EmvaButtonClass;
#endif
#if EMVA_PMIC_PMIC_ENABLED == 1
  emva.PmicClass = EmvaPmicClass;
#endif
#if EMVA_FM_WIFI_ENABLED == 1
  emva.WifiClass = EmvaWifiClass;
#endif
#if EMVA_FM_TIME_ENABLED == 1
  emva.TimeClass = EmvaTimeClass;
#endif
  emva.IteratorClass = EmvaEmvaIteratorApiClass;
  emva.ObserverClass = EmvaObserver;
  emva.SerializationClass = EmvaSerializationClass;
#if EMVA_FM_SERIAL_ENABLED == 1
  emva.FmtClass = EmvafmtClass;
#endif
#if EMVA_FM_IR_ENABLED == 1
  emva.IrTransmitClass = EmvaIrTransmitClass;
#endif
#if EMVA_FM_USBHID_ENABLED == 1
  emva.UsbHid = EmvaUsbHid;
#endif
#if EMVA_FM_NET_ENABLED == 1
  emva.Net = EmvaNet;
#endif
  AggregateInterface *CharDriverNameAg = emva.IteratorClass.new();
  AggregateInterface *NetDriverNameAg = emva.IteratorClass.new();
  emva.Driver.IteratorCdName = CharDriverNameAg->iterator(CharDriverNameAg);
  emva.Driver.IteratorNdName = NetDriverNameAg->iterator(NetDriverNameAg);
  char *driverName;
  int32 driverNumber = -1;
  uint32 versionPaddr = 0, datePaddr = 0, ownerPaddr = 0;
  // add char driver name to ag
  do
  {
    driverName = character_dev_device_foreach(&driverNumber, &versionPaddr, &datePaddr, &ownerPaddr);
    struct EmvaDriverInfo *driverInfo = (struct EmvaDriverInfo *)emva_malloc(sizeof(struct EmvaDriverInfo));
    driverInfo->DeviceName = driverName;
    driverInfo->DeviceNum = driverNumber;
    driverInfo->Version = (char *)versionPaddr;
    driverInfo->Date = (char *)datePaddr;
    driverInfo->Owner = (char *)ownerPaddr;
    if (driverName)
      CharDriverNameAg->ContainerAdd(CharDriverNameAg, (byte *)driverInfo, sizeof(struct EmvaDriverInfo));
  } while (driverName != NULL);
  // add net driver name to ag
  do
  {
    driverName = net_dev_device_foreach(&driverNumber, &versionPaddr, &datePaddr, &ownerPaddr);
    struct EmvaDriverInfo *driverInfo = (struct EmvaDriverInfo *)emva_malloc(sizeof(struct EmvaDriverInfo));
    driverInfo->DeviceName = driverName;
    driverInfo->DeviceNum = driverNumber;
    driverInfo->Version = (char *)versionPaddr;
    driverInfo->Date = (char *)datePaddr;
    driverInfo->Owner = (char *)ownerPaddr;
    if (driverName)
      NetDriverNameAg->ContainerAdd(NetDriverNameAg, (byte *)driverInfo, sizeof(struct EmvaDriverInfo));
  } while (driverName != NULL);
#if EMVA_FM_NET_ENABLED == 1
  emva.Driver.LoadNetDriver = EmvaNetInitialize;
#endif
#if EMVA_FM_WIFI_ENABLED == 1
  emva.Driver.LoadWifiDriver = EmvaWifiInitialize;
#endif
}

#ifdef EMVA_CREATE_LIB
void kernel_driver_init(void)
{
  extern void dv_time_init(void);
  extern void dv_serial_init(void);
  extern void dvTimeVirtualInit(void);
  extern void dv_button_init(void);
  extern void dv_pmic_init(void);
  extern void dv_usb_hid_init(void);
  extern void dv_ir_init(void);
  dv_time_init();
  dvTimeVirtualInit();
  dv_serial_init();
  dv_button_init();
  dv_pmic_init();
  dv_ir_init();
  dv_usb_hid_init();
}
#endif

void emvaIniaialize(void)
{
  // arch init
  hal.arch->Initialize();
#ifdef EMVA_CREATE_LIB
  // kernel driver module init
  kernel_driver_init();
#endif
  // driver module init
  DvModuleInitialize();
#if EMVA_FM_TIME_ENABLED == 1
  // fm time init
  EmvaTimeInitialize();
#endif
#if EMVA_FM_SERIAL_ENABLED == 1
  // fm serial init
  EmvaSerialInitialize();
#endif
#if EMVA_FM_WIFI_ENABLED == 1
  // fm wifi support
  EmvaWifiInitialize(EMVA_WIFI_DRIVER_NAME);
#endif
#if EMVA_FM_NET_ENABLED == 1
  // fm net support
  EmvaNetInitialize(EMVA_NET_DRIVER_NAME);
#endif
#if EMVA_FM_BUTTON_ENABLED == 1
  // fm button support
  EmvaButtonInitialize();
#endif
#if EMVA_PMIC_PMIC_ENABLED == 1
  // fm pmic support
  EmvaPmicInitialize();
#endif
#if EMVA_FM_USBHID_ENABLED == 1
  // fm usb hid support
  EmvaUsbHidInitialize();
#endif
  // emva api init
  EmvaApiInit();
}
