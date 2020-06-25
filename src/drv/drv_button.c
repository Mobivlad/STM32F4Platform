/*
 * drv_button.c
 *
 *  Created on: 25 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_button.h"

static drvButton_data button_data[drvButton_Count] =
{
        { halGPIO_PortA, halGPIO_Pin_0 },
};

void drvButton_init(drvButton_struct* buttonStruct) {
    halGPIO_struct* hal_GPIO_struct = (halGPIO_struct*) buttonStruct;
    hal_GPIO_struct->port = button_data[buttonStruct->button].port;

    const halGPIO_init_struct initStruct = {
            button_data[buttonStruct->button].pin, halGPIO_Mode_In,
            halGPIO_Speed_2Mhz, halGPIO_OT_PP, halGPIO_PuPd_No };

    halGPIOInit(hal_GPIO_struct, &initStruct);

    buttonStruct->click_counter = 0;
    buttonStruct->click_duration = 0;
}

void drvButton_setCallBack(drvButton_struct* buttonStruct,
        drvButton_action action, drvButtonActionCallBack callback) {
    buttonStruct->actionCallbacks[action] = callback;
}

static uint8_t drvButtonIsClicked(drvButton_struct* buttonStruct) {
    if (halGPIOReadPin((halGPIO_struct*)buttonStruct) && buttonStruct->click_counter < 110) {
        buttonStruct->click_counter++;
    } else if (buttonStruct->click_counter != 0) {
        buttonStruct->click_counter--;
    }
    if (buttonStruct->click_counter > 100) return drvButton_Clicked;
    if (buttonStruct->click_counter < 10)  return drvButton_Falled;
    return drvButton_Float;
}

void drvButton_Run(drvButton_struct* buttonStruct) {

    switch (drvButtonIsClicked(buttonStruct)) {

        case drvButton_Clicked:
            buttonStruct->click_duration++;
            break;

        case drvButton_Falled:
            if (buttonStruct->click_duration
                                    > drvButtonLongPressDuration)

                buttonStruct->actionCallbacks[drvButton_Press]();

            else if (buttonStruct->click_duration > drvButtonSimplePressDuration)

                buttonStruct->actionCallbacks[drvButton_LongPress]();

            buttonStruct->click_duration = 0;
            break;

        default:
            break;
    }
}



