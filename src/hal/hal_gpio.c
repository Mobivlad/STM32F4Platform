#include "hal_gpio.h"

GPIO_TypeDef* getPort(halPort halPort){
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

void enableRCC(halPort halPort){
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
    enableRCC(halPort);
    GPIO_InitTypeDef init;
    init.GPIO_Pin = initStruct->halPins;
    init.GPIO_PuPd = initStruct->halPuPd;
    init.GPIO_OType = initStruct->halOutType;
    init.GPIO_Mode = initStruct->halGpioMode;
    init.GPIO_Speed = initStruct->halGpioSpeed;
    GPIO_Init(getPort(halPort),&init);
    GPIO_SetBits(getPort(halPort),initStruct->halPins);
}

void halSetBits(halPort port,halPins pins){
    GPIO_SetBits(getPort(port),pins);
}

halPinState halReadPin(halPort port,halPins pin){
    uint8_t x = GPIO_ReadInputDataBit(getPort(port),pin);
    return x;
}

void halResetBits(halPort port,halPins pins){
    GPIO_SetBits(getPort(port),pins);
}

void halToggle(halPort port,halPins pins){
    GPIO_ToggleBits(getPort(port),pins);
}
