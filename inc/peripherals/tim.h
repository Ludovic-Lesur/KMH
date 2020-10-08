/*
 * tim.h
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludo
 */

#ifndef TIM_H
#define TIM_H

/*** TIM functions ***/

// TIM3: milliseconds counter.
void TIM3_Init(void);
void TIM3_Start(void);
void TIM3_Stop(void);
void TIM3_Disable(void);

// TIM2: seconds counter (slave of TIM3).
void TIM2_Init(void);
void TIM2_Start(void);
void TIM2_Stop(void);
void TIM2_Disable(void);
volatile unsigned int TIM2_GetSeconds(void);

// TIM4: CH3 input capture mode to trigger DMA requests (uPP DCLK edges).
void TIM4_Init(void);
void TIM4_Disable(void);

#endif /* TIM_H */
