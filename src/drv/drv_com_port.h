/*
 * drv_com_port.h
 *
 *  Created on: Jul 10, 2020
 *      Author: Sigma
 */

#ifndef DRV_DRV_COM_PORT_H_
#define DRV_DRV_COM_PORT_H_

#include "hal_uart.h"
#include <string.h>

typedef enum {
    drvCOMPort1 = halUART1,
    drvCOMPort_Count
} drvCOMPort;

typedef void(*drvCOMPortCallBack)(void);

typedef struct {
    halUART             uart;
    halUARTBaudRate     baudrate;
    halUARTWordLength   wordlen;
    halUARTStopBits     stopbits;
    halUARTParity       parity;
} com_port_struct;

/// COM port error codes
typedef enum {
    drvCOMPort_OK               = halUART_OK,
    drvCOMPort_BUSY             = halUART_BUSY,
    drvCOMPort_NOT_CONFIG       = halUART_NOT_CONFIG,
    drvCOMPort_ALREADY_CONFIG   = halUART_ALREADY_CONFIG,
    drvCOMPort_NULL_POINT       = halUART_NULL_POINT
} drvCOMPortErrorCode;

typedef struct {
    halUART_struct  uartStruct;
} drvCOMPort_struct;

drvCOMPortErrorCode  drvCOMPortInit(drvCOMPort_struct* comPortStruct,  drvCOMPort comPort);

drvCOMPortErrorCode  drvCOMPortDeinit(drvCOMPort_struct* comPortStruct);

drvCOMPortErrorCode  drvCOMPortReadString(drvCOMPort_struct* comPortStruct, char* stringDest, uint16_t destSize, drvCOMPortCallBack completeAction);

drvCOMPortErrorCode  drvCOMPortReadArray(drvCOMPort_struct* comPortStruct, uint8_t* arrayDest, uint16_t arraySize, drvCOMPortCallBack completeAction);

drvCOMPortErrorCode  drvCOMPortWriteString(drvCOMPort_struct* comPortStruct, char* stringSrc, drvCOMPortCallBack completeAction);

drvCOMPortErrorCode  drvCOMPortWriteArray(drvCOMPort_struct* comPortStruct, uint8_t* arraySrc, uint16_t arraySize, drvCOMPortCallBack completeAction);

#endif /* DRV_DRV_COM_PORT_H_ */
