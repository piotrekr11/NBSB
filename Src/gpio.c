/*
 * gpio.c
 *
 *  Created on: Jan 29, 2025
 *      Author: SK
 */


#include "gpio.h"

#define __NOOF_PORTS__ (20)
uint8_t gpio_clocks[__NOOF_PORTS__]={0};

extern radio_t radio;
extern spi_t spi;
extern usart_t usart;


void pin_init_clock(pin_t *p)
{
	if (p==NULL) return;
	if (p->halport==NULL) return;

	uint32_t n=p->port_id-'A';

	switch (p->port_id)
	{
	case 'A':	if (gpio_clocks[n]==0)	{__HAL_RCC_GPIOA_CLK_ENABLE(); gpio_clocks[n]=1; }	break;
	case 'B':	if (gpio_clocks[n]==0)	{__HAL_RCC_GPIOB_CLK_ENABLE(); gpio_clocks[n]=1; }	break;
	case 'C':	if (gpio_clocks[n]==0)	{__HAL_RCC_GPIOC_CLK_ENABLE(); gpio_clocks[n]=1; }	break;
	case 'D':	if (gpio_clocks[n]==0)	{__HAL_RCC_GPIOD_CLK_ENABLE(); gpio_clocks[n]=1; }	break;
	case 'E':	if (gpio_clocks[n]==0)	{__HAL_RCC_GPIOE_CLK_ENABLE(); gpio_clocks[n]=1; }	break;

	#ifndef NO_PORT_F
		case 'F':	if (gpio_clocks[n]==0)	{__HAL_RCC_GPIOF_CLK_ENABLE(); gpio_clocks[n]=1; }	break;
	#endif

	case 'G':	if (gpio_clocks[n]==0)	{__HAL_RCC_GPIOG_CLK_ENABLE(); gpio_clocks[n]=1; }	break;
	case 'H':	if (gpio_clocks[n]==0)	{__HAL_RCC_GPIOH_CLK_ENABLE(); gpio_clocks[n]=1; }	break;

	default: break;
	}
}


void pin_make_output   (pin_t *p, pin_pull_t pull_up_down, pin_speed_t speed, pin_ppod_t open_drain, uint32_t val)
{
	if (p==NULL) return;
	if (p->halport==NULL) return;

	pin_init_clock(p);
	GPIO_InitTypeDef q={0};
	q.Pin=p->halno;

	switch(open_drain)
	{
		case PIN_PUSH_PULL:  q.Mode=GPIO_MODE_OUTPUT_PP; break;
		case PIN_OPEN_DRAIN: q.Mode=GPIO_MODE_OUTPUT_OD; break;
		default: return;
	}

	switch(pull_up_down)
	{
		case PIN_NO_PULL: 	q.Pull=GPIO_NOPULL; break;
		case PIN_PULL_UP: 	q.Pull=GPIO_PULLUP; break;
		case PIN_PULL_DOWN: q.Pull=GPIO_PULLDOWN; break;
		default: return;
	}

	switch(speed)
	{
		case PIN_SPEED_0: 		q.Speed=GPIO_SPEED_LOW; break;
		case PIN_SPEED_1: 		q.Speed=GPIO_SPEED_MEDIUM; break;
		case PIN_SPEED_2: 		q.Speed=GPIO_SPEED_FAST; break;
		case PIN_SPEED_3: 		q.Speed=GPIO_SPEED_HIGH;   break;
		default: return;
	}


	HAL_GPIO_Init(p->halport, &q);
	if (val==0) pin_reset(p); else pin_set(p);
}



void pin_make_alternate(pin_t *p, pin_pull_t pull_up_down, pin_speed_t speed, pin_ppod_t open_drain, uint32_t altfun)
{
	if (p==NULL) return;
	if (p->halport==NULL) return;

	pin_init_clock(p);
	GPIO_InitTypeDef q={0};
	q.Pin=p->halno;

	switch(open_drain)
		{
			case PIN_PUSH_PULL:  q.Mode=GPIO_MODE_AF_PP; break;
			case PIN_OPEN_DRAIN: q.Mode=GPIO_MODE_AF_OD; break;
			default: return;
		}

		switch(pull_up_down)
		{
			case PIN_NO_PULL: 	q.Pull=GPIO_NOPULL; break;
			case PIN_PULL_UP: 	q.Pull=GPIO_PULLUP; break;
			case PIN_PULL_DOWN: q.Pull=GPIO_PULLDOWN; break;
			default: return;
		}

		switch(speed)
		{
			case PIN_SPEED_0: 		q.Speed=GPIO_SPEED_LOW; break;
			case PIN_SPEED_1: 		q.Speed=GPIO_SPEED_MEDIUM; break;
			case PIN_SPEED_2: 		q.Speed=GPIO_SPEED_FAST; break;
			case PIN_SPEED_3: 		q.Speed=GPIO_SPEED_HIGH;   break;
			default: return;
		}


	q.Alternate=altfun;
	HAL_GPIO_Init(p->halport, &q);
}


