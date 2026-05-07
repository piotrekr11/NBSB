
#include "apploops.h"
#include "main.h" //for Error_Handler


extern volatile uint32_t timer_flag; //for test purpose only
extern volatile globals_t globals;
extern buf_t rx_buffer;;
extern spi_t spi;
extern gpio_t gpio;

static void rx_on_error( void );
static void tx_on_error( void );
static void cond_scan(void);

uint8_t ignore_button2_flag=0;


#define FSK_FIX_LENGTH  (50)


void apploops( void )
{

	uint8_t error_flag=0;
	if (spi_init(&spi)==0)   {error_flag+=1;}
	if (gpio_init(&gpio)==0) {error_flag+=2;Error_Handler();}

	print_s_lf("Let's start!");
    if (error_flag!=0)
    {
    	print_s_lf("Critical error, infinite loop entered!");
    	Error_Handler();
    }

	radio_init(); //sets modem=FSK,  carrier=868MHz, and other default parameters
	simple_set_chiprate_coef(CHIPRATE_50000);
    simple_set_fdev_coef(FDEV_25000);
    reg_write( REG_RXBW, BW_50000 );
    reg_write( REG_AFCBW, BW_83333 );

    //tx and rx settings
    error_flag=0;
    error_flag +=set_soft_tx((globals_t*)(&globals));
    error_flag +=set_soft_rx((globals_t*)(&globals));
    if (error_flag!=0){print_s_lf("Infinite loop entered");for(;;){}}

    globals.device_role=ROLE_RX; //SELECT RX/TX
     //this->print_irqs=1; //switch irq debug on

    if (button2_down()!=0)
    {
    	globals.device_role=ROLE_TX;
    	if (button3_down()!=0) globals.device_role=ROLE_INTERFERER;
    	ignore_button2_flag=1;
    }


    switch (globals.device_role)
    {
    case ROLE_RX:
    	set_hard_rx((globals_t*)(&globals)); //copy settings to hardware
    	simple_set_single_sync_word(__SYNC__);
    	globals.rx.no_sync_to = 4000; //4ms TODO: auto calculation from preamble length
    	globals.rx.no_payload_ready_to = 10000; //10ms TODO: auto calculation from payload length
    	//add additional RX config here
    	//simple_set_sync_word(1,8,NULL); //for test only
    	break;
    case ROLE_TX:
       	set_hard_tx((globals_t*)(&globals)); //copy settings to hardware
    	simple_set_lowest_tx_pow();
    	simple_set_single_sync_word(__SYNC__);
    	//add additional TX config here
    	break;
    case ROLE_INTERFERER:
    	set_hard_tx((globals_t*)(&globals)); //copy settings to hardware
    	simple_set_lowest_tx_pow();
     	//add additional INTERFERER config here
    	break;
    default: break;
    }

    timer_init();

    /*
    timer_reset_100(1000000/100);
    timer_flag=0;
    uint32_t timer_flag_old=0;
    print_s_lf("A");
    HAL_Delay(500);
    for(;;)
    {
    	if (timer_flag!=timer_flag_old)
    	{
    		if (timer_flag) led2_on();else led2_off();
    		timer_flag_old=timer_flag;
    		print_s_lf("a");
    	}
    }
    */

    switch (globals.device_role)
        {
        case ROLE_RX:
        	print_s_lf("Entering Rx mode");
    		rx_loop();
        	break;
        case ROLE_TX:
        	led1_on();
        	print_s_lf("Entering Tx mode");
        	tx_loop();
        	break;
        case ROLE_INTERFERER:
        	print_s_lf("Entering interferer mode ");
        	interferer_loop();
        	break;
        default: break;
        }


	print_s_lf("Infinite loop. This should never happen!");
	for(;;){}
}



