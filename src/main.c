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

int main(){
    /*//initial functions
    blLedCounterInit();
    while(1){
        blLedCounterRun();
    }*/
    drvFRAMInit(drvFRAM_SPI1);
    uint8_t status;
    uint16_t x = 0;
    while(1){
        drvSPIErrorCode code = drvFRAMReadStatusRegister(drvFRAM_SPI1, &status);
        if(code==drvFRAM_OK){
            x++;
        }
    }
}
