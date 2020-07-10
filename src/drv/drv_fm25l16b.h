/*
 * drv_fm25l16b.h
 *
 *  Created on: 6 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_FM25L16B_H_
#define DRV_DRV_FM25L16B_H_

#include "hal_spi.h"

#define WP_BITS_OFFSET              2
#define ONE_BYTE_OFFSET             8

#define TWO_BITS_MASK               0x03
#define ADDRESS_FIRST_BYTE_MASK     0x07
#define ADDRESS_SECOND_BYTE_MASK    0xFF
#define ADDRESS_BYTE_COUNT          2

#define MAX_TIMEOUT                 0xFFFF

#define MEMORY_SIZE                 0x800

#define FIRST_ADDRESS_BYTE(x)       ((uint8_t)(((x) >> ONE_BYTE_OFFSET) & ADDRESS_FIRST_BYTE_MASK))
#define SECOND_ADDRESS_BYTE(x)      ((uint8_t)((x) & ADDRESS_SECOND_BYTE_MASK))
#define GET_BP_FROM_SR(x)           (((x) >> (WP_BITS_OFFSET)) & TWO_BITS_MASK)
#define GET_SR_FROM_BP(x)           (((x) & TWO_BITS_MASK) << (WP_BITS_OFFSET))

typedef void (*drvFRAM_opCompleteCallback)();

typedef enum {
    drvFRAM_OK                  = halSPI_OK,
    drvFRAM_NOT_CONFIG          = halSPI_NOT_CONFIG,
    drvFRAM_BUSY                = halSPI_BUSY,
    drvFRAM_DATA_NULL_POINTER   = halSPI_DATA_NULL_POINTER,
    drvFRAM_OUT_OF_MEMORY,
    drvFRAM_READ_ONLY,
    drvFRAM_INIT_ERROR
} drvFRAM_errorCode;

typedef enum {
    STATE_NOT_INITED,       // FRAM is not inited
    STATE_IN_INIT,          // FRAM is in the initialization
    STATE_PROTECTED,        // FRAM ready read only
    STATE_BP_CHANGING,      // FRAM is in process of changing block protection level
    STATE_WAIT_TO_RUN,      // FRAM have command but not execute it
    STATE_RUN,              // FRAM execute command
    STATE_READY             // FRAM ready to read and write
} drvFRAM_state;

typedef enum {
    STEP_WREN,
    STEP_OPERATION,
    STEP_ADDRESS,
    STEP_WRITE,
    STEP_READ,
    STEP_OPERATION_END
} drvFRAM_operationStep;

typedef struct {
    halSPI                      spi;
    halSPI_frequencyPrescaller  prescaller;
} fram_data_struct;

typedef enum {
    WREN = 0,       // Enable write operation
    WRDI,           // Disable write operation
    RDSR,           // Read status register
    WRSR,           // Write to status register
    READ,           // Read data from FRAM
    WRITE,          // Write data to FRAM
    opCodes_Count
} drvFRAM_opCodesNames;

/// Structure for write-read operation
typedef struct {
    drvFRAM_opCodesNames        opcode;                         /** Operation code */
    uint8_t                     address[ADDRESS_BYTE_COUNT];    /** Memory address for read/write operations */
    uint8_t*                    data;                           /** Sending/Receiving data array */
    uint16_t                    dataLen;                        /** Data array size */
    drvFRAM_operationStep       step;                           /** Data array size */
} drvFRAM_operationInstruction;

typedef enum {
    drvBP0 = 0x00,      // not protected
    drvBP1,             // last quarter is blocked
    drvBP2,             // second half is blocked
    drvBP3,             // full protected
    drvBL_Count
} drvFRAM_protectionLevel;

typedef struct {
    halSPI_struct                   spiStruct;
    drvFRAM_operationInstruction    currentOperation;
    drvFRAM_state                   state;
    drvFRAM_protectionLevel         protectionLevel;
} drvFRAM_struct;

/**
 * FRAM initial function
 * @param framStruct FRAM data structure
 * @return execute result code
 */
drvFRAM_errorCode drvFRAMInit(drvFRAM_struct* framStruct);

/**
 * FRAM execute function
 * @param framStruct FRAM data structure
 * @return execute result code
 */
drvFRAM_errorCode drvFRAMRun(drvFRAM_struct* framStruct);

/**
 * Read data array from FRAM
 * @param framStruct FRAM data structure
 * @param memoryAddress memory address for the first element
 * @param data pointer on data destination array
 * @param dataLen size of data destination array
 * @return execute result code
 */
drvFRAM_errorCode drvFRAMReadData(drvFRAM_struct* framStruct, uint16_t memoryAddress, uint8_t* data,
        uint16_t dataLen);

/**
 * Write data array to FRAM
 * @param framStruct FRAM data structure
 * @param memoryAddress memory address for the first element
 * @param data pointer on data source array
 * @param dataLen size of data source array
 * @return execute result code
 */
drvFRAM_errorCode drvFRAMWriteData(drvFRAM_struct* framStruct, uint16_t memoryAddress, uint8_t* data,
        uint16_t dataLen);

/**
 * Change block protection level
 * @param framStruct FRAM data structure
 * @param newBP new block protection level
 * @return execute result code
 */
drvFRAM_errorCode drvFRAMSetBP(drvFRAM_struct* framStruct, drvFRAM_protectionLevel newBP);

#endif /* DRV_DRV_FM25L16B_H_ */
