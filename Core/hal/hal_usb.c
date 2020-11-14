#include "hal_usb.h"
#include "usbh_def.h"
#include "stm32f4xx_hal_hcd.h"


static const halUSB_def_t usbDef = {
  USB_OTG_FS,
  {GPIOA, GPIO_PIN_12, RCC_AHB1ENR_GPIOAEN},  //dp
  {GPIOA, GPIO_PIN_11, RCC_AHB1ENR_GPIOAEN},  //dm
  GPIO_AF10_OTG_FS,
  RCC_AHB2ENR_OTGFSEN
};

static HCD_HandleTypeDef* hcdPointForInterupt;

halUSB_error halUSBInit(HCD_HandleTypeDef* hcd, halUSB_initStruct* initStruct) {

  if (hcd == NULL || initStruct == NULL) {
    return halUSB_NULL_POINT;
  }

  hcdPointForInterupt = hcd;

  SET_BIT(RCC->AHB1ENR, usbDef.dmPin.rcc);

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = usbDef.dmPin.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = usbDef.afDef;
  HAL_GPIO_Init(usbDef.dmPin.port, &GPIO_InitStruct);

  SET_BIT(RCC->AHB1ENR, usbDef.dpPin.rcc);

  GPIO_InitStruct.Pin = usbDef.dpPin.pin;
  HAL_GPIO_Init(usbDef.dpPin.port, &GPIO_InitStruct);

  __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

  HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

  hcd->Instance = usbDef.usbInstance;
  hcd->Init.Host_channels = initStruct->hostChannels;
  hcd->Init.speed = HCD_SPEED_FULL;
  hcd->Init.phy_itface = HCD_PHY_EMBEDDED;

  hcd->Init.vbus_sensing_enable = DISABLE;
  hcd->Init.low_power_enable = DISABLE;
  hcd->Init.dma_enable = DISABLE;
  hcd->Init.Sof_enable = DISABLE;
  hcd->Init.lpm_enable = DISABLE;

  if (HAL_HCD_Init(hcd) != HAL_OK) {
    return halUSB_ERROR;
  }

  return halUSB_OK;
}

halUSB_error halUSBDeinit(HCD_HandleTypeDef* hcd) {
  if (hcd == NULL) {
    return halUSB_NULL_POINT;
  }

  if (HAL_HCD_DeInit(hcd) != HAL_OK) {
    return halUSB_ERROR;
  }

  return halUSB_OK;
}

uint32_t halUSBGetCurrentFrame(HCD_HandleTypeDef* hcd) {
  return HAL_HCD_GetCurrentFrame(hcd);
}

void OTG_FS_IRQHandler(void)
{
  HAL_HCD_IRQHandler(hcdPointForInterupt);
}
