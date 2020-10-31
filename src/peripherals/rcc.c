/*
 * rcc.c
 *
 *  Created on: 7 dec. 2019
 *      Author: Ludovic
 */

#include "rcc.h"

#include "flash.h"
#include "rcc_reg.h"

/*** RCC local macros ***/

#define RCC_HSI_FREQUENCY_KHZ		16000
#define RCC_TCXO_FREQUENCY_KHZ		38400
#define RCC_PLLCLK_FREQUENCY_KHZ	160000
#define RCC_TIMEOUT_COUNT			10

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
static void RCC_Delay(void) {
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
	// Reset value.
	rcc_ctx.rcc_sysclk_khz = RCC_HSI_FREQUENCY_KHZ;
	rcc_ctx.rcc_pclk1_khz = rcc_ctx.rcc_sysclk_khz;
	rcc_ctx.rcc_pclk2_khz = rcc_ctx.rcc_sysclk_khz;
	// FLASH latency.
	FLASH_SetLatency(0);
#ifdef RCC_USE_MCO
	// Output SYSCLK / 5 on MCO2.
	RCC -> CFGR |= (0b00 << 30) | (0b111 << 27);
#endif
}

/* USE PLL AS SYSTEM CLOCK.
 * @param pll_use_hse:	Use HSI as source PLL source if 0, HSE otherwise.
 * @return:	None.
 */
void RCC_SwitchToPllclk(unsigned char pll_use_hse) {
	// Local variables.
	unsigned char local_pll_use_hse = ((pll_use_hse == 0) ? 0 : 1);
	unsigned int count = 0;
	// Start HSE if required.
	if (local_pll_use_hse != 0) {
		RCC -> CR |= (0b1 << 18); // HSEBYP = '1'.
		RCC -> CR |= (0b1 << 16); // HSEON = '1'.
		// Wait for HSERDY='1' or timeout.
		while ((((RCC -> CR) & (0b1 << 17)) == 0) && (count < RCC_TIMEOUT_COUNT)) {
			RCC_Delay();
			count++;
		}
		// Check timeout.
		if (count >= RCC_TIMEOUT_COUNT) {
			local_pll_use_hse = 0;
			RCC -> CR &= ~(0b1 << 16); // HSEON = '0'.
		}
	}
	// Disable main PLL before configure it.
	RCC -> CR &= ~(0b1 << 24); // PLLON='0'.
	count = 0;
	while ((((RCC -> CR) & (0b1 << 25)) != 0) && (count < RCC_TIMEOUT_COUNT)); // PLLRDY='0'.
	// Set PLL source.
	RCC -> PLLCFGR = 0;
	RCC -> PLLCFGR |= (local_pll_use_hse << 22);
	// Set PLL coefficients.
	if (local_pll_use_hse == 0) {
		// M=16 -> VCO input frequency = HSI/M = 16MHz/16 = 1MHz.
		// N=320 -> VCO output frequency = 320MHz.
		// P=2 -> PLL output frequency = 160MHz.
		// Q=8 -> USB/RNG frequency = 40MHz.
		RCC -> PLLCFGR |= (8 << 24) | (0 << 16) | (320 << 6) | (16 << 0);
	}
	else {
		// M=24 -> VCO input frequency = HSE/M = 38.4MHz/24 = 1.6MHz.
		// N=200 -> VCO output frequency = 320MHz.
		// P=2 ->  PLL output frequency = 160MHz.
		// Q=8 -> USB/RNG frequency = 40MHz.
		RCC -> PLLCFGR |= (8 << 24) | (0 << 16) | (200 << 6) | (24 << 0);
	}
	// Peripherals clock prescalers.
	// HPRE = 1 -> HCLK = SYSCLK = 160 (max 216).
	// PPRE1 = 4 -> PCLK1 = HCLK / 4 = 40MHz (max 54).
	// PPRE2 = 2 -> PCLK2 = HCLK / 2 = 80MHz (max 108).
	RCC -> CFGR &= 0xFFE0030F;
	RCC -> CFGR |= (0b101 << 10) | (0b100 << 13) | (0b0000 << 4);
	RCC -> DCKCFGR1 &= ~(0b1 << 24); // TIMxCLK = 2*PCLKx.
	// Enable main PLL.
	RCC -> CR |= (0b1 << 24); // PLLON='1'.
	count = 0;
	while ((((RCC -> CR) & (0b1 << 25)) == 0) && (count < RCC_TIMEOUT_COUNT)) ; // PLLRDY='1'.
	if (count < RCC_TIMEOUT_COUNT) {
		// Perform clock switch.
		FLASH_SetLatency(5);
		RCC -> CFGR |= (0b10 << 0); // SW='10'.
		// Wait for clock switch (SWS='10').
		count = 0;
		while ((((RCC -> CFGR) & (0b11 << 2)) != (0b10 << 2)) && (count < RCC_TIMEOUT_COUNT)) {
			RCC_Delay();
			count++;
		}
		// Update frequencies if success.
		if (count < RCC_TIMEOUT_COUNT) {
			rcc_ctx.rcc_sysclk_khz = RCC_PLLCLK_FREQUENCY_KHZ;
			rcc_ctx.rcc_pclk1_khz = (rcc_ctx.rcc_sysclk_khz / 4);
			rcc_ctx.rcc_pclk2_khz = (rcc_ctx.rcc_sysclk_khz / 2);
		}
		else {
			FLASH_SetLatency(0);
		}
	}
}

/* ENABLE LSI CLOCK.
 * @param:	None.
 * @return:	None.
 */
void RCC_EnableLsi(void) {
	// Enable LSI.
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
