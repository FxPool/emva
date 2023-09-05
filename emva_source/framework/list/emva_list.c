// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_list.h"
#include "emva_malloc.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include "util.h"
#if EMVA_FM_LIST_ENABLED == 1
EmvaLink_List *EmvaLink_List_Init(void)
{
    EmvaLink_List *pList = NULL;
    pList = (EmvaLink_List *)emva_malloc(sizeof(EmvaLink_List));
    if (pList == NULL)
    {
        return NULL;
    }
    pList->length = 0;
    pList->head = NULL;
    pList->trail = NULL;
    return pList;
}

int EmvaLink_List_Insert(EmvaLink_List *pList, void *pData, long index)
{
    long i = 0;
    if (pList == NULL)
        return -1;
    if (index < -1 || (index > pList->length && index != -1))
    {
        return -1;
    }
    if (pList->length == 0)
    {
        EmvaLink_Node *pNode = (EmvaLink_Node *)emva_malloc(sizeof(EmvaLink_Node));
        if (pNode == NULL)
            return -1;
        pNode->data = pData;
        pNode->priorNode = NULL;
        pNode->nextNode = NULL;
        pList->head = pNode;
        pList->trail = pNode;
        pList->length++;
        return 0;
    }
    else
    {
        if (-1 == index)
        {
            EmvaLink_Node *pNode = (EmvaLink_Node *)emva_malloc(sizeof(EmvaLink_Node));
            if (pNode == NULL)
                return -1;
            pNode->data = pData;
            pNode->nextNode = NULL;
            pNode->priorNode = pList->trail;

            pList->trail->nextNode = pNode;
            pList->trail = pNode;
            pList->length++;
        }
        else if (0 == index)
        {
            EmvaLink_Node *pNode = (EmvaLink_Node *)emva_malloc(sizeof(EmvaLink_Node));
            if (pNode == NULL)
                return -1;
            pNode->data = pData;
            pNode->nextNode = pList->head;
            pNode->priorNode = NULL;

            pList->head->priorNode = pNode;
            pList->head = pNode;
            pList->length++;
            return 0;
        }
        else
        {

            EmvaLink_Node *pNode = pList->head;
            i = 0;
            while (pNode != NULL)
            {
                if (index == i)
                {

                    EmvaLink_Node *pCurrentNode = (EmvaLink_Node *)emva_malloc(sizeof(EmvaLink_Node));
                    if (pCurrentNode == NULL)
                        return -1;
                    pCurrentNode->nextNode = pNode;
                    pCurrentNode->priorNode = pNode->priorNode;

                    pNode->priorNode->nextNode = pCurrentNode;
                    pNode->priorNode = pCurrentNode;
                    pList->length++;
                    return 0;
                }
                pNode = pNode->nextNode;
                i++;
            }
        }
    }
    return 0;
}

void *EmvaLink_List_GetAt(EmvaLink_List *pList, unsigned long index)
{
    int i = 0;
    EmvaLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return NULL;
    }
    i = 0;
    pNode = pList->head;
    while (pNode != NULL)
    {
        if (i == index)
        {
            return pNode->data;
        }
        pNode = pNode->nextNode;
        i++;
    }
    return NULL;
}

int EmvaLink_List_object_SearchAt(EmvaLink_List *pList, object_t *object)
{
    EmvaLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return -1;
    }
    int i = 0;
    pNode = pList->head;
    while (pNode != NULL)
    {
        if (is_equal((unsigned char *)pNode->data, (unsigned char *)object->data, object->length))
        {
            return i;
        }
        pNode = pNode->nextNode;
        i++;
    }
    return -1;
}

void EmvaLink_List_object_RemoveAt(EmvaLink_List *pList, object_t *object)
{
    EmvaLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return;
    }
    pNode = pList->head;
    while (pNode != NULL)
    {
        if (is_equal((unsigned char *)pNode->data, (unsigned char *)object->data, object->length))
        {
            if (pNode->priorNode == NULL)
            {
                EmvaLink_Node *head = pList->head;
                pList->head = head->nextNode;
                pList->head->priorNode = NULL;
                emva_free(head);
                pList->length--;
            }
            else if (pNode->nextNode == NULL)
            {
                EmvaLink_Node *trial = pList->trail;
                pList->trail = trial->priorNode;
                pList->trail->nextNode = NULL;
                emva_free(trial);
                pList->length--;
            }
            else
            {
                pNode->priorNode->nextNode = pNode->nextNode;
                pNode->nextNode->priorNode = pNode->priorNode;
                emva_free(pNode);
                pList->length--;
            }
            return;
        }
        pNode = pNode->nextNode;
    }
}

void EmvaLink_List_RemoveAt(EmvaLink_List *pList, unsigned long index)
{
    int i = 0;
    EmvaLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return;
    }
    i = 0;
    pNode = pList->head;
    while (pNode != NULL)
    {
        if (i == index)
        {
            if (pList->length == 0x01)
            {
                EmvaLink_List_Clear(pList);
                return;
            }

            if (pNode->priorNode == NULL)
            {
                EmvaLink_Node *head = pList->head;
                pList->head = head->nextNode;
                pList->head->priorNode = NULL;
                emva_free(head);
                pList->length--;
            }
            else if (pNode->nextNode == NULL)
            {
                EmvaLink_Node *trial = pList->trail;
                pList->trail = trial->priorNode;
                pList->trail->nextNode = NULL;
                emva_free(trial);
                pList->length--;
            }
            else
            {
                pNode->priorNode->nextNode = pNode->nextNode;
                pNode->nextNode->priorNode = pNode->priorNode;
                emva_free(pNode);
                pList->length--;
            }
            return;
        }
        pNode = pNode->nextNode;
        i++;
    }
}

void EmvaLink_List_Clear(EmvaLink_List *pList)
{
    EmvaLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return;
    }
    pNode = pList->trail;
    while (pNode != NULL)
    {
        pList->trail = pNode->priorNode;
        emva_free(pNode);
        pNode = pList->trail;
    }
    pList->length = 0;
    pList->head = NULL;
    pList->trail = NULL;
}

void EmvaLink_List_my_free(EmvaLink_List *pList)
{
    EmvaLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return;
    }
    pNode = pList->trail;
    while (pNode != NULL)
    {
        pList->trail = pNode->priorNode;
        emva_free(pNode);
        pNode = pList->trail;
    }
    pList->length = 0;
    pList->head = NULL;
    pList->trail = NULL;
    emva_free(pList);
    pList = NULL;
}
#endif
