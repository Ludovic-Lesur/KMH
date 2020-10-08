/*
 * flash.c
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludovic
 */

#include "flash.h"

#include "flash_reg.h"

/*** FLASH functions ***/

/* SET FLASH LATENCY.
 * @param:	None.
 * @return:	None.
 */
void FLASH_SetLatency(unsigned wait_states) {
	// Set latency.
	FLASH -> ACR &= ~(0b1111 << 0);
	FLASH -> ACR |= (wait_states << 0);
}
