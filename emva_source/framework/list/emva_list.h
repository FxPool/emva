// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef _EMVA_LIST_H
#define _EMVA_LIST_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "util.h"
    typedef struct _EmvaLink_Node
    {
        void *data;
        struct _EmvaLink_Node *priorNode;
        struct _EmvaLink_Node *nextNode;
    } EmvaLink_Node;
    typedef struct _EmvaLink_List
    {
        EmvaLink_Node *head;
        EmvaLink_Node *trail;
        unsigned long length;
    } EmvaLink_List;
    EmvaLink_List *EmvaLink_List_Init(void);
    int EmvaLink_List_Insert(EmvaLink_List *pList, void *pData, long index);
    void *EmvaLink_List_GetAt(EmvaLink_List *pList, unsigned long index);
    void EmvaLink_List_RemoveAt(EmvaLink_List *pList, unsigned long index);
    int32 EmvaLink_List_object_SearchAt(EmvaLink_List *pList, object_t *object);
    void EmvaLink_List_object_RemoveAt(EmvaLink_List *pList, object_t *object);
    void EmvaLink_List_Clear(EmvaLink_List *pList);
    void EmvaLink_List_Free(EmvaLink_List *pList);
#ifdef __cplusplus
}
#endif
#endif
