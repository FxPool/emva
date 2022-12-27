// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_FMT_H_
#define __EMVA_FMT_H_

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _fmt emvaFmt;
    struct _fmt
    {
        uint16 bufsize;
        uint8 *buffer;
        void (*write)(emvaFmt *this, const uint8 *source, const uint16 szie);
        void (*print)(emvaFmt *this, const char *format, ...);
        void (*println)(emvaFmt *this, const char *format, ...);
        void (*printhex)(emvaFmt *this, const uint8 *source, const uint16 szie);
        void (*io)(void *dt);
        void (*output)(void *dt, uint16 size);
    };
    typedef struct EmvaFmtApi emva_fmt;
    struct EmvaFmtApi
    {
        emvaFmt *(*new)(uint16 bufsize);
    };
    extern emva_fmt EmvafmtClass;
#ifdef __cplusplus
}
#endif
#endif
