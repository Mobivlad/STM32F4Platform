/*
 * bl_timer_blink.c
 *
 *  Created on: 1 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_timer_blink.h"

void blInitBlink(ledData* led){
    drvLedInit(led->port,led->pin);
    drvSysClockInit(1);
}

void blBlink(ledData* led,uint32_t time){
    if(drvIsTimePased(led->timer,time)){
        drvLedToggle(led->port,led->pin);
        led->timer = drvSysClockGetTick();
    }
}
