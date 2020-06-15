/*
 * main.c
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_fm25l16b.h"

int main() {
    // SPI test code
    drvFRAMInit(drvFRAM_SPI1);
    uint8_t data[5] = { 1, 2, 3, 4, 5 };
    uint8_t receive[5] = { 0 };
    uint8_t sended = 0;
    while (1) {
        if (!sended) {
            drvSPIErrorCode code = drvFRAMWriteArray(drvFRAM_SPI1, 0x0000, data,
                    5);
            if (code == drvFRAM_OK) {
                sended = 1;
            }
        } else {
            drvSPIErrorCode code = drvFRAMReadArray(drvFRAM_SPI1, 0x0000,
                    receive, 5);
            if (code == drvFRAM_OK) {
                sended = 1;
            }
        }
    }
}
