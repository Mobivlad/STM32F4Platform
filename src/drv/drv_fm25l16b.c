/*
 * drv_fm25l16b.c
 *
 *  Created on: 10 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_fm25l16b.h"

static drvFRAMStatus FRAM_status[drvFRAMCount];
static uint16_t dataCount[drvFRAMCount];
static uint16_t currentOperation[drvFRAMCount];
static uint8_t WRENStatus[drvFRAMCount];    // 1 if WREN is set, 0 - not
static drvFRAMOperationInstruction currentInstruction[drvFRAMCount] = { 0 };

static drvFRAMOperationInstruction WREN_instruction = { WREN, 0, 0, 0, 0 };
static drvFRAMOperationInstruction WRDI_instruction = { WRDI, 0, 0, 0, 0 };

void drvFRAMInit(drvFRAM_SPI spi) {
    halSPIInitStruct initStruct;
    initStruct.halSPIDataSize = halSPIDataSize8b;
    initStruct.halSPIDirection = halSPIFullDirection2Lines;
    initStruct.halSPIFirstBit = halSPIFirstBitMSB;
    initStruct.halSPITypeNSS = halSPINSSTypeSoft;
    initStruct.halSPIMode = halSPIMode0;

    if (spi == drvFRAM_SPI1)
        initStruct.halSPIFrequencyPrescaller = halSPIFrequencyPrescaller8;
    else
        initStruct.halSPIFrequencyPrescaller = halSPIFrequencyPrescaller4;

    halSPIInit(spi, &initStruct);

    FRAM_status[spi] = drvFRAM_READY;
    dataCount[spi] = 0;
    WRENStatus[spi] = 0;
    currentOperation[spi] = 0;
    drvFRAMInitStruct(&currentInstruction[spi]);
}

static drvSPIErrorCode drvFRAMExecuteSingleOperation(drvFRAM_SPI spi, drvFRAMOperationInstruction* instuction) {

    if (FRAM_status[spi] == drvFRAM_READY) {
        FRAM_status[spi] = drvFRAM_BUSY;
        dataCount[spi] = instuction -> dataLen;
        currentOperation[spi] = instuction->opcode;
        halSPIErrorCode res_code = halSPISetCS(spi);
        if(res_code != halSPI_OK){
            return res_code;
        }
    }

	if (instuction->opcode != 0) {
    	halSPIErrorCode byteSendCodeStatus = halSPISendByte(spi, instuction->opcode);
    	switch(byteSendCodeStatus){
            case halSPI_OK:
                instuction->opcode = 0;
                break;
            default:
                return byteSendCodeStatus;
    	}
    }

    if (instuction->addressCode != drvSended) {
        halSPIErrorCode byteSendCodeStatus;
        switch(instuction->addressCode){
            case drvNotSended:
                byteSendCodeStatus = halSPISendByte(spi,
                        drvFM2516B_UPPER_ADDRESS_BYTE(instuction->address));
                break;
            case drvInSending:
                byteSendCodeStatus = halSPISendByte(spi,
                        drvFM2516B_LOWER_ADDRESS_BYTE(instuction->address));
                break;
        }
        switch (byteSendCodeStatus) {
            case halSPI_OK:
                instuction->addressCode--;
                if (instuction->addressCode == drvSended)
                    break;
                return drvFRAM_IN_PROGRESS;
            default:
                return byteSendCodeStatus;
        }
    }

    // pointer on function send or receive data
    halSPIErrorCode (*function)(halSPI, uint8_t*);

    switch(currentOperation[spi]){
        case WRITE:
        case WRSR:
            function = halSPISendByte;
            break;
        case READ:
        case RDSR:
            function = halSPIReceiveByte;
            break;
    }

    if(function != 0) {
        if (dataCount[spi] != 0) {
            halSPIErrorCode byteSendCodeStatus = halSPISendByte(spi,
                    instuction->data[instuction->dataLen - dataCount[spi]]);
            switch (byteSendCodeStatus) {
                case halSPI_OK:
                    dataCount[spi]--;
                    break;
                default:
                    return byteSendCodeStatus;
            }
        }
    }

    if(dataCount[spi] != 0) {
        return drvFRAM_IN_PROGRESS;
    }

    FRAM_status[spi] = drvFRAM_READY;
    halSPIResetCS(spi);
    return drvFRAM_OK;
}

static drvSPIErrorCode drvFRAMExecuteOperation(drvFRAM_SPI spi, drvFRAMOperationInstruction* instuction){
    if (instuction == 0)
            return drvFRAM_NO_OPERATION;
    drvSPIErrorCode resultCode;
    switch (instuction->opcode) {
        case WRITE:
        case WRSR:
            if (!WRENStatus[spi]) {
                resultCode = drvFRAMExecuteSingleOperation(spi,&WREN_instruction);
                if(resultCode == drvFRAM_OK){
                    WRENStatus[spi] = 1;
                }
                return drvFRAM_IN_PROGRESS;
            } else {
                resultCode = drvFRAMExecuteSingleOperation(spi,instuction);
                if(resultCode == drvFRAM_OK){
                    WRENStatus[spi] = 0;
                }
                return resultCode;
            }
        default:
            return drvFRAMExecuteSingleOperation(spi,instuction);
    }
}

static void drvFRAMInitStruct(drvFRAMOperationInstruction* str){
    str->opcode = 0;
    str->address = 0;
    str->addressCode = 0;
    str->data = 0;
    str->dataLen = 0;
}

static drvSPIErrorCode drvFRAMInitSendInstruction(uint16_t address,
        uint8_t* data, uint16_t dataLen, drvFRAMOperationInstruction* operationDest) {
    // address if out of FRAM memory
    if (address > drvFM2516B_LAST_ADDRESS)
        return drvFRAM_OUT_OF_MEMORY;

    // check on out of FRAM memory
    if (drvFM2516B_LAST_ADDRESS - address + 1 > dataLen)
        return drvFRAM_OUT_OF_MEMORY;

    if (data == 0 || dataLen == 0)
        return drvFRAM_DATA_NULL_POINTER;

    drvFRAMInitStruct(operationDest);
    operationDest->opcode = WRITE;
    operationDest->address = address;
    operationDest->addressCode = drvNotSended;
    operationDest->data = data;
    operationDest->dataLen = dataLen;

	return drvFRAM_OK;
}

static drvSPIErrorCode drvFRAMInitRSTInstruction(uint8_t* stDest, drvFRAMOperationInstruction* operationDest) {

    drvFRAMInitStruct(operationDest);
    operationDest->opcode = RDSR;
    operationDest->data = stDest;
    operationDest->dataLen = 1;

    return drvFRAM_OK;
}

static drvSPIErrorCode drvFRAMInitWRSTInstruction(uint8_t stSrc, drvFRAMOperationInstruction* operationDest) {

    drvFRAMInitStruct(operationDest);
    operationDest->opcode = WRSR;
    operationDest->data = &stSrc;
    operationDest->dataLen = 1;

    return drvFRAM_OK;
}

static drvSPIErrorCode drvFRAMInitReceiveInstruction(uint16_t address,
        uint8_t* data, uint16_t dataLen, drvFRAMOperationInstruction* operationDest) {
    // address if out of FRAM memory
    if (address > drvFM2516B_LAST_ADDRESS)
        return drvFRAM_OUT_OF_MEMORY;

    // check on out of FRAM memory
    if (drvFM2516B_LAST_ADDRESS - address + 1 > dataLen)
        return drvFRAM_OUT_OF_MEMORY;

    drvFRAMInitStruct(operationDest);
    operationDest->opcode = READ;
    operationDest->address = address;
    operationDest->addressCode = drvNotSended;
    operationDest->data = data;
    operationDest->dataLen = dataLen;

    return drvFRAM_OK;
}



drvSPIErrorCode drvFRAMSetBlockProtection(drvFRAM_SPI spi, drvProtectionLevels level){
    if(currentInstruction[spi].opcode == 0){
        drvFRAMInitWRSTInstruction((level << WP_BITS_OFFSET), &currentInstruction[spi]);
    }
}
