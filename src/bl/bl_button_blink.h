/*
 * bl_button_toggle.h
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef BL_BL_BUTTON_BLINK_H_
#define BL_BL_BUTTON_BLINK_H_

#include "drv_button.h"
#include "drv_led.h"

/**
 * Initial function for button blink.
 */
void blButtonBlinkInit();

/**
 * Function for one step of button blink check.
 */
void blBlinkRun();

#endif /* BL_BL_BUTTON_BLINK_H_ */
