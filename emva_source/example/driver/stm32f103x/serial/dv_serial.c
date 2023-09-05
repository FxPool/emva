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
#include "emva_config.h"
#include "support_config.h"

#define EMVA_SERIAL_BUFSIZE 128
#define PORT_NAME_RANGE 3

// def type in this
typedef struct
{
    struct emva_kfifo *serial_kffio;
    void *obj;
		void (*call)(void *event, const void *d, uint32 size);
} SerialObj_t;
struct serial_info
{
    uint8 portName;
    uint32 baudRate;
    SerialParityModeU parity;
    SerialDataBitsModeU dataBits;
    SerialStopBitsModeU stopBits;
};

// value define in this
void *usart_obj[PORT_NAME_RANGE];
SerialObj_t serialObj[PORT_NAME_RANGE];
static void UsartWrite(const uint8_t portName, uint8_t d);
void (*usart_callback)(const void *obj, byte d);

// function define in this
void usart1_callback(uint8_t dt){usart_callback(usart_obj[1-1],dt);}
void usart2_callback(uint8_t dt){usart_callback(usart_obj[2-1],dt);}
void usart3_callback(uint8_t dt){usart_callback(usart_obj[3-1],dt);}
static void BspUsartWrite(const uint8_t portName, uint8_t d){UsartWrite(portName,d);}
static void *BspUsartRead(const uint8_t portName){return usart_obj[portName - 1];}
static void BspUsartCallBack(const uint8_t portName, const void *obj, void (*fun)(const void *obj, byte d))
{
	usart_obj[portName] = (void *)obj;
	usart_callback = fun;
}

// mcu irq def in this
void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
	{

		USART_ClearFlag(USART1, USART_FLAG_PE);
				USART_ReceiveData(USART1);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET){

		USART_ClearFlag(USART1, USART_FLAG_ORE);
				USART_ReceiveData(USART1);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
	{

		USART_ClearFlag(USART1, USART_FLAG_FE);
				USART_ReceiveData(USART1);
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		uint8_t c = (uint8_t)USART_ReceiveData(USART1);
		usart1_callback(c);
	}
}

void xUSART2_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)
	{
		USART_ClearFlag(USART2, USART_FLAG_PE);
		USART_ReceiveData(USART2);
	}
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(USART2, USART_FLAG_ORE);
		USART_ReceiveData(USART2);
	}
	if (USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)
	{
		USART_ClearFlag(USART2, USART_FLAG_FE);
		USART_ReceiveData(USART2);
	}
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		uint8_t c = (uint8_t)USART_ReceiveData(USART2);
		usart2_callback(c);
	}
}

void USART3_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)
	{

		USART_ClearFlag(USART3, USART_FLAG_PE);
		USART_ReceiveData(USART3);
	}
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{

		USART_ClearFlag(USART3, USART_FLAG_ORE);
		USART_ReceiveData(USART3);
	}
	if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)
	{

		USART_ClearFlag(USART3, USART_FLAG_FE);
		USART_ReceiveData(USART3);
	}
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		uint8_t c = (uint8_t)USART_ReceiveData(USART3);
		usart3_callback(c);
	}
}

