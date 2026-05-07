/*
 * scan.h
 *
 *  Created on: Dec 19, 2024
 *      Author: SK
 */

#ifndef INC_SCAN_H_
#define INC_SCAN_H_

#include "alt_driver.h"
#include "globals.h"
#include "cout.h"


void scan(void);

void simple_regs_debug(const char *c);
void simple_mod_debug(const char *c);
void simple_irqf_debug(const char *c);
void simple_packet_config_debug(const char *c);
void simple_misc_debug(const char *c);
void simple_freq_debug(const char *c);
void simple_to_debug(const char *c);
void app_state_debug(const char *c);

#endif /* INC_SCAN_H_ */
