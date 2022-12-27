// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "iterator.h"
#include "string.h"

void buttoncall(KeyEvent_t *k)
{
}
int main(void)
{
    emvaIniaialize();
    EmvaButton *button = emva.ButtonClass.new();
    button->ButtonNumber = 1;
    button->Create(button);
    button->SetCallback(button, buttoncall);
    while (1)
    {
    }
}
