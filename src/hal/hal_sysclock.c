/*
 * hal_sysclock.c
 *
 *  Created on: 29 . 2020 .
 *      Author: vladyslav.daliavskyi
 */
#include "hal_sysclock.h"

volatile uint32_t milis;

void halSysClockInit(u32 ms){
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/1000*ms);
    milis = 0;
}

u32 halGetTick(){
    return milis;
}

void halSetTick(u32 ms){
    milis = ms;
}

void SysTick_Handler(void){
    milis++;
}

