/*
 * bl_timer.h
 *
 *  Created on: Jul 18, 2020
 *      Author: Sigma
 */

#ifndef BL_BL_TIMER_H_
#define BL_BL_TIMER_H_

#include <stdbool.h>

#include "drv_7segment_display.h"
#include "drv_fm25l16b.h"
#include "drv_timer.h"

#define LIMIT_ADDRESS   0x0000
#define DEFAULT_DATA    0x00
#define TIMER_MAX       0x3E7

#define LIMIT_VALUE_ARRAY_SIZE  2
#define BYTE_SHIFT      8

#define BYTE_ARRAY_TO_U16(x) (x[1] << BYTE_SHIFT | x[0])
#define U16_TO_BYTE_ARRAY(x, arr) arr[0] = x & 0xFF; arr[1] = (x >> BYTE_SHIFT) & 0xFF

typedef enum {
    RUN = 0,
    PAUSE
} blTimer_state;

typedef enum {
    changeStateCommand,
    reloadCommand,
    setLimitCommand,
    clearLimitCommand
} blTimer_command;


typedef enum {
    blTimer_executeOK,
    blTimer_executeERROR,
    blTimer_executeINCORRECT_VAL,
    blTimer_executeFATAL_ERROR
} blTimer_executeError;

typedef struct {
    drvTimer_struct timer;
    drvFRAM_struct  fram;

    // reload value is saved in view of 2 bytes
    uint8_t         reloadValue[LIMIT_VALUE_ARRAY_SIZE];
    uint16_t        currentValue;

    blTimer_state   state;
} blTimer_struct;

void blTimerInit(blTimer_struct* timer_struct);

uint8_t blTimerExecuteCommand(blTimer_struct* timerStruct, blTimer_command command, void* param);

void blTimerRun(blTimer_struct* timer_struct);

#endif /* BL_BL_TIMER_H_ */
