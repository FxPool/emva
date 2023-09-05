// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_SERIALIZTION_H
#define __EMVA_SERIALIZTION_H

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct
    {
        uint8 *MsgHead;
        uint32 MsgLength;
        uint32 MsgClass;
        uint8 *MsgPayLoad;
    } MsgProtocol_t;
    typedef struct _emvaSerialization emvaSerialization;
    struct _emvaSerialization
    {
        void (*FmtU8)(emvaSerialization *p, uint8 d);
        void (*FmtU16)(emvaSerialization *p, uint16 d);
        void (*FmtU32)(emvaSerialization *p, uint32 d);
        void (*FmtStr)(emvaSerialization *p, int8 *str);
        void (*FmtHex)(emvaSerialization *p, uint8 *Dt, uint16 len);
        void (*FmtHead)(emvaSerialization *p);
        void (*FmtEnd)(emvaSerialization *p);
        void (*FmtCmd)(emvaSerialization *p, uint32 cmd);
        void (*FmtCrc16)(emvaSerialization *p);
        uint8 *(*GetBuffer)(emvaSerialization *p);
        uint32 (*GetLength)(emvaSerialization *p);
        void (*Analysis)(emvaSerialization *this, void *d);
        MsgProtocol_t MsgProtocol;
        uint8 *payload;
        uint32 payloadLength;
		uint32 payloadLengthSet;
        uint32 msgLength;
        uint32 payloadOffset;
        uint8 rcode;
        uint8 *lock;
        uint8 MsgHeadWidth;
        uint8 MsgLengthWidth;
        uint8 MsgClassWidth;
    };
    struct EmvaSerializationApi
    {
        emvaSerialization *(*new)(uint8 MsgHeadWidth, uint8 MsgLengthWidth, uint8 MsgClassWidth, uint16 PaylodaLen, const char *lock, const uint8 rcode);
    };
    extern struct EmvaSerializationApi EmvaSerializationClass;
#ifdef __cplusplus
}
#endif

#endif
