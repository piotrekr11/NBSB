//#ifndef INC_FRAME_PARSER_H_
//#define INC_FRAME_PARSER_H_
//
//#include "payload_length.h"
//#include "tools.h"
//#include "codec.h"
//#include "cout.h"
//
//#define FRAME_PARSER_OVERHEAD (sizeof(uint32_t)+sizeof(uint8_t))
//
//typedef struct frame_parser
//{
//	uint8_t buf[MAX_PAYLOAD_LENGTH+PAYLOAD_LENGTH_MARGIN];
//	uint32_t cnt;
//
//	uint32_t *counter;
//	uint8_t *length;
//	uint8_t *body;
//
//}frame_parser_t;
//
//
//void frame_parser_init(frame_parser_t *this);
//void frame_parser_reset(frame_parser_t *this);
//void frame_parser_parse(frame_parser_t *this,const uint8_t *src, uint32_t len);
//
//
//
//#endif /* INC_FRAME_PARSER_H_ */

#pragma once
#include <stdint.h>

#define FRAME_SYNC          0xB4BAU

#define FRAME_SYNC_LEN      2
#define FRAME_COUNTER_LEN   2
#define FRAME_LENFIELD_LEN  1
#define FRAME_PAYLOAD_MAX   9
#define FRAME_CRC_LEN       4

#define FRAME_HEADER_LEN    (FRAME_SYNC_LEN + FRAME_COUNTER_LEN + FRAME_LENFIELD_LEN)  /* 5  */
#define FRAME_PAYLOAD_OFF   FRAME_HEADER_LEN                                            /* 5  */
#define FRAME_CRC_OFF       (FRAME_PAYLOAD_OFF + FRAME_PAYLOAD_MAX)                     /* 14 */
#define FRAME_RAW_LEN       (FRAME_CRC_OFF + FRAME_CRC_LEN)                             /* 18 */

#define FRAME_ENCODED_LEN   50
#define FRAME_HAMMING_LEN   36
#define FRAME_FILLER_LEN    (FRAME_ENCODED_LEN - FRAME_HAMMING_LEN)                     /* 14 */

typedef struct {
    uint8_t  rx_buf[FRAME_ENCODED_LEN];
    uint8_t  encoded[FRAME_ENCODED_LEN];
    uint8_t  raw[FRAME_RAW_LEN];
    uint16_t previous_count;
    uint8_t  has_previous_count;
    uint32_t cnt;
} frame_parser_t;

void frame_parser_init(frame_parser_t *fp);
void frame_parser_reset(frame_parser_t *fp);
void frame_parser_parse(frame_parser_t *fp, const uint8_t *src, uint32_t len);
