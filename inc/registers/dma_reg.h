/*
 * dma_reg.h
 *
 *  Created on: 24 dec. 2019
 *      Author: Ludo
 */

#ifndef DMA_REG_H
#define DMA_REG_H

/*** DMA registers ***/

typedef struct {
	volatile unsigned int LISR;		// DMA low interrupt status register.
	volatile unsigned int HISR;		// DMA high interrupt status register.
	volatile unsigned int LIFCR;	// DMA low interrupt flag clear register.
	volatile unsigned int HIFCR;	// DMA high interrupt flag clear register.
	volatile unsigned int S0CR;		// DMA stream 0 configuration register.
	volatile unsigned int S0NDTR;	// DMA stream 0 number of data register.
	volatile unsigned int S0PAR;	// DMA stream 0 peripheral address register.
	volatile unsigned int S0M0AR;	// DMA stream 0 memory 0 address register.
	volatile unsigned int S0M1AR;	// DMA stream 0 memory 1 address register.
	volatile unsigned int S0FCR	;	// DMA stream 0 FIFO control register.
	volatile unsigned int S1CR;		// DMA stream 1 configuration register.
	volatile unsigned int S1NDTR;	// DMA stream 1 number of data register.
	volatile unsigned int S1PAR;	// DMA stream 1 peripheral address register.
	volatile unsigned int S1M0AR;	// DMA stream 1 memory 0 address register.
	volatile unsigned int S10M1AR;	// DMA stream 1 memory 1 address register.
	volatile unsigned int S1FCR	;	// DMA stream 1 FIFO control register.
	volatile unsigned int S2CR;		// DMA stream 2 configuration register.
	volatile unsigned int S2NDTR;	// DMA stream 2 number of data register.
	volatile unsigned int S2PAR;	// DMA stream 2 peripheral address register.
	volatile unsigned int S2M0AR;	// DMA stream 2 memory 0 address register.
	volatile unsigned int S2M1AR;	// DMA stream 2 memory 1 address register.
	volatile unsigned int S2FCR	;	// DMA stream 2 FIFO control register.
	volatile unsigned int S3CR;		// DMA stream 3 configuration register.
	volatile unsigned int S3NDTR;	// DMA stream 3 number of data register.
	volatile unsigned int S3PAR;	// DMA stream 3 peripheral address register.
	volatile unsigned int S3M0AR;	// DMA stream 3 memory 0 address register.
	volatile unsigned int S3M1AR;	// DMA stream 3 memory 1 address register.
	volatile unsigned int S3FCR	;	// DMA stream 3 FIFO control register.
	volatile unsigned int S4CR;		// DMA stream 4 configuration register.
	volatile unsigned int S4NDTR;	// DMA stream 4 number of data register.
	volatile unsigned int S4PAR;	// DMA stream 4 peripheral address register.
	volatile unsigned int S4M0AR;	// DMA stream 4 memory 0 address register.
	volatile unsigned int S4M1AR;	// DMA stream 4 memory 1 address register.
	volatile unsigned int S4FCR	;	// DMA stream 4 FIFO control register.
	volatile unsigned int S5CR;		// DMA stream 5 configuration register.
	volatile unsigned int S5NDTR;	// DMA stream 5 number of data register.
	volatile unsigned int S5PAR;	// DMA stream 5 peripheral address register.
	volatile unsigned int S5M0AR;	// DMA stream 5 memory 0 address register.
	volatile unsigned int S5M1AR;	// DMA stream 5 memory 1 address register.
	volatile unsigned int S5FCR	;	// DMA stream 5 FIFO control register.
	volatile unsigned int S6CR;		// DMA stream 6 configuration register.
	volatile unsigned int S6NDTR;	// DMA stream 6 number of data register.
	volatile unsigned int S6PAR;	// DMA stream 6 peripheral address register.
	volatile unsigned int S6M0AR;	// DMA stream 6 memory 0 address register.
	volatile unsigned int S6M1AR;	// DMA stream 6 memory 1 address register.
	volatile unsigned int S6FCR	;	// DMA stream 6 FIFO control register.
	volatile unsigned int S7CR;		// DMA stream 7 configuration register.
	volatile unsigned int S7NDTR;	// DMA stream 7 number of data register.
	volatile unsigned int S7PAR;	// DMA stream 7 peripheral address register.
	volatile unsigned int S7M0AR;	// DMA stream 7 memory 0 address register.
	volatile unsigned int S7M1AR;	// DMA stream 7 memory 1 address register.
	volatile unsigned int S7FCR	;	// DMA stream 7 FIFO control register.
} DMA_BaseAddress;

/*** DMA registers base address ***/

#define DMA1	((DMA_BaseAddress*) ((unsigned int) 0x40026000))
#define DMA2	((DMA_BaseAddress*) ((unsigned int) 0x40026400))

#endif /* DMA_REG_H */
