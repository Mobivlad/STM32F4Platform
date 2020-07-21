/*
 * ul_moving_average.h
 *
 *  Created on: 21 ���. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#ifndef UTILS_UL_MOVING_AVERAGE_H_
#define UTILS_UL_MOVING_AVERAGE_H_

#include "drv_adc.h"

#define WINDOW_SIZE             10

#define QUEUE_SIZE              10

#define MIN_DELAY               1
#define INCORRECT_DATA          0x1FFF
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

/**
 * Initial function for ADC with simple moving average filter
 */
void ulMovingAvaregeInit(ulMovingAvarege_struct* ulMA_struct);

/**
 * Execute function for ADC
 */
void ulMovingAvaregeRun(ulMovingAvarege_struct* ulMA_struct);

/**
 * Task function for thread creation
 */
void ulMovingAvaregeTaskFunction(void* parametr);
#endif /* UTILS_UL_MOVING_AVERAGE_H_ */
