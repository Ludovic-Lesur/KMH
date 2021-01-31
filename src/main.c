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
#include "nvic.h"
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
	// Init NVIC.
	NVIC_Init();
	// Init clock.
	RCC_Init();
	RCC_EnableLsi();
	RCC_SwitchToPllclk(0);
	// Init GPIOs.
	GPIO_Init();
	EXTI_Init();
	// Init peripherals.
	LPTIM1_Init();
	TIM1_Init();
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
		// Enter sleep mode.
		__asm volatile ("wfi");
		// Print I/Q data (mapped on PORTD).
		//USART1_SendValue(((GPIOD -> IDR) & 0xFF), USART_FORMAT_BINARY, 1);
		//USART1_SendValue('\n', USART_FORMAT_ASCII, 0);
		//LPTIM1_DelayMilliseconds(200);
	}
	return (0);
}
