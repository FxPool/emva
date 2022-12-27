// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#ifndef __EMVA_PMIC_H
#define __EMVA_PMIC_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct emva_pmic EmvaPmic;
    typedef struct
    {
        fp32_t bat;
        uint8 percent;
        bool isCharg;
    } emva_pmic_property;
    struct emva_pmic
    {
        void (*Shutdown)(EmvaPmic *);
        void (*Reboot)(EmvaPmic *);
        void (*Sleep)(EmvaPmic *);
        void (*Standby)(EmvaPmic *);
        void (*Stop)(EmvaPmic *);
        emva_pmic_property* (*GetProprety)(EmvaPmic *);
    };
    struct EmvaPmicApi
    {
        EmvaPmic *(*new)(void);
    };
    extern struct EmvaPmicApi EmvaPmicClass;
    extern void EmvaPmicInitialize(void);
#ifdef __cplusplus
}
#endif

#endif
