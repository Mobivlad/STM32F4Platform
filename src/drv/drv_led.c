/*
 * drv_led.c
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "drv_led.h"

static drvLed_data led_data[drvLed_Count] =
{
    {halGPIO_PortD, halGPIO_Pin_15},
};

void drvLedInit(drvLed_struct* ledStruct) {
    halGPIO_struct* hal_GPIO_struct = (halGPIO_struct*)ledStruct;
    hal_GPIO_struct->port = led_data[ledStruct->led].port;

    const halGPIO_init_struct initStruct = {
            led_data[ledStruct->led].pin,
            halGPIO_Mode_Out,
            halGPIO_Speed_2Mhz,
            halGPIO_OT_PP,
            halGPIO_PuPd_No
    };

    if (ledStruct->controlPin == drvLed_ControlPin_Anode) {
        if (ledStruct->state == drvLed_On)
            hal_GPIO_struct->state = halGPIO_Set;
        else
            hal_GPIO_struct->state = halGPIO_Reset;
    } else {
        if (ledStruct->state == drvLed_On)
            hal_GPIO_struct->state = halGPIO_Reset;
        else
            hal_GPIO_struct->state = halGPIO_Set;
    }
    halGPIOInit(hal_GPIO_struct, &initStruct);
}

void drvLedOn(drvLed_struct* ledStruct) {
    ledStruct -> state = drvLed_On;
    halGPIOSetPins((halGPIO_struct*)ledStruct);
}

void drvLedOff(drvLed_struct* ledStruct) {
    ledStruct -> state = drvLed_Off;
    halGPIOResetPins((halGPIO_struct*)ledStruct);
}

void drvLedToggle(drvLed_struct* ledStruct) {
    if (ledStruct -> state == drvLed_On)
        ledStruct -> state = drvLed_Off;
    else
        ledStruct -> state = drvLed_On;

    halGPIOTogglePins((halGPIO_struct*)ledStruct);
}
