/*
 * hal_base_timer.h
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_BASE_TIMER_H_
#define HAL_HAL_BASE_TIMER_H_

#include "stm32f4xx.h"

/// Callback function for timers
typedef void(*halTimerCallbackType)(void);

/// Base timers enumeration
typedef enum {
    halBaseTimer6 = 0,         /**< Base timer 0 */
    halBaseTimer7,             /**< Base timer 1 */
    halBaseTimersCount
} halBaseTimers;

/// Timer clock division
typedef enum {
    halBaseTimerClockDivision1 = TIM_CKD_DIV1,        /**< Timer clock div 1 */
    halBaseTimerClockDivision2 = TIM_CKD_DIV2,        /**< Timer clock div 2 */
    halBaseTimerClockDivision4 = TIM_CKD_DIV4         /**< Timer clock div 4 */
} halBaseTimersClockDivision;

typedef struct {
    uint16_t halBaseTimerPrescaler;                         /**< Timer prescaler for TIM clock division.
                                                                 Must be between 0x0000 and 0xFFFF */
    uint16_t halBaseTimerPeriod;                            /**< Value for update action interrupt
                                                                 Must be between 0x0000 and 0xFFFF */
    halBaseTimersClockDivision halBaseTimersClockDivision;  /**< Timer clock frequency division */
} halInitBaseTimerStruct;

/**
 * Base timer initial function.
 * @param timer value of halBaseTimers enumeration
 * @param initStruct pointer on base timer initial structure
 * @param callback calback function
 */
void halBaseTimerInit(halBaseTimers timer,
        halInitBaseTimerStruct* halInitStruct,
        halTimerCallbackType callback);

/**
 * Stop timer function. The function reset counter value to 0.
 * @param timer value of halBaseTimers enumeration
 */
void halBaseTimerStop(halBaseTimers timer);

/**
 * Start timer function.
 * @param timer value of halBaseTimers enumeration
 */
void halBaseTimerStart(halBaseTimers timer);

#endif /* HAL_HAL_BASE_TIMER_H_ */
