/*
 * drv_sysclock.c
 *
 *  Created on: 24 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_sysclock.h"

void drvSysClockInit() {
    halSysClockInit();
}

void drvSysClockDeinit() {
    halSysClockDeinit();
}

drvSysClockPassStatus drvIsTimePased(const uint32_t startTime, const uint32_t delay) {
    return (startTime + delay) < drvSysClockGetTick() ? PASSED : NO_PASSED;
}

uint32_t drvSysClockGetTick() {
    return halSysClockGetTick();
}
