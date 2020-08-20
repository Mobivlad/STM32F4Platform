/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    24 June 2020
  * @brief   Default main function.
  ******************************************************************************
*/

#include "bl_timerControler.h"
#include "ul_heart_beat.h"
#include "bl_timer.h"

#define BLINK_FREQUENCY_5_HZ 100

int main(void)
{
    drvSysClockInit();

    ulHeartBeatStruct heartBeat = {0};
    heartBeat.frequency = BLINK_FREQUENCY_5_HZ;
    ulHeartBeatInit(&heartBeat);

    blTimer_struct timer = {0};
    blTimerInit(&timer);

    blController_struct controller = {0};
    blControllerInit(&controller, &timer);

    while(1){
        ulHeartBeatRun(&heartBeat);
        blTimerRun(&timer);
        blControllerRun(&controller);
    }
}
