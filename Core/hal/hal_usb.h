#ifndef HAL_HAL_USB_H_

#define HAL_HAL_USB_H_

#include "stm32f4xx.h"

typedef struct {
  GPIO_TypeDef* const   port;
  uint16_t              pin;
  uint32_t              rcc;
} halUSB_pinDef_t;

typedef struct {
  USB_OTG_GlobalTypeDef*  usbInstance;
  halUSB_pinDef_t         dpPin;
  halUSB_pinDef_t         dmPin;
  uint8_t                 afDef;
  uint32_t                usbRCC;
} halUSB_def_t;

typedef enum {
  halUSB_OK = 0,
  halUSB_ERROR,
  halUSB_NULL_POINT
} halUSB_error;

typedef struct {
  uint32_t hostChannels;
} halUSB_initStruct;

halUSB_error halUSBInit(HCD_HandleTypeDef* hcd, halUSB_initStruct* initStruct);
halUSB_error halUSBDeinit(HCD_HandleTypeDef* hcd);

uint32_t halUSBGetCurrentFrame(HCD_HandleTypeDef* hcd);

//uint32_t halUSBGetCurrentFrame(HCD_HandleTypeDef* hcd);

#endif
