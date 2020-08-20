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
#include "bl_control.h"
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

    blCommand_struct command = {0};
    blCommandInit(&command);

    blCommandChangeLongClickAction(&command, blTimerReloadHandler);
    blCommandChangeClickAction(&command, blTimerHandler);
    blCommandChangeSetLimitAction(&command, blTimerSetLimit);
    blCommandChangeClearAction(&command, blTimerClearLimit);

    while(1){
        ulHeartBeatRun(&heartBeat);
        blTimerRun(&timer);
        blCommandRun(&command);
    }
}
