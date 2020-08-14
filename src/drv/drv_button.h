/*
 * drv_button.h
 *
 *  Created on: 25 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "hal_gpio.h"

#include <stddef.h>

#ifndef DRV_DRV_BUTTON_H_
#define DRV_DRV_BUTTON_H_

typedef enum {
    drvButton1,
    drvButton_Count
} drvButton;

typedef struct {
    halGPIO_port    port;
    halGPIO_pin     pin;
} drvButton_data;

enum {
    drvButton_Clicked = 0,
    drvButton_Float,
    drvButton_Falled
};

/// Duration of button presses
typedef enum {
    drvButtonNoPressDuration        = 0,
    drvButtonSimplePressDuration    = 50,
    drvButtonLongPressDuration      = 1000,
} drvButton_clickDuration;

enum {
    drvButtonLowLogicLevelLimit     = 10,
    drvButtonFloatLimit             = 100,
    drvButtonHighLogicLevelLimit    = 110
};

/// Duration of button presses
typedef enum {
    drvButton_NoAction = 0,     /**< Simple pressing. */
    drvButton_Press,            /**< Simple pressing. */
    drvButton_LongPress,        /**< Long pressing. */
    drvButton_ActionsCount
} drvButton_action;

typedef void (*drvButtonActionCallBack)();

/// Button initial structure
typedef struct {
    halGPIO_struct          halGPIO;
    drvButton               button;

    uint32_t                click_counter;
    uint32_t                click_duration;

    drvButtonActionCallBack actionCallbacks[drvButton_ActionsCount];
} drvButton_struct;

/**
 * Function that configurate button.
 * @param buttonStruct pointer on structure with data of button
 */
void drvButtonInit(drvButton_struct* const buttonStruct);

/**
 * Set action callback function
 * @param buttonStruct pointer on structure with data of button
 * @param action action value of drvButton_action enumeration
 * @param callback action callback
 */
void drvButtonSetCallBack(drvButton_struct* const buttonStruct, const drvButton_action action,
        const drvButtonActionCallBack callback);

/**
 * Run function for button
 * @param buttonStruct pointer on structure with data of button
 */
void drvButtonRun(drvButton_struct* const buttonStruct);

#endif /* DRV_DRV_BUTTON_H_ */
