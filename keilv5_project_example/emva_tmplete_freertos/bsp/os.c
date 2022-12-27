// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "bsp_type.h"
#include "emva_os.h"
#include "arch_stm32f411xx.h" 

inline void os_sleep(uint32_t nms)
{
	interface_delay_ms(nms);
}

inline void os_sleepus(uint32_t nus)
{
	interface_delay_us(nus);
}

