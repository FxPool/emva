// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "hal.h"
#include "dv_serial_module.h"
#include "dv_malloc.h"
#include "dv_virtual_time.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <stdio.h>
/**
  * @brief timer out handler for usart receive data
  * @param  None
  * @retval : None
**/
static void timer_out_handler(void *p_arg)
{
    serial_moudle_t *s = (serial_moudle_t *)(p_arg);
    dvVirtualTime.stop.time1();
    if (s->data_callback != NULL && s->is_datacallback_flag == 0x01)
    {
        s->data_callback((uint8 *)s->p_receive_buf, s->p_receive_buf_length);
        s->p_receive_buf_length = 0;
    }
    s->receive_comolete_flag = 1;
}

/**
  * @brief module_data_input 
  * @param  data :one frame of data
  * @retval : None
**/
static void dev_data_input(serial_moudle_t *s, uint8 c_data)
{
    /*synchronous receive data*/
    if (s->synchronous_locked == 0x00)
    {
        dvVirtualTime.start.time1(s, timer_out_handler, s->p_property->package_time);
        s->p_receive_buf[s->p_receive_buf_length++] = c_data;
        if (s->p_receive_buf_length == s->receive_data_buffer_size)
        {
            if (s->data_callback != NULL)
            {
                s->data_callback((uint8 *)s->p_receive_buf, s->p_receive_buf_length);
            }
            s->p_receive_buf_length = 0;
        }
    }
}

/**
  * @brief    check input str is contained in the return data
  * @param  	str Comparison parameter
  * @retval : 1 success ,0 failure
**/
static uint8 check(serial_moudle_t *s, char *str)
{
    uint8 tmp_result;

    /*synchronous handler data*/
    s->synchronous_locked = 1;
    if (strstr((int8 *)s->p_receive_buf, str) != NULL)
    {
        tmp_result = 1;
    }
    else
    {
        tmp_result = 0;
    }
    s->synchronous_locked = 0;

    return tmp_result;
}

/**
  * @brief    tranmission the hex data
  * @param  	data hex data ,send data length 
  * @retval :   None
**/
static void hex_data_tranmission(serial_moudle_t *s, uint8 *data, uint16 length)
{
    s->p_receive_buf_length = 0;
    while (length--)
    {
        s->p_property->uart_send_oneByte(*data++);
    }
}
static void send_str(serial_moudle_t *s, int8 *str)
{
    hex_data_tranmission(s, (uint8 *)str, strlen(str));
}
static void inside_hex_data_tranmission(serial_moudle_t *s, uint8 *data, uint16 length)
{
    s->is_datacallback_flag = 1;
    hex_data_tranmission(s, data, length);
}
/**
  * @brief    sned the at cmd to module
  * @param  	cmd at cmd return_str @ compare string 
  * @retval : 1 success ,0 failure
**/
static uint16 inside_send_cmd_hex_data(serial_moudle_t *s, uint8 *cmd, uint16 size, uint16 interval_timer, uint8 *re_buff, uint16 re_size)
{

    /* receive the RETURN_FLAG flag ? if not,it will be send RE_SENT_CNT times.
	Each interval RE_SENT_INTERVAL_TIMER ms resend*/

    /*clear memory for p_receive_buf*/
    memset(s->p_receive_buf, 0, s->receive_data_buffer_size);

    s->is_datacallback_flag = 0;

    /*interval sent*/
    hex_data_tranmission(s, cmd, size);
    s->receive_comolete_flag = 0;
    uint16 m_cnt = interval_timer;
    while (m_cnt--)
    {
        if (s->receive_comolete_flag == 0x01)
        {
            break;
        }
        s->p_property->net_module_delay_ms(1);
    }

    /* get data */
    if (s->p_receive_buf_length != 0)
    {
        if (re_size > s->p_receive_buf_length)
        {
            memcpy(re_buff, s->p_receive_buf, s->p_receive_buf_length);
            s->is_datacallback_flag = 1;
            return s->p_receive_buf_length;
        }
    }
    s->is_datacallback_flag = 1;
    return 0;
}
/**
  * @brief    sned the at cmd to module
  * @param  	cmd at cmd return_str @ compare string 
  * @retval : 1 success ,0 failure
**/
static uint8 inside_send_cmd_check(serial_moudle_t *s, int8 *return_str, int8 *cmd, ...)
{

    char *pthiscmdBuf = s->p_cmd_buf;
    va_list ap;
    uint32 size = 0;
    va_start(ap, cmd);
    size = vsnprintf((char *)pthiscmdBuf, s->cmd_buf_length, cmd, ap);
    va_end(ap);
    pthiscmdBuf[size] = 0;

    uint8 re_sent_cnt = s->p_property->re_sent_cnt - 1;

    s->is_datacallback_flag = 1;

    /* receive the RETURN_FLAG flag ? if not,it will be send RE_SENT_CNT times.
	Each interval RE_SENT_INTERVAL_TIMER ms resend*/
    do
    {
        /*clear memory for p_receive_buf*/
        memset(s->p_receive_buf, 0, s->receive_data_buffer_size);

        /*interval sent*/
        send_str(s, pthiscmdBuf);
        s->receive_comolete_flag = 0;
        uint16 m_cnt = s->p_property->re_sent_interval_timer;
        while (m_cnt--)
        {
            if (s->receive_comolete_flag == 0x01)
            {
                break;
            }
            s->p_property->net_module_delay_ms(1);
        }

        /* if (@param) return_str indicates that there is no need to
		determine the validity of the returned data*/
        if (return_str == NULL)
        {
            if (s->p_receive_buf[0] != 0)
            {
                return 1;
            }
        }
        /*if the return flag is correct*/
        else if (check(s, return_str))
        {
            return 1;
        }
    } while (re_sent_cnt--);
    return 0;
}

