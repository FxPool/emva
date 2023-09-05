// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "FreeRTOS.h"
#include "emva_time.h"
#include "task.h"
#include "string.h"
#include "app_main.h"
#include "croutine.h"

TaskHandle_t MainTask_Handler;
TaskHandle_t WifiTask_Handler;
TaskHandle_t NetTask_Handler;
emvaFmt* fmt;
EmvaSerial *ems1;
EmvaConn *client_conn;
static uint8 read_buff[128];

// Device init info
static void emvaDeviceInitInfo(uint16 code,const char* info)
{
	if(code!=0){
		while(1);
	}
}

// fmt io
static void fmt_io(void *dt)
{
	ems1->write(ems1,dt,strlen((char*)dt));
}

// serial call back
static void serial1_callback(byte *dt, uint32 length)
{
	if(client_conn)
	client_conn->Write(client_conn,dt,length);
}

// Main task
void MainTask(void* p)
{
	int32 n = 0;
	// usrat1
	ems1 = emva.SerialClass.new();
	ems1->portName = 1;
	ems1->parity = Parity_No;
	ems1->baudRate = 115200;
	ems1->dataBits = WordLength_8b;
	ems1->stopBits = StopBits_1;
	ems1->dtLength = 128;
	ems1->set_callBack(ems1,serial1_callback);
	ems1->open(ems1);
	// fmt
	fmt = emva.FmtClass.new(64);
	fmt->io = fmt_io;
	EmvaWifi *wifi = EmvaWifiClass.new();
	EmvaWifi *this = wifi;
	wifi->SetConnectInfo(this,"YJ","YJ123456");
	//	wifi->SetConnectInfo(this,"Redmi","12345678");
	wifi->Connect(this);
	while(wifi->GetSupplicanState(this)!=COMPLETED)
	{
		fmt->println(fmt,"Wifi Link is fail");
		emva_sleep(300);
	}
	fmt->println(fmt,"Wifi Link is ok");
	reconnect:
	client_conn = EmvaNet.Dial(NULL,"tcp","8.218.160.115:13557");
	if (client_conn==NULL)
	{
		emva_sleep(1000);
		goto reconnect;
	}
	for(;;)
	{
		client_conn->Read(client_conn,read_buff,sizeof(read_buff),&n);
		if(n>0){
			ems1->write(ems1,read_buff,n);
		}
	}
}

// main
int main(void)
{
	emvaIniaialize();
	emvaDeviceInit(emvaDeviceInitInfo);
	xTaskCreate((TaskFunction_t )MainTask,
							(const char*    )"main",
							(uint16_t       )512,
							(void*          )NULL, 
							(UBaseType_t    )1,
							(TaskHandle_t*  )&MainTask_Handler);
	vTaskStartScheduler();
}
