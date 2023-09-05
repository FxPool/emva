// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __DV_CHARACTER_DEVICE_H_
#define __DV_CHARACTER_DEVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif
  typedef struct
  {

    int32 (*open_module)(void *obj);
    int32 (*close_module)(void *obj);
    int32 (*read)(void *obj, void *d, uint32 size);
    int32 (*write)(void *obj, const void *d, const uint32 size);
    int32 (*ioctl)(void *obj, uint32 cmd, uint32 arg);
    int32 (*events)(void *obj, void (*fuc)(void *event, const void *d, uint32 size));

  } characterModuleInterface_t;

  typedef struct
  {
    char *DeviceName;
    int32 DeviceNumber;
    char *Date;
    char *Version;
    char *Owner;
    characterModuleInterface_t *ops;
  } character_driver_t;

  int32 character_dev_open(const char *device_name, void *obj);
  int32 character_dev_close(int32 fd, void *obj);
  int32 character_dev_read(int32 fd, void *obj, void *data, uint32 size);
  int32 character_dev_write(int32 fd, void *obj, void *data, uint32 size);
  int32 character_dev_ioctl(int32 fd, void *obj, uint32 cmd, uint32 arg);
  int32 character_dev_event(int32 fd, void *obj, void (*callback)(void *event, const void *d, uint32 size));
  emva_error *character_dev_device_registered(character_driver_t *ModuleDevice);
  EMVA_BOOL character_dev_device_remove(const char *name);
  char *character_dev_device_foreach(int32 *DeviceNum, uint32 *Version, uint32 *Date, uint32 *Owner);
#ifdef __cplusplus
}
#endif
#endif
