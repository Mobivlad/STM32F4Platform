/*
 * usbh_vbus.c
 *
 *  Created on: Oct 7, 2020
 *      Author: Sigma
 */

#include "usbh_vbus.h"

static halGPIO_struct* VBUS_PIN;

USBH_StatusTypeDef USBH_LL_SetVBUS(USBH_HandleTypeDef *phost, halGPIO_struct* pin) {
  VBUS_PIN = pin;
  return USBH_OK;
}

/**
  * @brief  Drives VBUS.
  * @param  phost: Host handle
  * @param  state: VBUS state
  *          This parameter can be one of these values:
  *           0: VBUS Active
  *           1: VBUS Inactive
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
  if(state == 0)
  {
    halGPIOResetPins(VBUS_PIN);
  }
  else
  {
    halGPIOSetPins(VBUS_PIN);
  }
  USBH_delay(200);
  return USBH_OK;
}
