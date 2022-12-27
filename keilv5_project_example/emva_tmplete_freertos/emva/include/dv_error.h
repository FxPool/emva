// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __DV_ERROR_H
#define __DV_ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _dv_error dv_error;
    struct _dv_error
    {
        char *err_info;
        uint16 err_size;
        void (*new)(dv_error *e, char *, ...);
    };
    dv_error *dv_error_new(uint16 err_size);
#ifdef __cplusplus
}
#endif

#endif
