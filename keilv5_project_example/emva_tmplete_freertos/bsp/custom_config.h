/**
  ******************************************************************************
  * @file    sustom_config.h
  * @author  HANSEN     emil:951868692@qq.com
  * @version 0.1.0
  * @date    23-3-2020
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
  * <h2><center>&copy; COPYRIGHT 2020 HansenTech</center></h2>
  ******************************************************************************
  */
#ifndef _CUSTOM_CONFIG_H_
#define _CUSTOM_CONFIG_H_

#define ATTITUDE_VERSIONE              2     
#define HARDWARE_VERSION               2     //0.0.2
#define SOFTWARE_VERSION               0x01  //00 0000 00  0.0.1
#define MEM_SIZE_SET                   5*1024
#define SYS_CLK                        96000000
#define UC_OSII_SUPPORT                0
#define FREE_RTOS_SUPPORT              1
//模块支持
#define NET_SUPPORT                    0
#define USB_SUPPORT                    1  
#define NRF24L01_SUPPORT               1
#define SYS_BUTTON_SUPPORT             1
#define SYS_LED_SUPPORT                1
#define IR_REMOTE_SUPPORT              1
#define SERIAL_SUPPORT                 1
#define TIME_SUPPORT                   1
//串口支持配置
#define SERIAL1_RX_SUPPORT             1
#define SERIAL1_TX_SUPPORT             1
#define SERIAL2_RX_SUPPORT             1
#define SERIAL2_TX_SUPPORT             1
#define SERIAL3_RX_SUPPORT             0
#define SERIAL3_TX_SUPPORT             0
//定时器支持配置
#define TIME2_SUPPORT                  1
#define TIME3_SUPPORT                  0
#define TIME4_SUPPORT                  0
/*注意使用RTOS时如果中断中要调用系统的函数 那么次中断一定要在RTOS可管理的范围内*/
//USB中断优先级配置
#define USB_FS_NVIC_IRQ_PP             9//抢占优先级
#define USB_FS_NVIC_IRQ_SP             0//子优先级
#define USB_REPORT_SIZE                32//32

//串口中断优先级配置
#define SERIAL1_NVIC_IRQ_PP            7//抢占优先级
#define SERIAL1_NVIC_IRQ_SP            0//子优先级

#define SERIAL2_NVIC_IRQ_PP            6//抢占优先级
#define SERIAL2_NVIC_IRQ_SP            0//子优先级

#define SERIAL3_NVIC_IRQ_PP            8//抢占优先级
#define SERIAL3_NVIC_IRQ_SP            0//子优先级

//定时器中断优先级配置
#define TIME2_NVIC_IRQ_PP              6//抢占优先级
#define TIME2_NVIC_IRQ_SP              0//子优先级

#define TIME3_NVIC_IRQ_PP              6//抢占优先级
#define TIME3_NVIC_IRQ_SP              0//子优先级

#define TIME4_NVIC_IRQ_PP              6//抢占优先级
#define TIME4_NVIC_IRQ_SP              0//子优先级

//IR中断优先极
#define IR_NVIC_IRQ_PP                 11
//RF中断优先级
#define RF_NVIC_IRQ_PP                 10
//按键中断优先级
#define BUTTON_NVIC_IRQ_PP             12
//MPU9250中断
#define MPU9250_NVIC_IRQ_PP            13
#endif
