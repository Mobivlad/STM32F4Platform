/*
 * drc_sd_driver.h
 *
 *  Created on: Jul 27, 2020
 *      Author: Sigma
 */

#ifndef DRV_DRV_SD_DRIVER_H_
#define DRV_DRV_SD_DRIVER_H_

#include "hal_sdio.h"
#include "ff.h"
#include "diskio.h"

#define SD_OPERATION_TIMEOUT    ((uint32_t)100000000)

typedef enum {
    drvSDDriver1,
    drvSDDriver_Count
} drvSDDriver;

typedef struct {
    halSDIO                     sdio;
    halSDIO_wideWigth           wideWigth;
    halSDIO_PuPdType            ppType;
    halSDIO_clockTransitionEdge clockEdge;
    halSDIO_clockBypass         clockBypass;
    halSDIO_clockPowerSave      powerSave;
    halSDIO_hardFlowControl     flowControl;
} drvSDDriver_def_t;

typedef enum {
    drvSD_TRANSFER_OK   = halSDIO_TRANSFER_OK,
    drvSD_TRANSFER_BUSY = halSDIO_TRANSFER_BUSY
} drvSD_state;

typedef enum
{
  drvSD_OK           = halSDIO_OK,
  drvSD_ERROR        = halSDIO_ERROR,
  drvSD_BUSY         = halSDIO_BUSY,
  drvSD_TIMEOUT      = halSDIO_TIMEOUT,
  drvSD_NOT_PRESENT  = halSDIO_SD_NOT_PRESENT,
  drvSD_NULL_POINT   = halSDIO_NULL_POINT
} drvSD_error;

typedef struct {
    halSDIO_struct  SDIO;
    drvSDDriver     sddriver;
} drvSDDriver_struct;

drvSD_error drvSDInit(drvSDDriver_struct* sdStruct);

drvSD_state drvSDGetCardState(drvSDDriver_struct* sdStruct);
drvSD_error drvSDGetStatus(drvSDDriver_struct* sdStruct);
void drvSDGetSDInfo(drvSDDriver_struct* sdStruct);

drvSD_error drvSDWriteBlocks(drvSDDriver_struct* sdStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber);
drvSD_error drvSDReadBlocks(drvSDDriver_struct* sdStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber);

#endif /* DRV_DRV_SD_DRIVER_H_ */
