/*
 * blCommand.h
 *
 *  Created on: Jul 18, 2020
 *      Author: Sigma
 */

#ifndef BL_BL_TIMERCONTROLER_H_
#define BL_BL_TIMERCONTROLER_H_

#include <string.h>
#include <stdio.h>

#include "drv_com_port.h"
#include "drv_button.h"

#include "bl_timer.h"

//@TODO Check if \0 are necessary
#define WELCOME_PHRAZE              "COMPORT is ready to listen commands.\r\n\0"
#define OK_PHRAZE                   "OK.\r\n\0"
#define TIMER_IN_RUN_PHRAZE         "Cannot reload timer in run.\r\n\0"
#define INCORRECT_PARAMETER_PHRAZE  "Incorrect parameter.\r\n\0"
#define INVALID_COMMAND_PHRAZE      "Invalid command.\r\n\0"
#define TOO_BIG_VAL_PHRAZE          "Reload value is too big.\r\n\0"

#define SET_LIMIT_COMMAND           "set_limit"
#define CLEAR_LIMIT_COMMAND         "clear_limit"

#define BUFFER_SIZE                 255
#define SET_LIMIT_VARS_COUNT        2
#define CLEAR_LIMIT_VARS_COUNT      1

typedef struct {
    drvButton_struct    button;
    drvCOMPort_struct   comPort;

    char uartBuffer[BUFFER_SIZE];

    blTimer_struct* controlTimer;

} blController_struct;

void blControllerInit(blController_struct* commandStruct, blTimer_struct* timer);

void blControllerRun(blController_struct* commandStruct);

#endif /* BL_BL_TIMERCONTROLER_H_ */
