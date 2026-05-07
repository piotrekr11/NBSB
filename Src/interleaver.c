#include "interleaver.h"

void interleaver_encode(const uint8_t *in, uint8_t *out)
{
    for (int r = 0; r < INTERLEAVER_ROWS; r++)
    {
        for (int c = 0; c < INTERLEAVER_COLS; c++)
        {
            out[c * INTERLEAVER_ROWS + r] = in[r * INTERLEAVER_COLS + c];
        }
    }
}

void interleaver_decode(const uint8_t *in, uint8_t *out)
{
    for (int r = 0; r < INTERLEAVER_ROWS; r++)
    {
        for (int c = 0; c < INTERLEAVER_COLS; c++)
        {
            out[r * INTERLEAVER_COLS + c] = in[c * INTERLEAVER_ROWS + r];
        }
    }
}
