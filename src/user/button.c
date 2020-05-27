#include "button.h"
static void initPins(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef initStruct;
	initStruct.GPIO_Mode = GPIO_Mode_IN;
	initStruct.GPIO_Pin = PIN_BUTTON;

	//there is hardware pull-down
	initStruct.GPIO_OType = GPIO_OType_PP;
	initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	initStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(PORT_BUTTON,&initStruct);
}

static void initExti(){
	//connect interrupt from pin 0 to port A
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	//interrupt configuration
	EXTI_InitTypeDef initStruct;
	initStruct.EXTI_Line = EXTI_Line0;
	initStruct.EXTI_LineCmd = ENABLE;
	initStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	initStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&initStruct);

	//config EXTI0 in interrupt controller
	NVIC_InitTypeDef nvicInitStruct;
	nvicInitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct);

	//change sysTick priority on higher
	NVIC_SetPriority(SysTick_IRQn, 0x00);

	//enable interrupt
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0x01);
}

void initButtons(){
	initPins();
	initExti();
}

