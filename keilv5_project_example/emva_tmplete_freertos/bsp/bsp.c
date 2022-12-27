// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "bsp.h"
#include "hal_gpio.h"
#include "hal_time.h"
#include "hal_usart.h"
#include "hal_arch.h"
#include "hal_usb_hid.h"
#include "arch_stm32f411xx.h"
#include "custom_config.h"
/////////////////////////////////////////////////////////////////////////////////////////
struct hal_gpio bsp_gpio = {
	.write.pin0 = NULL,
};
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
void (*time_callback0)(void);
void (*time_callback1)(void);
inline void time2_callback(void)
{
	if(time_callback0)time_callback0();
	if(time_callback1)time_callback1();
}
static void BspTimeInitialize(uint8_t Timex, uint32_t timePeriod) {if(Timex==0 || Timex == 1)Time2Init();}
static void BspTimeControl(uint8_t Timex, bool c) {
	if(Timex==0 || Timex == 1){
			if(c==TRUE)Time2Ctl(1);else if(c==FALSE)Time2Ctl(0);
		}
}
static void BspTimeCallBack(uint8_t Timex, void (*fuc)(void)) {
	if (Timex==0 )
	{
		time_callback0 = fuc;
	}
	if(Timex  ==1)
	{
		time_callback1 = fuc;
	}
}
struct hal_time bsp_time = {
	.Initialize = BspTimeInitialize,
	.Control = BspTimeControl,
	.CallBack = BspTimeCallBack,
};
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
#define PORT_NAME_RANGE 3
void *usart_obj[PORT_NAME_RANGE];
void (*usart_callback)(const void *obj, byte d);
void usart1_callback(uint8_t dt){if(usart_callback)usart_callback(usart_obj[1-1],dt);}
void usart2_callback(uint8_t dt){if(usart_callback)usart_callback(usart_obj[2-1],dt);}
void usart3_callback(uint8_t dt){if(usart_callback)usart_callback(usart_obj[3-1],dt);}
static bool BspUsartInitialize(const uint8_t portName, uint32_t baudRate, uint8_t dataBits, uint8_t parity, uint8_t stopBits){
	if (portName == 0 || portName > 3)
	{
		return FALSE;
	}
	return (bool)Stm32F411Usart_Initialize(portName,baudRate,dataBits,parity,stopBits);
}
static void BspUsartWrite(const uint8_t portName, uint8_t d){Stm32F411Usart_Write(portName,d);}
static void *BspUsartRead(const uint8_t portName){return usart_obj[portName - 1];}
static void BspUsartCallBack(const uint8_t portName, const void *obj, void (*fun)(const void *obj, byte d))
{
	usart_obj[portName - 1] = (void *)obj;
	usart_callback = fun;
}
struct hal_usart bsp_usart = {
	.Initialize = BspUsartInitialize,
	.Write = BspUsartWrite,
	.Read = BspUsartRead,
	.CallBack = BspUsartCallBack,
};
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////

static void UsbHidInit(void){}
static void UsbHidWrite(const void *d, uint16 l){}
static void UsbHidCallBack(void (*ptr)(const void *d, uint16 l)){}
struct hal_usb_hid bsp_usb_hid = {
	.Initialize = UsbHidInit,
	.Write = UsbHidWrite,
	.CallBack = UsbHidCallBack,
};

//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////

static void archInitialize(void)
{
	Stm32F411_mcuInit();
}
struct hal_arch bsp_arch = {
	.Initialize = archInitialize,
};
//////////////////////////////////////////////////////////////////////////////////////////




