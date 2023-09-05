// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_time.h"
#include "emva_list.h"
#include "emva_malloc.h"
#include "string.h"
#include "stdarg.h"
#include "dv_character_device.h"
#include "emva_config.h"

#if EMVA_FM_TIME_ENABLED == 1

EmvaLink_List *g_timeInstancelist;
static int32 g_fd;
static EMVA_BOOL g_isInit = FALSE;

static EMVA_BOOL osal_start_timerEx(EmvaTime *this, uint8 timerEx_id,
                                    void *p_arg, void (*ptr)(void *), uint32 timer_period)
{
    if (timerEx_id > (this->subTimerNum - 1))
    {
        return FALSE;
    }
    this->subTimerCall[timerEx_id].t_callBack = ptr;
    this->subTimerCall[timerEx_id].time_out = timer_period;
    this->subTimerCall[timerEx_id].time_count = 0;
    this->subTimerCall[timerEx_id].prg = p_arg;
    return TRUE;
}

static EMVA_BOOL osal_stop_timerEx(EmvaTime *this, uint8 timerEx_id)
{
    if (timerEx_id > (this->subTimerNum - 1))
    {
        return FALSE;
    }
    this->subTimerCall[timerEx_id].time_out = 0;
    this->subTimerCall[timerEx_id].time_count = 0;
    return TRUE;
}

EMVA_BOOL InstaceTimeStart(EmvaTime *this, void *p_arg, void (*ptr)(void *), uint32 args, ...)
{
    va_list ap;
    va_start(ap, args);
    static uint32 timer_period = 0;
    timer_period = args;
    static uint8 timerEx_id;
    timerEx_id = 0;
    if (this->subTimerNum > 1)
    {
        timerEx_id = va_arg(ap, int);
    }
    if (timerEx_id > SUB_TIMER_NUM)
    {
        timerEx_id = 0;
    }
    va_end(ap);
    return osal_start_timerEx(this, timerEx_id, p_arg, ptr, timer_period);
}
void InstaceTimeStop(void *args, ...)
{
    va_list ap;
    va_start(ap, args);
    EmvaTime *this = (EmvaTime *)args;
    static int timerEx_id = 0;
    timerEx_id = va_arg(ap, int);
    if (timerEx_id > SUB_TIMER_NUM)
    {
        osal_stop_timerEx(this, 0);
    }
    do
    {
        if (timerEx_id < SUB_TIMER_NUM)
        {
            osal_stop_timerEx(this, timerEx_id);
        }
        else
        {
            break;
        }
        timerEx_id = va_arg(ap, int);
    } while (1);
    va_end(ap);
}

EMVA_BOOL timerDel(EmvaTime *p)
{
    object_t obj;
    obj.data = (byte *)p;
    obj.length = sizeof(EmvaTime);
    EmvaLink_List_object_RemoveAt(g_timeInstancelist, &obj);
    if (p)
    {
        emva_free(p);
    }
    else
    {
        return FALSE;
    }
    if (p)
    {
        emva_free(p);
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

void virtualTimerRun(void);
inline void virtualTimerRun(void)
{
    EmvaTime *p = NULL;
    for (uint8 i = 0; i < g_timeInstancelist->length; i++)
    {
        p = (EmvaTime *)EmvaLink_List_GetAt(g_timeInstancelist, i);
        if (p == NULL)
            return;
        for (uint8 i = 0; i < p->subTimerNum; i++)
        {
            if (p->subTimerCall[i].time_count == 0xFFFFFFFFul)
                p->subTimerCall[i].time_count = 0;
            if (++(p->subTimerCall[i].time_count) == p->subTimerCall[i].time_out)
            {
                p->subTimerCall[i].time_count = 0;
                if (p->subTimerMode == SINGLE)
                {
                    p->subTimerCall[i].time_out = 0;
                }
                p->subTimerCall[i].t_callBack(p->subTimerCall[i].prg);
            }
        }
    }
}

void time_dev_event_callback(void *event, const void *d, uint32 size)
{
    virtualTimerRun();
}

EmvaTime *timerNew(timeModeU mode, uint8 sub_time_num)
{
    if (sub_time_num > SUB_TIMER_NUM || sub_time_num == 0 || g_isInit == FALSE)
    {
        return NULL;
    }
    EmvaTime *p = (EmvaTime *)emva_malloc(sizeof(EmvaTime));
    p->start = InstaceTimeStart;
    p->stop = InstaceTimeStop;
    p->subTimerCall = (timerEx_callback_t *)emva_malloc(sub_time_num * sizeof(timerEx_callback_t));
    p->subTimerNum = sub_time_num;
    p->subTimerMode = mode;
    if (g_timeInstancelist == NULL)
    {
        g_timeInstancelist = EmvaLink_List_Init();
        character_dev_event(g_fd, NULL, time_dev_event_callback);
    }
    EmvaLink_List_Insert(g_timeInstancelist, p, -1);
    character_dev_ioctl(g_fd, NULL, (uint32)TRUE, NULL);
    return p;
}

static EmvaTime *ThisNew(void)
{
    return timerNew(CYCLE, 1);
}
static EmvaTime *ThisSingleNew(void)
{
    return timerNew(SINGLE, 1);
}
static EmvaTime *ThisSubNew(uint8 sub_time_num)
{
    return timerNew(CYCLE, sub_time_num);
}
static EmvaTime *ThisSubSingleNew(uint8 sub_time_num)
{
    return timerNew(SINGLE, sub_time_num);
}

struct EmvaTimeApi EmvaTimeClass = {
    .new = ThisNew,
    .newSingleCycle = ThisSingleNew,
    .newSub = ThisSubNew,
    .newSubSingleCycle = ThisSubSingleNew,
};

EMVA_BOOL EmvaTimeInitialize(void)
{
    g_fd = character_dev_open(EMVA_TIME_DRIVER_NAME, NULL);
    g_isInit = (g_fd == -1) ? FALSE : TRUE;
    return g_isInit;
}
#endif
