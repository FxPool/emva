// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "string.h"

// timer callback
void t1call(void *arg) 
{
	EmvaTime *t1 = (EmvaTime*)arg;
	t1->stop(t1);
}
void t2call(void *arg) {}
void t3call1(void *arg)
{
	EmvaTime *t3 = (EmvaTime*)arg;
	// can stop mul index sub timer
	t3->stop(t3,0,1);
}
void t3call2(void *arg)
{
	char* str = (char*)arg;
	str = str;
	// printf("%s",str)
}
void t3call3(void *arg) 
{
	EmvaTime *t3 = (EmvaTime*)arg;
	// can stop index sub timer
	t3->stop(t3,2);
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
		
		// cycle timer
    EmvaTime *t1 = emva.TimeClass.new();
		// params t1 input
    t1->start(t1, t1, t1call, 1000);
		
		// single cycle timer
    EmvaTime *t2 = emva.TimeClass.newSingleCycle();
		// NULL parmas input
    t2->start(t2, NULL, t2call, 100);
		
		// sub timer
    EmvaTime *t3 = emva.TimeClass.newSub(3);
		// params t3 input
    t3->start(t3, t3, t3call1, 200, 0);
		// params string input
    t3->start(t3, "Hello World", t3call2, 200, 1);
		// params t3 input
    t3->start(t3, t3, t3call3, 200, 2);
		
    while(1);
}
