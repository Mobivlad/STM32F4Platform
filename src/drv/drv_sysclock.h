/*
 * drv_sysclock.h
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_SYSCLOCK_H_
#define DRV_DRV_SYSCLOCK_H_

#include "hal_sysclock.h"

/**
 * Initial function for SysTick
 * @param ms duration of 1 sysclock tick
 */
void drvSysClockInit(uint32_t ms);

/**
 * Function check if the time is elapsed
 * @param startTime value of start getTick
 * @param delay value of delay
 * @return 0 if time is not elapsed else value - elapsed
 */
uint8_t drvIsTimePased(uint32_t startTime, uint32_t delay);

/**
 * Return SysTick timer value
 * @return SysTick counter value
 */
uint32_t drvSysClockGetTick(void);

#endif /* DRV_DRV_SYSCLOCK_H_ */
