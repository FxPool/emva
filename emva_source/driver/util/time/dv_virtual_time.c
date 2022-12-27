// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dev_init.h"
#include "dv_virtual_time.h"

uint16 g_DvVittualTimeNum = (sizeof(dvVirtualTime.start) / sizeof(dvVirtualTime.start.time1));
DvVirtualtimer_callback_t DvVirtualtimer_callback[sizeof(dvVirtualTime.start) / sizeof(dvVirtualTime.start.time1)];

EMVA_BOOL gTimeStartFlag = FALSE;

static void virtualTimeStart1(void *arg, void (*fuc)(void *arg), uint32 ms)
{
    DvVirtualtimer_callback[0].arg = arg;
    DvVirtualtimer_callback[0].t_callBack = fuc;
    DvVirtualtimer_callback[0].time_out = ms;
    DvVirtualtimer_callback[0].time_count = 0;
    if (gTimeStartFlag == FALSE)
    {
        gTimeStartFlag = TRUE;
        hal.time->Control(1, TRUE);
    }
}
static void virtualTimeStop1(void)
{
    DvVirtualtimer_callback[0].time_out = 0;
    DvVirtualtimer_callback[0].time_count = 0;
}
dvVirtualTime_t dvVirtualTime = {
    .start.time1 = virtualTimeStart1,
    .stop.time1 = virtualTimeStop1,
};

void dvTimeVirtualRun(void);
inline void dvTimeVirtualRun(void)
{
    for (uint16 i = 0; i < g_DvVittualTimeNum; i++)
    {
        if (++(DvVirtualtimer_callback[i].time_count) == DvVirtualtimer_callback[i].time_out)
        {
            if (DvVirtualtimer_callback[i].t_callBack)
                DvVirtualtimer_callback[i].t_callBack(DvVirtualtimer_callback[i].arg);
        }
    }
}

static void timeCallback(void)
{
    dvTimeVirtualRun();
}

void dvTimeVirtualInit(void)
{
    hal.time->Initialize(1, 1000);
    hal.time->CallBack(1, timeCallback);
}
#ifndef EMVA_CREATE_LIB
DV_MODULE_REG(dvTimeVirtualInit);
#endif
