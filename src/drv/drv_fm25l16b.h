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

#define drvFM2516B_UPPER_ADDRESS_BYTE(X) (( (X) & 0x07 ) >> 8)
#define drvFM2516B_LOWER_ADDRESS_BYTE(X) (( (X) & 0xFF ))

/// SPI enumeration
typedef enum {
    drvFRAM_SPI1 = 0u,
    drvFRAM_SPI2,
    drvFRAM_SPI3,
    drvFRAMCount
} drvFRAM_SPI;

#define WP_BITS_OFFSET 2
#define GET_BP_FROM_SR(x) (((x) >> (WP_BITS_OFFSET)) & 0x03)

/// SPI enumeration
typedef enum {
    drvBP0 = 0x00,
    drvBP1,
    drvBP2,
    drvBP3,
    drvBL_Count
} drvProtectionLevels;

static drvFRAMFirstLockedAddress[drvBL_Count] = { 0x800, 0x600, 0x400, 0x000 };

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

#endif /* DRV_DRV_FM25L16B_H_ */
