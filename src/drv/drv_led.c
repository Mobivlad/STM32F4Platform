/*
 * drv_led.c
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "drv_led.h"

void drvLedInit(drvLedPort port,drvLedPins pins){
    halInitGPIOStruct initStruct;
    initStruct.halPins = pins;
    initStruct.halGpioSpeed = halSpeed2Mhz;
    initStruct.halPuPd = halPuPdNo;
    initStruct.halGpioMode = halModeOut;
    initStruct.halOutType = halOTPP;
    halGPIOInit(port,&initStruct);
}

void drvLedOn(drvLedPort port,drvLedPins pins){
    halSetBits(port,pins);
}

void drvLedOff(drvLedPort port,drvLedPins pins){
    halResetBits(port,pins);
}

void drvLedToggle(drvLedPort port,drvLedPins pins){
    halToggle(port,pins);
}
