#include "hamming.h"

/*
 * Encode one nibble into an 8-bit Hamming(8,4) codeword.
 *
 * Bit layout of returned byte [7..0]: p8 p1 p2 d3 p4 d2 d1 d0
 *
 *   p1 = d3 ^ d2 ^ d0
 *   p2 = d3 ^ d1 ^ d0
 *   p4 = d2 ^ d1 ^ d0
 *   p8 = overall parity of the 7-bit codeword (extended Hamming bit)
 */
static uint8_t encode_nibble(uint8_t nibble)
{
	/* wyciagniecie bitow - tworzenie "a" */
    uint8_t d0 = (nibble >> 0) & 1;
    uint8_t d1 = (nibble >> 1) & 1;
    uint8_t d2 = (nibble >> 2) & 1;
    uint8_t d3 = (nibble >> 3) & 1;

    /* "mnozenie przez G" */
    uint8_t p1 = d3 ^ d2 ^ d0;
    uint8_t p2 = d3 ^ d1 ^ d0;
    uint8_t p4 = d2 ^ d1 ^ d0;

    /* slowo kodowe b */
    /* 7-bit codeword: p1 p2 d3 p4 d2 d1 d0 */
    uint8_t cw7 = (p1 << 6) | (p2 << 5) | (d3 << 4)
                | (p4 << 3) | (d2 << 2) | (d1 << 1) | d0;

    /* overall parity of all 7 bits → extended bit p8 */
    uint8_t p8 = p1 ^ p2 ^ d3 ^ p4 ^ d2 ^ d1 ^ d0;

    return (uint8_t)((p8 << 7) | cw7);
}

/*
 * Decode one Hamming(8,4) codeword back to a nibble.
 *
 * Procedure:
 *   1. Strip p8 (MSB) — used only for 2-bit error detection, not correction.
 *   2. Compute syndrome from the 7-bit codeword to locate and correct
 *      any single-bit error.
 *   3. Extract data bits d3..d0.
 *
 * Syndrome bits (s1, s2, s4) indicate the 1-based position of the flipped
 * bit in the 7-bit codeword [bit6..bit0] = [pos1..pos7]:
 *
 *   s1 = p1 ^ d3 ^ d2 ^ d0          (checks positions 1,3,5,7)
 *   s2 = p2 ^ d3 ^ d1 ^ d0          (checks positions 2,3,6,7)
 *   s4 = p4 ^ d2 ^ d1 ^ d0          (checks positions 4,5,6,7)
 *
 *   syndrome = s4<<2 | s2<<1 | s1    (1-based error position, 0 = no error)
 *
 * Position map in cw7 [bit6..bit0]:
 *   bit6=p1(pos1)  bit5=p2(pos2)  bit4=d3(pos3)
 *   bit3=p4(pos4)  bit2=d2(pos5)  bit1=d1(pos6)  bit0=d0(pos7)
 */
static uint8_t decode_nibble(uint8_t codeword)
{
    /* strip extended parity bit — not used for correction */
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

    /* weryfikacja detekcji bledu */
    uint8_t syndrome = (s4 << 2) | (s2 << 1) | s1;

    if (syndrome != 0) {
        /* flip the erroneous bit (syndrome gives 1-based position) */
        cw7 ^= (uint8_t)(1u << (7 - syndrome));

        /* re-extract data bits after correction */
        d3 = (cw7 >> 4) & 1;
        d2 = (cw7 >> 2) & 1;
        d1 = (cw7 >> 1) & 1;
        d0 = (cw7 >> 0) & 1;
    }

    return (uint8_t)((d3 << 3) | (d2 << 2) | (d1 << 1) | d0);
}

/* --------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------- */

/*
 * Encode in_len bytes using Hamming(8,4).
 * Each input byte produces 2 output bytes (high nibble first, then low nibble).
 * Output buffer must be at least in_len * 2 bytes.
 * Returns number of bytes written (always in_len * 2).
 */
uint8_t hamming_encode_block(const uint8_t *in, uint8_t in_len, uint8_t *out)
{
    uint8_t out_idx = 0;

    for (uint8_t i = 0; i < in_len; i++) {
        out[out_idx++] = encode_nibble((in[i] >> 4) & 0x0F);  /* high nibble */
        out[out_idx++] = encode_nibble( in[i]        & 0x0F); /* low nibble  */
    }

    return out_idx;
}

/*
 * Decode in_len Hamming(8,4) codeword bytes back to in_len / 2 data bytes.
 * Each pair of input bytes decodes to one output byte (high nibble, low nibble).
 * Output buffer must be at least in_len / 2 bytes.
 * Returns number of bytes written (always in_len / 2), or 0 if in_len is odd.
 */
uint8_t hamming_decode_block(const uint8_t *in, uint8_t in_len, uint8_t *out)
{
    if (in_len & 1) return 0;   /* must be even */

    uint8_t out_idx = 0;

    for (uint8_t i = 0; i < in_len; i += 2) {
        uint8_t high = decode_nibble(in[i]);
        uint8_t low  = decode_nibble(in[i + 1]);
        out[out_idx++] = (uint8_t)((high << 4) | low);
    }

    return out_idx;
}
