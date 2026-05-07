/*
 * alt_driver.h
 *
 *  Created on: Dec 18, 2024
 *      Author: SK
 */

#include "alt_driver.h"


extern gpio_t gpio;
extern spi_t spi;


void led1_on()  {pin_set(&gpio.led_1);}
void led1_off() {pin_reset(&gpio.led_1);}
void led2_on()  {pin_set(&gpio.led_2);}
void led2_off() {pin_reset(&gpio.led_2);}

uint8_t button1_down(){return button_down(&gpio.button_1);}
uint8_t button2_down(){return button_down(&gpio.button_2);}
uint8_t button3_down(){return button_down(&gpio.button_3);}
uint8_t dip_switch_state(){	return button_down(&gpio.switch_1) | (button_down(&gpio.switch_2)<<1) |(button_down(&gpio.switch_3)<<2);}

uint8_t button1_pressed()
{
	static uint8_t state=0;
	uint8_t new_state=state;

	if (button_down(&gpio.button_1)!=0) new_state=1; else new_state=0;

	if ((state==0)&&(new_state==1)) {state=new_state;return 1;}

	state=new_state;
	return 0;

}

uint8_t button1_released()
{
	static uint8_t state=0;
	uint8_t new_state=state;

	if (button_down(&gpio.button_1)!=0) {new_state=1; } else new_state=0;

	if ((state==1)&&(new_state==0)) {state=new_state;return 1;}

	state=new_state;
	return 0;
}

uint8_t button2_pressed()
{
	static uint8_t state=0;
	uint8_t new_state=state;

	if (button_down(&gpio.button_2)!=0) new_state=1; else new_state=0;

	if ((state==0)&&(new_state==1)) {state=new_state;return 1;}

	state=new_state;
	return 0;

}

uint8_t button2_released()
{
	static uint8_t state=0;
	uint8_t new_state=state;

	if (button_down(&gpio.button_2)!=0) {new_state=1; } else new_state=0;

	if ((state==1)&&(new_state==0)) {state=new_state;return 1;}

	state=new_state;
	return 0;
}

uint8_t button3_pressed()
{
	static uint8_t state=0;
	uint8_t new_state=state;

	if (button_down(&gpio.button_3)!=0) new_state=1; else new_state=0;

	if ((state==0)&&(new_state==1)) {state=new_state;return 1;}

	state=new_state;
	return 0;

}

uint8_t button3_released()
{
	static uint8_t state=0;
	uint8_t new_state=state;

	if (button_down(&gpio.button_3)!=0) {new_state=1; } else new_state=0;

	if ((state==1)&&(new_state==0)) {state=new_state;return 1;}

	state=new_state;
	return 0;
}

uint8_t button_down(const pin_t *b)
{
	if (b==NULL) return 0;
	if (b->halport==NULL) return 0;
	if (HAL_GPIO_ReadPin(b->halport, b->halno) == GPIO_PIN_RESET) return 1; else return 0;
}



void set_carrier_coef(uint32_t c)
{
	reg_write(REG_FRFMSB,(uint8_t)((c&0x00FF0000)>>16));
	reg_write(REG_FRFMID,(uint8_t)((c&0x0000FF00)>>8));
	reg_write(REG_FRFLSB,(uint8_t)((c&0x000000FF)));
}

uint32_t get_carrier_coef()
{
	uint32_t qqq =reg_read(REG_FRFMSB);
	uint32_t qq  =reg_read(REG_FRFMID);
	uint32_t q   =reg_read(REG_FRFLSB);

	 return (qqq<<16)|(qq<<8)|q;
}

void set_modem(transceiver_mode_t t)
{
	switch(t)
	{
		case MODE_SIMPLE: reg_set_bit_off(REG_OPMODE,REG_OPMODE_LONGRANGEMODE_Pos); break;
		case MODE_LORA: reg_set_bit_on(REG_OPMODE,REG_OPMODE_LONGRANGEMODE_Pos); break;
		default: break;
	}
}

