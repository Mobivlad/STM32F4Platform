/*
 * main.c
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

//include file of timer blink BL
#include "bl_timer_blink.h"

//include file of blink on button toggle BL
#include "bl_button_blink.h"

#include "drv_fm25l16b.h"

int main() {
    /*//initial functions
     blLedCounterInit();
     while(1){
     blLedCounterRun();
     }*/

    // SPI test code
    drvFRAMInit(drvFRAM_SPI1);
    uint8_t data[5] = { 1,2,3,4,5 };
    uint8_t receive[5] = { 0 };
    uint8_t sended = 0;
    while (1) {
        if (!sended) {
            drvSPIErrorCode code = drvFRAMSendArray(drvFRAM_SPI1, 0x0000, data, 5);
            if (code == drvFRAM_OK) {
                sended = 1;
            }
        } else {
            drvSPIErrorCode code = drvFRAMReceiveArray(drvFRAM_SPI1, 0x0000, receive, 5);
            if (code == drvFRAM_OK) {
                sended = 1;
            }
        }
    }
}
