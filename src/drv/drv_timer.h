/*
 * drv_timer.h
 *
 *  Created on: 4 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_TIMER_H_
#define DRV_DRV_TIMER_H_

#include "hal_base_timer.h"

#define TIMER_MS_PRESCALER 41999
#define TIMER_PERIOD       1999

/// Callback function for timers
typedef void(*drvTimerCallbackType)(void);

/// Timers enumeration
typedef enum {
    drvTimer0,         /**< Timer 0 */
    drvTimer_Count
} drvTimers;

typedef struct {
    halBaseTimer                timer;

    uint16_t                    prescaller;
    uint16_t                    period;

    halBaseTimersClockDivision  clockDiv;
} drvTimer_def;

typedef struct {
    halTimer_struct timerStruct;
} drvTimer_struct;

void drvTimerInit(drvTimer_struct* timerStruct, drvTimerCallbackType callback);
void drvTimerDeinit(drvTimer_struct* timerStruct);

void drvTimerStart(drvTimer_struct* timerStruct);
void drvTimerStop(drvTimer_struct* timerStruct);

#endif /* DRV_DRV_TIMER_H_ */
