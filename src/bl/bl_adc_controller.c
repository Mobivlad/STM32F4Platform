/*
 * bl_adc_controller.c
 *
 *  Created on: 22 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_adc_controller.h"

static const uint8_t stopFilterCommand = ulMovingAvarege_STOP;
static const uint8_t startFilterCommand = ulMovingAvarege_START;

const static blADCDisplay_record stopDisplayCommand = {
        blADCDisplay_COMMAND, blADCDisplay_StopCommand
};
const static blADCDisplay_record startDisplayCommand = {
        blADCDisplay_COMMAND, blADCDisplay_StartCommand
};

const static blADCFileWriter_record stopFileWriterCommand = {
        blADCFileWriter_COMMAND, blADCFileWriter_StopCommand
};
const static blADCFileWriter_record startFileWriterCommand = {
        blADCFileWriter_COMMAND, blADCFileWriter_StartCommand
};

static blADCController_struct* currentStruct;

static void buttonAction(){
    xSemaphoreGive(currentStruct->switchHandlerSemaphore);
}

void blADCControllerInit(blADCController_struct* controllerStruct, uint32_t frequency,
        QueueHandle_t displayQueue, QueueHandle_t fileWriterQueue) {
    controllerStruct->switchHandlerSemaphore    = xSemaphoreCreateBinary();

    controllerStruct->filterControlQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));

    controllerStruct->button.button = drvButton1;
    drvButtonInit((drvButton_struct*)controllerStruct);

    drvButtonSetCallBack((drvButton_struct*)controllerStruct, drvButton_Press, buttonAction);
    drvButtonSetCallBack((drvButton_struct*)controllerStruct, drvButton_LongPress, buttonAction);

    controllerStruct->movingAvarageUtil.adc = ADC_DRIVER;
    controllerStruct->movingAvarageUtil.frequency = frequency;

    controllerStruct->fileWriterQueue = fileWriterQueue;
    controllerStruct->displayQueue    = displayQueue;

    controllerStruct->movingAvarageUtil.commandQueue    = controllerStruct->filterControlQueue;
    controllerStruct->movingAvarageUtil.fileValues = fileWriterQueue;
    controllerStruct->movingAvarageUtil.displayValues = displayQueue;

    ulMovingAvaregeInit(&controllerStruct->movingAvarageUtil);

    currentStruct=controllerStruct;
}

void blADCControllerButtonTask(void* parametr) {
    blADCController_struct* controllerStruct = (blADCController_struct*) parametr;
    while (1) {
        drvButtonRun((drvButton_struct*)controllerStruct);
        vTaskDelay(5);
    }
}

char x[6] = {0};

void blADCControllerSwitchTask(void* parametr) {
    blADCController_struct* controllerStruct = (blADCController_struct*) parametr;
    while (1) {
        xSemaphoreTake(controllerStruct->switchHandlerSemaphore, portMAX_DELAY);
        if (controllerStruct->state == IN_RUN) {
            xQueueSend(controllerStruct->filterControlQueue, &stopFilterCommand, 0);
            xQueueSend(controllerStruct->displayQueue, &stopDisplayCommand, 0);
            xQueueSend(controllerStruct->fileWriterQueue, &stopFileWriterCommand, 0);
            controllerStruct->state = SUSPENDED;
        } else {
            xQueueSend(controllerStruct->filterControlQueue, &startFilterCommand, 0);
            xQueueSend(controllerStruct->displayQueue, &startDisplayCommand, 0);
            xQueueSend(controllerStruct->fileWriterQueue, &startFileWriterCommand, 0);
            controllerStruct->state = IN_RUN;
        }
    }
}
