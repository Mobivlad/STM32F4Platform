/*
 * drc_sd_driver.c
 *
 *  Created on: Jul 27, 2020
 *      Author: Sigma
 */

#include "drv_sd_spi.h"

static sd_info_ptr sdinfo;

static uint8_t SD_cmd(void* voidPtrSDStruct, uint8_t cmd, uint32_t arg) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    uint8_t n, res;
    if (cmd & 0x80) {
        cmd &= 0x7F;
        res = SD_cmd(sdStruct, CMD55, 0);
        if (res > 1)
            return res;
    }
    uint8_t resData;
    // Select the card
    halSPISetCS(&sdStruct->SPI);
    halSPIReceive(&sdStruct->SPI, &resData, 1, SD_OPERATION_TIMEOUT);
    halSPIResetCS(&sdStruct->SPI);
    halSPIReceive(&sdStruct->SPI, &resData, 1, SD_OPERATION_TIMEOUT);
    // Send a command packet
    resData = cmd;
    halSPISend(&sdStruct->SPI, &resData, 1, SD_OPERATION_TIMEOUT);
    resData = (uint8_t) (arg >> 24);
    halSPISend(&sdStruct->SPI, &resData, 1, SD_OPERATION_TIMEOUT);
    resData = (uint8_t) (arg >> 16);
    halSPISend(&sdStruct->SPI, &resData, 1, SD_OPERATION_TIMEOUT);
    resData = (uint8_t) (arg >> 8);
    halSPISend(&sdStruct->SPI, &resData, 1, SD_OPERATION_TIMEOUT);
    resData = (uint8_t) arg;
    halSPISend(&sdStruct->SPI, &resData, 1, SD_OPERATION_TIMEOUT);
    n = 0x01;
    if (cmd == CMD0) {
        n = 0x95;
    }
    if (cmd == CMD8) {
        n = 0x87;
    }
    halSPISend(&sdStruct->SPI, &resData, 1, SD_OPERATION_TIMEOUT);
    n = 10;
    do {
        halSPIReceive(&sdStruct->SPI, &res, 1, SD_OPERATION_TIMEOUT);
    } while ((res & 0x80) && --n);
    return res;
}

static void SD_PowerOn(void)
{
    // @TODO Implement this function before use
    // Wait 20 ms
}

static void SPI_Release(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    uint8_t data = 0xFF;
    halSPISend(&sdStruct->SPI, &data, 1, SD_OPERATION_TIMEOUT);
}

static drvSD_error spiInit(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    halSPI_initStruct init;
    sdStruct->SPI.spi = sd_def.spi.spi;
    init.mode                 = sd_def.spi.mode;
    init.dataSize             = sd_def.spi.dataSize;
    init.direction            = sd_def.spi.direction;
    init.frequencyPrescaller  = sd_def.spi.prescaller;
    init.firstBit             = sd_def.spi.firstBit;
    init.typeNSS              = sd_def.spi.NSSType;
    return halSPIInit(&sdStruct->SPI, &init);
}

drvSD_error drvSDSPIInit(void* voidPtrSDStruct)
{
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    SD_PowerOn();
    uint8_t i, cmd;
    int16_t tmr;
    uint32_t temp;
    sdinfo.type = 0;
    uint8_t ocr[4];
    temp = sd_def.spi.prescaller;
    sd_def.spi.prescaller = SPI_BAUDRATEPRESCALER_128;
    spiInit(sdStruct);
    halSPISetCS(&sdStruct->SPI);
    for (i = 0; i < 10; i++)
        SPI_Release(sdStruct);
    sd_def.spi.prescaller = temp;
    spiInit(sdStruct);
    halSPISetCS(&sdStruct->SPI);
    if (SD_cmd(sdStruct, CMD0, 0) == 1)
            {
        SPI_Release(sdStruct);
        if (SD_cmd(sdStruct, CMD8, 0x1AA) == 1)
                {
            halSPIReceive(&sdStruct->SPI, ocr, 4, SD_OPERATION_TIMEOUT);
            if (ocr[2] == 0x01 && ocr[3] == 0xAA)
                    {
                for (tmr = 12000; tmr && SD_cmd(sdStruct, ACMD41, 1UL << 30); tmr--);
                if (tmr && SD_cmd(sdStruct, CMD58, 0) == 0) {
                    halSPIReceive(&sdStruct->SPI, ocr, 4, SD_OPERATION_TIMEOUT);
                    sdinfo.type = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;
                }
            }
        } else
        {
            if (SD_cmd(sdStruct, ACMD41, 0) <= 1) {
                sdinfo.type = CT_SD1;
                cmd = ACMD41; // SDv1
            } else {
                sdinfo.type = CT_MMC;
                cmd = CMD1; // MMCv3
            }
            for (tmr = 25000; tmr && SD_cmd(sdStruct, cmd, 0); tmr--); // Wait for leaving idle state
            if (!tmr || SD_cmd(sdStruct, CMD16, 512) != 0) // Set R/W block length to 512
                sdinfo.type = 0;
        }
    } else {
        return 1;
    }
    return 0;
}

