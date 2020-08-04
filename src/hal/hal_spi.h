/*
 * hal_spi.h
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_SPI_H_
#define HAL_HAL_SPI_H_

#include "stm32f7xx.h"
#include <stddef.h>

#define DUMMY_DATA 0x0000

#define SPI_TIMEOUT 0xFFFF

/// SPI enumeration
typedef enum {
    halSPI1 = 0,
    halSPI2,
    halSPI3,
    halSPI4,
    halSPI5,
    halSPI6,
    halSPI_Count
} halSPI;

typedef enum {
    APB1,
    APB2
} halSPI_bus;

typedef struct {
    GPIO_TypeDef*   port;
    uint16_t        pin;
} pin_data_t;

typedef struct {
    GPIO_TypeDef *  port;
    uint32_t        pin;
    uint32_t        port_rcc;
    uint8_t         af;
} spi_af_pin_data;

typedef struct {
    SPI_TypeDef*    spi;

    spi_af_pin_data mosi;
    spi_af_pin_data miso;
    spi_af_pin_data cls;
    spi_af_pin_data cs;

    halSPI_bus      bus;
    uint32_t        rcc;
} spi_data_struct;

typedef struct {
    SPI_HandleTypeDef   spi_def;

    halSPI              spi;

    // SPI status flag container
    uint8_t             spiStatus;
} halSPI_struct;

enum {
    halSPI_Config           = 0x01,
    halSPI_Ready            = 0x02,
    halSPI_ReadOperation    = 0x04,    // 1 - read operation, 0 - send operation
};

typedef enum {
    halSPI_OK = 0,
    halSPI_NOT_CONFIG,
    halSPI_BUSY,
    halSPI_DATA_NULL_POINTER,
    halSPI_OUT_TIMEOUT
} halSPI_errorCode;

/// CPOL,CPHA modes enumeration
typedef enum {
    halSPI_Mode0 = 0,   /**< CPOL = 0, CPHA = 0 */
    halSPI_Mode1,       /**< CPOL = 0, CPHA = 1 */
    halSPI_Mode2,       /**< CPOL = 1, CPHA = 0 */
    halSPI_Mode3        /**< CPOL = 1, CPHA = 1 */
} halSPI_mode;

/// SPI data sizes enumeration
typedef enum {
    halSPI_DataSize8b    = SPI_DATASIZE_8BIT,
    // halSPIDataSize16b = SPI_DataSize_16b   /** Not used. */
} halSPI_dataSize;

/// SPI data direction enumeration
typedef enum {
    halSPI_FullDirection2Lines   = SPI_DIRECTION_2LINES,
    halSPI_TXDirection1Line      = SPI_DIRECTION_1LINE,
    halSPI_RXDirection2Line      = SPI_DIRECTION_2LINES_RXONLY
} halSPI_direction;

/// SPI Frequency prescaller enumeration
typedef enum {
    halSPI_FrequencyPrescaller2      = SPI_BAUDRATEPRESCALER_2,
    halSPI_FrequencyPrescaller4      = SPI_BAUDRATEPRESCALER_4,
    halSPI_FrequencyPrescaller8      = SPI_BAUDRATEPRESCALER_8,
    halSPI_FrequencyPrescaller16     = SPI_BAUDRATEPRESCALER_16,
    halSPI_FrequencyPrescaller32     = SPI_BAUDRATEPRESCALER_32,
    halSPI_FrequencyPrescaller64     = SPI_BAUDRATEPRESCALER_64,
    halSPI_FrequencyPrescaller128    = SPI_BAUDRATEPRESCALER_128,
    halSPI_FrequencyPrescaller256    = SPI_BAUDRATEPRESCALER_256
} halSPI_frequencyPrescaller;

typedef enum {
    halSPI_FirstBitMSB = SPI_FIRSTBIT_MSB,
    halSPI_FirstBitLSB = SPI_FIRSTBIT_LSB
} halSPI_firstBit;

typedef enum {
    halSPI_NSSTypeHard = SPI_NSS_HARD_OUTPUT,
    halSPI_NSSTypeSoft = SPI_NSS_SOFT
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
halSPI_errorCode halSPIInit(halSPI_struct* spiStruct, halSPI_initStruct* initialStruct);

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

/**
 * Return ready status of SPI
 * @param spiStruct pointer on UART instance structure
 * @return halSPI_BUSY if SPI in transferring or receiving else halSPI_OK
 */
halSPI_errorCode halSPIIsReady(halSPI_struct* spiStruct);

#endif /* HAL_HAL_SPI_H_ */
