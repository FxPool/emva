// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_serial_module.h"
#include "emva_malloc.h"
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
    s->p_property->broken_frame_timer_stop();
		s->call_buf_length = s->p_receive_buf_length;
    if (s->data_callback != NULL && s->is_datacallback_flag == 0x01)
    {
        s->data_callback((uint8 *)s->p_receive_buf, s->p_receive_buf_length);
    }
		s->p_receive_buf_length = 0;
    s->receive_complete_flag = 1;
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
        s->p_property->broken_frame_timer_start(s, timer_out_handler, s->package_time);
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
    hex_data_tranmission(s, data, length);
}
/**
  * @brief    sned the at cmd to module
  * @param  	cmd at cmd return_str @ compare string 
  * @retval : 1 success ,0 failure
**/
static uint8 inside_send_cmd_check(serial_moudle_t *s, void (*return_call)(const void*,uint16),int8 *return_str, int8 *cmd, ...)
{
    char *pthiscmdBuf = s->p_cmd_buf;
    va_list ap;
    uint32 size = 0;
    va_start(ap, cmd);
    size = vsnprintf((char *)pthiscmdBuf, s->cmd_buf_length, cmd, ap);
    va_end(ap);
    pthiscmdBuf[size] = 0;
    uint8 re_sent_cnt = s->p_property->re_sent_cnt - 1;
    /* receive the RETURN_FLAG flag ? if not,it will be send RE_SENT_CNT times.
	Each interval RE_SENT_INTERVAL_TIMER ms resend*/
    do
    {
				if(s->p_property->pre_cmd_send_delay>0)
				{
					s->p_property->net_module_delay_ms(s->p_property->pre_cmd_send_delay);
				}
				/*clear memory for p_receive_buf*/
        memset(s->p_receive_buf, 0, s->receive_data_buffer_size);
        /*interval sent*/
        send_str(s, pthiscmdBuf);
        s->receive_complete_flag = 0;
        uint16 m_cnt = s->p_property->cmd_sent_timeout;
        while (m_cnt--)
        {
            if (s->receive_complete_flag == 0x01)
            {
                break;
            }
            s->p_property->net_module_delay_ms(1);
        }

        /* if (@param) return_str indicates that there is no need to
		determine the validity of the returned data*/
				if(return_call)
				{
					return_call(s->p_receive_buf,s->call_buf_length);
					s->call_buf_length = 0;
				}
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
static uint8 inside_send_cmd_check_custom_params(serial_moudle_t *s, int8 *return_str, uint16 send_delayms,uint16 cmd_sent_timeout, int8 *cmd, ...)
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
		if(send_delayms>0)
		{
			s->p_property->net_module_delay_ms(send_delayms);
		}
		/*clear memory for p_receive_buf*/
		memset(s->p_receive_buf, 0, s->receive_data_buffer_size);
    send_str(s, pthiscmdBuf);
    s->receive_complete_flag = 0;
		/*interval sent*/
    uint16 m_cnt = cmd_sent_timeout;
    while (m_cnt--)
    {
        if (s->receive_complete_flag == 0x01)
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

static EMVA_BOOL inside_set_tranmission_mode(serial_moudle_t *s, TRANMISSION_MODE_U mode,TRANMISSION_MODE_OP_U op)
{
		if(mode ==TRANSMISSION_MODE)
		{
			s->is_datacallback_flag = 1;
			s->package_time = s->p_property->tran_package_time_ms;
		}else if (mode ==CMD_MODE)
		{
			s->is_datacallback_flag = 0;
			s->package_time = s->p_property->cmd_package_time_ms;
		}
    return s->p_property->set_tranmission_mode(mode,op);
}

serial_moudle_t *new_serial_moudle(serial_moudle_interface_t *implement_serial_moudle_interface)
{
    serial_moudle_t *se = (serial_moudle_t *)emva_malloc(sizeof(serial_moudle_t));
    se->p_property = implement_serial_moudle_interface;
		se->package_time = implement_serial_moudle_interface->tran_package_time_ms;
    se->p_receive_buf = (uint8 *)emva_malloc(sizeof(uint8) * se->p_property->set_receive_data_buffer_size);
    se->receive_data_buffer_size = se->p_property->set_receive_data_buffer_size;
    se->p_cmd_buf = (int8 *)emva_malloc(sizeof(int8) * se->p_property->set_cmd_buf_size);
    se->cmd_buf_length = se->p_property->set_cmd_buf_size;
    se->send_cmd_check = inside_send_cmd_check;
    se->send_cmd_check_custom_params = inside_send_cmd_check_custom_params;
    se->send_hex_data = inside_hex_data_tranmission;
    se->set_data_callback = inside_data_callback;
    se->set_tranmission_mode = inside_set_tranmission_mode;
    se->p_property->set_data_call_back(se, dev_data_input);
    return se;
}
