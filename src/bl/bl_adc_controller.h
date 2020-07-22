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

typedef struct {
    drvButton_struct    button;

    SemaphoreHandle_t   controlSemaphore;
} blADCController_struct;



void blADCControllerInit(blADCController_struct* controllerStruct);

void blADCControllerTask(void* parametr);

#endif /* BL_BL_ADC_CONTROLLER_H_ */
