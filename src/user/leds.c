#include "leds.h"

void initLeds(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	GPIO_InitTypeDef ledsInitStruct;
	ledsInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	ledsInitStruct.GPIO_OType = GPIO_OType_PP;
	ledsInitStruct.GPIO_Pin = PIN_GREEN|PIN_RED;
	ledsInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ledsInitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(PORT_LEDS,&ledsInitStruct);
	GPIO_ResetBits(PORT_LEDS,PIN_GREEN|PIN_RED);
}

void onLEDs(u16 val){
	GPIO_SetBits(PORT_LEDS,val);
}

void offLEDs(u16 val){
	GPIO_ResetBits(PORT_LEDS,val);
}

void toggleLEDs(u16 val){
	GPIO_ToggleBits(PORT_LEDS,val);
}

