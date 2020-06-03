/*
 * bl_timer_blink.c
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_timer_blink.h"

static drvLedData led = {drvLedPin14, drvLedPortD};
static blBlinkData blink = { &led, 0 };

static uint32_t drvBlinkDelay;
void blBlinkInit(uint32_t time) {
    drvBlinkDelay = time;
    drvLedInit(blink.led->port, blink.led->pin);
    drvSysClockInit(1);
}

void blBlinkRun() {
    if (drvIsTimePased(blink.timer, drvBlinkDelay)) {
        drvLedToggle(blink.led->port, blink.led->pin);
        blink.timer = drvSysClockGetTick();
    }
}
