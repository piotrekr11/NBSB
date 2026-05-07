#include "frame_former.h"
#include "frame_parser.h"
#include <stdio.h>

int main(void)
{
    pc_frame_former_t tx;
    pc_frame_parser_t rx;
    pc_frame_former_init(&tx);
    pc_frame_parser_init(&rx);

    const uint8_t payload[] = "zwei";
    pc_frame_former_ret_t frame = pc_frame_former_next(&tx, payload, 4);

    uint16_t counter;
    uint8_t out_payload[FRAME_PAYLOAD_MAX];
    uint8_t out_len;

    if (!pc_frame_parser_parse(&rx, frame.buf, frame.len, &counter, out_payload, &out_len)) {
        printf("parse error\n");
        return 1;
    }

    printf("counter=%u len=%u data=%.*s\n", counter, out_len, out_len, out_payload);
    return 0;
}