// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#ifndef __APP_MAIN_H
#define __APP_MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "task.h"

#define H_ID  1
extern TaskHandle_t MainTask_Handler;
void MainTask(void* p);

#ifdef __cplusplus
}
#endif

#endif
