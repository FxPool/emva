// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_map.h"
#include "emva_malloc.h"
#include "string.h"
#include "util.h"

void CMapAdd(CemvaMap *this, CdMap *obj)
{
    EmvaLink_List_Insert(this->List, obj, -1);
}
void *CMapGet(CemvaMap *this, void *Key, int32 keyLen)
{
		for(uint16 i=0;i<this->List->length;i++)
		{
			CdMap *ThisMap = (CdMap *)EmvaLink_List_GetAt(this->List, i);
			if(ThisMap!=NULL)
			{
				if (is_equal((unsigned char *)ThisMap->key, (unsigned char *)Key, keyLen))
        {
           return ThisMap->value;
        }
			}
		}
		return NULL;
}
CemvaMap *CMapNew(void)
{
    CemvaMap *Map = (CemvaMap *)emva_malloc(sizeof(CemvaMap));
    Map->Add = CMapAdd;
    Map->Get = CMapGet;
    Map->List = EmvaLink_List_Init();
    return Map;
}
struct _CemvaMapApi CMapCalss = {
    .new = CMapNew,
};
