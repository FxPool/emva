// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_LED_H_
#define __EMVA_LED_H_
#include "emva_list.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define INFINITE 0

    // Define the necessary abstract function interface or parameter for LED operation.
    // The implementation requires the user to implement  at the user layer
    typedef struct
    {
        void (*Init)(void); /*! Initialize the LED */
        volatile uint32 *r_value;
        volatile uint32 *g_value;
        volatile uint32 *b_value;
    } LED_Interface_t;

    typedef struct
    {
        // CtlInterface for the control interface needs to implement the parameter return TRUE,FALSE,CtlCallback for the control callback function can be NULL, pulse interval is minimum 1ms
        // positive_time is positive and negative_time is mutual and can be interpreted as high and low level. negative_time can be 0, which means only positive_time but positive_time is executed
        // The value cannot be 0. num indicates the number of executions
        void (*Pulse1Start)(void (*CtlInterface)(EMVA_BOOL, void*), void*, uint16 positive_time, uint16 negative_time, uint16 num);
        void (*Pulse2Start)(void (*CtlInterface)(EMVA_BOOL, void*), void*, uint16 positive_time, uint16 negative_time, uint16 num);
        void (*Pulse3Start)(void (*CtlInterface)(EMVA_BOOL, void*), void*, uint16 positive_time, uint16 negative_time, uint16 num);
        void (*Pulse4Start)(void (*CtlInterface)(EMVA_BOOL, void*), void*, uint16 positive_time, uint16 negative_time, uint16 num);
        void (*Pulse5Start)(void (*CtlInterface)(EMVA_BOOL, void*), void*, uint16 positive_time, uint16 negative_time, uint16 num);
        void (*Pulse1Stop)(void);
        void (*Pulse2Stop)(void);
        void (*Pulse3Stop)(void);
        void (*Pulse4Stop)(void);
        void (*Pulse5Stop)(void);
        // RGB Color setting isBreathing indicates whether or not to breathe and the rate at which the 8th digit is not selected, and the bottom 7 digit is speed
        // B_Level is the brightness Par_BreathingCount is the number of breathings. 255 is breathingall the time L High 8 bits is when the breathingcount is off
        // The bottom 8 of middle B is the time when the light is on. In particular, it only indicates the time when the peak respiration rate comes, such as 0x88,0x7F ->1916ms
        // 25s For 13 breaths, return value The time for this breath is steady on and return to 0
        uint32 (*rgb_setValue10)(uint8 r, uint8 g, uint8 b, uint8 isBreathing, uint8 B_Level,
                                 uint8 Par_BreathingCount, uint32 L_B);
        uint32 (*rgb_setValue)(uint32 rgbValue, uint8 isBreathing, uint8 B_Level, uint8 Par_BreathingCount, uint32 L_B);
    } led_t;
    extern led_t led;
    extern LED_Interface_t LED_Interface;
    void init_sysled(void);

#ifdef __cplusplus
}
#endif
#endif
