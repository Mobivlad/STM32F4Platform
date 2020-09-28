/*
 * bl_adc_controller.h
 *
 *  Created on: 22 ���. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#ifndef BL_BL_ADC_CONTROLLER_H_
#define BL_BL_ADC_CONTROLLER_H_

#include "ul_moving_average.h"

#include "bl_adc_writer.h"

#include "drv_button.h"
#include "drv_led.h"

#define ADC_DRIVER          drvADC1

#define ADC_MAX_V           3.3f
#define ADC_MAX_INT_VAL     0xFFF

#define QUEUE_SIZE          200

#define BUTTON_DELAY_TIME   5

typedef enum {
    SUSPENDED,
    IN_RUN
} blADC_state;

typedef struct {
    drvButton_struct        button;
    drvLed_struct           led;

    ulMovingAvarege_struct  movingAvarageUtil;

    SemaphoreHandle_t       switchHandlerSemaphore;

    QueueHandle_t           filterControlQueue;
    QueueHandle_t           filterDataQueue;

    QueueHandle_t           writerQueue;

    blADC_state             state;

} blADCController_struct;


void blADCControllerInit(blADCController_struct* controllerStruct, uint32_t frequency, 
                        QueueHandle_t writerQueue);

void blADCControllerSwitchTask(void* parametr);
void blADCControllerTask(void* parametr);

#endif /* BL_BL_ADC_CONTROLLER_H_ */
