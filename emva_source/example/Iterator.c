// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "iterator.h"
#include "string.h"

int main(void)
{
    emvaIniaialize();
    AggregateInterface *ag = emva.IteratorClass.new();
    ag->ContainerAdd(ag, (byte *)"s", 1);
    ag->ContainerAdd(ag, (byte *)"ab", 2);
    IteratorInterface *it = ag->iterator(ag);
    while (it->hasNext(it))
    {
        object_t *obj = (object_t *)it->next(it);
    }
}
