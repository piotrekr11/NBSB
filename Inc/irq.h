/*
 * irq.h
 *
 *  Created on: Jan 31, 2025
 *      Author: SK
 */

#ifndef INC_IRQ_H_
#define INC_IRQ_H_

#include "gpio.h"

void dio0_handler(void);
void dio1_handler(void);
void dio2_handler(void);
void dio3_handler(void);
void dio4_handler(void);

void irq_map(const pin_t *p, uint8_t n);

#endif /* INC_IRQ_H_ */
