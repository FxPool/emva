// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "stm32f10x.h"
#include "dv_stm32f10x.h"
#include "dev_init.h"
#include "dv_character_device.h"
#include "emva_kfifo.h"
#include "emva_malloc.h"
#include "string.h"
#include "emva_map.h"
#include "util.h"
#include "emva_config.h"

#define BUTTON1    0
typedef struct
{
	GPIO_TypeDef* GPIO;
	uint16_t Pin;
}Stm32ButtonGpioMap_t;
CemvaMap *ButtonMap;
void (*gEventCall)(void *event, const void *d, uint32 size);
void (*buttonIrqCall)(u8);

/**
  * @brief  
  * @param  
  *   
  * @param  
  * @retval 
  */
Stm32ButtonGpioMap_t Button_Map[] = 
{
	{GPIOA,GPIO_Pin_15},//BUTTON1
};
/**
  * @brief  
  * @param  
  *   
  * @param  
  * @retval 
  */
void EXTI15_10_IRQHandler(void )
{
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		buttonIrqCall(BUTTON1);
	}
	EXTI_ClearITPendingBit(EXTI_Line15);
}
/**
  * @brief  
  * @param  
  *   
  * @param  
  * @retval 
  */
static u8 ButtonGetPinState(u8 index)
{
	return GPIO_ReadInputDataBit(Button_Map[index].GPIO,Button_Map[index].Pin);
}

/**
  * @brief  
  * @param  
  *   
  * @param  
  * @retval 
  */
static void stm32_buttonInit(u8 KeyNum)
{
	/*°´¼üÖÐ¶ÏÅäÖÃ*/
	GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	switch (KeyNum)
	{
		case BUTTON1:
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
			
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin =  Button_Map[BUTTON1].Pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_Init(Button_Map[KeyNum].GPIO, &GPIO_InitStructure);
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line15;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);
			break;
	}
}

/**
  * @brief  
  * @param  
  *   
  * @param  
  * @retval 
  */
static void ButtonIrqRegister(void (*ptr)(u8))
{
	buttonIrqCall = ptr;
}

static int32 button_open_module(void *obj)
{
    if (ButtonMap == NULL)
        ButtonMap = CMapCalss.new();
    return 0;
}
static int32 button_close_module(void *obj) { return 0; }
static int32 button_read(void *obj, void *d, uint32 size)
{
	u8 index = *(u8 *)obj;
	*(u8*)d = ButtonGetPinState(index);
	return 0;
}
static int32 button_write(void *obj, const void *d, const uint32 size) { return 0; }
static int32 button_ioct(void *obj, uint32 cmd, uint32 arg) { return 0; }
static void buttonCallback(uint8 key)
{
    void *v = (void *)ButtonMap->Get(ButtonMap, (uint8 *)&key, 1);
    if (gEventCall)
    {
        gEventCall(v, NULL, 0);
    }
}
static int32 button_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size))
{
    int16 KeyNum = *(int16 *)obj;
    CdMap *BMap = (CdMap *)emva_malloc(sizeof(CdMap));
    BMap->key = (uint8 *)emva_malloc(sizeof(uint8));
    if (BMap != NULL && BMap->key != NULL)
    {
        *(uint8 *)BMap->key = KeyNum;
        BMap->value = obj;
        ButtonMap->Add(ButtonMap, BMap);
        stm32_buttonInit(KeyNum);
        ButtonIrqRegister(buttonCallback);
        gEventCall = fuc;
        KeyNum++;
    }
    return 0;
}

characterModuleInterface_t dv_button_interface = {
    .open_module = button_open_module,
    .close_module = button_close_module,
    .read = button_read,
    .write = button_write,
    .ioctl = button_ioct,
    .events = button_events,
};

character_driver_t button_driver = {
    .DeviceName = EMVA_BUTTON_DRIVER_NAME,
    .Version = "1.0.0",
    .Date = "2021.10.12",
    .Owner = EMVA_DRIVER_OWNER,
    .ops = &dv_button_interface,
};

void dv_button_init(void)
{
    character_dev_device_registered(&button_driver);
}
DV_MODULE_REG(dv_button_init);