transceiver_mode_t	get_modem()
{
	uint8_t q=reg_get_one_bit(REG_OPMODE,REG_OPMODE_LONGRANGEMODE_Pos);
	if (q==0) return MODE_SIMPLE; else return MODE_LORA;
}


void set_opmode(opmode_t t)
{
	reg_set_bits(REG_OPMODE,(uint8_t)t,REG_OPMODE_MODE_Pos,REG_OPMODE_MODE_Mask );
}


void simple_set_fdev_coef(uint32_t c)
{
	reg_write16(REG_FDEVMSB,REG_FDEVLSB,c&0x3FFF); //14 bits
}

uint32_t simple_get_fdev_coef()
{
	return reg_read16(REG_FDEVMSB,REG_FDEVLSB)&0x3FFF; //14 bits
}


uint8_t  simple_is_ook() //warning: returns 0 in return
{
	 if (reg_get_bits(REG_OPMODE,REG_OPMODE_MODULATIONTYPE_Pos,REG_OPMODE_MODULATIONTYPE_Mask)==1) return 1; else return 0;
}


void 	 simple_set_chiprate_coef(uint32_t c) //XTAL/chiprate
{
	reg_write16(REG_BITRATEMSB,REG_BITRATELSB,c);
	//reg_write  (REG_BITRATEFRAC,(uint8_t)(c&0x0F));
	reg_write  (REG_BITRATEFRAC,0);
}


uint32_t simple_get_chiprate_coef()  //chip rate= 16*XTAL/ returned_value
{

	uint8_t frac=0;
	if (simple_is_ook()==0)  frac=reg_read(REG_BITRATEFRAC)&0x0F;
    return 16*reg_read16(REG_BITRATEMSB,REG_BITRATELSB)+frac;

}

void simple_set_diomap1(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	d0=(d0&3)<<6;
	d1=(d1&3)<<4;
	d2=(d2&3)<<2;
	d3=(d3&3);
	reg_write(REG_DIOMAPPING1,d0|d1|d2|d3);
}
void simple_set_diomap2(uint8_t d4, uint8_t d5, uint8_t if_preamble)
{
	d4=(d4&3)<<6;
	d5=(d5&3)<<4;

	uint8_t reg=d4 | d5;
	if (if_preamble!=0) reg|=1;
	reg_write(REG_DIOMAPPING2,reg);
}





void simple_set_lowest_tx_pow()
{
	reg_write(REG_PACONFIG,128);
	reg_write(REG_PADAC,0b10000100);
}


void simple_set_preamble_len(uint16_t len)
{
   reg_write( REG_PREAMBLEMSB, ( len >> 8 ) & 0x00FF );
   reg_write( REG_PREAMBLELSB, len & 0x00FF );
}


void simple_set_sync_word(uint8_t ifon, uint8_t size, const uint8_t *vals)
{
	uint8_t byte=reg_read(REG_SYNCCONFIG);
	if (ifon==0)
		U8_SET_BIT_OFF(byte,REG_SYNCCONFIG_SYNCON_Pos)
	else
		U8_SET_BIT_ON(byte,REG_SYNCCONFIG_SYNCON_Pos)

	if ((size>=1)&&(size<=8))
	{
		for(;;)
		{
			if (vals==NULL) break;

			if (vals[0]!=0) reg_write(REG_SYNCVALUE1,vals[0]);
			if (size==1) break;

			if (vals[1]!=0) reg_write(REG_SYNCVALUE2,vals[1]);
			if (size==2) break;

			if (vals[2]!=0) reg_write(REG_SYNCVALUE3,vals[2]);
			if (size==3) break;

			if (vals[3]!=0) reg_write(REG_SYNCVALUE4,vals[3]);
			if (size==4) break;

			if (vals[4]!=0) reg_write(REG_SYNCVALUE5,vals[4]);
			if (size==5) break;

			if (vals[5]!=0) reg_write(REG_SYNCVALUE6,vals[5]);
			if (size==6) break;

			if (vals[6]!=0) reg_write(REG_SYNCVALUE7,vals[6]);
			if (size==7) break;

			if (vals[7]!=0) reg_write(REG_SYNCVALUE8,vals[7]);
			break;
		}

		U8_SET_BITS(byte,size-1,REG_SYNCCONFIG_SYNCSIZE_Pos,REG_SYNCCONFIG_SYNCSIZE_Mask)
	}

	reg_write(REG_SYNCCONFIG,byte);
}


