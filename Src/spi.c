/*
 * spi.c
 *
 *  Created on: Jan 30, 2025
 *      Author: SK
 */
#include "spi.h"




uint8_t spi_init(spi_t *s)
{
	if (s==NULL) return 0;

	__HAL_RCC_SPI1_FORCE_RESET( );
	__HAL_RCC_SPI1_RELEASE_RESET( );
    __HAL_RCC_SPI1_CLK_ENABLE( );


	s->spi.Instance = SPI1;
	s->spi.Init.Mode = SPI_MODE_MASTER;
	s->spi.Init.Direction = SPI_DIRECTION_2LINES;
	s->spi.Init.DataSize = SPI_DATASIZE_8BIT;
	s->spi.Init.CLKPolarity = SPI_POLARITY_LOW;
	s->spi.Init.CLKPhase = SPI_PHASE_1EDGE;
	s->spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	s->spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	s->spi.Init.TIMode = SPI_TIMODE_DISABLE;
	s->spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	s->spi.Init.CRCPolynomial = 0x7;
	s->spi.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	s->spi.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	s->spi.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	s->spi.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	s->spi.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	s->spi.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	s->spi.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	s->spi.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	s->spi.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
	s->spi.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
	s->spi.Init.NSS = SPI_NSS_SOFT;

	if (HAL_SPI_Init(&(s->spi)) != HAL_OK) return 0;
	return 1;

}




