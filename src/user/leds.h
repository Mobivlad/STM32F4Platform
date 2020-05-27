/*
 * leds.h
 *
 *  Created on: 26 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef USER_LEDS_H_
#define USER_LEDS_H_

#include "stm32f4xx.h"

#define PORT_LEDS GPIOD
#define PIN_RED GPIO_Pin_14
#define PIN_GREEN GPIO_Pin_12

void initLeds(void);
void onLEDs(u16);
void offLEDs(u16);
void toggleLEDs(u16);



#endif /* USER_LEDS_H_ */
