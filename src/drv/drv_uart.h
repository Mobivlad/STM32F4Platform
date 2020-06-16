/*
 * drv_uart.h
 *
 *  Created on: 16 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_UART_H_
#define DRV_DRV_UART_H_

#include "hal_uart.h"

/// UART errors codes
typedef enum {
    drvUART_OK = halUART_OK,
    drvUART_IN_PROGRESS = halUART_IN_PROGRESS,
    drvUART_NOT_CONFIG = halUART_NOT_CONFIG,
    drvUART_NULL_POINT = halUART_NULL_POINT,
    drvUART_FULL_BUFF
} drvUARTErrorCode;

/// UART instance enumerations
typedef enum {
    drvUART1 = halUART1,
    drvUART2 = halUART2,
    drvUART3 = halUART3,
    drvUART4 = halUART4,
    drvUART5 = halUART5,
    drvUART6 = halUART6,
    drvUART_Count
} drvUART;

/// UART BaudRate speed
typedef enum {
    drvUARTBaudRate9600 = halUARTBaudRate9600,
    drvUARTBaudRate19200 = halUARTBaudRate9600,
    drvUARTBaudRate38400 = halUARTBaudRate38400,
    drvUARTBaudRate57600 = halUARTBaudRate57600,
    drvUARTBaudRate115200 = halUARTBaudRate115200
} drvUARTBaudRate;

/// UART data size
typedef enum {
    drvUARTWordLength8b = halUARTWordLength8b,
    drvUARTWordLength9b = halUARTWordLength9b
} drvUARTWordLength;

/// UART stop bits size
typedef enum {
    drvUARTStopBits0_5 = halUARTStopBits0_5,
    drvUARTStopBits1 = halUARTStopBits1,
    drvUARTStopBits1_5 = halUARTStopBits1_5,
    drvUARTStopBits2 = halUARTStopBits2
} drvUARTStopBits;

/// UART parity checking type
typedef enum {
    drvUARTNoParity = halUARTNoParity,
    drvUARTEvenParity = halUARTEvenParity,
    drvUARTOddParity = halUARTOddParity
} drvUARTParity;

/// Initial structure for UART
typedef struct {
    drvUARTBaudRate drvUARTBaudRate;
    drvUARTWordLength drvUARTWordLength;
    drvUARTStopBits drvUARTStopBits;
    drvUARTParity drvUARTParity;
} drvUARTInitStruct;

/**
 * Initial function for UART
 * @param uart instance of UART from drvUART enumeration
 * @param initStruct pointer on structure for UART initialization
 */
void drvInitUART(drvUART uart, drvUARTInitStruct* initStruct);

/**
 * Receive array of chars from UART
 * @param uart instance of UART from drvUART enumeration
 * @param dest pointer on destination for receive char array
 * @param destSize size of receive array
 * @return UART error code
 */
drvUARTErrorCode drvReceiveString(drvUART uart, char* dest, uint16_t destSize);

/**
 * Receive array of bytes from UART
 * @param uart instance of UART from drvUART enumeration
 * @param dest pointer on destination for receive array
 * @param size count byte to receive
 * @return UART error code
 */
drvUARTErrorCode drvReceiveDataArray(drvUART uart, uint8_t* dest, uint16_t size);

/**
 * Transfer array of chars to UART
 * @param uart instance of UART from drvUART enumeration
 * @param data pointer on transfer array
 * @return UART error code
 */
drvUARTErrorCode drvSendString(drvUART uart, char* str);

/**
 * Transfer array of bytes to UART
 * @param uart instance of UART from drvUART enumeration
 * @param data pointer on transfer array
 * @param size count byte to transfer
 * @return UART error code
 */
drvUARTErrorCode drvSendDataArray(drvUART uart, uint8_t* data, uint16_t size);

#endif /* DRV_DRV_UART_H_ */
