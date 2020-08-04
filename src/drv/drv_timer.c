/*
 * drv_timer.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_timer.h"

const static drvTimer_def timerDefenition = {
        halBaseTimer6,
        TIMER_MS_PRESCALER,
        TIMER_PERIOD,
        halBaseTimerClockDivision1
};

void drvTimerInit(drvTimer_struct* timerStruct, drvTimerCallbackType callback) {
    halInitBaseTimerStruct initStruct;
    initStruct.halBaseTimerPeriod = timerDefenition.period;
    initStruct.halBaseTimerPrescaler = timerDefenition.prescaller;
    initStruct.halBaseTimersClockDivision = timerDefenition.clockDiv;

    halBaseTimerInit((halTimer_struct*) timerStruct, &initStruct);

    halBaseTimerSetTimerCallback((halTimer_struct*) timerStruct, callback);
}

void drvTimerDeinit(drvTimer_struct* timerStruct) {
    halBaseTimerDeinit((halTimer_struct*) timerStruct);
}

void drvTimerStart(drvTimer_struct* timerStruct) {
    halBaseTimerStart((halTimer_struct*) timerStruct);
}

void drvTimerStop(drvTimer_struct* timerStruct) {
    halBaseTimerStop((halTimer_struct*) timerStruct);
}
