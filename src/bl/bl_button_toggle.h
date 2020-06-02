/*
 * bl_button_toggle.h
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef BL_BL_BUTTON_TOGGLE_H_
#define BL_BL_BUTTON_TOGGLE_H_

#include "drv_button.h"
#include "drv_led.h"
#include "bl_timer_blink.h"

/// Buttons pint enumeration
typedef enum {
    blButton1Pin = drvButtonPin0    /**< Button 1 pin*/
} blButtonPins;

/// Buttons port enumeration
typedef enum {
    blButton1Port = drvButtonPortA  /**< Button 1 port*/
} blButtonPort;

/// Structure of button data
typedef struct {
    blButtonPins pin;       /**< Button pin*/
    blButtonPort port;      /**< Button port */
    uint8_t numberPress;    /**< Pin 0 */
} blButtonData;

/// Buttons enumeration
enum
{
    blButton1 = 0,  /**< First button */
    blButtonCount
};

/**
 * Initial function for led toggle on button.
 * @param button pointer on blButtonData structure
 * @param led pointer on ledData structure
 */
void blButtonLedToggleInit(blButtonData* button,ledData* led);

/**
 * Button for physical click detection
 * @param button pointer on blButtonData structure
 */
uint8_t blButtonIsClick(blButtonData* button);

/**
 * Function for one step of main loop for led toggle BL
 * @param button pointer on blButtonData structure
 * @param led pointer on ledData structure
 */
void blToggleLedIfButtonClick(blButtonData* button,ledData* led);

#endif /* BL_BL_BUTTON_TOGGLE_H_ */
