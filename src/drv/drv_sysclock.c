/*
 * drv_sysclock.c
 *
 *  Created on: 29 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_sysclock.h"

void drvSysClockInit(uint32_t ms){
    halSysClockInit(ms);
}

uint8_t drvIsTimePased(uint32_t startTime, uint32_t delay){
    return (startTime+delay)<drvSysClockGetTick()?1:0;
}

uint32_t drvSysClockGetTick(){
    return halSysClockGetTick();
}
