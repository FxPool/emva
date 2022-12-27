// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef _EMVA_BUTTON_H
#define _EMVA_BUTTON_H

#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum
    {
        KEY_DOWN = 0,
        KEY_UP,
        KEY_DOUBLE,
        TREBLE
    } KeyState_Type;
    typedef struct
    {
        KeyState_Type KeyState;
        uint8 ContinuousPressCnt;
        uint16 PressTime;
    } KeyEvent_t;
    typedef struct
    {
        KeyEvent_t event;
        void (*SetCallback)(KeyEvent_t *);
    } KeyEventSet_t;
    typedef struct emva_button EmvaButton;
    struct emva_button
    {
        int16 ButtonNumber;
        uint16 DoubleClickIntervalTime;
        KeyEventSet_t KeyEventSet;
        void (*SetCallback)(EmvaButton *this, void (*ptr)(KeyEvent_t *));
        int16 (*Create)(EmvaButton *this);
        uint8 (*GetPinState)(EmvaButton *this);
    };
    struct EmvaButtonApi
    {
        EmvaButton *(*new)(void);
    };
    extern struct EmvaButtonApi EmvaButtonClass;
    extern void EmvaButtonInitialize(void);

#ifdef __cplusplus
}
#endif

#endif
