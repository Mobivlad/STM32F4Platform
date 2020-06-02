/*
 * bl_button_toggle.c
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_button_toggle.h"

void blButtonLedToggleInit(blButtonData* button,ledData* led){
    drvInitStructButton initStr;
    initStr.drvButtonPins = button->pin;
    initStr.drvButtonPort = button->port;
    initStr.drvButtonPullType = drvButtonHardPull;
    initStr.drvButtonPuPd = drvButtonPuPdNo;
    dvrButtonInit(&initStr);
    drvLedInit(led->port,led->pin);
}


uint8_t count;
uint8_t blButtonIsClick(blButtonData* button){
    uint8_t code = 0;;
    switch(drvButtonIsClicked(button->port,button->pin)){
        case 1:
            button->numberPress++;
            break;
        case 0:
            count = button->numberPress;
            if(count > 50) code = 1;
            button->numberPress = 0;
        }
    return code;
}

void blToggleLedIfButtonClick(blButtonData* button,ledData* led){
    if(blButtonIsClick(button)==1){
        drvLedToggle(led->port,led->pin);
    }
}
