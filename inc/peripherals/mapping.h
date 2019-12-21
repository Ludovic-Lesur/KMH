/*
 * mapping.h
 *
 *  Created on: 7 dec. 2019
 *      Author: Ludovic
 */

#ifndef MAPPING_H
#define MAPPING_H

#include "gpio.h"
#include "gpio_reg.h"

/*** MCU pins mapping ***/

// Analog.
static const GPIO GPIO_ANALOG_POWER_ENABLE =	(GPIO) {GPIOA, 0, 2, 0};
static const GPIO GPIO_DAC1 =					(GPIO) {GPIOA, 0, 4, 0};
static const GPIO GPIO_DAC2 =					(GPIO) {GPIOA, 0, 5, 0};
static const GPIO GPIO_AIN6 =					(GPIO) {GPIOA, 0, 6, 0};
static const GPIO GPIO_AIN7 =					(GPIO) {GPIOA, 0, 7, 0};
static const GPIO GPIO_AIN14 =					(GPIO) {GPIOC, 2, 4, 0};
// RF.
static const GPIO GPIO_RF_POWER_ENABLE =		(GPIO) {GPIOC, 2, 8, 0};
// CC1260 SPI.
static const GPIO GPIO_CC1260_CS =				(GPIO) {GPIOA, 0, 11, 0};
static const GPIO GPIO_SPI3_SCK =				(GPIO) {GPIOC, 2, 10, 6};	// AF6 = SPI3_SCK.
static const GPIO GPIO_SPI3_MISO =				(GPIO) {GPIOC, 2, 11, 6};	// AF6 = SPI3_MISO.
static const GPIO GPIO_SPI3_MOSI =				(GPIO) {GPIOC, 2, 12, 6};	// AF6 = SPI3_MOSI.
// CC1260 GPIOs.
static const GPIO GPIO_CC1260_RESET =			(GPIO) {GPIOG, 6, 14, 0};
static const GPIO GPIO_CC1260_GPIO0 =			(GPIO) {GPIOA, 0, 12, 0};
static const GPIO GPIO_CC1260_GPIO2 =			(GPIO) {GPIOG, 6, 12, 0};
static const GPIO GPIO_CC1260_GPIO3 =			(GPIO) {GPIOG, 6, 13, 0};
// CC1260 uPP interface.
static const GPIO GPIO_CC1260_DATA0 =			(GPIO) {GPIOD, 3, 0, 0};
static const GPIO GPIO_CC1260_DATA1 =			(GPIO) {GPIOD, 3, 1, 0};
static const GPIO GPIO_CC1260_DATA2 =			(GPIO) {GPIOD, 3, 2, 0};
static const GPIO GPIO_CC1260_DATA3 =			(GPIO) {GPIOD, 3, 3, 0};
static const GPIO GPIO_CC1260_DATA4 =			(GPIO) {GPIOD, 3, 4, 0};
static const GPIO GPIO_CC1260_DATA5 =			(GPIO) {GPIOD, 3, 5, 0};
static const GPIO GPIO_CC1260_DATA6 =			(GPIO) {GPIOD, 3, 6, 0};
static const GPIO GPIO_CC1260_DATA7 =			(GPIO) {GPIOD, 3, 7, 0};
static const GPIO GPIO_CC1260_DWAIT =			(GPIO) {GPIOD, 3, 8, 0};
static const GPIO GPIO_CC1260_DSTART =			(GPIO) {GPIOD, 3, 9, 0};
static const GPIO GPIO_CC1260_DENABLE =			(GPIO) {GPIOD, 3, 10, 0};
static const GPIO GPIO_CC1260_DCLK =			(GPIO) {GPIOD, 3, 14, 0};
static const GPIO GPIO_CC1260_UPP_DIR =			(GPIO) {GPIOG, 6, 11, 0};
// Screen GPIOs.
static const GPIO GPIO_SCREEN_IO0 =				(GPIO) {GPIOE, 4, 9, 0};
static const GPIO GPIO_SCREEN_IO1 =				(GPIO) {GPIOE, 4, 8, 0};
static const GPIO GPIO_SCREEN_IO2 =				(GPIO) {GPIOE, 4, 7, 0};
static const GPIO GPIO_SCREEN_IO3 =				(GPIO) {GPIOG, 6, 1, 0};
// Screen SPI.
static const GPIO GPIO_SPI4_NSS =				(GPIO) {GPIOE, 4, 11, 5};	// AF5 = SPI4_NSS.
static const GPIO GPIO_SPI4_SCK =				(GPIO) {GPIOE, 4, 12, 5};	// AF5 = SPI4_SCK.
static const GPIO GPIO_SPI4_MISO =				(GPIO) {GPIOE, 4, 13, 5};	// AF5 = SPI4_MISO.
static const GPIO GPIO_SPI4_MOSI =				(GPIO) {GPIOE, 4, 14, 5};	// AF5 = SPI4_MOSI.
// SAI1.
static const GPIO GPIO_SAI1_SD_B =				(GPIO) {GPIOF, 5, 6, 6};	// AF6 = SAI1_SD_B.
static const GPIO GPIO_SAI1_MCLK_B =			(GPIO) {GPIOF, 5, 7, 6};	// AF6 = SAI1_SCK_B.
static const GPIO GPIO_SAI1_FS_B =				(GPIO) {GPIOF, 5, 8, 6};	// AF6 = SAI1_FS_B.
static const GPIO GPIO_SAI1_SCK_B =				(GPIO) {GPIOF, 5, 9, 6};	// AF6 = SAI1_SCK_B.
// SAI2.
static const GPIO GPIO_SAI2_SD_A =				(GPIO) {GPIOD, 3, 11, 10};	// AF10 = SAI2_SD_A.
static const GPIO GPIO_SAI2_FS_A =				(GPIO) {GPIOD, 3, 12, 10};	// AF10 = SAI2_FS_A.
static const GPIO GPIO_SAI2_SCK_A =				(GPIO) {GPIOD, 3, 13, 10};	// AF10 = SAI2_SCK_A.
// USB OTG.
static const GPIO GPIO_OTG_HS_ID = 				(GPIO) {GPIOB, 1, 12, 12};	// AF12 = OTG_HS_ID.
static const GPIO GPIO_OTG_HS_VBUS =			(GPIO) {GPIOB, 1, 13, 12};	// AF12 = OTG_HS_VBUS.
static const GPIO GPIO_OTG_HS_DM =				(GPIO) {GPIOB, 1, 14, 12};	// AF12 = OTG_HS_DM.
static const GPIO GPIO_OTG_HS_DP =				(GPIO) {GPIOB, 1, 15, 12};	// AF12 = OTG_HS_DP.
// Debug serial link.
static const GPIO GPIO_USART1_TX =				(GPIO) {GPIOA, 0, 9, 7};	// AF7 = USART1 TX.
static const GPIO GPIO_USART1_RX =				(GPIO) {GPIOA, 0, 10, 7}; 	// AF7 = USART1 RX.
// LEDs.
static const GPIO GPIO_LED_RED =				(GPIO) {GPIOB, 1, 0, 0};
static const GPIO GPIO_LED_GREEN = 				(GPIO) {GPIOB, 1, 1, 0};
static const GPIO GPIO_LED_BLUE =				(GPIO) {GPIOB, 1, 2, 0};
// Buttons.
static const GPIO GPIO_BP2 =					(GPIO) {GPIOF, 5, 11, 0};
static const GPIO GPIO_BP3 =					(GPIO) {GPIOF, 5, 12, 0};

#endif /* MAPPING_H */
