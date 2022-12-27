// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org
#ifndef __DV_MAP_H
#define __DV_MAP_H
#include "dv_list.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct
    {
        void *key;
        void *value;
    } CdMap;
    typedef struct _CDvMap CDvMap;
    struct _CDvMap
    {
        DvLink_List *List;
        void (*Add)(CDvMap *this, CdMap *obj);
        void *(*Get)(CDvMap *this, void *Key, int32 keyLen);
    };
    struct _CDvMapApi
    {
        CDvMap *(*new)(void);
    };
    extern struct _CDvMapApi CMapCalss;
#ifdef __cplusplus
}
#endif
#endif
