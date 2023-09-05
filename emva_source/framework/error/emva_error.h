// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_ERROR_H
#define __EMVA_ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _emva_error emva_error;
    struct _emva_error
    {
        char *err_info;
        uint16 err_size;
        void (*new)(emva_error *e, char *, ...);
    };
    emva_error *emva_error_new(uint16 err_size);
#ifdef __cplusplus
}
#endif

#endif
