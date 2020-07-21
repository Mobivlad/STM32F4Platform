/*
 * ul_moving_average.c
 *
 *  Created on: 21 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "ul_moving_average.h"

void ulMovingAvaregeInit(ulMovingAvarege_struct* ulMA_struct) {
    ulMA_struct->adcValue = INCORRECT_DATA;

    ulMA_struct->queue = xQueueCreate(QUEUE_SIZE, sizeof(uint16_t));

    drvADCInit((drvADC_struct*) ulMA_struct, ulMA_struct->adc, ulMA_struct->frequency,
            ulMA_struct->queue);

    drvADCStart((drvADC_struct*) ulMA_struct);
}

static uint16_t getAverage(uint16_t* arr, uint16_t size){
    uint32_t sum = 0;
    for(uint16_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    return (uint16_t)(sum/size);
}

void ulMovingAvaregeRun(ulMovingAvarege_struct* ulMA_struct) {
    if (uxQueueMessagesWaiting(ulMA_struct->queue) != 0){
        xQueueReceive(ulMA_struct->queue, &(ulMA_struct->windowsData[ulMA_struct->windowsIndex++]),
                DELAY_TIME);

        if ((ulMA_struct->adcValue == INCORRECT_DATA && ulMA_struct->windowsIndex == WINDOW_SIZE) ||
                (ulMA_struct->adcValue != INCORRECT_DATA)) {
            ulMA_struct->adcValue = getAverage(ulMA_struct->windowsData, WINDOW_SIZE);
        }

        ulMA_struct->windowsIndex = ulMA_struct->windowsIndex % WINDOW_SIZE;
    }
}

void ulMovingAvaregeTaskFunction(void* parametr) {
    ulMovingAvarege_struct* const movingAverage = (ulMovingAvarege_struct* const ) parametr;
    while (1) {
        ulMovingAvaregeRun(movingAverage);
        vTaskDelay(MIN_DELAY);
    }
}
