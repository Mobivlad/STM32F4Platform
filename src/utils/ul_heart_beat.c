/*
 * ul_heart_beat.c
 *
 *  Created on: 24 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "ul_heart_beat.h"

void ulHeartBeatInit(ulHeartBeatStruct* const heartBeat, uint32_t frequency) {

    heartBeat->frequency = frequency;

    drvLed_struct* drvLedStruct = (drvLed_struct*) heartBeat;

    drvLedStruct->led           = drvLed1;
    drvLedStruct->controlPin    = drvLed_ControlPin_Anode;
    drvLedStruct->state         = drvLed_Off;

    drvLedInit((drvLed_struct*) heartBeat);
}

void ulHeartBeatRun(ulHeartBeatStruct* const heartBeat) {
    drvLedToggle((drvLed_struct*) heartBeat);
}

void ulHeartBeatTaskFunction(void* parametr) {
    ulHeartBeatStruct* const heartBeat = (ulHeartBeatStruct* const ) parametr;
    while (1) {
        ulHeartBeatRun(heartBeat);
        vTaskDelay(heartBeat->frequency);
    }
}


