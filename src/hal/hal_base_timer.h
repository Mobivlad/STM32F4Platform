/*
 * hal_base_timer.h
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_BASE_TIMER_H_
#define HAL_HAL_BASE_TIMER_H_

#include "stm32f4xx.h"
#include "stddef.h"

/// Callback function for timers
typedef void(*halTimerCallbackType)(void);

/// Base timers enumeration
typedef enum {
    halBaseTimer6 = 0,         /**< Base timer 0 */
    halBaseTimer7,             /**< Base timer 1 */
    halBaseTimer_Count
} halBaseTimer;

typedef struct {
    TIM_TimeBaseInitTypeDef timerDef;

    halBaseTimer            timerID;

    halTimerCallbackType    callBack;

} halTimer_struct;

typedef struct {
    TIM_TypeDef *   timer;
    uint32_t        timer_rcc;
    IRQn_Type       timer_irq;
} halTimer_def_t;

/// Timer clock division
typedef enum {
    halBaseTimerClockDivision1 = TIM_CKD_DIV1,        /**< Timer clock div 1 */
    halBaseTimerClockDivision2 = TIM_CKD_DIV2,        /**< Timer clock div 2 */
    halBaseTimerClockDivision4 = TIM_CKD_DIV4         /**< Timer clock div 4 */
} halBaseTimersClockDivision;

typedef struct {
    halBaseTimer    timer;
    uint16_t halBaseTimerPrescaler;                         /**< Timer prescaler for TIM clock division.
                                                                 Must be between 0x0000 and 0xFFFF */
    uint16_t halBaseTimerPeriod;                            /**< Value for update action interrupt
                                                                 Must be between 0x0000 and 0xFFFF */
    halBaseTimersClockDivision halBaseTimersClockDivision;  /**< Timer clock frequency division */
} halInitBaseTimerStruct;

void halBaseTimerInit(halTimer_struct* timerStruct, halInitBaseTimerStruct* halInitStruct);
void halBaseTimerDeinit(halTimer_struct* timerStruct);

void halBaseTimerStop(halTimer_struct* timerStruct);
void halBaseTimerStart(halTimer_struct* timerStruct);

void halBaseTimerSetTimerCallback(halTimer_struct* timerStruct, halTimerCallbackType callback);

#endif /* HAL_HAL_BASE_TIMER_H_ */
