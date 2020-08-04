/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    24 June 2020
  * @brief   Default main function.
  ******************************************************************************
*/

#include "drv_com_port.h"
#include "ul_heart_beat.h"
#include "drv_sysclock.h"

#define BLINK_FREQUENCY_5_HZ 200
drvCOMPort_struct comPortStruct;
char array[255] = "Hello world!";

void uartRec() {
    drvCOMPortWriteString(&comPortStruct, "OK\r\n", NULL);
    memset(array, 0, sizeof(array));
    drvCOMPortReadString(&comPortStruct, array, 255, uartRec);
}
int main(void)
{
    drvSysClockInit();

    drvCOMPortInit(&comPortStruct, drvCOMPort1);
    drvCOMPortWriteString(&comPortStruct, array, NULL);
    drvCOMPortReadString(&comPortStruct, array, 255, uartRec);
    //drvCOMPortWriteString(&comPortStruct, array, uartRec);

    ulHeartBeatStruct heartBeat;
    heartBeat.frequency = BLINK_FREQUENCY_5_HZ;
    ulHeartBeatInit(&heartBeat);

    while(1){
        ulHeartBeatRun(&heartBeat);
    }
}
