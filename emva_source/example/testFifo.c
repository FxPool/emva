// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "emva_serial.h"
#include "string.h"
#include "dv_kfifo.h"

uint8 buffer[256];

struct dv_kfifo *seriz_kfifo = NULL;
struct dv_kfifo new_seriz_kfifo;

int main(void)
{
    seriz_kfifo = &new_seriz_kfifo;
    dv_kfifo_init(seriz_kfifo, buffer, sizeof(buffer));
    uint8 buffer[20];
    uint8 len = 0;
    while (1)
    {
        len = dv_kfifo_get(seriz_kfifo, buffer, 20);
        if (len > 0)
        {
            // 开始用数据
        }
    }
}

void seris_irq(void)
{
    dv_kfifo_put(seriz_kfifo, data, 1);
}