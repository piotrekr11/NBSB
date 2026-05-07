#include "frame_former.h"
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

void pc_frame_former_init(pc_frame_former_t *ff)
{
    if (ff == 0) return;
    ff->counter = 0;
}

pc_frame_former_ret_t pc_frame_former_next(pc_frame_former_t *ff, const uint8_t *payload, uint8_t payload_len)
{
    pc_frame_former_ret_t ret = {0, 0};
    static uint8_t tx_buf[INTERLEAVER_SIZE];
    if (ff == 0 || payload == 0) return ret;
    if (payload_len > FRAME_PAYLOAD_MAX) payload_len = FRAME_PAYLOAD_MAX;

    ff->buf[0] = (FRAME_SYNC >> 8) & 0xFF;
    ff->buf[1] = FRAME_SYNC & 0xFF;
    ff->buf[2] = (ff->counter >> 8) & 0xFF;
    ff->buf[3] = ff->counter & 0xFF;
    ff->buf[4] = payload_len;
    memset(&ff->buf[FRAME_PAYLOAD_OFF], ' ', FRAME_PAYLOAD_MAX);
    memcpy(&ff->buf[FRAME_PAYLOAD_OFF], payload, payload_len);

    uint32_t crc = crc32_stm(ff->buf, FRAME_CRC_OFF);
    ff->buf[FRAME_CRC_OFF + 0] = (crc >> 24) & 0xFF;
    ff->buf[FRAME_CRC_OFF + 1] = (crc >> 16) & 0xFF;
    ff->buf[FRAME_CRC_OFF + 2] = (crc >> 8) & 0xFF;
    ff->buf[FRAME_CRC_OFF + 3] = crc & 0xFF;

    ff->counter++;
    uint8_t encoded_len = hamming_encode_block(ff->buf, FRAME_RAW_LEN, ff->encoded);
    memset(&ff->encoded[encoded_len], ' ', FRAME_ENCODED_LEN - encoded_len);
    interleaver_encode(ff->encoded, tx_buf);

    ret.buf = tx_buf;
    ret.len = INTERLEAVER_SIZE;
    return ret;
}
