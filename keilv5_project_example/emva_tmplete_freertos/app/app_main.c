// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "FreeRTOS.h"
#include "emva_time.h"
#include "emva_serial.h"
#include "task.h"
#include "app_main.h"
#include "croutine.h"
#include "limits.h"
#include "string.h"
#include "pro_bsp.h"

TaskHandle_t MainTask_Handler;
EmvaSerial *se;
void sreial1_callback(byte *bt,uint32 length)
{
	
}
void fmtout(void *dt)
{
	se->write(se,dt,strlen((char*)dt));
}
// Main task
void MainTask(void* p)
{
	se = emva.SerialClass.new();
	se->portName = 1;
	se->baudRate=115200;
	se->parity = Parity_No;
	se->dataBits = WordLength_8b;
	se->stopBits = StopBits_1;
	se->dtLength = 128;
	se->callBack(se,sreial1_callback);
	se->open(se);
	emvaFmt *fmt = emva.FmtClass.new(128);
	fmt->io = fmtout;
	// Iterators iterate over all drivers
	while(emva.Driver.IteratorCdName->hasNext(emva.Driver.IteratorCdName))
	{
		struct EmvaDriverInfo *driverInfo = (struct EmvaDriverInfo*)emva.Driver.IteratorCdName->next(emva.Driver.IteratorCdName)->data;
		fmt->println(fmt,"name:%s,dev_num:%d,version:%s,date:%s,owner:%s",driverInfo->DeviceName,driverInfo->DeviceNum,driverInfo->Version,driverInfo->Date,driverInfo->Owner);
		fmt->println(fmt,"");
	}
	for(;;)
	{
		os_sleep(100);
	}
}
