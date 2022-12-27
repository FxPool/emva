// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_IR_TRANMISSION_H
#define __EMVA_IR_TRANMISSION_H

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _emva_ir_transmission EmvaIrTranmission;
    struct _emva_ir_transmission
    {
        bool (*loadDriver)(EmvaIrTranmission *this, const char *name);
        bool (*write)(EmvaIrTranmission *this, const void *d, uint16 len);
        bool (*callback)(EmvaIrTranmission *this, void (*ptr)(const void *d, uint16 len));
        int32 g_fd;
    };
    struct EmvaIrTransmitApi
    {
        EmvaIrTranmission *(*new)(void);
    };
    extern struct EmvaIrTransmitApi EmvaIrTransmitClass;
#ifdef __cplusplus
}
#endif

#endif