static void UsartWrite(const uint8_t portName, uint8_t d)
{
	switch (portName)
	{
	case 1:

		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, d);
		break;
	case 2:

		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, d);
		break;
	case 3:

		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, d);
		break;
	}
}
// usart initialize def in this
static uint8_t Usart_Initialize(const uint8_t portName, uint32_t baudRate, uint8_t dataBits, uint8_t parity, uint8_t stopBits)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_TypeDef *USARTx;

	switch (portName)
	{
	case 1:

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
#if SERIAL1_TX_SUPPORT > 0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#if SERIAL1_RX_SUPPORT > 0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
		break;
	case 2:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

#if SERIAL2_TX_SUPPORT > 0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#if SERIAL2_RX_SUPPORT > 0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
		break;
	case 3:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

#if SERIAL3_TX_SUPPORT > 0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#if SERIAL3_RX_SUPPORT > 0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
		break;
	}

	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = dataBits;
	USART_InitStructure.USART_StopBits = stopBits;
	USART_InitStructure.USART_Parity = parity;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	switch (portName)
	{
	case 1:
		USARTx = USART1;
		break;
	case 2:
		USARTx = USART2;
		break;
	case 3:
		USARTx = USART3;
		break;
	}
	USART_Init(USARTx, &USART_InitStructure);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
	USART_ITConfig(USARTx, USART_IT_PE, DISABLE);
	USART_ITConfig(USARTx, USART_IT_ERR, DISABLE);
	USART_Cmd(USARTx, ENABLE);
	return 1;
}
// uart callback def in this 
static void serialCallback(const void *obj, byte d)
{
    SerialObj_t *thisSerial = (SerialObj_t *)(obj);
    if (thisSerial == NULL)
        return;
		if (thisSerial->call)
    {
        thisSerial->call(thisSerial->obj, &d, 1);
				return;
    }
		emva_kfifo_put(thisSerial->serial_kffio, &d, 1);
}
// dev interface impl
static int32 serial_open_module(void *obj) { return 0; }
static int32 serial_close_module(void *obj) { return 0; }
static int32 serial_read(void *obj, void *d, uint32 size)
{
    uint8 portName = *(uint8 *)obj;
    void *read = BspUsartRead(portName);
    if (read == NULL)
    {
        return -1;
    }
    SerialObj_t *thisSerial = (SerialObj_t *)(read);
    return emva_kfifo_get(thisSerial->serial_kffio, d, size);
}
static int32 serial_write(void *obj, const void *d, const uint32 size)
{
    uint8 portName = *(uint8 *)obj;
    byte *dt = (byte *)d;
    for (uint32 i = 0; i < size; i++)
        BspUsartWrite(portName, *dt++);
    return 0;
}
static int32 serial_ioct(void *obj, uint32 cmd, uint32 arg)
{
    struct serial_info serial_i;
    serial_i = *(struct serial_info *)(arg);
	  if(serial_i.portName<1|| serial_i.portName>PORT_NAME_RANGE)
		{
			return -1;
		}
    Usart_Initialize(serial_i.portName, serial_i.baudRate, (uint8)serial_i.dataBits, (uint8)serial_i.parity, (uint8)serial_i.stopBits);
		serial_i.portName = serial_i.portName-1;
		// obj init
    serialObj[serial_i.portName].serial_kffio = (struct emva_kfifo *)emva_malloc(sizeof(struct emva_kfifo));
    serialObj[serial_i.portName].obj = obj;
    byte *this_kbuf = (byte *)emva_malloc(sizeof(byte) * EMVA_SERIAL_BUFSIZE);
    emva_kfifo_init(serialObj[serial_i.portName].serial_kffio, this_kbuf, EMVA_SERIAL_BUFSIZE);
		// callback set
    BspUsartCallBack(serial_i.portName, (void *)&serialObj[serial_i.portName], serialCallback);
    return 0;
}
static int32 serial_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size))
{
		uint8 port_name = *(uint8*)obj;
		if(port_name<1|| port_name>PORT_NAME_RANGE)
		{
			return -1;
		}
		serialObj[port_name-1].call = fuc;
    return 0;
}

characterModuleInterface_t dv_serial_interface = {
    .open_module = serial_open_module,
    .close_module = serial_close_module,
    .read = serial_read,
    .write = serial_write,
    .ioctl = serial_ioct,
    .events = serial_events,
};

character_driver_t serial_driver = {
    .DeviceName = EMVA_SERIAL_DRIVER_NAME,
    .Version = "1.0.0",
    .Date = "2021.10.12",
    .Owner = EMVA_DRIVER_OWNER,
    .ops = &dv_serial_interface,
};

void dv_serial_init(void)
{
    character_dev_device_registered(&serial_driver);
}

DV_MODULE_REG(dv_serial_init);
