// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dev_init.h"
#include "dv_character_device.h"
#include "dv_kfifo.h"
#include "dv_malloc.h"
#include "string.h"
#include "stdlib.h"
#include "dv_map.h"
#include "dv_driver.h"
#include "dv_serial_module.h"
#include "util.h"
#include "dev_init.h"
#include "user_string.h"
#include "arch_stm32f411xx.h"

#define USART_PORT_NAME 2
#define RETURN_FLAG "+OK"

#define GPS_LOCATION_ASTATE  1
#define GPS_LOCATION_VSTATE  2
#define GPS_LOCATION_NSTATE  3

typedef struct
{
	char Date_State;
	uint32 jd;
	uint32 wd;
} gpspro_t;
typedef struct
{
	serial_moudle_t *se;
	void (*Input)(serial_moudle_t *s, uint8 d);
} gpsDataInput_t;
gpsDataInput_t gpsDataInput;
serial_moudle_t *serial_moudle_gps = NULL;
gpspro_t gpspro;
void (*gpseventCallbackSet)(void *event, const void *d, uint32 size);
static bool gps_set_tranmission_mode(TRANMISSION_MODE_U mode)
{
	if (mode == CMD_MODE)
	{
		if (serial_moudle_gps->send_cmd_check(serial_moudle_gps, RETURN_FLAG, "AT+VER\r\n"))
		{
			return TRUE;
		}
		dv_sleep(220);
		if (serial_moudle_gps->send_cmd_check(serial_moudle_gps, "a", "+++"))
		{
			return (bool)serial_moudle_gps->send_cmd_check(serial_moudle_gps, RETURN_FLAG, "a");
		}
		return FALSE;
	}
	else if (mode == TRANSMISSION_MODE)
	{
		return (bool)serial_moudle_gps->send_cmd_check(serial_moudle_gps, RETURN_FLAG, "AT+ENTM\r\n");
	}
	return FALSE;
}
static void gps_uart_send_oneByte(uint8 d)
{
	hal.usart->Write(USART_PORT_NAME, d);
}
static void gps_net_module_dv_sleep(uint32 ms)
{
	dv_sleep(ms);
}
static void gps_set_data_call_back(serial_moudle_t *s, void (*ptr)(serial_moudle_t *s, uint8 d))
{
	gpsDataInput.se = s;
	gpsDataInput.Input = ptr;
}
serial_moudle_interface_t serial_moudle_interface = {
	.set_tranmission_mode = gps_set_tranmission_mode,
	.uart_send_oneByte = gps_uart_send_oneByte,
	.net_module_delay_ms = gps_net_module_dv_sleep,
	.set_data_call_back = gps_set_data_call_back,
	.re_sent_cnt = 3,
	.set_receive_data_buffer_size = 128,
	.set_cmd_buf_size = 128,
	.package_time = 10,
	.return_flag = RETURN_FLAG,
	.re_sent_interval_timer = 50,
};
static void gpsCallback(uint8 *data, uint16 length)
{
	static uint8 buffer[1024*3];
	static uint8 gps_local_state = 0;
	if(length > (1024*3))
	{
		return;
	}
	memcpy(buffer, data, length);
	char *ptr = (char*)buffer;
	int16 gpsDindexof = user_IndexOf(ptr, "$GNRMC");
	if (gpsDindexof > 0)
	{
		user_subString((char *)ptr, gpsDindexof);
		gpsDindexof = user_IndexOf(ptr, "\r\n");
		user_subnString(ptr,0,gpsDindexof+2);
		uint8 cs_cal = ptr[1];
		char cs_source[2];
		memcpy(cs_source,ptr+(gpsDindexof-2),2);
		for(uint16 i=2;ptr[i] != '*';i++)
		{
			cs_cal ^=ptr[i];
		}
		char cs_result[2];
		hex_to_str(cs_result,&cs_cal,1);
		if(strcmp(cs_source,cs_result)!=0)
		{
			return;
		}
		char *p = NULL;
		p = strtok((char *)ptr, ","); //$GNRMC
		p = strtok(NULL, ",");		  //时间
		p = strtok(NULL, ",");		  //定位状态 A数据可用 V数据不可用
		if (p != NULL)
		gpspro.Date_State = *(char *)p;
		if (gpspro.Date_State == 'A')
		{
			p = strtok(NULL, ","); //维度
			if (p != NULL)
			{
				uint32 t_wd = (uint32)(atof(p) * 10000);
				if (t_wd != 0)
				{
					gpspro.wd = t_wd;
				}
			}
			p = strtok(NULL, ","); //N
			p = strtok(NULL, ","); //精度
			if (p != NULL)
			{
				uint32 t_jd = (uint32)(atof(p) * 10000);
				if (t_jd != 0)
				{
					gpspro.jd = t_jd;
				}
			}
			//回调函数
			if(gpseventCallbackSet)
			{
				gps_local_state = GPS_LOCATION_ASTATE;
				gpseventCallbackSet(&gps_local_state,&gpspro,sizeof(gpspro_t));
			}
		}
		else
		{
			if(gpseventCallbackSet)
			{
				gps_local_state = GPS_LOCATION_VSTATE;
				gpseventCallbackSet(&gps_local_state,&gpspro,sizeof(gpspro_t));
			}
		}
	}
	else
	{
		if(gpseventCallbackSet)
		{
			gps_local_state = GPS_LOCATION_NSTATE;
			gpseventCallbackSet(&gps_local_state,&gpspro,sizeof(gpspro_t));
		}
	}
}
static void Uart2CallBack(const void *arg,uint8 d)
{
	gpsDataInput.Input(gpsDataInput.se, d);
}
static int32 gps_open_module(void *obj)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	
	hal.usart->Initialize(USART_PORT_NAME,38400,0,0,0);
	hal.usart->CallBack(USART_PORT_NAME,NULL,Uart2CallBack);
	serial_moudle_gps = new_serial_moudle(&serial_moudle_interface);
	serial_moudle_gps->set_data_callback(serial_moudle_gps, gpsCallback);
	return 0;
}
static int32 gps_close_module(void *obj) { return 0; }
static int32 gps_read(void *obj, void *d, uint32 size) { return 0; }
static int32 gps_write(void *obj, const void *d, const uint32 size)
{
	serial_moudle_gps->send_hex_data(serial_moudle_gps, (uint8 *)d, size);
	return 0;
}
static int32 gps_ioct(void *obj, uint32 cmd, uint32 arg)
{
	if(cmd == 0)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	}
	else if(cmd == 1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
	}
	return 0; 
}
static int32 gps_events(void *obj, void (*fuc)(void *event, const void *d, uint32 size))
{
	gpseventCallbackSet = fuc;
	return 0;
}

characterModuleInterface_t dv_gps_interface = {
	.open_module = gps_open_module,
	.close_module = gps_close_module,
	.read = gps_read,
	.write = gps_write,
	.ioctl = gps_ioct,
	.events = gps_events,
};

character_driver_t gps_driver = {
	.DeviceName = "zd_gps",
	.Version = "1.0",
	.Date = "2021.10.10",
	.Owner = "hansen.jiang",
	.ops = &dv_gps_interface,
};

void dv_gps_init(void)
{
	character_dev_device_registered(&gps_driver);
}

DV_MODULE_REG(dv_gps_init);
