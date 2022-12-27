// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef _EMVA_LIST_H
#define _EMVA_LIST_H
#include "util.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _Link_Node
    {
        void *data;
        struct _Link_Node *priorNode;
        struct _Link_Node *nextNode;
    } Link_Node;
    typedef struct _Link_List
    {
        Link_Node *head;
        Link_Node *trail;
        unsigned long length;
    } Link_List;
    Link_List *Link_List_Init(void);
    int Link_List_Insert(Link_List *pList, void *pData, long index);
    void *Link_List_GetAt(Link_List *pList, unsigned long index);
    void Link_List_RemoveAt(Link_List *pList, unsigned long index);
    void Link_List_object_RemoveAt(Link_List *pList, object_t *object);
    int Link_List_object_SearchAt(Link_List *pList, object_t *object);
    void Link_List_Clear(Link_List *pList);
    void Link_List_Free(Link_List *pList);
#ifdef __cplusplus
}
#endif
#endif
