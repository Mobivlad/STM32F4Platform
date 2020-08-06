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

#define ADC_DRIVER  drvADC1

typedef enum {
    SUSPENDED,
    IN_RUN
} blADC_state;

typedef struct {
    drvButton_struct        button;

    ulMovingAvarege_struct  movingAvarageUtil;

    SemaphoreHandle_t       startStorSemaphore;
    SemaphoreHandle_t       fileMutex;
    SemaphoreHandle_t       displayMutex;

    blADC_state             state;

} blADCController_struct;


void blADCControllerInit(blADCController_struct* controllerStruct, uint32_t frequency);

void blADCControllerButtonTask(void* parametr);

void blADCControllerStartStopTask(void* parametr);

#endif /* BL_BL_ADC_CONTROLLER_H_ */
