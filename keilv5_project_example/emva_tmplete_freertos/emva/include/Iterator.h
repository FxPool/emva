// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __ITERATOR_H_
#define __ITERATOR_H_

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
        bool (*hasNext)(IteratorInterface *it);
        Link_List *container;
        int cursor;
    };

    struct _AggregateInterface
    {
        IteratorInterface *(*iterator)(AggregateInterface *ag);
        void (*ContainerAdd)(AggregateInterface *ag, byte *container, uint16 length);
        void (*ContainerFree)(AggregateInterface *ag);
        Link_List *AggregateList;
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
