/*
 * drv_fm25l16b.h
 *
 *  Created on: 10 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_FM25L16B_H_
#define DRV_DRV_FM25L16B_H_

#include "hal_spi.h"

#define drvFM2516B_LAST_ADDRESS 0x7FF

/// SPI enumeration
typedef enum {
    drvFRAM_SPI1 = 0u,
    drvFRAM_SPI2,
    drvFRAM_SPI3,
    drvFRAMCount
} drvFRAM_SPI;

#define WP_BITS_OFFSET 2
#define ONE_BYTE_OFFSET 8

#define TWO_BITS_MASK 0x03
#define LOWER_BYTE_MASK 0xFF
#define UPPER_BYTE_MASK 0x07

#define GET_BP_FROM_SR(x) (((x) >> (WP_BITS_OFFSET)) & TWO_BITS_MASK)

/// SPI enumeration
typedef enum {
    drvBP0 = 0x00,
    drvBP1,
    drvBP2,
    drvBP3,
    drvBL_Count
} drvProtectionLevels;

typedef enum {
    drvFRAM_READY = 0,
    drvFRAM_BUSY
} drvFRAMStatus;

typedef enum {
    drvFRAM_OK = halSPI_OK,
    drvFRAM_NOT_CONFIG = halSPI_NOT_CONFIG,
    drvFRAM_IN_PROGRESS = halSPI_IN_PROGRESS,
    drvFRAM_DATA_NULL_POINTER = halSPI_DATA_NULL_POINTER,
    drvFRAM_NO_OPERATION,
    drvFRAM_OUT_OF_MEMORY,
    drvFRAM_READ_ONLY
} drvSPIErrorCode;

typedef enum {
    WREN = 0x06,
    WRDI = 0x04,
    RDSR = 0x05,
    WRSR = 0x01,
    READ = 0x03,
    WRITE = 0x02
} drvFRAMOpCodes;

typedef enum {
    drvSended = 0,
    drvInSending,
    drvNotSended
} drvFRAMAddressStatusCodes;

/// Structure for write-read operation
typedef struct {
    uint8_t opcode;			                      /** Operation code */
    uint16_t address;                             /** Memory address for read/write operations */
	drvFRAMAddressStatusCodes addressCode;        /** Address code that shows the process of address sending */
    uint8_t* data;                                /** Sending/Receiving data array */
    uint16_t dataLen;                             /** Receiving data array size */
} drvFRAMOperationInstruction;

/**
 * Initial function for FRAM
 * @param spi instance of SPI on the board
 */
void drvFRAMInit(drvFRAM_SPI spi);

/**
 * Transmit data array to the FRAM
 * @param spi instance of SPI on the board
 * @param address memory destination address
 * @param data pointer on data array
 * @param dataLen data array size
 * @return operation result code
 */
drvSPIErrorCode drvFRAMWriteArray(drvFRAM_SPI spi, uint16_t address, uint8_t* data, uint16_t dataLen);

/**
 * Transmit data array to the FRAM
 * @param spi instance of SPI on the board
 * @param address memory destination address
 * @param data pointer on byte destination
 * @return operation result code
 */
drvSPIErrorCode drvFRAMWriteByte(drvFRAM_SPI spi, uint16_t address, uint8_t* data);

/**
 * Receive data array from the FRAM
 * @param spi instance of SPI on the board
 * @param address memory source address
 * @param data pointer on data array
 * @param dataLen data array size
 * @return operation result code
 */
drvSPIErrorCode drvFRAMReadArray(drvFRAM_SPI spi, uint16_t address, uint8_t* data, uint16_t dataLen);

/**
 * Receive one byte from the FRAM
 * @param spi instance of SPI on the board
 * @param address memory source address
 * @param data pointer on byte destination
 * @return operation result code
 */
drvSPIErrorCode drvFRAMReadByte(drvFRAM_SPI spi, uint16_t address, uint8_t* data);

/**
 * Receive data array to the FRAM
 * @param spi instance of SPI on the board
 * @param bpCode block protection code from drvProtectionLevels enumeration
 * @return operation result code
 */
drvSPIErrorCode drvFRAMSetBPLevel(drvFRAM_SPI spi, drvProtectionLevels bpCode);

#endif /* DRV_DRV_FM25L16B_H_ */
