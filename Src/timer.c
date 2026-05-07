/*
 * timer.c
 *
 *  Created on: Jan 28, 2025
 *      Author: SK
 */

#include "timer.h"


void timer_init()
{
	SKTIM_CLK_ENABLE //enable TIM6 clock or use __HAL_RCC_TIM6_CLK_ENABLE();
	for(volatile uint32_t ii=0;ii<10000;ii++){}

	SKTIM->PSC=SKTIM_PSC_100US;
	NVIC_SetPriority(SKTIM_IRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_EnableIRQ(SKTIM_IRQ);

}


void timer_reset_100(uint32_t hm)
{
	if (hm==0)
	{
		U32_SET_BIT_OFF( SKTIM->CR1, TIM_CR1_CEN_Pos);    //disable
		U32_SET_BIT_OFF( SKTIM->DIER, TIM_DIER_UIE_Pos);  //disable interrupt
		return;
	}

	U32_SET_BIT_OFF( SKTIM->CR1, TIM_CR1_CEN_Pos); 		//disable
	U32_SET_BIT_OFF( SKTIM->DIER, TIM_DIER_UIE_Pos);		//disable interrupt
	SKTIM->ARR = hm & 0x000FFFFF;						//update counter
	U32_SET_BIT_ON( SKTIM->EGR, TIM_EGR_UG_Pos);			//re-init
	U32_SET_BIT_OFF( SKTIM->SR, TIM_SR_UIF_Pos);			//clear pending IRG flag
	U32_SET_BIT_ON( SKTIM->DIER, TIM_DIER_UIE_Pos);		//enable interrupt
	U32_SET_BIT_ON( SKTIM->CR1, TIM_CR1_CEN_Pos); 		//enable
}
