/*
 * scan.c
 *
 *  Created on: Dec 19, 2024
 *      Author: SK
 */

#include "scan.h"

extern volatile globals_t globals;
extern volatile uint32_t PRNS_SEED;


static void read_and_print_reg(const char *txt, uint8_t addr, const char *format)
{
	uint8_t val=reg_read(addr);
	print_reg8_lf(txt,val,format);
}




void scan(void)
{
	print_lf();
	print_lf();
	print_s_lf("SCAN:");

	switch(get_modem())
	{
	case MODE_LORA:
		print_s_lf("Modem: Lora");
		break;
	case MODE_SIMPLE:
		print_s_lf("Modem: FSK/OOK");
		simple_mod_debug("MODULATION: ");
		print_lf();
		simple_regs_debug("REGISTERS:");
		print_lf();
		simple_packet_config_debug("PACKET SETTINGS:");
		print_lf();
		simple_irqf_debug("IRQ FLAGS: ");
		print_lf();
		simple_freq_debug("FREQ:");
		print_lf();
		app_state_debug("APP STATE:");
		print_lf();
		simple_misc_debug("MISC:");
		print_lf();
		simple_to_debug("TIMEOUTS:");
		print_lf();
		break;
	default:break;

	}

//	  RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
//
//	  CRC->CR = CRC_CR_RESET;
//
//	  ((volatile uint8_t*)(&(CRC->DR)))[0] = '1';
//	  ((volatile uint8_t*)(&(CRC->DR)))[0] = '2';
//	  ((volatile uint8_t*)(&(CRC->DR)))[0] = '3';
//	  ((volatile uint8_t*)(&(CRC->DR)))[0] = '4';
//	  ((volatile uint8_t*)(&(CRC->DR)))[0] = '5';
//	  ((volatile uint8_t*)(&(CRC->DR)))[0] = '6';
//	  ((volatile uint8_t*)(&(CRC->DR)))[0] = '7';
//	  ((volatile uint8_t*)(&(CRC->DR)))[0] = '8';
//	  ((volatile uint8_t*)(&(CRC->DR)))[0] = '9';
//
//	  uint32_t crc = CRC->DR;
//
//	  RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;
//
//	  print_sh_lf("CRC: 0x", crc);

}



void simple_freq_debug(const char *c)
{
	if (c!=NULL) print_s_lf(c);
	print_su_lf("xtal freq [Hz]: ",XTAL_FREQ);
	print_s("freq step [Hz]: "); print_s_lf(FREQ_STEP_STR);

	uint32_t chip_rate_coef = simple_get_chiprate_coef();
	uint32_t carrier_coef = get_carrier_coef();
	uint32_t fdev_coef = simple_get_fdev_coef();

	print_s_lf("--------------");
	print_su("carrier = ",carrier_coef); 			print_s_lf(" * freq_step");
	print_su("chip rate = 16 / ",chip_rate_coef);	print_s_lf(" * xtal");
	print_su("freq dev = ",fdev_coef);				print_s_lf(" * freq_step");
	print_s_lf("--------------");
	//print_s_lf("coarse freq step = %lu [Hz]",IMUL(COARSE_FREQ_STEP,FREQ_STEP_NUM,FREQ_STEP_DENOM));
	print_su_lf("coarse carrier [Hz] = ",IMUL(carrier_coef,COARSE_FREQ_STEP,FREQ_STEP_NUM,FREQ_STEP_DENOM));
	print_su_lf("coarse chip rate [chips/s]  = ",(XTAL_FREQ*16)/chip_rate_coef);
	print_su_lf("coarse freq dev [Hz] = ",IMUL(fdev_coef,COARSE_FREQ_STEP,FREQ_STEP_NUM,FREQ_STEP_DENOM));
	print_s_lf("--------------");
	//TODO: BW, AFCBW
}


