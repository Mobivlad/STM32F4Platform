/*
 * bl_adc_controller.c
 *
 *  Created on: 22 ���. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_adc_controller.h"

static const uint8_t stopFilterCommand = ulMovingAvarege_STOP;
static const uint8_t startFilterCommand = ulMovingAvarege_START;

static blADCController_struct* currentStruct;
BaseType_t ret;
static void buttonAction(){
    ret = xSemaphoreGiveFromISR(currentStruct->switchHandlerSemaphore, NULL);
    drvLedToggle(&currentStruct->led);
}

void blADCControllerInit(blADCController_struct* controllerStruct, uint32_t frequency, 
                        QueueHandle_t writerQueue) {
    controllerStruct->switchHandlerSemaphore    = xSemaphoreCreateBinary();

    controllerStruct->filterControlQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
    controllerStruct->filterDataQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint16_t));

    controllerStruct->led.led           = drvLed2;
    controllerStruct->led.controlPin    = drvLed_ControlPin_Anode;
    controllerStruct->led.state         = drvLed_Off;
    drvLedInit(&controllerStruct->led);
    
    controllerStruct->button.button = drvButton1;
    drvButtonInit((drvButton_struct*)controllerStruct);

    drvButtonSetCallBack((drvButton_struct*)controllerStruct, drvButton_Press, buttonAction);
    drvButtonSetCallBack((drvButton_struct*)controllerStruct, drvButton_LongPress, buttonAction);

    controllerStruct->writerQueue = writerQueue;

    controllerStruct->movingAvarageUtil.adc           = ADC_DRIVER;
    controllerStruct->movingAvarageUtil.frequency     = frequency;
    controllerStruct->movingAvarageUtil.commandQueue  = controllerStruct->filterControlQueue;
    controllerStruct->movingAvarageUtil.dataQueue     = controllerStruct->filterDataQueue;

    ulMovingAvaregeInit(&controllerStruct->movingAvarageUtil);

    currentStruct=controllerStruct;
}

void blADCControllerSwitchTask(void* parametr) {
    blADCController_struct* controllerStruct = (blADCController_struct*) parametr;
    while (1) {
        xSemaphoreTake(controllerStruct->switchHandlerSemaphore, portMAX_DELAY);
        if (controllerStruct->state == IN_RUN) {
            xQueueSend(controllerStruct->filterControlQueue, &stopFilterCommand, 0);
            controllerStruct->state = SUSPENDED;
        } else {
            xQueueSend(controllerStruct->filterControlQueue, &startFilterCommand, 0);
            controllerStruct->state = IN_RUN;
        }
    }
}

void blADCControllerTask(void* parametr) {
    blADCController_struct* controllerStruct = (blADCController_struct*) parametr;
    uint16_t ADCData;
    while (1) {
        xQueueReceive(controllerStruct->filterDataQueue, &ADCData, portMAX_DELAY);
        blADCWriter_record record = { blADCWriter_DATA,  ADCData * ADC_MAX_INT_VAL / ADC_MAX_V };
        xQueueSend(controllerStruct->writerQueue, &record, 0);
    }
}
