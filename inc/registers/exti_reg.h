/*
 * exti_reg.h
 *
 *  Created on: 11 oct. 2020
 *      Author: Ludo
 */

#ifndef EXTI_REG_H_
#define EXTI_REG_H_

/*** EXTI registers ***/

typedef struct {
	volatile unsigned int IMR;    	// EXTI interrupt mask register.
	volatile unsigned int EMR;    	// EXTI event mask register.
	volatile unsigned int RTSR;    	// EXTI rising edge trigger selection register.
	volatile unsigned int FTSR;    	// EXTI falling edge trigger selection register.
	volatile unsigned int SWIER;    // EXTI software interrupt event register.
	volatile unsigned int PR;    	// EXTI pending register.
} EXTI_BaseAddress;

/*** EXTI base address ***/

#define EXTI	((EXTI_BaseAddress*) ((unsigned int) 0x40013C00))

#endif /* EXTI_REG_H_ */
