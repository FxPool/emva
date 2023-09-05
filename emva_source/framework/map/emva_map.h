// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org
#ifndef __EMVA_MAP_H
#define __EMVA_MAP_H
#include "emva_list.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct
    {
        void *key;
        void *value;
    } CdMap;
    typedef struct _CemvaMap CemvaMap;
    struct _CemvaMap
    {
        EmvaLink_List *List;
        void (*Add)(CemvaMap *this, CdMap *obj);
        void *(*Get)(CemvaMap *this, void *Key, int32 keyLen);
    };
    struct _CemvaMapApi
    {
        CemvaMap *(*new)(void);
    };
    extern struct _CemvaMapApi CMapCalss;
#ifdef __cplusplus
}
#endif
#endif
