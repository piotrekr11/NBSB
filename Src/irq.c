/*
 * irq.c
 *
 *  Created on: Jan 31, 2025
 *      Author: SK
 */

#include "irq.h"

static uint16_t exti_mapping_array[10]={0xFFFF};


void irq_map(const pin_t *p, uint8_t n)
{
	if (p==NULL) return;
	if (p->halport==NULL) return;
	if (n>15) return;

	IRQn_Type  m = pin_to_exti(p);
	if (m<0) return;


	GPIO_InitTypeDef   q;
    q.Pin =  p->halno;
	q.Mode = GPIO_MODE_IT_RISING;
	q.Pull = GPIO_NOPULL;
	q.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init( p->halport, &q );

	exti_mapping_array[n]=p->halno;

	NVIC_SetPriority( m , 0 );
	NVIC_EnableIRQ( m );
}


//void HAL_GPIO_EXTI_Callback( uint16_t gpioPin )       // PK: dla L476
void HAL_GPIO_EXTI_Rising_Callback( uint16_t gpioPin )  // PK: dla U545
{
	if (gpioPin==exti_mapping_array[0]) {dio0_handler();return;}
	if (gpioPin==exti_mapping_array[1]) {dio1_handler();return;}
	if (gpioPin==exti_mapping_array[2]) {dio2_handler();return;}
	if (gpioPin==exti_mapping_array[3]) {dio3_handler();return;}
	if (gpioPin==exti_mapping_array[4]) {dio4_handler();return;}
}




void EXTI0_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}

void EXTI1_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_1 );
}

void EXTI2_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_2 );
}

void EXTI3_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_3 );
}

void EXTI4_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_4 );
}

void EXTI5_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_5 );
}

void EXTI6_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_6 );
}

void EXTI7_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_7 );
}

void EXTI8_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_8 );
}

void EXTI9_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_9 );
}

void EXTI10_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_10 );
}

void EXTI11_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_11 );
}

void EXTI12_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_12 );
}

void EXTI13_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_13 );
}

void EXTI14_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_14 );
}

void EXTI15_IRQHandler( void )
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_15 );
}



