/*
 * globals.c
 *
 *  Created on: 21 gru 2024
 *      Author: SK
 */

#include "globals.h"

volatile globals_t globals;


globals_error_t globals_tx(globals_t *this, uint16_t pre_len, uint8_t sync_len,payload_length_t pay_len, dcfree_mode_t dcfree,crc_mode_t crc )
{
	if(this==NULL) return G_ERR_NULL;

	this->print_irqs=0;
	this->device_role = ROLE_TX;
	this->tmode=MODE_SIMPLE;
	this->channel_sim=0;

	this->tx.state=TX_IDLE;
	this->tx.error=TX_ERROR_NONE;

	this->tx.dcfree_mode = dcfree;

	uint32_t pre_len_lim = MAX_PREAMBLE_LENGTH; //to avoid warning
	if ((pre_len==0)||(pre_len>pre_len_lim)) return G_ERR_PREAMBLE;
	this->tx.preamble_length=pre_len;

	if ((sync_len==0)||(sync_len>MAX_SYNC_LENGTH)) return G_ERR_SYNC;
	this->tx.sync_length=sync_len;

	this->tx.payload_length.len=0;
	if (pay_len.len==0) return G_ERR_PAYLOAD;
	if (pay_len.var!=0) return G_ERR_SUPP; //variable mode not supported yet
	this->tx.payload_length=pay_len;

	if (crc!=CRC_NONE) return G_ERR_SUPP;
	this->tx.crc_mode = crc;

	return G_ERR_NONE;
}



globals_error_t globals_rx(globals_t *this, uint16_t pre_len, uint8_t sync_len, payload_length_t pay_len, dcfree_mode_t dcfree,crc_mode_t crc )
{
	if(this==NULL) return G_ERR_NULL;

	this->print_irqs=0;
	this->device_role = ROLE_RX;
	this->tmode=MODE_SIMPLE;
	this->channel_sim=0;

	this->rx.dcfree_mode=dcfree;

	this->rx.state=RX_IDLE;
	this->rx.error=RX_ERROR_NONE;

	uint32_t pre_len_lim = MAX_PREAMBLE_LENGTH; //to avoid warning
	if ((pre_len==0)||(pre_len>pre_len_lim)) return G_ERR_PREAMBLE;
	this->rx.preamble_length=pre_len;

	if ((sync_len==0)||(sync_len>MAX_SYNC_LENGTH)) return G_ERR_SYNC;
	this->rx.sync_length=sync_len;

	this->rx.payload_length.len=0;
	if (pay_len.len==0) return G_ERR_PAYLOAD;
	if (pay_len.var!=0) return G_ERR_SUPP; //variable mode not supported yet
	this->rx.payload_length=pay_len;

	if (crc!=CRC_NONE) return G_ERR_SUPP;
	this->rx.crc_mode = crc;

	this->rx.no_sync_to=0;
	this->rx.no_payload_ready_to=0;

	return G_ERR_NONE;
}


uint32_t globals_get_tx_airtime(globals_t *this)

{
	if (this==0) return 0;

	uint32_t total = this->tx.payload_length.len;

	if (this->tx.crc_mode!=CRC_NONE) total+=2;

	if (this->tx.dcfree_mode==DCFREE_MAN) total*=2;

	total +=  this->tx.preamble_length + this->tx.sync_length;

	return total*8;
}



uint32_t globals_get_rx_airtime(globals_t *this)

{
	if (this==0) return 0;

	uint32_t total = this->rx.payload_length.len;

	if (this->rx.crc_mode!=CRC_NONE) total+=2;

	if (this->rx.dcfree_mode==DCFREE_MAN) total*=2;

	total +=  this->rx.preamble_length + this->rx.sync_length;

	return total*8;
}



uint32_t globals_get_airtime(globals_t *this)
{
	if (this==NULL) return 0;

	switch(this->device_role)
	{
		case ROLE_RX: return globals_get_rx_airtime(this);break;
		case ROLE_TX: return globals_get_tx_airtime(this);break;
		case ROLE_INTERFERER: return 0;	break;
		default: return 0; break;
	}

}


