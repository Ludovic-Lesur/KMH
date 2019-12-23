/*
 * main.c
 *
 *  Created on: 6 dec. 2019
 *      Author: Ludovic
 */

// Peripherals.
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

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {

	/* Init clock and memory */
	RCC_Init();
	FLASH_Init();

	/* Init timers */
	TIM2_Init();
	TIM3_Init();
	TIM2_Start();
	TIM3_Start();
	LPTIM1_Init();

	/* Init peripherals */
	GPIO_Init();
	USART1_Init();
	SPI3_Init();

	/* Init components */
	LED_Init();
	CC1260_Init();

	/* RX I/Q test */
	SPI3_PowerOn();
	CC1260_Unreset();
	CC1260_SetConfig(CC1260_OPTIMUM_SETTING_RX, CC1260_OPTIMUM_SETTING_RX_SIZE);
	CC1260_SetFrequency(868130000);
	CC1260_StartRxIq();

	/* Main loop */
	while(1) {
		// Print I/Q data (mapped on PORTD).
		USART1_SendValue(((GPIOD -> IDR) & 0xFF), USART_FORMAT_BINARY, 1);
		USART1_SendValue('\n', USART_FORMAT_ASCII, 0);
		LPTIM1_DelayMilliseconds(200);
	}

	return (0);
}
