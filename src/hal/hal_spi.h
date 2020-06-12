/*
 * hal_spi.h
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_SPI_H_
#define HAL_HAL_SPI_H_

#include "stm32f4xx.h"

#define DUMMY_DATA 0x0000

//+------+--------+--------+--------+--------+
//|      |  MOSI  |  MISO  |  CLS   |   CS   |
//+------+--------+--------+--------+--------+
//| SPI1 |  PA7   |  PA6   |  PA5   |  PA4   |
//+------+--------+--------+--------+--------+
//| SPI2 |  PB15  |  PB14  |  PB13  |  PB12  |
//+------+--------+--------+--------+--------+
//| SPI3 |  PB5   |  PB4   |  PB3   |  PA15  |
//+------+--------+--------+--------+--------+

/// SPI enumeration
typedef enum {
    halSPI1 = 0,
    halSPI2,
    halSPI3,
    halSPICount
} halSPI;

typedef enum {
    halSPI_OK = 0,
    halSPI_TIMEOUT,
    halSPI_NOT_CONFIG,
    halSPI_IN_PROGRESS,
    halSPI_DATA_NULL_POINTER
} halSPIErrorCode;

/// SPI data transferring enumeration
typedef enum {
    halSPINotConfigured  = 0,
    halSPIReady
} halSPIStatus;

/// CPOL,CPHA modes enumeration
typedef enum {
    halSPIMode0 = 0,   /**< CPOL = 0, CPHA = 0 */
    halSPIMode1,       /**< CPOL = 0, CPHA = 1 */
    halSPIMode2,       /**< CPOL = 1, CPHA = 0 */
    halSPIMode3        /**< CPOL = 1, CPHA = 1 */
} halSPIMode;

/// SPI data sizes enumeration
typedef enum {
    halSPIDataSize8b = SPI_DataSize_8b,
    // halSPIDataSize16b = SPI_DataSize_16b   /** Not used. */
} halSPIDataSize;

/// SPI data direction enumeration
typedef enum {
    halSPIFullDirection2Lines = SPI_Direction_2Lines_FullDuplex,
    halSPITXDirection1Line = SPI_Direction_1Line_Tx,
    halSPIRXDirection1Line = SPI_Direction_1Line_Rx,
    halSPIRXDirection2Line = SPI_Direction_2Lines_RxOnly
} halSPIDirection;

/// SPI Frequency prescaller enumeration
typedef enum {
    halSPIFrequencyPrescaller2 = SPI_BaudRatePrescaler_2,
    halSPIFrequencyPrescaller4 = SPI_BaudRatePrescaler_4,
    halSPIFrequencyPrescaller8 = SPI_BaudRatePrescaler_8,
    halSPIFrequencyPrescaller16 = SPI_BaudRatePrescaler_16,
    halSPIFrequencyPrescaller32 = SPI_BaudRatePrescaler_32,
    halSPIFrequencyPrescaller64 = SPI_BaudRatePrescaler_64,
    halSPIFrequencyPrescaller128 = SPI_BaudRatePrescaler_128,
    halSPIFrequencyPrescaller256 = SPI_BaudRatePrescaler_256
} halSPIFrequencyPrescaller;

typedef enum {
    halSPIFirstBitMSB = SPI_FirstBit_MSB,
    halSPIFirstBitLSB = SPI_FirstBit_LSB
} halSPIFirstBit;

typedef enum {
    halSPINSSTypeHard = SPI_NSS_Hard,
    halSPINSSTypeSoft = SPI_NSS_Soft
} halSPINSSType;

typedef struct{
    halSPIMode halSPIMode;
    halSPIDataSize halSPIDataSize;
    halSPIDirection halSPIDirection;
    halSPIFrequencyPrescaller halSPIFrequencyPrescaller;
    halSPIFirstBit halSPIFirstBit;
    halSPINSSType halSPITypeNSS;
} halSPIInitStruct;

/**
 * Initial function for SPI
 * @param spi value of spi enumeration for configuration
 * @param initStruct structure of SPI configuration options
 */
void halSPIInit(halSPI spi, halSPIInitStruct* initStruct);

/**
 * Send data array function
 * @param spi value of spi enumeration
 * @param src pointer on send data
 * @return SPI error code
 */
halSPIErrorCode halSPISendByte(halSPI spi, uint8_t* src);

/**
 * Receive data array function
 * @param spi value of spi enumeration
 * @param dest pointer for receive data
 * @return SPI error code
 */
halSPIErrorCode halSPIReceiveByte(halSPI spi, uint8_t* dest);

/**
 * Select SPI device
 * @param spi value of spi enumeration
 */
halSPIErrorCode halSPIResetCS(halSPI spi);

/**
 * Unselect SPI device
 * @param spi value of spi enumeration
 */
halSPIErrorCode halSPISetCS(halSPI spi);

#endif /* HAL_HAL_SPI_H_ */
