/*
 * payload_length.c
 *
 *  Created on: Jan 24, 2025
 *      Author: SK
 */

#include "payload_length.h"

uint8_t payload_length_set(payload_length_t *this, uint32_t N, uint8_t var )
{
	if (this==NULL) return 100;

	this->len=0;

	if (N>MAX_PAYLOAD_LENGTH) return 100; //TODO: remove after long payload support is added
	if (N<MIN_PAYLOAD_LENGTH) return 100;

	if ((N>MAX_PAYLOAD_LENGTH)    &&(var==0)) return 100; //fixed mode)
	if ((N>MAX_PAYLOAD_LENGTH_VAR)&&(var!=0)) return 100; //variable mode)

	this->len=N;
	this->var=var;


	return 0;
}

