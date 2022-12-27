// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_CONFIG_H
#define __EMVA_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif
// <<< Use Configuration Wizard in Context Menu >>>\n
// <h> Emva framwork Support

// <e> TIME_ENABLED  - emva_fm_time - time framework initializes&support
#ifndef EMVA_FM_TIME_ENABLED
#define EMVA_FM_TIME_ENABLED 1
#endif

// <o> SUB_TIMER_NUM - Number of self-timer. 
// <i> The Number of self-timer.
#ifndef SUB_TIMER_NUM
#define SUB_TIMER_NUM 8
#endif
// </e>

// <q> SERIAL_ENABLED  - emva_fm_serial - serial framework initializes&support
#ifndef EMVA_FM_SERIAL_ENABLED
#define EMVA_FM_SERIAL_ENABLED 1
#endif

// <q> WIFI_ENABLED  - emva_fm_wifi - wifi framework initializes&support
#ifndef EMVA_FM_WIFI_ENABLED
#define EMVA_FM_WIFI_ENABLED 0
#endif

// <e> NET_ENABLED  - emva_fm_net - net framework initializes&support
#ifndef EMVA_FM_NET_ENABLED
#define EMVA_FM_NET_ENABLED 0
#endif
// <o> NET_FIFO_BUFFER_SIZE - FIFO of the underlying network packets. 
// <i> The cache size of the FIFO of the underlying network packets.
#ifndef NET_FIFO_BUFFER_SIZE
#define NET_FIFO_BUFFER_SIZE 512
#endif
// </e>

// <q> BUTTON_ENABLED  - emva_fm_button - button framework initializes&support
#ifndef EMVA_FM_BUTTON_ENABLED
#define EMVA_FM_BUTTON_ENABLED 1
#endif

// <q> PMIC_ENABLED  - emva_fm_pmic - pmic framework initializes&support
#ifndef EMVA_PMIC_PMIC_ENABLED
#define EMVA_PMIC_PMIC_ENABLED 0
#endif

// <q> USBHID_ENABLED  - emva_fm_usbhid - usbhid framework initializes&support
#ifndef EMVA_FM_USBHID_ENABLED
#define EMVA_FM_USBHID_ENABLED 0
#endif

// <q> FMT_ENABLED  - emva_fm_fmt - fmt framework support
#ifndef EMVA_FM_FMT_ENABLED
#define EMVA_FM_FMT_ENABLED 1
#endif

// <q> IR_ENABLED  - emva_fm_ir - ir framework support
#ifndef EMVA_FM_IR_ENABLED
#define EMVA_FM_IR_ENABLED 0
#endif

// <q> ITERITOR_ENABLED  - emva_fm_Iterator - Iterator framework support
#ifndef EMVA_FM_ITERITOR_ENABLED
#define EMVA_FM_ITERITOR_ENABLED 1
#endif

// <q> KFIFO_ENABLED  - emva_fm_kfifo - kfifo framework support
#ifndef EMVA_FM_KFIFO_ENABLED
#define EMVA_FM_KFIFO_ENABLED 1
#endif

// <q> LIST_ENABLED  - emva_fm_list - list framework support
#ifndef EMVA_FM_LIST_ENABLED
#define EMVA_FM_LIST_ENABLED 1
#endif

// <q> OBSERVER_ENABLED  - emva_fm_observer - observer framework support
#ifndef EMVA_FM_OBSERVER_ENABLED
#define EMVA_FM_OBSERVER_ENABLED 1
#endif

// </h> 
//==========================================================
// <h> Libraries Support
// <q> CJSON_ENABLED  - cjson - Dave Gamble and cJSON contributors
#ifndef CJSON_ENABLED
#define CJSON_ENABLED 0
#endif

#define EMVA_DRIVER_VERSION "1.0.0"
#define EMVA_DRIVER_OWNER "emva"
#define EMVA_DRIVER_DATE "2021.10.12"
#define EMVA_WIFI_DRIVER_NAME "EMVA_WIFI"
#define EMVA_NET_DRIVER_NAME "EMVA_NET"
#define EMVA_SERIAL_DRIVER_NAME "EMVA_SERIAL"
#define EMVA_TIME_DRIVER_NAME "EMVA_TIME"
#define EMVA_BUTTON_DRIVER_NAME "EMVA_BUTTON"
#define EMVA_USB_HID_DRIVER_NAME "EMVA_USBHID"
#define EMVA_POWER_DRIVER_NAME "EMVA_PMIC"
#define EMVA_IR_DRIVER_NAME "EMVA_IR"
#ifdef __cplusplus
}
#endif
#endif
