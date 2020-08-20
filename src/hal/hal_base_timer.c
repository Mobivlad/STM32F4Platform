/*
 * hal_base_timer.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "hal_base_timer.h"

const static halTimer_def_t timer_table[halBaseTimer_Count] = {
        {TIM6, RCC_APB1Periph_TIM6, TIM6_DAC_IRQn},
        {TIM7, RCC_APB1Periph_TIM7, TIM7_IRQn}
};

static halTimer_struct* structPointers[halBaseTimer_Count];

/**
 * Set Timer callback for timer
 * @param timer value of halBaseTimers enumeration
 */
void halBaseTimerSetTimerCallback(halTimer_struct* timerStruct, halTimerCallbackType callback) {
    timerStruct->callBack = callback;
}

void halBaseTimerInit(halTimer_struct* timerStruct, halInitBaseTimerStruct* halInitStruct) {

    timerStruct->timerID = halInitStruct->timer;

    RCC_APB1PeriphClockCmd(timer_table[timerStruct->timerID].timer_rcc, ENABLE);

    TIM_TimeBaseInitTypeDef* initStruct = (TIM_TimeBaseInitTypeDef*)timerStruct;
    initStruct->TIM_Period = halInitStruct->halBaseTimerPeriod;
    initStruct->TIM_CounterMode = TIM_CounterMode_Up;
    initStruct->TIM_ClockDivision = halInitStruct->halBaseTimersClockDivision;
    initStruct->TIM_Prescaler = halInitStruct->halBaseTimerPrescaler;
    initStruct->TIM_RepetitionCounter = 0x00;

    TIM_TimeBaseInit(timer_table[timerStruct->timerID].timer, initStruct);

    TIM_ClearITPendingBit(timer_table[timerStruct->timerID].timer, TIM_IT_Update);
    TIM_ITConfig(timer_table[timerStruct->timerID].timer, TIM_IT_Update, ENABLE);
    NVIC_EnableIRQ(timer_table[timerStruct->timerID].timer_irq);

    structPointers[timerStruct->timerID] = timerStruct;

}

void halBaseTimerDeinit(halTimer_struct* timerStruct) {

	structPointers[timerStruct->timerID]->callBack = NULL;

	NVIC_DisableIRQ(timer_table[timerStruct->timerID].timer_irq);
	TIM_ITConfig(timer_table[timerStruct->timerID].timer, TIM_IT_Update, DISABLE);

	TIM_DeInit(timer_table[timerStruct->timerID].timer);

	RCC_APB1PeriphClockCmd(timer_table[timerStruct->timerID].timer_rcc, DISABLE);

    structPointers[timerStruct->timerID] = NULL;
}


void halBaseTimerStart(halTimer_struct* timerStruct) {
    TIM_Cmd(timer_table[timerStruct->timerID].timer, ENABLE);
}

void halBaseTimerStop(halTimer_struct* timerStruct) {
    TIM_Cmd(timer_table[timerStruct->timerID].timer, DISABLE);
}

/**
 * Increment counter function for interrupts
 * @param timer value of halBaseTimers enumeration
 */
static void halBaseTimerIncrementCounter(halBaseTimer timer) {
    TIM_TypeDef* haltimer = timer_table[timer].timer;

    if (TIM_GetITStatus(haltimer, TIM_IT_Update) != RESET) {

        halBaseTimerStop(structPointers[timer]);

        halTimerCallbackType callbackFunction = structPointers[timer]->callBack;

        if (callbackFunction != 0) {
            callbackFunction();
        }

        TIM_ClearITPendingBit(haltimer, TIM_IT_Update);
    }
}

/**
 * Interrupt function for TIM6.
 */
void TIM6_DAC_IRQHandler(void)
{
    halBaseTimerIncrementCounter(halBaseTimer6);
}

/**
 * Interrupt function for TIM7.
 */
void TIM7_IRQHandler(void)
{
    halBaseTimerIncrementCounter(halBaseTimer7);
}