void simple_regs_debug(const char *c)
{
	if (c!=NULL) print_s_lf(c);

	read_and_print_reg("OpMode  = ",	REG_OPMODE,"12113"); 		//0x01
	read_and_print_reg("BitrateMsb  = ",REG_BITRATEMSB,"8");	//0x02
	read_and_print_reg("BitrateLsb  = ",REG_BITRATELSB,"8");	//0x03
	read_and_print_reg("PaConfig = ",	REG_PACONFIG,"134");

	read_and_print_reg("Lna = ",REG_LNA,"3212");
	read_and_print_reg("RxConfig = ",REG_RXCONFIG,"111113");

	read_and_print_reg("PreambleDetect = ",REG_PREAMBLEDETECT,"125");

	read_and_print_reg("PreambleMsb  = ",REG_PREAMBLEMSB,"8");
	read_and_print_reg("PreambleLsb  = ",REG_PREAMBLELSB,"8");
	read_and_print_reg("SyncConfig   = ",REG_SYNCCONFIG,"21113");
	read_and_print_reg("SyncValue1 = ",REG_SYNCVALUE1,"8");
	read_and_print_reg("SyncValue2 = ",REG_SYNCVALUE2,"8");
	read_and_print_reg("SyncValue3 = ",REG_SYNCVALUE3,"8");
	read_and_print_reg("SyncValue4 = ",REG_SYNCVALUE4,"8");
	read_and_print_reg("SyncValue5 = ",REG_SYNCVALUE5,"8");
	read_and_print_reg("SyncValue6 = ",REG_SYNCVALUE6,"8");
	read_and_print_reg("SyncValue7 = ",REG_SYNCVALUE7,"8");
	read_and_print_reg("SyncValue8 = ",REG_SYNCVALUE8,"8");
	read_and_print_reg("PacketConfig1 = ",REG_PACKETCONFIG1,"121121");
	read_and_print_reg("PacketConfig2 = ",REG_PACKETCONFIG2,"111113");
	read_and_print_reg("PayLoadLength = ",REG_PAYLOADLENGTH,"8");

	read_and_print_reg("FifoThresh = ",REG_FIFOTHRESH,"116");
	read_and_print_reg("SeqConfig1 = ",REG_SEQCONFIG1,"1112111");
	read_and_print_reg("SeqConfig2 = ",REG_SEQCONFIG2,"323");

	read_and_print_reg("IrqFlags1 = ",REG_IRQFLAGS1,"8");
	read_and_print_reg("IrqFlags2 = ",REG_IRQFLAGS2,"8");
	read_and_print_reg("DioMapping1 = ",REG_DIOMAPPING1,"2222");
	read_and_print_reg("DioMapping2 = ",REG_DIOMAPPING2,"2231");
	read_and_print_reg("Version = ",REG_VERSION,"44");

	read_and_print_reg("PaDac = ",REG_PADAC,"53");

	read_and_print_reg("BitrateFrac = ",REG_BITRATEFRAC,"44");
		//end of the register list
}


void simple_mod_debug(const char *c)
{
	if (c!=NULL) print_s(c);
	if (simple_is_ook()==0) print_s_lf("FSK");else print_s_lf("OOK");
}


void app_state_debug(const char *c)
{
	if (c!=NULL) print_s_lf(c);

	print_s("Device role: ");
	switch (globals.device_role)
	{
		case ROLE_TX: print_s_lf("TX");break;
		case ROLE_RX: print_s_lf("RX");break;
		case ROLE_INTERFERER: print_s_lf("INTERFERER");break;
		default: print_s_lf("<UNK>");break;
	}

	print_s("Rx state: ");
	switch(globals.rx.state)
	{
		case RX_IDLE: print_s_lf("idle");break;
		case RX_SCANNING: print_s_lf("scanning");break;
		case RX_PREAMBLE: print_s_lf("preamble");break;
		case RX_SYNC: print_s_lf("sync");break;
		case RX_DOWNLOADING: print_s_lf("downloading");break;
		case RX_READY: print_s_lf("ready");break;
		case RX_ERROR: print_s_lf("error");break;
		default: print_s_lf("<unk>");break;
	}

	print_s("Tx state: ");
	switch(globals.tx.state)
	{
		case TX_IDLE: print_s_lf("idle");break;
		case TX_UPLOADING: print_s_lf("uploading");break;
		case TX_READY: print_s_lf("ready");break;
		case TX_ERROR: print_s_lf("error");break;
		default: print_s_lf("<unk>");break;
	}


}


void simple_misc_debug(const char *c)
{
	if (c!=NULL) print_s_lf(c);
	uint32_t soft_packet_time = globals_get_airtime((globals_t*)(&globals));
	uint32_t c_soft_packet_time=soft_packet_time*simple_get_chiprate_coef()/16/(XTAL_FREQ/1000000);

	print_su_lf("packet air time (software) [chips]: ",soft_packet_time);
	print_su_lf("coarse packet air time (software) [us]: ",c_soft_packet_time);

	print_reg8_lf("dip switch state: ",dip_switch_state(),"53");
	print_su_lf("PRNS seed: ",PRNS_SEED);
}



