/**
  ******************************************************************************
  * @file    user_string.c
  * @author  hansen.jiang 951868692@qq.com
  * @version 3.0.1
  * @date    2015 10 1
  * @brief   
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "user_string.h"
#include "stdlib.h"
#include "string.h"

/**
  * @brief  This function 
  * @param  None
  * @retval : None
  */
	#define __GNUC__
char *xuser_itoa(int value, char *string, int radix)
{
 #if defined( __GNUC__ )
 // return ( (char*)itoa( value, string, radix ) );
#else
  unsigned char tmp1[10] = "", tmp2[10] = "", tmp3[10] = "";
  unsigned short num1, num2, num3;
  unsigned char i;

  string[0] = '\0';

  if ( radix == 10 )
  {
    num1 = value % 10000;
    num2 = (value / 10000) % 10000;
    num3 = (unsigned short)(value / 100000000);

    if (num3) _itoa(num3, tmp3, 10);
    if (num2) _itoa(num2, tmp2, 10);
    if (num1) _itoa(num1, tmp1, 10);

    if (num3)
    {
      strcpy((char*)string, (char const*)tmp3);
      for (i = 0; i < 4 - strlen((char const*)tmp2); i++)
        strcat((char*)string, "0");
    }
    strcat((char*)string, (char const*)tmp2);
    if (num3 || num2)
    {
      for (i = 0; i < 4 - strlen((char const*)tmp1); i++)
        strcat((char*)string, "0");
    }
    strcat((char*)string, (char const*)tmp1);
    if (!num3 && !num2 && !num1)
      strcpy((char*)string, "0");
  }
  else if ( radix == 16 )
  {
    num1 = value & 0x0000FFFF;
    num2 = value >> 8;num2 = num2 >> 8;

    if (num2) _itoa(num2, tmp2, 16);
    if (num1) _itoa(num1, tmp1, 16);

    if (num2)
    {
      strcpy((char*)string,(char const*)tmp2);
      for (i = 0; i < 4 - strlen((char const*)tmp1); i++)
        strcat((char*)string, "0");
    }
    strcat((char*)string, (char const*)tmp1);
    if (!num2 && !num1)
      strcpy((char*)string, "0");
  }
  else
    return NULL;

  return string;
#endif
}

/**
  * @brief  This function 
  * @param  None
  * @retval : None
  */
char *user_itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;
    return string;
} /* NCL_Itoa */
/**
  * @brief  This function 
  * @param  None
  * @retval : None
  */
user_int8* user_subString(user_int8* string,user_uint16 index)
{
	user_uint16 i = 0x00;
	for(i=0;i<strlen(string)-index;i++)
	{
		string[i] = string[i+index];
	}
	string[i] = 0x00;
	return string;
}
/**
  * @brief  This function 
  * @param  None
  * @retval : None
  */
user_int8* user_subnString(user_int8* string,user_uint16 index,user_uint16 len)
{
	string = user_subString(string,index);
	string[len] = 0x00;
	return string;
}
/**
  * @brief  This function 
  * @param  None
  * @retval : None
  */
void user_strtrim(user_int8* p_str)
{
	user_int8 *p_tmp = p_str;
    while (*p_str != '\0')
    {
        if (*p_str != ' ')
        {
            *p_tmp++ = *p_str;
        }
        ++p_str;
    }
    *p_tmp = '\0';
}
/**
  * @brief  This function
  * @param  None
  * @retval : None
  */
user_int16 user_IndexOf(const user_int8* string,user_int8* value)
{
	user_uint16 i = 0x00;
	while(string[i]!='\0')
	{
		if(strncmp(string+i,value,strlen(value))==0){
				return i;
		}
		i++;
	}
	return -1;
}
/**
  * @brief  This function
  * @param  None
  * @retval : None
  */
user_int16 user_CharAppear(const user_int8* string,user_int8* value)
{
	user_uint16 i = 0x00;
	user_uint16 j = 0x00;
	while(string[i]!='\0')
	{
		if(strncmp(string+i,value,strlen(value))==0){
			j++;
		}
		i++;
	}
	return j;
}
/*********************************************************************
 * @fn      user_memcmp
 *
 * @brief
 *
 *   Generic memory compare.
 *
 * @param   src1 - source 1 addrexx
 * @param   src2 - source 2 address
 * @param   len - number of bytes to compare
 *
 * @return  user_true - same, user_false - different
 */
