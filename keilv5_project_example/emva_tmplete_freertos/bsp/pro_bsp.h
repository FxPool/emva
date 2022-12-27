// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#ifndef __PRO_BSP_H
#define __PRO_BSP_H
#ifdef __cplusplus

extern "C"
{
#endif

struct gps_bsp
{
	void (*init)(void);
	void (*power)(uint8 ctl);
	void (*setlocaltioncallback)(void (*ptr)(uint32 jd,uint32 wd));
};

struct usbhid_bsp
{
	void (*write)(void *dt,uint16 len);
};

struct pro_bsp_interface 
{
	struct gps_bsp *gps;
	struct usbhid_bsp *usbhid;
};

extern struct pro_bsp_interface pro_bsp;

#ifdef __cplusplus
}
#endif

#endif
