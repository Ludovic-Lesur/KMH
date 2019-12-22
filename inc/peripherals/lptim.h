/*
 * lptim.h
 *
 *  Created on: 21 dec. 2019
 *      Author: Ludo
 */

#ifndef LPTIM_H
#define LPTIM_H

/*** LPTIM functions ***/

void LPTIM1_Init(void);
void LPTIM1_Enable(void);
void LPTIM1_Disable(void);
void LPTIM1_DelayMilliseconds(unsigned int delay_ms);

#endif /* LPTIM_H */
