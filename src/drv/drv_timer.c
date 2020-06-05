/*
 * drv_timer.c
 *
 *  Created on: 4 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_timer.h"

void drvTimerInit(drvTimers timer, uint16_t durationInMs, drvTimerCallbackType callback){
    halInitBaseTimerStruct initStruct;
    initStruct.halBaseTimerPeriod = durationInMs;
    initStruct.halBaseTimerPrescaler = 20999;
    initStruct.halBaseTimersClockDivision = halBaseTimerClockDivision1;
    halBaseTimerInit(timer,&initStruct,callback);
}

void drvTimerStart(drvTimers timer){
    halBaseTimerStart(timer);
}

void drvTimerStop(drvTimers timer){
    halBaseTimerStop(timer);
}
