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
#include "tim.h"
#include "usart.h"
// Components.
#include "led.h"

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

	/* Init components */
	LED_Init();

	/* Main loop */
	while(1) {
		LED_SetColor(LED_GREEN);
		LPTIM1_DelayMilliseconds(1000);
		LED_SetColor(LED_OFF);
		LPTIM1_DelayMilliseconds(1000);
	}

	return (0);
}
