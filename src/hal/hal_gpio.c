#include "hal_gpio.h"

static GPIO_TypeDef* halGPIOGetPortByEnum(halPort halPort){
    switch(halPort){
        case halPortA:
            return GPIOA;
        case halPortB:
            return GPIOB;
        case halPortC:
            return GPIOC;
        case halPortD:
            return GPIOD;
        case halPortE:
            return GPIOE;
        }
    return 0;
}

static void halGPIOEnableRCC(halPort halPort){
    switch(halPort){
            case halPortA:
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
                break;
            case halPortB:
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
                break;
            case halPortC:
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
                break;
            case halPortD:
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
                break;
            case halPortE:
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
                break;
            }
}

void halGPIOInit(halPort halPort,halInitGPIOStruct* initStruct){
    halGPIOEnableRCC(halPort);
    GPIO_InitTypeDef init;
    init.GPIO_Pin = initStruct->halPins;
    init.GPIO_PuPd = initStruct->halPuPd;
    init.GPIO_OType = initStruct->halOutType;
    init.GPIO_Mode = initStruct->halGpioMode;
    init.GPIO_Speed = initStruct->halGpioSpeed;
    GPIO_Init(halGPIOGetPortByEnum(halPort),&init);
    GPIO_SetBits(halGPIOGetPortByEnum(halPort),initStruct->halPins);
}

void halGPIOSetPins(halPort port,halPins pins){
    GPIO_SetBits(halGPIOGetPortByEnum(port),pins);
}

halPinState halGPIOReadPin(halPort port,halPins pin){
    return GPIO_ReadInputDataBit(halGPIOGetPortByEnum(port),pin);
}

void halGPIOResetPins(halPort port,halPins pins){
    GPIO_SetBits(halGPIOGetPortByEnum(port),pins);
}

void halGPIOTogglePins(halPort port,halPins pins){
    GPIO_ToggleBits(halGPIOGetPortByEnum(port),pins);
}
