/*
This project is partially based on the following software:


 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Generic SX1276 driver implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/



#ifndef __RADIO_H__
#define __RADIO_H__

#include "irq.h"
#include "regs_fsk.h"
#include "memory.h"
#include "globals.h"
#include "alt_driver.h"
#include "payload_length.h"
#include "timer.h"
#include "cout.h"
#include "channel_sim.h"

void radio_reset(void);
void radio_init( void );
void radio_rx_cal(void);
void radio_init_registers(void);
void radio_set_modem(transceiver_mode_t t);
void radio_set_opmode(opmode_t opmode);
void radio_sleep(void);
void radio_stby( void );
void radio_tx(const uint8_t*buf, uint32_t size);
void radio_rx(void);
void radio_channel_sim(uint8_t*dest, uint32_t size);


#endif // __RADIO_H__