void simple_irqf_debug(const char *c)
{
	if (c!=NULL) print_s_lf(c);

	uint8_t r1=reg_read(REG_IRQFLAGS1);
	uint8_t r2=reg_read(REG_IRQFLAGS2);

	if ((r1&RF_IRQFLAGS1_MODEREADY)>0) print_s("mode_rdy ");
	if ((r1&RF_IRQFLAGS1_RXREADY)>0) print_s("rx_rdy ");
	if ((r1&RF_IRQFLAGS1_TXREADY)>0) print_s("tx_rdy ");
	if ((r1&RF_IRQFLAGS1_PLLLOCK)>0) print_s("pll ");
	if ((r1&RF_IRQFLAGS1_RSSI)>0) print_s("rssi ");
	if ((r1&RF_IRQFLAGS1_TIMEOUT)>0) print_s("to ");
	if ((r1&RF_IRQFLAGS1_PREAMBLEDETECT)>0) print_s("pre ");
	if ((r1&RF_IRQFLAGS1_SYNCADDRESSMATCH)>0) print_s("sync ");

	if ((r2&RF_IRQFLAGS2_FIFOFULL)>0) print_s("fifo_full ");
	if ((r2&RF_IRQFLAGS2_FIFOEMPTY)>0) print_s("fifo_emp ");
	if ((r2&RF_IRQFLAGS2_FIFOLEVEL)>0) print_s("fifo_lvl ");
	if ((r2&RF_IRQFLAGS2_FIFOOVERRUN)>0) print_s("fifo_ovr ");
	if ((r2&RF_IRQFLAGS2_PACKETSENT)>0) print_s("pckt_sent ");
	if ((r2&RF_IRQFLAGS2_PAYLOADREADY)>0) print_s("payload_rdy ");
	if ((r2&RF_IRQFLAGS2_CRCOK)>0) print_s("crc_ok ");
	if ((r2&RF_IRQFLAGS2_LOWBAT)>0) print_s("lowbat");

	print_lf();
}



void simple_packet_config_debug(const char *c)
{

	if (c!=NULL) print_s_lf(c);

	uint8_t c1=reg_read(REG_PACKETCONFIG1);
	uint8_t c2=reg_read(REG_PACKETCONFIG2);
	uint8_t	p=reg_read(REG_PAYLOADLENGTH);

	uint8_t vflag=0; //variable/fixed length
	print_s("length: ");
	switch(U8_GET_BITS(c1,REG_PACKETCONFIG1_PACKETFORMAT_Pos,REG_PACKETCONFIG1_PACKETFORMAT_Mask))
	{
		case 0: print_s_lf("fixed");break;
		case 1: print_s_lf("var");vflag=1;break;
		default: print_s_lf("<unk>");break;
	}

	print_s("DC free: ");
	switch(U8_GET_BITS(c1,REG_PACKETCONFIG1_DCFREE_Pos,REG_PACKETCONFIG1_DCFREE_Mask))
	{
		case 0: print_s_lf("off");break;
		case 1: print_s_lf("Manchester");break;
		case 2: print_s_lf("whitening");break;
		default: print_s_lf("<unk>");break;
	}

	print_s("CRC check: ");
	switch(U8_GET_BITS(c1,REG_PACKETCONFIG1_CRCON_Pos,REG_PACKETCONFIG1_CRCON_Mask))
	{
		case 0: print_s_lf("off");break;
		case 1: print_s_lf("on");break;
		default: print_s_lf("<unk>");break;
	}

	print_s("on CRC error: ");
	switch(U8_GET_BITS(c1,REG_PACKETCONFIG1_CRCAUTOCLEAROFF_Pos,REG_PACKETCONFIG1_CRCAUTOCLEAROFF_Mask))
	{
		case 0: print_s_lf("clear FIFO");break;
		case 1: print_s_lf("do not clear FIFO");break;
		default: print_s_lf("<unk>");break;
	}

	print_s("node addr filtering: ");
	switch(U8_GET_BITS(c1,REG_PACKETCONFIG1_ADDRESSFILTERING_Pos,REG_PACKETCONFIG1_ADDRESSFILTERING_Mask))
	{
		case 0: print_s_lf("off");break;
		case 1: print_s_lf("n");break;
		case 2: print_s_lf("n+b");break;
		default: print_s_lf("<unk>");break;
	}

	uint32_t pl=p|(U8_GET_BITS(c2,REG_PACKETCONFIG2_PAYLOADLENGTHMSB_Pos,REG_PACKETCONFIG2_PAYLOADLENGTHMSB_Mask)<<8);
	if (vflag==0)
		print_su_lf("payload length: ",pl);
	else
		print_su_lf("max payload length: ",pl);


}


void simple_to_debug(const char *c)
{
	if (c!=NULL) print_s_lf(c);
	print_su_lf("no sync word to [us]: ",globals.rx.no_sync_to*100);
	print_su_lf("no payload ready to [us]: ",globals.rx.no_payload_ready_to*100);
}
