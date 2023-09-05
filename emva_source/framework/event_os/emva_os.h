// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_OS_H
#define __EMVA_OS_H

#ifdef __cplusplus
extern "C"
{
#endif
#if UC_OSII_SUPPORT > 0
#include <ucos_ii.h>
#define MAIN_THREAD_PRO 30
    typedef void (*pTaskHandlerFn)(void *p);
    typedef struct
    {
        pTaskHandlerFn task;
        uint32 TaskPeriod;
        OS_STK *ptos;
        uint8 TaskPriority;
    } Thread_uCosII_list_t;
    extern Thread_uCosII_list_t Thread_uCosII_list[];
#elif FREE_RTOS_SUPPORT > 0
// https://www.freertos.org/xTaskNotifyFromISR.html RTOS API
#include "FreeRTOS.h"
#include "task.h"
#define MAIN_THREAD_PRO 20
typedef struct
{
    const char *Name;
    TaskFunction_t Thread;
    uint32 TaskPeriod;
    TaskHandle_t Handler;
    uint16 StkSize;
    uint8 TaskPriority;
} Thread_RTOS_list_t;
extern Thread_RTOS_list_t Thread_RTOS_list[];
extern TaskHandle_t MainThread_Handler;
#else
#define SYS_TASK_PRO 0
typedef void (*task_callback)(void *);
typedef void (*pTaskHandlerFn)(void *p);
typedef struct
{
    uint8 TaskPriority;
    uint32 TaskPeriod;
    pTaskHandlerFn task;
} Task_list_t;
typedef struct
{
    uint8 InitFlag;
    uint32 SysTimerCnt;
    uint8 this_priority;
    void (*SetTaskStart_Cb)(uint8, void (*)(void));
    void (*SetTaskStop_Cb)(uint8, void (*)(void));
} Task_t;
typedef struct
{
    task_callback task_callBack;
    uint32 time_out;
    uint32 NextTick;
    uint8 no_timer;
    uint8 OS_Ready;
    uint8 InitFlag;
    uint8 IsSuspend;
    void (*SuspendCb)(void);
    void (*ResumeCb)(void);
} TaskOs_callBack_t;

uint8 Timer_Task_OS_Create(uint8 priority, void (*task)(void *), uint32 timer_period);
uint8 Timer_Task_OS_Init(uint8 task_cnt);
void Timer_Task_OS_Run(void);
uint8 Timer_Task_OS_UpdataPeriod(uint8 priority, uint32 timer_period);
void Timer_Task_OS_Suspend(uint8 priority);
extern void Timer_Task_OS_Resume(uint8 priority);
extern uint32 Tick_Cnt;
extern uint8 TasksCnt;
extern void Sys_Task(void *ptr);
extern void MainThread(void *p);
extern Task_list_t Task_list[];
#endif
#ifdef __cplusplus
}
#endif

#endif
