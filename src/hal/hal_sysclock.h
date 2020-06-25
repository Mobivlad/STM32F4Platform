/*
 * hal_sysclock.h
 *
 *  Created on: 29 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_SYSCLOCK_H_
#define HAL_HAL_SYSCLOCK_H_

#include "stm32f4xx.h"

/**
 * Initial function for SysTick timer
 */
void halSysClockInit(void);

/**
 * Return SysTick timer value
 * @return SysTick counter value
 */
uint32_t halSysClockGetTick(void);

#endif /* HAL_HAL_SYSCLOCK_H_ */
