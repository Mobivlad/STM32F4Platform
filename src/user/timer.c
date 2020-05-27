/*
 * timer.c
 *
 *  Created on: 26 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "timer.h"

void initTimer(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitTypeDef initStruct;
	//config on 42MHz
	initStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	//config on 1 ms
	initStruct.TIM_Prescaler = 41999;
	//config reload on 0
	initStruct.TIM_Period = 500;
	//IT on every Update action
	initStruct.TIM_RepetitionCounter = 0;
	//no metter
	initStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6,&initStruct);
	//enable 0.5s IT
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	//start timer
	TIM_Cmd(TIM6, ENABLE);

	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	NVIC_SetPriority(TIM6_DAC_IRQn, 0x0);
}
