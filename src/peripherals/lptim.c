/*
 * lptim.c
 *
 *  Created on: 29 apr. 2018
 *      Author: Ludo
 */

#include "lptim.h"

#include "lptim_reg.h"
#include "rcc.h"
#include "rcc_reg.h"
#include "tim.h"

/*** LPTIM local macros ***/

#define LPTIM_TIMEOUT_SECONDS	3

/*** LPTIM functions ***/

/* INIT LPTIM FOR DELAY OPERATION.
 * @param:	None.
 * @return:	None.
 */
void LPTIM1_Init(void) {
	// Select peripheral clock.
	RCC -> DCKCFGR2 &= ~(0b11 << 24); // LPTIMSEL='00' (PCLK1 clock selected).
	RCC -> APB1ENR |= (0b1 << 9); // LPTIM1EN='1'.
	// Configure peripheral.
	LPTIM1 -> CR &= ~(0b1 << 0); // Disable LPTIM1 (ENABLE='0'), needed to write CFGR.
	LPTIM1 -> CFGR |= (0b1 << 19); // Enable timeout.
	LPTIM1 -> CNT &= 0xFFFF0000; // Reset counter.
	LPTIM1 -> CR |= (0b1 << 0); // Enable LPTIM1 (ENABLE='1'), needed to write ARR.
	LPTIM1 -> ARR = RCC_GetPclk1Khz(); // Overflow period = 1ms.
	unsigned int loop_start_time = TIM2_GetSeconds();
	while (((LPTIM1 -> ISR) & (0b1 << 4)) == 0) {
		// Wait for ARROK='1' or timeout.
		if (TIM2_GetSeconds() > (loop_start_time + LPTIM_TIMEOUT_SECONDS)) break;
	}
	// Clear all flags.
	LPTIM1 -> ICR |= (0b1111111 << 0);
	// Disable peripheral by default.
	LPTIM1 -> CR &= ~(0b1 << 0); // Disable LPTIM1 (ENABLE='0').
}

/* ENABLE LPTIM1 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void LPTIM1_Enable(void) {
	// Enable timer clock.
	RCC -> APB1ENR |= (0b1 << 9); // LPTIM1EN='1'.
}

/* DISABLE LPTIM1 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void LPTIM1_Disable(void) {
	// Disable timer.
	LPTIM1 -> CR &= ~(0b1 << 0); // Disable LPTIM1 (ENABLE='0').
	LPTIM1 -> CNT = 0;
	RCC -> APB1ENR &= ~(0b1 << 9); // LPTIM1EN='0'.
}

/* DELAY FUNCTION.
 * @param delay_ms:	Number of milliseconds to wait.
 * @return:			None.
 */
void LPTIM1_DelayMilliseconds(unsigned int delay_ms) {
	// Enable timer.
	LPTIM1 -> CR |= (0b1 << 0); // Enable LPTIM1 (ENABLE='1').
	// Make as many overflows as required.
	unsigned int ms_count = 0;
	unsigned int loop_start_time = 0;
	unsigned char lptim_default = 0;
	for (ms_count=0 ; ms_count<delay_ms ; ms_count++) {
		// Start counter.
		loop_start_time = TIM2_GetSeconds();
		LPTIM1 -> CNT &= 0xFFFF0000;
		LPTIM1 -> CR |= (0b1 << 1); // SNGSTRT='1'.
		while (((LPTIM1 -> ISR) & (0b1 << 1)) == 0) {
			if (TIM2_GetSeconds() > (loop_start_time + LPTIM_TIMEOUT_SECONDS)) {
				lptim_default = 1;
				break;
			}
		}
		// Clear flag.
		LPTIM1 -> ICR |= (0b1 << 1);
		// Exit in case of timeout.
		if (lptim_default != 0) {
			break;
		}
	}
	// Disable timer.
	LPTIM1 -> CR &= ~(0b1 << 0); // Disable LPTIM1 (ENABLE='0').
}
