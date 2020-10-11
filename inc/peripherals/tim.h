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

// TIM2: CH3/4 input capture mode to trigger DMA1 stream 1/6 requests (uPP DSTART edges).
void TIM2_Init(void);
void TIM2_Start(void);
void TIM2_Stop(void);
void TIM2_Disable(void);

// TIM8: CH1 input capture mode to trigger DAC conversions (uPP DSTART edges).
void TIM8_Init(void);
void TIM8_Start(void);
void TIM8_Stop(void);
void TIM8_Disable(void);

#endif /* TIM_H */
