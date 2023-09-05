// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

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
#include "dv_character_device.h"
#include "dv_net_device.h"
#include "emva_iterator.h"
#include "string.h"
#include "emva_config.h"

struct emva_api emva;
void emvaApiInit(void)
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
#if EMVA_FM_OBSERVER_ENABLED == 1
  emva.ObserverClass = EmvaObserver;
#endif
  emva.SerializationClass = EmvaSerializationClass;
#if EMVA_FM_SERIAL_ENABLED == 1
  emva.FmtClass = EmvafmtClass;
#endif
#if EMVA_FM_NET_ENABLED == 1
  emva.Net = EmvaNet;
#endif
#if EMVA_FM_NET_ENABLED == 1
  emva.Driver.LoadNetDriver = EmvaNetInitialize;
#endif
#if EMVA_FM_WIFI_ENABLED == 1
  emva.Driver.LoadWifiDriver = EmvaWifiInitialize;
#endif
}

void emvaDriverIteratorInit(void)
{
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
}

void emvaDeviceInit(void (*call)(uint16 code, const char *info))
{
  EMVA_BOOL OK = FALSE;
#if EMVA_FM_TIME_ENABLED == 1
  // fm time init
  OK = EmvaTimeInitialize();
  if (call)
  {
    (OK == FALSE) ? call(0x2555, "emva time init [fail]") : call(0, "emva time init [ok]");
  }
#endif
#if EMVA_FM_SERIAL_ENABLED == 1
  // fm serial init
  OK = EmvaSerialInitialize();
  if (call)
  {
    (OK == FALSE) ? call(0x2556, "emva serial init [fail]") : call(0, "emva serial init [ok]");
  }
#endif
#if EMVA_FM_WIFI_ENABLED == 1
  // fm wifi support
  OK = EmvaWifiInitialize(EMVA_WIFI_DRIVER_NAME);
  if (call)
  {
    (OK == FALSE) ? call(0x2557, "emva wifi init [fail]") : call(0, "emva wifi init [ok]");
  }
#endif
#if EMVA_FM_NET_ENABLED == 1
  // fm net support
  OK = EmvaNetInitialize(EMVA_NET_DRIVER_NAME);
  if (call)
  {
    (OK == FALSE) ? call(0x2558, "emva net init [fail]") : call(0, "emva net init [ok]");
  }
#endif
#if EMVA_FM_BUTTON_ENABLED == 1
  // fm button support
  OK = EmvaButtonInitialize();
  if (call)
  {
    (OK == FALSE) ? call(0x2559, "emva button init [fail]") : call(0, "emva button init [ok]");
  }
#endif
#if EMVA_PMIC_PMIC_ENABLED == 1
  // fm pmic support
  OK = EmvaPmicInitialize();
  if (call)
  {
    (OK == FALSE) ? call(0x255a, "emva pmic init [fail]") : call(0, "emva pmic init [ok]");
  }
#endif
}

void emvaIniaialize(void)
{
  // arch init
  emva_arch_init();
	
	// mem init
	emva_mallocInit();

  // driver module manage init
  drmanage_init();

  // emva api init
  emvaApiInit();
}
