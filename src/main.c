/*
 * main.c
 *
 *  Created on: 6 dec. 2019
 *      Author: Ludovic
 */

#include "gpio.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "rcc.h"
#include "rcc_reg.h"
#include "usart.H"

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {

	/* Init clock */
	RCC_Init();

	/* Init RGB LED pins */
	GPIO_Init();
	GPIO_Configure(&GPIO_LED_RED, GPIO_MODE_OUTPUT, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LED_GREEN, GPIO_MODE_OUTPUT, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LED_BLUE, GPIO_MODE_OUTPUT, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);

	/* Init peripherals */
	USART1_Init();

	/* Main loop */
	unsigned int i = 0;
	unsigned int j = 0;
	while(1) {
		GPIOB -> ODR = 0b011;
		for (i=0 ; i<500000 ; i++);
		GPIOB -> ODR = 0b001;
		for (i=0 ; i<500000 ; i++);
		GPIOB -> ODR = 0b101;
		for (i=0 ; i<500000 ; i++);
		GPIOB -> ODR = 0b100;
		for (i=0 ; i<500000 ; i++);
		GPIOB -> ODR = 0b110;
		for (i=0 ; i<500000 ; i++);
		GPIOB -> ODR = 0b010;
		for (i=0 ; i<500000 ; i++);
		USART1_SendByte(j, USART_FORMAT_DECIMAL);
		j++;
	}

	return (0);
}
