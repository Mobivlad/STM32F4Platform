#include "hal_usb.h"
#include "usbd_def.h"


static const halUSB_def_t usbDef = {
  USB_OTG_FS,
  {GPIOA, GPIO_PIN_12, RCC_AHB1ENR_GPIOAEN},
  {GPIOA, GPIO_PIN_11, RCC_AHB1ENR_GPIOAEN},
  GPIO_AF10_OTG_FS,
  RCC_AHB2ENR_OTGFSEN
};

static PCD_HandleTypeDef* pcdPointForInterupt;

static halUSB_error halUSBInitPins() {
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

  return halUSB_OK;
}

halUSB_error halUSBInit(PCD_HandleTypeDef* pcd, halUSB_initStruct* initStruct) {

  if (pcd == NULL || initStruct == NULL) {
    return halUSB_NULL_POINT;
  }

  pcdPointForInterupt = pcd;

  halUSBInitPins();

  pcd->Instance = usbDef.usbInstance;
  pcd->Init.dev_endpoints = initStruct->endPointCount;
  pcd->Init.speed = PCD_SPEED_FULL;

  pcd->Init.phy_itface = PCD_PHY_EMBEDDED;
  pcd->Init.Sof_enable = DISABLE;
  pcd->Init.low_power_enable = DISABLE;
  pcd->Init.lpm_enable = DISABLE;
  pcd->Init.vbus_sensing_enable = DISABLE;
  pcd->Init.use_dedicated_ep1 = DISABLE;
  pcd->Init.dma_enable = DISABLE;

  if (HAL_PCD_Init(pcd) != HAL_OK) {
    return halUSB_ERROR;
  }

  HAL_PCDEx_SetRxFiFo(pcd, EP_BUFFER_SIZE);
  HAL_PCDEx_SetTxFiFo(pcd, TX_EP0_NUM, EP_BUFFER_SIZE);
  HAL_PCDEx_SetTxFiFo(pcd, TX_EP1_NUM, EP_BUFFER_SIZE);

  return halUSB_OK;
}

halUSB_error halUSBDeinit(PCD_HandleTypeDef* pcd) {
  if (pcd == NULL) {
    return halUSB_NULL_POINT;
  }

  if (HAL_PCD_DeInit(pcd) != HAL_OK) {
    return halUSB_ERROR;
  }

  return halUSB_OK;
}

void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(pcdPointForInterupt);
}
