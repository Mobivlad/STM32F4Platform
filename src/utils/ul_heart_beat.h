/*
 * ul_heart_beat.h
 *
 *  Created on: 24 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef UTILS_UL_HEART_BEAT_H_
#define UTILS_UL_HEART_BEAT_H_

#include "drv_led.h"
#include "FreeRTOS.h"
#include "task.h"


typedef struct {
    drvLed_struct   led;           // information about led
    uint32_t        frequency;     // time between blink
} ulHeartBeatStruct;

/**
 * Initial function for heart beat
 */
void ulHeartBeatInit(ulHeartBeatStruct* const heartBeat, uint32_t frequency);

/**
 * Run function for heart beat
 */
void ulHeartBeatRun(ulHeartBeatStruct* const heartBeat);

/**
 * HeartBeat thread function
 */
void ulHeartBeatTaskFunction(void* parametr);

#endif /* UTILS_UL_HEART_BEAT_H_ */