drvSD_error drvSDSPIGetStatus(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    // @TODO Implement this function before use
    return 0;
}

drvSD_state drvSDSPIGetCardState(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    // @TODO Implement this function before use
    return 0;
}

void drvSDSPIGetSDInfo(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    // @TODO Implement this function before use
    return;
}

drvSD_state drvSDSPIGetSectorCount(void* voidPtrSDStruct, uint32_t* buff) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    uint8_t n, csd[16];
    DWORD csize;
    if ((SD_cmd(sdStruct, CMD9, 0) == 0) && halSPIReceive(&sdStruct->SPI, csd, 16, SD_OPERATION_TIMEOUT)) {
        if ((csd[0] >> 6) == 1) {
            /* SDC V2 */
            csize = csd[9] + ((WORD) csd[8] << 8) + 1;
            *(DWORD*) buff = (DWORD) csize << 10;
        } else {
            /* MMC or SDC V1 */
            n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
            csize = (csd[8] >> 6) + ((WORD) csd[7] << 2) + ((WORD) (csd[6] & 3) << 10) + 1;
            *(DWORD*) buff = (DWORD) csize << (n - 9);
        }
        return RES_OK;
    }
    return RES_OK;
}

drvSD_state drvSDSPIGetSectorSize(void* voidPtrSDStruct, uint32_t* buff) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    *(WORD*)buff = 512;
    return RES_OK;
}

static uint8_t SPI_wait_ready(void* voidPtrSDStruct) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    uint8_t res;
    uint16_t cnt;
    cnt = 0;
    do {
        halSPIReceive(&sdStruct->SPI, &res, 1, SD_OPERATION_TIMEOUT);
        cnt++;
    } while ((res != 0xFF) && (cnt < 0xFFFF));
    if (cnt >= 0xFFFF)
        return 1;
    return res;
}

drvSD_state drvSDSPISynh(void* voidPtrSDStruct, uint8_t* buff) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    halSPIResetCS(&sdStruct->SPI);
    if (SPI_wait_ready(sdStruct) == 0xFF)
        return RES_OK;
    return RES_PARERR;
}

drvSD_error drvSDSPIWriteBlocks(void* voidPtrSDStruct, const uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    uint8_t result;
    uint16_t cnt;
    result = SD_cmd(sdStruct, CMD24, blockAdd);
    if (result != 0x00)
        return 6;
    SPI_Release(sdStruct);
    uint8_t data = 0xFE;
    halSPISend(&sdStruct->SPI, &data, 1, SD_OPERATION_TIMEOUT);
    halSPISend(&sdStruct->SPI, (uint8_t*) pData, blocksNumber, SD_OPERATION_TIMEOUT);
    SPI_Release(sdStruct);
    SPI_Release(sdStruct);
    halSPIReceive(&sdStruct->SPI, &result, 1, SD_OPERATION_TIMEOUT);
    if ((result & 0x05) != 0x05)
        return 6;
    cnt = 0;
    do {
        halSPIReceive(&sdStruct->SPI, &result, 1, SD_OPERATION_TIMEOUT);
        cnt++;
    } while ((result != 0xFF) && (cnt < 0xFFFF));
    if (cnt >= 0xFFFF)
        return 6;
    return 0;
}

drvSD_error drvSDSPIReadBlocks(void* voidPtrSDStruct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber) {
    drvSDDriver_struct* sdStruct = (drvSDDriver_struct*) voidPtrSDStruct;
    uint8_t result;
    uint16_t cnt;
    result = SD_cmd(sdStruct, CMD17, blockAdd);
    if (result != 0x00)
        return 5;
    SPI_Release(sdStruct);
    cnt = 0;
    do {
        halSPIReceive(&sdStruct->SPI, &result, 1, SD_OPERATION_TIMEOUT);
        cnt++;
    } while ((result != 0xFE) && (cnt < 0xFFFF));
    if (cnt >= 0xFFFF)
        return 5;
    halSPIReceive(&sdStruct->SPI, pData, blocksNumber, SD_OPERATION_TIMEOUT);
    SPI_Release(sdStruct);
    SPI_Release(sdStruct);
    return 0;
}

