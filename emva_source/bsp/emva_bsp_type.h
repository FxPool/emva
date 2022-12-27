
// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#ifndef __BSP_TYPE_H
#define __BSP_TYPE_H
#ifdef __cplusplus

extern "C"
{
#endif
    typedef unsigned char uint8;   /* Unsigned  8 bit quantity                           */
    typedef char int8;             /* Signed    8 bit quantity                           */
    typedef unsigned short uint16; /* Unsigned 16 bit quantity                           */
    typedef short int16;           /* Signed   16 bit quantity                           */
    typedef unsigned int uint32;   /* Unsigned 32 bit quantity                           */
    typedef int int32;             /* Signed   32 bit quantity                           */
    typedef float fp32_t;          /* Single precision floating point                    */
    typedef double fp64_t;         /* Double precision floating point                    */
    typedef unsigned char byte;    /* byte precision floating point                      */
    typedef enum
    {
        FALSE = 0,
        TRUE = !FALSE,
    } EMVA_BOOL;

#ifdef __cplusplus
}
#endif

#endif
