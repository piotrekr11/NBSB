/*
 * alt_driver.h
 *
 *  Created on: Dec 18, 2024
 *      Author: SK
 */

#ifndef INC_ALT_DRIVER_H_
#define INC_ALT_DRIVER_H_

#ifndef NULL
	#define NULL ((void*)(0))
#endif


#include "regs_fsk.h"
#include "spi.h"
#include "cout.h"

#define XTAL_FREQ                                   ((uint32_t) 32000000)
#define FREQ_STEP_STR                                "61.03515625"
#define COARSE_FREQ_STEP                             ((uint32_t) 61) //floating point maths not used
#define FREQ_STEP_NUM                                ((uint32_t) 35)
#define FREQ_STEP_DENOM                              ((uint32_t) 1000)

#define CARRIER_868000000							(14221312) 	// x FREQ_STEP
#define FDEV_25000							   		(409) 		// x FREQ_STEP
#define CHIPRATE_50000							   	(640) 		//XTAL/chiprate

#define BW_50000 									(0x0B) //predefined register value
#define BW_83333 									(0x12)

#define RX_FIFO_THRESHOLD 							(30)
#define TX_FIFO_THRESHOLD 							(30)


#define __SYNC__ ((uint8_t)(0b11001110))


typedef enum
{
	OPMODE_SLEEP=0,
	OPMODE_STDBY=1,
	OPMODE_FSTX=2,
	OPMODE_TX=3,
	OPMODE_FSRX=4,
	OPMODE_RX=5,
}opmode_t;

typedef enum transceiver_mode {MODE_SIMPLE, MODE_LORA}transceiver_mode_t;
typedef enum crc_mode {CRC_NONE, CRC_AUTO, CRC_MANUAL} crc_mode_t;
typedef enum dcfree_mode {DCFREE_NONE, DCFREE_MAN, DCFREE_WHITE, } dcfree_mode_t;



//board specific

void led1_on();
void led1_off();
void led2_on();
void led2_off();

uint8_t button1_down();
uint8_t button2_down();
uint8_t button3_down();

uint8_t button_down(const pin_t *b); 	//state detection

uint8_t button1_pressed();    			//state transition detection
uint8_t button1_released();    			//state transition detection
uint8_t button2_pressed();    			//state transition detection
uint8_t button2_released();    			//state transition detection
uint8_t button3_pressed();    			//state transition detection
uint8_t button3_released();    			//state transition detection

uint8_t dip_switch_state();				//state detection



//common for simple and lora
//carrier = coef *FREQ_STEP [Hz]
void 		set_carrier_coef(uint32_t c);
uint32_t 	get_carrier_coef();

void 				set_modem(transceiver_mode_t t); //works in the sleep mode only
transceiver_mode_t	get_modem();
void 				set_opmode(opmode_t t);

uint8_t  simple_is_ook();					//warning: returns 0 on error

//deviation = coef *FREQ_STEP [Hz]
void 	 simple_set_fdev_coef(uint32_t c);
uint32_t simple_get_fdev_coef();

///chip rate= 16 * XTAL/ coef  [chips/s]
void 	 simple_set_chiprate_coef(uint32_t c);	//no frac allowed
uint32_t simple_get_chiprate_coef();


void simple_set_lowest_tx_pow();
void simple_set_preamble_len(uint16_t len);
void simple_set_sync_word(uint8_t ifon, uint8_t size, const uint8_t *vals); //entries=0 ignored!
void simple_set_single_sync_word(uint8_t w);
void simple_set_payload_len(uint32_t length); //no error checking!
void simple_set_payload_len_ex(uint32_t length, uint8_t var); //no error checking!
void simple_set_dcfree_mode(dcfree_mode_t c);
void simple_set_crc_mode(crc_mode_t c);
void simple_set_diomap1(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
void simple_set_diomap2(uint8_t d4, uint8_t d5, uint8_t if_preamble);
void simple_set_fifo_threshold(uint8_t threshold);
void simple_set_fifo_threshold_ex(uint8_t threshold, uint8_t tx_start_cond); //FIFO threshold and Tx start condition: 0: FIFO>thre, !0: FIFO>0
void simple_clear_irqs();


//uses spi
uint8_t 	reg_read(uint8_t addr);
void 		reg_write(uint8_t addr, uint8_t val);
uint32_t 	reg_read16(uint8_t msb_addr, uint8_t lsb_addr);
void 		reg_write16(uint8_t msb_addr, uint8_t lsb_addr, uint32_t val);
void 		fifo_read(uint8_t *dest, uint8_t hm);
void 		fifo_write(const uint8_t *src, uint8_t hm);

void 	reg_set_bits(uint8_t addr, uint8_t val, uint8_t pos, uint8_t mask);
uint8_t	reg_get_bits(uint8_t addr, uint8_t pos, uint8_t mask);
void	reg_set_bit_on(uint8_t addr, uint8_t pos);
void	reg_set_bit_off(uint8_t addr, uint8_t pos);
uint8_t	reg_get_one_bit(uint8_t addr, uint8_t pos);



#endif /* INC_ALT_DRIVER_H_ */

