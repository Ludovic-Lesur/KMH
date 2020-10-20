/*
 * cc1260.h
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludo
 */

#ifndef CC1260_H
#define CC1260_H

#include "cc1260_config.h"

/*** CC1260 structures ***/

typedef enum {
	CC1260_RX_ANALOG_BW_300KHZ = 0b00,
	CC1260_RX_ANALOG_BW_600KHZ = 0b01,
	CC1260_RX_ANALOG_BW_1000KHZ = 0b10,
	CC1260_RX_ANALOG_BW_1500MHZ = 0b11,
} CC1260_RxAnalogFilterBandwidth;

/*** CC1260 functions ***/

// Common.
void CC1260_Init(void);
void CC1260_Reset(void);
void CC1260_Unreset(void);
void CC1260_SetConfig(const CC1260_RegisterSetting* cc1260_config, unsigned char cc1260_config_size);
void CC1260_SetFrequency(unsigned int freq_hz);
// RX.
void CC1260_SetRxSampleRate(unsigned int rx_sample_rate_hz);
void CC1260_SetRxAnalogFilter(CC1260_RxAnalogFilterBandwidth rx_ana_bw);
void CC1260_SetLnaAtten(unsigned char atten_db);
void CC1260_StartRxIq(void);

#endif /* CC1260_H */
