/*
 * main.c
 *
 *  Created on: 6 dec. 2019
 *      Author: Ludo
 */

// Peripherals.
#include "dac.h"
#include "dma.h"
#include "flash.h"
#include "gpio.h"
#include "lptim.h"
#include "mapping.h"
#include "rcc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
// Components.
#include "led.h"
#include "cc1260.h"
#include "cc1260_config.h"
#include "cc1260_reg.h"

/*** MAIN macros ***/

#define KMH_RX_CENTER_FREQUENCY_HZ	868130000
#define KMH_RX_SAMPLE_RATE_BPS		192000
#define KMH_IQ_BUFFER_SIZE			64

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {
	// Init clock.
	RCC_Init();
	RCC_SwitchToPllclk(0);
	// Init timers for timeout and delay.
	TIM2_Init();
	TIM3_Init();
	TIM2_Start();
	TIM3_Start();
	LPTIM1_Init();
	// Init peripherals.
	GPIO_Init();
	SPI3_Init();
	DMA1_Init();
	DAC_Init();
	USART1_Init();
	// Init components.
	LED_Init();
	CC1260_Init();
	// RX I/Q test.
	unsigned short iq_buf1[KMH_IQ_BUFFER_SIZE];
	unsigned short iq_buf2[KMH_IQ_BUFFER_SIZE];
	unsigned int byte_idx = 0;
	for (byte_idx=0 ; byte_idx<KMH_IQ_BUFFER_SIZE ; byte_idx++) iq_buf1[byte_idx] = 0;
	for (byte_idx=0 ; byte_idx<KMH_IQ_BUFFER_SIZE ; byte_idx++) iq_buf2[byte_idx] = 0;
	// Turn RF front-end on.
	GPIO_Configure(&GPIO_SCREEN_IO0, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	TIM4_Init();
	SPI3_PowerOn();
	CC1260_Unreset();
	CC1260_SetConfig(CC1260_OPTIMUM_SETTING_RX, CC1260_OPTIMUM_SETTING_RX_SIZE);
	CC1260_SetFrequency(KMH_RX_CENTER_FREQUENCY_HZ);
	CC1260_SetRxSampleRate(KMH_RX_SAMPLE_RATE_BPS);
	CC1260_StartRxIq((unsigned int) &iq_buf1, (unsigned int) &iq_buf2, KMH_IQ_BUFFER_SIZE);
	// Main loop.
	while(1);
	return (0);
}
