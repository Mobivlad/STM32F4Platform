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

#define WELCOME_PHRAZE              "COMPORT is ready to listen commands.\r\n\0"
#define OK_PHRAZE                   "OK.\r\n\0"
#define INCORRECT_PARAMETER_PHRAZE  "Incorrect parameter.\r\n\0"
#define INVALID_COMMAND_PHRAZE      "Invalid command.\r\n\0"

#define SET_LIMIT_COMMAND           "set_limit"
#define CLEAR_LIMIT_COMMAND         "clear_limit"

#define BUFFER_SIZE                 255
#define SET_LIMIT_VARS_COUNT        2
#define CLEAR_LIMIT_VARS_COUNT      1

typedef struct {
    drvButton_struct    button;
    drvCOMPort_struct   comPort;

    char uartBuffer[BUFFER_SIZE];

    uint8_t(*clearLimitAction)();
    uint8_t(*setLimitAction)(uint8_t);

} blCommand_struct;

void blCommandInit(blCommand_struct* commandStruct);
void blCommandChangeClickAction(blCommand_struct* commandStruct, void(*action)());
void blCommandChangeLongClickAction(blCommand_struct* commandStruct, void(*action)());
void blCommandChangeClearAction(blCommand_struct* commandStruct, uint8_t(*action)());
void blCommandChangeSetLimitAction(blCommand_struct* commandStruct, uint8_t(*action)(uint8_t));
void blCommandRun(blCommand_struct* commandStruct);

#endif /* BL_BL_CONTROL_H_ */
