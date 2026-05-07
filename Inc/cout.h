/*
 * cout.h
 *
 *  Created on: Jan 30, 2025
 *      Author: SK
 */

#ifndef INC_COUT_H_
#define INC_COUT_H_

#include "tools.h"
#include "stm32u5xx_hal.h"


void print_lf();

void print_s(const char *s);
void print_s_lf(const char *s);

void print_su(const char *s, uint32_t u);
void print_su_lf(const char *s, uint32_t u);

void print_sh(const char *s, uint32_t u);
void print_sh_lf(const char *s, uint32_t u);

void print_reg8_lf(const char *s, uint8_t val, const char *format);

#endif /* INC_COUT_H_ */
