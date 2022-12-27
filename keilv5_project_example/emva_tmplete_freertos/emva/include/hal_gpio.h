// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __HAL_GPIO_H
#define __HAL_GPIO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_type.h"

    struct hal_gpio_write
    {
        void (*pin0)(uint8 h_l);
        void (*pin1)(uint8 h_l);
        void (*pin2)(uint8 h_l);
        void (*pin3)(uint8 h_l);
        void (*pin4)(uint8 h_l);
        void (*pin5)(uint8 h_l);
        void (*pin6)(uint8 h_l);
        void (*pin7)(uint8 h_l);
        void (*pin8)(uint8 h_l);
        void (*pin9)(uint8 h_l);
        void (*pin10)(uint8 h_l);
        void (*pin11)(uint8 h_l);
        void (*pin12)(uint8 h_l);
        void (*pin13)(uint8 h_l);
        void (*pin14)(uint8 h_l);
        void (*pin15)(uint8 h_l);
        void (*pin16)(uint8 h_l);
        void (*pin17)(uint8 h_l);
        void (*pin18)(uint8 h_l);
        void (*pin19)(uint8 h_l);
        void (*pin20)(uint8 h_l);
        void (*pin21)(uint8 h_l);
        void (*pin22)(uint8 h_l);
        void (*pin23)(uint8 h_l);
        void (*pin24)(uint8 h_l);
        void (*pin25)(uint8 h_l);
        void (*pin26)(uint8 h_l);
        void (*pin27)(uint8 h_l);
        void (*pin28)(uint8 h_l);
        void (*pin29)(uint8 h_l);
        void (*pin30)(uint8 h_l);
        void (*pin31)(uint8 h_l);
        void (*pin32)(uint8 h_l);
        void (*pin33)(uint8 h_l);
        void (*pin34)(uint8 h_l);
        void (*pin35)(uint8 h_l);
        void (*pin36)(uint8 h_l);
        void (*pin37)(uint8 h_l);
        void (*pin38)(uint8 h_l);
        void (*pin39)(uint8 h_l);
        void (*pin40)(uint8 h_l);
        void (*pin41)(uint8 h_l);
        void (*pin42)(uint8 h_l);
        void (*pin43)(uint8 h_l);
        void (*pin44)(uint8 h_l);
        void (*pin45)(uint8 h_l);
        void (*pin46)(uint8 h_l);
        void (*pin47)(uint8 h_l);
        void (*pin48)(uint8 h_l);
        void (*pin49)(uint8 h_l);
        void (*pin50)(uint8 h_l);
        void (*pin51)(uint8 h_l);
        void (*pin52)(uint8 h_l);
        void (*pin53)(uint8 h_l);
        void (*pin54)(uint8 h_l);
        void (*pin55)(uint8 h_l);
        void (*pin56)(uint8 h_l);
        void (*pin57)(uint8 h_l);
        void (*pin58)(uint8 h_l);
        void (*pin59)(uint8 h_l);
        void (*pin60)(uint8 h_l);
    };

    struct hal_gpio_read
    {
        uint8 (*pin0)(void);
        uint8 (*pin1)(void);
        uint8 (*pin2)(void);
        uint8 (*pin3)(void);
        uint8 (*pin4)(void);
        uint8 (*pin5)(void);
        uint8 (*pin6)(void);
        uint8 (*pin7)(void);
        uint8 (*pin8)(void);
        uint8 (*pin9)(void);
        uint8 (*pin10)(void);
        uint8 (*pin11)(void);
        uint8 (*pin12)(void);
        uint8 (*pin13)(void);
        uint8 (*pin14)(void);
        uint8 (*pin15)(void);
        uint8 (*pin16)(void);
        uint8 (*pin17)(void);
        uint8 (*pin18)(void);
        uint8 (*pin19)(void);
        uint8 (*pin20)(void);
        uint8 (*pin21)(void);
        uint8 (*pin22)(void);
        uint8 (*pin23)(void);
        uint8 (*pin24)(void);
        uint8 (*pin25)(void);
        uint8 (*pin26)(void);
        uint8 (*pin27)(void);
        uint8 (*pin28)(void);
        uint8 (*pin29)(void);
        uint8 (*pin30)(void);
        uint8 (*pin31)(void);
        uint8 (*pin32)(void);
        uint8 (*pin33)(void);
        uint8 (*pin34)(void);
        uint8 (*pin35)(void);
        uint8 (*pin36)(void);
        uint8 (*pin37)(void);
        uint8 (*pin38)(void);
        uint8 (*pin39)(void);
        uint8 (*pin40)(void);
        uint8 (*pin41)(void);
        uint8 (*pin42)(void);
        uint8 (*pin43)(void);
        uint8 (*pin44)(void);
        uint8 (*pin45)(void);
        uint8 (*pin46)(void);
        uint8 (*pin47)(void);
        uint8 (*pin48)(void);
        uint8 (*pin49)(void);
        uint8 (*pin50)(void);
        uint8 (*pin51)(void);
        uint8 (*pin52)(void);
        uint8 (*pin53)(void);
        uint8 (*pin54)(void);
        uint8 (*pin55)(void);
        uint8 (*pin56)(void);
        uint8 (*pin57)(void);
        uint8 (*pin58)(void);
        uint8 (*pin59)(void);
        uint8 (*pin60)(void);
    };
    struct hal_gpio_nrf24l01
    {
        uint8 (*sda_read)(void);
        uint8 (*irq_pin_read)(void);
        void (*sda_write)(uint8 h_l);
        void (*scl)(uint8 h_l);
        void (*sck)(uint8 h_l);
    };

    struct hal_gpio
    {
        struct hal_gpio_write write;
        struct hal_gpio_read read;
        struct hal_gpio_nrf24l01 nrf24l01;
    };

#ifdef __cplusplus
}
#endif

#endif
