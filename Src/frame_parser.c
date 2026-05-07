/////*
//// * frame_parser.c
//// *
//// *  Created on: Dec 17, 2024
//// *      Author: SK
//// */
////
////
////#include "frame_parser.h"
////
////void frame_parser_init(frame_parser_t *this)
////{
////	if (this==NULL) return;
////
////	this->counter = (uint32_t*)(this->buf);
////	this->length =  this->buf+sizeof(uint32_t);
////	this->body=this->buf + FRAME_PARSER_OVERHEAD;
////
////	this->cnt=0;
////}
////
////void frame_parser_reset(frame_parser_t *this)
////{
////	if (this==NULL) return;
////	this->cnt=0;
////}
////
////
//////void frame_parser_parse(frame_parser_t *this,const uint8_t *src, uint32_t len)
//////{
//////	if (this==NULL) 				{ print_s_lf("parse error: parser is NULL"); return;}
//////	if (src==NULL) 					{ print_s_lf("parse error: source is NULL"); return;}
//////	if (len<FRAME_PARSER_OVERHEAD)  { print_s_lf("parse error: payload too short"); return;}
//////	if (len>MAX_PAYLOAD_LENGTH)  	{ print_s_lf("parse error: payload too long"); return;}
//////
//////	fastmemcpy(this->buf,src,len);
//////	uint32_t counter=this->counter[0];
//////	uint32_t length=this->length[0];
//////
//////
//////	if (this->cnt!=counter)
//////		{
//////			print_s_lf("*** FRAME ORDER ERROR ***");
//////			this->cnt=counter;
//////		}
//////	this->cnt ++;
//////
//////	if (length>len-FRAME_PARSER_OVERHEAD) {print_s_lf("*** FRAME LENGTH ERROR ***");return;}
//////
//////	//even parity
//////	even_parity_dec(this->body,length,'*');
//////	this->body[length]=0;
//////	print_s_lf((const char*)this->body);
//////}
////
////void frame_parser_parse(frame_parser_t *this, const uint8_t *src, uint32_t len)
////{
////	if (this==NULL) { print_s_lf("parse error: parser is NULL"); return;}
////	if (src==NULL) { print_s_lf("parse error: source is NULL"); return;}
////	if (len!=50) { print_su_lf("parse error: payload len = ",len); return;}
////
////	fastmemcpy(this->buf, src, len);
////	my_frame_parser(this->buf);
////}
////
////
////
////void my_frame_parser(uint8_t *buf)
////{
////	print_s_lf(buf);
////}
//
//#include "frame_parser.h"
//#include "hamming.h"
//#include "interleaver.h"
//#include <stdio.h>
//#include <string.h>
//#include <main.h>
//
//static void frame_parser_handle_raw(frame_parser_t *fp, const uint8_t *raw)
//{
//    uint16_t sync;
//    uint16_t count;
//    uint8_t  data_len;
//    uint32_t rx_crc;
//    uint32_t calc_crc;
//    char data[FRAME_PAYLOAD_MAX + 1];
//    char line[160];
//
//    /* sync - big endian */
//    sync = ((uint16_t)raw[0] << 8) | (uint16_t)raw[1];
//    if (sync != FRAME_SYNC) {
//        return;
//    }
//
//    /* counter - big endian */
//    count = ((uint16_t)raw[2] << 8) | (uint16_t)raw[3];
//
//    /* len */
//    data_len = raw[4];
//    if (data_len > FRAME_PAYLOAD_MAX) {
//        print_s_lf("parse error: bad len");
//        return;
//    }
//
//    /* payload */
//    memcpy(data, &raw[FRAME_PAYLOAD_OFF], data_len);
//    data[data_len] = '\0';
//
//    /* CRC from raw frame - big endian */
//    rx_crc = ((uint32_t)raw[FRAME_CRC_OFF + 0] << 24) |
//             ((uint32_t)raw[FRAME_CRC_OFF + 1] << 16) |
//             ((uint32_t)raw[FRAME_CRC_OFF + 2] << 8)  |
//             ((uint32_t)raw[FRAME_CRC_OFF + 3]);
//
//    /* CRC exactly like in frame_former:
//       over raw[0..13] = sync + counter + len + full 9-byte payload field */
//    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
//    CRC->CR = CRC_CR_RESET;
//    for (uint32_t i = 0; i < FRAME_CRC_OFF; i++) {
//        ((volatile uint8_t *)(&(CRC->DR)))[0] = raw[i];
//    }
//    calc_crc = CRC->DR;
//    RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;
//
//    if (calc_crc != rx_crc) {
//        snprintf(line, sizeof(line),
//                 "[%05u] len=%u crc=ERR data=\"%s\"",
//                 (unsigned)count, (unsigned)data_len, data);
//        print_s_lf(line);
//        return;
//    }
//
//    /* sequence check only for CRC-correct frames */
//    if (!fp->has_previous_count) {
//        fp->previous_count = (uint16_t)(count - 1);
//        fp->has_previous_count = 1;
//    }
//
//    uint16_t expected = (uint16_t)(fp->previous_count + 1);
//    while (expected != count) {
//        snprintf(line, sizeof(line), "missing frame for counter: %05u", (unsigned)expected);
//        print_s_lf(line);
//        expected++;
//    }
//
//    fp->previous_count = count;
//
//    snprintf(line, sizeof(line),
//             "[%05u] len=%u crc=OK  data=\"%s\"",
//             (unsigned)count, (unsigned)data_len, data);
//    print_s_lf(line);
//}
//
//void frame_parser_init(frame_parser_t *fp)
//{
//    if (fp == NULL) return;
//    fp->previous_count = 0;
//    fp->has_previous_count = 0;
//    fp->cnt = 0;
//}
//
//void frame_parser_reset(frame_parser_t *fp)
//{
//    if (fp == NULL) return;
//    fp->previous_count = 0;
//    fp->has_previous_count = 0;
//    fp->cnt = 0;
//}
//
//void frame_parser_parse(frame_parser_t *fp, const uint8_t *src, uint32_t len)
//{
//    uint8_t raw_len;
//
//    if (fp == NULL) {
//        print_s_lf("parse error: parser is NULL");
//        return;
//    }
//    if (src == NULL) {
//        print_s_lf("parse error: source is NULL");
//        return;
//    }
//    if (len != INTERLEAVER_SIZE) {
//        print_su_lf("parse error: frame len = ", len);
//        return;
//    }
//
//    /* 1. copy received interleaved frame */
//    memcpy(fp->rx_buf, src, INTERLEAVER_SIZE);
//
//    /* 2. deinterleave: back to original 50-byte encoded frame */
//    interleaver_decode(fp->rx_buf, fp->encoded);
//
//    /* 3. decode only first 36 bytes from Hamming(8,4) */
//    raw_len = hamming_decode_block(fp->encoded, FRAME_HAMMING_LEN, fp->raw);
//    if (raw_len != FRAME_RAW_LEN) {
//        print_s_lf("parse error: hamming decode failed");
//        return;
//    }
//
//    /* 4. parse recovered raw frame */
//    frame_parser_handle_raw(fp, fp->raw);
//    fp->cnt++;
//}