void pin_make_input(pin_t *p, pin_pull_t pull_up_down)
{
	if (p==NULL) return;
	if (p->halport==NULL) return;

	pin_init_clock(p);
	GPIO_InitTypeDef q={0};
	q.Pin=p->halno;

	q.Mode=GPIO_MODE_INPUT;

	switch(pull_up_down)
	{
		case PIN_NO_PULL: 	q.Pull=GPIO_NOPULL; break;
		case PIN_PULL_UP: 	q.Pull=GPIO_PULLUP; break;
		case PIN_PULL_DOWN: q.Pull=GPIO_PULLDOWN; break;
		default: return;
	}

	HAL_GPIO_Init(p->halport, &q);
}


void pin_make_analog(pin_t *p)
{
	if (p==NULL) return;
	if (p->halport==NULL) return;

	pin_init_clock(p);
	GPIO_InitTypeDef q={0};
	q.Pin=p->halno;
	q.Mode=GPIO_MODE_ANALOG;
	q.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(p->halport, &q);
}

void pin_make_undef(pin_t *p)
{
	if (p==NULL) return;
	if (p->halport==NULL) return;

	pin_init_clock(p);
}










void pin_conf_to_hardware(pin_t *p)
{
	if (p==NULL) return;

	switch (p->type)
	{
	case PIN_TYPE_OUT:
		pin_make_output(p,PIN_NO_PULL, PIN_SPEED_0, PIN_PUSH_PULL,0);
		break;
	case PIN_TYPE_IN:
		pin_make_input(p,PIN_PULL_UP);
		break;

	case PIN_TYPE_SPI_NSS:
		pin_make_output(p,PIN_NO_PULL, PIN_SPEED_3, PIN_PUSH_PULL,1);//1 = slave inactive
		break;
	case PIN_TYPE_SPI_RX:
	case PIN_TYPE_SPI_TX:
		pin_make_alternate(p,PIN_PULL_DOWN,PIN_SPEED_3,PIN_PUSH_PULL,GPIO_AF5_SPI1);
		break;

	//initial configuration, reconfigured in radio_init() and radio_reset()
	case PIN_TYPE_RFM_DIO:
		pin_make_input(p,PIN_PULL_UP);
		break;
	case PIN_TYPE_RFM_RST:
		pin_make_input(p,PIN_PULL_UP);
		break;

	//enabling clock only, an actual initialization elsewhere
	case PIN_TYPE_USART_RX:
	case PIN_TYPE_USART_TX:
		pin_make_undef(p);
		break;
	default:return;
	}
}















uint8_t gpio_init(gpio_t *q)
{
	if (q==NULL) return 0;

	pin_init(&q->led_1,PIN_LED_1, PIN_TYPE_OUT);
	pin_init(&q->led_2,PIN_LED_2, PIN_TYPE_OUT);

	pin_init(&q->button_1,PIN_BUTTON_1, PIN_TYPE_IN);
	pin_init(&q->button_2,PIN_BUTTON_2, PIN_TYPE_IN);
	pin_init(&q->button_3,PIN_BUTTON_3, PIN_TYPE_IN);

	pin_init(&q->switch_1,PIN_SWITCH_1, PIN_TYPE_IN);
	pin_init(&q->switch_2,PIN_SWITCH_2, PIN_TYPE_IN);
	pin_init(&q->switch_3,PIN_SWITCH_3, PIN_TYPE_IN);

	//initialized as INPUT with PULL-UP
	pin_init(&radio.dio0, PIN_RFM_DIO0, PIN_TYPE_RFM_DIO);
	pin_init(&radio.dio1, PIN_RFM_DIO1, PIN_TYPE_RFM_DIO);
	pin_init(&radio.dio2, PIN_RFM_DIO2, PIN_TYPE_RFM_DIO);
	pin_init(&radio.dio3, PIN_RFM_DIO3, PIN_TYPE_RFM_DIO);
	pin_init(&radio.dio4, PIN_RFM_DIO4, PIN_TYPE_RFM_DIO);
	pin_init(&radio.dio5, PIN_RFM_DIO5, PIN_TYPE_RFM_DIO);
	pin_init(&radio.rst,  PIN_RFM_RST,  PIN_TYPE_RFM_RST);

	pin_init(&spi.nss,PIN_SPI_NSS, PIN_TYPE_SPI_NSS);
	pin_init(&spi.clk,PIN_SPI_CLK, PIN_TYPE_SPI_TX);
	pin_init(&spi.mosi,PIN_SPI_MOSI, PIN_TYPE_SPI_TX);
	pin_init(&spi.miso,PIN_SPI_MISO, PIN_TYPE_SPI_RX);

	//enabling clock only, an actual initialization elsewhere
	pin_init(&usart.rx,PIN_USART_RX, PIN_TYPE_USART_RX);
	pin_init(&usart.tx,PIN_USART_TX, PIN_TYPE_USART_TX);

	return 1;

}


