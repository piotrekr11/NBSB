/*
 * payload_length.h
 *
 *  Created on: Jan 23, 2025
 *      Author: SK
 */

#ifndef INC_PAYLOAD_LENGTH_H_
#define INC_PAYLOAD_LENGTH_H_

#include "tools.h"


#define MAX_PAYLOAD_LENGTH 			(60)  //TODO: support for long payload
//#define MAX_PAYLOAD_LENGTH 		(2000)
#define MAX_PAYLOAD_LENGTH_VAR 		(255)
#define MAX_PREAMBLE_LENGTH			(65536)
#define MAX_SYNC_LENGTH				(8)
#define VAR_PAYLOAD_LENGTH_REG		(2047)
#define PAYLOAD_LENGTH_MARGIN		(16)


#define MIN_PAYLOAD_LENGTH (2)


typedef struct payload_length
{
	uint32_t len;
	uint8_t var;
}payload_length_t;


uint8_t payload_length_set(payload_length_t *this, uint32_t N, uint8_t var );



#endif /* INC_PAYLOAD_LENGTH_H_ */
