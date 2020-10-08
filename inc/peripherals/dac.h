/*
 * dac.h
 *
 *  Created on: 23 dec. 2019
 *      Author: Ludo
 */

#ifndef DAC_H
#define DAC_H

/*** DAC functions ***/

void DAC_Init(void);
void DAC_SetVoltageMv(unsigned char channel, unsigned int voltage_mv);

#endif /* PERIPHERALS_DAC_H_ */
