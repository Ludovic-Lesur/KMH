/*
 * dac.c
 *
 *  Created on: 23 dec. 2019
 *      Author: Ludo
 */

#include "dac.h"

#include "dac_reg.h"
#include "exti.h"
#include "gpio.h"
#include "mapping.h"
#include "rcc_reg.h"

/*** DAC local macros ***/

#define DAC_FULL_SCALE	4095
#define DAC_VREF_MV		2048

/*** DAC functions ***/

/* INIT DAC MODULE.
 * @param:	None.
 * @return:	None.
 */
void DAC_Init(void) {
	// Enable peripheral clock.
	RCC -> APB1ENR |= (0b1 << 29); // DACEN='1'.
	// Configure analog GPIOs.
	GPIO_Configure(&GPIO_DAC1, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_DAC2, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	// Configure trigger GPIO (DSTART).
	GPIO_Configure(&GPIO_CC1260_DSTART, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_DOWN);
	EXTI_ConfigureGpio(&GPIO_CC1260_DSTART, EXTI_TRIGGER_RISING_EDGE);
	EXTI_ClearAllFlags();
	// Select EXTI line 9 as trigger.
	DAC -> CR |= (0b110 << 19) | (0b110 << 3); // TSELX='110'.
}

/* START DAC CONVERSION FROM TRIGGER AND DMA.
 * @param:	None.
 * @return:	None.
 */
void DAC_Start(void) {
	// Enable trigger on both channel.
	DAC -> CR |= (0b1 << 18) | (0b1 << 2);
	// Enable DMA request on channel 1 since only 1 request is required.
	DAC -> CR |= (0b1 << 12);
	// Enable channels.
	DAC -> CR |= (0b1 << 16) | (0b1 << 0); // EN1='1' and EN2='1'.
}

/* STOP DAC CONVERSION.
 * @param:	None.
 * @return:	None.
 */
void DAC_Stop(void) {
	// Disable trigger, DMA and channels.
	DAC -> CR &= 0xEFFAEFFA;
}
