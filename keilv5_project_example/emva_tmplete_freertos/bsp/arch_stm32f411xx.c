/**
  ******************************************************************************
  * @file    hs_syatem.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    24-1-2019
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.twc.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2019 HansenTech</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_iwdg.h"
#include <stdio.h>
#include "arch_stm32f411xx.h" 
#include "FreeRTOS.h"
#include "task.h"

#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)
static u8  fac_us=0;
static u16 fac_ms=0;
extern void xPortSysTickHandler(void);

/*中断管理项----------------------------------------------------------------------*/
//异常中断管理
void MemManage_Handler(void)
{
	if (CoreDebug->DHCSR & 1) {
      __breakpoint(0);
  }  
	while(1);
}
//硬件中断
void HardFault_Handler(void)
{
	NVIC_SystemReset();
}

//systick中断服务函数,使用os时用到
void SysTick_Handler(void)
{	
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();	
    }
}

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval : None
  */
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
//串口二中断
void USART2_IRQHandler(void)
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
// 定时器二
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	
	{
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		time2_callback();
	}
}

/*延时管理----------------------------------------------------------------------*/
// 延时初始化
void delay_init(u8 SYSCLK)
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//选择外部时钟  HCLK
	fac_us=SYSCLK;				//不论是否使用OS,fac_us都需要使用
	reload=SYSCLK;				//每秒钟的计数次数 单位为M  
	reload*=1000000/configTICK_RATE_HZ;			//根据configTICK_RATE_HZ设定溢出时间
												//reload为24位寄存器,最大值:16777216,在72M下,约合0.233s左右	
	fac_ms=1000/configTICK_RATE_HZ;				//代表OS可以延时的最少单位	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 						//每1/configTICK_RATE_HZ秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    
}	
// us 延时
void interface_delay_us(u32 nus)
{
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}
// ms 延时
void interface_delay_ms(u32 nms)
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{		
		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   		vTaskDelay(nms/fac_ms);	 		//FreeRTOS延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	interface_delay_us((u32)(nms*1000));				//普通方式延时
}

/*定时器管理---------------------------------------------------------------------*/
void Time2Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseStructure.TIM_Prescaler = 48000-1;
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

/*串口管理----------------------------------------------------------------------*/
//串口初始化
uint8_t Stm32F411Usart_Initialize(const uint8_t portName, uint32_t baudRate, uint8_t dataBits, uint8_t parity, uint8_t stopBits)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_TypeDef *USARTx;

	switch (portName)
	{
	case 1:
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

#if SERIAL1_TX_SUPPORT > 0
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#if SERIAL1_RX_SUPPORT > 0
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
		break;
	case 2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

#if SERIAL2_TX_SUPPORT > 0
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#if SERIAL2_RX_SUPPORT > 0
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
		break;
	case 3:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

#if SERIAL3_TX_SUPPORT > 0
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#if SERIAL3_RX_SUPPORT > 0
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
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
//串口写
void Stm32F411Usart_Write(const uint8_t portName, uint8_t d)
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
/*中断配置项----------------------------------------------------------------------*/
void ADP_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 优先级组 说明了抢占优先级所用的位数，和子优先级所用的位数*/   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
#if SERIAL1_RX_SUPPORT > 0	
	/*USART1*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SERIAL1_NVIC_IRQ_PP;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SERIAL1_NVIC_IRQ_SP;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif	
#if SERIAL2_RX_SUPPORT > 0	
	/*USART2*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SERIAL2_NVIC_IRQ_PP;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SERIAL2_NVIC_IRQ_SP;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif	
#if SERIAL3_RX_SUPPORT > 0	
	/*USART3*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SERIAL3_NVIC_IRQ_PP;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SERIAL3_NVIC_IRQ_SP;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif	
#if TIME2_SUPPORT > 0
 	/*TIMER2*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIME2_NVIC_IRQ_PP;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIME2_NVIC_IRQ_SP;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
#if TIME3_SUPPORT > 0
 	/*TIMER3*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIME3_NVIC_IRQ_PP;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIME3_NVIC_IRQ_SP;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
#if TIME4_SUPPORT > 0
	/*TIMER4*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIME4_NVIC_IRQ_PP;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIME4_NVIC_IRQ_SP;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
}
// GPIO 初始化
void ADP_GPIO_Configuration(void)
{
	/*JTAG IO ½ûÖ¹*/
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  //RCC_APB2PeriphResetCmd(0X01FC,ENABLE);
}
// 单片机初始化
void Stm32F411_mcuInit(void)
{	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
	ADP_NVIC_Configuration();
	ADP_GPIO_Configuration();
	delay_init(SYS_CLK / 1000000);
}
// uuid获取
u8* SystemMcuGetUUID(void)
{
	static u8 ID[12];
	static u32 UID_ADDR = 0x1FFF7A10;
	for(u8 i=0;i<12;i++) {
		 ID[i] = (*(__IO u32 *)(UID_ADDR+(i/4)*4) >>((i%4)*8))&0x000000ff;
	}
	return ID;
}
//系统复位
void SystemMcuRest(void)
{
	NVIC_SystemReset();
}


