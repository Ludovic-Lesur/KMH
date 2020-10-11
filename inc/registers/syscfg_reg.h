/*
 * syscfg_reg.h
 *
 *  Created on: 18 june 2018
 *      Author: Ludo
 */

#ifndef SYSCFG_REG_H_
#define SYSCFG_REG_H_

/*** SYSCFG registers ***/

typedef struct {
	volatile unsigned int MEMRMP;    	// SYSCFG memory remap register.
	volatile unsigned int PMC;    		// SYSCFG peripheral mode configuration register.
	volatile unsigned int EXTICR[4];  	// SYSCFG external interrupt configuration registers 1-4.
	volatile unsigned int RESERVED[2];
	volatile unsigned int CMPCR;   		// SYSCFG control and status register.
} SYSCFG_BaseAddress;

/*** SYSCFG base address ***/

#define SYSCFG	((SYSCFG_BaseAddress*) ((unsigned int) 0x40013800))

#endif /* SYSCFG_REG_H_ */
