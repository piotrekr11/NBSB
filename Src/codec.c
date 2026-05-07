/*
 * codec.c
 *
 *  Created on: Dec 17, 2024
 *      Author: SK
 */

#include "codec.h"

static inline uint8_t even_no_of_ones(const uint8_t b)
{

	uint8_t mask=(uint8_t)(1);
	uint8_t cnt=0;
	for (uint8_t ii=0;ii<8;ii++)
	{
		if ((b&mask)>0) cnt++;
		mask<<=1;
	}
	if (cnt%2==0) return 1; else return 0;
}


void even_parity_enc(uint8_t *buf, uint8_t hm)
{
	if (buf==NULL) return;

	for (uint8_t ii=0;ii<hm;ii++)
	{
		buf[ii]&=~((uint8_t)(128)); //zero bit 7
		if (even_no_of_ones(buf[ii])==0) buf[ii]|=((uint8_t)(128));
	}
}


void even_parity_dec(uint8_t *buf, uint8_t hm, char errorchar)
{
	if (buf==NULL) return;

	for (uint8_t ii=0;ii<hm;ii++)
	{
		if (even_no_of_ones(buf[ii])==0) buf[ii]=(uint8_t)errorchar;
		buf[ii]&=~((uint8_t)(128)); //remove the seventh bit
	}
}
