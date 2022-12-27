// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_UTIL_H_
#define __EMVA_UTIL_H_
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct
    {
        byte *data;
        uint32 length;
    } object_t;
    uint8 isEqual(const byte *Sdata, const byte *Ddata, int32 length);
    uint16 MessageCountCRC16(uint8 *CRC_Data,uint8 CrcLenth);
#ifdef __cplusplus
}
#endif
#endif
