/*
 * hal_uart.h
 *
 *  Created on: 15 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_UART_H_
#define HAL_HAL_UART_H_

#include "stm32f4xx.h"
#include <stddef.h>

typedef void(*halUARTCallBack)(void);

//+------+--------+--------+--------+--------+--------+--------+
//|      |  UART1 |  UART2 |  UART3 |  UART4 |  UART5 |  UART6 |
//+------+--------+--------+--------+--------+--------+--------+
//|  RX  |  PA10  |  PA3   |  PB11  |  PC11  |  PD2   |   PC7  |
//+------+--------+--------+--------+--------+--------+--------+
//|  TX  |  PA9   |  PA2   |  PB10  |  PC10  |  PC12  |   PC6  |
//+------+--------+--------+--------+--------+--------+--------+

/// UART instance enumerations
typedef enum {
    halUART1 = 0,
    halUART2,
    halUART3,
    halUART4,
    halUART5,
    halUART6,
    halUART_Count
} halUART;

/// UART BaudRate speed
typedef enum {
    halUARTBaudRate9600 = 9600,
    halUARTBaudRate19200 = 19200,
    halUARTBaudRate38400 = 38400,
    halUARTBaudRate57600 = 57600,
    halUARTBaudRate115200 = 115200
} halUARTBaudRate;

/// UART data size
typedef enum {
    halUARTWordLength8b = USART_WordLength_8b,
    halUARTWordLength9b = USART_WordLength_9b
} halUARTWordLength;

/// UART stop bits size
typedef enum {
    halUARTStopBits0_5 = USART_StopBits_0_5,
    halUARTStopBits1 =  USART_StopBits_1,
    halUARTStopBits1_5 = USART_StopBits_1_5,
    halUARTStopBits2 = USART_StopBits_2
} halUARTStopBits;

/// UART parity checking type
typedef enum {
    halUARTNoParity = USART_Parity_No,
    halUARTEvenParity = USART_Parity_Even,
    halUARTOddParity = USART_Parity_Odd
} halUARTParity;

/// Initial structure for UART
typedef struct {
    halUARTBaudRate halUARTBaudRate;
    halUARTWordLength halUARTWordLength;
    halUARTStopBits halUARTStopBits;
    halUARTParity halUARTParity;
} halUARTInitStruct;

/// UART errors codes
typedef enum {
    halUART_OK = 0,
    halUART_BUSY,
    halUART_NOT_CONFIG,
    halUART_NULL_POINT
} halUARTErrorCode;

typedef enum {
    CONFIG = 0x01,
    RX_READY = 0x02,
    RX_STRING = 0x04,
    TX_READY = 0x08,
    TX_STRING = 0x10,
} halUARTReadyStatus;

typedef enum {
    halUARTDataType_String,
    halUARTDataType_Array
} halUARTDataType;

/**
 * Initial function for UART
 * @param uart instance of UART from halUART enumeration
 * @param initStruct pointer on structure for UART initialization
 */
void halInitUART(halUART uart, halUARTInitStruct* initStruct);

/**
 * UART receive function
 * @param uart instance of UART from halUART enumeration
 * @param dataType type of data: String or Byte Array.
 * @param buffer pointer on data destination
 * @param bufferSize size of buffer
 * @param receiveCallBack callback for operation complete action
 * @return operation result code
 */
halUARTErrorCode halUARTReceive(halUART uart, halUARTDataType dataType, uint8_t* buffer,
        uint16_t bufferSize, halUARTCallBack receiveCallBack);

/**
 * UART transfer function
 * @param uart instance of UART from halUART enumeration
 * @param dataType type of data: String or Byte Array.
 * @param buffer pointer on data source
 * @param bufferSize size of buffer
 * @param transferCallBack callback for operation complete action
 * @return operation result code
 */
halUARTErrorCode halUARTTransfer(halUART uart, halUARTDataType dataType, uint8_t* buffer,
        uint16_t bufferSize, halUARTCallBack transferCallBack);

/**
 * Change overload action
 * @param uart instance of UART from halUART enumeration
 * @param callback overload action
 */
void halUARTSetOverloadCallBack(halUART uart, halUARTCallBack callback);

/**
 * Clear overload action
 * @param uart instance of UART from halUART enumeration
 */
void halUARTClearOverloadCallBack(halUART uart);

#endif /* HAL_HAL_UART_H_ */
