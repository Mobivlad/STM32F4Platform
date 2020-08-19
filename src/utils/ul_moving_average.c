/*
 * ul_moving_average.c
 *
 *  Created on: 21 ���. 2020 �.
 *      Author: vladyslav.daliavskyi
 */
#include "ul_moving_average.h"

#include "bl_adc_display.h"
#include "bl_adc_file_writer.h"

static void ulMovingAvaregeStart(ulMovingAvarege_struct* ulMA_struct) {
    drvADCStart((drvADC_struct*) ulMA_struct);
    for (uint8_t i=0; i<WINDOW_SIZE; i++) {
        ulMA_struct->windowsData[i] = INCORRECT_DATA;
    }
}

static void ulMovingAvaregeStop(ulMovingAvarege_struct* ulMA_struct) {
    drvADCStop((drvADC_struct*) ulMA_struct);
}

void ulMovingAvaregeInit(ulMovingAvarege_struct* ulMA_struct) {

    ulMA_struct->valuesQueue      = xQueueCreate(UTIL_QUEUE_SIZE, sizeof(uint16_t));
    drvADCInit((drvADC_struct*) ulMA_struct, ulMA_struct->adc, ulMA_struct->frequency,
            ulMA_struct->valuesQueue);

    ulMovingAvaregeStop(ulMA_struct);
}

static uint16_t getAverage(uint16_t* arr, uint16_t size) {
    uint32_t sum = 0;
    for(uint16_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    return (uint16_t)(sum/size);
}

static blADCDisplay_record displayRecord = {
        blADCDisplay_DATA
};

static blADCFileWriter_record writerRecord = {
        blADCFileWriter_DATA
};

void ulMovingAvaregeCalculateTask(void* parametr) {
    ulMovingAvarege_struct* const ulMA_struct = (ulMovingAvarege_struct* const ) parametr;
    while (1) {
        xQueueReceive(ulMA_struct->valuesQueue, &(ulMA_struct->windowsData[ulMA_struct->windowsIndex++]),
                portMAX_DELAY);
        if ((ulMA_struct->windowsData[WINDOW_SIZE-1] != INCORRECT_DATA)) {
            uint16_t currectADCValue = getAverage(ulMA_struct->windowsData, WINDOW_SIZE);

            displayRecord.data = currectADCValue;
            writerRecord.data = currectADCValue;

            xQueueSend(ulMA_struct->fileValues, &displayRecord, 0);
            xQueueSend(ulMA_struct->displayValues, &writerRecord, 0);
        }

        ulMA_struct->windowsIndex = ulMA_struct->windowsIndex % WINDOW_SIZE;
    }
}

void ulMovingAvaregeControlTask(void* parametr) {
    ulMovingAvarege_struct* const ulMA_struct = (ulMovingAvarege_struct* const ) parametr;
    uint8_t command;
    while (1) {
        xQueueReceive(ulMA_struct->commandQueue, &command, portMAX_DELAY);
        switch (command) {
            case ulMovingAvarege_STOP:
                ulMovingAvaregeStop(ulMA_struct);
                break;
            case ulMovingAvarege_START:
                ulMovingAvaregeStart(ulMA_struct);
                break;
            default:
                break;
        }
    }
}


