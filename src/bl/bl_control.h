/*
 * blCommand.h
 *
 *  Created on: Jul 18, 2020
 *      Author: Sigma
 */

#ifndef BL_BL_CONTROL_H_
#define BL_BL_CONTROL_H_

#include <string.h>
#include <stdio.h>

#include "drv_com_port.h"
#include "drv_button.h"

#define WELCOME_PHRAZE              "COMPORT is ready to listen commands.\r\n"
#define OK_PHRAZE                   "OK.\r\n"
#define INCORRECT_PARAMETER_PHRAZE  "Incorrect parameter.\r\n"
#define INVALID_COMMAND_PHRAZE      "Invalid command.\r\n"

#define SET_LIMIT_COMMAND           "set_limit"
#define CLEAR_LIMIT_COMMAND         "clear_limit"

#define BUFFER_SIZE                 255
#define SET_LIMIT_VARS_COUNT        2
#define CLEAR_LIMIT_VARS_COUNT      1

typedef struct {
    drvButton_struct    button;
    drvCOMPort_struct   comPort;

    char uartBuffer[BUFFER_SIZE];

    void(*clearLimitAction)();
    void(*setLimitAction)(uint8_t);

} blCommand_struct;

void blCommandInit(blCommand_struct* commandStruct);
void blCommandChangeClickAction(blCommand_struct* commandStruct, void(*action)());
void blCommandChangeLongClickAction(blCommand_struct* commandStruct, void(*action)());
void blCommandChangeClearAction(blCommand_struct* commandStruct, void(*action)());
void blCommandChangeSetLimitAction(blCommand_struct* commandStruct, void(*action)(uint8_t));
void blCommandRun(blCommand_struct* commandStruct);

#endif /* BL_BL_CONTROL_H_ */
