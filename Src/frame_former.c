/*
 * frame_former.c
 *
 *  Created on: Dec 17, 2024
 *      Author: SK
 */
//
//#include "frame_former.h"
//#include <string.h>
//#include <main.h>
//#include "hamming.h"
//
//
//#define DATA_COUNT  20
//
//static const char *my_data[DATA_COUNT] = {
//    "eins",     "zwei",      "drei",     "vier",      "funf",
//    "sechs",    "sieben",    "acht",     "neun",      "zehn",
//    "elf",      "zwoelf",    "dreizehn", "vierzehn",  "funfzehn",
//    "sechzehn", "siebzehn",  "achtzehn", "neunzehn",  "zwanzig"
//};
//
//void frame_former_init(frame_former_t *ff)
//{
//    if (ff == NULL) return;
//    ff->counter = 0;
//}
//
//frame_former_ret_t frame_former_next(frame_former_t *ff)
//{
//    frame_former_ret_t ret = {NULL, 0};
//    if (ff == NULL) return ret;
//
//    const char *payload = my_data[ff->counter % DATA_COUNT];
//    uint8_t     pay_len = (uint8_t)strlen(payload);
//
//    /* should never trigger */
//    if (pay_len > FRAME_PAYLOAD_MAX)
//        pay_len = FRAME_PAYLOAD_MAX;
//
//    uint8_t *buf = ff->buf;
//
//    /* sync word - big endian */
//    buf[0] = (FRAME_SYNC >> 8) & 0xFF;
//    buf[1] =  FRAME_SYNC       & 0xFF;
//
//    /* counter - big endian */
//    buf[2] = (ff->counter >> 8) & 0xFF;
//    buf[3] =  ff->counter       & 0xFF;
//
//    /* payload length */
//    buf[4] = pay_len;
//
//    /* payload field: fill with spaces first, then copy string */
//    memset(&buf[FRAME_PAYLOAD_OFF], ' ',   FRAME_PAYLOAD_MAX);
//    memcpy(&buf[FRAME_PAYLOAD_OFF], payload, pay_len);
//    /* padding */
////    memset(&buf[FRAME_PADDING_OFF], 'o',   FRAME_PADDING_LEN);
//
//    /* CRC append at the end - last 4 bytes */
//
//	  RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
//
////	  CRC->CR = CRC_CR_RESET;
//
////	  for(int i = 0; i < pay_len; i++){
////	      ((volatile uint8_t*)(&(CRC->DR)))[0] = payload[i];
////	  }
////
////
////	  uint32_t crc = CRC->DR;
//
////	  CRC->CR = CRC_CR_RESET;
////
////	  for (uint32_t i = 0; i < FRAME_TOTAL_LEN - 4; i++) {
////	      ((volatile uint8_t*)(&(CRC->DR)))[0] = buf[i];
////	  }
////
////	  uint32_t crc = CRC->DR;
////
////	  RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;
////
////	  buf[FRAME_TOTAL_LEN - 4] = (crc >> 24) & 0xFF;
////	  buf[FRAME_TOTAL_LEN - 3] = (crc >> 16) & 0xFF;
////	  buf[FRAME_TOTAL_LEN - 2] = (crc >> 8)  & 0xFF;
////	  buf[FRAME_TOTAL_LEN - 1] =  crc        & 0xFF;
//
////
////	  memset(&buf[FRAME_PADDING_OFF], 'o', FRAME_PADDING_LEN);
//
//	  CRC->CR = CRC_CR_RESET;
//
//	  for (uint32_t i = 0; i < FRAME_CRC_OFF; i++) {
//	      ((volatile uint8_t*)(&(CRC->DR)))[0] = buf[i];
//	  }
//
//	  uint32_t crc = CRC->DR;
//
//	  buf[FRAME_PAYLOAD_OFF + pay_len + 0] = (crc >> 24) & 0xFF;
//	  buf[FRAME_PAYLOAD_OFF + pay_len + 1] = (crc >> 16) & 0xFF;
//	  buf[FRAME_PAYLOAD_OFF + pay_len + 2] = (crc >> 8)  & 0xFF;
//	  buf[FRAME_PAYLOAD_OFF + pay_len + 3] =  crc        & 0xFF;
//
//
////	  print_sh_lf("CRC: 0x", crc);
//
//    /* advance counter - rolls over naturally at 65535 */
//    ff->counter++;
//
//    /* --- Hamming(7,4) encode full 28-byte raw frame → 49 bytes --- */
//    uint8_t encoded_len = hamming_encode_block(buf, FRAME_TOTAL_LEN, ff->encoded);
//
//    /* filler byte at position 49 — can be 0x00 or any sentinel value */
//    ff->encoded[encoded_len] = 0x00;
//
//    ret.buf = (const uint8_t *)ff->encoded;
//    ret.len = FRAME_ENCODED_LEN;   /* 50 */
//    return ret;
//}