void tx_loop(void)
{
	frame_former_t ff;
//	frame_former_init(&ff,globals.tx.payload_length,1);
	frame_former_init(&ff);

	static uint32_t tx_wait_ms;

    for(;;)
    {

    	cond_scan();
    	if ( (dip_switch_state()&2)>0) globals.channel_sim=1; else globals.channel_sim=0;

    	switch(globals.tx.state)
    	{
    	case TX_IDLE:
    	   	if ((button1_down()!=0) || ((dip_switch_state()&4)>0))
    	   	{
    	   		frame_former_ret_t ret=frame_former_next(&ff);
    	   		//100 for duty cycle of 1%, 1000 ms/s, beware of roundings
    	   		tx_wait_ms=globals_get_tx_airtime((globals_t*)(&globals))*simple_get_chiprate_coef()/16/(XTAL_FREQ/1000/100); //100 for duty cycle of 1%, 1000 ms/s
    	   		led2_on();
    	   		radio_tx(ret.buf,ret.len);
    	   	}
    	   	break;
    	case TX_READY:
    		HAL_Delay(100);
    		led2_off();
    		HAL_Delay( tx_wait_ms );
    		globals.tx.state=TX_IDLE;
    		break;
    	case TX_ERROR:
    		radio_sleep();
    		tx_on_error(); //globals.tx_err cleared internally
     		globals.tx.state=TX_READY; //to force led off and delay
    		break;
    	case TX_UPLOADING:
    		break;			//transmission in progress, do not disturb
    	default:
    		print_s_lf("! TX ERROR UNKNOWN STATE");
    		break;
    	}//switch
    }//infinite for
}




void rx_loop(void)
{
	frame_parser_t ff;
	frame_parser_init(&ff);

	for(;;)
	{

		cond_scan();

		switch(globals.rx.state)
		{
			case RX_IDLE:
				radio_rx();
				break;
			case RX_READY:
				frame_parser_parse(&ff, rx_buffer.content, rx_buffer.curr_size);
				HAL_Delay(100); //adjust Rx dead time after reception of a valid packet
				globals.rx.state=RX_IDLE;
				break;
			case RX_ERROR:
				radio_sleep();
				rx_on_error();
				globals.rx.state=RX_IDLE;
				break;
			case RX_SCANNING:   	//transmission in progress, do not disturb
			case RX_PREAMBLE:  		//transmission in progress, do not disturb
			case RX_SYNC:  			//transmission in progress, do not disturb
			case RX_DOWNLOADING:  	//transmission in progress, do not disturb
			   	break;
			default:
				print_s_lf("! RX ERROR UNKNOWN STATE");
				break;
		}
	}//infinite for

}


void interferer_loop(void)
{
	//like in TX
	simple_set_diomap1(0,2,0,0);
	simple_set_diomap2(0,3,0);
	simple_set_fifo_threshold_ex(TX_FIFO_THRESHOLD,1); //start TX when FIFO>0, ignore TX_FIFO_THRESHOLD

	uint8_t flag=0, cnt=0, cnt_max=10;
	for(;;)
	 {

		//scan
		cond_scan();

		//do not transmit interference
		if ((dip_switch_state()&4)==0) continue;

		//blinking LEDS
		if (cnt>=cnt_max)
		{
			if (flag==0) flag=1;else flag=0;
			cnt=0;
		}
		else
		{
			cnt++;
		}

		//transmit interference
		uint32_t __FOR__FAST__SLEEP__=reg_read(REG_OPMODE)&(0b11111000);
		radio_set_opmode( OPMODE_TX );
		reg_write(REG_OPMODE,__FOR__FAST__SLEEP__);
		HAL_Delay(2+prng()%23);
		if (flag==0){ led1_on(); led2_on(); } else {led1_off(); led2_off(); }
	}//infinite for

}





void rx_on_error( void )
{
	switch(globals.rx.error)
	{
	case RX_ERROR_NONE:
		print_s_lf("! RX ERROR NONE");
		break;
	case RX_ERROR_STATE:
		print_s_lf("! RX ERROR STATE");
		break;
	case RX_ERROR_PAYLOAD_SIZE:
		print_s_lf("! RX ERROR PAYLOAD SIZE");
		break;
	case RX_ERROR_TO:
		switch(globals.user_data1)
		{
			case 1: print_s_lf("! RX ERROR TO: NO SYNC WORD");break;
			case 2: print_s_lf("! RX ERROR TO: PAYLOAD");break;
			default: print_s_lf("! RX ERROR TO");break;
		}
		break;
	default:
		print_s_lf("! RX ERROR UNKNOWN");
		break;
	}
	globals.rx.error=RX_ERROR_NONE;
}


