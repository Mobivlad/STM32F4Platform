/*
 * bl_timer_blink.c
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_timer_blink.h"

static blBlinkData blink = { drvLedPin14, drvLedPortD, 0 };

static uint32_t drvBlinkDelay;
void blBlinkInit(uint32_t time) {
    drvBlinkDelay = time;
    drvLedInit(blink.port, blink.pin);
    drvSysClockInit(1);
}

void blBlinkRun() {
    if (drvIsTimePased(blink.timer, drvBlinkDelay)) {
        drvLedToggle(blink.port, blink.pin);
        blink.timer = drvSysClockGetTick();
    }
}
