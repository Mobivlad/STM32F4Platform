/*
 * ul_fat_fs.c
 *
 *  Created on: Jul 29, 2020
 *      Author: Sigma
 */

#include "ul_fat_fs.h"

static ulFatFS_struct* current_fatfs;

static volatile DSTATUS Stat = STA_NOINIT;

static DSTATUS SD_initialize(BYTE lun);
static DSTATUS SD_status(BYTE lun);
static DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count);
static DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count);
static DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff);

char SDPath[4];

static const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize, SD_status, SD_read,
#if  _USE_WRITE == 1
  SD_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  SD_ioctl,
#endif /* _USE_IOCTL == 1 */
};

void test() {
    FATFS FatFs;
    FIL fil; /* File object */
    FRESULT fr; /* FatFs return code */

    /* Gives a work area to the default drive */
    FRESULT res = f_mount(&FatFs, "", 0);
    if (res != FR_OK) {
        Error_Handler();
    }
    f_mkdir("hello");
    /* Open a text file */
    res = f_open(&fil, "message.txt", FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        Error_Handler();
    }
    /* Close the file */
    res = f_close(&fil);
    if (res != FR_OK) {
        Error_Handler();
    }
    res = f_mount(0, "", 0);
    if (res != FR_OK) {
        Error_Handler();
    }
}

void ulFatFSInit(ulFatFS_struct* fatfs) {
    current_fatfs = fatfs;
    fatfs->sd.sddriver = drvSDDriver1;
    FATFS_LinkDriver(&SD_Driver, SDPath);
}

static DSTATUS SD_CheckStatus(BYTE lun) {
    Stat = STA_NOINIT;

    if (drvSDGetStatus(&current_fatfs->sd) == drvSD_OK) {
        Stat &= ~STA_NOINIT;
    }

    return Stat;
}

static DSTATUS SD_initialize(BYTE lun) {
    Stat = STA_NOINIT;

#if !defined(DISABLE_SD_INIT)

    if (drvSDInit(&current_fatfs->sd) == drvSD_OK) {
        Stat = SD_CheckStatus(lun);
    }

#else
    Stat = SD_CheckStatus(lun);
#endif

    return Stat;
}

static DSTATUS SD_status(BYTE lun)
{
  return SD_CheckStatus(lun);
}

static DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count) {
    DRESULT res = RES_ERROR;

    if (drvSDReadBlocks(&current_fatfs->sd, buff, sector, count) == drvSD_OK) {
        /* wait until the read operation is finished */
        while (drvSDGetCardState(&current_fatfs->sd) != drvSD_TRANSFER_OK);
        res = RES_OK;
    }

    return res;
}

#if _USE_WRITE == 1

static DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count) {
    DRESULT res = RES_ERROR;

    if (drvSDWriteBlocks(&current_fatfs->sd, buff, sector, count) == drvSD_OK) {
        /* wait until the read operation is finished */
        while (drvSDGetCardState(&current_fatfs->sd) != drvSD_TRANSFER_OK) {
        }
        res = RES_OK;
    }

    return res;
}
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
static DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff) {
    DRESULT res = RES_ERROR;

    if (Stat & STA_NOINIT)
        return RES_NOTRDY;

    switch (cmd) {
        case CTRL_SYNC:
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT:
            drvSDGetSDInfo(&current_fatfs->sd);
            *(DWORD*) buff = current_fatfs->sd.SDIO.sdInfo.LogBlockNbr;
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            drvSDGetSDInfo(&current_fatfs->sd);
            *(WORD*) buff = current_fatfs->sd.SDIO.sdInfo.LogBlockSize;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE:
            drvSDGetSDInfo(&current_fatfs->sd);
            *(DWORD*) buff = current_fatfs->sd.SDIO.sdInfo.LogBlockSize / SD_BLOCK_SIZE;
            res = RES_OK;
            break;

        default:
            res = RES_PARERR;
    }

    return res;
}
#endif /* _USE_IOCTL == 1 */
