/*
 * drv_led.c
 *
 *  Created on: 29 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */
#include "drv_led.h"

static const drvLed_def_t led_define[drvLed_Count] =
{
        { halGPIO_PortB, halGPIO_Pin0 },
        { halGPIO_PortB, halGPIO_Pin7 },
        { halGPIO_PortB, halGPIO_Pin8 },
};

void drvLedInit(const drvLed_struct* const ledStruct) {
    if (ledStruct == NULL) return;
    halGPIO_struct* hal_GPIO_struct = (halGPIO_struct*) ledStruct;

    hal_GPIO_struct->port = led_define[ledStruct->led].port;

    const halGPIO_initStruct initStruct = {
            led_define[ledStruct->led].pin,
            halGPIO_ModeOut,
            halGPIO_Speed2Mhz,
            halGPIO_OTPP,
            halGPIO_PuPdNo
    };

    if (ledStruct->controlPin == drvLed_ControlPin_Anode) {
        hal_GPIO_struct->state = (ledStruct->state == drvLed_On) ? halGPIO_Set : halGPIO_Reset;
    } else {
        hal_GPIO_struct->state = (ledStruct->state == drvLed_On) ? halGPIO_Reset : halGPIO_Set;
    }

    halGPIOInit(hal_GPIO_struct, &initStruct);
}

void drvLedOn(drvLed_struct* const ledStruct) {
    if (ledStruct == NULL) return;
    // change state in structure
    ledStruct->state = drvLed_On;

    halGPIOSetPins((halGPIO_struct*) ledStruct);
}

void drvLedOff(drvLed_struct* const ledStruct) {
    if (ledStruct == NULL) return;
    // change state in structure
    ledStruct->state = drvLed_Off;

    halGPIOResetPins((halGPIO_struct*) ledStruct);
}

void drvLedToggle(drvLed_struct* const ledStruct) {
    if (ledStruct == NULL) return;
    // change state in structure
    ledStruct->state = (ledStruct->state == drvLed_On) ? drvLed_Off : drvLed_On;

    halGPIOTogglePins((halGPIO_struct*) ledStruct);
}
