/*
 * spi_reg.h
 *
 *  Created on: 21 dec. 2019
 *      Author: Ludo
 */

#ifndef SPI_REG_H
#define SPI_REG_H

/*** SPI registers ***/

typedef struct {
	volatile unsigned int CR1;    	// SPI control register 1.
	volatile unsigned int CR2;    	// SPI control register 2.
	volatile unsigned int SR;    	// SPI status register.
	volatile unsigned int DR;    	// SPI data register.
	volatile unsigned int CRCPR;    // SPI CRC polynomial register.
	volatile unsigned int RXCRCR;   // SPI RX CRC register.
	volatile unsigned int TXCRCR;   // SPI TX CRC register.
	volatile unsigned int I2SCFGR;	// SPI I2S configuration register.
	volatile unsigned int I2SPR;    // SPI I2S prescaler register.
} SPI_BaseAddress;

/*** SPI base addresses ***/

#define SPI1	((SPI_BaseAddress*) ((unsigned int) 0x40013000))
#define SPI2	((SPI_BaseAddress*) ((unsigned int) 0x40003800))
#define SPI3	((SPI_BaseAddress*) ((unsigned int) 0x40003C00))
#define SPI4	((SPI_BaseAddress*) ((unsigned int) 0x40013400))
#define SPI5	((SPI_BaseAddress*) ((unsigned int) 0x40015000))

#endif /* SPI_REG_H */
