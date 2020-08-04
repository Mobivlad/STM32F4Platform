/*
 * drv_sd_driver.c
 *
 *  Created on: Aug 3, 2020
 *      Author: Sigma
 */

#include "drv_sd_driver.h"

#include "drv_sd_sdio.h"
#include "drv_sd_spi.h"

drvSDDriver_def_t sd_def = {
        // SDIO PART
        {
        halSDIO2,
        halSDIO_4Bits,
        halSDIO_Soft,
        halSDIO_Rising,
        halSDIO_clockBypassDisable,
        halSDIO_clockPowerSaveDisable,
        halSDIO_hardFlowControlEnable
        },
        // SPI PART
        {
        halSPI2,
        halSPI_DataSize8b,
        halSPI_Mode0,
        halSPI_FullDirection2Lines,
        halSPI_FrequencyPrescaller2,
        halSPI_FirstBitMSB,
        halSPI_NSSTypeSoft
        }
};

drvSDDriver_handler_t sd_haldlers[drvSD_CommInterfaceCount] = {
        {
                drvSDSDIOGetStatus,
                drvSDSDIOGetCardState,
                drvSDSDIOGetSDInfo,
                drvSDSDIOGetSectorCount,
                drvSDSDIOGetSectorSize,
                drvSDSDIOSynh,
                drvSDSDIOWriteBlocks,
                drvSDSDIOReadBlocks
        },
        {
                drvSDSPIGetStatus,
                drvSDSPIGetCardState,
                drvSDSPIGetSDInfo,
                drvSDSPIGetSectorCount,
                drvSDSPIGetSectorSize,
                drvSDSPISynh,
                drvSDSPIWriteBlocks,
                drvSDSPIReadBlocks
        }
};

drvSD_error drvSDInit(drvSDDriver_struct* sdStruct) {
    if (sdStruct == NULL) {
        return drvSD_ERROR;
    }
    sdStruct->handler = sd_haldlers[drvSD_SDIO];
    drvSD_error res = drvSDSDIOInit((void*)sdStruct);
    if (res == drvSD_OK) {
        //res = drvSDSPIInit((void*)sdStruct);
    }
    return res;
}

drvSD_error drvSD_SetCommInterface(drvSDDriver_struct* sdStruct, drvSD_commInterface interface) {
    if (sdStruct == NULL) {
        return drvSD_ERROR;
    }
    sdStruct->handler = sd_haldlers[interface];
    return drvSD_OK;
}

drvSD_error drvSDGetStatus(drvSDDriver_struct* sdStruct) {
    return sdStruct->handler.SD_GetStatus((void*)sdStruct);
}

drvSD_state drvSDGetCardState(drvSDDriver_struct* sdStruct) {
    return sdStruct->handler.SD_GetCardState((void*)sdStruct);
}

void drvSDGetInfo(drvSDDriver_struct* sdStruct) {
    sdStruct->handler.SD_GetSDInfo((void*)sdStruct);
}

drvSD_state drvSDGetSectorCount(drvSDDriver_struct* sdStruct, uint32_t* buff) {
    return sdStruct->handler.SD_GetSectorCount((void*)sdStruct, buff);
}

drvSD_state drvSDGetSectorSize(drvSDDriver_struct* sdStruct, uint32_t* buff) {
    return sdStruct->handler.SD_GetSectorSize((void*)sdStruct, buff);
}

drvSD_state drvSDSynh(drvSDDriver_struct* sdStruct, uint8_t* buff) {
    return sdStruct->handler.SD_Synh((void*)sdStruct, buff);
}

drvSD_error drvSDWriteBlocks(drvSDDriver_struct* sdStruct, const uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber) {
    return sdStruct->handler.SD_WriteBlocks((void*)sdStruct, pData, blockAdd, blocksNumber);
}

drvSD_error drvSDReadBlocks(drvSDDriver_struct* sdStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber) {
    return sdStruct->handler.SD_ReadBlocks((void*)sdStruct, pData, blockAdd, blocksNumber);
}
