/*
 * drv_fm25l16b.c
 *
 *  Created on: 10 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_fm25l16b.h"

/// Variables for simple command execution
/// Simple instruction consist of only one command, like WRDI
/// Complex instruction consist of several commands, like WRITE(WREN + WRITE)

static drvFRAMStatus FRAM_status[drvFRAMCount];
static uint16_t dataCount[drvFRAMCount];
static uint8_t currentOperation[drvFRAMCount];


/// Write enable instruction
static drvFRAMOperationInstruction WREN_instruction = { WREN, 0, 0, 0, 0 };
/// Write disable instruction
static drvFRAMOperationInstruction WRDI_instruction = { WRDI, 0, 0, 0, 0 };

/// Current executing instruction
static drvFRAMOperationInstruction instractions[drvFRAMCount] = { 0 };

/// Structure cleaning function
static void drvFRAMInitStruct(drvFRAMOperationInstruction* str) {
    str->opcode = 0;
    str->address = 0;
    str->addressCode = 0;
    str->data = 0;
    str->dataLen = 0;
}

/// Return address bytes by index from 2-bytes address
static uint8_t getAddressByte(uint16_t address, uint8_t byteAddressIndex){
    switch(byteAddressIndex){
        case 0:
            return address & 0xFF;
        case 1:
            return ( (address) & 0x07 ) >> 8;
    }
}

/// Execution simple instruction
static drvSPIErrorCode drvFRAMExecuteOperation(drvFRAM_SPI spi, drvFRAMOperationInstruction* instuction) {
    if (FRAM_status[spi] == drvFRAM_READY) {
        halSPIErrorCode res_code = halSPISetCS(spi);
        if (res_code != halSPI_OK){
            return res_code;
        } else {
            FRAM_status[spi] = drvFRAM_BUSY;
            dataCount[spi] = instuction->dataLen;
            currentOperation[spi] = instuction->opcode;
        }
    }

    if (currentOperation[spi] != 0) {
        halSPIErrorCode byteSendCodeStatus = halSPISendByte(spi, &(currentOperation[spi]));
        switch (byteSendCodeStatus) {
        case halSPI_OK:
            currentOperation[spi] = 0;
            break;
        default:
            return byteSendCodeStatus;
        }
    }

    if (instuction->addressCode != drvSended) {
        halSPIErrorCode byteSendCodeStatus;
        switch (instuction->addressCode) {
            case drvNotSended:
                byteSendCodeStatus = halSPISendByte(spi, getAddressByte(instuction->address, 1));
                break;
            case drvInSending:
                byteSendCodeStatus = halSPISendByte(spi, getAddressByte(instuction->address, 0));
                break;
            default:
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

    switch (currentOperation[spi]) {
        case WRITE:
        case WRSR:
            function = halSPISendByte;
            break;
        case READ:
        case RDSR:
            function = halSPIReceiveByte;
            break;
    }

    if (function != 0) {
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

    if (dataCount[spi] != 0) {
        return drvFRAM_IN_PROGRESS;
    }

    FRAM_status[spi] = drvFRAM_READY;
    halSPIResetCS(spi);
    return drvFRAM_OK;
}

/// Initial function for send instruction
static drvSPIErrorCode drvFRAMInitSendInstruction(uint16_t address, uint8_t* data, uint16_t dataLen,
        drvFRAMOperationInstruction* operationDest) {
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

/// Initial function for read status register instruction
static drvSPIErrorCode drvFRAMInitRDSRInstruction(uint8_t* stDest, drvFRAMOperationInstruction* operationDest) {

    drvFRAMInitStruct(operationDest);
    operationDest->opcode = RDSR;
    operationDest->data = stDest;
    operationDest->dataLen = 1;

    return drvFRAM_OK;
}

/// Initial function for write status register instruction
static drvSPIErrorCode drvFRAMInitWRSRInstruction(uint8_t stSrc, drvFRAMOperationInstruction* operationDest) {

    drvFRAMInitStruct(operationDest);
    operationDest->opcode = WRSR;
    operationDest->data = &stSrc;
    operationDest->dataLen = 1;

    return drvFRAM_OK;
}

/// Initial function for read data instruction
static drvSPIErrorCode drvFRAMInitReceiveInstruction(uint16_t address, uint8_t* data, uint16_t dataLen,
        drvFRAMOperationInstruction* operationDest) {
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
    currentOperation[spi] = 0;
    drvFRAMInitStruct(&instractions[spi]);
}

drvSPIErrorCode drvFRAMSetBPLevel(drvFRAM_SPI spi, drvProtectionLevels bpCode) {
    uint8_t newValue = bpCode << WP_BITS_OFFSET;
    drvSPIErrorCode status;
    if (instractions[spi].opcode == 0x00) {
        instractions[spi] = WREN_instruction;
    }

    if (instractions[spi].opcode == WREN) {
        status = drvFRAMExecuteOperation(spi, &instractions[spi]);
        if (status != drvFRAM_OK)
            return status;
        status = drvFRAMInitWRSRInstruction(newValue, &instractions[spi]);
        if (status != drvFRAM_OK)
            return status;
    }

    instractions[spi].opcode = 0;
    return drvFRAM_OK;
}

drvSPIErrorCode drvFRAMReceiveArray(drvFRAM_SPI spi, uint16_t address, uint8_t* data, uint16_t dataLen) {
    if (instractions[spi].opcode == 0) {
        drvSPIErrorCode status = drvFRAMInitReceiveInstruction(address, data, dataLen, &instractions[spi]);
        if (status != drvFRAM_OK)
            return status;
    }

    drvSPIErrorCode resCode = drvFRAMExecuteOperation(spi, &instractions[spi]);
    if (resCode == drvFRAM_OK) {
        instractions[spi].opcode = 0;
        return drvFRAM_OK;
    } else {
        return resCode;
    }
}

drvSPIErrorCode drvFRAMSendArray(drvFRAM_SPI spi, uint16_t address, uint8_t* data, uint16_t dataLen) {
    uint8_t statusData;
    drvSPIErrorCode status;

    if (instractions[spi].opcode == 0) {
        status = drvFRAMInitRDSRInstruction(&statusData, &instractions[spi]);
        if (status != drvFRAM_OK)
            return status;
    }

    // checking on memory write ability
    uint8_t bp_code = GET_BP_FROM_SR(statusData);
    switch (bp_code) {
        case 0x00:
            break;
        case 0x01:
        case 0x10:{
            uint32_t tmp_address = address + dataLen;
            if (tmp_address >= drvFRAMFirstLockedAddress[bp_code])
                return drvFRAM_READ_ONLY;
            break;
        }
        case 0x11:
            return drvFRAM_READ_ONLY;
    }

    if (instractions[spi].opcode == RDSR){
        status = drvFRAMExecuteOperation(spi, &instractions[spi]);
        if (status != drvFRAM_OK)
            return status;
        instractions[spi] = WREN_instruction;
    }

    if (instractions[spi].opcode == WREN) {
        status = drvFRAMExecuteOperation(spi, &instractions[spi]);
        if (status != drvFRAM_OK)
            return status;
        status = drvFRAMInitSendInstruction(address, data, dataLen, &instractions[spi]);
        if (status != drvFRAM_OK)
            return status;
    }

    if (instractions[spi].opcode == WRITE) {
        status = drvFRAMExecuteOperation(spi, &instractions[spi]);
        if (status != drvFRAM_OK)
            return status;
    }

    instractions[spi].opcode = 0;
    return drvFRAM_OK;
}
