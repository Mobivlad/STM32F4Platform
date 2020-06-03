/*
 * bl_button_toggle.c
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include <bl_button_blink.h>

static drvButtonData button = { drvButtonPin0, drvButtonPortA };
static drvLedData led = { drvLedPin12, drvLedPortD };

void blButtonBlinkInit() {
    drvInitStructButton initStr;
    initStr.drvButtonPins = button.pin;
    initStr.drvButtonPort = button.port;
    initStr.drvButtonPullType = drvButtonHardPull;
    initStr.drvButtonPuPd = drvButtonPuPdNo;
    dvrButtonInit(&initStr);
    drvLedInit(led.port, led.pin);
}

void blButtonBlinkRun() {
    drvButtonPressType res = drvButtonIsClick(button.port,button.pin);
    if (res==drvButtonSimplePress || res==drvButtonLongPress) {
        drvLedToggle(led.port, led.pin);
    }
}
