/*
 * drc_sd_driver.c
 *
 *  Created on: Jul 27, 2020
 *      Author: Sigma
 */

#include "drv_sd_sdio.h"

drvSD_error drvSDSDIOInit(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    halSDIO_initStruct init;
    sdStruct->SDIO.sdio = sd_def.sdio.sdio;
    init.wideWigth      = sd_def.sdio.wideWigth;
    init.ppType         = sd_def.sdio.ppType;
    init.clockEdge      = sd_def.sdio.clockEdge;
    init.clockBypass    = sd_def.sdio.clockBypass;
    init.powerSave      = sd_def.sdio.powerSave;
    init.flowControl    = sd_def.sdio.flowControl;
    init.clockDiv       = 0;
    return halSDIOInit(&(sdStruct->SDIO), &init);
}

drvSD_error drvSDSDIOGetStatus(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    return halSDIOGetStatus(&(sdStruct->SDIO));
}

drvSD_state drvSDSDIOGetCardState(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    return halSDIOGetCardState(&(sdStruct->SDIO));
}

void drvSDSDIOGetSDInfo(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    halSDIOGetCardInfo(&(sdStruct->SDIO));
}

drvSD_state drvSDSDIOGetSectorCount(void* voidPtrSDStruct, uint32_t* buff) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    drvSDSDIOGetSDInfo(sdStruct);
    *(DWORD*) buff = sdStruct->SDIO.sdInfo.LogBlockNbr;
    return RES_OK;
}

drvSD_state drvSDSDIOGetSectorSize(void* voidPtrSDStruct, uint32_t* buff) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    drvSDSDIOGetSDInfo(sdStruct);
    *(DWORD*) buff = sdStruct->SDIO.sdInfo.LogBlockSize;
    return RES_OK;
}

drvSD_state drvSDSDIOSynh(void* voidPtrSDStruct, uint8_t* buff) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    return RES_OK;
}

drvSD_error drvSDSDIOWriteBlocks(void* voidPtrSDStruct, const uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    return halSDIOWriteBlocks(&(sdStruct->SDIO), pData, blockAdd, blocksNumber, SD_OPERATION_TIMEOUT);
}

drvSD_error drvSDSDIOReadBlocks(void* voidPtrSDStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    return halSDIOReadBlocks(&(sdStruct->SDIO), pData, blockAdd, blocksNumber, SD_OPERATION_TIMEOUT);
}

