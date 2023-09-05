// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "stm32f10x.h"
#include "dv_stm32f10x.h"
#include "dev_init.h"
#include "dv_character_device.h"
#include "emva_malloc.h"
#include "string.h"
#include "emva_config.h"
#include "support_config.h"

void (*eventCallbackSet)(void *event, const void *d, uint32 size);
void (*time_callback0)(void);
void (*time_callback1)(void);
inline void time2_callback(void)
{
	if(time_callback0)time_callback0();
	if(time_callback1)time_callback1();
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	
	{
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		time2_callback();
	}
}
void Time2Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 2 - 1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM2, DISABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}
void Time2Ctl(uint8_t en)
{
	if(en == 1){
		TIM_Cmd(TIM2,ENABLE);
	}else if(en ==0){
		TIM_Cmd(TIM2,DISABLE);
	}
}

static void BspTimeInitialize(uint8_t Timex, uint32_t timePeriod) {if(Timex==0 || Timex == 1)Time2Init();}
static void BspTimeControl(uint8_t Timex, EMVA_BOOL c) {
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

static void timeCallback(void)
{
    if (eventCallbackSet)
        eventCallbackSet(NULL, NULL, 0);
}

static int32 time_open_module(void *obj)
{
    BspTimeInitialize(0, 1000);
    return 0;
}
static int32 time_close_module(void *obj) { return -1; }
static int32 time_read(void *obj, void *d, uint32 size) { return -1; }
static int32 time_write(void *obj, const void *d, const uint32 size) { return -1; }
static int32 time_ioct(void *obj, uint32 cmd, uint32 arg)
{
    BspTimeControl(0, (EMVA_BOOL)cmd);
    return 0;
}
static int32 time_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size))
{
    eventCallbackSet = fuc;
    BspTimeCallBack(0, timeCallback);
    return 0;
}

characterModuleInterface_t dv_time_interface = {
    .open_module = time_open_module,
    .close_module = time_close_module,
    .read = time_read,
    .write = time_write,
    .ioctl = time_ioct,
    .events = time_events,
};

character_driver_t time_driver = {
    .DeviceName = EMVA_TIME_DRIVER_NAME,
    .Version = "1.0.0",
    .Date = "2021.10.12",
    .Owner = EMVA_DRIVER_OWNER,
    .ops = &dv_time_interface,
};

void dv_time_init(void)
{
    character_dev_device_registered(&time_driver);
}
DV_MODULE_REG(dv_time_init);
