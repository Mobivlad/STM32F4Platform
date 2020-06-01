/*
 * bl_timer_blink.h
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef BL_BL_TIMER_BLINK_H_
#define BL_BL_TIMER_BLINK_H_

#include "drv_led.h"
#include "drv_sysclock.h"

typedef enum{
    blRedLedPin = drvLedPin12,
    blGreenLedPin = drvLedPin14
} blLedPins;

typedef enum{
    blRedLedPort = drvLedPortD,
    blGreenLedPort = drvLedPortD
} blLedPorts;

typedef struct {
    blLedPins pin;
    blLedPorts port;
    uint32_t timer;
} ledData;

enum
{
    blLedRed = 0,
    blLedGreen,
    blLedCount
};

/**
 * Initial function for blink.
 * @param value of blink led
 */
void blInitBlink(ledData* led);

/**
 * Function for one step of blink.
 * @param value of blink led
 * @param time count of time between operations in default tick time
 */
void blBlink(ledData* led,uint32_t time);

#endif /* BL_BL_TIMER_BLINK_H_ */
