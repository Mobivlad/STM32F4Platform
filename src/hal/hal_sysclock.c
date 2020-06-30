/*
 * hal_sysclock.c
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "hal_sysclock.h"

volatile uint32_t halSysTick_msCounter;

static uint8_t inited = 0;

void halSysClockInit() {
    if (!inited) {
        SystemCoreClockUpdate();
        SysTick_Config(SystemCoreClock / 1000);

        halSysTick_msCounter = 0;
        inited = 1;
    }
}

uint32_t halSysClockGetTick() {
    return halSysTick_msCounter;
}

void SysTick_Handler(void) {
    halSysTick_msCounter++;
}

