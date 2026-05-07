#ifndef INC_APPLOOPS_H_
#define INC_APPLOOPS_H_

#include "cout.h"
#include "frame_former.h"
#include "frame_parser.h"
#include "tools.h"
#include "alt_driver.h"
#include "scan.h"
#include "globals.h"
#include "memory.h"
#include "radio.h"
#include "payload_length.h"
#include "timer.h"

void apploops(void);
void rx_loop(void);
void tx_loop(void);
void interferer_loop(void);


//set globals
uint8_t set_soft_tx(globals_t *g);
uint8_t set_soft_rx(globals_t *g);

//set radio registers using globals
void set_hard_tx(const globals_t *g);
void set_hard_rx(const globals_t *g);

//helper function
void set_hard_payload_length(payload_length_t p);

#endif /* INC_APPLOOPS_H_ */
