/*
 * drv_button.h
 *
 *  Created on: 25 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "hal_gpio.h"

#ifndef DRV_DRV_BUTTON_H_
#define DRV_DRV_BUTTON_H_

typedef enum {
    drvButton1,
    drvButton_Count
} drvButton;

typedef struct {
    halGPIO_port port;
    halGPIO_pin pin;
} drvButton_data;

enum {
    drvButton_Clicked = 0,
    drvButton_Float,
    drvButton_Falled
};

/// Duration of button presses
typedef enum {
    drvButtonNoPressDuration = 0,               /**< No pressing. */
    drvButtonSimplePressDuration = 5000,       /**< Simple pressing. */
    drvButtonLongPressDuration = 50000,        /**< Long pressing. */
} drvButton_ClickDuration;

/// Duration of button presses
typedef enum {
    drvButton_NoAction = 0,       /**< Simple pressing. */
    drvButton_Press,       /**< Simple pressing. */
    drvButton_LongPress,       /**< Long pressing. */
    drvButton_ActionsCount
} drvButton_action;

typedef void(*drvButtonActionCallBack)();

/// Button initial structure
typedef struct {
    halGPIO_struct halGPIO;
    drvButton button;
    drvButtonActionCallBack actionCallbacks[drvButton_ActionsCount];
    uint32_t click_counter;
    uint32_t click_duration;
} drvButton_struct;

/**
 * Function that configurate button.
 * @param buttonStruct pointer on structure with data of button
 */
void drvButton_init(drvButton_struct* buttonStruct);

/**
 * Set action callback function
 * @param buttonStruct pointer on structure with data of button
 * @param action action value of drvButton_action enumeration
 * @param callback action callback
 */
void drvButton_setCallBack(drvButton_struct* buttonStruct,
        drvButton_action action, drvButtonActionCallBack callback);

/**
 * Run function for button
 * @param buttonStruct pointer on structure with data of button
 */
void drvButton_Run(drvButton_struct* buttonStruct);

#endif /* DRV_DRV_BUTTON_H_ */
