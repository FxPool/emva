// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_malloc.h"
#include "emva_led.h"
#include "string.h"

EmvaTime *ledTime = NULL;
#define RGB_TIME_EVENT 0
#define LED_PULSE1_TIME_EVENT 1
#define LED_PULSE2_TIME_EVENT 2
#define LED_PULSE3_TIME_EVENT 3
#define LED_PULSE4_TIME_EVENT 4
#define LED_PULSE5_TIME_EVENT 5

LED_Interface_t *pLED_Implements; //LED abstract function structure variable
uint8 red8, green8, blue8;        //Three primary color components
uint8 BreathingRate = 5;          //Respiratory rate unit ms
uint8 BreathingPeakLevel = 255;   //The peak breath brightness is 255 by default. This value can be used to change the breathing speed
uint8 BreathingCount = 0;
uint16 BreathingBTime = 0;
uint16 BreathingLTime = 0;
uint8 IsAllWalawaysBreak = 0;

typedef struct
{
    uint8 PulseFlag;
    uint16 negative_time;
    uint16 positive_time;
    uint8 timer_event;
    uint32 num;
    EMVA_BOOL state;
    EMVA_BOOL InfiniteFlag;
    void (*Interface)(EMVA_BOOL, void *);
    void *valuePtr;
} led_ctl_t;
led_ctl_t led_ctl[5];

// Pulse operation
static void PulseOperate(void *p)
{
    led_ctl_t *pLed = (led_ctl_t *)p;
    if (pLed->Interface != NULL)
    {
        pLed->Interface(pLed->PulseFlag > 0 ? FALSE : TRUE, pLed->valuePtr);
        pLed->PulseFlag = !pLed->PulseFlag;
        if (pLed->state == FALSE)
        {
            pLed->state = TRUE;
            if (pLed->InfiniteFlag == FALSE)
            {
                pLed->num--;
                if (pLed->num == 0)
                {
                    ledTime->stop(ledTime, pLed->timer_event);
                    return;
                }
            }
            ledTime->start(ledTime, pLed, PulseOperate, pLed->negative_time, pLed->timer_event);
        }
        else
        {
            pLed->state = FALSE;
            ledTime->start(ledTime, pLed, PulseOperate, pLed->positive_time, pLed->timer_event);
        }
    }
}

// Pulse Settings
static void LED_PulseStart(uint8 index, uint8 TimerEvent, void (*CtlInterface)(EMVA_BOOL, void *), void *v, uint16 positive_time, uint16 negative_time, uint16 num)
{
    led_ctl[index].state = TRUE;
    led_ctl[index].PulseFlag = 0;
    // Get the control interface
    led_ctl[index].Interface = CtlInterface;
    // Reversal time
    led_ctl[index].negative_time = negative_time;
    // Get the forward time
    led_ctl[index].positive_time = positive_time;
    // Get the timer event
    led_ctl[index].timer_event = TimerEvent;
    // Get the value callback
    led_ctl[index].valuePtr = v;
    // The number of times it was executed
    led_ctl[index].num = num;
    led_ctl[index].InfiniteFlag = (num > 0 ? FALSE : TRUE);
    // Start a timer for 1ms before changing the parameters
    ledTime->start(ledTime, &led_ctl[index], PulseOperate, 1, TimerEvent);
}
static void LED_Pulse1Start(void (*CtlInterface)(EMVA_BOOL, void *v), void *ptr, uint16 positive_time, uint16 negative_time, uint16 num)
{
    LED_PulseStart(0, LED_PULSE1_TIME_EVENT, CtlInterface, ptr, positive_time, negative_time, num);
}
static void LED_Pulse2Start(void (*CtlInterface)(EMVA_BOOL, void *v), void *ptr, uint16 positive_time, uint16 negative_time, uint16 num)
{
    LED_PulseStart(1, LED_PULSE2_TIME_EVENT, CtlInterface, ptr, positive_time, negative_time, num);
}
static void LED_Pulse3Start(void (*CtlInterface)(EMVA_BOOL, void *v), void *ptr, uint16 positive_time, uint16 negative_time, uint16 num)
{
    LED_PulseStart(2, LED_PULSE3_TIME_EVENT, CtlInterface, ptr, positive_time, negative_time, num);
}
static void LED_Pulse4Start(void (*CtlInterface)(EMVA_BOOL, void *v), void *ptr, uint16 positive_time, uint16 negative_time, uint16 num)
{
    LED_PulseStart(3, LED_PULSE4_TIME_EVENT, CtlInterface, ptr, positive_time, negative_time, num);
}
static void LED_Pulse5Start(void (*CtlInterface)(EMVA_BOOL, void *v), void *ptr, uint16 positive_time, uint16 negative_time, uint16 num)
{
    LED_PulseStart(4, LED_PULSE5_TIME_EVENT, CtlInterface, ptr, positive_time, negative_time, num);
}
static void LED_Pulse1Stop(void)
{
    if (led_ctl[0].Interface != NULL)
        led_ctl[0].Interface(FALSE, NULL);
    ledTime->stop(ledTime, LED_PULSE1_TIME_EVENT);
}
static void LED_Pulse2Stop(void)
{
    if (led_ctl[1].Interface != NULL)
        led_ctl[1].Interface(FALSE, NULL);
    ledTime->stop(ledTime, LED_PULSE2_TIME_EVENT);
}
static void LED_Pulse3Stop(void)
{
    if (led_ctl[2].Interface != NULL)
        led_ctl[2].Interface(FALSE, NULL);
    ledTime->stop(ledTime, LED_PULSE3_TIME_EVENT);
}
static void LED_Pulse4Stop(void)
{
    if (led_ctl[3].Interface != NULL)
        led_ctl[3].Interface(FALSE, NULL);
    ledTime->stop(ledTime, LED_PULSE4_TIME_EVENT);
}
static void LED_Pulse5Stop(void)
{
    if (led_ctl[4].Interface != NULL)
        led_ctl[4].Interface(FALSE, NULL);
    ledTime->stop(ledTime, LED_PULSE5_TIME_EVENT);
}

