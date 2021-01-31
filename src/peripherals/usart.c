/*
 * usart.c
 *
 *  Created on: 7 dec. 2019
 *      Author: Ludo
 */

#include "usart.h"

#include "gpio.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "rcc_reg.h"
#include "usart_reg.h"

/*** USART local macros ***/

// Baud rate.
#define USART_BAUD_RATE 		9600
// Buffer sizes.
#define USART_TX_BUFFER_SIZE	32
#define USART_RX_BUFFER_SIZE	32

/*** USART local structures ***/

typedef struct {
	unsigned char tx_buf[USART_TX_BUFFER_SIZE]; 	// Transmit buffer
	unsigned int tx_read_idx; 						// Reading index in TX buffer.
	unsigned int tx_write_idx; 						// Writing index in TX buffer.
} USART_Context;

/*** USART local global variables ***/

static USART_Context usart1_ctx;

/*** USART local functions ***/

/* USART INTERRUPT HANDLER
 * @param:	None.
 * @return:	None.
 */
void USART1_IRQHandler(void) {
	// TX.
	if (((USART1 -> ISR) & (0b1 << 7)) != 0) { // TXE='1'.
		if ((usart1_ctx.tx_read_idx) != (usart1_ctx.tx_write_idx)) {
			// Send byte.
			USART1 -> TDR = (usart1_ctx.tx_buf)[usart1_ctx.tx_read_idx];
			// Increment read index.
			usart1_ctx.tx_read_idx++;
			if (usart1_ctx.tx_read_idx == USART_TX_BUFFER_SIZE) {
				usart1_ctx.tx_read_idx = 0;
			}
		}
		else {
			// No more bytes.
			USART1 -> CR1 &= ~(0b1 << 7); // TXEIE = '0'.
		}
	}
	// RX.
	if (((USART1 -> ISR) & (0b1 << 5)) != 0) { // RXNE='1'.
		// Get and store new byte into RX buffer.
		unsigned char rx_byte = USART1 -> RDR;
	}
	if (((USART1 -> ISR) & (0b1 << 3)) != 0) { // ORE='1'.
		// Clear flag.
		USART1 -> ICR |= (0b1 << 3);
	}
}

/* APPEND A NEW BYTE TO TX BUFFER AND MANAGE INDEX ROLL-OVER.
 * @param newbyte:		Byte to store in buffer.
 * @return:				None.
 */
static void USART1_FillTxBuffer(unsigned char new_tx_byte) {
	(usart1_ctx.tx_buf)[usart1_ctx.tx_write_idx] = new_tx_byte;
	// Increment index and manage roll-over.
	usart1_ctx.tx_write_idx++;
	if (usart1_ctx.tx_write_idx == USART_TX_BUFFER_SIZE) {
		usart1_ctx.tx_write_idx = 0;
	}
}

/* CONVERTS A 4-BIT WORD TO THE ASCII CODE OF THE CORRESPONDING HEXADECIMAL CHARACTER.
 * @param n:	The word to converts.
 * @return:		The results of conversion.
 */
static char USARTx_HexaToAscii(unsigned char hexa_value) {
	char hexa_ascii = 0;
	if (hexa_value <= 15) {
		hexa_ascii = (hexa_value <= 9 ? (char) (hexa_value + '0') : (char) (hexa_value + ('A' - 10)));
	}
	return hexa_ascii;
}

/* COMPUTE A POWER A 10.
 * @param power:	The desired power.
 * @return result:	Result of computation.
 */
