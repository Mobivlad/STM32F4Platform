#include "hal_gpio.h"

static const halGPIO_portTable port_table[halGPIO_PortCount] =
{
        { GPIOA, RCC_AHB1Periph_GPIOA },
        { GPIOB, RCC_AHB1Periph_GPIOB },
        { GPIOC, RCC_AHB1Periph_GPIOC },
        { GPIOD, RCC_AHB1Periph_GPIOD },
        { GPIOE, RCC_AHB1Periph_GPIOE }
};

void halGPIOInit(halGPIO_struct* const gpioStruct, const halGPIO_initStruct* const initStruct) {
    // enable RCC
    RCC_AHB1PeriphClockCmd(port_table[gpioStruct->port].portClock, ENABLE);

    gpioStruct->pinInitStruct.GPIO_Mode     = initStruct->mode;
    gpioStruct->pinInitStruct.GPIO_OType    = initStruct->outType;
    gpioStruct->pinInitStruct.GPIO_Pin      = initStruct->pin;
    gpioStruct->pinInitStruct.GPIO_PuPd     = initStruct->PuPd;
    gpioStruct->pinInitStruct.GPIO_Speed    = initStruct->speed;

    GPIO_Init(port_table[gpioStruct->port].portDef, (GPIO_InitTypeDef*) gpioStruct);

    gpioStruct->state == halGPIO_Set ?
            GPIO_SetBits(port_table[gpioStruct->port].portDef, gpioStruct->pinInitStruct.GPIO_Pin) :
            GPIO_ResetBits(port_table[gpioStruct->port].portDef,
                    gpioStruct->pinInitStruct.GPIO_Pin);

}

void halGPIOSetPins(halGPIO_struct* const gpioStruct) {
    gpioStruct->state = halGPIO_Set;

    GPIO_SetBits(port_table[gpioStruct->port].portDef, gpioStruct->pinInitStruct.GPIO_Pin);
}

void halGPIOResetPins(halGPIO_struct* const gpioStruct) {
    gpioStruct->state = halGPIO_Reset;

    GPIO_ResetBits(port_table[gpioStruct->port].portDef, gpioStruct->pinInitStruct.GPIO_Pin);
}

void halGPIOTogglePins(halGPIO_struct* const gpioStruct) {
    gpioStruct->state = (gpioStruct->state == halGPIO_Set) ? halGPIO_Reset : halGPIO_Set;

    GPIO_ToggleBits(port_table[gpioStruct->port].portDef, gpioStruct->pinInitStruct.GPIO_Pin);
}

halGPIO_pinState halGPIOReadPin(const halGPIO_struct* const gpioStruct) {
    return GPIO_ReadInputDataBit(port_table[gpioStruct->port].portDef,
            gpioStruct->pinInitStruct.GPIO_Pin);
}
