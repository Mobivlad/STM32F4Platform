/*
 * drv_fm25l16b.h
 *
 *  Created on: 10 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_FM25L16B_H_
#define DRV_DRV_FM25L16B_H_

#include "hal_spi.h"

/// SPI enumeration
typedef enum {
    drvFRAM_SPI1 = 0,
    drvFRAM_SPI2,
    drvFRAM_SPI3,
    drvFRAMCount
} drvFRAM_SPI;

typedef enum {
    drvFRAM_OK = halSPI_OK,
    drvFRAM_NOT_CONFIG = halSPI_NOT_CONFIG,
    drvFRAM_IN_PROGRESS = halSPI_IN_PROGRESS,
    drvFRAM_DATA_NULL_POINTER = halSPI_DATA_NULL_POINTER,
    drvFRAM_OUT_OF_MEMORY
} drvSPIErrorCode;

typedef enum {
    WREN = 0x06,
    WRDI = 0x04,
    RDSR = 0x05,
    WRSR = 0x01,
    READ = 0x03,
    WRITE = 0x02
} drvFRAMOpCodes;

/// Structure for write-read operation
typedef struct {
    uint16_t* source;           /** Sending data array */
    uint16_t sourceLen;         /** Sending data array size */
    uint16_t* dest;             /** Data array for receive data */
    uint16_t destLen;           /** Receive data length*/
} drvFRAMOperationInstruction;

/**
 * Initial function for FRAM
 * @param spi instance of SPI on the board
 */
void drvFRAMInit(drvFRAM_SPI spi);

/**
 * Write to FRAM function
 * @param spi instance of SPI on the board
 * @param address flash memory start address
 * @param data write data source
 * @param dataLen length of source data
 * @return operation error code from drvSPIErrorCode enumeration
 */
drvSPIErrorCode drvFRAMSendData(drvFRAM_SPI spi, uint16_t address,
        uint8_t* data, uint16_t dataLen);

/**
 * Read from FRAM function
 * @param spi instance of SPI on the board
 * @param address flash memory start address
 * @param data pointer on read data destination
 * @param dataLen length of destination data
 * @return operation error code from drvSPIErrorCode enumeration
 */
drvSPIErrorCode drvFRAMReceiveData(drvFRAM_SPI spi, uint16_t address,
        uint8_t* data, uint16_t dataLen);

/**
 * Change value of status register
 * @param spi instance of SPI on the board
 * @param data new status register value
 * @return operation error code from drvSPIErrorCode enumeration
 */
drvSPIErrorCode drvFRAMWriteStatusRegister(drvFRAM_SPI spi, uint8_t data);

/**
 * Get value of status register
 * @param spi instance of SPI on the board
 * @param data pointer on status register destination
 * @return operation error code from drvSPIErrorCode enumeration
 */
drvSPIErrorCode drvFRAMReadStatusRegister(drvFRAM_SPI spi, uint8_t* dest);

#endif /* DRV_DRV_FM25L16B_H_ */
