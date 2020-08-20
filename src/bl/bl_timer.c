/*
 * bl_timer.c
 *
 *  Created on: Jul 18, 2020
 *      Author: Sigma
 */

#include "bl_timer.h"

static blTimer_struct* current_timer;

static void timer_callback() {
    current_timer->currentValue++;
    uint16_t reloadVal = BYTE_ARRAY_TO_U16(current_timer->reloadValue);
    if (current_timer->currentValue > reloadVal) {
        current_timer->currentValue = 0;
    }
    drvSSDisplaySetValue(current_timer->currentValue);
    drvTimerStart(&(current_timer->timer));
}

static uint8_t blTimerChangeState(blTimer_struct* timer) {
    if (timer->state == PAUSE) {
        timer->state = RUN;
        drvTimerStart(&(timer->timer));
    } else {
        timer->state = PAUSE;
        drvTimerStop(&(timer->timer));
    }
    return blTimer_executeOK;
}

static uint8_t blTimerReloadTimer(blTimer_struct* timer) {
    if (timer->state == PAUSE) {
        timer->currentValue = 0;
        drvSSDisplaySetValue(timer->currentValue);
    } else {
        return blTimer_executeERROR;
    }
    return blTimer_executeOK;
}

static void readValueFromFRAM(blTimer_struct* timer_struct, uint8_t* dest, bool waitEnd) {
    drvFRAMReadData(&(timer_struct->fram), LIMIT_ADDRESS, dest, LIMIT_VALUE_ARRAY_SIZE);
    if (waitEnd) {
        while (drvFRAMGetState(&(timer_struct->fram)) != STATE_READY) {
            drvFRAMRun(&(timer_struct->fram));
        }
    }
}

static void writeValueToFRAM(blTimer_struct* timer_struct, uint8_t* src, uint8_t waitEnd) {
    drvFRAMWriteData(&(timer_struct->fram), LIMIT_ADDRESS, src, LIMIT_VALUE_ARRAY_SIZE);
    if (waitEnd) {
        while (drvFRAMGetState(&(timer_struct->fram)) != STATE_READY) {
            drvFRAMRun(&(timer_struct->fram));
        }
    }
}

static uint8_t blTimerSetLimit(int limit) {
    if (current_timer->state == RUN) {
        return blTimer_executeERROR;
    }
    if (limit > TIMER_MAX || limit < 0) {
        return blTimer_executeINCORRECT_VAL;
    }
    U16_TO_BYTE_ARRAY(limit,current_timer->reloadValue);
    writeValueToFRAM(current_timer, current_timer->reloadValue, false);
    return blTimer_executeOK;
}

static uint8_t blTimerClearLimit() {
    if (current_timer->state == RUN) {
        return blTimer_executeERROR;
    }
    U16_TO_BYTE_ARRAY(TIMER_MAX,current_timer->reloadValue);
    writeValueToFRAM(current_timer, current_timer->reloadValue, false);
    return blTimer_executeOK;
}

uint8_t blTimerExecuteCommand(blTimer_struct* timerStruct, blTimer_command command, void* param) {
    if (timerStruct == NULL) {
        return blTimer_executeFATAL_ERROR;
    }
    switch (command) {
        case changeStateCommand:
            return blTimerChangeState(timerStruct);
        case reloadCommand:
            return blTimerReloadTimer(timerStruct);
        case setLimitCommand:
            if (param == NULL) return blTimer_executeFATAL_ERROR;
            return blTimerSetLimit(*(uint16_t*)param);
        case clearLimitCommand:
            return blTimerClearLimit();
        default:
            return blTimer_executeFATAL_ERROR;
    }
}

void blTimerInit(blTimer_struct* timer_struct) {
    // Initialization 7segmant display
    drvSSDisplayInit();
    drvFRAMInit(&(timer_struct->fram));

    if (timer_struct->fram.protectionLevel == drvBP3) {
        drvFRAMSetBP(&(timer_struct->fram), drvBP2);
    }
    uint8_t limitArr[LIMIT_VALUE_ARRAY_SIZE];
    readValueFromFRAM(timer_struct, limitArr, true);
    uint16_t limit = BYTE_ARRAY_TO_U16(limitArr);
    if (limit == DEFAULT_DATA) {
        U16_TO_BYTE_ARRAY(TIMER_MAX, limitArr);
        writeValueToFRAM(timer_struct, limitArr, true);
    }

    timer_struct->currentValue = 0;
    U16_TO_BYTE_ARRAY(limit, timer_struct->reloadValue);

    drvTimerInit(&(timer_struct->timer), timer_callback);
    drvSSDisplaySetValue(timer_struct->currentValue);

    timer_struct->state = PAUSE;

    current_timer = timer_struct;
}

void blTimerRun(blTimer_struct* timer_struct) {
    drvFRAMRun(&(timer_struct->fram));
}
