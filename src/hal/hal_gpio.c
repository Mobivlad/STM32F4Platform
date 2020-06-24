#include "hal_gpio.h"

static GPIO_TypeDef* halGPIOGetPortByEnum(halGPIO_port port){
    switch (port) {
        case halGPIO_PortA:
            return GPIOA;
        case halGPIO_PortB:
            return GPIOB;
        case halGPIO_PortC:
            return GPIOC;
        case halGPIO_PortD:
            return GPIOD;
        case halGPIO_PortE:
            return GPIOE;
    }
    return 0;
}

static void halGPIOEnableRCC(halGPIO_port halPort){
    switch (halPort) {
        case halGPIO_PortA:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
            break;
        case halGPIO_PortB:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
            break;
        case halGPIO_PortC:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
            break;
        case halGPIO_PortD:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
            break;
        case halGPIO_PortE:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
            break;
    }
}

void halGPIOInit(halGPIO_struct* gpioStruct, halGPIO_init_struct* initStruct) {
    // enable RCC
    halGPIOEnableRCC(gpioStruct->port);

    gpioStruct ->spi_gpio_struct.GPIO_Mode = initStruct->mode;
    gpioStruct ->spi_gpio_struct.GPIO_OType = initStruct->outType;
    gpioStruct ->spi_gpio_struct.GPIO_Pin = initStruct->pins;
    gpioStruct ->spi_gpio_struct.GPIO_PuPd = initStruct->PuPd;
    gpioStruct ->spi_gpio_struct.GPIO_Speed = initStruct->speed;

    GPIO_Init(halGPIOGetPortByEnum(gpioStruct->port), gpioStruct);

    if (gpioStruct->state == halGPIO_Set)
        GPIO_SetBits(halGPIOGetPortByEnum(gpioStruct->port),
                gpioStruct->spi_gpio_struct.GPIO_Pin);
    else
        GPIO_ResetBits(halGPIOGetPortByEnum(gpioStruct->port),
                gpioStruct->spi_gpio_struct.GPIO_Pin);
}

void halGPIOSetPins(halGPIO_struct* gpioStruct) {
    gpioStruct->state = halGPIO_Set;

    GPIO_SetBits(halGPIOGetPortByEnum(gpioStruct->port),
            gpioStruct->spi_gpio_struct.GPIO_Pin);
}

void halGPIOResetPins(halGPIO_struct* gpioStruct) {
    gpioStruct->state = halGPIO_Reset;

    GPIO_ResetBits(halGPIOGetPortByEnum(gpioStruct->port),
            gpioStruct->spi_gpio_struct.GPIO_Pin);
}

void halGPIOTogglePins(halGPIO_struct* gpioStruct) {
    if (gpioStruct->state == halGPIO_Set)
        gpioStruct->state = halGPIO_Reset;
    else
        gpioStruct->state = halGPIO_Set;

    GPIO_ToggleBits(halGPIOGetPortByEnum(gpioStruct->port),
            gpioStruct->spi_gpio_struct.GPIO_Pin);
}

halGPIO_pinState halGPIOReadPin(halGPIO_struct* gpioStruct) {
    return GPIO_ReadInputDataBit(halGPIOGetPortByEnum(gpioStruct->port),
            gpioStruct->spi_gpio_struct.GPIO_Pin);
}
