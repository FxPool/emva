// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __DV_VIRTUAL_TIME_H
#define __DV_VIRTUAL_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct
    {
        void (*t_callBack)(void *);
        uint32 time_out;
        uint32 time_count;
        void *arg;
    } DvVirtualtimer_callback_t;

    struct dv_virtual_time_strat
    {
        void (*time1)(void *arg, void (*fuc)(void *arg), uint32 ms);
    };

    struct dv_virtual_time_stop
    {
        void (*time1)(void);
    };

    typedef struct
    {
        struct dv_virtual_time_strat start;
        struct dv_virtual_time_stop stop;
    } dvVirtualTime_t;
    extern dvVirtualTime_t dvVirtualTime;
#ifdef __cplusplus
}
#endif

#endif
