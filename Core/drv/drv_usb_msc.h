#ifndef DRV_DRV_USB_CDC_H_
#define DRV_DRV_USB_CDC_H_

#include "hal_usb.h"
#include "hal_gpio.h"

#include "usbh_def.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "usbh_conf.h"

#define HOST_FS 		        0
#define USB_MSC_HOST_COUNT  11

#define VBUS_PORT halGPIO_PortG
#define VBUS_PIN  halGPIO_Pin6

typedef struct {
  USBH_HandleTypeDef  usbHost;
  HCD_HandleTypeDef   hcd;
  halGPIO_struct      vbus;
} drvUSBMSC_struct;

typedef void(*drvUSBAction)();

typedef struct {
  drvUSBAction onDisconnect;
  drvUSBAction onConnect;
  drvUSBAction onReady;
} drvUSBMSC_initStruct;

typedef enum {
  drvUSBMSC_OK = 0,
  drvUSBMSC_ERROR
} drvUSBMSC_error;

typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_START,
  APPLICATION_READY,
  APPLICATION_DISCONNECT
}ApplicationTypeDef;

typedef struct {
  drvUSBAction onDisconnect;
  drvUSBAction onConnect;
  drvUSBAction onReady;
} drvUSBActions;

drvUSBMSC_error drvUSBMSCInit(drvUSBMSC_struct* mscStruct, drvUSBMSC_initStruct* initStruct);

drvUSBMSC_error drvUSBMSCRun(drvUSBMSC_struct* mscStruct);

#endif //DRV_DRV_USB_MSD_H_
