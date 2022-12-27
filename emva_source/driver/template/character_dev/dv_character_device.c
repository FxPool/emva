// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dv_error.h"
#include "dv_character_device.h"
#include "dv_list.h"
#include "dv_malloc.h"
#include "string.h"
static dv_error *g_err;
DvLink_List *g_characterModuleDeviceList = NULL;
character_driver_t **g_characterModuleDevArray = NULL;

int32 character_dev_open(const char *device_name, void *obj)
{
	if (g_characterModuleDeviceList == NULL)
	{
		return -1;
	}
	for (int32 i = 0; i < g_characterModuleDeviceList->length; i++)
	{
		character_driver_t *p = (character_driver_t *)DvLink_List_GetAt(g_characterModuleDeviceList, i);
		if (strcmp(p->DeviceName, device_name) == 0)
		{
			p->ops->open_module(obj);
			return i;
		}
	}

	return -1;
}

int32 character_dev_close(int32 fd, void *obj)
{
	if (fd < 0)
	{
		return -1;
	}
	if (g_characterModuleDevArray)
	{
		return g_characterModuleDevArray[fd]->ops->close_module(obj);
	}
	return -1;
}

int32 character_dev_read(int32 fd, void *obj, void *data, uint32 size)
{
	if (fd < 0)
	{
		return -1;
	}
	if (g_characterModuleDevArray)
	{
		return g_characterModuleDevArray[fd]->ops->read(obj, data, size);
	}
	return -1;
}

int32 character_dev_write(int32 fd, void *obj, void *data, uint32 size)
{
	if (fd < 0)
	{
		return -1;
	}
	if (g_characterModuleDevArray)
	{
		return g_characterModuleDevArray[fd]->ops->write(obj, data, size);
	}
	return -1;
}

int32 character_dev_ioctl(int32 fd, void *obj, uint32 cmd, uint32 arg)
{
	if (fd < 0)
	{
		return -1;
	}
	if (g_characterModuleDevArray)
	{
		return g_characterModuleDevArray[fd]->ops->ioctl(obj, cmd, arg);
	}
	return -1;
}

int32 character_dev_event(int32 fd, void *obj, void (*callback)(void *event, const void *d, uint32 size))
{
	if (fd < 0)
	{
		return -1;
	}
	if (g_characterModuleDevArray)
	{
		return g_characterModuleDevArray[fd]->ops->events(obj, callback);
	}
	return -1;
}

dv_error *character_dev_device_registered(character_driver_t *ModuleDevice)
{
	if (g_err == NULL)
	{
		g_err = dv_error_new(128);
	}
	if (g_characterModuleDeviceList == NULL)
	{
		g_characterModuleDeviceList = DvLink_List_Init();
	}
	if (g_characterModuleDeviceList->length == 0)
	{
		DvLink_List_Insert(g_characterModuleDeviceList, ModuleDevice, -1);
		return NULL;
	}
	else
	{
		for (uint8 i = 0; i < g_characterModuleDeviceList->length; i++)
		{
			character_driver_t *p = (character_driver_t *)DvLink_List_GetAt(g_characterModuleDeviceList, i);
			if (strcmp(p->DeviceName, ModuleDevice->DeviceName) == 0)
			{
				g_err->new (g_err, "[%s]Registration failed. The device name already exists", ModuleDevice->DeviceName);
				return g_err;
			}
		}
		ModuleDevice->DeviceNumber = g_characterModuleDeviceList->length;
		DvLink_List_Insert(g_characterModuleDeviceList, ModuleDevice, -1);
	}
	if (g_characterModuleDevArray != NULL)
	{
		dv_free(g_characterModuleDevArray);
	}
	g_characterModuleDevArray = (character_driver_t **)dv_malloc(sizeof(character_driver_t *) * g_characterModuleDeviceList->length);
	for (uint16 i = 0; i < g_characterModuleDeviceList->length; i++)
	{
		character_driver_t *p = (character_driver_t *)DvLink_List_GetAt(g_characterModuleDeviceList, i);
		g_characterModuleDevArray[i] = p;
	}
	return NULL;
}

EMVA_BOOL character_dev_device_remove(const char *name)
{
	for (uint8 i = 0; i < g_characterModuleDeviceList->length; i++)
	{
		character_driver_t *p = (character_driver_t *)DvLink_List_GetAt(g_characterModuleDeviceList, i);
		if (strcmp(p->DeviceName, name) == 0)
		{
			DvLink_List_RemoveAt(g_characterModuleDeviceList, i);
			return TRUE;
		}
	}
	return FALSE;
}

char *character_dev_device_foreach(int32 *DeviceNum, uint32 *Version, uint32 *Date, uint32 *Owner)
{
	static uint16 i = 0;
	if (g_characterModuleDeviceList == NULL)
	{
		return NULL;
	}
	character_driver_t *p = (character_driver_t *)DvLink_List_GetAt(g_characterModuleDeviceList, i++);
	if (i > g_characterModuleDeviceList->length)
	{
		i = 0;
		return NULL;
	}
	*DeviceNum = (i - 1);
	*Version = (uint32)p->Version;
	*Date = (uint32)p->Date;
	*Owner = (uint32)p->Owner;
	return p->DeviceName;
}
