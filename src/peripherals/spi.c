/*
 * spi.c
 *
 *  Created on: 21 dec. 2019
 *      Author: Ludo
 */

#include "spi.h"

#include "gpio.h"
#include "lptim.h"
#include "mapping.h"
#include "rcc_reg.h"
#include "spi_reg.h"

/*** SPI local macros ***/

#define SPI_ACCESS_TIMEOUT_COUNT	1000000

/*** SPI functions ***/

/* CONFIGURE SPI3.
 * @param:	None.
 * @return:	None.
 */
void SPI3_Init(void) {
	// Enable peripheral clock.
	RCC -> APB1ENR |= (0b1 << 15); // SPI3EN='1'.
	// Configure power enable pin.
	GPIO_Configure(&GPIO_RF_POWER_ENABLE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Write(&GPIO_RF_POWER_ENABLE, 0);
	// Configure NSS, SCK, MISO and MOSI (first as high impedance).
	GPIO_Configure(&GPIO_SPI3_SCK, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_SPI3_MOSI, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_SPI3_MISO, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_CS, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	// Configure peripheral.
	SPI3 -> CR1 &= 0xFFFF0000; // Disable peripheral before configuration (SPE='0').
	SPI3 -> CR1 |= (0b1 << 2); // Master mode (MSTR='1').
	SPI3 -> CR1 |= (0b010 << 3); // Baud rate = PCLK1 / 8 = 1.2MHz.
	SPI3 -> CR1 &= ~(0b11 << 0); // CPOL='0' and CPHA='0'.
	SPI3 -> CR2 &= 0xFFFF8000;
	SPI3 -> CR2 |= (0b0111 << 8); // 8-bits format (DS='0111').
	SPI3 -> CR2 |= (0b1 << 2); // Enable output (SSOE='1').
	SPI3 -> CR2 |= (0b1 << 12); // RXNE set as soon as 8-bits has been received (FRXTH='1').
	// Enable peripheral.
	SPI3 -> CR1 |= (0b1 << 6); // SPE='1'.
}

/* ENABLE SPI3 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void SPI3_Enable(void) {
	// Enable SPI3 peripheral.
	RCC -> APB1ENR |= (0b1 << 15); // SPI3EN='1'.
	SPI3 -> CR1 |= (0b1 << 6);
	// Configure power enable pins.
	GPIO_Configure(&GPIO_RF_POWER_ENABLE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Write(&GPIO_RF_POWER_ENABLE, 0);
}

/* DISABLE SPI3 PERIPHERAL.
 * @param:	None.
 * @return:	None.
 */
void SPI3_Disable(void) {
	// Disable power control pin.
	GPIO_Configure(&GPIO_RF_POWER_ENABLE, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Disable SPI3 peripheral.
	SPI3 -> CR1 &= ~(0b1 << 6);
	// Disable peripheral clock.
	RCC -> APB1ENR &= ~(0b1 << 15); // SPI3EN='0'.
}

/* SWITCH ALL SPI3 SLAVES ON.
 * @param:	None.
 * @return:	None.
 */
void SPI3_PowerOn(void) {
	// Turn CC1260 on.
	GPIO_Write(&GPIO_RF_POWER_ENABLE, 1);
	// Wait for power-on.
	LPTIM1_DelayMilliseconds(50);
	// Enable GPIOs.
	GPIO_Configure(&GPIO_SPI3_SCK, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_SPI3_MOSI, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_SPI3_MISO, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	GPIO_Write(&GPIO_CC1260_CS, 1); // CS high (idle state).
	GPIO_Configure(&GPIO_CC1260_CS, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	// Wait for power-on.
	LPTIM1_DelayMilliseconds(50);
}

/* SWITCH ALL SPI3 SLAVES OFF.
 * @param:	None.
 * @return:	None.
 */
void SPI3_PowerOff(void) {
	// Turn CC1260 off.
	GPIO_Write(&GPIO_RF_POWER_ENABLE, 0);
	GPIO_Write(&GPIO_CC1260_CS, 0); // CS low (to avoid powering slaves via SPI bus).
	// Disable SPI alternate function.
	GPIO_Configure(&GPIO_SPI3_SCK, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_SPI3_MOSI, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_SPI3_MISO, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_CS, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	// Wait for power-off.
	LPTIM1_DelayMilliseconds(100);
}

/* SEND A BYTE THROUGH SPI3.
 * @param tx_data:	Data to send (8-bits).
 * @return:			1 in case of success, 0 in case of failure.
 */
unsigned char SPI3_WriteByte(unsigned char tx_data) {
	// Send data.
	*((volatile unsigned char*) &(SPI3 -> DR)) = tx_data;
	// Wait for transmission to complete.
	unsigned int loop_count = 0;
	while ((((SPI3 -> SR) & (0b1 << 1)) == 0) || (((SPI3 -> SR) & (0b1 << 7)) != 0)) {
		// Wait for TXE='1' and BSY='0' or timeout.
		loop_count++;
		if (loop_count > SPI_ACCESS_TIMEOUT_COUNT) return 0;
	}
	return 1;
}

/* READ A BYTE FROM SPI3.
 * @param rx_data:	Pointer to byte that will contain the data to read (8-bits).
 * @return:			1 in case of success, 0 in case of failure.
 */
unsigned char SPI3_ReadByte(unsigned char tx_data, unsigned char* rx_data) {
	// Dummy read to DR to clear RXNE flag.
	unsigned int loop_count = 0;
	while (((SPI3 -> SR) & (0b1 << 0)) != 0) {
		(*rx_data) = *((volatile unsigned char*) &(SPI3 -> DR));
		// Wait for RXNE='0' or timeout.
		loop_count++;
		if (loop_count > SPI_ACCESS_TIMEOUT_COUNT) return 0;
	}
	// Send dummy data on MOSI to generate clock.
	*((volatile unsigned char*) &(SPI3 -> DR)) = tx_data;
	// Wait for incoming data.
	loop_count = 0;
	while (((SPI3 -> SR) & (0b1 << 0)) == 0) {
		// Wait for RXNE='1' or timeout.
		loop_count++;
		if (loop_count > SPI_ACCESS_TIMEOUT_COUNT) return 0;
	}
	(*rx_data) = *((volatile unsigned char*) &(SPI3 -> DR));
	// Wait for reception to complete.
	loop_count = 0;
	while ((((SPI3 -> SR) & (0b1 << 0)) != 0) || (((SPI3 -> SR) & (0b1 << 7)) != 0)) {
		// Wait for RXNE='0' and BSY='0' or timeout.
		loop_count++;
		if (loop_count > SPI_ACCESS_TIMEOUT_COUNT) return 0;
	}
	return 1;
}
