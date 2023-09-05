// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "emva_serial.h"
#include "string.h"
#include "emva_kfifo.h"

uint8 buffer[256];

struct emva_kfifo *seriz_kfifo = NULL;
struct emva_kfifo new_seriz_kfifo;
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
    seriz_kfifo = &new_seriz_kfifo;
    emva_kfifo_init(seriz_kfifo, buffer, sizeof(buffer));
    uint8 buffer[20];
    uint8 len = 0;
    while (1)
    {
        len = emva_kfifo_get(seriz_kfifo, buffer, 20);
        if (len > 0)
        {
            // 开始用数据
        }
    }
}

void seris_irq(void)
{
    emva_kfifo_put(seriz_kfifo, data, 1);
}