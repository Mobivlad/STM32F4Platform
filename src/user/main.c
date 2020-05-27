/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "main.h"

void EXTI0_IRQHandler(){
	if(EXTI_GetFlagStatus(EXTI_Line0)==SET){
		delayMs(40);
		toggleLEDs(PIN_RED);
		//clear IT from  Line 0
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void TIM6_DAC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){
    	toggleLEDs(PIN_GREEN);
    	//clear UPDATE_IT flag
    	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
    }
}

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	initDelay();
	initLeds();
	initButtons();
	initTimer();
	for(;;);
}
