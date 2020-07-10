/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    24 June 2020
  * @brief   Default main function.
  ******************************************************************************
*/

#include "drv_fm25l16b.h"
#include "ul_heart_beat.h"

#define BLINK_FREQUENCY_5_HZ 100
uint8_t data[5] = {0x05, 0x04, 0x03, 0x02, 0x01};
drvFRAM_struct fram_struct1;
int main(void)
{
    /*drvSSDisplayInit();
    drvSSDisplaySetValue(125);
    for(;;);*/
    ulHeartBeatStruct heartBeat;
    heartBeat.frequency = BLINK_FREQUENCY_5_HZ;
    ulHeartBeatInit(&heartBeat);


    drvFRAMInit(&fram_struct1);
    drvFRAMReadData(&fram_struct1, 0x0000, data, 5);
    while(1){
        drvFRAMRun(&fram_struct1);
        ulHeartBeatRun(&heartBeat);
    }
}
