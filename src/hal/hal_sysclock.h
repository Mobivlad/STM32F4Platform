/*
 * hal_sysclock.h
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_SYSCLOCK_H_
#define HAL_HAL_SYSCLOCK_H_

#include "stm32f4xx.h"

/**
 * Initial function for SysTick
 * @param ms duration of 1 sysclock tick
 */
void halSysClockInit(uint32_t ms);

/**
 * Return SysTick timer value
 * @return SysTick counter value
 */
uint32_t halSysClockGetTick(void);

#endif /* HAL_HAL_SYSCLOCK_H_ */
