/*
 * cout.c
 *
 *  Created on: Jan 30, 2025
 *      Author: SK
 */
#include "cout.h"


const char *__NULLSTR__ = "<null>";
const char *__LF__ = "\r\n";

#define __BUF_LENGTH__ (96)
char buf[__BUF_LENGTH__+4];

extern UART_HandleTypeDef huart1;




void print_s(const char *s)
{
	const uint8_t *ptr;

	if (s==NULL) ptr=(const uint8_t*)__NULLSTR__; else ptr=(const uint8_t*)s;

	while (ptr[0]!='\0')
	{
		HAL_UART_Transmit(&huart1,ptr,1, HAL_MAX_DELAY);
		ptr++;
	}
}

void print_lf()
{
	print_s(__LF__);
}


void print_s_lf(const char *s)
{
	print_s(s);
	print_lf();
}



void print_su(const char *s, uint32_t val)
{
	buf[__BUF_LENGTH__]='\0'; //for safety reasons
	if (s!=NULL) print_s(s);
	uint2str(buf,val);
	print_s(buf);
}


void print_su_lf(const char *s,uint32_t val)
{
	print_su(s,val);
	print_lf();
}


void print_sh(const char *s, uint32_t val)
{
	buf[__BUF_LENGTH__]='\0'; //for safety reasons
	if (s!=NULL) print_s(s);
	uint32_2_hex(buf,val);
	print_s(buf);
}


void print_sh_lf(const char *s,uint32_t val)
{
	print_sh(s,val);
	print_lf();
}


void print_reg8_lf(const char *s, uint8_t val, const char *format)
{
	buf[__BUF_LENGTH__]='\0'; //for safety reasons
	if (s!=NULL) print_s(s);
	uint8_2_bin(buf,val,format);
	print_s(buf);
	print_lf();
}



