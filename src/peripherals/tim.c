/*
 * tim.c
 *
 *  Created on: 4 may 2018
 *      Author: Ludo
 */

#include "tim.h"

#include "rcc.h"
#include "rcc_reg.h"
#include "tim_reg.h"

/*** TIM functions ***/

/* CONFIGURE TIM3 TO OVERFLOW EVERY SECOND.
 * @param:	None.
 * @return:	None.
 */
void TIM3_Init(void) {

	/* Enable peripheral clock */
	RCC -> APB1ENR |= (0b1 << 1); // TIM3EN='1'.

	/* Reset timer before configuration */
	TIM3 -> CR1 &= ~(0b1 << 0); // Disable TIM3 (CEN='0').
	TIM3 -> CNT = 0; // Reset counter.
	TIM3 -> SR &= 0xFFFFE1A0; // Clear all flags.

	/* Configure TIM3 as master to count milliseconds and overflow every seconds */
	TIM3 -> PSC = (2 * RCC_GetPclk1Khz()); // Timer is clocked by 2*PCLK1.
	TIM3 -> ARR = 1000;
	TIM3 -> CR2 &= ~(0b111 << 4); // Reset bits 4-6.
	TIM3 -> CR2 |= (0b010 << 4); // Generate trigger on update event (MMS='010').

	/* Generate event to update registers */
	TIM3 -> EGR |= (0b1 << 0); // UG='1'.
}

/* ENABLE TIM3 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void TIM3_Start(void) {

	/* Enable peripheral clock */
	RCC -> APB1ENR |= (0b1 << 1); // TIM3EN='1'.

	/* Enable TIM3 peripheral */
	TIM3 -> SR &= ~(0b1 << 0); // Clear flag (UIF='0').
	TIM3 -> CR1 |= (0b1 << 0); // Enable TIM3 (CEN='1').
}

/* STOP TIM3 COUNTER.
 * @param:	None.
 * @return:	None.
 */
void TIM3_Stop(void) {

	/* Stop TIM3 */
	TIM3 -> CR1 &= ~(0b1 << 0); // CEN='0'.
}

/* DISABLE TIM3 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void TIM3_Disable(void) {

	/* Disable TIM3 peripheral */
	TIM3 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM3 -> CNT = 0;
	RCC -> APB1ENR &= ~(0b1 << 1); // TIM3EN='0'.
}

/* CONFIGURE TIM2 TO COUNT SECONDS SINCE START-UP.
 * @param:	None.
 * @return:	None.
 */
void TIM2_Init(void) {

	/* Enable peripheral clock */
	RCC -> APB1ENR |= (0b1 << 0); // TIM2EN='1'.

	/* Reset timer before configuration */
	TIM2 -> CR1 &= ~(0b1 << 0); // Disable TIM2 (CEN='0').
	TIM2 -> CNT = 0; // Reset counter.
	TIM2 -> SR &= 0xFFFFE1A0; // Clear all flags.

	/* Configure TIM2 as slave to count seconds */
	TIM2 -> SMCR &= ~(0b111 << 4); // Reset bits 4-6.
	TIM2 -> SMCR |= (0b010 << 4); // TS = '010' to select ITR2 = TIM3 as trigger input.
	TIM2 -> SMCR |= (0b0111 << 0); // SMS = '0111' to enable slave mode with external clock.

	/* Generate event to update registers */
	TIM2 -> EGR |= (0b1 << 0); // UG='1'.
}

/* ENABLE TIM2 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void TIM2_Start(void) {

	/* Enable peripheral clock */
	RCC -> APB1ENR |= (0b1 << 0); // TIM2EN='1'.

	/* Enable TIM2 peripheral */
	TIM2 -> CR1 |= (0b1 << 0); // Enable TIM2 (CEN='1').
}

/* STOP TIM2 COUNTER.
 * @param:	None.
 * @return:	None.
 */
void TIM2_Stop(void) {

	/* Disable TIM2 peripheral */
	TIM2 -> CR1 &= ~(0b1 << 0); // CEN='0'.
}

/* DISABLE TIM2 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void TIM2_Disable(void) {

	/* Disable TIM2 peripheral */
	TIM2 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM2 -> CNT = 0;
	RCC -> APB1ENR &= ~(0b1 << 0); // TIM2EN='0'.
}

/* RETURNS THE NUMBER OF SECONDS ELLAPSED SINCE START-UP.
 * @param:	None.
 * @return:	Number of seconds ellapsed since start-up.
 */
volatile unsigned int TIM2_GetSeconds(void) {
	return (TIM2 -> CNT);
}