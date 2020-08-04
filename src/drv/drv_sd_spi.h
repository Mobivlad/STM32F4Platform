/*
 * drc_sd_driver.h
 *
 *  Created on: Jul 27, 2020
 *      Author: Sigma
 */

#ifndef DRV_DRV_SD_SPI_H_
#define DRV_DRV_SD_SPI_H_

#include "hal_spi.h"
#include "drv_sd_driver.h"

#define CMD0 (0x40+0) // GO_IDLE_STATE
#define CMD1 (0x40+1) // SEND_OP_COND (MMC)
#define ACMD41 (0xC0+41) // SEND_OP_COND (SDC)
#define CMD8 (0x40+8) // SEND_IF_COND
#define CMD9 (0x40+9) // SEND_CSD
#define CMD16 (0x40+16) // SET_BLOCKLEN
#define CMD17 (0x40+17) // READ_SINGLE_BLOCK
#define CMD24 (0x40+24) // WRITE_BLOCK
#define CMD55 (0x40+55) // APP_CMD
#define CMD58 (0x40+58) // READ_OCR

#define CT_MMC 0x01 /* MMC ver 3 */
#define CT_SD1 0x02 /* SD ver 1 */
#define CT_SD2 0x04 /* SD ver 2 */
#define CT_SDC (CT_SD1|CT_SD2) /* SD */
#define CT_BLOCK 0x08

typedef struct sd_info {
  volatile uint8_t type;
} sd_info_ptr;

drvSD_error drvSDSPIInit(void* voidPtrSDStruct);

drvSD_error drvSDSPIGetStatus(void* voidPtrSDStruct);
drvSD_state drvSDSPIGetCardState(void* voidPtrSDStruct);

void        drvSDSPIGetSDInfo(void* voidPtrSDStruct);
drvSD_state drvSDSPIGetSectorCount(void* voidPtrSDStruct, uint32_t* buff);
drvSD_state drvSDSPIGetSectorSize(void* voidPtrSDStruct, uint32_t* buff);
drvSD_state drvSDSPISynh(void* voidPtrSDStruct, uint8_t* buff);

drvSD_error drvSDSPIWriteBlocks(void* voidPtrSDStruct, const uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber);
drvSD_error drvSDSPIReadBlocks(void* voidPtrSDStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber);

#endif /* DRV_DRV_SD_SPI_H_ */
