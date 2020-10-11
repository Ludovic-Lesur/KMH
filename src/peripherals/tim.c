/*
 * tim.c
 *
 *  Created on: 4 may 2018
 *      Author: Ludo
 */

#include "tim.h"

#include "gpio.h"
#include "mapping.h"
#include "rcc.h"
#include "rcc_reg.h"
#include "tim_reg.h"
#include "nvic.h"

/*** TIM functions ***/

/* CONFIGURE TIM1_CH1 IN INPUT CAPTURE MODE TO TRIGGER DMA2 STREAM 1 REQUEST FROM UPP_DCLK.
 * @param:	None.
 * @return:	None.
 */
void TIM1_Init(void) {
	// Configure DCLK GPIO.
	GPIO_Configure(&GPIO_CC1260_DCLK, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_DOWN);
	// Enable peripheral clock.
	RCC -> APB2ENR |= (0b1 << 0); // TIM1EN='1'.
	// Reset timer before configuration.
	TIM1 -> CR1 &= ~(0b1 << 0); // Disable TIM1 (CEN='0').
	TIM1 -> CNT = 0; // Reset counter.
	TIM1 -> CCER &= ~(0b1 << 0); // Disable input capture (CC1E='0').
	// Configure timer in input capture mode.
	TIM1 -> CCMR1 |= (0b01 << 0); // Input mode, IC1 mapped on TI1 (CC1S='01').
	TIM1 -> CCER |= (0b101 << 1); // Both edges capture (CC1P='1' and CC1NP='1').
	// Generate event to update registers.
	TIM1 -> EGR |= (0b1 << 0); // UG='1'.
	TIM1 -> SR &= 0xFFFFE1A0; // Clear all flags.
	// Note: enabling the counter is not required to trigger DMA request.
}

/* START TIM1 OPERATION.
 * @param:	None.
 * @return:	None.
 */
void TIM1_Start(void) {
	// Enable DMA request trigger.
	TIM1 -> CCER |= (0b1 << 0); // Enable input capture (CC1E='1').
	TIM1 -> DIER |= (0b1 << 9); // CC1DE='1'.
}

/* STOP TIM1 OPERATION.
 * @param:	None.
 * @return:	None.
 */
void TIM1_Stop(void) {
	// Disable DMA request trigger.
	TIM1 -> DIER &= ~(0b1 << 9); // CC3DE='0'.
	TIM1 -> CCER &= ~(0b1 << 0); // Disable input capture (CC1E='0').
}

/* DISABLE TIM1 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void TIM1_Disable(void) {
	// Disable TIM1 peripheral.
	TIM1 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM1 -> CNT = 0;
	RCC -> APB2ENR &= ~(0b1 << 0); // TIM1EN='0'.
}

/* CONFIGURE TIM2_CH3/4 IN INPUT CAPTURE MODE TO TRIGGER DMA1 STREAM 1/6 REQUESTS FROM UPP_DSTART.
 * @param:	None.
 * @return:	None.
 */
void TIM2_Init(void) {
	// Configure DSTART GPIOs.
	//GPIO_Configure(&GPIO_CC1260_DSTART_I, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_DOWN);
	//GPIO_Configure(&GPIO_CC1260_DSTART_Q, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_DOWN);
	// Enable peripheral clock.
	RCC -> APB1ENR |= (0b1 << 0); // TIM2EN='1'.
	// Reset timer before configuration.
	TIM2 -> CR1 &= ~(0b1 << 0); // Disable TIM2 (CEN='0').
	TIM2 -> CNT = 0; // Reset counter.
	TIM2 -> CCER &= 0xFFFFEEFF; // Disable input captures (CC3E='0' and CC4E='0').
	// Configure timer in input capture mode.
	TIM2 -> CCMR2 |= (0b01 << 8) | (0b01 << 0); // Input mode, IC3/4 mapped on TI3/4 (CC3S='01' and CC4S='01').
	TIM2 -> CCER &= 0xFFFF55FF; // Rising edge capture (CC3P='0', CC3NP='0', CC4P='0' and CC4NP='0').
	// Generate event to update registers.
	TIM2 -> EGR |= (0b1 << 0); // UG='1'.
	TIM2 -> SR &= 0xFFFFE1A0; // Clear all flags.
	// Note: enabling the counter is not required to trigger DMA requests.
}

/* START TIM2 OPERATION.
 * @param:	None.
 * @return:	None.
 */
void TIM2_Start(void) {
	// Enable DMA request triggers.
	TIM2 -> CCER |= (0b1 << 12) | (0b1 << 8); // Enable input capture (CC3E='1' and CC4E='1').
	TIM2 -> DIER |= (0b11 << 11); // CC3DE='1' and CC4DE='1'.
}

/* STOP TIM2 OPERATION.
 * @param:	None.
 * @return:	None.
 */
void TIM2_Stop(void) {
	// Disable DMA request trigger.
	TIM2 -> DIER &= ~(0b11 << 11); // CC3DE='0' and CC4DE='0'.
	TIM2 -> CCER &= 0xFFFFEEFF; // Disable input captures (CC3E='0' and CC4E='0').
}

/* DISABLE TIM1 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void TIM2_Disable(void) {
	// Disable TIM2 peripheral.
	TIM2 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM2 -> CNT = 0;
	RCC -> APB1ENR &= ~(0b1 << 0); // TIM2EN='0'.
}

/* CONFIGURE TIM8_CH1 IN INPUT CAPTURE MODE TO TRIGGER DAC CONVERSION FROM UPP_DSTART.
 * @param:	None.
 * @return:	None.
 */
void TIM8_Init(void) {
	// Configure DCLK GPIO.
	//GPIO_Configure(&GPIO_CC1260_DSTART_DAC, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_DOWN);
	// Enable peripheral clock.
	RCC -> APB2ENR |= (0b1 << 1); // TIM8EN='1'.
	// Reset timer before configuration.
	TIM8 -> CR1 &= ~(0b1 << 0); // Disable TIM8 (CEN='0').
	TIM8 -> CNT = 0; // Reset counter.
	TIM8 -> CCER &= ~(0b1 << 0); // Disable input capture (CC1E='0').
	// Configure timer in input capture mode.
	TIM8 -> CCMR1 |= (0b01 << 0); // Input mode, IC1 mapped on TI1 (CC1S='01').
	TIM8 -> CCER |= (0b001 << 1); // Falling edge capture (CC1P='1' and CC1NP='0').
	TIM8 -> CR2 |= (0b0011 << 20) | (0b011 << 4); // TRGO mapped on CC1IF (MMS2='0011' and MMS='011').
	// Generate event to update registers.
	TIM8 -> EGR |= (0b1 << 0); // UG='1'.
	TIM8 -> SR &= 0xFFFFE1A0; // Clear all flags.
	// Note: enabling the counter is not required to trigger DMA request.
}

/* START TIM8 OPERATION.
 * @param:	None.
 * @return:	None.
 */
void TIM8_Start(void) {
	// Enable trigger.
	TIM8 -> CCER |= (0b1 << 0); // Enable input capture (CC1E='1').
}

/* STOP TIM8 OPERATION.
 * @param:	None.
 * @return:	None.
 */
void TIM8_Stop(void) {
	// Disable trigger.
	TIM8 -> CCER &= ~(0b1 << 0); // Disable input capture (CC1E='0').
}

/* DISABLE TIM8 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void TIM8_Disable(void) {
	// Disable TIM1 peripheral.
	TIM8 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM8 -> CNT = 0;
	RCC -> APB2ENR &= ~(0b1 << 1); // TIM1EN='0'.
}
