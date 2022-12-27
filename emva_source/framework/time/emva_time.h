// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_TIME_H
#define __EMVA_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "emva_os.h"
    typedef enum
    {
        CYCLE = 0,
        SINGLE,
    } timeModeU;
    typedef struct
    {
        void (*t_callBack)(void *);
        uint32 time_out;
        uint32 time_count;
        void *prg;
    } timerEx_callback_t;
    typedef struct _timer EmvaTime;
    struct _timer
    {
        EMVA_BOOL(*start)
        (EmvaTime *this, void *p_arg, void (*ptr)(void *), uint32 args, ...);
        void (*stop)(void *args, ...);
        timerEx_callback_t *subTimerCall;
        uint8 subTimerNum;
        timeModeU subTimerMode;
    };
    struct EmvaTimeApi
    {
        EmvaTime *(*new)(void);
        EmvaTime *(*newSingleCycle)(void);
        EmvaTime *(*newSub)(uint8);
        EmvaTime *(*newSubSingleCycle)(uint8);
    };
    extern struct EmvaTimeApi EmvaTimeClass;
    void EmvaTimeInitialize(void);
    extern inline void emva_sleep(uint32 nms)
    {
        os_sleep(nms);
    }
#define sleep(x) emva_sleep(x)
#ifdef __cplusplus
}
#endif

#endif
