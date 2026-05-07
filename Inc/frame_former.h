/*
 * frame_former.h
 *
 *  Created on: Dec 17, 2024
 *      Author: SK
 */
//
//#ifndef INC_FRAME_FORMER_H_
//#define INC_FRAME_FORMER_H_
//
//#include "payload_length.h"
//#include "tools.h"
//#include "codec.h"
//#include "cout.h"
//
//#ifndef NULL
//	#define NULL ((void*)(0))
//#endif
//
//
//#define FRAME_FORMER_OVERHEAD (sizeof(uint32_t)+sizeof(uint8_t))
//
//typedef struct frame_former_ret
//{
//	const uint8_t *buf;
//	uint32_t len;
//}frame_former_ret_t;
//
//typedef struct frame_former
//{
//	const uint8_t *origin;
//	uint8_t buf[MAX_PAYLOAD_LENGTH+PAYLOAD_LENGTH_MARGIN];
//	payload_length_t payload_length;
//	const uint8_t *ptr;
//	uint32_t cnt;
//
//	uint32_t *counter;
//	uint8_t *length;
//	uint8_t *body;
//
//	uint8_t dbg;
//}frame_former_t;
//
//
//void frame_former_init(frame_former_t*ff, payload_length_t size, uint8_t _dbg);
//void frame_former_reset(frame_former_t*ff);
//frame_former_ret_t frame_former_next(frame_former_t*ff, uint8_t counter_init);
////void my_frame_former(uint8_t *buf);
//void my_frame_former(uint8_t *buf, uint8_t counter_init);
//
//
//
//#endif /* INC_FRAME_FORMER_H_ */
//
//#pragma once
//#include <stdint.h>
//
//#define FRAME_TOTAL_LEN     50
//
//#define FRAME_SYNC          0xB4BAU
//#define FRAME_SYNC_LEN      2
//#define FRAME_COUNTER_LEN   2
//#define FRAME_LENFIELD_LEN  1
//#define FRAME_PAYLOAD_MAX   9
//#define FRAME_HEADER_LEN    (FRAME_SYNC_LEN + FRAME_COUNTER_LEN + FRAME_LENFIELD_LEN)  /* 5  */
//#define FRAME_PAYLOAD_OFF   FRAME_HEADER_LEN                                            /* 5  */
//#define FRAME_PADDING_OFF   (FRAME_PAYLOAD_OFF + FRAME_PAYLOAD_MAX)                     /* 14 */
//#define FRAME_PADDING_LEN   (FRAME_TOTAL_LEN   - FRAME_PADDING_OFF)                     /* 36 */
//
//typedef struct {
//    uint8_t  buf[FRAME_TOTAL_LEN];
//    uint16_t counter;
//} frame_former_t;
//
//typedef struct {
//    const uint8_t *buf;
//    uint16_t       len;
//} frame_former_ret_t;
//
//void               frame_former_init(frame_former_t *ff);
//frame_former_ret_t frame_former_next(frame_former_t *ff);
//
//#pragma once
//#include <stdint.h>
//
//#define FRAME_TOTAL_LEN     50
//
//#define FRAME_SYNC          0xB4BAU
//#define FRAME_SYNC_LEN      2
//#define FRAME_COUNTER_LEN   2
//#define FRAME_LENFIELD_LEN  1
//#define FRAME_PAYLOAD_MAX   9
//#define FRAME_CRC_LEN       4
//
//#define FRAME_HEADER_LEN    (FRAME_SYNC_LEN + FRAME_COUNTER_LEN + FRAME_LENFIELD_LEN)  /* 5  */
//#define FRAME_PAYLOAD_OFF   FRAME_HEADER_LEN                                            /* 5  */
//#define FRAME_PADDING_OFF   (FRAME_PAYLOAD_OFF + FRAME_PAYLOAD_MAX)                     /* 14 */
//#define FRAME_CRC_OFF       (FRAME_TOTAL_LEN - FRAME_CRC_LEN)                           /* 46 */
//#define FRAME_PADDING_LEN   (FRAME_CRC_OFF - FRAME_PADDING_OFF)                         /* 32 */
//
//typedef struct {
//    uint8_t  buf[FRAME_TOTAL_LEN];
//    uint16_t counter;
//} frame_former_t;
//
//typedef struct {
//    const uint8_t *buf;
//    uint16_t       len;
//} frame_former_ret_t;
//
//void frame_former_init(frame_former_t *ff);
//frame_former_ret_t frame_former_next(frame_former_t *ff);

//
//#pragma once
//#include <stdint.h>
//
//#define FRAME_TOTAL_LEN     28   /* before Hamming encoding */
//
//#define FRAME_SYNC          0xB4BAU
//#define FRAME_SYNC_LEN      2
//#define FRAME_COUNTER_LEN   2
//#define FRAME_LENFIELD_LEN  1
//#define FRAME_PAYLOAD_MAX   9
//#define FRAME_CRC_LEN       4
//
//#define FRAME_HEADER_LEN    (FRAME_SYNC_LEN + FRAME_COUNTER_LEN + FRAME_LENFIELD_LEN)
//#define FRAME_PAYLOAD_OFF   FRAME_HEADER_LEN                        /* 5  */
//#define FRAME_PADDING_OFF   (FRAME_PAYLOAD_OFF + FRAME_PAYLOAD_MAX) /* 14 */
//#define FRAME_CRC_OFF       (FRAME_TOTAL_LEN - FRAME_CRC_LEN)       /* 24 */
//#define FRAME_PADDING_LEN   (FRAME_CRC_OFF - FRAME_PADDING_OFF)     /* 10 */
//
//#define FRAME_ENCODED_LEN   50   /* after Hamming(7,4) encoding + 1 filler byte */
//
//typedef struct {
//    uint8_t  buf[FRAME_TOTAL_LEN];
//    uint8_t  encoded[FRAME_ENCODED_LEN];
//    uint16_t counter;
//} frame_former_t;
//
//typedef struct {
//    const uint8_t *buf;
//    uint16_t       len;
//} frame_former_ret_t;
//
//void frame_former_init(frame_former_t *ff);
//frame_former_ret_t frame_former_next(frame_former_t *ff);

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
#define FRAME_HAMMING_LEN   36   /* 18 bytes * 2 nibbles * 8 bits (Hamming(8,4)) = 36 bytes */
#define FRAME_FILLER_LEN    (FRAME_ENCODED_LEN - FRAME_HAMMING_LEN)                     /* 14 */

typedef struct {
    uint8_t  buf[FRAME_RAW_LEN];
    uint8_t  encoded[FRAME_ENCODED_LEN];
    uint16_t counter;
} frame_former_t;

typedef struct {
    const uint8_t *buf;
    uint16_t       len;
} frame_former_ret_t;

void frame_former_init(frame_former_t *ff);
frame_former_ret_t frame_former_next(frame_former_t *ff);
