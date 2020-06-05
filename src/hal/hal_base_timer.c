/*
 * hal_base_timer.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "hal_base_timer.h"


static halTimerCallbackType halTimersCallBacks[] = { 0 };

/**
 * Get Timer by halBaseTimers enumeration
 * @param timer value of halBaseTimers enumeration
 */
static TIM_TypeDef* halBaseTimerGetTimerByEnum(halBaseTimers timer){
    switch (timer) {
        case halBaseTimer6:
            return TIM6;
        case halBaseTimer7:
            return TIM7;
    }
    return 0;
}

/**
 * Get Timer callback by halBaseTimers enumeration
 * @param timer value of halBaseTimers enumeration
 */
static halTimerCallbackType halBaseTimerGetTimerCallbackByEnum(halBaseTimers timer){
    switch (timer) {
        case halBaseTimer6:
            return halTimersCallBacks[0];
        case halBaseTimer7:
            return halTimersCallBacks[1];
    }
    return 0;
}

/**
 * Set Timer callback for timer
 * @param timer value of halBaseTimers enumeration
 */
static void halBaseTimerSetTimerCallbackByEnum(halBaseTimers timer, halTimerCallbackType callback){
    switch (timer) {
        case halBaseTimer6:
            halTimersCallBacks[0] = callback;
            return;
        case halBaseTimer7:
            halTimersCallBacks[1] = callback;
            return;
    }
    return;
}

/**
 * Enable timer RCC function
 * @param timer value of halBaseTimers enumeration
 */
static void halBaseTimerEnableRCCByEnum(halBaseTimers timer){
    switch (timer) {
        case halBaseTimer6:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            return;
        case halBaseTimer7:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
            return;
    }
    return;
}

/**
 * Enable timer interrupts function
 * @param timer value of halBaseTimers enumeration
 */
static void halBaseTimerEnableIRnQByEnum(halBaseTimers timer){
    TIM_ITConfig(halBaseTimerGetTimerByEnum(timer), TIM_IT_Update, ENABLE);
    switch (timer) {
        case halBaseTimer6:
            NVIC_EnableIRQ(TIM6_DAC_IRQn);
            return;
        case halBaseTimer7:
            NVIC_EnableIRQ(TIM7_IRQn);
            return;
    }
    return;
}

/**
 * Disable timer interrupts function
 * @param timer value of halBaseTimers enumeration
 */
static void halBaseTimerDisableIRnQByEnum(halBaseTimers timer){
    TIM_ITConfig(halBaseTimerGetTimerByEnum(timer), TIM_IT_Update, ENABLE);
    switch (timer) {
        case halBaseTimer6:
            NVIC_DisableIRQ(TIM6_DAC_IRQn);
            return;
        case halBaseTimer7:
            NVIC_DisableIRQ(TIM7_IRQn);
            return;
    }
    return;
}


void halBaseTimerInit(halBaseTimers timer,
        halInitBaseTimerStruct* halInitStruct,
        halTimerCallbackType callback) {
    halBaseTimerEnableRCCByEnum(timer);
    TIM_TimeBaseInitTypeDef initStruct;
    initStruct.TIM_Period = halInitStruct->halBaseTimerPeriod;
    initStruct.TIM_CounterMode = TIM_CounterMode_Up;
    initStruct.TIM_ClockDivision = halInitStruct->halBaseTimersClockDivision;
    initStruct.TIM_Prescaler = halInitStruct->halBaseTimerPrescaler;
    initStruct.TIM_RepetitionCounter = 0x00;
    TIM_TypeDef* initialTimer = halBaseTimerGetTimerByEnum(timer);
    TIM_TimeBaseInit(initialTimer, &initStruct);
    TIM_ClearITPendingBit(initialTimer, TIM_IT_Update);
    halBaseTimerSetTimerCallbackByEnum(timer,callback);
    halBaseTimerEnableIRnQByEnum(timer);

}

void halBaseTimerStart(halBaseTimers timer){
    TIM_Cmd(halBaseTimerGetTimerByEnum(timer),ENABLE);
}

void halBaseTimerStop(halBaseTimers timer){
    TIM_Cmd(halBaseTimerGetTimerByEnum(timer),DISABLE);
}

/**
 * Increment counter function for interrupts
 * @param timer value of halBaseTimers enumeration
 */
static void halBaseTimerIncrementCounter(halBaseTimers timer) {
    TIM_TypeDef* haltimer = halBaseTimerGetTimerByEnum(timer);
    if (TIM_GetITStatus(haltimer, TIM_IT_Update) != RESET) {
        halBaseTimerStop(timer);
        halTimerCallbackType callbackFunction =
                halBaseTimerGetTimerCallbackByEnum(timer);
        if (callbackFunction != 0)
            callbackFunction();
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
