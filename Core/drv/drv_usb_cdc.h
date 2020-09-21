#ifndef DRV_DRV_USB_CDC_H_
#define DRV_DRV_USB_CDC_H_

#include "hal_usb.h"
#include "usbd_def.h"
#include "usbd_cdc.h"
#include "usbd_desc.h"
#include "usbd_cdc_if.h"

#define USB_CDC_ENDPOINT_COUNT 4
#define USBD_INIT_TIME         10

#define USB_FS_ID 0

typedef struct {
  halUSB_struct halUSB;
  USBD_HandleTypeDef usbDevice;
  PCD_HandleTypeDef pcd;
} drvUSBCDC_struct;

typedef void(*receiveCallback)();
typedef receiveCallback receiveCallback_t;

typedef struct {

} drvUSBCDC_initStruct;

typedef enum {
  drvUSBCDC_OK = 0,
  drvUSBCDC_ERROR
} drvUSBCDC_error;

drvUSBCDC_error drvUSBCDCInit(drvUSBCDC_struct* cdcStruct, drvUSBCDC_initStruct* initStruct);

drvUSBCDC_error drvUSBCDCWriteStr(drvUSBCDC_struct* cdcStruct, char* str);
drvUSBCDC_error drvUSBCDCReadStrWithCbk(drvUSBCDC_struct* cdcStruct, char* str, uint16_t maxSize, receiveCallback_t receiveCallback);
drvUSBCDC_error drvUSBCDCReadStr(drvUSBCDC_struct* cdcStruct, char* str, uint16_t maxSize);

#endif //DRV_DRV_USB_CDC_H_
