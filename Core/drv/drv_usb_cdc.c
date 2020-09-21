#include "drv_usb_cdc.h"

drvUSBCDC_error drvUSBCDCInit(drvUSBCDC_struct* cdcStruct, drvUSBCDC_initStruct* initStruct) {
  if (USBD_Init(&cdcStruct->usbDevice, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    return drvUSBCDC_ERROR;
  }

  cdcStruct->pcd.pData = &cdcStruct->usbDevice;
  cdcStruct->usbDevice.pData = &cdcStruct->pcd;

  halUSB_initStruct halUSB;
  halUSB.endPointCount = USB_CDC_ENDPOINT_COUNT;

  if(halUSBInit(&cdcStruct->pcd, &halUSB) != halUSB_OK) {
    return drvUSBCDC_ERROR;
  }

  if (USBD_RegisterClass(&cdcStruct->usbDevice, &USBD_CDC) != USBD_OK)
  {
    return drvUSBCDC_ERROR;
  }
  if (USBD_CDC_RegisterInterface(&cdcStruct->usbDevice, &USBD_Interface_fops_FS) != USBD_OK)
  {
    return drvUSBCDC_ERROR;
  }
  if (USBD_Start(&cdcStruct->usbDevice) != USBD_OK)
  {
    return drvUSBCDC_ERROR;
  }

  USBD_Delay(USBD_INIT_TIME);

  return drvUSBCDC_OK;
}

drvUSBCDC_error drvUSBCDCWriteStr(drvUSBCDC_struct* cdcStruct, char* str) {
  if (CDC_Transmit_FS(&cdcStruct->usbDevice, (uint8_t*)str, strlen(str)) != USBD_OK) {
    return drvUSBCDC_ERROR;
  }
  return drvUSBCDC_OK;
}

drvUSBCDC_error drvUSBCDCReadStrWithCbk(drvUSBCDC_struct* cdcStruct, char* str, uint16_t maxSize, receiveCallback_t receiveCallback) {
  if (CDC_ReceiveCplt_FS(&cdcStruct->usbDevice, (uint8_t*)str, maxSize, receiveCallback) != USBD_OK) {
    return drvUSBCDC_ERROR;
  }
  return drvUSBCDC_OK;
}

drvUSBCDC_error drvUSBCDCReadStr(drvUSBCDC_struct* cdcStruct, char* str, uint16_t maxSize) {

  return drvUSBCDCReadStrWithCbk(cdcStruct, str, maxSize, NULL);
}




