// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_ITERATOR_H_
#define __EMVA_ITERATOR_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "emva_list.h"

    typedef struct _IteratorInterface IteratorInterface;
    typedef struct _AggregateInterface AggregateInterface;

    struct _IteratorInterface
    {
        object_t *(*next)(IteratorInterface *it);
        EMVA_BOOL (*hasNext)
        (IteratorInterface *it);
        EmvaLink_List *container;
        int cursor;
    };

    struct _AggregateInterface
    {
        IteratorInterface *(*iterator)(AggregateInterface *ag);
        void (*ContainerAdd)(AggregateInterface *ag, byte *container, uint16 length);
        void (*ContainerFree)(AggregateInterface *ag);
        EmvaLink_List *AggregateList;
        object_t *thisContainer;
    };
    struct EmvaIteratorApi
    {
        AggregateInterface *(*new)(void);
    };
    extern struct EmvaIteratorApi EmvaEmvaIteratorApiClass;
#ifdef __cplusplus
}
#endif

#endif