void tx_on_error( void )
{
	switch(globals.tx.error)
	{
	case TX_ERROR_NONE:
		print_s_lf("! TX ERROR NONE");
		break;
	case TX_ERROR_PAYLOAD_SIZE:
		print_s_lf("! TX ERROR PAYLOAD SIZE");
		break;
	default:
		print_s_lf("! TX ERROR UNKNOWN");
		break;
	}
	globals.tx.error=TX_ERROR_NONE;
}


uint8_t set_soft_tx(globals_t *g)
{

	payload_length_t plen;

    uint8_t error_flag0=0;
    if (payload_length_set(&plen,FSK_FIX_LENGTH,0)!=0)
    {
    	print_s_lf("Incorrect TX payload length");
    	error_flag0=1;
    }

    uint8_t error_flag=1;
    //preamble_len, sync_len, payload_len, dcfree_mode. crc_mode
    switch (globals_tx(g, 5, 1, plen, DCFREE_WHITE, CRC_NONE))
	{
	   	case G_ERR_NONE: 		error_flag=0; break;
	   	case G_ERR_NULL: 		print_s_lf("Incorrect TX config: NULL ");break;
	   	case G_ERR_SUPP: 		print_s_lf("Incorrect TX config: option not supported ");break;
	   	case G_ERR_PREAMBLE: 	print_s_lf("Incorrect TX preamble length");break;
	   	case G_ERR_SYNC: 		print_s_lf("Incorrect TX sync word length");break;
	   	case G_ERR_ADDR: 		print_s_lf("Incorrect TX node address");break;
	   	case G_ERR_PAYLOAD: 	print_s_lf("Incorrect TX payload length");break;
	   	default: 				print_s_lf("Incorrect TX config, unknown error");break;
	}

    return error_flag+error_flag0;
}


uint8_t set_soft_rx(globals_t *g)
{
	payload_length_t plen;

	uint8_t error_flag0=0;
	if (payload_length_set(&plen,FSK_FIX_LENGTH,0)!=0)
	{
	  	print_s_lf("Incorrect RX payload length");
	   	error_flag0=1;
	}

	uint8_t error_flag=1;
	//preamble_len, sync_len, payload_len, dcfree_mode. crc_mode
    switch (globals_rx(g, 5, 1, plen, DCFREE_WHITE, CRC_NONE))
	{
	    	case G_ERR_NONE: 		error_flag=0; break;
	    	case G_ERR_NULL: 		print_s_lf("Incorrect RX config: NULL ");break;
	    	case G_ERR_SUPP: 		print_s_lf("Incorrect RX config: option not supported ");break;
	    	case G_ERR_PREAMBLE: 	print_s_lf("Incorrect RX preamble length");break;
	    	case G_ERR_SYNC: 		print_s_lf("Incorrect RX sync word length");break;
	    	case G_ERR_ADDR: 		print_s_lf("Incorrect RX node address");break;
	    	case G_ERR_PAYLOAD: 	print_s_lf("Incorrect RX payload length");break;
	    	default: 				print_s_lf("Incorrect RX config, unknown error");break;
	 }

    return error_flag+error_flag0;
}


void set_hard_payload_length(payload_length_t p)
{
	uint32_t L=p.len;
	if (p.var>0) L=VAR_PAYLOAD_LENGTH_REG;
	simple_set_payload_len_ex(L,p.var);
}


void set_hard_tx(const globals_t *g)
{
	if (g==NULL) return;
	simple_set_preamble_len(g->tx.preamble_length);
	simple_set_sync_word(1,g->tx.sync_length,NULL);
	set_hard_payload_length(g->tx.payload_length);
	simple_set_dcfree_mode(g->tx.dcfree_mode);
	simple_set_crc_mode(g->tx.crc_mode);
}

void set_hard_rx(const globals_t *g)
{
	if (g==NULL) return;
	simple_set_preamble_len(g->rx.preamble_length);
	simple_set_sync_word(1,g->rx.sync_length,NULL);
	set_hard_payload_length(g->rx.payload_length);
	simple_set_dcfree_mode(g->rx.dcfree_mode);
	simple_set_crc_mode(g->rx.crc_mode);
}



void cond_scan(void)
{
	if (button2_released()!=0)
	{
		if	(ignore_button2_flag!=0)
		{
			ignore_button2_flag=0;
		}
		else
		{
			scan();
		}
	}
}
