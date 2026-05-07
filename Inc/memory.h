/*
 * memory.h
 *
 *  Created on: 29 gru 2024
 *      Author: SK
 */

#ifndef INC_MEMORY_H_
#define INC_MEMORY_H_

#include "tools.h"
#include "payload_length.h"

#ifndef NULL
	#define NULL ((void*)(0))
#endif


typedef enum buf_error {BUF_ERROR_OK, BUF_ERROR_OVERRUN, BUF_ERROR_UNDERRUN}buf_error_t;
typedef enum buf_request {BUF_REQ_UNK, BUF_REQ_READ, BUF_REQ_WRITE}buf_request_t;

typedef struct buf
{
	uint8_t content[MAX_PAYLOAD_LENGTH+PAYLOAD_LENGTH_MARGIN];
	volatile uint32_t curr_size,limit, last_request_size;
	volatile buf_error_t error;
	volatile buf_request_t last_request;
}buf_t;



typedef struct buf_ret
{
	uint8_t *ptr;   			//dest pointer (write), source pointer (read)
	uint32_t howmany;			//how many bytes to read/write
	buf_error_t error;		    //error
}buf_ret_t;

void buf_init(buf_t*buf, uint32_t _limit);
buf_ret_t buf_append_segment(buf_t*buf, uint8_t hm);
buf_ret_t buf_get_segment(buf_t*buf, uint8_t hm);
void buf_confirm(buf_t*buf);







#endif /* INC_MEMORY_H_ */
