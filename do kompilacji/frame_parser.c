#include "frame_parser.h"
#include <string.h>

static uint32_t crc32_stm(const uint8_t *data, uint32_t len)
{
    uint32_t crc = 0xFFFFFFFFu;
    for (uint32_t i = 0; i < len; i++) {
        crc ^= ((uint32_t)data[i] << 24);
        for (uint8_t b = 0; b < 8; b++) crc = (crc & 0x80000000u) ? (crc << 1) ^ 0x04C11DB7u : (crc << 1);
    }
    return crc;
}

void pc_frame_parser_init(pc_frame_parser_t *fp)
{
    if (fp == 0) return;
    fp->previous_count = 0;
    fp->has_previous_count = 0;
    fp->cnt = 0;
}

void pc_frame_parser_reset(pc_frame_parser_t *fp)
{
    pc_frame_parser_init(fp);
}

uint8_t pc_frame_parser_parse(pc_frame_parser_t *fp, const uint8_t *src, uint32_t len, uint16_t *out_count, uint8_t *out_payload, uint8_t *out_len)
{
    if (fp == 0 || src == 0 || out_count == 0 || out_payload == 0 || out_len == 0) return 0;
    if (len != INTERLEAVER_SIZE) return 0;

    memcpy(fp->rx_buf, src, INTERLEAVER_SIZE);
    interleaver_decode(fp->rx_buf, fp->encoded);
    if (hamming_decode_block(fp->encoded, FRAME_HAMMING_LEN, fp->raw) != FRAME_RAW_LEN) return 0;

    uint16_t sync = ((uint16_t)fp->raw[0] << 8) | fp->raw[1];
    if (sync != FRAME_SYNC) return 0;

    uint16_t count = ((uint16_t)fp->raw[2] << 8) | fp->raw[3];
    uint8_t data_len = fp->raw[4];
    if (data_len > FRAME_PAYLOAD_MAX) return 0;

    uint32_t rx_crc = ((uint32_t)fp->raw[FRAME_CRC_OFF + 0] << 24) |
                      ((uint32_t)fp->raw[FRAME_CRC_OFF + 1] << 16) |
                      ((uint32_t)fp->raw[FRAME_CRC_OFF + 2] << 8)  |
                      ((uint32_t)fp->raw[FRAME_CRC_OFF + 3]);
    if (crc32_stm(fp->raw, FRAME_CRC_OFF) != rx_crc) return 0;

    if (!fp->has_previous_count) {
        fp->previous_count = (uint16_t)(count - 1);
        fp->has_previous_count = 1;
    }
    fp->previous_count = count;
    fp->cnt++;

    memcpy(out_payload, &fp->raw[FRAME_PAYLOAD_OFF], data_len);
    *out_count = count;
    *out_len = data_len;
    return 1;
}
