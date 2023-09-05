// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_iterator.h"
#include "emva_list.h"
#include "emva_malloc.h"
#include "string.h"
#include "stdarg.h"
#if EMVA_FM_ITERITOR_ENABLED == 1
static object_t *thisNext(IteratorInterface *it)
{
    if (it->container->length == 0)
    {
        it->cursor = 0;
        return NULL;
    }
    return (object_t *)EmvaLink_List_GetAt(it->container, ++it->cursor);
}

static EMVA_BOOL thisHasNext(IteratorInterface *it)
{
    if (it->container->length == (it->cursor + 1))
    {
        it->cursor = -1;
        return FALSE;
    }
    return TRUE;
}

static void thisFreeContainer(AggregateInterface *ag)
{
    for (uint8 i = 0; i < ag->AggregateList->length; i++)
    {
        ag->thisContainer = (object_t *)EmvaLink_List_GetAt(ag->AggregateList, i);
        emva_free(ag->thisContainer->data);
        emva_free(ag->thisContainer);
    }
    EmvaLink_List_Clear(ag->AggregateList);
}

static void thisSetContainer(AggregateInterface *ag, byte *data, uint16 length)
{
    ag->thisContainer = emva_malloc(sizeof(object_t));
    ag->thisContainer->data = emva_malloc(length * sizeof(byte));
    memcpy(ag->thisContainer->data, data, length);
    ag->thisContainer->length = length;
    EmvaLink_List_Insert(ag->AggregateList, ag->thisContainer, -1);
}

static IteratorInterface *thisIterator(AggregateInterface *ag)
{
    IteratorInterface *it = (IteratorInterface *)emva_malloc(sizeof(IteratorInterface));
    it->next = thisNext;
    it->hasNext = thisHasNext;
    it->cursor = -1;
    it->container = ag->AggregateList;
    return it;
}

AggregateInterface *AggregateNew(void)
{
    AggregateInterface *ag = (AggregateInterface *)emva_malloc(sizeof(AggregateInterface));
    ag->ContainerAdd = thisSetContainer;
    ag->ContainerFree = thisFreeContainer;
    ag->iterator = thisIterator;
    ag->AggregateList = EmvaLink_List_Init();
    return ag;
}

struct EmvaIteratorApi EmvaEmvaIteratorApiClass = {
    .new = AggregateNew,
};
#endif
