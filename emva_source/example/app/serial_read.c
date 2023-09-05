// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "emva_serial.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"

TaskHandle_t MainThread_Handler;

// Device init info
static void emvaDeviceInitInfo(uint16 code, const char *info)
{
    if (code != 0)
    {
    }
}
// Main task
void MainThread(void *p)
{
    EmvaSerial *ems = emva.SerialClass.new();
    ems->portName = 1;
    ems->parity = Parity_No;
    ems->baudRate = 115200;
    ems->dataBits = WordLength_8b;
    ems->stopBits = StopBits_1;
    ems->dtLength = 128;
    ems->open(ems);
    byte buf[20];
    int n;
    while (1)
    {
        ems->read(ems, buf, 20, &n);
        if (n > 0)
        {
            buf[n] = 0;
            ems->write(ems, (byte *)buf, strlen((char *)buf));
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
