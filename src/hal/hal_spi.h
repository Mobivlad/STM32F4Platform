/*
 * hal_spi.h
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_SPI_H_
#define HAL_HAL_SPI_H_

#include "stm32f4xx.h"
#include <stddef.h>

#define DUMMY_DATA 0x0000

typedef void (*halSPICallBack)();

/// SPI enumeration
typedef enum {
    halSPI1 = 0,
    halSPI2,
    halSPI3,
    halSPI_Count
} halSPI;

typedef struct {
    GPIO_TypeDef*   port;
    uint16_t        pin;
} pin_data_t;

typedef struct {
    pin_data_t      pin_data;
    uint8_t         pin_source;
    uint32_t        pin_rcc;
} spi_af_pin_data;

typedef struct {
    SPI_TypeDef*    spi;

    spi_af_pin_data mosi;
    spi_af_pin_data miso;
    spi_af_pin_data cls;
    pin_data_t      cs;

    uint8_t         spi_af;
    IRQn_Type       spi_irnq;

    uint32_t        spi_rcc;
    void(*spi_rcc_function)(uint32_t, FunctionalState);
} spi_data_struct;

//+------+--------+--------+--------+--------+
//|      |  MOSI  |  MISO  |  CLS   |   CS   |
//+------+--------+--------+--------+--------+
//| SPI1 |  PA7   |  PA6   |  PA5   |  PA4   |
//+------+--------+--------+--------+--------+
//| SPI2 |  PB15  |  PB14  |  PB13  |  PB12  |
//+------+--------+--------+--------+--------+
//| SPI3 |  PB5   |  PB4   |  PB3   |  PA15  |
//+------+--------+--------+--------+--------+

typedef struct {
    SPI_TypeDef     spi_def;

    halSPI          spi;

    // SPI status flag container
    uint8_t         spiStatus;

    // callback on action complete
    halSPICallBack  callback;

    // transfer and receive buffers
    uint8_t*        buffer;
    uint16_t        bufferSize;

    // transfer and receive iteration counter
    uint16_t        tx_index;
    uint16_t        rx_index;
} halSPI_struct;

enum {
    halSPI_Config           = 0x01,
    halSPI_Ready            = 0x02,
    halSPI_ReadOperation    = 0x04,    // 1 - read operation, 0 - send operation
};

typedef enum {
    halSPI_Read = 0,
    halSPI_Write = !halSPI_Read
} halSPI_operation;

typedef enum {
    halSPI_OK = 0,
    halSPI_NOT_CONFIG,
    halSPI_BUSY,
    halSPI_DATA_NULL_POINTER,
    halSPI_OUT_TIMEOUT
} halSPI_errorCode;

/// SPI data transferring enumeration
typedef enum {
    halSPINotConfigured  = 0,
    halSPIReady
} halSPI_status;

/// CPOL,CPHA modes enumeration
typedef enum {
    halSPI_Mode0 = 0,   /**< CPOL = 0, CPHA = 0 */
    halSPI_Mode1,       /**< CPOL = 0, CPHA = 1 */
    halSPI_Mode2,       /**< CPOL = 1, CPHA = 0 */
    halSPI_Mode3        /**< CPOL = 1, CPHA = 1 */
} halSPI_mode;

/// SPI data sizes enumeration
typedef enum {
    halSPI_DataSize8b    = SPI_DataSize_8b,
    // halSPIDataSize16b = SPI_DataSize_16b   /** Not used. */
} halSPI_dataSize;

/// SPI data direction enumeration
typedef enum {
    halSPI_FullDirection2Lines   = SPI_Direction_2Lines_FullDuplex,
    halSPI_TXDirection1Line      = SPI_Direction_1Line_Tx,
    halSPI_RXDirection1Line      = SPI_Direction_1Line_Rx,
    halSPI_RXDirection2Line      = SPI_Direction_2Lines_RxOnly
} halSPI_direction;

/// SPI Frequency prescaller enumeration
typedef enum {
    halSPI_FrequencyPrescaller2      = SPI_BaudRatePrescaler_2,
    halSPI_FrequencyPrescaller4      = SPI_BaudRatePrescaler_4,
    halSPI_FrequencyPrescaller8      = SPI_BaudRatePrescaler_8,
    halSPI_FrequencyPrescaller16     = SPI_BaudRatePrescaler_16,
    halSPI_FrequencyPrescaller32     = SPI_BaudRatePrescaler_32,
    halSPI_FrequencyPrescaller64     = SPI_BaudRatePrescaler_64,
    halSPI_FrequencyPrescaller128    = SPI_BaudRatePrescaler_128,
    halSPI_FrequencyPrescaller256    = SPI_BaudRatePrescaler_256
} halSPI_frequencyPrescaller;

typedef enum {
    halSPI_FirstBitMSB = SPI_FirstBit_MSB,
    halSPI_FirstBitLSB = SPI_FirstBit_LSB
} halSPI_firstBit;

typedef enum {
    halSPI_NSSTypeHard = SPI_NSS_Hard,
    halSPI_NSSTypeSoft = SPI_NSS_Soft
} halSPI_NSSType;

typedef struct{
    halSPI_mode                  mode;
    halSPI_dataSize              dataSize;
    halSPI_direction             direction;
    halSPI_frequencyPrescaller   frequencyPrescaller;
    halSPI_firstBit              firstBit;
    halSPI_NSSType               typeNSS;
} halSPI_initStruct;

/**
 * Initial function for SPI
 * @param spiStruct pointer on UART instance structure
 * @param initStruct structure of SPI configuration options
 */
halSPI_errorCode halSPIInit(halSPI_struct* spiStruct, halSPI_initStruct* initStruct);

/**
 * Select SPI device
 * @param spiStruct pointer on UART instance structure
 */
halSPI_errorCode halSPIResetCS(halSPI_struct* spiStruct);

/**
 * Unselect SPI device
 * @param spiStruct pointer on UART instance structure
 */
halSPI_errorCode halSPISetCS(halSPI_struct* spiStruct);

/**
 * Send data array function using interrupt
 * @param spiStruct pointer on UART instance structure
 * @param data pointer on send data
 * @param dataSize size of send data
 * @param sendAction action on send complete
 * @return SPI error code
 */
halSPI_errorCode halSPISendIT(halSPI_struct* spiStruct, uint8_t* data, uint16_t dataSize,
        halSPICallBack sendAction);

/**
 * Receive data array function using interrupt
 * @param spiStruct pointer on UART instance structure
 * @param data pointer on send data
 * @param dataSize size of send data
 * @param sendAction action on send complete
 * @return SPI error code
 */
halSPI_errorCode halSPIReceiveIT(halSPI_struct* spiStruct, uint8_t* data, uint16_t dataSize,
        halSPICallBack receiveAction);

/**
 * Send data array function using interrupt
 * @param spiStruct pointer on UART instance structure
 * @param data pointer on send data
 * @param dataSize size of send data
 * @param timeout timeout for one element sending
 * @return SPI error code
 */
halSPI_errorCode halSPISend(halSPI_struct* spiStruct, uint8_t* data, uint16_t dataSize,
        uint16_t timeout);

/**
 * Receive data array function using interrupt
 * @param spiStruct pointer on UART instance structure
 * @param data pointer on send data
 * @param dataSize size of send data
 * @param timeout timeout for one element receiving
 * @return SPI error code
 */
halSPI_errorCode halSPIReceive(halSPI_struct* spiStruct, uint8_t* data, uint16_t dataSize,
        uint16_t timeout);

#endif /* HAL_HAL_SPI_H_ */
