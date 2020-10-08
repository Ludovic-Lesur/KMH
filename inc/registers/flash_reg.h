/*
 * flash_reg.h
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludo
 */

#ifndef FLASH_REG_H
#define FLASH_REG_H

/*** FLASH registers ***/

typedef struct {
	volatile unsigned int ACR;		// FLASH access control register.
	volatile unsigned int KEYR;		// FLASH key register.
	volatile unsigned int OPTKEYR;	// FLASH option bytes unlock key register.
	volatile unsigned int SR;		// FLASH status register.
	volatile unsigned int CR;		// FLASH control register.
	volatile unsigned int OPTCR;	// FLASH option control register.
	volatile unsigned int OPTCR1;	// FLASH option control register.
	volatile unsigned int OPTCR2;	// FLASH option control register.
} FLASH_BaseAddress;

/*** FLASH registers base address ***/

#define FLASH	((FLASH_BaseAddress*) ((unsigned int) 0x40023C00))

#endif /* FLASH_REG_H */
