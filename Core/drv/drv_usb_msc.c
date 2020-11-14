#include "drv_usb_msc.h"

static drvUSBActions currentUSBActions = {0};

static void USBH_UserProcess (USBH_HandleTypeDef *phost, uint8_t id) {
  switch(id)
    {
    case HOST_USER_DISCONNECTION:
      if(currentUSBActions.onDisconnect != NULL) {
        currentUSBActions.onDisconnect();
      }
      break;

    case HOST_USER_CLASS_ACTIVE:
      if(currentUSBActions.onReady != NULL) {
        currentUSBActions.onReady();
      }
      break;

    case HOST_USER_CONNECTION:
      if(currentUSBActions.onConnect != NULL) {
        currentUSBActions.onConnect();
      }
      break;

    default:
      break;
    }
}


drvUSBMSC_error drvUSBMSCInit(drvUSBMSC_struct* mscStruct, drvUSBMSC_initStruct* initStruct) {
  halGPIO_initStruct vbus_init = {0};
  vbus_init.mode = halGPIO_ModeOut;
  vbus_init.outType = halGPIO_OTPP;
  vbus_init.pin = VBUS_PIN;
  vbus_init.PuPd = halGPIO_PuPdNo;
  vbus_init.speed = halGPIO_Speed50Mhz;

  mscStruct->vbus.port = VBUS_PORT;
  mscStruct->vbus.state = halGPIO_Reset;

  currentUSBActions.onConnect = initStruct->onConnect;
  currentUSBActions.onReady = initStruct->onReady;
  currentUSBActions.onDisconnect = initStruct->onDisconnect;

  halGPIOInit(&mscStruct->vbus, &vbus_init);

  USBH_LL_SetVBUS(&mscStruct->usbHost, &mscStruct->vbus);
  
  if (USBH_Init(&mscStruct->usbHost, USBH_UserProcess, HOST_FS) != USBH_OK)
  {
    return drvUSBMSC_ERROR;
  }

  mscStruct->hcd.pData = &mscStruct->usbHost;
  mscStruct->usbHost.pData = &mscStruct->hcd;

  halUSB_initStruct USB_initStruct;
  USB_initStruct.hostChannels = USB_MSC_HOST_COUNT;

  if (halUSBInit(&mscStruct->hcd, &USB_initStruct) != halUSB_OK) {
    return drvUSBMSC_ERROR;
  }

  USBH_LL_SetTimer(&mscStruct->usbHost, halUSBGetCurrentFrame(&mscStruct->hcd));

  if (USBH_RegisterClass(&mscStruct->usbHost, USBH_MSC_CLASS) != USBH_OK)
  {
    return drvUSBMSC_ERROR;
  }
  if (USBH_Start(&mscStruct->usbHost) != USBH_OK)
  {
    return drvUSBMSC_ERROR;
  }

  return drvUSBMSC_OK;
}

drvUSBMSC_error drvUSBMSCRun(drvUSBMSC_struct* mscStruct) {
   USBH_Process(&mscStruct->usbHost);
   return drvUSBMSC_OK;
}



