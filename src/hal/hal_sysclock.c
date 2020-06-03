/*
 * hal_sysclock.c
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "hal_sysclock.h"

volatile uint32_t hal_ms_counter;
static uint8_t inited = 0;
void halSysClockInit(uint32_t ms) {
    if(!inited){
        SystemCoreClockUpdate();
        SysTick_Config(SystemCoreClock / 1000 * ms);
        hal_ms_counter = 0;
        inited = 1;
    }

}

uint32_t halSysClockGetTick() {
    return hal_ms_counter;
}

void SysTick_Handler(void) {
    hal_ms_counter++;
}

