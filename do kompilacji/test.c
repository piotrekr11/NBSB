#include "frame_former.h"
#include "frame_parser.h"

#include <stdio.h>
#include <string.h>

static int test_roundtrip(const char *msg)
{
    pc_frame_former_t tx;
    pc_frame_parser_t rx;
    pc_frame_former_init(&tx);
    pc_frame_parser_init(&rx);

    pc_frame_former_ret_t frame = pc_frame_former_next(&tx, (const uint8_t *)msg, (uint8_t)strlen(msg));

    uint16_t counter = 0;
    uint8_t payload[FRAME_PAYLOAD_MAX] = {0};
    uint8_t payload_len = 0;

    if (!pc_frame_parser_parse(&rx, frame.buf, frame.len, &counter, payload, &payload_len)) {
        return 0;
    }

    if (counter != 0) return 0;
    if (payload_len != (uint8_t)strlen(msg)) return 0;
    if (memcmp(payload, msg, payload_len) != 0) return 0;

    return 1;
}

int main(void)
{
    const char *cases[] = {"zwei", "abc", "123456789"};

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        if (!test_roundtrip(cases[i])) {
            printf("FAIL: %s\n", cases[i]);
            return 1;
        }
    }

    printf("OK\n");
    return 0;
}
