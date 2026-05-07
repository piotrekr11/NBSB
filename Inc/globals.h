/*
 * globals.h
 *
 *  Created on: 21 gru 2024
 *      Author: SK
 */

#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_
#include "alt_driver.h"
#include "payload_length.h"

#ifndef NULL
	#define NULL ((void*)(0))
#endif


typedef enum rx_states {RX_IDLE, RX_SCANNING, RX_PREAMBLE, RX_SYNC, RX_DOWNLOADING, RX_READY, RX_ERROR}rx_states_t;
typedef enum tx_states {TX_IDLE, TX_UPLOADING, TX_READY, TX_ERROR}tx_states_t;

typedef enum rx_errors {RX_ERROR_NONE, RX_ERROR_STATE, RX_ERROR_PAYLOAD_SIZE, RX_ERROR_TO }rx_errors_t;
typedef enum tx_errors {TX_ERROR_NONE, TX_ERROR_PAYLOAD_SIZE}tx_errors_t;

typedef enum device_role {ROLE_TX, ROLE_RX, ROLE_INTERFERER} device_role_t;

typedef enum {G_ERR_NONE, G_ERR_NULL, G_ERR_SUPP,G_ERR_PREAMBLE,G_ERR_SYNC, G_ERR_ADDR, G_ERR_PAYLOAD}globals_error_t;

typedef struct simple_rx
{
	rx_states_t state;
	rx_errors_t error;

	uint16_t preamble_length;
	uint8_t sync_length;
	payload_length_t payload_length;
	crc_mode_t crc_mode; //TODO
	dcfree_mode_t dcfree_mode;
	uint32_t no_sync_to, no_payload_ready_to;

}simple_rx_t;


typedef struct simple_tx
{
	tx_states_t state;
	tx_errors_t error;

	uint16_t preamble_length;
	uint8_t sync_length;
	payload_length_t payload_length;
	crc_mode_t crc_mode; //TODO
	dcfree_mode_t dcfree_mode;


}simple_tx_t;




typedef struct globals
{
	uint8_t print_irqs, channel_sim;
	transceiver_mode_t tmode;
	device_role_t device_role;
	simple_rx_t rx;
	simple_tx_t tx;
	//lora_rx_t lorarx;
	//lora_tx_t loratx;
	uint32_t user_data1;

}globals_t;

//pay_len includes packet_length (in var length mode) and node address if present
globals_error_t globals_tx(globals_t *this, uint16_t pre_len, uint8_t sync_len, payload_length_t pay_len, dcfree_mode_t dcfree, crc_mode_t crc );
globals_error_t globals_rx(globals_t *this, uint16_t pre_len, uint8_t sync_len, payload_length_t pay_len, dcfree_mode_t dcfree, crc_mode_t crc );


//returns airtime in [chips]
uint32_t globals_get_rx_airtime(globals_t *this);
uint32_t globals_get_tx_airtime(globals_t *this);

//selects rx or tx according to istxflag
uint32_t globals_get_airtime(globals_t *this);





#endif /* INC_GLOBALS_H_ */
