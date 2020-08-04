/*
 * drv_sd_driver.h
 *
 *  Created on: Aug 3, 2020
 *      Author: Sigma
 */

#ifndef DRV_DRV_SD_DRIVER_H_
#define DRV_DRV_SD_DRIVER_H_

#include "ff.h"
#include "diskio.h"

#include "hal_sdio.h"
#include "hal_spi.h"

#define SD_OPERATION_TIMEOUT    ((uint32_t)0xFFFF)

typedef enum {
    drvSDDriver1,
    drvSDDriver_Count
} drvSDDriver;

typedef enum {
    drvSD_SDIO,
    drvSD_SPI,
    drvSD_CommInterfaceCount
} drvSD_commInterface;

typedef struct {
    halSPI                     spi;
    halSPI_dataSize            dataSize;
    halSPI_mode                mode;
    halSPI_direction           direction;
    halSPI_frequencyPrescaller prescaller;
    halSPI_firstBit            firstBit;
    halSPI_NSSType             NSSType;
} drvSDSPI_def_t;

typedef struct {
    halSDIO                     sdio;
    halSDIO_wideWigth           wideWigth;
    halSDIO_PuPdType            ppType;
    halSDIO_clockTransitionEdge clockEdge;
    halSDIO_clockBypass         clockBypass;
    halSDIO_clockPowerSave      powerSave;
    halSDIO_hardFlowControl     flowControl;
} drvSDSDIO_def_t;

typedef enum
{
  drvSD_OK           = halSDIO_OK,
  drvSD_ERROR        = halSDIO_ERROR,
  drvSD_BUSY         = halSDIO_BUSY,
  drvSD_TIMEOUT      = halSDIO_TIMEOUT,
  drvSD_NOT_PRESENT  = halSDIO_SD_NOT_PRESENT
} drvSD_error;

typedef enum {
    drvSD_TRANSFER_OK   = halSDIO_TRANSFER_OK,
    drvSD_TRANSFER_BUSY = halSDIO_TRANSFER_BUSY
} drvSD_state;

typedef struct {
    drvSDSDIO_def_t sdio;
    drvSDSPI_def_t  spi;
} drvSDDriver_def_t;

extern drvSDDriver_def_t sd_def;

typedef struct {
    drvSD_error (*SD_GetStatus)        (void*);
    drvSD_state (*SD_GetCardState)     (void*);
    void        (*SD_GetSDInfo)        (void*);
    drvSD_state (*SD_GetSectorCount)   (void*, uint32_t*);
    drvSD_state (*SD_GetSectorSize)    (void*, uint32_t*);
    drvSD_state (*SD_Synh)             (void*, uint8_t*);
    drvSD_error (*SD_WriteBlocks)      (void*, const uint8_t *, uint32_t, uint32_t);
    drvSD_error (*SD_ReadBlocks)       (void*, uint8_t *, uint32_t, uint32_t);
} drvSDDriver_handler_t;

typedef struct {
    halSDIO_struct          SDIO;
    halSPI_struct           SPI;
    drvSDDriver             sddriver;
    drvSDDriver_handler_t   handler;
} drvSDDriver_struct;

drvSD_error drvSDInit(drvSDDriver_struct* sdStruct);

drvSD_error drvSD_SetCommInterface(drvSDDriver_struct* sdStruct, drvSD_commInterface);

drvSD_error drvSDGetStatus(drvSDDriver_struct* sdStruct);
drvSD_state drvSDGetCardState(drvSDDriver_struct* sdStruct);
void drvSDGetInfo(drvSDDriver_struct* sdStruct);
drvSD_state drvSDGetSectorCount(drvSDDriver_struct* sdStruct, uint32_t* buff);
drvSD_state drvSDGetSectorSize(drvSDDriver_struct* sdStruct, uint32_t* buff);
drvSD_state drvSDSynh(drvSDDriver_struct* sdStruct, uint8_t* buff);

drvSD_error drvSDWriteBlocks(drvSDDriver_struct* sdStruct, const uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber);
drvSD_error drvSDReadBlocks(drvSDDriver_struct* sdStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber);


#endif /* DRV_DRV_SD_DRIVER_H_ */
