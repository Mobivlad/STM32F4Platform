/*
 * hal_sysclock.h
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_SYSCLOCK_H_
#define HAL_HAL_SYSCLOCK_H_

#define SYSTICK_MAX_VALUE (0xFFFFFF)

#include "stm32f4xx.h";

/**
 * Initial function for SysTick
 * @param ms duration of 1 sysclock tick
 */
void halSysClockInit(u32 ms);

/**
 * Return SysTick timer value
 * @return SysTick value register
 */
u32 halGetTick(void);

/**
 * Function to change SysTick timer value
 * @param ms new register value
 */
void halSetTick(u32 ms);

/**
 * SysTick overload register detector
 * @return 1 if timer reached 0 after last halGetTick() call, else 0
 */
uint8_t halWasOverload(void);

#endif /* HAL_HAL_SYSCLOCK_H_ */
