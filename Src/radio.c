/*
 * radio.c
 *
 *  Created on: Jan 31, 2025
 *      Author: SK
 */
#include "radio.h"


uint8_t __FOR__FAST__SLEEP__;
buf_t tx_buffer, rx_buffer;
extern volatile globals_t globals;
volatile uint32_t timer_flag;
extern radio_t radio;
extern volatile uint32_t PRNS_SEED;



void radio_init(void)
{
	radio_reset( );
    radio_rx_cal();
    radio_set_opmode( OPMODE_SLEEP );

    irq_map(&radio.dio0,0);
    irq_map(&radio.dio1,1);
    irq_map(&radio.dio2,2);
    irq_map(&radio.dio3,3);
    irq_map(&radio.dio4,4);
    //dio5 unused

    radio_init_registers();
    radio_set_modem(MODE_SIMPLE);
    set_carrier_coef(CARRIER_868000000);
}









void radio_tx( const uint8_t *buffer, uint32_t size)
{

  payload_length_t ss=globals.tx.payload_length;
  //TODO: long packets support


  if ((buffer==NULL)||(ss.len<MIN_PAYLOAD_LENGTH)||(ss.len!=size) ||  ((ss.var>0)&&(size>MAX_PAYLOAD_LENGTH_VAR)) )
  {
	  radio_sleep();
	  globals.tx.state=TX_ERROR;
	  globals.tx.error=TX_ERROR_PAYLOAD_SIZE;
	  return;
  }

  if (ss.var>0)
  {
	  buf_init(&tx_buffer, size+1);
	  tx_buffer.content[0]=size;
	  tx_buffer.curr_size=1;
  }
  else
  {
	  buf_init(&tx_buffer, size);
  }

  buf_ret_t ret=buf_append_segment(&tx_buffer, size);
  fastmemcpy(ret.ptr,buffer,ret.howmany);
  buf_confirm(&tx_buffer);

  // DIO0=PacketSent
  // DIO1=FifoFull
  // DIO2=FifoFull
  // DIO3=FifoEmpty
  // DIO4=Temp/LowBat
  // DIO5=ModeReady

  simple_set_diomap1(0,2,0,0);
  simple_set_diomap2(0,3,0);


  if (globals.channel_sim>0)   radio_channel_sim(tx_buffer.content, tx_buffer.curr_size);

  simple_set_fifo_threshold_ex(TX_FIFO_THRESHOLD,1); //start TX when FIFO>0, ignore TX_FIFO_THRESHOLD

  fifo_write( tx_buffer.content, (uint8_t)size ); //TODO: long packets support

  __FOR__FAST__SLEEP__=reg_read(REG_OPMODE)&(0b11111000);
  globals.tx.state=TX_UPLOADING;
  radio_set_opmode( OPMODE_TX );
}






void radio_rx( void )
{

	payload_length_t ss=globals.tx.payload_length;

	// DIO0=PayloadReady
    // DIO1=FifoLevel
    // DIO2=SyncAddr
    // DIO3=FifoEmpty
    // DIO4=Preamble/ not RSSI
    // DIO5=ModeReady

     simple_set_diomap1(0,0,3,0);
     simple_set_diomap2(3,3,1);

     //TimerSetValue( &RxTimeoutTimer, timeout );
     //TimerStart( &RxTimeoutTimer );

     simple_set_fifo_threshold(RX_FIFO_THRESHOLD);
     if (ss.var==0) simple_set_payload_len_ex(ss.len,0); else simple_set_payload_len_ex(VAR_PAYLOAD_LENGTH_REG,1); //TODO: CRC MANUAL MODE

     buf_init(&rx_buffer,ss.len); //TODO: CRC MANUAL MODE, in variable length mode buffer will be re-inited after the first byte is received

     __FOR__FAST__SLEEP__=reg_read(REG_OPMODE)&(0b11111000);
     globals.rx.state=RX_SCANNING;
     radio_set_opmode( OPMODE_RX );

}




static void rx_preamble_detected_fsk()
{
		//reg_write(REG_IRQFLAGS1,2); //SK bug fix: clear PreambleReady Flag

		if (globals.rx.state==RX_SCANNING)
		{
			timer_reset_100(globals.rx.no_sync_to/100);
			globals.rx.state=RX_PREAMBLE; //new preamble detected only if my_buffer is empty!
		}
}


static void rx_syncword_detected_fsk()
{
	 if (globals.rx.state==RX_PREAMBLE)
     {
		 timer_reset_100(globals.rx.no_payload_ready_to/100);
		 globals.rx.state=RX_SYNC;
    }
}


