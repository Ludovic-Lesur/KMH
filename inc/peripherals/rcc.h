/*
 * rcc.h
 *
 *  Created on: 7 dec. 2019
 *      Author: Ludovic
 */

#ifndef RCC_H
#define RCC_H

/*** RCC functions ***/

void RCC_Init(void);
void RCC_EnableLsi(void);
unsigned int RCC_GetSysclkKhz(void);
unsigned int RCC_GetPclk1Khz(void);
unsigned int RCC_GetPclk2Khz(void);

#endif /* RCC_H */
