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

typedef struct {
    GPIO_TypeDef*   port;
    uint16_t        pin;
    uint8_t         pin_source;
    uint32_t        pin_rcc;
} uart_af_pin_data;

typedef struct {
    USART_TypeDef*      usart;

    uart_af_pin_data    rx;
    uart_af_pin_data    tx;

    uint8_t             usart_af;
    IRQn_Type           usart_irnq;

    uint32_t            usart_rcc;
    void(*usart_rcc_function)(uint32_t, FunctionalState);
} uart_data_struct;

/// UART instance enumerations
typedef enum {
    halUART1 = 0,       // DO NOT WORK ON STM32F4DISCOVERY
    halUART2,
    halUART3,
    halUART4,
    halUART5,
    halUART6,
    halUART_Count
} halUART;

typedef struct {
    USART_InitTypeDef   uart_def;

    halUART             uart;
    uint8_t             uartStatus;

    uint8_t*            r_buffer;
    uint16_t            r_buffer_size;
    halUARTCallBack     rCallBack;
    uint16_t            r_index;

    uint8_t*            t_buffer;
    uint16_t            t_buffer_size;
    halUARTCallBack     tCallBack;
    uint16_t            t_index;

    halUARTCallBack     overloadCallBack;
} halUART_struct;

/// UART BaudRate speed
typedef enum {
    halUARTBaudRate9600     = 9600,
    halUARTBaudRate19200    = 19200,
    halUARTBaudRate38400    = 38400,
    halUARTBaudRate57600    = 57600,
    halUARTBaudRate115200   = 115200
} halUARTBaudRate;

/// UART data size
typedef enum {
    halUARTWordLength8b = USART_WordLength_8b,
    halUARTWordLength9b = USART_WordLength_9b
} halUARTWordLength;

/// UART stop bits size
typedef enum {
    halUARTStopBits0_5  = USART_StopBits_0_5,
    halUARTStopBits1    =  USART_StopBits_1,
    halUARTStopBits1_5  = USART_StopBits_1_5,
    halUARTStopBits2    = USART_StopBits_2
} halUARTStopBits;

/// UART parity checking type
typedef enum {
    halUARTNoParity     = USART_Parity_No,
    halUARTEvenParity   = USART_Parity_Even,
    halUARTOddParity    = USART_Parity_Odd
} halUARTParity;

/// Initial structure for UART
typedef struct {
    halUARTBaudRate     halUARTBaudRate;
    halUARTWordLength   halUARTWordLength;
    halUARTStopBits     halUARTStopBits;
    halUARTParity       halUARTParity;
} halUARTInitStruct;

/// UART errors codes
typedef enum {
    halUART_OK = 0,
    halUART_BUSY,
    halUART_NOT_CONFIG,
    halUART_ALREADY_CONFIG,
    halUART_NULL_POINT
} halUARTErrorCode;

typedef enum {
    CONFIG      = 0x01,
    RX_READY    = 0x02,
    RX_STRING   = 0x04,
    TX_READY    = 0x08,
    TX_STRING   = 0x10,
} halUARTReadyStatus;

typedef enum {
    halUARTDataType_String,
    halUARTDataType_Array
} halUARTDataType;

/**
 * Initial function for UART
 * @param uartStruct pointer on UART instance structure
 * @param initStruct pointer on structure for UART initialization
 */
halUARTErrorCode halUARTInit(halUART_struct* uartStruct, halUARTInitStruct* initStruct);

/**
 * Deinitial function for UART
 * @param uartStruct pointer on UART instance structure
 * @param initStruct pointer on structure for UART initialization
 */
halUARTErrorCode halUARTDeinit(halUART_struct* uartStruct);

/**
 * UART receive function
 * @param uartStruct pointer on UART instance structure
 * @param dataType type of data: String or Byte Array.
 * @param buffer pointer on data source
 * @param bufferSize size of buffer
 * @param receiveCallBack callback for operation complete action
 * @return operation result code
 */
halUARTErrorCode halUARTReceive(halUART_struct* uartStruct, halUARTDataType dataType,
        uint8_t* buffer, uint16_t bufferSize, halUARTCallBack receiveCallBack);

/**
 * UART transfer function
 * @param uartStruct pointer on UART instance structure
 * @param dataType type of data: String or Byte Array.
 * @param buffer pointer on data source
 * @param bufferSize size of buffer
 * @param transferCallBack callback for operation complete action
 * @return operation result code
 */
halUARTErrorCode halUARTTransfer(halUART_struct* uartStruct, halUARTDataType dataType,
        uint8_t* buffer, uint16_t bufferSize, halUARTCallBack transferCallBack);
#endif /* HAL_HAL_UART_H_ */
