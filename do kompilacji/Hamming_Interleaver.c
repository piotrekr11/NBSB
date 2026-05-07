#include "frame_former.h"

static uint8_t encode_nibble(uint8_t nibble)
{
    uint8_t d0 = (nibble >> 0) & 1;
    uint8_t d1 = (nibble >> 1) & 1;
    uint8_t d2 = (nibble >> 2) & 1;
    uint8_t d3 = (nibble >> 3) & 1;
    uint8_t p1 = d3 ^ d2 ^ d0;
    uint8_t p2 = d3 ^ d1 ^ d0;
    uint8_t p4 = d2 ^ d1 ^ d0;
    uint8_t cw7 = (p1 << 6) | (p2 << 5) | (d3 << 4) | (p4 << 3) | (d2 << 2) | (d1 << 1) | d0;
    uint8_t p8 = p1 ^ p2 ^ d3 ^ p4 ^ d2 ^ d1 ^ d0;
    return (uint8_t)((p8 << 7) | cw7);
}

static uint8_t decode_nibble(uint8_t codeword)
{
    uint8_t cw7 = codeword & 0x7F;
    uint8_t p1 = (cw7 >> 6) & 1;
    uint8_t p2 = (cw7 >> 5) & 1;
    uint8_t d3 = (cw7 >> 4) & 1;
    uint8_t p4 = (cw7 >> 3) & 1;
    uint8_t d2 = (cw7 >> 2) & 1;
    uint8_t d1 = (cw7 >> 1) & 1;
    uint8_t d0 = (cw7 >> 0) & 1;
    uint8_t s1 = p1 ^ d3 ^ d2 ^ d0;
    uint8_t s2 = p2 ^ d3 ^ d1 ^ d0;
    uint8_t s4 = p4 ^ d2 ^ d1 ^ d0;
    uint8_t syndrome = (s4 << 2) | (s2 << 1) | s1;
    if (syndrome != 0) {
        cw7 ^= (uint8_t)(1u << (7 - syndrome));
        d3 = (cw7 >> 4) & 1;
        d2 = (cw7 >> 2) & 1;
        d1 = (cw7 >> 1) & 1;
        d0 = (cw7 >> 0) & 1;
    }
    return (uint8_t)((d3 << 3) | (d2 << 2) | (d1 << 1) | d0);
}

uint8_t hamming_encode_block(const uint8_t *in, uint8_t in_len, uint8_t *out)
{
    uint8_t out_idx = 0;
    for (uint8_t i = 0; i < in_len; i++) {
        out[out_idx++] = encode_nibble((in[i] >> 4) & 0x0F);
        out[out_idx++] = encode_nibble(in[i] & 0x0F);
    }
    return out_idx;
}

uint8_t hamming_decode_block(const uint8_t *in, uint8_t in_len, uint8_t *out)
{
    if (in_len & 1u) return 0;
    uint8_t out_idx = 0;
    for (uint8_t i = 0; i < in_len; i += 2) {
        uint8_t high = decode_nibble(in[i]);
        uint8_t low = decode_nibble(in[i + 1]);
        out[out_idx++] = (uint8_t)((high << 4) | low);
    }
    return out_idx;
}

void interleaver_encode(const uint8_t *in, uint8_t *out)
{
    for (int r = 0; r < INTERLEAVER_ROWS; r++)
        for (int c = 0; c < INTERLEAVER_COLS; c++)
            out[c * INTERLEAVER_ROWS + r] = in[r * INTERLEAVER_COLS + c];
}

void interleaver_decode(const uint8_t *in, uint8_t *out)
{
    for (int r = 0; r < INTERLEAVER_ROWS; r++)
        for (int c = 0; c < INTERLEAVER_COLS; c++)
            out[r * INTERLEAVER_COLS + c] = in[c * INTERLEAVER_ROWS + r];
}
