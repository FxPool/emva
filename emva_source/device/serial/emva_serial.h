// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_SERIAL_H
#define __EMVA_SERIAL_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "emva_time.h"
    typedef enum
    {
        Parity_No = 0,
        Parity_Even,
        Parity_Odd,
    } SerialParityModeU;
    typedef enum
    {
        StopBits_1 = 0,
        StopBits_0_5,
        StopBits_2,
        StopBits_1_5,
    } SerialStopBitsModeU;
    typedef enum
    {
        WordLength_8b = 0,
        WordLength_9b,
    } SerialDataBitsModeU;
    typedef enum
    {
        DataCallBackEvent = 0,
    } SerialEventU;
    typedef struct emva_serial EmvaSerial;
    typedef void (*data_call_t)(byte *dt, uint32 length);
    typedef void (*data_event_t)(SerialEventU Sevent);
    struct emva_serial
    {
        uint8 portName;
        uint32 baudRate;
        uint16 dtLength;
        byte *dtBuffer;
        SerialParityModeU parity;
        SerialDataBitsModeU dataBits;
        SerialStopBitsModeU stopBits;
        EmvaTime *time;
        data_call_t dt_call;
        data_event_t dt_event;
        void (*open)(EmvaSerial *this);
        void (*close)(EmvaSerial *this);
        void (*write)(EmvaSerial *this, byte *dt, uint32 length);
        void (*read)(EmvaSerial *this, byte *dt, int32 nread, int32 *nrecv);
        void (*set_event_call)(EmvaSerial *this, void (*fuc)(SerialEventU Sevent));
        void (*set_callBack)(EmvaSerial *this, void (*fuc)(byte *dt, uint32 length));
    };
    struct EmvaSerialApi
    {
        EmvaSerial *(*new)(void);
    };
    extern struct EmvaSerialApi EmvaSerialClass;
    extern EMVA_BOOL EmvaSerialInitialize(void);
#ifdef __cplusplus
}
#endif

#endif
