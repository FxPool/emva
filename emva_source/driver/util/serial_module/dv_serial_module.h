// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __DV_SERIAL_MODULE_H
#define __DV_SERIAL_MODULE_H

#ifdef __cplusplus

extern "C"
{
#endif
    typedef enum
    {
        CMD_MODE = 0,
        TRANSMISSION_MODE,
    } TRANMISSION_MODE_U;

    typedef struct _serial_moudle serial_moudle_t;
    typedef struct _serial_moudle_interface serial_moudle_interface_t;
    typedef void (*callback_Def)(uint8 *data, uint16 length);

    struct _serial_moudle
    {

        uint8 *p_receive_buf;
        uint16 p_receive_buf_length;
        uint16 receive_data_buffer_size;
        int8 *p_cmd_buf;
        uint16 cmd_buf_length;
        uint8 receive_comolete_flag;
        uint8 is_datacallback_flag;
        uint8 synchronous_locked;
        serial_moudle_interface_t *p_property;
        callback_Def data_callback;
        uint8 (*send_cmd_check_with_timeout)(serial_moudle_t *s, int8 *return_str, uint16 interval_timer, int8 *cmd, ...);
        uint8 (*send_cmd_check)(serial_moudle_t *s, int8 *return_str, int8 *cmd, ...);
        uint16 (*send_cmd_hex_data)(serial_moudle_t *s, uint8 *cmd, uint16 size, uint16 interval_timer, uint8 *re_buff, uint16 re_size);
        void (*send_hex_data)(serial_moudle_t *s, uint8 *cmd, uint16 size);
        void (*set_data_callback)(serial_moudle_t *s, void (*ptr)(uint8 *data, uint16 length));
        EMVA_BOOL(*set_tranmission_mode)
        (serial_moudle_t *s, TRANMISSION_MODE_U);
    };

    /*interface Def------------------------------------------------------------*/
    struct _serial_moudle_interface
    {
        EMVA_BOOL(*set_tranmission_mode)
        (TRANMISSION_MODE_U);
        void (*uart_send_oneByte)(uint8);
        void (*net_module_delay_ms)(uint32);
        void (*set_data_call_back)(serial_moudle_t *s, void (*ptr)(serial_moudle_t *s, uint8 d));
        uint8 re_sent_cnt;
        uint16 set_receive_data_buffer_size;
        uint16 set_cmd_buf_size;
        uint16 package_time;
        int8 *return_flag;
        uint32 re_sent_interval_timer;
    };

    serial_moudle_t *new_serial_moudle(serial_moudle_interface_t *implement_serial_moudle_interface);
#ifdef __cplusplus
}
#endif

#endif
