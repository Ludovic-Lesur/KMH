/*
 * transfox.c
 *
 *  Created on: 11 oct. 2020
 *      Author: Ludo
 */

#include "transfox.h"

#include "cc1260.h"
#include "cc1260_config.h"
#include "cc1260_reg.h"
#include "dac.h"
#include "dac_reg.h"
#include "dma.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "spi.h"
#include "tim.h"

/*** TRANSFOX local macros ***/

#define TRANSFOX_RX_CENTER_FREQUENCY_HZ		868130000
#define TRANSFOX_RX_SAMPLE_RATE_BPS			192000
#define TRANSFOX_IQ_BUFFER_SIZE				64

/*** TRANSFOX local global variables ***/

unsigned int transfox_iq_buf1[TRANSFOX_IQ_BUFFER_SIZE] = {0x00};
unsigned int transfox_iq_buf2[TRANSFOX_IQ_BUFFER_SIZE] = {0x00};

/*** TRANSFOX functions ***/

/* START ANALOG TRANSFOX MODE IN RX.
 * @param:	None.
 * @return:	None.
 */
void TRANSFOX_StartAnalogRx(void) {
	// Configure DMA for GPIO to buffer transfer.
	DMA2_STR1_SetPeripheralAddress((unsigned int) &(GPIO_CC1260_UPP_PORT -> IDR));
	DMA2_STR1_SetMemoryAddress((unsigned int) &(transfox_iq_buf1[0]), (unsigned int) &(transfox_iq_buf2[0]), TRANSFOX_IQ_BUFFER_SIZE);
	DMA2_STR1_SetDirection(DMA_DIRECTION_PERIPHERAL_TO_MEMORY);
	// Configure DMA1 for buffer to DAC transfer.
	DMA1_STR5_SetPeripheralAddress((unsigned int) &(DAC -> DHR12LD));
	DMA1_STR5_SetMemoryAddress((unsigned int) &(transfox_iq_buf2[0]), (unsigned int) &(transfox_iq_buf1[0]), TRANSFOX_IQ_BUFFER_SIZE);
	DMA1_STR5_SetDirection(DMA_DIRECTION_MEMORY_TO_PERIPHERAL);
	// Start DMA streams.
	DAC_Start();
	TIM8_Start();
	DMA1_STR5_Start();
	DMA2_STR1_Start();
	TIM1_Start();
	// Start radio.
	SPI3_PowerOn();
	CC1260_Unreset();
	CC1260_SetConfig(CC1260_OPTIMUM_SETTING_RX, CC1260_OPTIMUM_SETTING_RX_SIZE);
	CC1260_SetFrequency(TRANSFOX_RX_CENTER_FREQUENCY_HZ);
	CC1260_SetRxSampleRate(TRANSFOX_RX_SAMPLE_RATE_BPS);
	CC1260_StartRxIq();
}

