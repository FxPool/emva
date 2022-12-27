/*
*********************************************************************************************************
*                                               USER_STRING
*                                         
*
*
*                                (c) Copyright 2015, Micrium, Weston, FL
*                                          All Rights Reserved
*
*                                           ARM Cortex-M3 Port
*
* File      : USER_STRING.H
* Version   : V2.00
* By        : MING HONG. JIANG
*
* For       : ARMv7M Cortex-M3
* Mode      : Thumb2
* Toolchain : RealView Development Suite
*             RealView Microcontroller Development Kit (MDK)
*             ARM Developer Suite (ADS)
*             Keil uVision
*********************************************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_STRING_H
#define __USER_STRING_H


/* Includes ------------------------------------------------------------------*/

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/
typedef unsigned char  user_uint8;                    	/* Unsigned  8 bit quantity                           */
typedef 			   char  user_int8;                 /* Signed    8 bit quantity                           */
typedef unsigned short user_uint16;                   	/* Unsigned 16 bit quantity                           */
typedef          short user_int16;                  	/* Signed   16 bit quantity                           */
typedef unsigned int   user_uint32;                  	/* Unsigned 32 bit quantity                           */
typedef 			   int   user_int32;                /* Signed   32 bit quantity                           */
typedef float          user_fp32;                     	/* Single precision floating point                    */
typedef double         user_fp64;                     	/* Double precision floating point                    */

typedef enum {user_false = 0, user_true = !user_false} user_bool;
/*整数转为字符串*/
char *user_itoa(int value, char *string, int radix);
/*从index开始截取字符串*/
user_int8* user_subString(user_int8* string,user_uint16 index);
/*从index截取len个字符串*/
user_int8* user_subnString(user_int8* string,user_uint16 index,user_uint16 len);
/*取字符串value在string中第一次出现的位置*/
user_int16 user_IndexOf(const user_int8* str,user_int8* value);
/*字符串拷贝函数*/
void *user_memcpy( void *dst, const user_uint8 *src, user_uint32 len );
/*字符串比较函数*/
user_bool user_memcmp(const user_uint8 *src1, const user_uint8 *src2, user_uint32 len);
/*二进制连接函数*/
user_uint8* user_strcat(user_uint8* strDest,const user_uint8* strSrc,user_uint16 Dlen,user_uint16 Slen);
/*计算字符出现的个数*/
user_int16 user_CharAppear(const user_int8* string,user_int8* value);
/*去除字符串中的空格*/
void user_strtrim(user_int8* p_str);
/*HEX 转字符串*/
char* hex_to_str(char *pbDest, unsigned char *pbSrc, int nLen);
/*二进制比较函数 相同返回1 不同返回0*/
int hexcmp(unsigned char *sc_hex,unsigned char *to_hex,int front_indexof);
#endif
