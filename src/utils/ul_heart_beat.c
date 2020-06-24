/*
 * ul_heart_beat.c
 *
 *  Created on: 24 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "ul_heart_beat.h"

void ulHeartBeatInit(ulHeartBeatStruct* heartBeat){
    drvLed_struct* drvLedStruct = (drvLed_struct*)heartBeat;

    drvLedStruct->led = drvLed1;
    drvLedStruct->controlPin = drvLed_ControlPin_Anode;
    drvLedStruct->state = drvLed_On;

    heartBeat->startTime = drvSysClockGetTick();

    drvLedInit((drvLed_struct*)heartBeat);
}

void ulHeartBeatRun(ulHeartBeatStruct* heartBeat){
    if (drvIsTimePased(heartBeat->startTime, heartBeat->frequency) == PASSED){
        drvLedToggle((drvLed_struct*)heartBeat);
        heartBeat->startTime = drvSysClockGetTick();
    }
}
