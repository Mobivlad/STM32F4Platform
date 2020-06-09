/*
 * drv_fm25l16b.h
 *
 *  Created on: 5 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_FM25L16B_H_
#define DRV_DRV_FM25L16B_H_

#include "hal_spi.h"

#define DRV_FM25L16B_TIMEOUT 0xFFFF
#define DRV_FM25L16B_LAST_ADDRESS 0x7FF

/// SPI enumeration
typedef enum {
    drvFRAM_SPI1 = 0,
    drvFRAM_SPI2,
    drvFRAM_SPI3
} drvFRAM_SPI;

typedef enum {
    drvFRAM_OK = halSPI_OK,
    drvFRAM_TIMEOUT = halSPI_TIMEOUT,
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
