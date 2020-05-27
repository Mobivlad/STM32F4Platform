/*
 * button.h
 *
 *  Created on: 26 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef USER_BUTTON_H_
#define USER_BUTTON_H_

#include "stm32f4xx.h"

#define PORT_BUTTON GPIOA
#define PIN_BUTTON GPIO_Pin_0

void initButtons(void);

#endif /* USER_BUTTON_H_ */
