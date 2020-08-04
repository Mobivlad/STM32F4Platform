/*
 * bl_timer.c
 *
 *  Created on: Jul 18, 2020
 *      Author: Sigma
 */

#include "bl_timer.h"

blTimer_struct* current_timer;

static void timer_callback() {
    current_timer->currentValue++;
    if (current_timer->currentValue > current_timer->reloadValue) {
        current_timer->currentValue = 0;
    }
    drvSSDisplaySetValue(current_timer->currentValue);
    drvTimerStart(&(current_timer->timer));
}

void blTimerButtonClickAction() {
    if (current_timer->state == PAUSE) {
        current_timer->state = RUN;
        drvTimerStart(&(current_timer->timer));
    } else {
        current_timer->state = PAUSE;
        drvTimerStop(&(current_timer->timer));
    }
}

void blTimerButtonLongClickAction() {
    if (current_timer->state == PAUSE) {
        current_timer->currentValue = 0;
        drvSSDisplaySetValue(current_timer->currentValue);
    } else {
        current_timer->state = PAUSE;
        drvTimerStop(&(current_timer->timer));
    }
}

void blTimer_init(blTimer_struct* timer_struct) {
    drvFRAMInit(&(timer_struct->fram));
    drvSSDisplayInit();

    if (timer_struct->fram.protectionLevel == drvBP3) {
        drvFRAMSetBP(&(timer_struct->fram), drvBP2);
    }
    uint8_t limit;
    drvFRAMReadData(&(timer_struct->fram), LIMIT_ADDRESS, &limit, 1);
    while (drvFRAMGetState(&(timer_struct->fram)) != STATE_READY) {
        drvFRAMRun(&(timer_struct->fram));
    }

    if (limit == DEFAULT_DATA) {
        limit = TIMER_MAX;
        drvFRAMWriteData(&(timer_struct->fram), LIMIT_ADDRESS, &limit, 1);
        while (drvFRAMGetState(&(timer_struct->fram)) != STATE_READY) {
            drvFRAMRun(&(timer_struct->fram));
        }
    }

    timer_struct->currentValue = 0;
    timer_struct->reloadValue = limit;

    current_timer = timer_struct;

    drvTimerInit(&(timer_struct->timer), timer_callback);
    drvSSDisplaySetValue(timer_struct->currentValue);

    //blTimer_state = PAUSE;
}

void blTimer_Run(blTimer_struct* timer_struct) {
    drvFRAMRun(&(timer_struct->fram));
}
