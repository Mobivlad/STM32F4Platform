/*
 * drv_led.h
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_LED_H_
#define DRV_DRV_LED_H_

#include "hal_gpio.h"

/// Led enumeration
typedef enum {
    drvLed1 = 0,
    drvLed_Count
} drvLed;

/// Control pin type
typedef enum {
    drvLed_ControlPin_Anode = 0,
    drvLed_ControlPin_Cathode,
} drvLed_controlPin;

/// Led states
typedef enum {
    drvLed_On = 0,
    drvLed_Off,
} drvLed_state;

/// Led data structure
typedef struct {
    halGPIO_struct halGPIO;
    drvLed led;
    drvLed_state state;
    drvLed_controlPin controlPin;
} drvLed_struct;

typedef struct {
    halGPIO_port port;
    halGPIO_pin pin;
} drvLed_data;

/**
 * Led configuration function.
 * @param ledStruct pointer on led data structure
 */
void drvLedInit(const drvLed_struct* const ledStruct);

/**
 * Turn on led function.
 * @param ledStruct pointer on led data structure
 */
void drvLedOn(drvLed_struct* const ledStruct);

/**
 * Turn off led function.
 * @param ledStruct pointer on led data structure
 */
void drvLedOff(drvLed_struct* const ledStruct);

/**
 * Toggle led function
 * @param ledStruct pointer on led data structure
 */
void drvLedToggle(drvLed_struct* const ledStruct);

#endif /* DRV_DRV_LED_H_ */
