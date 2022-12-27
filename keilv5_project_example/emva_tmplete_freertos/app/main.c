// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "app_main.h"
#include "croutine.h"

int main(void)
{
	emvaIniaialize();
	xTaskCreate((TaskFunction_t )MainTask,
							(const char*    )"main",
							(uint16_t       )64,
							(void*          )NULL, 
							(UBaseType_t    )2,
							(TaskHandle_t*  )&MainTask_Handler);
	vTaskStartScheduler();
}
