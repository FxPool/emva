// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef _DV_LIST_H
#define _DV_LIST_H
#include "util.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _DvLink_Node
    {
        void *data;
        struct _DvLink_Node *priorNode;
        struct _DvLink_Node *nextNode;
    } DvLink_Node;
    typedef struct _DvLink_List
    {
        DvLink_Node *head;
        DvLink_Node *trail;
        unsigned long length;
    } DvLink_List;
    DvLink_List *DvLink_List_Init(void);
    int DvLink_List_Insert(DvLink_List *pList, void *pData, long index);
    void *DvLink_List_GetAt(DvLink_List *pList, unsigned long index);
    void DvLink_List_RemoveAt(DvLink_List *pList, unsigned long index);
    int32 DvLink_List_object_SearchAt(DvLink_List *pList, object_t *object);
    void DvLink_List_object_RemoveAt(DvLink_List *pList, object_t *object);
    void DvLink_List_Clear(DvLink_List *pList);
    void DvLink_List_Free(DvLink_List *pList);
#ifdef __cplusplus
}
#endif
#endif
