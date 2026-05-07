/*
 * timer.h
 *
 *  Created on: Jan 28, 2025
 *      Author: SK
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "tools.h"
#include "stm32u5xx_hal.h"

#define SKTIM_PSC_100US 					(400-1)

#define SKTIM 								(TIM6)
#define SKTIM_CLK_ENABLE                    {RCC->APB1ENR1|=RCC_APB1ENR1_TIM6EN;}
#define SKTIM_IRQ							(TIM6_IRQn)
#define SKTIM_IRQ_HANDLER					TIM6_IRQHandler



void timer_reset_100(uint32_t hm);
void timer_init();

#endif /* INC_TIMER_H_ */
