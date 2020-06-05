/*
 * drv_fm25l16b.h
 *
 *  Created on: 5 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_FM25L16B_H_
#define DRV_DRV_FM25L16B_H_

/// SPI enumeration
typedef enum {
    drvFRAM_SPI1 = 0,
    drvFRAM_SPI2,
    drvFRAM_SPI3
} drvFRAM_SPI;


/**
 * Initial function for FRAM
 *
 */
void drvFRAMInit();
#endif /* DRV_DRV_FM25L16B_H_ */