static void rx_read_first_segment(payload_length_t ss)
{
	uint8_t first_byte;
	fifo_read(&first_byte,1);

	if (ss.var!=0) //variable payload length mode
	{
		if (first_byte<MIN_PAYLOAD_LENGTH)
		{
			radio_sleep();
			globals.rx.state=RX_ERROR;
			globals.rx.error=RX_ERROR_PAYLOAD_SIZE;
			return;
		};
		buf_init(&rx_buffer,first_byte+1); //TODO: CRC MANUAL MODE
	}

	rx_buffer.content[0]=first_byte;
	rx_buffer.curr_size=1;

	buf_ret_t ret=buf_append_segment(&rx_buffer, RX_FIFO_THRESHOLD);
	fifo_read(ret.ptr, (uint8_t)ret.howmany);
	buf_confirm(&rx_buffer);
}


static void rx_read_next_segment(payload_length_t ss)
{
	(void)(ss);
	buf_ret_t ret=buf_append_segment(&rx_buffer, RX_FIFO_THRESHOLD+1);
	fifo_read(ret.ptr, (uint8_t)ret.howmany);
	buf_confirm(&rx_buffer);
}




static void rx_fifo_threshold_fsk()
{

    payload_length_t ss=globals.rx.payload_length;

    switch(globals.rx.state)
    {
    	case RX_SYNC:
    		rx_read_first_segment(ss);
    		globals.rx.state=RX_DOWNLOADING;
    		break;
    	case RX_DOWNLOADING:
    		rx_read_next_segment(ss);
    		break;
    	default:
    		radio_sleep();
    		globals.rx.state=RX_ERROR;
    		globals.rx.error=RX_ERROR_STATE;
    		return;
    }
}


static void rx_done_fsk()
{
	payload_length_t ss=globals.rx.payload_length;

	timer_reset_100(0); //stop the timer

	switch(globals.rx.state)
	{
	  	case RX_SYNC:
	   		rx_read_first_segment(ss);
	   		globals.rx.state=RX_DOWNLOADING;
	   		break;
	   	case RX_DOWNLOADING:
	   		rx_read_next_segment(ss);
	   		break;
	   	default:
	   		radio_sleep();
	   		globals.rx.state=RX_ERROR;
	   		globals.rx.error=RX_ERROR_STATE;
	   		return;
	 }

	reg_write(REG_OPMODE,__FOR__FAST__SLEEP__);
	globals.rx.state=RX_READY;

}



//static void tx_fifo_empty_fsk(){}



static void tx_done_fsk()
{
	 radio_sleep();
	 globals.tx.state=TX_READY;
}





void SKTIM_IRQ_HANDLER(void)
{
	U32_SET_BIT_OFF( SKTIM->SR, TIM_SR_UIF_Pos);			//clear pending IRG flag
	radio_sleep(); //also stops the timer
	if (timer_flag==0) timer_flag=1;else timer_flag=0; //for test purpose only
	switch(globals.rx.state)
	{
		case RX_PREAMBLE: globals.user_data1=1;break;
		case RX_SYNC:
		case RX_DOWNLOADING: globals.user_data1=2;break;
		default: globals.user_data1=0;break;
	}

	globals.rx.state=RX_ERROR;
	globals.rx.error=RX_ERROR_TO;

}



void dio0_handler( void ) //Payload ready (RX) / Packet sent (TX)
{

	//a patch for forcing Tx sleep as fast as possible
	if ((globals.device_role==ROLE_TX)&&(globals.tmode==MODE_SIMPLE))
	{
		reg_write(REG_OPMODE,__FOR__FAST__SLEEP__);
	}


    if (globals.print_irqs>0) print_s_lf("D0");


	switch(globals.device_role)
	{
		case ROLE_RX:
			switch(globals.tmode)
			{
				case MODE_SIMPLE:  rx_done_fsk();  break;
				case MODE_LORA: break;
				default: break;
			}
			break;
		case ROLE_TX:
			switch(globals.tmode)
			{
				case MODE_SIMPLE: tx_done_fsk();break; //see a patch above
				case MODE_LORA: break;
				default: break;
			}
			break;
		case ROLE_INTERFERER:	break;
		default: break;
	}

}




void dio1_handler( void ) //FIFO level (RX) / FIFO empty (TX)
{

	if (globals.print_irqs>0) print_s_lf("D1");

	switch(globals.device_role)
	{
		case ROLE_RX:
			switch(globals.tmode)
			{
				case MODE_SIMPLE:  rx_fifo_threshold_fsk(); break;
				case MODE_LORA: break;
				default: break;
			}
			break;
		case ROLE_TX:
			switch(globals.tmode)
			{
				case MODE_SIMPLE: break;
				case MODE_LORA: break;
				default: break;
			}
			break;
		case ROLE_INTERFERER:	break;
		default: break;
	}

}




void dio2_handler( void ) //sync word detected (RX)
{
	if (globals.print_irqs>0) print_s_lf("D2");

	switch(globals.device_role)
	{
		case ROLE_RX:
			switch(globals.tmode)
			{
				case MODE_SIMPLE:  rx_syncword_detected_fsk(); break;
				case MODE_LORA: break;
				default: break;
			}
			break;
		case ROLE_TX:
			switch(globals.tmode)
			{
				case MODE_SIMPLE: break;
				case MODE_LORA: break;
				default: break;
			}
			break;
		case ROLE_INTERFERER:	break;
		default: break;
	}
}



