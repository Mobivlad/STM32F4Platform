/*
 * drv_button.c
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_button.h"

/**
 * Function that configurate button.
 * @param initStruct pointer to structure that consist of main port configuration of button
 */

void dvrButtonInit(drvInitStructButton* initStruct){
    halInitGPIOStruct halInitStruct;
    halInitStruct.halPins = initStruct->drvButtonPins;
    halInitStruct.halGpioMode = halModeIn;
    halInitStruct.halGpioSpeed = halSpeed2Mhz;
    halInitStruct.halOutType = halOTPP;
    if(initStruct->drvButtonPullType){
        halInitStruct.halPuPd = halPuPdNo;
    } else {
        halInitStruct.halPuPd = initStruct->drvButtonPuPd;
    }
    halGPIOInit(initStruct->drvButtonPort,&halInitStruct);
}


volatile uint8_t buttonClickCount=0;
uint8_t drvIsButtonClicked(drvButtonPort port,drvButtonPins pin){
    if(halReadPin(port,pin) && buttonClickCount<110){
        buttonClickCount++;
    } else if(buttonClickCount!=0){
        buttonClickCount--;
    }
    if(buttonClickCount>100)return 1;
    if(buttonClickCount<10)return 0;
    return 2;
}
