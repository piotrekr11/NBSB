/*
 * channel_sim.h
 *
 *  Created on: Feb 6, 2025
 *      Author: SK
 */

#ifndef INC_CHANNEL_SIM_H_
#define INC_CHANNEL_SIM_H_


#include "tools.h"

typedef struct err_pos
{
	uint32_t byte;
	uint8_t bit;
} err_pos_t;

#define SIM_ERR_NO (4)

uint32_t prng(void);
void sim_apply(uint8_t*buf, uint32_t hm);

#endif /* INC_CHANNEL_SIM_H_ */
