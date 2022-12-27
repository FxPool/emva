// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org
#include "hal.h"
#include "dv_error.h"
#include "dv_malloc.h"
#include "stdarg.h"
#include <stdio.h>

static void error_new_info(dv_error *e, char *format, ...)
{
  uint16 size;
  va_list ap;
  va_start(ap, format);
  size = vsnprintf((char *)e->err_info, e->err_size, format, ap);
  va_end(ap);
  e->err_info[size] = 0;
}

dv_error *dv_error_new(uint16 err_size)
{
  dv_error *pErr = (dv_error *)dv_malloc(sizeof(dv_error));
  pErr->new = error_new_info;
  pErr->err_size = err_size;
  pErr->err_info = (char *)dv_malloc(sizeof(char) * err_size);
  return pErr;
}