void dio3_handler( void ) //FIFO empty (TX)
{
	if (globals.print_irqs>0) print_s_lf("D3");
}




void dio4_handler( void )  //Preamble detected (RX)
{
	if (globals.print_irqs>0) print_s_lf("D4");

	switch(globals.device_role)
	{
			case ROLE_RX:
				switch(globals.tmode)
				{
					case MODE_SIMPLE:  rx_preamble_detected_fsk();	break;
					case MODE_LORA: break;
					default: break;
				}
				break;
			case ROLE_TX:
				switch(globals.tmode)
				{
					case MODE_SIMPLE: break;
					case MODE_LORA: break;
					default: break;
				}
				break;
			case ROLE_INTERFERER:	break;
			default: break;
	}
}




//auxiliary functions
void radio_channel_sim(uint8_t*dest,uint32_t size)
{
	sim_apply(dest,size);
}



void radio_rx_cal( void )
{

    // Save register values
    uint8_t regPaConfigInitVal = reg_read( REG_PACONFIG );
    uint32_t initialFreq = get_carrier_coef();

    // Switch the PA off, low power
    reg_write( REG_PACONFIG, 0x00 );

    // Launch Rx chain calibration for LF band
    reg_write( REG_IMAGECAL, ( reg_read( REG_IMAGECAL ) & RF_IMAGECAL_IMAGECAL_MASK ) | RF_IMAGECAL_IMAGECAL_START );
    while( ( reg_read( REG_IMAGECAL ) & RF_IMAGECAL_IMAGECAL_RUNNING ) == RF_IMAGECAL_IMAGECAL_RUNNING )
    {
    }

    // Sets freq
    set_carrier_coef( CARRIER_868000000 );

    // run Rx  calibration in the HF band
    reg_write( REG_IMAGECAL, ( reg_read( REG_IMAGECAL ) & RF_IMAGECAL_IMAGECAL_MASK ) | RF_IMAGECAL_IMAGECAL_START );
    while( ( reg_read( REG_IMAGECAL ) & RF_IMAGECAL_IMAGECAL_RUNNING ) == RF_IMAGECAL_IMAGECAL_RUNNING )
    {
    }

    //Get prng seed
    radio_set_opmode(OPMODE_RX);
    HAL_Delay(7);
    PRNS_SEED=reg_read(REG_RSSIVALUE)+reg_read(REG_TEMP);
    radio_sleep();
    simple_clear_irqs();

    // Restore register values
    reg_write( REG_PACONFIG, regPaConfigInitVal );
    set_carrier_coef( initialFreq );
}


void radio_init_registers()
{
    //radio_set_modem( MODE_LORA );
    //reg_write(REG_LR_DETECTOPTIMIZE  , 0x43);
    //reg_write(REG_LR_PAYLOADMAXLENGTH, 0x40);

    radio_set_modem(MODE_SIMPLE);
    reg_write( REG_LNA                , 0x23 );
	reg_write( REG_RXCONFIG           , 0x1E );
	reg_write( REG_RSSICONFIG         , 0xD2 );
	reg_write( REG_AFCFEI             , 0x01 );
	reg_write( REG_PREAMBLEDETECT     , 0xAA );
	reg_write( REG_OSC                , 0x07 ); //no signal on CLKOUT
	reg_write( REG_SYNCCONFIG         , 0x12 );
	reg_write( REG_SYNCVALUE1         , 0xC1 );
	reg_write( REG_SYNCVALUE2         , 0x94 );
	reg_write( REG_SYNCVALUE3         , 0xC1 );
	reg_write( REG_PACKETCONFIG1      , 0xD8 );
	reg_write( REG_FIFOTHRESH         , 0x8F );
	reg_write( REG_IMAGECAL           , 0x02 );
	reg_write( REG_DIOMAPPING1        , 0x00 );
	reg_write( REG_DIOMAPPING2        , 0x30 );
}


void radio_sleep(void)
{
	timer_reset_100(0);
    radio_set_opmode(OPMODE_SLEEP );
}


void radio_stby( void )
{
	timer_reset_100(0);
    radio_set_opmode( OPMODE_STDBY );
}


void radio_reset( void )
{
	timer_reset_100(0);

	// Set rst pin to 0
    pin_make_output(&radio.rst,GPIO_NOPULL,GPIO_SPEED_MEDIUM, 0, 0);

    HAL_Delay( 2 );

    // Configure rst as input
    pin_make_input(&radio.rst,GPIO_PULLUP);

    HAL_Delay( 6 );
}


void radio_set_opmode(opmode_t opmode)
{
     set_opmode(opmode);
}


void radio_set_modem(transceiver_mode_t t )
{

	radio_set_opmode( OPMODE_SLEEP );
	set_modem(t);

}
