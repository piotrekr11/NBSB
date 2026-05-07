#pragma once
#include <stdint.h>

/*
 * Hamming(8,4) — extended systematic block code
 *
 * Each nibble (4 data bits d3..d0) encodes to one 8-bit codeword:
 *
 *   p1 = d3 ^ d2 ^ d0
 *   p2 = d3 ^ d1 ^ d0
 *   p4 = d2 ^ d1 ^ d0
 *   p8 = p1 ^ p2 ^ d3 ^ p4 ^ d2 ^ d1 ^ d0   (overall parity — extended bit)
 *
 *   codeword bits [7..0]: p8 p1 p2 d3 p4 d2 d1 d0
 *
 * Properties:
 *   - 1-bit errors: correctable
 *   - 2-bit errors: detectable (not correctable)
 *   - Each nibble maps to exactly 1 byte — no bit-stream packing needed
 *
 * hamming_encode_block:
 *   in      - pointer to source bytes
 *   in_len  - number of source bytes (18 for this frame)
 *   out     - destination buffer, must hold in_len * 2 bytes (36 for 18-byte input)
 *   returns - number of bytes written (always in_len * 2)
 *
 * hamming_decode_block:
 *   in      - pointer to encoded bytes (36 for this frame)
 *   in_len  - number of encoded bytes (must be even)
 *   out     - destination buffer, must hold in_len / 2 bytes (18 for 36-byte input)
 *   returns - number of bytes written (always in_len / 2)
 *             NOTE: returns 0 if in_len is odd (invalid input)
 */
uint8_t hamming_encode_block(const uint8_t *in, uint8_t in_len, uint8_t *out);
uint8_t hamming_decode_block(const uint8_t *in, uint8_t in_len, uint8_t *out);
