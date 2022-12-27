// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dv_list.h"
#include "dv_malloc.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"

DvLink_List *DvLink_List_Init(void)
{
    DvLink_List *pList = NULL;
    pList = (DvLink_List *)dv_malloc(sizeof(DvLink_List));
    if (pList == NULL)
    {
        return NULL;
    }
    pList->length = 0;
    pList->head = NULL;
    pList->trail = NULL;
    return pList;
}

int DvLink_List_Insert(DvLink_List *pList, void *pData, long index)
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
        DvLink_Node *pNode = (DvLink_Node *)dv_malloc(sizeof(DvLink_Node));
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
            DvLink_Node *pNode = (DvLink_Node *)dv_malloc(sizeof(DvLink_Node));
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
            DvLink_Node *pNode = (DvLink_Node *)dv_malloc(sizeof(DvLink_Node));
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

            DvLink_Node *pNode = pList->head;
            i = 0;
            while (pNode != NULL)
            {
                if (index == i)
                {

                    DvLink_Node *pCurrentNode = (DvLink_Node *)dv_malloc(sizeof(DvLink_Node));
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

void *DvLink_List_GetAt(DvLink_List *pList, unsigned long index)
{
    int i = 0;
    DvLink_Node *pNode = NULL;
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
int32 DvLink_List_object_SearchAt(DvLink_List *pList, object_t *object)
{
    DvLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return -1;
    }
    int i = 0;
    pNode = pList->head;
    while (pNode != NULL)
    {
        if (isEqual((unsigned char *)pNode->data, (unsigned char *)object->data, object->length))
        {
            return i;
        }
        pNode = pNode->nextNode;
        i++;
    }
    return -1;
}
void DvLink_List_object_RemoveAt(DvLink_List *pList, object_t *object)
{
    DvLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return;
    }
    pNode = pList->head;
    while (pNode != NULL)
    {
        if (isEqual((unsigned char *)pNode->data, (unsigned char *)object->data, object->length))
        {
            if (pNode->priorNode == NULL)
            {
                DvLink_Node *head = pList->head;
                pList->head = head->nextNode;
                pList->head->priorNode = NULL;
                dv_free(head);
                pList->length--;
            }
            else if (pNode->nextNode == NULL)
            {
                DvLink_Node *trial = pList->trail;
                pList->trail = trial->priorNode;
                pList->trail->nextNode = NULL;
                dv_free(trial);
                pList->length--;
            }
            else
            {
                pNode->priorNode->nextNode = pNode->nextNode;
                pNode->nextNode->priorNode = pNode->priorNode;
                dv_free(pNode);
                pList->length--;
            }
            return;
        }
        pNode = pNode->nextNode;
    }
}
void DvLink_List_RemoveAt(DvLink_List *pList, unsigned long index)
{
    int i = 0;
    DvLink_Node *pNode = NULL;
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
                DvLink_List_Clear(pList);
                return;
            }

            if (pNode->priorNode == NULL)
            {
                DvLink_Node *head = pList->head;
                pList->head = head->nextNode;
                pList->head->priorNode = NULL;
                dv_free(head);
                pList->length--;
            }
            else if (pNode->nextNode == NULL)
            {
                DvLink_Node *trial = pList->trail;
                pList->trail = trial->priorNode;
                pList->trail->nextNode = NULL;
                dv_free(trial);
                pList->length--;
            }
            else
            {
                pNode->priorNode->nextNode = pNode->nextNode;
                pNode->nextNode->priorNode = pNode->priorNode;
                dv_free(pNode);
                pList->length--;
            }
            return;
        }
        pNode = pNode->nextNode;
        i++;
    }
}

void DvLink_List_Clear(DvLink_List *pList)
{
    DvLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return;
    }
    pNode = pList->trail;
    while (pNode != NULL)
    {
        pList->trail = pNode->priorNode;
        dv_free(pNode);
        pNode = pList->trail;
    }
    pList->length = 0;
    pList->head = NULL;
    pList->trail = NULL;
}

void DvLink_List_dv_free(DvLink_List *pList)
{
    DvLink_Node *pNode = NULL;
    if (pList == NULL)
    {
        return;
    }
    pNode = pList->trail;
    while (pNode != NULL)
    {
        pList->trail = pNode->priorNode;
        dv_free(pNode);
        pNode = pList->trail;
    }
    pList->length = 0;
    pList->head = NULL;
    pList->trail = NULL;
    dv_free(pList);
    pList = NULL;
}
