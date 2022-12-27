// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva_framework.h"
#include "emva_button.h"
#include "emva_malloc.h"
#include "emva_time.h"
#include "dv_character_device.h"
#include "string.h"
#include "emva_config.h"

#if EMVA_FM_BUTTON_ENABLED == 1
#define KEY_TIME_EVENT 0
#define KEY_JITTER_TIME_EVENT 1
#define KEY_PRESS_TIME_EVENT 2
#define KEY_ELIMINATE_JITTER_TIME_OUT 25

static int32 g_fd;
static EMVA_BOOL g_isInit = FALSE;
static EmvaTime *gButtonTime;

static void KeyEventHandler(void *obj)
{
    EmvaButton *this = (EmvaButton *)obj;
    gButtonTime->stop(gButtonTime, KEY_TIME_EVENT);
    if (this->KeyEventSet.SetCallback != NULL)
    {
        this->KeyEventSet.SetCallback(&this->KeyEventSet.event);
    }
    this->KeyEventSet.event.ContinuousPressCnt = 0;
}
static void KeyPressTimeHandler(void *obj)
{
    EmvaButton *this = (EmvaButton *)obj;
    this->KeyEventSet.event.PressTime++;
}
static void KeyJitterCommon(void *obj)
{
    EmvaButton *this = (EmvaButton *)obj;
    gButtonTime->stop(gButtonTime, KEY_JITTER_TIME_EVENT);
    if (this->GetPinState(this) == 0)
    {
        this->KeyEventSet.event.KeyState = KEY_DOWN;
        this->KeyEventSet.event.ContinuousPressCnt++;
        this->KeyEventSet.event.PressTime = 0;
        gButtonTime->start(gButtonTime, obj, KeyPressTimeHandler, 1, KEY_PRESS_TIME_EVENT);
    }
    else if (this->GetPinState(this) == 1 && this->KeyEventSet.event.KeyState == KEY_DOWN)
    {
        this->KeyEventSet.event.KeyState = KEY_UP;
        gButtonTime->stop(gButtonTime, KEY_PRESS_TIME_EVENT);
    }
    if (this->KeyEventSet.event.ContinuousPressCnt > 1)
    {
        this->KeyEventSet.event.KeyState = KEY_DOUBLE;
    }
    gButtonTime->start(gButtonTime, obj, KeyEventHandler, this->DoubleClickIntervalTime, KEY_TIME_EVENT);
}
static void KeyEventCallback(void *obj, const void *d, uint32 size)
{
    gButtonTime->start(gButtonTime, obj, KeyJitterCommon, KEY_ELIMINATE_JITTER_TIME_OUT, KEY_JITTER_TIME_EVENT);
}
static void buttonSetCallback(EmvaButton *this, void (*ptr)(KeyEvent_t *))
{
    this->KeyEventSet.SetCallback = ptr;
}
static uint8 buttonGetPinState(EmvaButton *this)
{
    uint8 read = 0;
    character_dev_read(g_fd, this, (uint8 *)&read, 1);
    return read;
}
static int16 buttonCreate(EmvaButton *this)
{
    if (this->ButtonNumber == -1)
    {
        return -1;
    }
    character_dev_event(g_fd, this, KeyEventCallback);
    return this->ButtonNumber;
}
EmvaButton *newButton(void)
{
    EmvaButton *pButton = (EmvaButton *)emva_malloc(sizeof(EmvaButton));
    if (g_isInit == FALSE || pButton == NULL)
    {
        return NULL;
    }
    pButton->SetCallback = buttonSetCallback;
    pButton->Create = buttonCreate;
    pButton->GetPinState = buttonGetPinState;
    pButton->ButtonNumber = -1;
    // default interval time is 500 ms
    pButton->DoubleClickIntervalTime = 500;
    return pButton;
}
struct EmvaButtonApi EmvaButtonClass = {
    .new = newButton,
};
void EmvaButtonInitialize(void)
{
    g_fd = character_dev_open(EMVA_BUTTON_DRIVER_NAME, NULL);
    if (g_fd == -1)
    {
        g_isInit = FALSE;
        return;
    }
    g_isInit = TRUE;
    if (gButtonTime == NULL)
    {
        gButtonTime = EmvaTimeClass.newSub(3);
    }
}
#endif
