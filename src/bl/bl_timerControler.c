/*
 * blCommand.c
 *
 *  Created on: Jul 18, 2020
 *      Author: Sigma
 */

#include "bl_timerControler.h"

static drvCOMPort comPort_def = drvCOMPort1;
static drvButton button_def = drvButton1;

static blController_struct* currentCommandStruct = NULL;

static void blCOMPortReceiveCallback() {
    char command[BUFFER_SIZE] = { 0 };
    int commandParametr;

    uint8_t readCount = sscanf(currentCommandStruct->uartBuffer, "%s %d", command,
            &commandParametr);

    if (strcmp(command, SET_LIMIT_COMMAND) == 0 && readCount == SET_LIMIT_VARS_COUNT) {
        if (commandParametr > 0) {
            switch (blTimerExecuteCommand(currentCommandStruct->controlTimer, setLimitCommand,
                    &commandParametr)) {
                case blTimer_executeOK:
                    drvCOMPortWriteString(&(currentCommandStruct->comPort), OK_PHRAZE, NULL);
                    break;
                case blTimer_executeERROR:
                    drvCOMPortWriteString(&(currentCommandStruct->comPort), TIMER_IN_RUN_PHRAZE,
                            NULL);
                    break;
                case blTimer_executeINCORRECT_VAL:
                    drvCOMPortWriteString(&(currentCommandStruct->comPort), TOO_BIG_VAL_PHRAZE,
                            NULL);
                    break;
                default:
                    break;
            }
        } else {
            drvCOMPortWriteString(&(currentCommandStruct->comPort), INCORRECT_PARAMETER_PHRAZE,
            NULL);
        }
    } else if (strcmp(command, CLEAR_LIMIT_COMMAND) == 0 && readCount == CLEAR_LIMIT_VARS_COUNT) {
        switch (blTimerExecuteCommand(currentCommandStruct->controlTimer, clearLimitCommand, NULL)) {
            case blTimer_executeOK:
                drvCOMPortWriteString(&(currentCommandStruct->comPort), OK_PHRAZE, NULL);
                break;
            case blTimer_executeERROR:
                drvCOMPortWriteString(&(currentCommandStruct->comPort), TIMER_IN_RUN_PHRAZE, NULL);
                break;
            default:
                break;
        }
    } else {
        drvCOMPortWriteString(&(currentCommandStruct->comPort), INVALID_COMMAND_PHRAZE, NULL);
    }

    memset(currentCommandStruct->uartBuffer, 0, BUFFER_SIZE);
    drvCOMPortReadString(&(currentCommandStruct->comPort), currentCommandStruct->uartBuffer,
            BUFFER_SIZE, blCOMPortReceiveCallback);
}

static void blControllerButtonClickAction() {
    blTimerExecuteCommand(currentCommandStruct->controlTimer, changeStateCommand, NULL);
}

static void blControllerButtonLongClickAction() {
    blTimerExecuteCommand(currentCommandStruct->controlTimer, reloadCommand, NULL);
}

void blControllerInit(blController_struct* commandStruct, blTimer_struct* timer) {
    commandStruct->controlTimer = timer;
    currentCommandStruct = commandStruct;

    drvButtonInit(&(commandStruct->button), button_def);
    drvCOMPortInit(&(commandStruct->comPort), comPort_def);

    memset(currentCommandStruct->uartBuffer, 0, BUFFER_SIZE);

    drvButtonSetCallBack(&(commandStruct->button), drvButton_Press, blControllerButtonClickAction);
    drvButtonSetCallBack(&(commandStruct->button), drvButton_LongPress,
            blControllerButtonLongClickAction);

    drvCOMPortReadString(&(commandStruct->comPort), currentCommandStruct->uartBuffer, BUFFER_SIZE,
            blCOMPortReceiveCallback);
    drvCOMPortWriteString(&(commandStruct->comPort), WELCOME_PHRAZE, NULL);
}

void blControllerRun(blController_struct* commandStruct) {
    drvButtonRun(&(commandStruct->button));
}
