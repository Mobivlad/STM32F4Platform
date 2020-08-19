/*
 * bl_adc_controller.h
 *
 *  Created on: 22 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef BL_BL_ADC_CONTROLLER_H_
#define BL_BL_ADC_CONTROLLER_H_

#include "ul_moving_average.h"

#include "bl_adc_file_writer.h"
#include "bl_adc_display.h"

#include "drv_button.h"

#define ADC_DRIVER  drvADC1

#define QUEUE_SIZE  200

typedef enum {
    SUSPENDED,
    IN_RUN
} blADC_state;

typedef struct {
    drvButton_struct        button;

    ulMovingAvarege_struct  movingAvarageUtil;

    SemaphoreHandle_t       switchHandlerSemaphore;

    QueueHandle_t           filterControlQueue;

    QueueHandle_t           fileWriterQueue;
    QueueHandle_t           displayQueue;

    blADC_state             state;

} blADCController_struct;


void blADCControllerInit(blADCController_struct* controllerStruct, uint32_t frequency,
        QueueHandle_t displayQueue, QueueHandle_t fileWriterQueue);

void blADCControllerButtonTask(void* parametr);

void blADCControllerSwitchTask(void* parametr);

#endif /* BL_BL_ADC_CONTROLLER_H_ */