#include "frame_former.h"
#include "hamming.h"
#include "interleaver.h"
#include <string.h>
#include <main.h>

#define DATA_COUNT  20

//static const char *my_data[DATA_COUNT] = {
//    "__eins__",     "__zwei__",      "__drei__",     "vier",         "__funf__",
//    "__sechs__",    "__sieben__",    "__acht__",     "__neun__",     "__zehn__",
//    "elf",          "__zwoelf__",    "__dreizehn__", "__vierzehn__", "__funfzehn__",
//    "__sechzehn__", "__siebzehn__",  "__achtzehn__", "__neunzehn__", "__zwanzig__"
//};

static const char *my_data[DATA_COUNT] = {
    "eins",     "zwei",      "drei",     "vier",      "funf",
    "sechs",    "sieben",    "acht",     "neun",      "zehn",
    "elf",      "zwoelf",    "dreizehn", "vierzehn",  "funfzehn",
    "sechzehn", "siebzehn",  "achtzehn", "neunzehn",  "zwanzig"
};

void frame_former_init(frame_former_t *ff)
{
    if (ff == NULL) return;
    ff->counter = 0;
}

frame_former_ret_t frame_former_next(frame_former_t *ff)
{
    frame_former_ret_t ret = {NULL, 0};
    if (ff == NULL) return ret;

    const char *payload = my_data[ff->counter % DATA_COUNT];
    uint8_t     pay_len = (uint8_t)strlen(payload);

    /* should never trigger */
    if (pay_len > FRAME_PAYLOAD_MAX)
        pay_len = FRAME_PAYLOAD_MAX;

    uint8_t *buf = ff->buf;

    /* sync word - big endian */
    buf[0] = (FRAME_SYNC >> 8) & 0xFF;
    buf[1] =  FRAME_SYNC       & 0xFF;

    /* counter - big endian */
    buf[2] = (ff->counter >> 8) & 0xFF;
    buf[3] =  ff->counter       & 0xFF;

    /* payload length */
    buf[4] = pay_len;

    /* payload field: fill with spaces first, then copy string */
    memset(&buf[FRAME_PAYLOAD_OFF], ' ', FRAME_PAYLOAD_MAX);
    memcpy(&buf[FRAME_PAYLOAD_OFF], payload, pay_len);

    /* CRC immediately after payload - no padding */
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;

    CRC->CR = CRC_CR_RESET;

    for (uint32_t i = 0; i < FRAME_CRC_OFF; i++) {
        ((volatile uint8_t *)(&(CRC->DR)))[0] = buf[i];
    }

    uint32_t crc = CRC->DR;

    buf[FRAME_CRC_OFF + 0] = (crc >> 24) & 0xFF;
    buf[FRAME_CRC_OFF + 1] = (crc >> 16) & 0xFF;
    buf[FRAME_CRC_OFF + 2] = (crc >> 8)  & 0xFF;
    buf[FRAME_CRC_OFF + 3] =  crc        & 0xFF;

    /* advance counter - rolls over naturally at 65535 */
    ff->counter++;

    /* Hamming(8,4): 18 raw bytes → 36 encoded bytes (1 nibble = 1 byte out) */
    uint8_t encoded_len = hamming_encode_block(buf, FRAME_RAW_LEN, ff->encoded);

    /* fill remaining bytes to 50 with spaces */
    memset(&ff->encoded[encoded_len], ' ', FRAME_ENCODED_LEN - encoded_len);

//    ret.buf = (const uint8_t *)ff->encoded;
//    ret.len = FRAME_ENCODED_LEN;

    static uint8_t tx_buf[INTERLEAVER_SIZE];

    interleaver_encode(ff->encoded, tx_buf);

    ret.buf = tx_buf;
    ret.len = INTERLEAVER_SIZE;

    return ret;
}
