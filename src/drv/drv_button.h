/*
 * drv_button.h
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_BUTTON_H_
#define DRV_DRV_BUTTON_H_

#include "hal_gpio.h"

/// Port's pin enumeration
typedef enum {
    drvButtonPin0 = halPin_0,       /**< Pin 0 */
    drvButtonPin1 = halPin_1,       /**< Pin 1 */
    drvButtonPin2 = halPin_2,       /**< Pin 2 */
    drvButtonPin3 = halPin_3,       /**< Pin 3 */
    drvButtonPin4 = halPin_4,       /**< Pin 4 */
    drvButtonPin5 = halPin_5,       /**< Pin 5 */
    drvButtonPin6 = halPin_6,       /**< Pin 6 */
    drvButtonPin7 = halPin_7,       /**< Pin 7 */
    drvButtonPin8 = halPin_8,       /**< Pin 8 */
    drvButtonPin9 = halPin_9,       /**< Pin 9 */
    drvButtonPin10 = halPin_10,     /**< Pin 10 */
    drvButtonPin11 = halPin_11,     /**< Pin 11 */
    drvButtonPin12 = halPin_12,     /**< Pin 12 */
    drvButtonPin13 = halPin_13,     /**< Pin 13 */
    drvButtonPin14 = halPin_14,     /**< Pin 14 */
    drvButtonPin15 = halPin_15      /**< Pin 15 */
} drvButtonPins;

/// STM32F407-Disk1 port enumeration
typedef enum {
    drvButtonPortA = halPortA,   /**< Port A */
    drvButtonPortB = halPortB,   /**< Port B */
    drvButtonPortC = halPortC,   /**< Port C */
    drvButtonPortD = halPortD,   /**< Port D */
    drvButtonPortE = halPortE,   /**< Port E */
} drvButtonPort;

/// Pull type
typedef enum {
    drvButtonHardPull = 0,
    drvButtonSoftPull = !drvButtonHardPull,
} drvButtonPullType;

/// Internal pull-up type enumeration
typedef enum {
    drvButtonPuPdUp   = halPuPdUp,     /**< Up to VCC. */
    drvButtonPuPdDown = halPuPdDown,   /**< Down to Ground. */
    drvButtonPuPdNo = halPuPdNo,
} drvButtonPuPd;

/// Button initial structure
typedef struct {
    drvButtonPins drvButtonPins;
    drvButtonPort drvButtonPort;
    drvButtonPullType drvButtonPullType;
    drvButtonPuPd drvButtonPuPd;
} drvInitStructButton;

/**
 * Function that configurate button.
 * @param initStruct pointer to structure that consist of main port configuration of button
 */
void dvrButtonInit(drvInitStructButton* initStruct);

/**
 * Function that detect one abstract button click.
 * @param port button port
 * @param pin button pin
 */
uint8_t drvIsButtonClicked(drvButtonPort port,drvButtonPins pin);

#endif /* DRV_DRV_BUTTON_H_ */
