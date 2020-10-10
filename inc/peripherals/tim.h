/*
 * tim.h
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludo
 */

#ifndef TIM_H
#define TIM_H

/*** TIM functions ***/

// TIM1: CH1 input capture mode to trigger DMA2 stream 1 requests (uPP DCLK edges).
void TIM1_Init(void);
void TIM1_Start(void);
void TIM1_Stop(void);
void TIM1_Disable(void);

#endif /* TIM_H */
