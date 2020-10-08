/*
 * dac.c
 *
 *  Created on: 23 dec. 2019
 *      Author: Ludo
 */

#include "dac.h"

#include "dac_reg.h"
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
	GPIO_Configure(&GPIO_DAC1, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_DAC2, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Configure peripheral.
	DAC -> CR |= (0b1 << 0); // EN1='1'.
	DAC -> CR |= (0b1 << 16); // EN2='1'.
	// Enable output buffer and disable trigger.
	DAC -> CR &= ~(0b11 << 1); // BOFF1='0' and TEN1='0'.
	DAC -> CR &= ~(0b11 << 17); // BOFF2='0' and TEN2='0'.
}

/* SET DAC OUTPUT VOLTAGE.
 * @param channel: 	DAC channel (0 for 'Channel1' on PA4, 1 for 'Channel2' on PA5).
 * @param voltage: 	Output voltage expressed in mV (between 0 and VCC_MV).
 * @return: 		None.
 */
void DAC_SetVoltageMv(unsigned char channel, unsigned int voltage_mv) {
	// Ensure new voltage is reachable.
	unsigned int real_voltage_mv = voltage_mv;
	if (real_voltage_mv < 0) {
		real_voltage_mv = 0;
	}
	if (real_voltage_mv > DAC_VREF_MV) {
		real_voltage_mv = DAC_VREF_MV;
	}
	if (channel == 0) {
		DAC -> DHR12R1 = (DAC_FULL_SCALE * real_voltage_mv) / (DAC_VREF_MV);
	}
	else {
		DAC -> DHR12R2 = (DAC_FULL_SCALE * real_voltage_mv) / (DAC_VREF_MV);
	}
}
