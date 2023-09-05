// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "emva_iterator.h"
#include "string.h"

// Device init info
static void emvaDeviceInitInfo(uint16 code,const char* info)
{
	if(code!=0){
		
	}
}
int main(void)
{
    emvaIniaialize();
    emvaDeviceInit(emvaDeviceInitInfo);
    AggregateInterface *ag = emva.IteratorClass.new();
    ag->ContainerAdd(ag, (byte *)"s", 1);
    ag->ContainerAdd(ag, (byte *)"ab", 2);
    IteratorInterface *it = ag->iterator(ag);
    while (it->hasNext(it))
    {
        object_t *obj = (object_t *)it->next(it);
    }
}
