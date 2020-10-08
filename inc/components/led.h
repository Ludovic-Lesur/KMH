/*
 * led.h
 *
 *  Created on: 22 dec. 2019
 *      Author: Ludo
 */

#ifndef LED_H
#define LED_H

/*** LED structures ***/

typedef enum {
	LED_OFF,
	LED_RED,
	LED_GREEN,
	LED_BLUE,
	LED_YELLOW,
	LED_CYAN,
	LED_PURPLE,
	LED_WHITE,
} LED_Color;

/*** LED functions ***/

void LED_Init(void);
void LED_SetColor(LED_Color led_color);

#endif /* LED_H */