#include "frame_parser.h"
#include "hamming.h"
#include "interleaver.h"

#include <stdio.h>
#include <string.h>
#include <main.h>

static void frame_parser_handle_raw(frame_parser_t *fp, const uint8_t *raw)
{
    uint16_t sync;
    uint16_t count;
    uint8_t  data_len;
    uint32_t rx_crc;
    uint32_t calc_crc;
    uint16_t expected_count;

    char data[FRAME_PAYLOAD_MAX + 1];
    char line[160];

    /* sync - big endian */
    sync = ((uint16_t)raw[0] << 8) | (uint16_t)raw[1];

    if (sync != FRAME_SYNC) {
        return;
    }

    /* counter - big endian */
    count = ((uint16_t)raw[2] << 8) | (uint16_t)raw[3];

    /* len */
    data_len = raw[4];
    if (data_len > FRAME_PAYLOAD_MAX) {
        print_s_lf("parse error: bad len");
        return;
    }

    /* payload */
    memcpy(data, &raw[FRAME_PAYLOAD_OFF], data_len);
    data[data_len] = '\0';

    /* CRC from raw frame - big endian */
    rx_crc = ((uint32_t)raw[FRAME_CRC_OFF + 0] << 24) |
             ((uint32_t)raw[FRAME_CRC_OFF + 1] << 16) |
             ((uint32_t)raw[FRAME_CRC_OFF + 2] << 8)  |
             ((uint32_t)raw[FRAME_CRC_OFF + 3]);

    /* CRC exactly like in frame_former:
       over raw[0..13] = sync + counter + len + full 9-byte payload field */
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
    CRC->CR = CRC_CR_RESET;

    for (uint32_t i = 0; i < FRAME_CRC_OFF; i++) {
        ((volatile uint8_t *)(&(CRC->DR)))[0] = raw[i];
    }

    calc_crc = CRC->DR;
    RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;

    /* sequence tracking independent of CRC result */
    if (!fp->has_previous_count) {
        fp->previous_count = (uint16_t)(count - 1);
        fp->has_previous_count = 1;
    }

    expected_count = (uint16_t)(fp->previous_count + 1);

    if (count != expected_count) {
        snprintf(line, sizeof(line),
                 "missing frame: expected=%u received=%u",
                 (unsigned)expected_count,
                 (unsigned)count);
        print_s_lf(line);
    }

    /* always remember received counter (for valid sync frame) */
    fp->previous_count = count;

    snprintf(line, sizeof(line),
             "count=%u len=%u data=%s crc=0x%08lX %s",
             (unsigned)count,
             (unsigned)data_len,
             data,
             (unsigned long)rx_crc,
             (calc_crc == rx_crc) ? "CRC_OK" : "CRC_ERROR");

    print_s_lf(line);
}

void frame_parser_init(frame_parser_t *fp)
{
    if (fp == NULL) return;

    fp->previous_count = 0;
    fp->has_previous_count = 0;
    fp->cnt = 0;
}

void frame_parser_reset(frame_parser_t *fp)
{
    if (fp == NULL) return;

    fp->previous_count = 0;
    fp->has_previous_count = 0;
    fp->cnt = 0;
}

void frame_parser_parse(frame_parser_t *fp, const uint8_t *src, uint32_t len)
{
    uint8_t raw_len;

    if (fp == NULL) {
        print_s_lf("parse error: parser is NULL");
        return;
    }

    if (src == NULL) {
        print_s_lf("parse error: source is NULL");
        return;
    }

    if (len != INTERLEAVER_SIZE) {
        print_su_lf("parse error: frame len = ", len);
        return;
    }

    /* 1. copy received interleaved frame */
    memcpy(fp->rx_buf, src, INTERLEAVER_SIZE);

    /* 2. deinterleave: back to original 50-byte encoded frame */
    interleaver_decode(fp->rx_buf, fp->encoded);

    /* 3. decode only first 36 bytes from Hamming(8,4) */
    raw_len = hamming_decode_block(fp->encoded, FRAME_HAMMING_LEN, fp->raw);

    if (raw_len != FRAME_RAW_LEN) {
        print_s_lf("parse error: hamming decode failed");
        return;
    }

    /* 4. parse recovered raw frame */
    frame_parser_handle_raw(fp, fp->raw);

    fp->cnt++;
}
