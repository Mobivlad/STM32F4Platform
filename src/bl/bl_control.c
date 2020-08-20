/*
 * blCommand.c
 *
 *  Created on: Jul 18, 2020
 *      Author: Sigma
 */

#include "bl_control.h"

static drvCOMPort comPort_def = drvCOMPort1;
static drvButton button_def = drvButton1;

static blCommand_struct* currentCommandStruct = NULL;

static void blCOMPortReceiveCallback() {
    char command[BUFFER_SIZE] = { 0 };
    int commandParametr;

    uint8_t readCount = sscanf(currentCommandStruct->uartBuffer, "%s %d", command,
            &commandParametr);

    if (strcmp(command, SET_LIMIT_COMMAND) == 0 && readCount == SET_LIMIT_VARS_COUNT) {

        if (commandParametr > 0) {

            if (currentCommandStruct->setLimitAction != NULL) {
                currentCommandStruct->setLimitAction(commandParametr);
            }
            drvCOMPortWriteString(&(currentCommandStruct->comPort), OK_PHRAZE, NULL);

        } else {
            drvCOMPortWriteString(&(currentCommandStruct->comPort), INCORRECT_PARAMETER_PHRAZE,
            NULL);
        }

    } else if (strcmp(command, CLEAR_LIMIT_COMMAND) == 0 && readCount == CLEAR_LIMIT_VARS_COUNT) {

        if (currentCommandStruct->clearLimitAction != NULL) {

            currentCommandStruct->clearLimitAction();
            drvCOMPortWriteString(&(currentCommandStruct->comPort), OK_PHRAZE, NULL);

        }
    } else {
        drvCOMPortWriteString(&(currentCommandStruct->comPort), INVALID_COMMAND_PHRAZE, NULL);
    }

    memset(currentCommandStruct->uartBuffer, 0, BUFFER_SIZE);
    drvCOMPortReadString(&(currentCommandStruct->comPort), currentCommandStruct->uartBuffer,
            BUFFER_SIZE, blCOMPortReceiveCallback);
}

void blCommandInit(blCommand_struct* commandStruct) {
    currentCommandStruct = commandStruct;

    drvButtonInit(&(commandStruct->button), button_def);
    drvCOMPortInit(&(commandStruct->comPort), comPort_def);

    memset(currentCommandStruct->uartBuffer, 0, BUFFER_SIZE);

    commandStruct->setLimitAction = NULL;
    commandStruct->clearLimitAction = NULL;
    drvCOMPortReadString(&(commandStruct->comPort), currentCommandStruct->uartBuffer, BUFFER_SIZE,
            blCOMPortReceiveCallback);
    drvCOMPortWriteString(&(commandStruct->comPort), WELCOME_PHRAZE, NULL);
}

void blCommandChangeClickAction(blCommand_struct* commandStruct, void (*action)()) {
    commandStruct->button.actionCallbacks[drvButton_Press] = action;
}

void blCommandChangeLongClickAction(blCommand_struct* commandStruct, void (*action)()) {
    commandStruct->button.actionCallbacks[drvButton_LongPress] = action;
}

void blCommandChangeClearAction(blCommand_struct* commandStruct, uint8_t (*action)()) {
    commandStruct->clearLimitAction = action;
}

void blCommandChangeSetLimitAction(blCommand_struct* commandStruct, uint8_t (*action)(uint8_t)) {
    commandStruct->setLimitAction = action;
}

void blCommandRun(blCommand_struct* commandStruct) {
    drvButtonRun(&(commandStruct->button));
}
