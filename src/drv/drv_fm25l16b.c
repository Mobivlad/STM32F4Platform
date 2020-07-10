#include "drv_fm25l16b.h"

static const fram_data_struct FRAM_data =
{
        .spi = halSPI1,
        .prescaller = halSPI_FrequencyPrescaller8
};

static const uint8_t drvFRAM_opCodeValue[opCodes_Count] = { 0x06, 0x04, 0x05, 0x01, 0x03, 0x02 };

static drvFRAM_errorCode readBlockProtection(drvFRAM_struct* framStruct) {
    halSPISetCS((halSPI_struct*) framStruct);

    // send command "Read Status Register"
    uint8_t operationResultCode = halSPISend((halSPI_struct*) framStruct, &drvFRAM_opCodeValue[RDSR], 1,
            MAX_TIMEOUT);

    if (operationResultCode != halSPI_OK) {
        // error
        halSPIResetCS((halSPI_struct*) framStruct);
        return operationResultCode;
    }
    uint8_t status;
    // get SR value
    operationResultCode = halSPIReceive((halSPI_struct*) framStruct, &status, 1,
            MAX_TIMEOUT);

    if (operationResultCode != halSPI_OK) {
        // error
        halSPIResetCS((halSPI_struct*) framStruct);
        return operationResultCode;
    }

    // read block protection level from status register
    framStruct->protectionLevel = GET_BP_FROM_SR(status);
    halSPIResetCS((halSPI_struct*) framStruct);

    return drvFRAM_OK;
}

drvFRAM_errorCode drvFRAMInit(drvFRAM_struct* framStruct) {

    framStruct->state = STATE_IN_INIT;

    halSPI_initStruct SPI_init_struct =
    {
            halSPI_Mode0,
            halSPI_DataSize8b,
            halSPI_FullDirection2Lines,
            FRAM_data.prescaller,
            halSPI_FirstBitMSB,
            halSPI_NSSTypeSoft
    };

    halSPI_struct* spiStruct = (halSPI_struct*) framStruct;

    spiStruct->spi = FRAM_data.spi;

    halSPI_errorCode spiInitCode = halSPIInit(spiStruct, &SPI_init_struct);

    if (spiInitCode != halSPI_OK) {
        return spiInitCode;
    }

    if (readBlockProtection(framStruct) != drvFRAM_OK) {
        framStruct->state = STATE_NOT_INITED;
        return drvFRAM_INIT_ERROR;
    }

    framStruct->state = (framStruct->protectionLevel == drvBP3) ? STATE_PROTECTED : STATE_READY;

    return drvFRAM_OK;
}

drvFRAM_errorCode drvFRAMSetBP(drvFRAM_struct* framStruct, drvFRAM_protectionLevel newBP) {
    drvFRAM_state startState = framStruct->state;

    if (!(framStruct->state == STATE_READY || STATE_READY == STATE_PROTECTED)) {
        return drvFRAM_BUSY;
    }

    framStruct->state = STATE_BP_CHANGING;

    halSPISetCS((halSPI_struct*) framStruct);

    uint8_t operationResultCode = halSPISend((halSPI_struct*) framStruct, &drvFRAM_opCodeValue[WREN], 1,
            MAX_TIMEOUT);
    if (operationResultCode != halSPI_OK) {
        halSPIResetCS((halSPI_struct*) framStruct);
        framStruct->state = startState;
        return operationResultCode;
    }

    halSPIResetCS((halSPI_struct*) framStruct);
    halSPISetCS((halSPI_struct*) framStruct);

    operationResultCode = halSPISend((halSPI_struct*) framStruct, &drvFRAM_opCodeValue[WRSR], 1,
            MAX_TIMEOUT);
    if (operationResultCode != halSPI_OK) {
        halSPIResetCS((halSPI_struct*) framStruct);
        framStruct->state = startState;
        return operationResultCode;
    }

    uint8_t operationData = GET_SR_FROM_BP(newBP);
    operationResultCode = halSPISend((halSPI_struct*) framStruct, &operationData, 1,
            MAX_TIMEOUT);
    if (operationResultCode != halSPI_OK) {
        halSPIResetCS((halSPI_struct*) framStruct);
        framStruct->state = startState;
        return operationResultCode;
    }

    halSPISetCS((halSPI_struct*) framStruct);

    framStruct->protectionLevel = newBP;

    framStruct->state = (newBP == drvBP3) ? STATE_PROTECTED : STATE_READY;

    return drvFRAM_OK;
}

