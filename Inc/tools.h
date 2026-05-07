/*
 * tools.h
 *
 *  Created on: Dec 18, 2024
 *      Author: SK
 */

#ifndef INC_TOOLS_H_
#define INC_TOOLS_H_

#include <stdint.h>




#ifndef NULL
	#define NULL ((void*)(0))
#endif


#define MIN( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#define MAX( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )


#define U32_SET_BIT_ON(dest,pos)			{ (dest)|=((uint32_t)(1))<<(pos);}
#define U32_SET_BIT_OFF(dest,pos)			{ (dest)&=~(((uint32_t)(1))<<(pos));}
#define U32_GET_ONE_BIT(src,pos)   			((  (src)&(  ((uint32_t)(1)<<(pos)) ))>>(pos))
#define U32_SET_BITS(dest, val, Pos, Mask) 	{ uint32_t m=(Mask), p=(Pos), v=(val); dest&=~m; m>>=p; v&=m; v<<=p; dest|=v; }
#define U32_GET_BITS(src, Pos, Mask) 		( ((src)&(Mask))>>(Pos) )


#define U8_SET_BIT_ON(dest,pos)			{ (dest)|=((uint8_t)(1))<<(pos);}
#define U8_SET_BIT_OFF(dest,pos)		{ (dest)&=~(((uint8_t)(1))<<(pos));}
#define U8_GET_ONE_BIT(src,pos)   		((  (src)&(  ((uint8_t)(1)<<(pos)) ))>>(pos))
#define U8_SET_BITS(dest, val, Pos, Mask) { uint8_t m=(Mask), p=(Pos), v=(val); dest&=~m; m>>=p; v&=m; v<<=p; dest|=v; }
#define U8_GET_BITS(src, Pos, Mask) ( ((src)&(Mask))>>(Pos) )


uint8_t bits_2_byte(uint8_t b7, uint8_t b6, uint8_t b5, uint8_t b4, uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0);
void    byte_2_bits(uint8_t *b7, uint8_t *b6, uint8_t *b5, uint8_t *b4, uint8_t *b3, uint8_t *b2, uint8_t *b1, uint8_t *b0, uint8_t src);

#define IMUL(x1, x2_int,x2_num,x2_denom) ( (x1)*(x2_int) + (x1)*(x2_num)/(x2_denom) )

void uint2str(char *buf, uint32_t var);
void uint8_2_bin(char *buf, uint8_t var, const char *format);
void uint32_2_bin(char *buf, uint32_t var, const char *format);
void uint32_2_hex(char *buf, uint32_t var);

void fastmemcpy( uint8_t *dst, const uint8_t *src, uint32_t size );
void stringcpy(char*dest, const char *src);
uint32_t stringlen(const char *src);




#endif /* INC_TOOLS_H_ */
