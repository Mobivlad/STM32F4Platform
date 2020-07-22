/*
 * ul_moving_average.c
 *
 *  Created on: 21 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "ul_moving_average.h"

void ulMovingAvaregeInit(ulMovingAvarege_struct* ulMA_struct) {
    ulMA_struct->adcValue = INCORRECT_DATA;

    ulMA_struct->queue      = xQueueCreate(QUEUE_SIZE, sizeof(uint16_t));
    ulMA_struct->semaphore  = xSemaphoreCreateBinary();

    drvADCInit((drvADC_struct*) ulMA_struct, ulMA_struct->adc, ulMA_struct->frequency,
            ulMA_struct->queue, ulMA_struct->semaphore);

    ulMA_struct->state = SUSPENDED;

    ulMovingAvaregeStop(ulMA_struct);
}

static uint16_t getAverage(uint16_t* arr, uint16_t size){
    uint32_t sum = 0;
    for(uint16_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    return (uint16_t)(sum/size);
}

void ulMovingAvaregeStart(ulMovingAvarege_struct* ulMA_struct) {
    drvADCStart((drvADC_struct*) ulMA_struct);
    ulMA_struct->state = IN_RUN;
}

void ulMovingAvaregeStop(ulMovingAvarege_struct* ulMA_struct) {
    drvADCStop((drvADC_struct*) ulMA_struct);
    ulMA_struct->state = SUSPENDED;
}

void ulMovingAvaregeTaskFunction(void* parametr) {
    ulMovingAvarege_struct* const ulMA_struct = (ulMovingAvarege_struct* const ) parametr;

    while (1) {
        xQueueReceive(ulMA_struct->queue, &(ulMA_struct->windowsData[ulMA_struct->windowsIndex++]),
                portMAX_DELAY);

        if ((ulMA_struct->adcValue == INCORRECT_DATA && ulMA_struct->windowsIndex == WINDOW_SIZE)
                || (ulMA_struct->adcValue != INCORRECT_DATA)) {
            ulMA_struct->adcValue = getAverage(ulMA_struct->windowsData, WINDOW_SIZE);
        }

        ulMA_struct->windowsIndex = ulMA_struct->windowsIndex % WINDOW_SIZE;
    }
}

void ulMovingAvaregeControlFunction(void* parametr) {
    ulMovingAvarege_struct* const ulMA_struct = (ulMovingAvarege_struct* const ) parametr;
    while (1) {
        xSemaphoreTake(ulMA_struct->semaphore, portMAX_DELAY);
        if (ulMA_struct->state == IN_RUN) {
            ulMovingAvaregeStop(ulMA_struct);
        } else {
            ulMovingAvaregeStart(ulMA_struct);
        }
    }
}
