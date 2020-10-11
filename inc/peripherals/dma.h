/*
 * dma.h
 *
 *  Created on: 24 dec. 2019
 *      Author: Ludo
 */

#ifndef DMA_H
#define DMA_H

/*** DMA structures ***/

typedef enum {
	DMA_DIRECTION_PERIPHERAL_TO_MEMORY = 0b00,
	DMA_DIRECTION_MEMORY_TO_PERIPHERAL = 0b01,
	DMA_DIRECTION_MEMORY_TO_MEMORY = 0b10
} DMA_Direction;

/*** DMA functions ***/

// DMA1 performs buffer to DAC tranfer.
void DMA1_STR5_Init(void);
void DMA1_STR5_SetPeripheralAddress(unsigned int periph_addr);
void DMA1_STR5_SetMemoryAddress(unsigned int buf1_addr, unsigned int buf2_addr, unsigned int buffers_size);
void DMA1_STR5_SetDirection(DMA_Direction dma_direction);
void DMA1_STR5_Start(void);
void DMA1_STR5_Stop(void);

// DMA2 performs GPIOs to buffer transfer.
void DMA2_STR1_Init(void);
void DMA2_STR1_SetPeripheralAddress(unsigned int periph_addr);
void DMA2_STR1_SetMemoryAddress(unsigned int buffer1_addr, unsigned int buffer2_addr, unsigned int buffers_size);
void DMA2_STR1_SetDirection(DMA_Direction dma_direction);
void DMA2_STR1_Start(void);
void DMA2_STR1_Stop(void);

#endif /* DMA_H */
