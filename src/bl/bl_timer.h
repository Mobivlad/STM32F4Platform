/*
 * bl_timer.h
 *
 *  Created on: Jul 18, 2020
 *      Author: Sigma
 */

#ifndef BL_BL_TIMER_H_
#define BL_BL_TIMER_H_

#include "drv_7segment_display.h"
#include "drv_fm25l16b.h"
#include "drv_timer.h"

#define LIMIT_ADDRESS   0x0000
#define DEFAULT_DATA    0x00
#define TIMER_MAX       0xFF

typedef enum {
    RUN,
    PAUSE
} blTimer_state;

typedef struct {
    drvTimer_struct timer;
    drvFRAM_struct  fram;

    uint8_t         reloadValue;
    uint8_t         currentValue;

    blTimer_state   state;
} blTimer_struct;

void blTimer_init(blTimer_struct* timer_struct);

#endif /* BL_BL_TIMER_H_ */