/**
  * @brief    sned the at cmd to module
  * @param  	cmd at cmd return_str @ compare string 
  * @retval : 1 success ,0 failure
**/
static uint8 inside_send_cmd_check_with_timeout(serial_moudle_t *s, int8 *return_str, uint16 interval_timer, int8 *cmd, ...)
{

    char *pthiscmdBuf = s->p_cmd_buf;
    va_list ap;
    uint32 size = 0;
    va_start(ap, cmd);
    size = vsnprintf((char *)pthiscmdBuf, s->cmd_buf_length, cmd, ap);
    va_end(ap);
    pthiscmdBuf[size] = 0;

    /* receive the RETURN_FLAG flag ? if not,it will be send RE_SENT_CNT times.
	Each interval RE_SENT_INTERVAL_TIMER ms resend*/

    /*clear memory for p_receive_buf*/
    memset(s->p_receive_buf, 0, s->receive_data_buffer_size);

    s->is_datacallback_flag = 1;

    /*interval sent*/
    send_str(s, pthiscmdBuf);
    s->receive_comolete_flag = 0;
    uint16 m_cnt = interval_timer;
    while (m_cnt--)
    {
        if (s->receive_comolete_flag == 0x01)
        {
            break;
        }
        s->p_property->net_module_delay_ms(1);
    }

    /* if (@param) return_str indicates that there is no need to
	determine the validity of the returned data*/
    if (return_str == NULL)
    {
        if (s->p_receive_buf_length != 0)
        {
            return 1;
        }
    }
    /*if the return flag is correct*/
    else if (check(s, return_str))
    {
        return 1;
    }

    return 0;
}

static void inside_data_callback(serial_moudle_t *s, void (*ptr)(uint8 *data, uint16 length))
{
    s->data_callback = ptr;
}
static EMVA_BOOL inside_set_tranmission_mode(serial_moudle_t *s, TRANMISSION_MODE_U mode)
{
    return s->p_property->set_tranmission_mode(mode);
}

serial_moudle_t *new_serial_moudle(serial_moudle_interface_t *implement_serial_moudle_interface)
{
    serial_moudle_t *se = (serial_moudle_t *)dv_malloc(sizeof(serial_moudle_t));
    se->p_property = implement_serial_moudle_interface;
    se->p_receive_buf = (uint8 *)dv_malloc(sizeof(uint8) * se->p_property->set_receive_data_buffer_size);
    se->receive_data_buffer_size = se->p_property->set_receive_data_buffer_size;
    se->p_cmd_buf = (int8 *)dv_malloc(sizeof(int8) * se->p_property->set_cmd_buf_size);
    se->cmd_buf_length = se->p_property->set_cmd_buf_size;
    se->send_cmd_check = inside_send_cmd_check;
    se->send_cmd_check_with_timeout = inside_send_cmd_check_with_timeout;
    se->send_cmd_hex_data = inside_send_cmd_hex_data;
    se->send_hex_data = inside_hex_data_tranmission;
    se->set_data_callback = inside_data_callback;
    se->set_tranmission_mode = inside_set_tranmission_mode;
    se->p_property->set_data_call_back(se, dev_data_input);
    /* default open callback */
    se->is_datacallback_flag = 0x01;
    return se;
}
