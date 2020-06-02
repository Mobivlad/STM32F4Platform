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

///leds pins enumeration
typedef enum{
    blRedLedPin = drvLedPin12,      /**< red led pin */
    blGreenLedPin = drvLedPin14     /**< green led pin */
} blLedPins;

///leds ports enumeration
typedef enum{
    blRedLedPort = drvLedPortD,     /**< red led port */
    blGreenLedPort = drvLedPortD    /**< green led port */
} blLedPorts;

///structure for led data
typedef struct {
    blLedPins pin;      /**< led pin */
    blLedPorts port;    /**< led port */
    uint32_t timer;     /**< timer for delay */
} ledData;

///enumeration of leds
enum
{
    blLedRed = 0,   /**< Red led */
    blLedGreen,     /**< Green led */
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
