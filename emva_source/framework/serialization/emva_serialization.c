// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_serialization.h"
#include "emva_malloc.h"
#include "string.h"

//8
static void ThisFmtU8(emvaSerialization *this, uint8 d)
{
    this->payload[this->payloadOffset + this->payloadLength++] = d;
}
//16
static void ThisFmtU16(emvaSerialization *this, uint16 d)
{
    memcpy(&this->payload[this->payloadOffset + this->payloadLength], &d, 2);
    this->payloadLength += 2;
}
//32
static void ThisFmtU32(emvaSerialization *this, uint32 d)
{
    memcpy(&this->payload[this->payloadOffset + this->payloadLength], &d, 4);
    this->payloadLength += 4;
}
//str
static void ThisFmtStr(emvaSerialization *this, char *str)
{
    memcpy(&this->payload[this->payloadOffset + this->payloadLength], str, strlen(str));
    this->payloadLength += strlen(str);
}
//hex
static void ThisFmtHex(emvaSerialization *this, uint8 *Dt, uint16 Len)
{
    memcpy(&this->payload[this->payloadOffset + this->payloadLength], Dt, Len);
    this->payloadLength += Len;
}
//heard
static void ThisFmtHead(emvaSerialization *this)
{
    this->payloadLength = 0;
    for (uint8 i = 0; i < this->MsgHeadWidth; i++)
    {
        this->payload[i] = this->lock[i] ^ this->rcode;
    }
}
//end
static void ThisFmtend(emvaSerialization *this)
{
    this->msgLength = this->payloadLength + this->MsgHeadWidth + this->MsgLengthWidth + this->MsgClassWidth;
    uint32 tmp_length = this->msgLength ^ this->rcode;
    memcpy(&this->payload[this->MsgHeadWidth], &tmp_length, this->MsgLengthWidth);
}
//cmd
static void ThisFmtCmd(emvaSerialization *this, uint32 cmd)
{
    uint32 tmp_cmd = cmd ^ this->rcode;
    memcpy(&this->payload[this->MsgHeadWidth + this->MsgLengthWidth], &tmp_cmd, this->MsgClassWidth);
}
//get length
static uint32 ThisGetLength(emvaSerialization *this)
{
    return this->msgLength;
}
//get buffer
static uint8 *ThisGetBuffer(emvaSerialization *this)
{
    return (uint8 *)this->payload;
}
// Analysis
void Protocol_Analysis(emvaSerialization *this, void *d)
{
    uint8 *Dt = (uint8 *)d;
    for (uint8 i = 0; i < this->MsgHeadWidth; i++)
    {
        this->MsgProtocol.MsgHead[i] = Dt[i] ^ this->rcode;
    }
    if (this->MsgLengthWidth == 1)
    {
        this->MsgProtocol.MsgLength = *(uint8 *)(Dt + this->MsgHeadWidth);
    }
    else if (this->MsgLengthWidth == 2)
    {
        this->MsgProtocol.MsgLength = *(uint16 *)(Dt + this->MsgHeadWidth);
    }
    else if (this->MsgLengthWidth == 4)
    {
        this->MsgProtocol.MsgLength = *(uint32 *)(Dt + this->MsgHeadWidth);
    }
    this->MsgProtocol.MsgLength ^= this->rcode;
    if (this->MsgClassWidth == 1)
    {
        this->MsgProtocol.MsgClass = *(uint8 *)(Dt + this->MsgHeadWidth + this->MsgLengthWidth);
    }
    else if (this->MsgClassWidth == 2)
    {
        this->MsgProtocol.MsgClass = *(uint16 *)(Dt + this->MsgHeadWidth + this->MsgLengthWidth);
    }
    else if (this->MsgClassWidth == 4)
    {
        this->MsgProtocol.MsgClass = *(uint32 *)(Dt + this->MsgHeadWidth + this->MsgLengthWidth);
    }
    this->MsgProtocol.MsgClass ^= this->rcode;
    memcpy(this->MsgProtocol.MsgPayLoad, (Dt + this->MsgHeadWidth + this->MsgLengthWidth + this->MsgClassWidth), this->payloadLengthSet - (this->MsgHeadWidth + this->MsgLengthWidth + this->MsgClassWidth));
}
//get object
emvaSerialization *ProtocolFormatNew(uint8 MsgHeadWidth, uint8 MsgLengthWidth, uint8 MsgClassWidth, uint16 PaylodaLen, const char *lock, const uint8 rcode)
{
    emvaSerialization *SendProtocol = (emvaSerialization *)emva_malloc(sizeof(emvaSerialization));
    SendProtocol->MsgProtocol.MsgHead = (uint8 *)emva_malloc(sizeof(uint8) * MsgHeadWidth);
    SendProtocol->payload = (uint8 *)emva_malloc(sizeof(uint8) * PaylodaLen);
    SendProtocol->MsgProtocol.MsgPayLoad = (uint8 *)emva_malloc(sizeof(uint8) * PaylodaLen);
    SendProtocol->lock = (uint8 *)emva_malloc(sizeof(uint8) * MsgHeadWidth);
    SendProtocol->FmtU8 = ThisFmtU8;
    SendProtocol->FmtU16 = ThisFmtU16;
    SendProtocol->FmtU32 = ThisFmtU32;
    SendProtocol->FmtHex = ThisFmtHex;
    SendProtocol->FmtStr = ThisFmtStr;
    SendProtocol->FmtHead = ThisFmtHead;
    SendProtocol->FmtEnd = ThisFmtend;
    SendProtocol->FmtCmd = ThisFmtCmd;
    SendProtocol->GetBuffer = ThisGetBuffer;
    SendProtocol->GetLength = ThisGetLength;
    SendProtocol->Analysis = Protocol_Analysis;
    SendProtocol->MsgHeadWidth = MsgHeadWidth;
    SendProtocol->MsgLengthWidth = MsgLengthWidth;
    SendProtocol->MsgClassWidth = MsgClassWidth;
    SendProtocol->payloadOffset = MsgHeadWidth + MsgLengthWidth + MsgClassWidth;
    SendProtocol->payloadLength = PaylodaLen;
    SendProtocol->payloadLengthSet = PaylodaLen;
    memcpy(SendProtocol->lock, lock, MsgHeadWidth);
    SendProtocol->rcode = rcode;
    return SendProtocol;
}
struct EmvaSerializationApi EmvaSerializationClass = {
    .new = ProtocolFormatNew,
};
