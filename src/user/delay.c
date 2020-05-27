/*
 * delay.c
 *
 *  Created on: 26 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "delay.h"

//variable for delay timer
volatile u32 delayCount;
void SysTick_Handler(void){
	if(delayCount)delayCount--;
}

void initDelay(void){
	//update SystemCoreClock value
	SystemCoreClockUpdate();
	//config SysTick on 1ms
	SysTick_Config(SystemCoreClock/1000);
}

void delayMs(u32 ms){
	delayCount = ms;
	while(delayCount);
}
