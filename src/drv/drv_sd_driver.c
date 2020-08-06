/*
 * drc_sd_driver.c
 *
 *  Created on: Jul 27, 2020
 *      Author: Sigma
 */

#include "drv_sd_driver.h"

static const drvSDDriver_def_t sd_def = {
    halSDIO2,
    halSDIO_4Bits,
    halSDIO_Soft,
    halSDIO_Rising,
    halSDIO_clockBypassDisable,
    halSDIO_clockPowerSaveDisable,
    halSDIO_hardFlowControlEnable
};

drvSD_error drvSDInit(drvSDDriver_struct* sdStruct) {
    if (sdStruct == NULL) {
        return drvSD_NULL_POINT;
    }
    halSDIO_initStruct init;
    sdStruct->SDIO.sdio = sd_def.sdio;
    init.wideWigth      = sd_def.wideWigth;
    init.ppType         = sd_def.ppType;
    init.clockEdge      = sd_def.clockEdge;
    init.clockBypass    = sd_def.clockBypass;
    init.powerSave      = sd_def.powerSave;
    init.flowControl    = sd_def.flowControl;
    init.clockDiv       = 0;
    return halSDIOInit((halSDIO_struct*)sdStruct, &init);
}

drvSD_error drvSDGetStatus(drvSDDriver_struct* sdStruct) {
    if (sdStruct == NULL) {
        return drvSD_NULL_POINT;
    }
    return halSDIOGetStatus((halSDIO_struct*)sdStruct);
}

drvSD_state drvSDGetCardState(drvSDDriver_struct* sdStruct) {
    if (sdStruct == NULL) {
        return drvSD_NULL_POINT;
    }
    return halSDIOGetCardState((halSDIO_struct*)sdStruct);
}

void drvSDGetSDInfo(drvSDDriver_struct* sdStruct) {
    if (sdStruct == NULL) {
        return;
    }
    halSDIOGetCardInfo((halSDIO_struct*)sdStruct);
}

drvSD_error drvSDWriteBlocks(drvSDDriver_struct* sdStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber) {
    if (sdStruct == NULL || pData == NULL) {
        return drvSD_NULL_POINT;
    }
    return halSDIOWriteBlocks((halSDIO_struct*)sdStruct, pData, blockAdd, blocksNumber, SD_OPERATION_TIMEOUT);
}

drvSD_error drvSDReadBlocks(drvSDDriver_struct* sdStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber) {
    if (sdStruct == NULL || pData == NULL) {
        return drvSD_NULL_POINT;
    }
    return halSDIOReadBlocks((halSDIO_struct*)sdStruct, pData, blockAdd, blocksNumber, SD_OPERATION_TIMEOUT);
}

