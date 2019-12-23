/*
 * spi.h
 *
 *  Created on: 21 dec. 2019
 *      Author: Ludo
 */

#ifndef SPI_H
#define SPI_H

/*** SPI functions ***/

void SPI3_Init(void);
void SPI3_Enable(void);
void SPI3_Disable(void);
void SPI3_PowerOn(void);
void SPI3_PowerOff(void);
unsigned char SPI3_WriteByte(unsigned char tx_data);
unsigned char SPI3_ReadByte(unsigned char tx_data, unsigned char* rx_data);

#endif /* SPI_H */
