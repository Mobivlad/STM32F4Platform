/*
 * drv_timer.h
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_TIMER_H_
#define DRV_DRV_TIMER_H_

#include "hal_base_timer.h"

/// Callback function for timers
typedef void(*drvTimerCallbackType)(void);

/// Timers enumeration
typedef enum {
    drvTimer0 = halBaseTimer6,         /**< Timer 0 */
    drvTimer1 = halBaseTimer7,         /**< Timer 1 */
    drvTimersCount
} drvTimers;

/**
 * Initial function for timer.
 * @param timer value of drvTimers enumeration
 * @param durationInMs time between 2 timer actions
 * callback timer callback
 */
void drvTimerInit(drvTimers timer, uint16_t durationInMs, drvTimerCallbackType callback);

/**
 * Start timer function.
 * @param timer value of drvTimers enumeration
 */
void drvTimerStart(drvTimers timer);

/**
 * Stop timer function.
 * @param timer value of drvTimers enumeration
 */
void drvTimerStop(drvTimers timer);

#endif /* DRV_DRV_TIMER_H_ */
