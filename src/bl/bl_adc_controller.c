/*
 * bl_adc_controller.c
 *
 *  Created on: 22 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_adc_controller.h"

static blADCController_struct* currentStruct;

static void buttonAction(){
    xSemaphoreGive(currentStruct->startStorSemaphore);
}

void blADCControllerInit(blADCController_struct* controllerStruct, uint32_t frequency) {
    controllerStruct->startStorSemaphore    = xSemaphoreCreateBinary();

    controllerStruct->displayQueue       = xQueueCreate(QUEUE_SIZE, sizeof(uint16_t));
    controllerStruct->fileWriterQueue    = xQueueCreate(QUEUE_SIZE, sizeof(uint16_t));

    controllerStruct->button.button = drvButton1;
    drvButtonInit((drvButton_struct*)controllerStruct);

    drvButtonSetCallBack((drvButton_struct*)controllerStruct, drvButton_Press, buttonAction);
    drvButtonSetCallBack((drvButton_struct*)controllerStruct, drvButton_LongPress, buttonAction);

    controllerStruct->movingAvarageUtil.adc = ADC_DRIVER;
    controllerStruct->movingAvarageUtil.frequency = frequency;

    controllerStruct->movingAvarageUtil.fileValues      = controllerStruct->fileWriterQueue;
    controllerStruct->movingAvarageUtil.displayValues   = controllerStruct->displayQueue;

    ulMovingAvaregeInit(&controllerStruct->movingAvarageUtil);

    currentStruct=controllerStruct;
}

void blADCControllerButtonTask(void* parametr) {
    blADCController_struct* controllerStruct = (blADCController_struct*) parametr;
    while (1) {
        drvButtonRun((drvButton_struct*)controllerStruct);
    }
}

void blADCControllerStartStopTask(void* parametr) {
    blADCController_struct* controllerStruct = (blADCController_struct*) parametr;
    while (1) {
        xSemaphoreTake(controllerStruct->startStorSemaphore, portMAX_DELAY);
        if (controllerStruct->state == IN_RUN) {
            ulMovingAvaregeStop(&controllerStruct->movingAvarageUtil);
            blADCDisplayStop(&controllerStruct->adcd);
            blADCCloseFile(&controllerStruct->adcw);
            controllerStruct->state = SUSPENDED;
        } else {
            ulMovingAvaregeStart(&controllerStruct->movingAvarageUtil);
            blADCDisplayStart(&controllerStruct->adcd);
            blADCOpenFile(&controllerStruct->adcw);
            controllerStruct->state = IN_RUN;
        }
    }
}
