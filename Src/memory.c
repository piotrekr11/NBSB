/*
 * memory.c
 *
 *  Created on: 29 gru 2024
 *      Author: SK
 */


#include "memory.h"

void buf_init(buf_t*buf, uint32_t _limit)
{
	if (buf==NULL) return;
	if (_limit>MAX_PAYLOAD_LENGTH) _limit=MAX_PAYLOAD_LENGTH;

	buf->limit=_limit;
	buf->curr_size=0;
	buf->last_request=BUF_REQ_UNK;
	buf->last_request_size=0;
}



buf_ret_t buf_append_segment(buf_t*buf,uint8_t hm)
{
	buf_ret_t ret={NULL, 0, BUF_ERROR_OK};
	if (buf==NULL) return ret;

	buf->last_request = BUF_REQ_WRITE;

	if (buf->curr_size + hm > buf->limit)
	{
		hm=buf->limit - buf->curr_size;
		ret.error=BUF_ERROR_OVERRUN;
	}

	buf->last_request_size=hm;

	ret.ptr=buf->content + buf->curr_size ;
	ret.howmany=hm;

	return ret;
}


buf_ret_t buf_get_segment(buf_t*buf,uint8_t hm)
{
	buf_ret_t ret={NULL, 0, BUF_ERROR_OK};
	if (buf==NULL) return ret;

	buf->last_request = BUF_REQ_READ;

	if (hm > buf->curr_size)
	{
		hm = buf->curr_size;
		ret.error=BUF_ERROR_UNDERRUN;
	}

	buf->last_request_size=hm;

	ret.ptr=buf->content + buf->curr_size - hm;
	ret.howmany=hm;

	return ret;
}



void buf_confirm(buf_t*buf)
{
	if (buf==NULL) return;

	switch(buf->last_request)
	{
	case BUF_REQ_UNK: break;
	case BUF_REQ_WRITE:
		buf->curr_size += buf->last_request_size;
		break;
	case BUF_REQ_READ:
		buf->curr_size -= buf->last_request_size;
		break;
	default: break;
	}

	buf->last_request_size=0;
	buf->last_request=BUF_REQ_UNK;
}
