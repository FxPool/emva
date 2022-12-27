
// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#ifndef __BSP_CONFIGURE_H
#define __BSP_CONFIGURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#define BSP_CFG_ARCH_AVAILABLE_TIEMX 4
#define DV_MEM_SIZE_SET 5 * 1024
#define DV_MEM_BANK_MIN_SIZE 8
#define DV_MEM_SIZE (DV_MEM_SIZE_SET)
#define DV_MMTS (DV_MEM_SIZE / DV_MEM_BANK_MIN_SIZE)
#define EMVA_MEM_SIZE_SET 2 * 1024
#define EMVA_MEM_BANK_MIN_SIZE 8
#define EMVA_MEM_SIZE (EMVA_MEM_SIZE_SET)
#define EMVA_MMTS (EMVA_MEM_SIZE / EMVA_MEM_BANK_MIN_SIZE)

#ifdef __cplusplus
}
#endif
#endif	
