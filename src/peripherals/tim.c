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
#include "nvic.h"

#include "gpio.h"
#include "mapping.h"

/*** TIM local functions ***/

/* TIM4 INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void TIM1_CC_IRQHandler(void) {
	// Check flag.
	if (((TIM1 -> SR) & (0b1 << 1)) != 0) {
		GPIOE -> ODR ^= (0b1 << 9);
		TIM1 -> SR &= ~(0b1 << 1);
	}
}

/*** TIM functions ***/

/* CONFIGURE TIM1_CH1 IN INPUT CAPTURE MODE TO TRIGGER DMA2 STREAM 1 REQUEST.
 * @param:	None.
 * @return:	None.
 */
void TIM1_Init(void) {
	// Enable peripheral clock.
	RCC -> APB2ENR |= (0b1 << 0); // TIM1EN='1'.
	// Reset timer before configuration.
	TIM1 -> CR1 &= ~(0b1 << 0); // Disable TIM1 (CEN='0').
	TIM1 -> CNT = 0; // Reset counter.
	TIM1 -> CCER &= ~(0b1 << 0); // Disable input capture (CC1E='0').
	// Configure timer in input capture mode.
	TIM1 -> CCMR1 |= (0b01 << 0); // Input mode, IC1 mapped on TI1 (CC1S='01').
	TIM1 -> CCER |= (0b101 << 1); // Both edges capture (CC1P='1' and CC1NP='1').
	TIM1 -> CCER |= (0b1 << 0); // Enable input capture (CC1E='1').
	//TIM1 -> DIER |= (0b1 << 1); // CC1IE='1'.
	// Generate event to update registers.
	TIM1 -> EGR |= (0b1 << 0); // UG='1'.
	// Note: enabling the counter is not required to trigger DMA request.
	TIM1 -> SR &= 0xFFFFE1A0; // Clear all flags.
	//NVIC_EnableInterrupt(NVIC_IT_TIM1_CC);
}

/* START TIM1 OPERATION.
 * @param:	None.
 * @return:	None.
 */
void TIM1_Start(void) {
	// Enable DMA request trigger.
	TIM1 -> DIER |= (0b1 << 9); // CC1DE='1'.
}

/* STOP TIM1 OPERATION.
 * @param:	None.
 * @return:	None.
 */
void TIM1_Stop(void) {
	// Enable DMA request trigger.
	TIM1 -> DIER &= ~(0b1 << 9); // CC3DE='0'.
}

/* DISABLE TIM1 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void TIM1_Disable(void) {
	// Disable TIM4 peripheral.
	TIM1-> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM1 -> CNT = 0;
	RCC-> APB2ENR &= ~(0b1 << 0); // TIM1EN='0'.
}
