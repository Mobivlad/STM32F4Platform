/*
 * hal_uart.h
 *
 *  Created on: 15 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_UART_H_
#define HAL_HAL_UART_H_

#include "stm32f4xx.h"

#define BUFFER_SIZE 20

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
    halUART_IN_PROGRESS,
    halUART_NOT_CONFIG,
    halUART_NULL_POINT
} halUARTErrorCode;

/**
 * Initial function for UART
 * @param uart instance of UART from halUART enumeration
 * @param initStruct pointer on structure for UART initialization
 */
void halInitUART(halUART uart, halUARTInitStruct* initStruct);

/**
 * Receive byte function
 * @param uart instance of UART from halUART enumeration
 * @param data pointer on byte destination
 * @return halUART error code
 */
halUARTErrorCode halUARTReceiveByte(halUART uart, uint8_t* data);

/**
 * Send byte function
 * @param uart instance of UART from halUART enumeration
 * @param data transfer byte
 * @return halUART error code
 */
halUARTErrorCode halUARTSendByte(halUART uart, uint8_t data);

#endif /* HAL_HAL_UART_H_ */
