/*
 * bl_adc_controller.h
 *
 *  Created on: 22 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef BL_BL_ADC_CONTROLLER_H_
#define BL_BL_ADC_CONTROLLER_H_

#include "ul_moving_average.h"
#include "drv_button.h"

#include "bl_adc_file_writer.h"
#include "bl_adc_display.h"

#define ADC_DRIVER  drvADC1

#define QUEUE_SIZE  200

typedef enum {
    SUSPENDED,
    IN_RUN
} blADC_state;

typedef struct {
    drvButton_struct        button;

    ulMovingAvarege_struct  movingAvarageUtil;

    SemaphoreHandle_t       startStorSemaphore;

    QueueHandle_t           fileWriterQueue;
    QueueHandle_t           displayQueue;

    blADC_state             state;

    blADCDisplay_struct adcd;
    blADCFileWriter_struct adcw;

} blADCController_struct;


void blADCControllerInit(blADCController_struct* controllerStruct, uint32_t frequency);

void blADCControllerButtonTask(void* parametr);

void blADCControllerStartStopTask(void* parametr);

#endif /* BL_BL_ADC_CONTROLLER_H_ */
