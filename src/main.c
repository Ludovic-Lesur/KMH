/*
 * main.c
 *
 *  Created on: 6 dec. 2019
 *      Author: Ludo
 */

// Peripherals.
#include "dac.h"
#include "dma.h"
#include "exti.h"
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
// Applicative.
#include "transfox.h"

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {
	// Init clock.
	RCC_Init();
	RCC_SwitchToPllclk(0);
	// Init GPIOs.
	GPIO_Init();
	EXTI_Init();
	// Init peripherals.
	LPTIM1_Init();
	TIM1_Init();
	TIM8_Init();
	SPI3_Init();
	DMA1_STR5_Init();
	DMA2_STR1_Init();
	DAC_Init();
	USART1_Init();
	// Init components.
	LED_Init();
	CC1260_Init();
	// Analog transfox mode test.
	TRANSFOX_StartAnalogRx();
	// Main loop.
	while(1) {
		__asm volatile ("wfi");
	}
	return (0);
}
