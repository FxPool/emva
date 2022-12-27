// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva_framework.h"
#include "emva_fmt.h"
#include "emva_malloc.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#if EMVA_FM_FMT_ENABLED == 1
static void this_write(emvaFmt *this, const uint8 *source, const uint16 szie)
{
    uint8 *pBuf = this->buffer;
    uint8 this_size = szie;
    if (this_size > this->bufsize)
    {
        this_size = this->bufsize;
    }
    memcpy(pBuf, source, this_size);
    if (this->output)
    {
        this->output((char *)pBuf, this_size);
    }
}

static void this_print(emvaFmt *this, const char *format, ...)
{
    uint8 *pBuf = this->buffer;
    va_list ap;
    uint32 size = 0;

    va_start(ap, format);
    size = vsnprintf((char *)pBuf, this->bufsize, format, ap);
    va_end(ap);
    pBuf[size] = 0;
    if (this->io)
    {
        this->io((char *)pBuf);
    }
}

static void this_println(emvaFmt *this, const char *format, ...)
{
    uint8 *pBuf = this->buffer;
    va_list ap;
    uint32 size = 0;

    va_start(ap, format);
    size = vsnprintf((char *)pBuf, this->bufsize, format, ap);
    va_end(ap);
    pBuf[size] = '\r';
    pBuf[size + 1] = '\n';
    pBuf[size + 2] = 0;
    if (this->io)
    {
        this->io((char *)pBuf);
    }
}

static void this_printhex(emvaFmt *this, const uint8 *source, const uint16 szie)
{
    for (uint16 i = 0; i < szie; i++)
    {
        this_print(this, "%02x ", (uint8)source[i]);
    }
}

// new
static emvaFmt *FmtNew(uint16 bufsize)
{
    emvaFmt *this_fmt = (emvaFmt *)emva_malloc(sizeof(emvaFmt));
    this_fmt->bufsize = bufsize;
    this_fmt->buffer = (uint8 *)emva_malloc(sizeof(uint8) * bufsize);
    this_fmt->write = this_write;
    this_fmt->print = this_print;
    this_fmt->println = this_println;
    this_fmt->printhex = this_printhex;
    return this_fmt;
}

// api
emva_fmt EmvafmtClass = {
    .new = FmtNew,
};
#endif
