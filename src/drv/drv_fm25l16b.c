/*
 * drv_fm25l16b.c
 *
 *  Created on: 5 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_fm25l16b.h"

void drvFRAMInit(drvFRAM_SPI spi) {
    halSPIInitStruct initStruct;
    initStruct.halSPIDataSize = halSPIDataSize8b;
    initStruct.halSPIDirection = halSPIFullDirection2Lines;
    initStruct.halSPIFirstBit = halSPIFirstBitMSB;
    if (spi == drvFRAM_SPI1)
        initStruct.halSPIFrequencyPrescaller = halSPIFrequencyPrescaller8;
    else
        initStruct.halSPIFrequencyPrescaller = halSPIFrequencyPrescaller4;
    initStruct.halSPIMode = halSPIMode0;
    initStruct.halSPITypeNSS = halSPINSSTypeSoft;
    halSPIInit(spi, &initStruct);
}

/**
 * Send write enable command
 * @param spi instance of SPI on the board
 * @return operation error code from drvSPIErrorCode enumeration
 */
static drvSPIErrorCode drvFRAMEnableWrite(drvFRAM_SPI spi) {
    drvSPIErrorCode operationResult = drvFRAM_OK;
    uint8_t dataArray[] = { WREN };
    halSPISetCS(spi);
    operationResult = halSPISendDataArray(spi, dataArray, 1,
            DRV_FM25L16B_TIMEOUT);
    halSPIResetCS(spi);
    return operationResult;
}

/**
 * Send write disable command
 * @param spi instance of SPI on the board
 * @return operation error code from drvSPIErrorCode enumeration
 */
static drvSPIErrorCode drvFRAMDisableWrite(drvFRAM_SPI spi) {
    drvSPIErrorCode operationResult = drvFRAM_OK;
    uint8_t dataArray[] = { WRDI };
    halSPISetCS(spi);
    operationResult = halSPISendDataArray(spi, dataArray, 1,
    DRV_FM25L16B_TIMEOUT);
    halSPIResetCS(spi);
    return operationResult;
}

/**
 * Initial function for data transferring
 * @param spi instance of SPI on the board
 * @param address start transferring address
 * @return operation error code from drvSPIErrorCode enumeration
 */
static drvSPIErrorCode drvFRAMInitSend(drvFRAM_SPI spi, uint16_t address) {
    drvSPIErrorCode operationResult = drvFRAMEnableWrite(spi);
    halSPISetCS(spi);
    if (operationResult != drvFRAM_OK)
        return operationResult;
    uint8_t dataArray[] = { WRITE, ((address >> 8) & 0x07), (address & 0xFF) };
    operationResult = halSPISendDataArray(spi, dataArray, 3,
    DRV_FM25L16B_TIMEOUT);
    return operationResult;
}

/**
 * Unselect FRAM chip
 * @param spi instance of SPI on the board
 */
static void drvFRAMStopOperation(drvFRAM_SPI spi) {
    halSPIResetCS(spi);
}

/**
 * Initial function for data receiving
 * @param spi instance of SPI on the board
 * @param address start receiving address
 * @return operation error code from drvSPIErrorCode enumeration
 */
static drvSPIErrorCode drvFRAMInitReceive(drvFRAM_SPI spi, uint16_t address) {
    halSPISetCS(spi);
    drvSPIErrorCode operationResult = drvFRAM_OK;
    uint8_t dataArray[] = { READ, ((address >> 8) & 0x07), (address & 0xFF) };
    operationResult = halSPISendDataArray(spi, dataArray, 3,
    DRV_FM25L16B_TIMEOUT);
    return operationResult;
}

drvSPIErrorCode drvFRAMSendData(drvFRAM_SPI spi, uint16_t address,
        uint8_t* data, uint16_t dataLen) {
    if (DRV_FM25L16B_LAST_ADDRESS - address + 1 < dataLen)
        return drvFRAM_OUT_OF_MEMORY;
    drvSPIErrorCode operationResult = drvFRAMInitSend(spi, address);
    if (operationResult != drvFRAM_OK)
        return operationResult;
    operationResult = halSPISendDataArray(spi, data, dataLen,
            DRV_FM25L16B_TIMEOUT);
    drvFRAMStopOperation(spi);
    return operationResult;
}

drvSPIErrorCode drvFRAMReceiveData(drvFRAM_SPI spi, uint16_t address,
        uint8_t* data, uint16_t dataLen) {
    if (DRV_FM25L16B_LAST_ADDRESS - address + 1 < dataLen)
        return drvFRAM_OUT_OF_MEMORY;
    drvSPIErrorCode operationResult = drvFRAMInitReceive(spi, address);
    if (operationResult != drvFRAM_OK)
        return operationResult;
    operationResult = halSPIReceiveDataArray(spi, data, dataLen,
    DRV_FM25L16B_TIMEOUT);
    drvFRAMStopOperation(spi);
    return operationResult;
}

drvSPIErrorCode drvFRAMReadStatusRegister(drvFRAM_SPI spi, uint8_t* dest) {
    halSPISetCS(spi);
    uint8_t dataArray[] = { RDSR };
    drvSPIErrorCode operationResult = halSPISendDataArray(spi, dataArray, 1,
    DRV_FM25L16B_TIMEOUT);
    if (operationResult != drvFRAM_OK)
        return operationResult;
    operationResult = halSPIReceiveDataArray(spi, dest, 1,
    DRV_FM25L16B_TIMEOUT);
    drvFRAMStopOperation(spi);
    return operationResult;
}

drvSPIErrorCode drvFRAMWriteStatusRegister(drvFRAM_SPI spi, uint8_t data) {
    drvFRAMEnableWrite(spi);
    halSPISetCS(spi);
    uint8_t dataArray[] = { WRSR, data };
    drvSPIErrorCode operationResult = halSPISendDataArray(spi, dataArray, 2,
    DRV_FM25L16B_TIMEOUT);
    halSPIResetCS(spi);
    return operationResult;
}

