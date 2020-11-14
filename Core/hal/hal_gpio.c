#include "hal_gpio.h"

static const halGPIO_portDef_t port_table[halGPIO_PortCount] = {
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

static const halGPIO_pinData_t halGPIO_pinData[halGPIO_PinCount] = {
  { GPIO_PIN_0,  EXTI0_IRQn },
  { GPIO_PIN_1,  EXTI1_IRQn },
  { GPIO_PIN_2,  EXTI2_IRQn },
  { GPIO_PIN_3,  EXTI3_IRQn },
  { GPIO_PIN_4,  EXTI4_IRQn },
  { GPIO_PIN_5,  EXTI9_5_IRQn },
  { GPIO_PIN_6,  EXTI9_5_IRQn },
  { GPIO_PIN_7,  EXTI9_5_IRQn },
  { GPIO_PIN_8,  EXTI9_5_IRQn },
  { GPIO_PIN_9,  EXTI9_5_IRQn },
  { GPIO_PIN_10, EXTI15_10_IRQn },
  { GPIO_PIN_11, EXTI15_10_IRQn },
  { GPIO_PIN_12, EXTI15_10_IRQn },
  { GPIO_PIN_13, EXTI15_10_IRQn },
  { GPIO_PIN_14, EXTI15_10_IRQn },
  { GPIO_PIN_15, EXTI15_10_IRQn }
};

static halGPIO_EXTIstruct* pinEXTIHandle[halGPIO_PinCount] = { 0 };

void halGPIOInit(halGPIO_struct* const gpioStruct,
    const halGPIO_initStruct* const initStruct) {
  if (gpioStruct == NULL || initStruct == NULL) {
    return;
  }
  // enable RCC
  SET_BIT(RCC->AHB1ENR, port_table[gpioStruct->port].enablePortClockDef);

  gpioStruct->pinInitStruct.Pin = halGPIO_pinData[initStruct->pin].pin;
  gpioStruct->pinInitStruct.Pull = initStruct->PuPd;
  gpioStruct->pinInitStruct.Speed = initStruct->speed;

  switch (initStruct->mode) {
  case halGPIO_ModeIn:
    if (initStruct->exti == NULL) {
      gpioStruct->pinInitStruct.Mode = GPIO_MODE_INPUT;
    } else {
      gpioStruct->pinInitStruct.Mode = initStruct->exti->type;
      initStruct->exti->port = port_table[gpioStruct->port].portDef;
      pinEXTIHandle[initStruct->pin] = initStruct->exti;
      HAL_NVIC_EnableIRQ(halGPIO_pinData[initStruct->pin].exti);
      HAL_NVIC_SetPriority(halGPIO_pinData[initStruct->pin].exti, EXTI_PRIORITY,
          EXTI_SUBPRIORITY);
    }
    break;
  case halGPIO_ModeOut:
    gpioStruct->pinInitStruct.Mode =
        (initStruct->outType == halGPIO_OTPP) ?
            GPIO_MODE_OUTPUT_PP : GPIO_MODE_OUTPUT_OD;
    break;
  case halGPIO_ModeAF:
    gpioStruct->pinInitStruct.Mode =
        (initStruct->outType == halGPIO_OTPP) ?
            GPIO_MODE_AF_PP : GPIO_MODE_AF_OD;
    break;
  case halGPIO_ModeAn:
    gpioStruct->pinInitStruct.Mode = GPIO_MODE_ANALOG;
    break;
  }

  HAL_GPIO_Init(port_table[gpioStruct->port].portDef,
      (GPIO_InitTypeDef*) gpioStruct);

  gpioStruct->state == halGPIO_Set ?
      HAL_GPIO_WritePin(port_table[gpioStruct->port].portDef,
          gpioStruct->pinInitStruct.Pin, SET) :
      HAL_GPIO_WritePin(port_table[gpioStruct->port].portDef,
          gpioStruct->pinInitStruct.Pin, RESET);

}

void halGPIOSetPins(halGPIO_struct* const gpioStruct) {
  if (gpioStruct == NULL) {
    return;
  }
  gpioStruct->state = halGPIO_Set;

  HAL_GPIO_WritePin(port_table[gpioStruct->port].portDef,
      gpioStruct->pinInitStruct.Pin, SET);
}

void halGPIOResetPins(halGPIO_struct* const gpioStruct) {
  if (gpioStruct == NULL) {
    return;
  }
  gpioStruct->state = halGPIO_Reset;

  HAL_GPIO_WritePin(port_table[gpioStruct->port].portDef,
      gpioStruct->pinInitStruct.Pin, RESET);
}

void halGPIOTogglePins(halGPIO_struct* const gpioStruct) {
  if (gpioStruct == NULL) {
    return;
  }
  gpioStruct->state =
      (gpioStruct->state == halGPIO_Set) ? halGPIO_Reset : halGPIO_Set;

  HAL_GPIO_TogglePin(port_table[gpioStruct->port].portDef,
      gpioStruct->pinInitStruct.Pin);
}

halGPIO_pinState halGPIOReadPin(const halGPIO_struct* const gpioStruct) {
  return HAL_GPIO_ReadPin(port_table[gpioStruct->port].portDef,
      gpioStruct->pinInitStruct.Pin);
}

void EXTI_IRQHandler(uint16_t pin) {
  if (pinEXTIHandle[pin] == NULL) {
    return;
  }
  if (__HAL_GPIO_EXTI_GET_IT(halGPIO_pinData[pin].pin)) {

    if (HAL_GPIO_ReadPin(pinEXTIHandle[pin]->port, halGPIO_pinData[pin].pin)
        == GPIO_PIN_SET) {
      if (pinEXTIHandle[pin]->risingCallback != NULL) {
        pinEXTIHandle[pin]->risingCallback(pin);
      }
    } else {
      if (pinEXTIHandle[pin]->fallingCallback != NULL) {
        pinEXTIHandle[pin]->fallingCallback(pin);
      }
    }

    __HAL_GPIO_EXTI_CLEAR_IT(halGPIO_pinData[pin].pin);
  }
}

void EXTI0_IRQHandler(void) {
  EXTI_IRQHandler(halGPIO_Pin0);
}

void EXTI1_IRQHandler(void) {
  EXTI_IRQHandler(halGPIO_Pin1);
}

void EXTI2_IRQHandler(void) {
  EXTI_IRQHandler(halGPIO_Pin2);
}

void EXTI3_IRQHandler(void) {
  EXTI_IRQHandler(halGPIO_Pin3);
}

void EXTI4_IRQHandler(void) {
  EXTI_IRQHandler(halGPIO_Pin4);
}

void EXTI9_5_IRQHandler(void) {
  EXTI_IRQHandler(halGPIO_Pin5);
  EXTI_IRQHandler(halGPIO_Pin6);
  EXTI_IRQHandler(halGPIO_Pin7);
  EXTI_IRQHandler(halGPIO_Pin8);
  EXTI_IRQHandler(halGPIO_Pin9);
}

void EXTI15_10_IRQHandler(void) {
  EXTI_IRQHandler(halGPIO_Pin10);
  EXTI_IRQHandler(halGPIO_Pin11);
  EXTI_IRQHandler(halGPIO_Pin12);
  EXTI_IRQHandler(halGPIO_Pin13);
  EXTI_IRQHandler(halGPIO_Pin14);
}

