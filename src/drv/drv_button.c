/*
 * drv_button.c
 *
 *  Created on: 29 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_button.h"

static uint32_t numberPress;
static uint8_t buttonClickCount;

void dvrButtonInit(drvInitStructButton* initStruct) {
    halInitGPIOStruct halInitStruct;
    halInitStruct.halPins = initStruct->drvButtonPins;
    halInitStruct.halGpioMode = halModeIn;
    halInitStruct.halGpioSpeed = halSpeed2Mhz;
    halInitStruct.halOutType = halOTPP;
    if (initStruct->drvButtonPullType) {
        halInitStruct.halPuPd = halPuPdNo;
    } else {
        halInitStruct.halPuPd = initStruct->drvButtonPuPd;
    }
    halGPIOInit(initStruct->drvButtonPort, &halInitStruct);
    numberPress=0;
    buttonClickCount=0;
}

static uint8_t drvButtonIsClicked(drvButtonPort port, drvButtonPins pin) {
    if (halGPIOReadPin(port, pin) && buttonClickCount < 110) {
        buttonClickCount++;
    } else if (buttonClickCount != 0) {
        buttonClickCount--;
    }
    if (buttonClickCount > 100) return 1;
    if (buttonClickCount < 10)  return 0;
    return 2;
}

drvButtonPressType drvButtonIsClick(drvButtonPort port, drvButtonPins pin) {
    drvButtonPressType pressType = drvButtonNoPress;
    switch (drvButtonIsClicked(port, pin)==1) {
        case 1:
            numberPress++;
            break;
        case 0:
            if (numberPress > drvButtonSimplePressDuration) pressType = drvButtonSimplePress;
            if (numberPress > drvButtonLongPressDuration) pressType = drvButtonLongPress;
            numberPress = 0;
    }
    return pressType;
}