drvFRAM_errorCode drvFRAMWriteData(drvFRAM_struct* framStruct, uint16_t memoryAddress,
        uint8_t* data, uint16_t dataLen) {
    if (framStruct->state != STATE_READY) {
        return drvFRAM_BUSY;
    }

    drvFRAM_operationInstruction* operation = &framStruct->currentOperation;
    operation->opcode = WRITE;
    operation->address[0] = FIRST_ADDRESS_BYTE(memoryAddress);
    operation->address[1] = SECOND_ADDRESS_BYTE(memoryAddress);
    operation->data = data;
    operation->dataLen = dataLen;
    operation->step = STEP_WREN;
    framStruct->state = STATE_WAIT_TO_RUN;

    return drvFRAM_OK;
}

drvFRAM_errorCode drvFRAMReadData(drvFRAM_struct* framStruct, uint16_t memoryAddress, uint8_t* data,
        uint16_t dataLen) {
    if (!(framStruct->state == STATE_READY || framStruct->state == STATE_PROTECTED)) {
        return drvFRAM_BUSY;
    }

    drvFRAM_operationInstruction* operation = &framStruct->currentOperation;
    operation->opcode = READ;
    operation->address[0] = FIRST_ADDRESS_BYTE(memoryAddress);
    operation->address[1] = SECOND_ADDRESS_BYTE(memoryAddress);
    operation->data = data;
    operation->dataLen = dataLen;
    operation->step = STEP_OPERATION;
    framStruct->state = STATE_WAIT_TO_RUN;

    return drvFRAM_OK;
}

drvFRAM_errorCode drvFRAMRun(drvFRAM_struct* framStruct) {

    if (framStruct == NULL) {
        return drvFRAM_DATA_NULL_POINTER;
    }

    switch (framStruct->state) {
        case STATE_IN_INIT:
        case STATE_NOT_INITED: {
            return drvFRAM_NOT_CONFIG;
        }

        case STATE_WAIT_TO_RUN: {
            switch (framStruct->currentOperation.step) {
                case STEP_WREN:
                    halSPISetCS((halSPI_struct*) framStruct);
                    halSPISendIT((halSPI_struct*) framStruct, &drvFRAM_opCodeValue[WREN], 1, NULL);

                    framStruct->state = STATE_RUN;
                    framStruct->currentOperation.step = STEP_OPERATION;
                    break;

                case STEP_OPERATION:
                    // new CS clock after WREN
                    halSPIResetCS((halSPI_struct*) framStruct);
                    halSPISetCS((halSPI_struct*) framStruct);

                    halSPISendIT((halSPI_struct*) framStruct,
                            &drvFRAM_opCodeValue[framStruct->currentOperation.opcode], 1, NULL);

                    framStruct->state = STATE_RUN;
                    framStruct->currentOperation.step = STEP_ADDRESS;
                    break;

                case STEP_ADDRESS:
                    halSPISendIT((halSPI_struct*) framStruct, framStruct->currentOperation.address,
                            ADDRESS_BYTE_COUNT, NULL);

                    framStruct->state = STATE_RUN;
                    framStruct->currentOperation.opcode == WRITE ?
                            (framStruct->currentOperation.step = STEP_WRITE) :
                            (framStruct->currentOperation.step = STEP_READ);
                    break;

                case STEP_WRITE:
                case STEP_READ:
                    framStruct->currentOperation.step == STEP_WRITE ?
                            halSPISendIT((halSPI_struct*) framStruct,
                                    framStruct->currentOperation.data,
                                    framStruct->currentOperation.dataLen, NULL) :
                            halSPIReceiveIT((halSPI_struct*) framStruct,
                                    framStruct->currentOperation.data,
                                    framStruct->currentOperation.dataLen, NULL);

                    framStruct->currentOperation.step = STEP_OPERATION_END;
                    framStruct->state = STATE_RUN;
                    break;

                case STEP_OPERATION_END:
                    halSPIResetCS((halSPI_struct*) framStruct);

                    framStruct->state = STATE_READY;
                    break;

                default:
                    break;
            }
            break;
        }

        case STATE_RUN: {
            if (halSPIIsReady((halSPI_struct*) framStruct) != halSPI_BUSY) {
                framStruct->state = STATE_WAIT_TO_RUN;
            }
            break;
        }

        default: {
            break;
        }
    }
    return drvFRAM_OK;
}
