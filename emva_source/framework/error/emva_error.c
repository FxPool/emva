// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org
#include "emva.h"
#include "emva_error.h"
#include "emva_malloc.h"
#include "stdarg.h"
#include <stdio.h>

static void error_new_info(emva_error *e, char *format, ...)
{
  uint16 size;
  va_list ap;
  va_start(ap, format);
  size = vsnprintf((char *)e->err_info, e->err_size, format, ap);
  va_end(ap);
  e->err_info[size] = 0;
}

emva_error *emva_error_new(uint16 err_size)
{
  emva_error *pErr = (emva_error *)emva_malloc(sizeof(emva_error));
  pErr->new = error_new_info;
  pErr->err_size = err_size;
  pErr->err_info = (char *)emva_malloc(sizeof(char) * err_size);
  return pErr;
}
