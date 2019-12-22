/*
 * rcc.c
 *
 *  Created on: 7 dec. 2019
 *      Author: Ludovic
 */

#include "rcc.h"

#include "rcc_reg.h"

/*** RCC local macros ***/

#define RCC_HSI_FREQUENCY_KHZ	16000
#define RCC_TCXO_FREQUENCY_KHZ	38400
#define RCC_TIMEOUT_COUNT		10

/*** RCC local structures ***/

typedef struct {
	unsigned int rcc_sysclk_khz;
	unsigned int rcc_pclk1_khz;
	unsigned int rcc_pclk2_khz;
} RCC_Context;

/*** RCC local variables ***/

static RCC_Context rcc_ctx;

/*** RCC local functions ***/

/* PERFORM A MANUAL DELAY.
 * @param:	None.
 * @return:	None.
 */
void RCC_Delay(void) {
	unsigned int j = 0;
	unsigned int loop_count = 100000; // Value for 100ms.
	for (j=0 ; j<loop_count ; j++) {
		// Poll a bit always read as '0' (required to avoid compiler optimization).
		if (((RCC -> CR) & (0b1 << 24)) != 0) {
			break;
		}
	}
}

/*** RCC functions ***/

/* CONFIGURE MCU CLOCK TREE.
 * @param: None.
 * @return: None.
 */
void RCC_Init(void) {

	/* Peripherals clock prescalers */
	// HPRE = 1 -> HCLK = SYSCLK = 16 or 38.4MHz (max 216).
	// PPRE1 = 4 -> PCLK1 = HCLK / 4 = 4 or 9.6MHz (max 54).
	// PPRE2 = 4 -> PCLK2 = HCLK / 4 = 4 or 9.6MHz (max 108).
	RCC -> CFGR &= 0xFFE0030F; // Reset bits 4-7 and 10-15 + HPRE='0000' (1).
	RCC -> CFGR |= (0b101 << 10) | (0b101 << 13); // PPRE1='101' (4) and PPRE2='101' (4).
	RCC -> DCKCFGR1 &= ~(0b1 << 24); // Timers clock is 2*PLCKx (PPRE1 and PPRE2 != 1).

	/* Reset value */
	rcc_ctx.rcc_sysclk_khz = RCC_HSI_FREQUENCY_KHZ;

	/* Start HSE */
	RCC -> CR |= (0b1 << 18); // HSEBYP = '1'.
	RCC -> CR |= (0b1 << 16); // HSEON = '1'.
	unsigned int count = 0;
	// Wait for HSERDY='1'.
	while ((((RCC -> CR) & (0b1 << 17)) == 0) && (count < RCC_TIMEOUT_COUNT)) {
		RCC_Delay();
		count++;
	}

	/* Check timeout */
	if (count < RCC_TIMEOUT_COUNT) {
		// Use HSE as system clock.
		RCC -> CFGR |= (0b01 << 0); // SW='01'.
		// Wait for clock switch (SWS='01').
		count = 0;
		while ((((RCC -> CFGR) & (0b11 << 2)) != (0b01 << 2)) && (count < RCC_TIMEOUT_COUNT)) {
			RCC_Delay();
			count++;
		}

		/* Check timeout */
		if (count < RCC_TIMEOUT_COUNT) {
			// Disable HSI.
			RCC -> CR &= ~(0b1 << 0); // HSI16ON='0'.
			// Update system clock frequency.
			rcc_ctx.rcc_sysclk_khz = RCC_TCXO_FREQUENCY_KHZ;
		}
		else {
			// Use HSI as system clock.
			RCC -> CFGR &= ~(0b11 << 0); // SW='00'.
			// Turn HSE off.
			RCC -> CR &= ~(0b1 << 16); // HSEON = '0'.
		}
	}
	else {
		// Turn HSE off.
		RCC -> CR &= ~(0b1 << 16); // HSEON = '0'.
	}

	/* Update peripherals clock frequency */
	rcc_ctx.rcc_pclk1_khz = (rcc_ctx.rcc_sysclk_khz / 4);
	rcc_ctx.rcc_pclk2_khz = (rcc_ctx.rcc_sysclk_khz / 4);
}

/* ENABLE LSI CLOCK.
 * @param:	None.
 * @return:	None.
 */
void RCC_EnableLsi(void) {

	/* Enable LSI */
	RCC -> CSR |= (0b1 << 0); // LSION='1'.
	unsigned int count = 0;
	// Wait for LSIRDY='1'.
	while ((((RCC -> CSR) & (0b1 << 1)) == 0) && (count < RCC_TIMEOUT_COUNT)) {
		RCC_Delay();
		count++;
	}
}

/* GET CURRENT SYSTEM CLOCK FREQUENCY.
 * @param:					None.
 * @return rcc_sysclk_khz:	Current system clock frequency in kHz.
 */
unsigned int RCC_GetSysclkKhz(void) {
	return (rcc_ctx.rcc_sysclk_khz);
}

/* GET CURRENT PCLK1 FREQUENCY.
 * @param:					None.
 * @return rcc_pclk2_khz:	Current PCLK1 frequency in kHz.
 */
unsigned int RCC_GetPclk1Khz(void) {
	return (rcc_ctx.rcc_pclk1_khz);
}

/* GET CURRENT PCLK2 FREQUENCY.
 * @param:					None.
 * @return rcc_pclk1_khz:	Current PCLK2 frequency in kHz.
 */
unsigned int RCC_GetPclk2Khz(void) {
	return (rcc_ctx.rcc_pclk2_khz);
}
