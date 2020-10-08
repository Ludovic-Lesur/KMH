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
#include "tim.h"

/*** DMA local macros ***/

#define DMA_TIMEOUT_SECONDS		3

/*** DMA functions ***/

/* CONFIGURE DMA1 FOR I/Q DATA TRANSFER.
 * @param:	None.
 * @return:	None.
 */
void DMA1_Init(void) {
	// Enable peripheral clock.
	RCC -> AHB1ENR |= (0b1 << 21); // DMA1EN='1'.
	// Disable DMA stream before configuration.
	DMA1 -> S7CR &= ~(0b1 << 0);
	// Wait for EN bit to be cleared.
	unsigned int loop_start_time = TIM2_GetSeconds();
	while (((DMA1 -> S7CR) & (0b1 << 0)) != 0) {
		if (TIM2_GetSeconds() > (loop_start_time + DMA_TIMEOUT_SECONDS)) break;
	}
	// Clear all flags.
	DMA1 -> LIFCR |= 0x0F7D0F7D;
	DMA1 -> HIFCR |= 0x0F7D0F7D;
	// Channel selection.
	DMA1 -> S7CR |= (0b0010 << 25); // Request mapping: DMA1 channel 2 stream 7 = TIM4_CH3.
	// Transfer settings.
	DMA1 -> S7FCR |= (0b1 << 2); // Disable direct mode to enable FIFO (8 to 16bits transfer).
	DMA1 -> S7CR |= (0b01 << 23); // MBURST=INCR4.
	DMA1 -> S7FCR |= (0b01 << 0); // FIFO threshold = 1/2 full.
	DMA1 -> S7CR |= (0b01 << 13); // Memory size = 16bits (MSIZE='01').
	DMA1 -> S7CR |= (0b1 << 10); // Memory pointer incremented according to MSIZE.
	DMA1 -> S7CR &= ~(0b11 << 11); // Peripheral size = 8bits (PSIZE='00').
	DMA1 -> S7CR &= ~(0b1 << 9); // Peripheral address fixed.
	DMA1 -> S7CR |= (0b1 << 18); // Double-buffer mode.
	DMA1 -> S7CR |= (0b1 << 8); // Circular mode enabled (already set by hardware with double-buffer mode).
}

/* SET DMA STREAM PERIPHERAL ADDRESS.
 * @param periph_addr:	Peripheral address.
 * @return:				None.
 */
void DMA1_SetPeripheralAddress(unsigned int periph_addr) {
	// Set peripheral address.
	DMA1 -> S7PAR = periph_addr;
}

/* SET DMA STREAM MEMORY ADDRESS.
 * @param buffer1_addr:	Buffer 1 address.
 * @param buffer2_addr:	BUffer 2 address.
 * @return:				None.
 */
void DMA1_SetMemoryAddress(unsigned int buffer1_addr, unsigned int buffer2_addr, unsigned int buffers_size) {
	// Set memory address.
	DMA1 -> S7M0AR = buffer1_addr;
	DMA1 -> S7M1AR = buffer2_addr;
	// Number of data to transfer.
	DMA1 -> S7NDTR = buffers_size; // Each I/Q sample is sent on 4 bytes.
}

/* SET DMA TRANSFER DIRECTION.
 * @param dma_direction:	See enumeration in dma.h.
 * @return:					None.
 */
void DMA1_SetDirection(DMA_Direction dma_direction) {
	// Set direction.
	DMA1 -> S7CR &= ~(0b11 << 6);
	DMA1 -> S7CR |= (dma_direction << 6);
}

/* START DMA TRANSFER.
 * @param:	None.
 * @return:	None.
 */
void DMA1_Start(void) {
	// Enable stream.
	DMA1 -> S7CR |= (0b1 << 0);
}

/* STOP DMA TRANSFER.
 * @param:	None.
 * @return:	None.
 */
void DMA1_Stop(void) {
	// Disable stream.
	DMA1 -> S7CR &= ~(0b1 << 0);
}
