/*
 * channel_sim.c
 *
 *  Created on: Feb 6, 2025
 *      Author: SK
 */



#include "channel_sim.h"




volatile uint32_t PRNS_SEED = 3333;


uint32_t prng(void)
{
	PRNS_SEED*=2039;
	PRNS_SEED%=995;
	return PRNS_SEED;
}


static err_pos_t sim_adjacent_pos(err_pos_t prev_pos, uint32_t buf_size)
{
	err_pos_t next_pos = prev_pos;

	next_pos.bit+=1;
	if (next_pos.bit>7)
	{
		next_pos.bit=0;
		next_pos.byte+=1;
	}
	if (next_pos.byte>=buf_size) next_pos.byte=0;
	return next_pos;
}


static void sim_toggle(uint8_t *buf, const err_pos_t *pos)
{
	for (uint32_t ii=0; ii<SIM_ERR_NO; ii++)
	{
		buf[pos[ii].byte]^=((uint8_t)1)<<pos[ii].bit;
	}
}

static void sim_error_positions(err_pos_t *pos, uint32_t buf_size)
{
	uint8_t adj_flag=0;
	for(uint32_t ii=0;ii<SIM_ERR_NO;ii++)
	{
		if (adj_flag==0)
		{
			pos[ii].byte=prng()%buf_size;
			pos[ii].bit=prng()%8;
		}
		else
		{
			pos[ii]=sim_adjacent_pos(pos[ii-1],buf_size);
		}

		adj_flag=prng()%2;
	}
}


void sim_apply(uint8_t*buf, uint32_t hm)
{
	if (buf==NULL) return;
	if (hm==0) return;

	err_pos_t pos[SIM_ERR_NO];
	sim_error_positions(pos,hm);
	sim_toggle(buf,pos);
}



