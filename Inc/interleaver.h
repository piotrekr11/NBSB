#pragma once
#include <stdint.h>

#define INTERLEAVER_ROWS 5
#define INTERLEAVER_COLS 10
#define INTERLEAVER_SIZE (INTERLEAVER_ROWS * INTERLEAVER_COLS)

void interleaver_encode(const uint8_t *in, uint8_t *out);
void interleaver_decode(const uint8_t *in, uint8_t *out);