static void BreathingHandler(void *p)
{
    static uint16 B_Level = 0;
    static uint8 Br_Flag = 1;
    static uint8 EffectFlag = 0;
    if (EffectFlag)
    {
        EffectFlag = 0;
        ledTime->start(ledTime, NULL, BreathingHandler, BreathingRate, RGB_TIME_EVENT);
    }
    if (Br_Flag)
        B_Level++;
    else
        B_Level--;
    if (B_Level > BreathingPeakLevel)
    {
        Br_Flag = 0;
        ledTime->start(ledTime, NULL, BreathingHandler, BreathingLTime, RGB_TIME_EVENT);
        EffectFlag = 1;
    }
    if (B_Level == 0)
    {
        Br_Flag = 1;
#if 0
				if(BreathingOutTime > 0){
						ledTime->start(ledTime->instance,NULL,BreathingHandler,BreathingOutTime*100,RGB_TIME_EVENT);
				}
#else
        if (BreathingCount > 0)
        {
            ledTime->start(ledTime, NULL, BreathingHandler, BreathingBTime, RGB_TIME_EVENT);
        }
#endif
        EffectFlag = 1;
        if (IsAllWalawaysBreak == 0)
        {
            if (BreathingCount == 0)
            {
                ledTime->stop(ledTime, RGB_TIME_EVENT);
            }
            else
            {
                BreathingCount--;
            }
        }
    }
    *(uint16 *)pLED_Implements->r_value = (900 * B_Level / 255) * (red8) >> 8;
    *(uint16 *)pLED_Implements->g_value = (900 * B_Level / 255) * (green8) >> 8;
    *(uint16 *)pLED_Implements->b_value = (900 * B_Level / 255) * (blue8) >> 8;
}

void rgb_setBrightLevel(uint8 B_Level)
{
    *(uint16 *)pLED_Implements->r_value = (900 * B_Level / 255) * (red8) >> 8;
    *(uint16 *)pLED_Implements->g_value = (900 * B_Level / 255) * (green8) >> 8;
    *(uint16 *)pLED_Implements->b_value = (900 * B_Level / 255) * (blue8) >> 8;
}

static uint32 rgb_setValue(uint32 rgbValue, uint8 isBreathing, uint8 B_Level,
                           uint8 Par_BreathingCount, uint32 L_B)
{
    if (Par_BreathingCount == 255)
    {
        IsAllWalawaysBreak = 1;
    }
    red8 = rgbValue >> 16;
    green8 = rgbValue >> 8;
    blue8 = rgbValue;
    if (isBreathing & 0x80)
    {
        // You get the respiration rate and the peak brightness and the number of breaths
        BreathingRate = isBreathing & 0x7F;
        BreathingPeakLevel = B_Level;
#if 0
					BreathingCount = (Par_BreathingCount&0x3F) - 1;
					/* Get breathing dark time,1, 2, 3s*/
					BreathingBTime = (Par_BreathingCount&0xC0)>>6;
#else
        if (Par_BreathingCount > 0)
        {
            BreathingCount = Par_BreathingCount - 1;
        }
        /* Light on high, dark on low 0xFFFF0000*/
        BreathingBTime = L_B & 0xFFFF;
#endif
        ledTime->start(ledTime, NULL, BreathingHandler, BreathingRate, RGB_TIME_EVENT);
        return Par_BreathingCount * (BreathingLTime + BreathingBTime + (2 * BreathingRate * BreathingPeakLevel));
    }
    /* The non-breathing lightness is assigned to pwm */
    rgb_setBrightLevel(B_Level);
    // BreathingCount = 0;
    ledTime->stop(ledTime, RGB_TIME_EVENT);
    return 0;
}
/* *
* @brief This function rgb light color setting three components set separately
* Special only indicates the time of peak respiration rate
* 0x88,0x7F ->1916ms 25s takes 13 breaths
* @param  None
* @retval: The time to return this breath is steady on back to 0
* */
static uint32 rgb_setValue10(uint8 r, uint8 g, uint8 b, uint8 isBreathing, uint8 B_Level,
                             uint8 Par_BreathingCount, uint32 L_B)
{
    uint32 rgb_value = (r << 16) + (g << 8) + b;
    return rgb_setValue(rgb_value, isBreathing, B_Level, Par_BreathingCount, L_B);
}

led_t led =
    {
        .Pulse1Start = LED_Pulse1Start,
        .Pulse2Start = LED_Pulse2Start,
        .Pulse3Start = LED_Pulse3Start,
        .Pulse4Start = LED_Pulse4Start,
        .Pulse5Start = LED_Pulse5Start,
        .Pulse1Stop = LED_Pulse1Stop,
        .Pulse2Stop = LED_Pulse2Stop,
        .Pulse3Stop = LED_Pulse3Stop,
        .Pulse4Stop = LED_Pulse4Stop,
        .Pulse5Stop = LED_Pulse5Stop,
        .rgb_setValue10 = rgb_setValue10,
        .rgb_setValue = rgb_setValue,
};

void init_sysled(void)
{
    ledTime = emva.TimeClass.newSub(6);
    pLED_Implements = &LED_Interface;
    pLED_Implements->Init();
}
