/*
 * bl_led_counter.h
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef BL_BL_LED_COUNTER_H_
#define BL_BL_LED_COUNTER_H_

#include "drv_seven_segment.h"
#include "drv_sysclock.h"
#include "drv_button.h"
#include "drv_timer.h"


/**
 * Initial function for led counter.
 */
void blLedCounterInit();

/**
 * Function for one step counter display.
 */
void blLedCounterRun();


#endif /* BL_BL_LED_COUNTER_H_ */
