// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_error.h"
#include "dv_net_device.h"
#include "emva_list.h"
#include "emva_malloc.h"
#include "string.h"

static emva_error *g_err;
static EmvaLink_List *g_netModuleDeviceList = NULL;
static net_driver_t **g_net_module_dev_array = NULL;

int32 net_dev_open(const char *device_name)
{
	if (g_netModuleDeviceList == NULL)
	{
		return -1;
	}
	for (int i = 0; i < g_netModuleDeviceList->length; i++)
	{
		net_driver_t *p = (net_driver_t *)EmvaLink_List_GetAt(g_netModuleDeviceList, i);
		if (strcmp(p->DeviceName, device_name) == 0)
		{
			if (p->ops->ndo_open() != -1)
			{
				return i;
			}
			else
			{
				return -1;
			}
		}
	}
	return -1;
}

int32 net_dev_stop(int32 fd, void *obj)
{
	if (fd < 0)
	{
		return -1;
	}
	if (g_net_module_dev_array)
	{
		return g_net_module_dev_array[fd]->ops->ndo_stop(obj);
	}
	return -1;
}

int32 net_dev_start_xmit(int32 fd, void *obj, void *data, uint32 size)
{
	if (fd < 0)
	{
		return -1;
	}
	if (g_net_module_dev_array)
	{
		return g_net_module_dev_array[fd]->ops->ndo_start_xmit(obj, data, size);
	}
	return -1;
}

int32 net_dev_do_ioctl(int32 fd, void *obj, uint32 cmd, uint32 arg)
{
	if (fd < 0)
	{
		return -1;
	}
	if (g_net_module_dev_array)
	{
		return g_net_module_dev_array[fd]->ops->ndo_do_ioctl(obj, cmd, arg);
	}
	return -1;
}

int32 net_dev_event(int32 fd, void *obj, void (*callback)(void *obj, void *d, uint32 size))
{
	if (fd < 0)
	{
		return -1;
	}
	if (g_net_module_dev_array)
	{
		return g_net_module_dev_array[fd]->ops->ndo_data_bus_input(obj, callback);
	}
	return -1;
}

emva_error *net_dev_device_registered(net_driver_t *ModuleDevice)
{
	if (g_err == NULL)
	{
		g_err = emva_error_new(128);
	}
	if (g_netModuleDeviceList == NULL)
	{
		g_netModuleDeviceList = EmvaLink_List_Init();
	}
	if (g_netModuleDeviceList->length == 0)
	{
		EmvaLink_List_Insert(g_netModuleDeviceList, ModuleDevice, -1);
	}
	else
	{
		for (uint8 i = 0; i < g_netModuleDeviceList->length; i++)
		{
			net_driver_t *p = (net_driver_t *)EmvaLink_List_GetAt(g_netModuleDeviceList, i);
			if (strcmp(p->DeviceName, ModuleDevice->DeviceName) == 0)
			{
				g_err->new (g_err, "[%s]Registration failed. The device name already exists", ModuleDevice->DeviceName);
				return g_err;
			}
		}
		ModuleDevice->DeviceNumber = g_netModuleDeviceList->length;
		EmvaLink_List_Insert(g_netModuleDeviceList, ModuleDevice, -1);
	}
	if (g_net_module_dev_array != NULL)
	{
		emva_free(g_net_module_dev_array);
	}
	g_net_module_dev_array = (net_driver_t **)emva_malloc(sizeof(net_driver_t *) * g_netModuleDeviceList->length);
	for (uint16 i = 0; i < g_netModuleDeviceList->length; i++)
	{
		net_driver_t *p = (net_driver_t *)EmvaLink_List_GetAt(g_netModuleDeviceList, i);
		g_net_module_dev_array[i] = p;
	}

	return NULL;
}

char *net_dev_device_foreach(int32 *DeviceNum, uint32 *Version, uint32 *Date, uint32 *Owner)
{
	static uint16 i = 0;
	if (g_netModuleDeviceList == NULL)
	{
		return NULL;
	}
	net_driver_t *p = (net_driver_t *)EmvaLink_List_GetAt(g_netModuleDeviceList, i++);
	if (i > g_netModuleDeviceList->length)
	{
		i = 0;
		return NULL;
	}
	*DeviceNum = (i - 1);
	*Version = (uint32)&p->Version;
	*Date = (uint32)&p->Date;
	*Owner = (uint32)&p->Owner;
	return p->DeviceName;
}
