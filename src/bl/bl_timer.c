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
    if (current_timer->currentValue > current_timer->reloadValue) {
        current_timer->currentValue = 0;
    }
    drvSSDisplaySetValue(current_timer->currentValue);
    drvTimerStart(&(current_timer->timer));
}

void blTimerHandler() {
    if (current_timer->state == PAUSE) {
        current_timer->state = RUN;
        drvTimerStart(&(current_timer->timer));
    } else {
        current_timer->state = PAUSE;
        drvTimerStop(&(current_timer->timer));
    }
}

void blTimerReloadHandler() {
    if (current_timer->state == PAUSE) {
        current_timer->currentValue = 0;
        drvSSDisplaySetValue(current_timer->currentValue);
    } else {
        current_timer->state = PAUSE;
        drvTimerStop(&(current_timer->timer));
    }
}

static void readValueFromFRAM(blTimer_struct* timer_struct, uint8_t* dest, uint8_t waitEnd) {
    drvFRAMReadData(&(timer_struct->fram), LIMIT_ADDRESS, dest, 1);
    if (waitEnd) {
        while (drvFRAMGetState(&(timer_struct->fram)) != STATE_READY) {
            drvFRAMRun(&(timer_struct->fram));
        }
    }
}

static void writeValueToFRAM(blTimer_struct* timer_struct, uint8_t* src, uint8_t waitEnd) {
    drvFRAMWriteData(&(timer_struct->fram), LIMIT_ADDRESS, src, 1);
    if (waitEnd) {
        while (drvFRAMGetState(&(timer_struct->fram)) != STATE_READY) {
            drvFRAMRun(&(timer_struct->fram));
        }
    }
}

uint8_t blTimerSetLimit(uint8_t limit) {
    if (current_timer->state == RUN) {
        return 1;
    }
    current_timer->reloadValue = limit;
    writeValueToFRAM(current_timer, &current_timer->reloadValue, 0);
    return 0;
}

uint8_t blTimerClearLimit() {
    if (current_timer->state == RUN) {
        return 1;
    }
    current_timer->reloadValue = 0xFF;
    writeValueToFRAM(current_timer, &current_timer->reloadValue, 0);
    return 0;
}

void blTimerInit(blTimer_struct* timer_struct) {

    drvSSDisplayInit();
    drvFRAMInit(&(timer_struct->fram));

    if (timer_struct->fram.protectionLevel == drvBP3) {
        drvFRAMSetBP(&(timer_struct->fram), drvBP2);
    }
    uint8_t limit;
    readValueFromFRAM(timer_struct, &limit, 1);
    if (limit == DEFAULT_DATA) {
        limit = TIMER_MAX;
        writeValueToFRAM(timer_struct, &limit, 1);
    }

    timer_struct->currentValue = 0;
    timer_struct->reloadValue = limit;

    drvTimerInit(&(timer_struct->timer), timer_callback);
    drvSSDisplaySetValue(timer_struct->currentValue);

    timer_struct->state = PAUSE;

    current_timer = timer_struct;
}

void blTimerRun(blTimer_struct* timer_struct) {
    drvFRAMRun(&(timer_struct->fram));
}
