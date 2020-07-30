/*
 * ul_fat_fs.h
 *
 *  Created on: Jul 29, 2020
 *      Author: Sigma
 */

#ifndef UTILS_UL_FAT_FS_H_
#define UTILS_UL_FAT_FS_H_

#include "drv_sd_driver.h"
#include "ff_gen_drv.h"
#include "ffconf.h"

#if defined(SDMMC_DATATIMEOUT)
#define SD_TIMEOUT SDMMC_DATATIMEOUT
#elif defined(SD_DATATIMEOUT)
#define SD_TIMEOUT SD_DATATIMEOUT
#else
#define SD_TIMEOUT 30 * 1000
#endif
#define SD_BLOCK_SIZE 512

typedef struct {
    drvSDDriver_struct sd;
} ulFatFS_struct;

void ulFatFSInit(ulFatFS_struct* fatfs);

void test();



#endif /* UTILS_UL_FAT_FS_H_ */
