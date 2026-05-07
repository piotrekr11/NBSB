/*
 * gpio.h
 *
 *  Created on: Jan 29, 2025
 *      Author: SK
 */
#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "tools.h"
#include "stm32u5xx_hal.h"

#define PIN_BUTTON_1 	"PC_8"
#define PIN_BUTTON_2 	"PA_2"
#define PIN_BUTTON_3 	"PA_3"

#define PIN_LED_2 		"PA_8"
#define PIN_LED_1 		"PC_7"

#define PIN_SPI_NSS 	"PC_9"
#define PIN_SPI_MOSI    "PA_7"
#define PIN_SPI_MISO    "PA_6"
#define PIN_SPI_CLK     "PA_5"

#define PIN_SWITCH_1 	"PC_13"
#define PIN_SWITCH_2 	"PH_1"
#define PIN_SWITCH_3 	"PB_3"

#define PIN_RFM_RST 	"PA_11"
#define PIN_RFM_DIO0 	"PB_2"
#define PIN_RFM_DIO1 	"PB_1"
#define PIN_RFM_DIO2 	"PB_15"
#define PIN_RFM_DIO3 	"PB_14"
#define PIN_RFM_DIO4 	"PB_13"
#define PIN_RFM_DIO5 	"PB_8"

#define PIN_USART_RX 	"PA_10"
#define PIN_USART_TX 	"PA_9"


#define NO_PORT_F //(no F in this MCU model


typedef enum pin_type {PIN_TYPE_UNK, PIN_TYPE_OUT, PIN_TYPE_IN, PIN_TYPE_RFM_RST, PIN_TYPE_RFM_DIO, PIN_TYPE_SPI_NSS, PIN_TYPE_SPI_TX, PIN_TYPE_SPI_RX, PIN_TYPE_USART_RX, PIN_TYPE_USART_TX}pin_type_t;

typedef enum pin_pull  {PIN_NO_PULL, PIN_PULL_DOWN, PIN_PULL_UP} pin_pull_t;
typedef enum pin_speed {PIN_SPEED_0, PIN_SPEED_1, PIN_SPEED_2, PIN_SPEED_3} pin_speed_t;
typedef enum pin_ppod {PIN_OPEN_DRAIN, PIN_PUSH_PULL} pin_ppod_t;


typedef struct pin
{
	GPIO_TypeDef *halport;
	uint16_t halno;
	char name[6];
	pin_type_t type;
	char port_id;
	uint8_t pin_id;
}pin_t;


typedef struct _spi_
{
	SPI_HandleTypeDef spi;
	pin_t nss, clk, mosi, miso;
}spi_t;

typedef struct _usart_
{
	UART_HandleTypeDef usart;
	pin_t rx, tx;
}usart_t;


typedef struct _radio_
{
	pin_t rst,dio0,dio1,dio2,dio3,dio4, dio5;  //dio5 unused
}radio_t;

//add new pins to gpio_init()!
typedef struct _gpio_
{
	pin_t led_1, led_2;
	pin_t button_1, button_2, button_3; //blue not supported
	pin_t switch_1, switch_2, switch_3;
}gpio_t;





uint8_t gpio_init(gpio_t *q);
void pin_init(pin_t *p,const char *c, pin_type_t tt); //ABCDE GH ports supported (no F in this MCU model), 0-15 pins supported
void pin_set(pin_t *p);
void pin_reset(pin_t *p);
void pin_init_clock(pin_t *p);
void pin_make_output   (pin_t *p, pin_pull_t pull_up_down, pin_speed_t speed, pin_ppod_t open_drain, uint32_t val);
void pin_make_alternate(pin_t *p, pin_pull_t pull_up_down, pin_speed_t speed, pin_ppod_t open_drain, uint32_t altfun);
void pin_make_input(pin_t *p, pin_pull_t pull_up_down);
void pin_make_analog(pin_t *p);
void pin_make_undef(pin_t *p);
int32_t pin_to_exti(const pin_t *p);


#endif /* INC_GPIO_H_ */
