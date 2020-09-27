/*
 * drv_button.c
 *
 *  Created on: 25 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_button.h"

static const drvButton_data buttonData[drvButton_Count] = {
    { halGPIO_PortC, halGPIO_Pin13 }
};

static drvButton_struct* buttons[drvButton_Count];

void drvButtonSetCallBack(drvButton_struct* const buttonStruct,
    const drvButton_action action, const drvButtonActionCallBack callback) {
  if (buttonStruct == NULL)
    return;

  buttonStruct->actionCallbacks[action] = callback;
}

static drvButton_struct* findButtonByPin(uint16_t pin) {
  for (uint8_t i = 0; i < drvButton_Count; i++) {
    if (pin == buttonData[i].pin) {
      return buttons[i];
    }
  }
  return NULL;
}

static void drvButtonRisingEvent(uint16_t pinPtr) {
  drvButton_struct* button = findButtonByPin(pinPtr);
  button->fallingTime = xTaskGetTickCount();
}

static void drvButtonFallingEvent(uint16_t pinPtr) {
  drvButton_struct* button = findButtonByPin(pinPtr);

  uint32_t pressTime = xTaskGetTickCount() - button->fallingTime;
  drvButton_action action = drvButton_NoAction;

  if (pressTime > drvButtonLongPressDuration) {
    action = drvButton_LongPress;
  } else if (pressTime > drvButtonSimplePressDuration) {
    action = drvButton_Press;
  }

  if (action != drvButton_NoAction) {
    if (button->actionCallbacks[action] != NULL) {
      button->actionCallbacks[action]();
    }
  }
}

void drvButtonInit(drvButton_struct* const buttonStruct) {
  if (buttonStruct == NULL)
    return;
  halGPIO_struct* hal_GPIO_struct = (halGPIO_struct*) buttonStruct;

  hal_GPIO_struct->port = buttonData[buttonStruct->button].port;

  halGPIO_EXTIstruct* extiStruct = &buttonStruct->extiStruct;
  extiStruct->fallingCallback = drvButtonFallingEvent;
  extiStruct->risingCallback = drvButtonRisingEvent;
  extiStruct->type = RISING_FALING;

  const halGPIO_initStruct initStruct = { buttonData[buttonStruct->button].pin,
      halGPIO_ModeIn, halGPIO_Speed2Mhz, halGPIO_OTPP, halGPIO_PuPdNo,
      extiStruct };

  halGPIOInit(hal_GPIO_struct, &initStruct);

  buttonStruct->fallingTime = 0;

  buttons[buttonStruct->button] = buttonStruct;

  for (uint8_t i = 0; i < drvButton_ActionsCount; i++) {
    buttonStruct->actionCallbacks[i] = NULL;
  }
}