user_bool user_memcmp(const user_uint8 *src1, const user_uint8 *src2, user_uint32 len)
{
  const user_uint8  *pSrc1;
  const user_uint8  *pSrc2;

  pSrc1 = src1;
  pSrc2 = src2;

  while ( len-- )
  {
    if( *pSrc1++ != *pSrc2++ )
      return user_false;
  }
  return user_true;
}

/*********************************************************************
 * @fn      user_memcpy
 *
 * @brief
 *
 *   Generic memory copy.
 *
 *   Note: This function differs from the standard memcpy(), since
 *         it returns the pointer to the next destination uint8. The
 *         standard memcpy() returns the original destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
void *user_memcpy( void *dst, const user_uint8 *src, user_uint32 len )
{
  user_uint8 *pDst;
  const user_uint8  *pSrc;

  pSrc = src;
  pDst = dst;

  while ( len-- )
    *pDst++ = *pSrc++;

  return ( pDst );
}
/**
  * @brief  自定义二进制连接函数，可以连接包括0x00在内的数据
  * @param  None
  * @retval : None
  */ 
user_uint8* user_strcat(user_uint8* strDest,const user_uint8* strSrc,user_uint16 Dlen,user_uint16 Slen)
{
    user_uint8* address=strDest;
    while(	Dlen-- )
    {
        strDest++;
    }
    while( Slen-- )
			*strDest++ = *strSrc++;
    return (address);
}
/**
  * @brief  
  * @param  None
  * @retval : None
  */ 
user_uint8* StringToHex(user_int8* hex)
{
	static user_uint8 buffer[8] = {0};
	user_uint8 i;
	user_uint16 rev1 = 0,rev2 = 0;
	for(i=0;i<strlen(hex)/2;i++)
	{
		rev1=hex[2*i];
		rev2=hex[2*i+1];
		if(rev1>=65){/**65-ASCII-A*/
			rev1-=55;
		}
		else{
			rev1-=48;
		}
		if(rev2>=65){
			rev2-=55;
		}
		else{
			rev2-=48;
		}
		buffer[i] = rev1*16 +rev2;
	}
	return buffer;
}
/**
  * @brief  This function 
  * @param  None
  * @retval : None
  */
void user_toupper(user_uint8* str)
{
	while(*str!='\0')
	{
		if(*str>='a' && *str<='z')
			*str = *str - 32;
		str++;
	}
}
/**
  * @brief  This function 
  * @param  None
  * @retval : None
  */
void SNtransforHEX(unsigned char sn[], char* p,int len)
{
	int i;
	int n = 0;
	int k = 0;
	int temp = 0;
	for(i = 0; i < len; i++)
	{
		if(p[i]>='A' && p[i]<='F')
		{
			n=p[i]-'A'+10;
		}
		else if(p[i] >= 'a'&& p[i]<='f') {
			n = p[i]-'a'+10;
		}
		else {
			n = p[i]-'0';
		}
		temp = temp*16+n;
		if(i%2) {
			sn[k] = temp;
			temp = 0;
			k++;
		}
	}
}
/**
  * @brief  This function 16进制转字符串
  * @param  None
  * @retval : Nonem 
  */
char* hex_to_str(char *pbDest, unsigned char *pbSrc, int nLen)
{
  char	ddl,ddh;
  int i;
  for (i=0; i<nLen; i++)
  {
    ddh = 48 + pbSrc[i] / 16;
    ddl = 48 + pbSrc[i] % 16;
    if (ddh > 57) ddh = ddh + 7;
    if (ddl > 57) ddl = ddl + 7;
    pbDest[i*2] = ddh;
    pbDest[i*2+1] = ddl;
  }
  pbDest[nLen*2] = '\0';
	return pbDest;
}
/**
  * @brief  hex 比较函数
  * @param  None
  * @retval : None
*/
int hexcmp(unsigned char *sc_hex,unsigned char *to_hex,int front_indexof) 
{
	int i;
	for(i=0;i<front_indexof;i++) {
			if(to_hex[i] != sc_hex[i])
					return 0;
	}
	return 1;
}
