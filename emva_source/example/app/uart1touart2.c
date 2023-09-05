// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "emva_serial.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"

TaskHandle_t MainThread_Handler;
EmvaSerial *serial1_handler;

// usrat callback
static void serial2_callback(byte *dt, uint32 length)
{
	if(serial1_handler)
	{
		serial1_handler->write(serial1_handler,dt,length);
	}
}

// Device init info
static void emvaDeviceInitInfo(uint16 code, const char *info)
{
//    if (code != 0)
//    {
//			while(1);
//    }
}
// Main task
void MainThread(void *p)
{
		// usrat1
    EmvaSerial *ems1 = emva.SerialClass.new();
    ems1->portName = 1;
    ems1->parity = Parity_No;
    ems1->baudRate = 115200;
    ems1->dataBits = WordLength_8b;
    ems1->stopBits = StopBits_1;
    ems1->dtLength = 128;
    ems1->open(ems1);
		serial1_handler = ems1;
		
		// usart2
		EmvaSerial *ems2 = emva.SerialClass.new();
		ems2->portName = 2;
    ems2->parity = Parity_No;
    ems2->baudRate = 57600;
    ems2->dataBits = WordLength_8b;
    ems2->stopBits = StopBits_1;
    ems2->dtLength = 128;
		ems2->set_callBack(ems2,serial2_callback);
    ems2->open(ems2);
		
		serial1_handler->write(serial1_handler,"1",2);
	
    byte buf[128];
    int n;
    while (1)
    {
        ems1->read(ems1, buf, 20, &n);
        if (n > 0)
        {
            buf[n] = 0;
						// write usart2
            ems2->write(ems2, (byte *)buf, strlen((char *)buf));
        }
    }
}
int main(void)
{
    emvaIniaialize();
    emvaDeviceInit(emvaDeviceInitInfo);

    xTaskCreate((TaskFunction_t)MainThread,
                (const char *)"main",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)2,
                (TaskHandle_t *)&MainThread_Handler);
    vTaskStartScheduler();
}
