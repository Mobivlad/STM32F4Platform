/*
 * ul_fat_fs.h
 *
 *  Created on: Jul 29, 2020
 *      Author: Sigma
 */

#ifndef UTILS_UL_FAT_FS_H_
#define UTILS_UL_FAT_FS_H_

#include <drv_sd_sdio.h>
#include <string.h>
#include "ff_gen_drv.h"
#include "ffconf.h"

#if defined(SDMMC_DATATIMEOUT)
    #define SD_TIMEOUT SDMMC_DATATIMEOUT
#elif defined(SD_DATATIMEOUT)
    #define SD_TIMEOUT SD_DATATIMEOUT
#else
    #define SD_TIMEOUT 30 * 1000
#endif

#define SD_BLOCK_SIZE            512
#define SD_ERROR_VALUE           -1

#define ulFatFS_File_t           FIL
#define ulFatFS_Directory_t      DIR

typedef struct {
    drvSDDriver_struct sd;
    FATFS FatFs;
} ulFatFS_struct;

typedef enum {
    ulFatFS_OK                   = FR_OK,
    ulFatFS_DISK_ERR             = FR_DISK_ERR,
    ulFatFS_INT_ERR              = FR_INT_ERR,
    ulFatFS_NOT_READY            = FR_NOT_READY,
    ulFatFS_NO_FILE              = FR_NO_FILE,
    ulFatFS_NO_PATH              = FR_NO_PATH,
    ulFatFS_INVALID_NAME         = FR_INVALID_NAME,
    ulFatFS_DENIED               = FR_DENIED,
    ulFatFS_EXIST                = FR_EXIST,
    ulFatFS_INVALID_OBJECT       = FR_INVALID_OBJECT,
    ulFatFS_WRITE_PROTECTED      = FR_WRITE_PROTECTED,
    ulFatFS_INVALID_DRIVE        = FR_INVALID_DRIVE,
    ulFatFS_NOT_ENABLED          = FR_NOT_ENABLED,
    ulFatFS_NO_FILESYSTEM        = FR_NO_FILESYSTEM,
    ulFatFS_TIMEOUT              = FR_TIMEOUT,
    ulFatFS_LOCKED               = FR_LOCKED,
    ulFatFS_NOT_ENOUGH_CORE      = FR_NOT_ENOUGH_CORE,
    ulFatFS_TOO_MANY_OPEN_FILES  = FR_TOO_MANY_OPEN_FILES,
    ulFatFS_INVALID_PARAMETER    = FR_INVALID_PARAMETER,
    ulFatFS_DATA_NULL_POINT,
    ulFatFS_COMPLETE_OPERATION_ERROR
} ulFatFS_error;

typedef enum {
    R   = FA_READ,
    RW  = R | FA_WRITE,
    CW  = FA_CREATE_ALWAYS | FA_WRITE,
    CWR = CW | R,
    A   = FA_OPEN_APPEND | FA_READ,
    AR  = A  | FA_READ,
    OW  = FA_CREATE_NEW | FA_WRITE,
    OWR = OW | R
} ulFatFS_filePosix;

ulFatFS_error ulFatFSInit(ulFatFS_struct* fatfs);

void test();



#endif /* UTILS_UL_FAT_FS_H_ */
