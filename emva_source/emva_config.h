// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
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
#define EMVA_FM_WIFI_ENABLED 1
#endif

// <e> NET_ENABLED  - emva_fm_net - net framework initializes&support
#ifndef EMVA_FM_NET_ENABLED
#define EMVA_FM_NET_ENABLED 1
#endif
// <o> NET_FIFO_BUFFER_SIZE - FIFO of the underlying network packets. 
// <i> The cache size of the FIFO of the underlying network packets.
#ifndef NET_FIFO_BUFFER_SIZE
#define NET_FIFO_BUFFER_SIZE 512
#endif
// </e>

// <e> RF_PAIR_ENABLED  - emva_fm_rf_pair - rf pair framework initializes&support
#ifndef EMVA_FM_RF_PAIR_ENABLED
#define EMVA_FM_RF_PAIR_ENABLED 0
#endif
// <o> PAIR_TIME_OUT - 
#ifndef PAIR_TIME_OUT
#define PAIR_TIME_OUT 25000
#endif

// <o> ID_LENGTH - 
#ifndef ID_LENGTH
#define ID_LENGTH 12
#endif

// <o> CODING_LENGTH - 
#ifndef CODING_LENGTH
#define CODING_LENGTH 2
#endif

// <o> RX_BUF_LEN - 
#ifndef RX_BUF_LEN
#define RX_BUF_LEN 32
#endif

// <o> SUC_PCKED_NUM - 
#ifndef SUC_PCKED_NUM
#define SUC_PCKED_NUM 5
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

#ifdef __cplusplus
}
#endif
#endif
