/*
 * codec.h
 *
 *  Created on: Dec 17, 2024
 *      Author: SK
 */

#ifndef INC_CODEC_H_
#define INC_CODEC_H_

#include <stdint.h>

#ifndef NULL
	#define NULL ((void*)(0))
#endif


void even_parity_enc(uint8_t *buf, uint8_t hm);
void even_parity_dec(uint8_t *buf, uint8_t hm, char errorchar);


#endif /* INC_CODEC_H_ */
