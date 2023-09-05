// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "string.h"

static void buttoncall(KeyEvent_t *k)
{
	switch((uint8)k->KeyState)
	{
		case KEY_DOWN:
			break;

		case KEY_UP:
			break;
		
		case KEY_DOUBLE:
			break;
	}
}
// Device init info
static void emvaDeviceInitInfo(uint16 code,const char* info)
{
	if(code!=0){
		while(1);
	}
}
int main(void)
{
    emvaIniaialize();
    emvaDeviceInit(emvaDeviceInitInfo);
    EmvaButton *button1 = emva.ButtonClass.new();
    button1->ButtonNumber = 0;
    button1->Create(button1);
    button1->SetCallback(button1, buttoncall);	
    while (1);
}
