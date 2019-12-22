/*
 * led.c
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludovic
 */

#include "led.h"

#include "gpio.h"
#include "mapping.h"

/*** LED functions ***/

/* CONFIGURE RGB LED.
 * @param:	None.
 * @return:	None.
 */
void LED_Init(void) {

	/* Configure LED pins as output */
	GPIO_Configure(&GPIO_LED_RED, GPIO_MODE_OUTPUT, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LED_GREEN, GPIO_MODE_OUTPUT, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LED_BLUE, GPIO_MODE_OUTPUT, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);

	/* Turn LED off by default */
	LED_SetColor(LED_OFF);
}

/* SET LED COLOR.
 * @param led_color:	LED color (see enumeration in led.h).
 * @return:				None.
 */
void LED_SetColor(LED_Color led_color) {

	/* Update pins */
	switch (led_color) {
	case LED_OFF:
		GPIO_Write(&GPIO_LED_RED, 1);
		GPIO_Write(&GPIO_LED_GREEN, 1);
		GPIO_Write(&GPIO_LED_BLUE, 1);
		break;
	case LED_RED:
		GPIO_Write(&GPIO_LED_RED, 0);
		GPIO_Write(&GPIO_LED_GREEN, 1);
		GPIO_Write(&GPIO_LED_BLUE, 1);
		break;
	case LED_GREEN:
		GPIO_Write(&GPIO_LED_RED, 1);
		GPIO_Write(&GPIO_LED_GREEN, 0);
		GPIO_Write(&GPIO_LED_BLUE, 1);
		break;
	case LED_BLUE:
		GPIO_Write(&GPIO_LED_RED, 1);
		GPIO_Write(&GPIO_LED_GREEN, 1);
		GPIO_Write(&GPIO_LED_BLUE, 0);
		break;
	case LED_YELLOW:
		GPIO_Write(&GPIO_LED_RED, 0);
		GPIO_Write(&GPIO_LED_GREEN, 0);
		GPIO_Write(&GPIO_LED_BLUE, 1);
		break;
	case LED_CYAN:
		GPIO_Write(&GPIO_LED_RED, 1);
		GPIO_Write(&GPIO_LED_GREEN, 0);
		GPIO_Write(&GPIO_LED_BLUE, 0);
		break;
	case LED_PURPLE:
		GPIO_Write(&GPIO_LED_RED, 0);
		GPIO_Write(&GPIO_LED_GREEN, 1);
		GPIO_Write(&GPIO_LED_BLUE, 0);
		break;
	case LED_WHITE:
		GPIO_Write(&GPIO_LED_RED, 0);
		GPIO_Write(&GPIO_LED_GREEN, 0);
		GPIO_Write(&GPIO_LED_BLUE, 0);
		break;
	default:
		break;
	}
}
