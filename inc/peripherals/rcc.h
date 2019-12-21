/*
 * rcc.h
 *
 *  Created on: 7 dec. 2019
 *      Author: Ludovic
 */

#ifndef RCC_H
#define RCC_H

/*** RCC macros ***/

// If defined, output clocks on MCO1 and MCO2 pins.
//#define RCC_OUTPUT_CLOCK

// System clocks frequency in kHz.
#define RCC_SYSCLK_KHZ	16000
#define RCC_PCLK1_KHZ	4000
#define RCC_PCLK2_KHZ	4000

/*** RCC functions ***/

void RCC_Init(void);

#endif /* RCC_H */
