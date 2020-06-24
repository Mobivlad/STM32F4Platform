/*
 * drv_sysclock.c
 *
 *  Created on: 24 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_sysclock.h"

drvSysClockPassStatus drvIsTimePased(uint32_t startTime, uint32_t delay) {
    return (startTime + delay) < drvSysClockGetTick() ? PASSED : NO_PASSED;
}

uint32_t drvSysClockGetTick() {
    return halSysClockGetTick();
}
