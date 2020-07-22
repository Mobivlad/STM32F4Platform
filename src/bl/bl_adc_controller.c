/*
 * bl_adc_controller.c
 *
 *  Created on: 22 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_adc_controller.h"

static blADCController_struct* currentStruct;

static void buttonAction(){
    xSemaphoreGive(currentStruct->controlSemaphore);
}

void blADCControllerInit(blADCController_struct* controllerStruct) {
    controllerStruct->button.button = drvButton1;
    drvButtonInit((drvButton_struct*)controllerStruct);

    drvButtonSetCallBack((drvButton_struct*)controllerStruct, drvButton_Press, buttonAction);
    drvButtonSetCallBack((drvButton_struct*)controllerStruct, drvButton_LongPress, buttonAction);

    currentStruct=controllerStruct;
}

void blADCControllerTask(void* parametr) {
    blADCController_struct* controllerStruct = (blADCController_struct*) parametr;
    while (1) {
        drvButtonRun((drvButton_struct*)controllerStruct);
    }
}
