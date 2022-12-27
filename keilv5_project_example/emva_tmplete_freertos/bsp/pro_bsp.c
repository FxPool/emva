// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "util.h"
#include "string.h"
#include "pro_bsp.h"
#include "dv_character_device.h"

typedef struct
{
	char Date_State;
	uint32 jd;
	uint32 wd;
} gpspro_t;

static int32 g_fd;
static bool g_isInit = FALSE;
static gpspro_t gpspro;
void (*gpsdatacallback)(uint32 jd,uint32 wd);
static void gps_power(uint8 ctl)
{
	character_dev_ioctl(g_fd,NULL,ctl,NULL);
}
static void gps_event(void *event, const void *d, uint32 size)
{
	uint8 e = *(uint8*)event;
	if(e == 1)
	{
		gpspro = *(gpspro_t*)d;
		//update
		gpsdatacallback(gpspro.jd,gpspro.wd);
	}else{
		//history
		gpsdatacallback(gpspro.jd,gpspro.wd);
	}
}
static void gps_init(void)
{
	g_fd = character_dev_open("zd_gps", NULL);
	if (g_fd == -1)
	{
			g_isInit = FALSE;
			return;
	}
	g_isInit = TRUE;
}
static void gps_setcallback(void (*ptr)(uint32 jd,uint32 wd))
{
	if(g_isInit == TRUE)
	{
		gpsdatacallback = ptr;
		character_dev_event(g_fd,NULL,gps_event);
	}
}

static void usbhid_write(void *dt,uint16 len)
{
	emva.UsbHid.tranmission((void*)dt,len);
}

struct gps_bsp gps_interface = 
{
	.init = gps_init,
	.power = gps_power,
	.setlocaltioncallback = gps_setcallback,
};

struct usbhid_bsp usbhid_interface = 
{
	.write = usbhid_write,
};

struct pro_bsp_interface pro_bsp = 
{
	.gps = &gps_interface,
	.usbhid = &usbhid_interface,
};