static unsigned int USARTx_Pow10(unsigned char power) {
	unsigned int result = 0;
	unsigned int pow10_buf[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	if (power <= 9) {
		result = pow10_buf[power];
	}
	return result;
}

/*** USART functions ***/

/* CONFIGURE USART PERIPHERAL.
 * @param:	None.
 * @return: None.
 */
void USART1_Init(void) {
	// Init context.
	unsigned int i = 0;
	for (i=0 ; i<USART_TX_BUFFER_SIZE ; i++) (usart1_ctx.tx_buf)[i] = 0;
	usart1_ctx.tx_read_idx = 0;
	usart1_ctx.tx_write_idx = 0;
	// Enable peripheral clock.
	RCC -> APB2ENR |= (0b1 << 4); // USART1EN='1'.
	// Configure TX and RX GPIOs.
	GPIO_Configure(&GPIO_USART1_TX, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_USART1_RX, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Configure peripheral.
	USART2 -> CR3 |= (0b1 << 12); // No overrun detection (OVRDIS='1').
	USART1 -> BRR = (RCC_GetPclk2Khz() * 1000) / (USART_BAUD_RATE); // USART1 is clocked by PCLK2.
	// Enable transmitter and receiver.
	USART1 -> CR1 |= (0b1 << 5) | (0b11 << 2); // TE='1', RE='1' and RXNEIE='1'.
	// Set interrupt priority.
	NVIC_SetPriority(NVIC_IT_USART1, 6);
	// Enable peripheral.
	USART1 -> CR1 |= (0b1 << 0); // UE='1'.
}

/* SEND A VALUE THROUGH USART.
 * @param tx_value:		Value to print.
 * @param format:		Display format (see USART_Format enumeration in usart.h).
 * @param print_prexix	Print '0b' or '0x' prefix if non zero.
 * @return: 			None.
 */
void USART1_SendValue(unsigned int tx_value, USART_Format format, unsigned char print_prefix) {
	// Disable interrupt.
	NVIC_DisableInterrupt(NVIC_IT_USART1);
	// Local variables.
	unsigned char first_non_zero_found = 0;
	unsigned int idx;
	unsigned char current_value = 0;
	unsigned int current_power = 0;
	unsigned int previous_decade = 0;
	// Fill TX buffer according to format.
	switch (format) {
	case USART_FORMAT_BINARY:
		if (print_prefix != 0) {
			// Print "0b" prefix.
			USART1_FillTxBuffer('0');
			USART1_FillTxBuffer('b');
		}
		// Maximum 32 bits.
		for (idx=31 ; idx>=0 ; idx--) {
			if (tx_value & (0b1 << idx)) {
				USART1_FillTxBuffer('1'); // = '1'.
				first_non_zero_found = 1;
			}
			else {
				if ((first_non_zero_found != 0) || (idx == 0)) {
					USART1_FillTxBuffer('0'); // = '0'.
				}
			}
			if (idx == 0) {
				break;
			}
		}
		break;
	case USART_FORMAT_HEXADECIMAL:
		if (print_prefix != 0) {
			// Print "0x" prefix.
			USART1_FillTxBuffer('0');
			USART1_FillTxBuffer('x');
		}
		// Maximum 4 bytes.
		for (idx=3 ; idx>=0 ; idx--) {
			current_value = (tx_value & (0xFF << (8*idx))) >> (8*idx);
			if (current_value != 0) {
				first_non_zero_found = 1;
			}
			if ((first_non_zero_found != 0) || (idx == 0)) {
				USART1_FillTxBuffer(USARTx_HexaToAscii((current_value & 0xF0) >> 4));
				USART1_FillTxBuffer(USARTx_HexaToAscii(current_value & 0x0F));
			}
			if (idx == 0) {
				break;
			}
		}
		break;
	case USART_FORMAT_DECIMAL:
		// Maximum 10 digits.
		for (idx=9 ; idx>=0 ; idx--) {
			current_power = USARTx_Pow10(idx);
			current_value = (tx_value - previous_decade) / current_power;
			previous_decade += current_value * current_power;
			if (current_value != 0) {
				first_non_zero_found = 1;
			}
			if ((first_non_zero_found != 0) || (idx == 0)) {
				USART1_FillTxBuffer(current_value + '0');
			}
			if (idx == 0) {
				break;
			}
		}
		break;
	case USART_FORMAT_ASCII:
		// Raw byte.
		if (tx_value <= 0xFF) {
			USART1_FillTxBuffer(tx_value);
		}
		break;
	}
	// Enable interrupt.
	USART1 -> CR1 |= (0b1 << 7); // (TXEIE = '1').
	NVIC_EnableInterrupt(NVIC_IT_USART1);
}
