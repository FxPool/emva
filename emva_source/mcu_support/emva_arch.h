
// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

#ifndef __EMVA_ARCH_H
#define __EMVA_ARCH_H
#ifdef __cplusplus

extern "C"
{
#endif

#ifdef EMVA_ARCH_STM32F10X
    typedef unsigned char uint8;   /* Unsigned  8 bit quantity                           */
    typedef char int8;             /* Signed    8 bit quantity                           */
    typedef unsigned short uint16; /* Unsigned 16 bit quantity                           */
    typedef short int16;           /* Signed   16 bit quantity                           */
    typedef unsigned int uint32;   /* Unsigned 32 bit quantity                           */
    typedef int int32;             /* Signed   32 bit quantity                           */
    typedef float fp32;            /* Single precision floating point                    */
    typedef double fp64;           /* Double precision floating point                    */
    typedef unsigned char byte;    /* byte precision floating point                      */
    typedef enum
    {
        FALSE = 0,
        TRUE = !FALSE,
    } EMVA_BOOL;
#endif

#ifdef EMVA_ARCH_STM32F030F4PX
    typedef unsigned char uint8;   /* Unsigned  8 bit quantity                           */
    typedef signed char int8;      /* Signed    8 bit quantity                           */
    typedef unsigned short uint16; /* Unsigned 16 bit quantity                           */
    typedef signed short int16;    /* Signed   16 bit quantity                           */
    typedef unsigned int uint32;   /* Unsigned 32 bit quantity                           */
    typedef int int32;             /* Signed   32 bit quantity                           */
    typedef float fp32;            /* Single precision floating point                    */
    typedef double fp64;           /* Double precision floating point                    */
    typedef unsigned char byte;    /* byte precision floating point                      */
    typedef enum
    {
        FALSE = 0,
        TRUE = !FALSE,
    } EMVA_BOOL;
#endif
    // nedd mem manage implementation
    extern void emva_mem_pool_init(void);
    extern void *emva_mem_pool_malloc(uint32 size);
    extern uint8 emva_mem_pool_free(void *mem_pointer);

    // nedd arch init implementation
    extern inline void emva_arch_init(void);

    // nedd sleep implementation
    extern inline void os_sleep(uint32 nms);
    extern inline void os_sleepus(uint32 nus);

#ifdef __cplusplus
}
#endif

#endif
