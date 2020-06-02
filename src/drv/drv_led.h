/*
 * drv_led.h
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_LED_H_
#define DRV_DRV_LED_H_

#include "hal_gpio.h"

/// Port's pin enumeration
typedef enum {
    drvLedPin0 = halPin_0,       /**< Pin 0 */
    drvLedPin1 = halPin_1,       /**< Pin 1 */
    drvLedPin2 = halPin_2,       /**< Pin 2 */
    drvLedPin3 = halPin_3,       /**< Pin 3 */
    drvLedPin4 = halPin_4,       /**< Pin 4 */
    drvLedPin5 = halPin_5,       /**< Pin 5 */
    drvLedPin6 = halPin_6,       /**< Pin 6 */
    drvLedPin7 = halPin_7,       /**< Pin 7 */
    drvLedPin8 = halPin_8,       /**< Pin 8 */
    drvLedPin9 = halPin_9,       /**< Pin 9 */
    drvLedPin10 = halPin_10,     /**< Pin 10 */
    drvLedPin11 = halPin_11,     /**< Pin 11 */
    drvLedPin12 = halPin_12,     /**< Pin 12 */
    drvLedPin13 = halPin_13,     /**< Pin 13 */
    drvLedPin14 = halPin_14,     /**< Pin 14 */
    drvLedPin15 = halPin_15      /**< Pin 15 */
} drvLedPins;

/// STM32F407-Disk1 port enumeration
typedef enum {
    drvLedPortA = halPortA,   /**< Port A */
    drvLedPortB = halPortB,   /**< Port B */
    drvLedPortC = halPortC,   /**< Port C */
    drvLedPortD = halPortD,   /**< Port D */
    drvLedPortE = halPortE,   /**< Port E */
} drvLedPort;

/// Led data structure
typedef struct {
    drvLedPins pin;      /**< led pin */
    drvLedPort port;    /**< led port */
} drvLedData;

/**
 * Function to configurate led.
 * @param port value of drvLedPort enumeration
 * @param pins value of drvLedPins enumeration
 */
void drvLedInit(drvLedPort port, drvLedPins pins);

/**
 * Function to on led.
 * @param port value of drvLedPort enumeration
 * @param pins value of drvLedPins enumeration
 */
void drvLedOn(drvLedPort port, drvLedPins pins);

/**
 * Function to off led.
 * @param port value of drvLedPort enumeration
 * @param pins value of drvLedPins enumeration
 */
void drvLedOff(drvLedPort port, drvLedPins pins);

/**
 * Function to change current led state.
 * @param port value of drvLedPort enumeration
 * @param pins value of drvLedPins enumeration
 */
void drvLedToggle(drvLedPort port, drvLedPins pins);

#endif /* DRV_DRV_LED_H_ */
