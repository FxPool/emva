// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva_framework.h"
#include "string.h"
#include "stdio.h"
#include "emva_malloc.h"

TaskOs_callBack_t *task_call;
uint8 Timer_tasksCnt;
uint32 Tick_Cnt = 0;
Task_t OsTask;

/**
  * @brief  Set the address of the callback function that needs to be performed when the task is suspended
  * @param  this_priority Current task priority
  * @retval : 1 suc 0fail
**/
static void SetTaskStop_Cb(uint8 this_priority, void (*ptr)(void))
{
    task_call[this_priority].SuspendCb = ptr;
}
/**
  * @brief  Set the address of the callback function that needs to be performed during task recovery
  * @param  this_priority Current task priority
  * @retval : 1 suc 0 fail
**/
static void SetTaskStart_Cb(uint8 this_priority, void (*ptr)(void))
{

    if (this_priority > (Timer_tasksCnt - 1))
    {
        return;
    }

    task_call[this_priority].ResumeCb = ptr;
}

/**
  * @brief  Task suspension
  * @param  priority Current task priority
  * @retval : None
**/
void Timer_Task_OS_Suspend(uint8 priority)
{
    if (priority > (Timer_tasksCnt - 1))
    {
        return;
    }

    // Execution of the current task suspends the callback event
    if (task_call[priority].SuspendCb != NULL)
    {
        task_call[priority].SuspendCb();
    }

    task_call[priority].IsSuspend = 1;
}

/**
  * @brief Resume a suspended task
  * @param  priority
  * @retval : None
**/
void Timer_Task_OS_Resume(uint8 priority)
{
    if (priority > (Timer_tasksCnt - 1))
    {
        return;
    }

    // Execution of the current task recovery callback event
    if (task_call[priority].ResumeCb != NULL)
    {
        task_call[priority].ResumeCb();
    }

    task_call[priority].IsSuspend = 0;
}
/**
  * @brief  Example Change the periodic period of a task
  * @param  priority  timer_period
  * @retval : 0 fail 1 succ
**/
uint8 Timer_Task_OS_UpdataPeriod(uint8 priority, uint32 timer_period)
{
    if (priority > (Timer_tasksCnt - 1))
    {
        return 0;
    }
    /*timer_period = NULL Indicates the highest speed at which tasks are executed without delay. The command speed of the single-chip computer is */
    if (timer_period == NULL)
    {
        task_call[priority].no_timer = 0x01;
    }
    else
    {
        task_call[priority].no_timer = 0x00;
    }
    task_call[priority].time_out = timer_period;
    task_call[priority].NextTick = Tick_Cnt + timer_period;
    task_call[priority].OS_Ready = 0x01;
    return 1;
}

/**
  * @brief  Task_OS_Create
  * @param  priority 0 maximum ,ptr: task. timer_period Indicates that the time period of a task is NULL= Maximum speed without delay
  * @retval : 0 fail 1 succ
**/
uint8 Timer_Task_OS_Create(uint8 priority, void (*task)(void *), uint32 timer_period)
{
    if (priority > (Timer_tasksCnt - 1))
    {
        return 0;
    }
    /* timer_period = NULL Indicates the highest speed at which tasks are executed without delay. The command speed of the single-chip computer is */
    if (timer_period == NULL)
    {
        task_call[priority].no_timer = 0x01;
    }
    else
    {
        task_call[priority].no_timer = 0x00;
    }
    task_call[priority].task_callBack = task;
    task_call[priority].time_out = timer_period;
    task_call[priority].NextTick = timer_period;
    task_call[priority].OS_Ready = 0x01;
    return 1;
}

/**
  * @brief  Task_OS_Init
  * @param  task_cnt The maximum number of tasks is 255
  * @retval : 0 fail 1 success
**/
uint8 Timer_Task_OS_Init(uint8 task_cnt)
{
    if (task_cnt >= 255)
    {
        return 0;
    }

    Timer_tasksCnt = task_cnt;

    task_call = (TaskOs_callBack_t *)emva_malloc(sizeof(TaskOs_callBack_t) * Timer_tasksCnt);

    if (task_call == NULL)
    {
        return 0;
    }

    for (int i = 0; i < Timer_tasksCnt; i++)
    {
        task_call[i].NextTick = 0;
        task_call[i].time_out = 0;
        task_call[i].task_callBack = NULL;
        task_call[i].no_timer = 0;
        task_call[i].OS_Ready = 0;
        task_call[i].InitFlag = 1;
        task_call[i].IsSuspend = 0;
    }

    /* Function instantiation of the callback function address set for the start task stop task */
    OsTask.SetTaskStart_Cb = SetTaskStart_Cb;
    OsTask.SetTaskStop_Cb = SetTaskStop_Cb;

    return 1;
}

/**
  * @brief  
  * @param  None
  * @retval : None
**/
static uint32 GetTimingTick(void)
{
    if (Tick_Cnt > 4294960000ul)
    {
        for (int i = 0; i < Timer_tasksCnt; i++)
        {
            task_call[i].NextTick = 0;
            task_call[i].time_out = 0;
            task_call[i].task_callBack = NULL;
            task_call[i].no_timer = 0;
            task_call[i].OS_Ready = 0;
        }
        Tick_Cnt = 0;
    }
    return Tick_Cnt;
}

/**
  * @brief  Task execution in rotation training tasks can be delayed
  * @param  None
  * @retval : None
**/
void Timer_Task_OS_Run(void)
{
    for (;;)
    {
        for (int i = 0; i < Timer_tasksCnt; i++)
        {
            /* Without delay direct operation delay is the MCU instruction delay */
            if (task_call[i].no_timer == 0x01)
            {
                OsTask.SysTimerCnt = Tick_Cnt;
                OsTask.this_priority = i;
                if (task_call[i].InitFlag == 1)
                {
                    OsTask.InitFlag = 1;
                    if (!task_call[i].IsSuspend)
                    {
                        task_call[i].task_callBack(&OsTask);
                    }
                    task_call[i].InitFlag = 0;
                }
                else
                {
                    OsTask.InitFlag = 0;
                    if (!task_call[i].IsSuspend)
                    {
                        task_call[i].task_callBack(&OsTask);
                    }
                }
            }
            else
            {
                /* Time is up and task is ready */
                if ((task_call[i].NextTick <= GetTimingTick()) && (task_call[i].OS_Ready))
                {
                    OsTask.SysTimerCnt = Tick_Cnt;
                    OsTask.this_priority = i;
                    if (task_call[i].InitFlag == 1)
                    {
                        OsTask.InitFlag = 1;
                        /* The time of the next moment is equal to the current time + timing time */
                        task_call[i].NextTick = (Tick_Cnt + task_call[i].time_out);

                        /* Determines whether the current task is suspended */
                        if (!task_call[i].IsSuspend)
                        {
                            task_call[i].task_callBack(&OsTask);
                        }
                        task_call[i].InitFlag = 0;
                    }
                    else
                    {
                        OsTask.InitFlag = 0;
                        task_call[i].NextTick = (Tick_Cnt + task_call[i].time_out);
                        if (!task_call[i].IsSuspend)
                        {
                            task_call[i].task_callBack(&OsTask);
                        }
                    }
                }
            }
        }
    }
}
