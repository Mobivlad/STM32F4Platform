/*
 * drc_sd_driver.h
 *
 *  Created on: Jul 27, 2020
 *      Author: Sigma
 */

#ifndef DRV_DRV_SD_SDIO_H_
#define DRV_DRV_SD_SDIO_H_

#include "hal_sdio.h"
#include "drv_sd_driver.h"

drvSD_error drvSDSDIOInit(void* voidPtrSDStruct);

drvSD_error drvSDSDIOGetStatus(void* voidPtrSDStruct);
drvSD_state drvSDSDIOGetCardState(void* voidPtrSDStruct);

void        drvSDSDIOGetSDInfo(void* voidPtrSDStruct);
drvSD_state drvSDSDIOGetSectorCount(void* voidPtrSDStruct, uint32_t* buff);
drvSD_state drvSDSDIOGetSectorSize(void* voidPtrSDStruct, uint32_t* buff);
drvSD_state drvSDSDIOSynh(void* voidPtrSDStruct, uint8_t* buff);

drvSD_error drvSDSDIOWriteBlocks(void* voidPtrSDStruct, const uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber);
drvSD_error drvSDSDIOReadBlocks(void* voidPtrSDStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber);

#endif /* DRV_DRV_SD_SDIO_H_ */
