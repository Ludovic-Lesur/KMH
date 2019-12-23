/*
 * cc1260.c
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludovic
 */

#include "cc1260.h"

#include "cc1260_config.h"
#include "cc1260_reg.h"
#include "gpio.h"
#include "lptim.h"
#include "mapping.h"
#include "spi.h"

/*** CC1260 local macros ***/

#define CC1260_XOSC_FREQUENCY_HZ	38400000
#define CC1260_LO_DIVIDER			4

/*** CC1260 local functions ***/

/* CC1260 COMMAND ACCESS.
 * @param command:	Command to sent.
 * @return:			None.
 */
void CC1260_SendCommand(unsigned char command) {

	/* Write access sequence */
	GPIO_Write(&GPIO_CC1260_CS, 0); // Falling edge on CS pin.
	SPI3_WriteByte(command & 0x3F);
	GPIO_Write(&GPIO_CC1260_CS, 1); // Set CS pin.
}

/* CC1260 SINGLE ACCESS WRITE FUNCTION.
 * @param addr:		Register address.
 * @param value:	Value to write in register.
 * @return:			None.
 */
void CC1260_WriteRegister(unsigned short addr, unsigned char value) {

	/* Build SPI frames */
	unsigned char cc1260_header_byte = (addr >> 8) & 0x3F; // '0 0 A13 A12 A11 A10 A9 A8' for a write access.
	unsigned char cc1260_address_byte = (addr & 0x00FF);  // 'A7 A6 A5 A4 A3 A2 A1 A0'.

	/* Write access sequence */
	GPIO_Write(&GPIO_CC1260_CS, 0); // Falling edge on CS pin.
	SPI3_WriteByte(cc1260_header_byte);
	SPI3_WriteByte(cc1260_address_byte);
	SPI3_WriteByte(value);
	GPIO_Write(&GPIO_CC1260_CS, 1); // Set CS pin.
}

/* CC1260 SINGLE ACCESS READ FUNCTION.
 * @param addr:		Register address.
 * @param value:	Pointer to byte that will contain the register Value.
 * @return:			None.
 */
void CC1260_ReadRegister(unsigned short addr, unsigned char* value) {

	/* Build SPI frames */
	unsigned char cc1260_header_byte = ((addr >> 8) & 0x3F) | 0x80; // '1 0 A13 A12 A11 A10 A9 A8' for a write access.
	unsigned char cc1260_address_byte = (addr & 0x00FF);  // 'A7 A6 A5 A4 A3 A2 A1 A0'.

	/* Write access sequence */
	GPIO_Write(&GPIO_CC1260_CS, 0); // Falling edge on CS pin.
	SPI3_WriteByte(cc1260_header_byte);
	SPI3_WriteByte(cc1260_address_byte);
	SPI3_ReadByte(0xFF, value);
	GPIO_Write(&GPIO_CC1260_CS, 1); // Set CS pin.
}

/*** CC1260 functions ***/

/* INIT CC1260 GPIOs.
 * @param:	None.
 * @return:	None.
 */
void CC1260_Init(void) {

	/* Configure reset pin */
	GPIO_Configure(&GPIO_CC1260_RESET, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Write(&GPIO_CC1260_RESET, 0);

	/* Configure GPIOs (input by default) */
	GPIO_Configure(&GPIO_CC1260_GPIO0, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_GPIO2, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_GPIO3, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);

	/* Configure uPP DWAIT pin (always input) and uPP direction */
	GPIO_Configure(&GPIO_CC1260_DWAIT, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_UPP_DIR, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
}

/* RESET CC1260 CHIP.
 * @param:	None.
 * @return:	None.
 */
void CC1260_Reset(void) {
	// Low level on reset pin.
	GPIO_Write(&GPIO_CC1260_RESET, 0);
}

/* UNRESET CC1260 CHIP.
 * @param:	None.
 * @return:	None.
 */
void CC1260_Unreset(void) {
	// High level on reset pin.
	GPIO_Write(&GPIO_CC1260_RESET, 1);
	LPTIM1_DelayMilliseconds(100);
}

/* SET A CC1260 REGISTERS CONFIGURATION.
 * @param cc1260_config:		Registers setting array.
 * @param cc1260_config_size:	Size of the input array.
 */
void CC1260_SetConfig(const CC1260_RegisterSetting* cc1260_config, unsigned char cc1260_config_size) {
	// Write all registers.
	unsigned char addr_idx = 0;
	for (addr_idx=0 ; addr_idx<cc1260_config_size ; addr_idx++) {
		CC1260_WriteRegister(cc1260_config[addr_idx].cc1260_reg_addr, cc1260_config[addr_idx].cc1260_reg_value);
	}
}

/* SET CC1260 RF FREQUENCY.
 * @param freq_hz:	Frequency to set in Hz.
 * @return:			None.
 */
void CC1260_SetFrequency(unsigned int freq_hz) {

	/* Compute register value */
	// LO divider = 4 (see p.53 of CC1260 programming manual: FSD_BAND_SELECT = 0x02).
	// fRF = fVCO / LO divider = fVCO / 4.
	// fVCO = (FREQ / 2^(16)) * XOSC (assuming FREQOFF=0).
	unsigned long long freq_reg_value = ((unsigned long long) (freq_hz)) << 16;
	freq_reg_value *= (unsigned long long) CC1260_LO_DIVIDER;
	freq_reg_value /= (unsigned long long) CC1260_XOSC_FREQUENCY_HZ;

	/* Program frequency */
	CC1260_WriteRegister(CC1260_REG_FREQ2, ((freq_reg_value >> 16) & 0xFF));
	CC1260_WriteRegister(CC1260_REG_FREQ1, ((freq_reg_value >> 8) & 0xFF));
	CC1260_WriteRegister(CC1260_REG_FREQ0, ((freq_reg_value >> 0) & 0xFF));
}

/* START RX I/Q OPERATION.
 * @param:	None.
 * @return:	None.
 */
void CC1260_StartRxIq(void) {

	/* Configure uPP pins of the MCU as input */
	GPIO_Configure(&GPIO_CC1260_DATA0, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DATA1, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DATA2, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DATA3, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DATA4, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DATA5, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DATA6, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DATA7, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DSTART, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DENABLE, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	GPIO_Configure(&GPIO_CC1260_DCLK, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN);

	/* Set uPP direction */
	GPIO_Write(&GPIO_CC1260_UPP_DIR, 0);

	/* Configure uPP pins of the CC1260 to drive zero (except DWAIT) */
	CC1260_WriteRegister(CC1260_REG_UPP_CTRL2, 0x15);
	CC1260_WriteRegister(CC1260_REG_UPP_CTRL1, 0x55);
	CC1260_WriteRegister(CC1260_REG_UPP_CTRL0, 0x55);

	/* Start RX */
	CC1260_SendCommand(CC1260_COMMAND_SRX);
}