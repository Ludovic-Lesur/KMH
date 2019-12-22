/*
 * flash.c
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludovic
 */

#include "flash.h"

#include "flash_reg.h"
#include "rcc.h"

/*** FLASH functions ***/

/* INIT FLASH LATENCY ACCORDING TO SYSTEM CLOCK.
 * @param:	None.
 * @return:	None.
 */
void FLASH_Init(void) {

	/* Get latency (for 3.3V power supply) */
	unsigned char wait_states = 0;
	if (RCC_GetSysclkKhz() <= 30000) {
		wait_states = 0;
	}
	else if ((RCC_GetSysclkKhz() <= 60000)) {
		wait_states = 1;
	}
	else if ((RCC_GetSysclkKhz() <= 90000)) {
		wait_states = 2;
	}
	else if ((RCC_GetSysclkKhz() <= 120000)) {
		wait_states = 3;
	}
	else if ((RCC_GetSysclkKhz() <= 150000)) {
		wait_states = 4;
	}
	else if ((RCC_GetSysclkKhz() <= 180000)) {
		wait_states = 5;
	}
	else if ((RCC_GetSysclkKhz() <= 210000)) {
		wait_states = 6;
	}
	else {
		wait_states = 7;
	}

	/* Set latency */
	FLASH -> ACR &= ~(0b1111 << 0);
	FLASH -> ACR |= (wait_states << 0);
}


