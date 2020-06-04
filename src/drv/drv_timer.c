/*
 * drv_timer.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_timer.h"

void drvTimerInit(drvTimers timer, uint16_t durationInMs){
    halInitBaseTimerStruct initStruct;
    initStruct.halBaseTimerPeriod = durationInMs;
    initStruct.halBaseTimerPrescaler = 20999;
    initStruct.halBaseTimersClockDivision = halBaseTimerClockDivision1;
    halBaseTimerInit(timer,&initStruct);
}

void drvTimerStart(drvTimers timer){
    halBaseTimerStart(timer);
}

void drvTimerStop(drvTimers timer){
    halBaseTimerStop(timer);
}

void drvTimerPause(drvTimers timer){
    halBaseTimerPause(timer);
}

uint32_t drvTimerGetTime(drvTimers timer){
    return halBaseTimerGetTime(timer);
}