void simple_set_single_sync_word(uint8_t w)
{
	simple_set_sync_word(1,1,&w);
}


void simple_set_payload_len(uint32_t length)
{

	uint32_t l_msb = (length>>8) & 0x07;
	uint32_t l_lsb = length & 0xFF;

	reg_set_bits(REG_PACKETCONFIG2,(uint8_t)l_msb,REG_PACKETCONFIG2_PAYLOADLENGTHMSB_Pos,REG_PACKETCONFIG2_PAYLOADLENGTHMSB_Mask);
	reg_write(REG_PAYLOADLENGTH,(uint8_t)l_lsb);
}



void simple_set_payload_len_ex(uint32_t length, uint8_t var )
{

	if (var==0)
	{
		//fixed
		reg_set_bit_off(REG_PACKETCONFIG1,REG_PACKETCONFIG1_PACKETFORMAT_Pos);
	}
	else
	{
		//variable
		reg_set_bit_on(REG_PACKETCONFIG1,REG_PACKETCONFIG1_PACKETFORMAT_Pos);
	}

	simple_set_payload_len(length);

}


void simple_set_dcfree_mode(dcfree_mode_t c)
{
	switch(c)
	{
		case DCFREE_NONE: 	reg_set_bits(REG_PACKETCONFIG1,0,REG_PACKETCONFIG1_DCFREE_Pos,REG_PACKETCONFIG1_DCFREE_Mask);break;
		case DCFREE_MAN: 	reg_set_bits(REG_PACKETCONFIG1,1,REG_PACKETCONFIG1_DCFREE_Pos,REG_PACKETCONFIG1_DCFREE_Mask);break;
		case DCFREE_WHITE: 	reg_set_bits(REG_PACKETCONFIG1,2,REG_PACKETCONFIG1_DCFREE_Pos,REG_PACKETCONFIG1_DCFREE_Mask);break;
		default: return; //invalid input
	}
}


void simple_set_crc_mode(crc_mode_t c)
{
	uint8_t mask=BB2<<REG_PACKETCONFIG1_CRCAUTOCLEAROFF_Pos;
	switch(c)
	{
		case CRC_NONE: 		reg_set_bits(REG_PACKETCONFIG1,0,REG_PACKETCONFIG1_CRCAUTOCLEAROFF_Pos,mask);break; //3=CRCAUTOCLEAROFF, 4=CRCON
		case CRC_AUTO: 		reg_set_bits(REG_PACKETCONFIG1,2,REG_PACKETCONFIG1_CRCAUTOCLEAROFF_Pos,mask);break;
		case CRC_MANUAL: 	reg_set_bits(REG_PACKETCONFIG1,3,REG_PACKETCONFIG1_CRCAUTOCLEAROFF_Pos,mask);break;
		default: return; //invalid input
	}
}


void simple_set_fifo_threshold(uint8_t threshold)
{
	uint8_t reg = reg_read(REG_FIFOTHRESH);
	reg &= ~(0x3F);
	reg|= threshold & 0x3F;
	reg_write(REG_FIFOTHRESH,reg);
}


void simple_set_fifo_threshold_ex(uint8_t threshold, uint8_t tx_start_cond)
{
	uint8_t reg = reg_read(REG_FIFOTHRESH);
	reg &= ~(0x3F);
	reg|= threshold & 0x3F;

	if (tx_start_cond==0) reg &= 0x7F; else reg |= 0x80;

	reg_write(REG_FIFOTHRESH,reg);
}