void pin_init(pin_t *p, const char *c, pin_type_t tt)
{
	if (p==NULL) return;
	p->halno=0xFFFF;
	p->name[0]='\0';
	p->halport=NULL;
	p->port_id='\0';
	p->pin_id=0xFF;

	//validation
	if (c==NULL) return;
	if (c[0]!='P') return;
	if ((c[1]<'A')||(c[1]>'H')) return;
	if (c[2]!='_') return;
	if ((c[3]<'0')||(c[3]>'9')) return;
	if (c[4]!='\0')
	{
		if ((c[4]<'0')||(c[4]>'9')) return;
		if (c[5]!='\0') return;
	}

	if ((c[3]=='0')&&(c[4]!='\0')) return; //"PC_01" not allowed!


	uint16_t val=c[3]-'0';
	if (c[4]!='\0')	{	val*=10;	val+=c[4]-'0';	}
	if (val>15) return;

	//all ok
	p->port_id=c[1];
	p->pin_id=(uint8_t)val;

	switch (c[1])
	{
		case 'A': p->halport=GPIOA; break;
		case 'B': p->halport=GPIOB; break;
		case 'C': p->halport=GPIOC; break;
		case 'D': p->halport=GPIOD; break;
		case 'E': p->halport=GPIOE; break;
		#ifndef NO_PORT_F
			case 'F': p->halport=GPIOF; break;
		#endif
		case 'G': p->halport=GPIOG; break;
		case 'H': p->halport=GPIOH; break;
		default: return; //imposible
	}

	switch(val)
	{
		case 0:p->halno=GPIO_PIN_0; break;
		case 1:p->halno=GPIO_PIN_1; break;
		case 2:p->halno=GPIO_PIN_2; break;
		case 3:p->halno=GPIO_PIN_3; break;
		case 4:p->halno=GPIO_PIN_4; break;
		case 5:p->halno=GPIO_PIN_5; break;
		case 6:p->halno=GPIO_PIN_6; break;
		case 7:p->halno=GPIO_PIN_7; break;
		case 8:p->halno=GPIO_PIN_8; break;
		case 9:p->halno=GPIO_PIN_9; break;
		case 10:p->halno=GPIO_PIN_10; break;
		case 11:p->halno=GPIO_PIN_11; break;
		case 12:p->halno=GPIO_PIN_12; break;
		case 13:p->halno=GPIO_PIN_13; break;
		case 14:p->halno=GPIO_PIN_14; break;
		case 15:p->halno=GPIO_PIN_15; break;
		default: return; //imposible
	}

	stringcpy(p->name,c);
	p->type=tt;

	pin_init_clock(p); //do not remove
	pin_conf_to_hardware(p);
}



void pin_set(pin_t *p)
{
	if (p==NULL) return;
	if (p->halport==NULL) return;
	HAL_GPIO_WritePin(p->halport,p->halno,GPIO_PIN_SET);
}

void pin_reset(pin_t *p)
{
	if (p==NULL) return;
	if (p->halport==NULL) return;
	HAL_GPIO_WritePin(p->halport,p->halno,GPIO_PIN_RESET);
}



int32_t pin_to_exti(const pin_t *p)
{
	if (p==NULL)			return HardFault_IRQn;
	if (p->halport==NULL) 	return HardFault_IRQn;


	switch(p->pin_id )
    {
	    case  0: return EXTI0_IRQn;
	    case  1: return EXTI1_IRQn;
	    case  2: return EXTI2_IRQn;
	    case  3: return EXTI3_IRQn;
	    case  4: return EXTI4_IRQn;
	    case  5: return EXTI5_IRQn;
	    case  6: return EXTI6_IRQn;
	    case  7: return EXTI7_IRQn;
	    case  8: return EXTI8_IRQn;
	    case  9: return EXTI9_IRQn;
	    case  10: return EXTI10_IRQn;
	    case  11: return EXTI11_IRQn;
	    case  12: return EXTI12_IRQn;
	    case  13: return EXTI13_IRQn;
	    case  14: return EXTI14_IRQn;
	    case  15: return EXTI15_IRQn;
	    default: return HardFault_IRQn;
   }
}
