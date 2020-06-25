#include "hal_gpio.h"

const halGPIO_RegTable port_table[halGPIO_PortCount] =
{
  {GPIOA, RCC_AHB1Periph_GPIOA},
  {GPIOB, RCC_AHB1Periph_GPIOB},
  {GPIOC, RCC_AHB1Periph_GPIOC},
  {GPIOD, RCC_AHB1Periph_GPIOD},
  {GPIOE, RCC_AHB1Periph_GPIOE}
};

void halGPIOInit(halGPIO_struct* gpioStruct, const halGPIO_init_struct* const initStruct) {
    // enable RCC
    RCC_AHB1PeriphClockCmd(port_table[gpioStruct->port].reg_port_clock, ENABLE);

    gpioStruct ->pin_init_struct.GPIO_Mode = initStruct->mode;
    gpioStruct ->pin_init_struct.GPIO_OType = initStruct->outType;
    gpioStruct ->pin_init_struct.GPIO_Pin = initStruct->pin;
    gpioStruct ->pin_init_struct.GPIO_PuPd = initStruct->PuPd;
    gpioStruct ->pin_init_struct.GPIO_Speed = initStruct->speed;

    GPIO_Init(port_table[gpioStruct->port].reg_port, (GPIO_InitTypeDef*)gpioStruct);

    if (gpioStruct->state == halGPIO_Set)
        GPIO_SetBits(port_table[gpioStruct->port].reg_port,
                gpioStruct->pin_init_struct.GPIO_Pin);
    else
        GPIO_ResetBits(port_table[gpioStruct->port].reg_port,
                gpioStruct->pin_init_struct.GPIO_Pin);
}

void halGPIOSetPins(halGPIO_struct* gpioStruct) {
    gpioStruct->state = halGPIO_Set;

    GPIO_SetBits(port_table[gpioStruct->port].reg_port,
            gpioStruct->pin_init_struct.GPIO_Pin);
}

void halGPIOResetPins(halGPIO_struct* gpioStruct) {
    gpioStruct->state = halGPIO_Reset;

    GPIO_ResetBits(port_table[gpioStruct->port].reg_port,
            gpioStruct->pin_init_struct.GPIO_Pin);
}

void halGPIOTogglePins(halGPIO_struct* gpioStruct) {
    if (gpioStruct->state == halGPIO_Set)
        gpioStruct->state = halGPIO_Reset;
    else
        gpioStruct->state = halGPIO_Set;

    GPIO_ToggleBits(port_table[gpioStruct->port].reg_port,
            gpioStruct->pin_init_struct.GPIO_Pin);
}

halGPIO_pinState halGPIOReadPin(halGPIO_struct* gpioStruct) {
    return GPIO_ReadInputDataBit(port_table[gpioStruct->port].reg_port,
            gpioStruct->pin_init_struct.GPIO_Pin);
}
