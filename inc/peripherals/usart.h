/*
 * usart.h
 *
 *  Created on: 7 dec. 2019
 *      Author: Ludo
 */

#ifndef USART_H
#define USART_H

/*** USART structures ***/

// Display format.
typedef enum {
	USART_FORMAT_BINARY,
	USART_FORMAT_HEXADECIMAL,
	USART_FORMAT_DECIMAL,
	USART_FORMAT_ASCII
} USART_Format;

/*** USART functions ***/

void USART1_Init(void);
void USART1_SendValue(unsigned int tx_value, USART_Format format, unsigned char print_prefix);

#endif /* _USART_H */
