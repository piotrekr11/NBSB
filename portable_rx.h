#pragma once
#include <stdint.h>
#define FRAME_SYNC          0xB4BAU
#define FRAME_SYNC_LEN      2
#define FRAME_COUNTER_LEN   2
#define FRAME_LENFIELD_LEN  1
#define FRAME_PAYLOAD_MAX   9
#define FRAME_CRC_LEN       4

#define FRAME_HEADER_LEN    (FRAME_SYNC_LEN + FRAME_COUNTER_LEN + FRAME_LENFIELD_LEN)
#define FRAME_PAYLOAD_OFF   FRAME_HEADER_LEN
#define FRAME_CRC_OFF       (FRAME_PAYLOAD_OFF + FRAME_PAYLOAD_MAX)
#define FRAME_RAW_LEN       (FRAME_CRC_OFF + FRAME_CRC_LEN)

#define FRAME_ENCODED_LEN   50
#define FRAME_HAMMING_LEN   36
#define FRAME_FILLER_LEN    (FRAME_ENCODED_LEN - FRAME_HAMMING_LEN)

#define INTERLEAVER_ROWS 5
#define INTERLEAVER_COLS 10
#define INTERLEAVER_SIZE (INTERLEAVER_ROWS * INTERLEAVER_COLS)
typedef struct {
    uint8_t  rx_buf[FRAME_ENCODED_LEN];
    uint8_t  encoded[FRAME_ENCODED_LEN];
    uint8_t  raw[FRAME_RAW_LEN];
    uint16_t previous_count;
    uint8_t  has_previous_count;
    uint32_t cnt;
} pc_frame_parser_t;

uint8_t hamming_decode_block(const uint8_t *in, uint8_t in_len, uint8_t *out);
void interleaver_decode(const uint8_t *in, uint8_t *out);

void pc_frame_parser_init(pc_frame_parser_t *fp);
void pc_frame_parser_reset(pc_frame_parser_t *fp);
uint8_t pc_frame_parser_parse(pc_frame_parser_t *fp, const uint8_t *src, uint32_t len, uint16_t *out_count, uint8_t *out_payload, uint8_t *out_len);
