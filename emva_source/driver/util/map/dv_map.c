// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dv_map.h"
#include "dv_malloc.h"
#include "string.h"
#include "util.h"

void CMapAdd(CDvMap *this, CdMap *obj)
{
    DvLink_List_Insert(this->List, obj, -1);
}
void *CMapGet(CDvMap *this, void *Key, int32 keyLen)
{
    CdMap *ThisMap = NULL;
    object_t object;
    object.data = Key;
    object.length = keyLen;
    ThisMap = (CdMap *)DvLink_List_GetAt(this->List, DvLink_List_object_SearchAt(this->List, &object));
    return ThisMap->value;
}
CDvMap *CMapNew(void)
{
    CDvMap *Map = (CDvMap *)dv_malloc(sizeof(CDvMap));
    Map->Add = CMapAdd;
    Map->Get = CMapGet;
    Map->List = DvLink_List_Init();
    return Map;
}
struct _CDvMapApi CMapCalss = {
    .new = CMapNew,
};
