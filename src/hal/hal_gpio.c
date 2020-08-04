#include "hal_gpio.h"

static const halGPIO_portDef_t port_table[halGPIO_PortCount] =
{
        { GPIOA, RCC_AHB1ENR_GPIOAEN },
        { GPIOB, RCC_AHB1ENR_GPIOBEN },
        { GPIOC, RCC_AHB1ENR_GPIOCEN },
        { GPIOD, RCC_AHB1ENR_GPIODEN },
        { GPIOE, RCC_AHB1ENR_GPIOEEN },
        { GPIOF, RCC_AHB1ENR_GPIOFEN },
        { GPIOG, RCC_AHB1ENR_GPIOGEN },
        { GPIOH, RCC_AHB1ENR_GPIOHEN },
        { GPIOI, RCC_AHB1ENR_GPIOIEN },
        { GPIOJ, RCC_AHB1ENR_GPIOJEN },
        { GPIOK, RCC_AHB1ENR_GPIOKEN }
};

void halGPIOInit(halGPIO_struct* const gpioStruct, const halGPIO_initStruct* const initStruct) {
	if (gpioStruct == NULL || initStruct == NULL) {
		return;
	}
    // enable RCC
    SET_BIT(RCC->AHB1ENR, port_table[gpioStruct->port].enablePortClockDef);

    gpioStruct->pinInitStruct.Pin      = initStruct->pin;
    gpioStruct->pinInitStruct.Pull     = initStruct->PuPd;
    gpioStruct->pinInitStruct.Speed    = initStruct->speed;

    switch (initStruct->mode) {
        case halGPIO_ModeIn:
            gpioStruct->pinInitStruct.Mode = GPIO_MODE_INPUT;
            break;
        case halGPIO_ModeOut:
            gpioStruct->pinInitStruct.Mode = (initStruct->outType == halGPIO_OTPP) ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_OUTPUT_OD;
            break;
        case halGPIO_ModeAF:
            gpioStruct->pinInitStruct.Mode = (initStruct->outType == halGPIO_OTPP) ? GPIO_MODE_AF_PP : GPIO_MODE_AF_OD;
            break;
        case halGPIO_ModeAn:
            gpioStruct->pinInitStruct.Mode = GPIO_MODE_ANALOG;
            break;
    }

    HAL_GPIO_Init(port_table[gpioStruct->port].portDef, (GPIO_InitTypeDef*) gpioStruct);

    gpioStruct->state == halGPIO_Set ?
            HAL_GPIO_WritePin(port_table[gpioStruct->port].portDef, gpioStruct->pinInitStruct.Pin,
                    SET) :
            HAL_GPIO_WritePin(port_table[gpioStruct->port].portDef, gpioStruct->pinInitStruct.Pin,
                    RESET);

}

void halGPIOSetPins(halGPIO_struct* const gpioStruct) {
	if (gpioStruct == NULL) {
		return;
	}
    gpioStruct->state = halGPIO_Set;

    HAL_GPIO_WritePin(port_table[gpioStruct->port].portDef, gpioStruct->pinInitStruct.Pin, SET);
}

void halGPIOResetPins(halGPIO_struct* const gpioStruct) {
	if (gpioStruct == NULL) {
		return;
	}
	gpioStruct->state = halGPIO_Reset;

    HAL_GPIO_WritePin(port_table[gpioStruct->port].portDef, gpioStruct->pinInitStruct.Pin, RESET);
}

void halGPIOTogglePins(halGPIO_struct* const gpioStruct) {
	if (gpioStruct == NULL) {
		return;
	}
    gpioStruct->state = (gpioStruct->state == halGPIO_Set) ? halGPIO_Reset : halGPIO_Set;

    HAL_GPIO_TogglePin(port_table[gpioStruct->port].portDef, gpioStruct->pinInitStruct.Pin);
}

halGPIO_pinState halGPIOReadPin(const halGPIO_struct* const gpioStruct) {
    return HAL_GPIO_ReadPin(port_table[gpioStruct->port].portDef,
            gpioStruct->pinInitStruct.Pin);
}
