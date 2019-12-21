/*
 * rcc.c
 *
 *  Created on: 7 dec. 2019
 *      Author: Ludovic
 */

#include "rcc.h"

#include "rcc_reg.h"

/*** RCC functions ***/

/* CONFIGURE MCU CLOCK TREE.
 * @param: None.
 * @return: None.
 */
void RCC_Init(void) {

	/* Use HSI first for start-up  */
	RCC -> CR &= ~(0b1 << 16); // HSEON = '0'.
	RCC -> CR |= (0b1 << 0); // HSION='1'.
	// Wait for HSI to be stable.
	while (((RCC -> CR) & (0b1 << 1)) == 0); // HSIRDY='1'.
	// Select HSI as system clock.
	RCC -> CFGR &= ~(0b11 << 0); // SW='00'.
	// Wait for clock switch.
	while (((RCC -> CFGR) & (0b11 << 0)) != 0b00); // SWS='00'.

	/* Peripherals clock prescalers */
	// HPRE = 1 -> HCLK = SYSCLK = 16MHz (max 216).
	// PPRE1 = 4 -> PCLK1 = HCLK / 4 = 4MHz (max 54).
	// PPRE2 = 4 -> PCLK2 = HCLK / 4 = 4MHz (max 108).
	RCC -> CFGR &= 0xFFE0030F; // Reset bits 4-7 and 10-15 + HPRE='0000' (1).
	RCC -> CFGR |= (0b101 << 10) | (0b101 << 13); // PPRE1='101' (4) and PPRE2='101' (4).
	RCC -> DCKCFGR1 &= ~(0b1 << 24); // Timers clock is 2*PLCKx (PPRE1 and PPRE2 != 1).

#ifdef RCC_OUTPUT_CLOCK
	// Output HSI on MCO1 (PA8 as AF0) and SYSCLK on MCO2 (PC9 as AF0) with both prescalers = 4.
	RCC -> CFGR &= 0x369FFFFF;
	RCC -> CFGR |= 0x36000000;
#endif
}