void simple_clear_irqs()
{
	reg_write(REG_IRQFLAGS1,REG_IRQFLAGS1_RSSI_Mask | REG_IRQFLAGS1_PREAMBLEDETECT_Mask);
	reg_write(REG_IRQFLAGS2,REG_IRQFLAGS2_FIFOOVERRUN_Mask);
}






void fifo_read(uint8_t *dest, uint8_t hm)
{
	uint8_t addr=REG_FIFO&0x7f;

	//NSS = 0;
	pin_reset(&(spi.nss));

	HAL_SPI_Transmit( &(spi.spi), &addr, 1, HAL_MAX_DELAY);
	while(HAL_SPI_GetState(&(spi.spi)) != HAL_SPI_STATE_READY);
	HAL_SPI_Receive( &(spi.spi), dest, hm, HAL_MAX_DELAY);
	while(HAL_SPI_GetState(&(spi.spi)) != HAL_SPI_STATE_READY);

	//NSS = 1;
	pin_set(&(spi.nss));
}

void fifo_write(const uint8_t *src, uint8_t hm)
{

	uint8_t addr=REG_FIFO|0x80;

	//NSS = 0;
	pin_reset(&(spi.nss));

	HAL_SPI_Transmit( &(spi.spi), &addr, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit( &(spi.spi), src, hm, HAL_MAX_DELAY);

	//NSS = 1;
	pin_set(&(spi.nss));
}


uint8_t reg_read(uint8_t addr)
{
	    uint8_t buf=0;

	    //NSS = 0;
	    pin_reset(&(spi.nss));

	    addr &= 0x7f; //TODO: why?
	    HAL_SPI_Transmit( &(spi.spi), &addr, 1, HAL_MAX_DELAY);
	    while(HAL_SPI_GetState(&(spi.spi)) != HAL_SPI_STATE_READY);
	    HAL_SPI_Receive( &(spi.spi), &buf, 1, HAL_MAX_DELAY);
	    while(HAL_SPI_GetState(&(spi.spi)) != HAL_SPI_STATE_READY);

	    //NSS = 1;
	    pin_set(&(spi.nss));

	    return buf;
}


void reg_write(uint8_t addr, uint8_t val)
{
	//NSS = 0;
	pin_reset(&(spi.nss));

	addr|= 0x80; //TODO: why
	HAL_SPI_Transmit( &(spi.spi), &addr, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit( &(spi.spi), &val,  1, HAL_MAX_DELAY);

	//NSS = 1;
	pin_set(&(spi.nss));
}


uint32_t reg_read16(uint8_t msb_addr, uint8_t lsb_addr)
{
	uint32_t qq  =reg_read(msb_addr);
	uint32_t q  =reg_read(lsb_addr);

	return (qq<<8)|q;
}

void 	reg_write16(uint8_t msb_addr, uint8_t lsb_addr, uint32_t val)
{
	reg_write(msb_addr,(uint8_t)((val&0x0000FF00)>>8));
	reg_write(lsb_addr,(uint8_t)((val&0x000000FF)));
}


void 	reg_set_bits(uint8_t addr, uint8_t val, uint8_t pos, uint8_t mask)
{
	uint8_t reg=reg_read(addr);
	U8_SET_BITS(reg,val,pos,mask);
	reg_write(addr,reg);
}

uint8_t	reg_get_bits(uint8_t addr, uint8_t pos, uint8_t mask)
{
	uint8_t reg=reg_read(addr);
	return U8_GET_BITS(reg,pos,mask);
}

void	reg_set_bit_on(uint8_t addr, uint8_t pos)
{
	uint8_t reg=reg_read(addr);
	U8_SET_BIT_ON(reg,pos);
	reg_write(addr,reg);
}


void	reg_set_bit_off(uint8_t addr, uint8_t pos)
{
	uint8_t reg=reg_read(addr);
	U8_SET_BIT_OFF(reg,pos);
	reg_write(addr,reg);
}


uint8_t reg_get_one_bit(uint8_t addr, uint8_t pos)
{
	uint8_t reg=reg_read(addr);
	return U8_GET_ONE_BIT(reg,pos);

}
