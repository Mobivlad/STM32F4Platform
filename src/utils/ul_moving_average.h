/*
 * ul_moving_average.h
 *
 *  Created on: 21 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef UTILS_UL_MOVING_AVERAGE_H_
#define UTILS_UL_MOVING_AVERAGE_H_

#include "drv_adc.h"

#define WINDOW_SIZE             10

#define QUEUE_SIZE              10

#define MIN_DELAY               1
#define INCORRECT_DATA          0xFFFF
#define DELAY_TIME              0xFF


typedef struct {
    drvADC_struct           adc_srtuct;

    drvADC                  adc;
    uint32_t                frequency;

    uint16_t                windowsData[WINDOW_SIZE];
    uint8_t                 windowsIndex;

    uint16_t                adcValue;
    QueueHandle_t           queue;
} ulMovingAvarege_struct;

void ulMovingAvaregeInit(ulMovingAvarege_struct* ulMA_struct);

void ulMovingAvaregeRun(ulMovingAvarege_struct* ulMA_struct);

void ulMovingAvaregeTaskFunction(void* parametr);
#endif /* UTILS_UL_MOVING_AVERAGE_H_ */
