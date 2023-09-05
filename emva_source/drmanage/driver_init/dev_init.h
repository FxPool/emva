// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org
#ifndef __DEV_INIT_H_
#define __DEV_INIT_H_

#ifdef __cplusplus
extern "C"
{
#endif
  typedef struct
  {
    void (*_init_func)(void);
  } dev_init_func_t;
#if defined(__CC_ARM) || defined(__CLANG_ARM) /* ARMCC Complier */
#define INIT_SECTION(x) __attribute__((section(x)))
#define INIT_USED __attribute__((used))
#elif defined(__ICCARM__) /* IAR ARMCC Complier */
#elif defined(__GNUC__)   /* GNUC Complier */
#else
#error "not support tool chain"
#endif

#define DV_MODULE_REG(handler) INIT_USED dev_init_func_t _init_##handler##_func INIT_SECTION("INIT_LIST") = { \
                                   handler,                                                                   \
}
  extern void drmanage_init(void);

#ifdef __cplusplus
}
#endif
#endif
