/*
 * dma.c
 *
 *  Created on: 24 dec. 2019
 *      Author: Ludo
 */

#include "dma.h"

#include "dma_reg.h"
#include "gpio_reg.h"
#include "rcc_reg.h"

/*** DMA local macros ***/

#define DMA_TIMEOUT_COUNT	10000

/*** DMA functions ***/

/* CONFIGURE DMA2 STREAM1 FOR I/Q DATA TRANSFER FROM/TO CC1260.
 * @param:	None.
 * @return:	None.
 */
void DMA2_STR1_Init(void) {
	// Enable peripheral clock.
	RCC -> AHB1ENR |= (0b1 << 22); // DMA2EN='1'.
	// Disable DMA stream before configuration.
	DMA2 -> S1CR &= ~(0b1 << 0);
	// Wait for EN bit to be cleared.
	unsigned int loop_count = 0;
	while (((DMA2 -> S1CR) & (0b1 << 0)) != 0) {
		loop_count++;
		if (loop_count > DMA_TIMEOUT_COUNT) break;
	}
	// Clear all flags.
	DMA2 -> LIFCR |= 0x0F7D0F7D;
	DMA2 -> HIFCR |= 0x0F7D0F7D;
	// Channel selection.
	DMA2 -> S1CR |= (0b0110 << 25); // Request mapping: DMA2 stream 1 channel 6 = TIM1_CH1.
	// Transfer settings.
	DMA2 -> S1FCR |= (0b1 << 2); // Disable direct mode to enable FIFO (8 to 16bits transfer).
	DMA2 -> S1CR |= (0b01 << 13); // Memory size = 16bits (MSIZE='01').
	DMA2 -> S1CR |= (0b1 << 10); // Memory pointer incremented according to MSIZE.
	DMA2 -> S1CR &= ~(0b11 << 11); // Peripheral size = 8bits (PSIZE='00').
	DMA2 -> S1CR &= ~(0b1 << 9); // Peripheral address fixed (PINC='0').
	DMA2 -> S1CR |= (0b1 << 18); // Double-buffer mode.
	DMA2 -> S1CR |= (0b1 << 8); // Circular mode enabled (already set by hardware with double-buffer mode).
}

/* SET DMA2 STREAM 1 PERIPHERAL ADDRESS.
 * @param periph_addr:	Peripheral address.
 * @return:				None.
 */
void DMA2_STR1_SetPeripheralAddress(unsigned int periph_addr) {
	// Set peripheral address.
	DMA2 -> S1PAR = periph_addr;
}

/* SET DMA2 STREAM 1 MEMORY ADDRESS.
 * @param buffer1_addr:	Buffer 1 address.
 * @param buffer2_addr:	BUffer 2 address.
 * @return:				None.
 */
void DMA2_STR1_SetMemoryAddress(unsigned int buffer1_addr, unsigned int buffer2_addr, unsigned int buffers_size) {
	// Set memory address.
	DMA2 -> S1M0AR = buffer1_addr;
	DMA2 -> S1M1AR = buffer2_addr;
	// Number of data to transfer.
	DMA2 -> S1NDTR = (2 * buffers_size); // Twice the length because 2 8-bits transfers are required for 1 16-bits data.
}

/* SET DMA TRANSFER DIRECTION.
 * @param dma_direction:	See enumeration in dma.h.
 * @return:					None.
 */
void DMA2_STR1_SetDirection(DMA_Direction dma_direction) {
	// Set direction.
	DMA2 -> S1CR &= ~(0b11 << 6);
	DMA2 -> S1CR |= (dma_direction << 6);
}

/* START DMA2 STREAM 1 TRANSFER.
 * @param:	None.
 * @return:	None.
 */
void DMA2_STR1_Start(void) {
	// Enable stream.
	DMA2 -> S1CR |= (0b1 << 0);
}

/* STOP DMA2 STREAM 1 TRANSFER.
 * @param:	None.
 * @return:	None.
 */
void DMA2_STR1_Stop(void) {
	// Disable stream.
	DMA2 -> S1CR &= ~(0b1 << 0);
}
