/*
 * fault.c
 *
 *  Created on: 4 apr. 2021
 *      Author: Ludo
 */

#include "scb_reg.h"

/* NON MASKABLE INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void NMI_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/* HARD FAULT INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void HardFault_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/* MEMORY FAULT INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void MemManage_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/* BUS FAULT INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void BusFault_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/* USAGE FAULT INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void UsageFault_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}
