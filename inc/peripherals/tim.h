/*
 * tim.h
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludo
 */

#ifndef TIM_H
#define TIM_H

/*** TIM functions ***/

void TIM3_Init(void);
void TIM3_Start(void);
void TIM3_Stop(void);
void TIM3_Disable(void);

void TIM2_Init(void);
void TIM2_Start(void);
void TIM2_Stop(void);
void TIM2_Disable(void);
volatile unsigned int TIM2_GetSeconds(void);

#endif /* TIM_H */
