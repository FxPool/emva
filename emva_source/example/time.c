// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "iterator.h"
#include "string.h"

void t1call(void *arg) {}
void t2call(void *arg) {}
void t3call1(void *arg) {}
void t3call2(void *arg) {}
void t3call3(void *arg) {}

int main(void)
{
    emvaIniaialize();
    EmvaTime *t1 = emva.TimeClass.new();
    t1->start(t1, NULL, t1call, 1000);
    EmvaTime *t2 = emva.TimeClass.newSingleCycle();
    t2->start(t2, NULL, t2call, 100);
    EmvaTime *t3 = emva.TimeClass.newSub(3);
    t3->start(t3, NULL, t3call1, 200, 0);
    t3->start(t3, NULL, t3call2, 200, 1);
    t3->start(t3, NULL, t3call3, 200, 3);
    t1->stop(t1);
    while (1)
    {
    }
}
