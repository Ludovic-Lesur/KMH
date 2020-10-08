/*
 * cc1260.h
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludo
 */

#ifndef CC1260_H
#define CC1260_H

#include "cc1260_config.h"

/*** CC1260 functions ***/

// Common.
void CC1260_Init(void);
void CC1260_Reset(void);
void CC1260_Unreset(void);
void CC1260_SetConfig(const CC1260_RegisterSetting* cc1260_config, unsigned char cc1260_config_size);
void CC1260_SetFrequency(unsigned int freq_hz);
// RX.
void CC1260_SetRxSampleRate(unsigned int rx_sample_rate_hz);
void CC1260_StartRxIq(unsigned int buffer1_addr, unsigned int buffer2_addr, unsigned int buffers_size);

#endif /* CC1260_H */
