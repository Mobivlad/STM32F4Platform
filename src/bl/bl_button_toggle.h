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

typedef enum {
    blButton1Pin = drvButtonPin0
} blButtonPins;

typedef enum {
    blButton1Port = drvButtonPortA
} blButtonPort;

typedef struct {
    blButtonPins pin;
    blButtonPort port;
    uint8_t numberPress;
} blButtonData;

enum
{
    blButton1 = 0,
    blButtonCount
};

void blButtonLedToggleInit(blButtonData* button,ledData* led);

uint8_t blIsButtonClick(blButtonData* button);

void blToggleIfClick(blButtonData* button,ledData* led);

#endif /* BL_BL_BUTTON_TOGGLE_H_ */
