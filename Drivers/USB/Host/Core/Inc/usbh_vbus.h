/*
 * usbh_vbus.h
 *
 *  Created on: Oct 7, 2020
 *      Author: Sigma
 */

#ifndef INC_USBH_VBUS_H_
#define INC_USBH_VBUS_H_

#include "hal_gpio.h"
#include "usbh_def.h"

USBH_StatusTypeDef USBH_LL_SetVBUS(USBH_HandleTypeDef *phost, halGPIO_struct* pin);

USBH_StatusTypeDef   USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state);

#endif /* INC_USBH_VBUS_H_ */
