// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __DV_NET_DEVICE_H_
#define __DV_NET_DEVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct _hs_net_interface hs_net_interface_t;

  int net_dev_open(const char *device_name);
  int net_dev_stop(int fd, void *obj);
  int net_dev_start_xmit(int fd, void *obj, void *d, uint32 len);
  int net_dev_do_ioctl(int fd, void *obj, uint32 cmd, uint32 arg);
  int net_dev_event(int fd, void *obj, void (*ptr)(void *obj, void *d, uint32 len));

  struct _hs_net_interface
  {
    int (*ndo_open)(void);
    int (*ndo_stop)(void *obj);
    int (*ndo_start_xmit)(void *obj, void *d, uint32 len);
    int (*ndo_do_ioctl)(void *obj, uint32 cmd, uint32 arg);
    int (*ndo_data_bus_input)(void *obj, void (*ptr)(void *obj, void *d, uint32 len));
  };

  typedef struct
  {
    char *DeviceName;
    int32 DeviceNumber;
    char *Date;
    char *Version;
    char *Owner;
    hs_net_interface_t *ops;
  } net_driver_t;

  emva_error *net_dev_device_registered(net_driver_t *ModuleDevice);
  char *net_dev_device_foreach(int32 *DeviceNum, uint32 *Version, uint32 *Date, uint32 *Owner);

#ifdef __cplusplus
}
#endif
#endif
