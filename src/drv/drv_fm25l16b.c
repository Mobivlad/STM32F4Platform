/*
 * drv_fm25l16b.c
 *
 *  Created on: 10 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_fm25l16b.h"

static uint16_t sendCount[drvFRAMCount];
static uint16_t receiveCount[drvFRAMCount];

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

    sendCount[spi] = 0;
    receiveCount[spi] = 0;
}

static drvSPIErrorCode drvFRAMExecuteOperation(drvFRAM_SPI spi, drvFRAMOperationInstruction* instuction) {
    if (instuction == 0)
            return drvFRAM_DATA_NULL_POINTER;

    if (instuction->dest == 0 || instuction->source)
                return drvFRAM_DATA_NULL_POINTER;

    if(sendCount[spi]==0 && receiveCount[spi]==0){
        sendCount[spi] = instuction->sourceLen;
        receiveCount[spi] = instuction->destLen;
    }

    if (sendCount[spi] != 0) {
        halSPIErrorCode byteSendCodeStatus = halSPISendByte(spi,
                instuction->source[instuction->sourceLen - sendCount[drvFRAMCount]]);
        switch(byteSendCodeStatus){
            case halSPI_OK:
                sendCount[drvFRAMCount]--;
                break;
            default:
                return byteSendCodeStatus;
        }
    }

    if(sendCount[spi] != 0) {
        return drvFRAM_IN_PROGRESS;
    }

    if (receiveCount[spi] != 0) {
        drvSPIErrorCode byteReceiveResCode =
                halSPIReceiveByte(spi,
                        instuction->dest[instuction->destLen
                                - receiveCount[drvFRAMCount]]);
        switch (byteReceiveResCode) {
            case halSPI_OK:
                receiveCount[drvFRAMCount]--;
                break;
            default:
                return byteReceiveResCode;
        }
    }

    if(receiveCount[spi] != 0) {
        return drvFRAM_IN_PROGRESS;
    }

    return drvFRAM_OK;
}
