/*
 * tools.c
 *
 *  Created on: Dec 18, 2024
 *      Author: SK
 */


#include "tools.h"


void uint2str(char *buf, uint32_t var)
{
	if (buf==NULL) return;

	if (var==0) {buf[0]='0';buf[1]='\0';return;}

	uint32_t flag=0;
	uint32_t d=1000000000;

	for (;;)
	{
		uint32_t digit=var/d;
		if (digit==0)
		{
			if (flag>0) {buf[0]='0';buf++;}
		}
		else
		{
			flag=1;
			buf[0]=(char)(digit+48);
			buf++;
			var=var-digit*d;
		}

		d/=10;
		if (d==0) break; //for loop
	}

	buf[0]='\0';
}


static uint8_t format2groups(uint8_t*groups, uint8_t len, uint8_t *grlen, const char *format)
{

	groups[0]=len;
	grlen[0]=1;
	if (format==NULL) return 0; //ok
	if (format[0]=='\0') return 0; //ok


	uint8_t g=0, error_flag=0;
	for(uint8_t ii=0;ii<len+1;ii++)
	{
		if (format[ii]=='\0') break;
		if (ii==len) {error_flag=1;break;}
		if ((format[ii]<'1')||(format[ii]>'9')) {error_flag=1;break;}
		groups[ii]=format[ii]-'0';
		g+=groups[ii];
		grlen[0]=ii+1;
	}
	if (g!=len) error_flag=1;
	return error_flag;

}


void uint8_2_bin(char *buf, uint8_t var, const char *format)
{
	if (buf==NULL) return;

	uint8_t groups[8],gr_len;

	if (format2groups(groups,8,&gr_len,format)!=0) {stringcpy(buf, "<FORMAT ERROR>");return;};

	uint8_t mask=128, jj=0;
	for (uint8_t ii=0;ii<gr_len; ii++)
	{
		for (uint8_t pp=0;pp<groups[ii]; pp++)
		{
			if ((var&mask)==0) buf[jj]='0'; else buf[jj]='1';
			mask>>=1;
			jj++;
		}
		buf[jj]=' ';
		jj++;
	}
	buf[jj-1]='\0';

}


void uint32_2_bin(char *buf, uint32_t var, const char *format)
{
	if (buf==NULL) return;

	uint8_t groups[32],gr_len;

	if (format2groups(groups,32,&gr_len,format)!=0) {stringcpy(buf, "<FORMAT ERROR>");return;};

	uint32_t mask=(1<<31), jj=0;
	for (uint8_t ii=0;ii<gr_len; ii++)
	{
		for (uint8_t pp=0;pp<groups[ii]; pp++)
		{
			if ((var&mask)==0) buf[jj]='0'; else buf[jj]='1';
			mask>>=1;
			jj++;
		}
		buf[jj]=' ';
		jj++;
	}
	buf[jj-1]='\0';
}


void uint32_2_hex(char *buf, uint32_t var)
{
	if (buf==NULL) return;


	buf[8]='\0';

	uint8_t ii=8;
	for(;;)
	{
		ii--;
		uint32_t x = var & ((uint32_t)(0x0000000F));

		if (x<10)
		{
			buf[ii]=(char)x  + '0';
		}
		else
		{
			switch(x)
			{
				case 10: buf[ii]='A';break;
				case 11: buf[ii]='B';break;
				case 12: buf[ii]='C';break;
				case 13: buf[ii]='D';break;
				case 14: buf[ii]='E';break;
				case 15: buf[ii]='F';break;
				default: buf[ii]='_';break;
			}
		}

		if (ii==0) break;
		var>>=4;
	}
}



uint8_t bits_2_byte(uint8_t b7, uint8_t b6, uint8_t b5, uint8_t b4, uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0)
{
	return (b7<<7) | (b6<<6) | (b5<<5) | (b4<<4) | (b3<<3) | (b2<<2) | (b1<<1) | b0;
}

void    byte_2_bits(uint8_t *b7, uint8_t *b6, uint8_t *b5, uint8_t *b4, uint8_t *b3, uint8_t *b2, uint8_t *b1, uint8_t *b0, uint8_t src)
{
	*b7=U8_GET_ONE_BIT(src,7);
	*b6=U8_GET_ONE_BIT(src,6);
	*b5=U8_GET_ONE_BIT(src,5);
	*b4=U8_GET_ONE_BIT(src,4);
	*b3=U8_GET_ONE_BIT(src,3);
	*b2=U8_GET_ONE_BIT(src,2);
	*b1=U8_GET_ONE_BIT(src,1);
	*b0=U8_GET_ONE_BIT(src,0);
}


void stringcpy(char*dest, const char *src)
{
	uint32_t ii=0;
	for(;;)
	{
		dest[ii]=src[ii];
		if (dest[ii]=='\0') return;
		ii++;
	}
}

uint32_t stringlen(const char *src)
{
	if (src==NULL) return 0;
	uint32_t u=0;
	for(;;)
	{
		if (src[u]=='\0') return u;
		u++;
	}
}


void fastmemcpy( uint8_t *dst, const uint8_t *src, uint32_t size )
{
	if (dst==NULL) return;
	if (src==NULL) return;


	uint32_t q4=size/4;
	uint32_t q1=size%4;

	uint32_t 		*dst4=(uint32_t*)dst;
	const uint32_t  *src4=(const uint32_t*)src;

	for (uint32_t ii=0;ii<q4;ii++)
	{
		dst4[ii]=src4[ii];
	}

	dst4+=q4;
	src4+=q4;

	dst=(uint8_t*      )dst4;
	src=(const uint8_t*)src4;

	for (uint32_t ii=0;ii<q1;ii++)
	{
		dst[ii]=src[ii];
	}
}







uint8_t set_bits_uint8(uint8_t src, uint8_t val, uint8_t pos, uint8_t len)
{

	if (pos>7) return src; //error
	if (pos+len>8) return src; //error

	uint8_t mask=(255>>(8-len));
	val&=mask;
	val<<=pos;
	mask<<=pos;

	src&=~mask;
	src|=val;

	return src;

}


uint8_t get_bits_uint8(uint8_t src, uint8_t pos, uint8_t len)
{

	if (pos>7) return src; //error
	if (pos+len>8) return src; //error

	uint8_t mask=(255>>(8-len));
	mask<<=pos;
	return (src&mask)>>pos;

}

